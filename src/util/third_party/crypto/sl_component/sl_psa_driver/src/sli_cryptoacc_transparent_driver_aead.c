/***************************************************************************//**
 * @file
 * @brief Silicon Labs PSA Crypto Transparent Driver AEAD functions.
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 ******************************************************************************/

#include "em_device.h"

#if defined(CRYPTOACC_PRESENT)

#include "sli_cryptoacc_transparent_types.h"
#include "sli_cryptoacc_transparent_functions.h"
#include "sli_psa_driver_common.h"
#include "cryptoacc_management.h"
// Replace inclusion of psa/crypto_xxx.h with the new psa driver common
// interface header file when it becomes available.
#include "psa/crypto_platform.h"
#include "psa/crypto_sizes.h"
#include "psa/crypto_struct.h"
#include "sx_aes.h"
#include "sx_errors.h"
#include "cryptolib_types.h"

#include <string.h>

#if defined(PSA_WANT_ALG_CCM) || defined(PSA_WANT_ALG_GCM)

static psa_status_t check_aead_parameters(const psa_key_attributes_t *attributes,
                                          psa_algorithm_t alg,
                                          size_t nonce_length,
                                          size_t additional_data_length)
{
  size_t tag_length = PSA_AEAD_TAG_LENGTH(psa_get_key_type(attributes),
                                          psa_get_key_bits(attributes),
                                          alg);

  switch (PSA_ALG_AEAD_WITH_TAG_LENGTH(alg, 0)) {
#if defined(PSA_WANT_ALG_CCM)
    case PSA_ALG_AEAD_WITH_TAG_LENGTH(PSA_ALG_CCM, 0):
      if (psa_get_key_type(attributes) != PSA_KEY_TYPE_AES) {
        return PSA_ERROR_NOT_SUPPORTED;
      }
      if (tag_length == 0
          || tag_length == 2
          || tag_length > 16
          || tag_length % 2 != 0
          || nonce_length < 7
          || nonce_length > 13) {
        return PSA_ERROR_INVALID_ARGUMENT;
      }
      break;
#endif // PSA_WANT_ALG_CCM
#if defined(PSA_WANT_ALG_GCM)
    case PSA_ALG_AEAD_WITH_TAG_LENGTH(PSA_ALG_GCM, 0):
      if (psa_get_key_type(attributes) != PSA_KEY_TYPE_AES) {
        return PSA_ERROR_NOT_SUPPORTED;
      }
      // AD are limited to 2^64 bits, so 2^61 bytes.
      // We need not check if SIZE_MAX (max of size_t) is less than 2^61 (0x2000000000000000)
#if SIZE_MAX > 0x2000000000000000ull
      if (additional_data_length >> 61 != 0) {
        return PSA_ERROR_INVALID_ARGUMENT;
      }
#else // SIZE_MAX > 0x2000000000000000ull
      (void) additional_data_length;
#endif // SIZE_MAX > 0x2000000000000000ull
      if ((tag_length < 4) || (tag_length > 16)) {
        return PSA_ERROR_INVALID_ARGUMENT;
      }
      if (nonce_length == 0) {
        return PSA_ERROR_INVALID_ARGUMENT;
      }
#if !defined(SLI_PSA_SUPPORT_GCM_IV_CALCULATION)
      if (nonce_length != AES_IV_GCM_SIZE) {
        // Libcryptosoc only supports 12 bytes long IVs.
        return PSA_ERROR_NOT_SUPPORTED;
      }
#endif // ! SLI_PSA_SUPPORT_GCM_IV_CALCULATION
      break;
#endif // PSA_WANT_ALG_GCM
    default:
      return PSA_ERROR_NOT_SUPPORTED;
      break;
  }

#if !defined(PSA_WANT_ALG_GCM)
  (void) additional_data_length;
#endif // !PSA_WANT_ALG_GCM

  switch (psa_get_key_bits(attributes)) {
    case 128: // fallthrough
    case 192: // fallthrough
    case 256:
      break;
    default:
      return PSA_ERROR_INVALID_ARGUMENT;
  }

  return PSA_SUCCESS;
}

#endif // PSA_WANT_ALG_CCM || PSA_WANT_ALG_GCM

#if defined(SLI_PSA_SUPPORT_GCM_IV_CALCULATION) && defined(PSA_WANT_ALG_GCM)
/* Do GCM in software in case the IV isn't 12 bytes, since that's the only
 * thing the accelerator supports. */
static psa_status_t sli_cryptoacc_software_gcm(const uint8_t* keybuf,
                                               size_t key_length,
                                               const uint8_t* nonce,
                                               size_t nonce_length,
                                               const uint8_t* additional_data,
                                               size_t additional_data_length,
                                               const uint8_t* input,
                                               uint8_t* output,
                                               size_t plaintext_length,
                                               size_t tag_length,
                                               uint8_t* tag,
                                               bool encrypt_ndecrypt)
{
  // Step 1: calculate H = Ek(0)
  uint8_t Ek[16] = { 0 };
  uint32_t sx_ret = CRYPTOLIB_CRYPTO_ERR;
  block_t key = block_t_convert(keybuf, key_length);
  block_t data_in = block_t_convert(Ek, sizeof(Ek));
  block_t data_out = block_t_convert(Ek, sizeof(Ek));

  psa_status_t status = cryptoacc_management_acquire();
  if (status != PSA_SUCCESS) {
    return status;
  }
  sx_ret = sx_aes_ecb_encrypt((const block_t *)&key,
                              (const block_t *)&data_in,
                              &data_out);
  status = cryptoacc_management_release();
  if (sx_ret != CRYPTOLIB_SUCCESS
      || status != PSA_SUCCESS) {
    return PSA_ERROR_HARDWARE_FAILURE;
  }

  // Step 2: calculate IV = GHASH(H, {}, IV)
  uint8_t iv[16] = { 0 };
  uint64_t HL[16], HH[16];

  sli_psa_software_ghash_setup(Ek, HL, HH);

  for (size_t i = 0; i < nonce_length; i += 16) {
    // Mix in IV
    for (size_t j = 0; j < (nonce_length - i > 16 ? 16 : nonce_length - i); j++) {
      iv[j] ^= nonce[i + j];
    }
    // Update result
    sli_psa_software_ghash_multiply(HL, HH, iv, iv);
  }

  iv[12] ^= (nonce_length * 8) >> 24;
  iv[13] ^= (nonce_length * 8) >> 16;
  iv[14] ^= (nonce_length * 8) >>  8;
  iv[15] ^= (nonce_length * 8) >>  0;

  sli_psa_software_ghash_multiply(HL, HH, iv, iv);

  // Step 3: Calculate first counter block for tag generation
  uint8_t tagbuf[16] = { 0 };
  data_in = block_t_convert(iv, sizeof(iv));
  data_out = block_t_convert(tagbuf, sizeof(tagbuf));
  status = cryptoacc_management_acquire();
  if (status != PSA_SUCCESS) {
    return status;
  }
  sx_ret = sx_aes_ecb_encrypt((const block_t *)&key,
                              (const block_t *)&data_in,
                              &data_out);
  status = cryptoacc_management_release();
  if (sx_ret != CRYPTOLIB_SUCCESS
      || status != PSA_SUCCESS) {
    return PSA_ERROR_HARDWARE_FAILURE;
  }

  // If we're decrypting, mix in the to-be-checked tag value before transforming
  if (!encrypt_ndecrypt) {
    for (size_t i = 0; i < tag_length; i++) {
      tagbuf[i] ^= tag[i];
    }
  }

  // Step 4: increment IV (ripple increment)
  for (size_t i = 0; i < 16; i++) {
    iv[15 - i]++;

    if (iv[15 - i] != 0) {
      break;
    }
  }

  // Step 5: Accumulate additional data
  memset(Ek, 0, sizeof(Ek));
  for (size_t i = 0; i < additional_data_length; i += 16) {
    // Mix in additional data as much as we have
    for (size_t j = 0;
         j < (additional_data_length - i > 16 ? 16 : additional_data_length - i);
         j++) {
      Ek[j] ^= additional_data[i + j];
    }

    sli_psa_software_ghash_multiply(HL, HH, Ek, Ek);
  }

  // Step 6: If we're decrypting, accumulate the ciphertext before it gets transformed
  if (!encrypt_ndecrypt) {
    for (size_t i = 0; i < plaintext_length; i += 16) {
      // Mix in ciphertext
      for (size_t j = 0;
           j < (plaintext_length - i > 16 ? 16 : plaintext_length - i);
           j++) {
        Ek[j] ^= input[i + j];
      }

      sli_psa_software_ghash_multiply(HL, HH, Ek, Ek);
    }
  }

  // Step 7: transform data using AES-CTR
  if (plaintext_length) {
    data_in = block_t_convert(input, plaintext_length);
    data_out = block_t_convert(output, plaintext_length);
    block_t nonce_internal = block_t_convert(iv, sizeof(iv));

    status = cryptoacc_management_acquire();
    if (status != PSA_SUCCESS) {
      return status;
    }
    sx_ret = sx_aes_ctr_encrypt((const block_t *)&key,
                                (const block_t *)&data_in,
                                &data_out,
                                (const block_t *)&nonce_internal);

    status = cryptoacc_management_release();
    if (sx_ret != CRYPTOLIB_SUCCESS
        || status != PSA_SUCCESS) {
      return PSA_ERROR_HARDWARE_FAILURE;
    }
  }

  // Step 8: If we're encrypting, accumulate the ciphertext now
  if (encrypt_ndecrypt) {
    for (size_t i = 0; i < plaintext_length; i += 16) {
      // Mix in ciphertext
      for (size_t j = 0;
           j < (plaintext_length - i > 16 ? 16 : plaintext_length - i);
           j++) {
        Ek[j] ^= output[i + j];
      }

      sli_psa_software_ghash_multiply(HL, HH, Ek, Ek);
    }
  }

  // Step 9: add len(A) || len(C) block to tag calculation
  uint64_t bitlen = additional_data_length * 8;
  Ek[0]  ^= bitlen >> 56;
  Ek[1]  ^= bitlen >> 48;
  Ek[2]  ^= bitlen >> 40;
  Ek[3]  ^= bitlen >> 32;
  Ek[4]  ^= bitlen >> 24;
  Ek[5]  ^= bitlen >> 16;
  Ek[6]  ^= bitlen >>  8;
  Ek[7]  ^= bitlen >>  0;

  bitlen = plaintext_length * 8;
  Ek[8]  ^= bitlen >> 56;
  Ek[9]  ^= bitlen >> 48;
  Ek[10] ^= bitlen >> 40;
  Ek[11] ^= bitlen >> 32;
  Ek[12] ^= bitlen >> 24;
  Ek[13] ^= bitlen >> 16;
  Ek[14] ^= bitlen >>  8;
  Ek[15] ^= bitlen >>  0;

  sli_psa_software_ghash_multiply(HL, HH, Ek, Ek);

  // Step 10: calculate tag value
  for (size_t i = 0; i < tag_length; i++) {
    tagbuf[i] ^= Ek[i];
  }

  // Step 11: output tag for encrypt operation, check tag for decrypt
  if (encrypt_ndecrypt) {
    memcpy(tag, tagbuf, tag_length);
  } else {
    uint8_t accumulator = 0;
    for (size_t i = 0; i < tag_length; i++) {
      accumulator |= tagbuf[i];
    }
    if (accumulator != 0) {
      return PSA_ERROR_INVALID_SIGNATURE;
    }
  }

  return PSA_SUCCESS;
}
#endif // SLI_PSA_SUPPORT_GCM_IV_CALCULATION && PSA_WANT_ALG_GCM

psa_status_t sli_cryptoacc_transparent_aead_encrypt(const psa_key_attributes_t *attributes,
                                                    const uint8_t *key_buffer,
                                                    size_t key_buffer_size,
                                                    psa_algorithm_t alg,
                                                    const uint8_t *nonce,
                                                    size_t nonce_length,
                                                    const uint8_t *additional_data,
                                                    size_t additional_data_length,
                                                    const uint8_t *plaintext,
                                                    size_t plaintext_length,
                                                    uint8_t *ciphertext,
                                                    size_t ciphertext_size,
                                                    size_t *ciphertext_length)
{
#if defined(PSA_WANT_ALG_CCM) || defined(PSA_WANT_ALG_GCM)
  if (ciphertext_size <= plaintext_length) {
    return PSA_ERROR_BUFFER_TOO_SMALL;
  }

  size_t tag_length = 0;
  psa_status_t psa_status = sli_cryptoacc_transparent_aead_encrypt_tag(
    attributes, key_buffer, key_buffer_size, alg,
    nonce, nonce_length,
    additional_data, additional_data_length,
    plaintext, plaintext_length,
    ciphertext, plaintext_length, ciphertext_length,
    &ciphertext[plaintext_length], ciphertext_size - plaintext_length, &tag_length);

  if (psa_status == PSA_SUCCESS) {
    *ciphertext_length += tag_length;
  }

  return psa_status;

#else // PSA_WANT_ALG_CCM || PSA_WANT_ALG_GCM

  (void)attributes;
  (void)key_buffer;
  (void)key_buffer_size;
  (void)alg;
  (void)nonce;
  (void)nonce_length;
  (void)additional_data;
  (void)additional_data_length;
  (void)plaintext;
  (void)plaintext_length;
  (void)ciphertext;
  (void)ciphertext_size;
  (void)ciphertext_length;

  return PSA_ERROR_NOT_SUPPORTED;

#endif // PSA_WANT_ALG_CCM || PSA_WANT_ALG_GCM
}

psa_status_t sli_cryptoacc_transparent_aead_decrypt(const psa_key_attributes_t *attributes,
                                                    const uint8_t *key_buffer,
                                                    size_t key_buffer_size,
                                                    psa_algorithm_t alg,
                                                    const uint8_t *nonce,
                                                    size_t nonce_length,
                                                    const uint8_t *additional_data,
                                                    size_t additional_data_length,
                                                    const uint8_t *ciphertext,
                                                    size_t ciphertext_length,
                                                    uint8_t *plaintext,
                                                    size_t plaintext_size,
                                                    size_t *plaintext_length)
{
#if defined(PSA_WANT_ALG_CCM) || defined(PSA_WANT_ALG_GCM)
  if (attributes == NULL) {
    return PSA_ERROR_INVALID_ARGUMENT;
  }

  size_t tag_length = PSA_AEAD_TAG_LENGTH(psa_get_key_type(attributes),
                                          psa_get_key_bits(attributes),
                                          alg);

  if (ciphertext_length < tag_length
      || ciphertext == NULL
      || (tag_length > 16)) {
    return PSA_ERROR_INVALID_ARGUMENT;
  }

  // Split the tag in its own buffer to avoid potential issues when the
  // plaintext buffer extends into the tag area
  uint8_t check_tag[16];
  memcpy(check_tag, &ciphertext[ciphertext_length - tag_length], tag_length);

  return sli_cryptoacc_transparent_aead_decrypt_tag(
    attributes, key_buffer, key_buffer_size, alg,
    nonce, nonce_length,
    additional_data, additional_data_length,
    ciphertext, ciphertext_length - tag_length,
    check_tag, tag_length,
    plaintext, plaintext_size, plaintext_length);
#else // PSA_WANT_ALG_CCM || PSA_WANT_ALG_GCM

  (void)attributes;
  (void)key_buffer;
  (void)key_buffer_size;
  (void)alg;
  (void)nonce;
  (void)nonce_length;
  (void)additional_data;
  (void)additional_data_length;
  (void)plaintext;
  (void)plaintext_size;
  (void)plaintext_length;
  (void)ciphertext;
  (void)ciphertext_length;

  return PSA_ERROR_NOT_SUPPORTED;

#endif // PSA_WANT_ALG_CCM || PSA_WANT_ALG_GCM
}

psa_status_t sli_cryptoacc_transparent_aead_encrypt_tag(const psa_key_attributes_t *attributes,
                                                        const uint8_t *key_buffer,
                                                        size_t key_buffer_size,
                                                        psa_algorithm_t alg,
                                                        const uint8_t *nonce,
                                                        size_t nonce_length,
                                                        const uint8_t *additional_data,
                                                        size_t additional_data_length,
                                                        const uint8_t *plaintext,
                                                        size_t plaintext_length,
                                                        uint8_t *ciphertext,
                                                        size_t ciphertext_size,
                                                        size_t *ciphertext_length,
                                                        uint8_t *tag,
                                                        size_t tag_size,
                                                        size_t *tag_length)
{
#if defined(PSA_WANT_ALG_CCM) || defined(PSA_WANT_ALG_GCM)

  if (key_buffer == NULL
      || attributes == NULL
      || nonce == NULL
      || (additional_data == NULL && additional_data_length > 0)
      || (plaintext == NULL && plaintext_length > 0)
      || (plaintext_length > 0 && (ciphertext == NULL || ciphertext_size == 0))
      || ciphertext_length == NULL || tag_length == NULL
      || tag_size == 0 || tag == NULL) {
    return PSA_ERROR_INVALID_ARGUMENT;
  }

  size_t key_bits = psa_get_key_bits(attributes);
  *tag_length = PSA_AEAD_TAG_LENGTH(psa_get_key_type(attributes),
                                    psa_get_key_bits(attributes),
                                    alg);

  // Verify that the driver supports the given parameters.
  psa_status_t status = check_aead_parameters(attributes, alg, nonce_length, additional_data_length);
  if (status != PSA_SUCCESS) {
    return status;
  }

  // Check input-key size.
  if (key_buffer_size < PSA_BITS_TO_BYTES(key_bits)) {
    return PSA_ERROR_INVALID_ARGUMENT;
  }

  // Check sufficient output buffer size.
  if ((ciphertext_size < plaintext_length)
      || (tag_size < *tag_length)) {
    return PSA_ERROR_BUFFER_TOO_SMALL;
  }

  // Our drivers only support full or no overlap between input and output
  // buffers. So in the case of partial overlap, copy the input buffer into
  // the output buffer and process it in place as if the buffers fully
  // overlapped.
  if ((ciphertext > plaintext) && (ciphertext < (plaintext + plaintext_length))) {
    memmove(ciphertext, plaintext, plaintext_length);
    plaintext = ciphertext;
  }

  psa_status_t return_status = PSA_ERROR_CORRUPTION_DETECTED;
  uint32_t sx_ret = CRYPTOLIB_CRYPTO_ERR;

  block_t key = block_t_convert(key_buffer, PSA_BITS_TO_BYTES(key_bits));
  block_t aad_block = block_t_convert(additional_data, additional_data_length);
  block_t nonce_internal = block_t_convert(nonce, nonce_length);
  block_t data_in = block_t_convert(plaintext, plaintext_length);
  block_t data_out = block_t_convert(ciphertext, plaintext_length);
  block_t tag_block = block_t_convert(tag, *tag_length);

  switch (PSA_ALG_AEAD_WITH_TAG_LENGTH(alg, 0)) {
#if defined(PSA_WANT_ALG_CCM)
    case PSA_ALG_AEAD_WITH_TAG_LENGTH(PSA_ALG_CCM, 0):

      // Check length of plaintext.
    {
      unsigned char q = 16 - 1 - (unsigned char) nonce_length;
      if (q < sizeof(plaintext_length)
          && plaintext_length >= (1UL << (q * 8))) {
        return PSA_ERROR_INVALID_ARGUMENT;
      }
    }

      status = cryptoacc_management_acquire();
      if (status != PSA_SUCCESS) {
        return status;
      }
      sx_ret = sx_aes_ccm_encrypt((const block_t *)&key,
                                  (const block_t *)&data_in,
                                  &data_out,
                                  (const block_t *)&nonce_internal,
                                  &tag_block,
                                  (const block_t *)&aad_block);
      status = cryptoacc_management_release();
      if (sx_ret != CRYPTOLIB_SUCCESS
          || status != PSA_SUCCESS) {
        return PSA_ERROR_HARDWARE_FAILURE;
      }

      return_status = PSA_SUCCESS;
      break;
#endif // PSA_WANT_ALG_CCM
#if defined(PSA_WANT_ALG_GCM)
    case PSA_ALG_AEAD_WITH_TAG_LENGTH(PSA_ALG_GCM, 0):
      if (nonce_length == AES_IV_GCM_SIZE) {
        uint8_t tagbuf[16];
        tag_block = block_t_convert(tagbuf, sizeof(tagbuf));

        status = cryptoacc_management_acquire();
        if (status != PSA_SUCCESS) {
          return status;
        }
        sx_ret = sx_aes_gcm_encrypt((const block_t *)&key,
                                    (const block_t *)&data_in,
                                    &data_out,
                                    (const block_t *)&nonce_internal,
                                    &tag_block,
                                    (const block_t *)&aad_block);
        status = cryptoacc_management_release();
        if (sx_ret != CRYPTOLIB_SUCCESS
            || status != PSA_SUCCESS) {
          return PSA_ERROR_HARDWARE_FAILURE;
        }
        // Copy only requested part of computed tag to user output buffer.
        memcpy(tag, tagbuf, *tag_length);
        return_status = PSA_SUCCESS;
      }
#if defined(SLI_PSA_SUPPORT_GCM_IV_CALCULATION)
      else {
        return_status = sli_cryptoacc_software_gcm(key_buffer, PSA_BITS_TO_BYTES(key_bits),
                                                   nonce, nonce_length,
                                                   additional_data, additional_data_length,
                                                   plaintext,
                                                   ciphertext,
                                                   plaintext_length,
                                                   *tag_length,
                                                   tag,
                                                   true);
      }
#else // SLI_PSA_SUPPORT_GCM_IV_CALCULATION
      else {
        return_status = PSA_ERROR_NOT_SUPPORTED;
      }
#endif // SLI_PSA_SUPPORT_GCM_IV_CALCULATION
      break;
#endif // PSA_WANT_ALG_GCM
  }

  if (return_status == PSA_SUCCESS) {
    *ciphertext_length = plaintext_length;
  } else {
    *ciphertext_length = 0;
    *tag_length = 0;
  }

  return return_status;

#else // PSA_WANT_ALG_CCM || PSA_WANT_ALG_GCM

  (void)attributes;
  (void)key_buffer;
  (void)key_buffer_size;
  (void)alg;
  (void)nonce;
  (void)nonce_length;
  (void)additional_data;
  (void)additional_data_length;
  (void)plaintext;
  (void)plaintext_length;
  (void)ciphertext;
  (void)ciphertext_size;
  (void)ciphertext_length;
  (void)tag;
  (void)tag_size;
  (void)tag_length;

  return PSA_ERROR_NOT_SUPPORTED;

#endif // PSA_WANT_ALG_CCM || PSA_WANT_ALG_GCM
}

psa_status_t sli_cryptoacc_transparent_aead_decrypt_tag(const psa_key_attributes_t *attributes,
                                                        const uint8_t *key_buffer,
                                                        size_t key_buffer_size,
                                                        psa_algorithm_t alg,
                                                        const uint8_t *nonce,
                                                        size_t nonce_length,
                                                        const uint8_t *additional_data,
                                                        size_t additional_data_length,
                                                        const uint8_t *ciphertext,
                                                        size_t ciphertext_length,
                                                        const uint8_t* tag,
                                                        size_t tag_length,
                                                        uint8_t *plaintext,
                                                        size_t plaintext_size,
                                                        size_t *plaintext_length)
{
#if defined(PSA_WANT_ALG_CCM) || defined(PSA_WANT_ALG_GCM)
  if (attributes == NULL
      || key_buffer == NULL
      || nonce == NULL
      || (additional_data == NULL && additional_data_length > 0)
      || (ciphertext == NULL && ciphertext_length > 0)
      || (plaintext == NULL && plaintext_size > 0)
      || plaintext_length == NULL
      || tag == NULL) {
    return PSA_ERROR_INVALID_ARGUMENT;
  }

  // Verify that the driver supports the given parameters.
  size_t key_bits = psa_get_key_bits(attributes);
  psa_status_t status = check_aead_parameters(attributes, alg, nonce_length, additional_data_length);
  if (status != PSA_SUCCESS) {
    return status;
  }

  // Check input-key size.
  if (key_buffer_size < PSA_BITS_TO_BYTES(key_bits)) {
    return PSA_ERROR_INVALID_ARGUMENT;
  }

  // Check sufficient output buffer size.
  if (plaintext_size < ciphertext_length) {
    return PSA_ERROR_BUFFER_TOO_SMALL;
  }

  // Our drivers only support full or no overlap between input and output
  // buffers. So in the case of partial overlap, copy the input buffer into
  // the output buffer and process it in place as if the buffers fully
  // overlapped.
  if ((plaintext > ciphertext) && (plaintext < (ciphertext + ciphertext_length))) {
    memmove(plaintext, ciphertext, ciphertext_length);
    ciphertext = plaintext;
  }

  psa_status_t return_status = PSA_ERROR_CORRUPTION_DETECTED;
  uint32_t sx_ret = CRYPTOLIB_CRYPTO_ERR;
  block_t key = NULL_blk;
  block_t aad_block = NULL_blk;
  block_t tag_block = NULL_blk;
  block_t nonce_internal = NULL_blk;
  block_t data_in = NULL_blk;
  block_t data_out = NULL_blk;
  switch (PSA_ALG_AEAD_WITH_TAG_LENGTH(alg, 0)) {
#if defined(PSA_WANT_ALG_CCM)
    case PSA_ALG_AEAD_WITH_TAG_LENGTH(PSA_ALG_CCM, 0):

    {
      // Check length of ciphertext.
      unsigned char q = 16 - 1 - (unsigned char) nonce_length;
      if (q < sizeof(ciphertext_length)
          && ciphertext_length >= (1UL << (q * 8))) {
        return PSA_ERROR_INVALID_ARGUMENT;
      }
    }

      key = block_t_convert(key_buffer, PSA_BITS_TO_BYTES(key_bits));
      aad_block = block_t_convert(additional_data, additional_data_length);
      tag_block = block_t_convert(tag, tag_length);
      nonce_internal = block_t_convert(nonce, nonce_length);
      data_in = block_t_convert(ciphertext, ciphertext_length);
      data_out = block_t_convert(plaintext, ciphertext_length);

      status = cryptoacc_management_acquire();
      if (status != PSA_SUCCESS) {
        return status;
      }
      sx_ret = sx_aes_ccm_decrypt_verify((const block_t *)&key,
                                         (const block_t *)&data_in,
                                         &data_out,
                                         (const block_t *)&nonce_internal,
                                         &tag_block,
                                         (const block_t *)&aad_block);
      status = cryptoacc_management_release();
      if (sx_ret == CRYPTOLIB_INVALID_SIGN_ERR) {
        return_status = PSA_ERROR_INVALID_SIGNATURE;
      } else if (sx_ret != CRYPTOLIB_SUCCESS || status != PSA_SUCCESS) {
        return_status = PSA_ERROR_HARDWARE_FAILURE;
      } else {
        *plaintext_length = ciphertext_length;
        return_status = PSA_SUCCESS;
      }
      break;
#endif // PSA_WANT_ALG_CCM
#if defined(PSA_WANT_ALG_GCM)
    case PSA_ALG_AEAD_WITH_TAG_LENGTH(PSA_ALG_GCM, 0):
    {
      uint8_t tagbuf[16];
      uint32_t diff = 0;

      if (nonce_length == AES_IV_GCM_SIZE) {
        key = block_t_convert(key_buffer, PSA_BITS_TO_BYTES(key_bits));
        aad_block = block_t_convert(additional_data, additional_data_length);
        tag_block = block_t_convert(tagbuf, sizeof(tagbuf));
        nonce_internal = block_t_convert(nonce, nonce_length);
        data_in = block_t_convert(ciphertext, ciphertext_length);
        data_out = block_t_convert(plaintext, ciphertext_length);

        status = cryptoacc_management_acquire();
        if (status != PSA_SUCCESS) {
          return status;
        }
        sx_ret = sx_aes_gcm_decrypt((const block_t *)&key,
                                    (const block_t *)&data_in,
                                    &data_out,
                                    (const block_t *)&nonce_internal,
                                    &tag_block,
                                    (const block_t *)&aad_block);
        status = cryptoacc_management_release();
        if (sx_ret != CRYPTOLIB_SUCCESS || status != PSA_SUCCESS) {
          return_status = PSA_ERROR_HARDWARE_FAILURE;
        } else {
          // Check that the provided tag equals the calculated one
          // (in constant time). Note that the tag returned by ccm_auth_crypt
          // is encrypted, so we don't have to decrypt the tag.
          diff = sli_psa_safer_memcmp(tag, tagbuf, tag_length);
          sli_psa_zeroize(tagbuf, tag_length);

          if (diff != 0) {
            return_status = PSA_ERROR_INVALID_SIGNATURE;
          } else {
            *plaintext_length = ciphertext_length;
            return_status = PSA_SUCCESS;
          }

          break;
        }
      }
#if defined(SLI_PSA_SUPPORT_GCM_IV_CALCULATION)
      else {
        return_status = sli_cryptoacc_software_gcm(key_buffer, PSA_BITS_TO_BYTES(key_bits),
                                                   nonce, nonce_length,
                                                   additional_data, additional_data_length,
                                                   ciphertext,
                                                   plaintext,
                                                   ciphertext_length,
                                                   tag_length,
                                                   (uint8_t*)tag,
                                                   false);
        if (return_status == PSA_SUCCESS) {
          *plaintext_length = ciphertext_length;
        }
      }
#else // SLI_PSA_SUPPORT_GCM_IV_CALCULATION
      else {
        return_status = PSA_ERROR_NOT_SUPPORTED;
      }
#endif // SLI_PSA_SUPPORT_GCM_IV_CALCULATION
      break;
    }
#endif // PSA_WANT_ALG_GCM
    default:
      return PSA_ERROR_NOT_SUPPORTED;
  }

  if (return_status != PSA_SUCCESS) {
    *plaintext_length = 0;
    sli_psa_zeroize(plaintext, plaintext_size);
  }

  return return_status;

#else // PSA_WANT_ALG_CCM || PSA_WANT_ALG_GCM

  (void)attributes;
  (void)key_buffer;
  (void)key_buffer_size;
  (void)alg;
  (void)nonce;
  (void)nonce_length;
  (void)additional_data;
  (void)additional_data_length;
  (void)plaintext;
  (void)plaintext_size;
  (void)plaintext_length;
  (void)ciphertext;
  (void)ciphertext_length;
  (void)tag;
  (void)tag_length;

  return PSA_ERROR_NOT_SUPPORTED;

#endif // PSA_WANT_ALG_CCM || PSA_WANT_ALG_GCM
}

#if defined(PSA_CRYPTO_AEAD_MULTIPART_SUPPORTED)

psa_status_t sli_cryptoacc_transparent_aead_encrypt_setup(sli_cryptoacc_transparent_aead_operation_t *operation,
                                                          const psa_key_attributes_t *attributes,
                                                          const uint8_t *key_buffer,
                                                          size_t key_buffer_size,
                                                          psa_algorithm_t alg)
{
  (void)operation;
  (void)attributes;
  (void)key_buffer;
  (void)key_buffer_size;
  (void)alg;
  return PSA_ERROR_NOT_SUPPORTED;
}

psa_status_t sli_cryptoacc_transparent_aead_decrypt_setup(sli_cryptoacc_transparent_aead_operation_t *operation,
                                                          const psa_key_attributes_t *attributes,
                                                          const uint8_t *key_buffer,
                                                          size_t key_buffer_size,
                                                          psa_algorithm_t alg)
{
  (void)operation;
  (void)attributes;
  (void)key_buffer;
  (void)key_buffer_size;
  (void)alg;
  return PSA_ERROR_NOT_SUPPORTED;
}

psa_status_t sli_cryptoacc_transparent_aead_generate_nonce(sli_cryptoacc_transparent_aead_operation_t *operation,
                                                           uint8_t *nonce,
                                                           size_t nonce_size,
                                                           size_t *nonce_length)
{
  (void)operation;
  (void)nonce;
  (void)nonce_size;
  (void)nonce_length;
  return PSA_ERROR_NOT_SUPPORTED;
}

psa_status_t sli_cryptoacc_transparent_aead_set_nonce(sli_cryptoacc_transparent_aead_operation_t *operation,
                                                      const uint8_t *nonce,
                                                      size_t nonce_size)
{
  (void)operation;
  (void)nonce;
  (void)nonce_size;
  return PSA_ERROR_NOT_SUPPORTED;
}

psa_status_t sli_cryptoacc_transparent_aead_set_lengths(sli_cryptoacc_transparent_aead_operation_t *operation,
                                                        size_t ad_length,
                                                        size_t plaintext_length)
{
  (void)operation;
  (void)ad_length;
  (void)plaintext_length;
  return PSA_ERROR_NOT_SUPPORTED;
}

psa_status_t sli_cryptoacc_transparent_aead_update_ad(sli_cryptoacc_transparent_aead_operation_t *operation,
                                                      const uint8_t *input,
                                                      size_t input_length)
{
  (void)operation;
  (void)input;
  (void)input_length;
  return PSA_ERROR_NOT_SUPPORTED;
}

psa_status_t sli_cryptoacc_transparent_aead_update(sli_cryptoacc_transparent_aead_operation_t *operation,
                                                   const uint8_t *input,
                                                   size_t input_length,
                                                   uint8_t *output,
                                                   size_t output_size,
                                                   size_t *output_length)
{
  (void)operation;
  (void)input;
  (void)input_length;
  (void)output;
  (void)output_size;
  (void)output_length;
  return PSA_ERROR_NOT_SUPPORTED;
}

psa_status_t sli_cryptoacc_transparent_aead_finish(sli_cryptoacc_transparent_aead_operation_t *operation,
                                                   uint8_t *ciphertext,
                                                   size_t ciphertext_size,
                                                   size_t *ciphertext_length,
                                                   uint8_t *tag,
                                                   size_t tag_size,
                                                   size_t *tag_length)
{
  (void)operation;
  (void)ciphertext;
  (void)ciphertext_size;
  (void)ciphertext_length;
  (void)tag;
  (void)tag_size;
  (void)tag_length;
  return PSA_ERROR_NOT_SUPPORTED;
}

psa_status_t sli_cryptoacc_transparent_aead_verify(sli_cryptoacc_transparent_aead_operation_t *operation,
                                                   uint8_t *plaintext,
                                                   size_t plaintext_size,
                                                   size_t *plaintext_length,
                                                   const uint8_t *tag,
                                                   size_t tag_length)
{
  (void)operation;
  (void)plaintext;
  (void)plaintext_size;
  (void)plaintext_length;
  (void)tag;
  (void)tag_length;
  return PSA_ERROR_NOT_SUPPORTED;
}

#endif // defined(PSA_CRYPTO_AEAD_MULTIPART_SUPPORTED)

#endif // defined(SEMAILBOX_PRESENT)
