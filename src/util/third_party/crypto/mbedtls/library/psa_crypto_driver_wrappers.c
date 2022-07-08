/*
 *  Functions to delegate cryptographic operations to an available
 *  and appropriate accelerator.
 *  Warning: This file will be auto-generated in the future.
 */
/*  Copyright The Mbed TLS Contributors
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include "psa_crypto_aead.h"
#include "psa_crypto_cipher.h"
#include "psa_crypto_core.h"
#include "psa_crypto_driver_wrappers.h"
#include "psa_crypto_hash.h"
#include "psa_crypto_mac.h"

#include "mbedtls/platform.h"

#if defined(MBEDTLS_PSA_CRYPTO_C)

#if defined(MBEDTLS_PSA_CRYPTO_DRIVERS)

/* Include test driver definition when running tests */
#if defined(PSA_CRYPTO_DRIVER_TEST)
#ifndef PSA_CRYPTO_DRIVER_PRESENT
#define PSA_CRYPTO_DRIVER_PRESENT
#endif
#ifndef PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT
#define PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT
#endif
#include "test/drivers/test_driver.h"
#endif /* PSA_CRYPTO_DRIVER_TEST */

/* Repeat above block for each JSON-declared driver during autogeneration */

#include "em_device.h"
#include <string.h>
#if defined(SEMAILBOX_PRESENT)
#ifndef PSA_CRYPTO_DRIVER_PRESENT
#define PSA_CRYPTO_DRIVER_PRESENT
#endif
#ifndef PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT
#define PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT
#endif
#include "sli_se_transparent_types.h"
#include "sli_se_transparent_functions.h"
#endif /* SEMAILBOX_PRESENT */
#if defined(SEMAILBOX_PRESENT) && \
  ( (_SILICON_LABS_SECURITY_FEATURE == _SILICON_LABS_SECURITY_FEATURE_VAULT) || \
    defined(MBEDTLS_PSA_CRYPTO_BUILTIN_KEYS) )
#ifndef PSA_CRYPTO_DRIVER_PRESENT
#define PSA_CRYPTO_DRIVER_PRESENT
#endif
#ifndef PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT
#define PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT
#endif
#include "sli_se_opaque_types.h"
#include "sli_se_opaque_functions.h"
#endif /* SEMAILBOX_PRESENT  && (_SILICON_LABS_SECURITY_FEATURE == _SILICON_LABS_SECURITY_FEATURE_VAULT) */
#if defined(CRYPTOACC_PRESENT)
#ifndef PSA_CRYPTO_DRIVER_PRESENT
#define PSA_CRYPTO_DRIVER_PRESENT
#endif
#ifndef PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT
#define PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT
#endif
#include "sli_cryptoacc_transparent_types.h"
#include "sli_cryptoacc_transparent_functions.h"
#endif /* CRYPTOACC_PRESENT */
#if defined(CRYPTO_PRESENT)
#ifndef PSA_CRYPTO_DRIVER_PRESENT
#define PSA_CRYPTO_DRIVER_PRESENT
#endif
#ifndef PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT
#define PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT
#endif
#include "sli_crypto_transparent_types.h"
#include "sli_crypto_transparent_functions.h"
#endif /* CRYPTO_PRESENT */

#endif /* MBEDTLS_PSA_CRYPTO_DRIVERS */

/* Auto-generated values depending on which drivers are registered.
 * ID 0 is reserved for unallocated operations.
 * ID 1 is reserved for the Mbed TLS software driver. */
#define PSA_CRYPTO_MBED_TLS_DRIVER_ID (1)

#if defined(PSA_CRYPTO_DRIVER_TEST)
#define PSA_CRYPTO_TRANSPARENT_TEST_DRIVER_ID (2)
#define PSA_CRYPTO_OPAQUE_TEST_DRIVER_ID (3)
#endif /* PSA_CRYPTO_DRIVER_TEST */

/* Auto-generated values depending on which drivers are registered. ID 0 is
 * reserved for unallocated operations, 1 for software fallback, and 2 and 3
 * for test drivers.
 * NOTE: these values have no correlation with the driver 'location' as defined
 * by the accelerator specification document. The driver IDs are runtime values
 * to indicate which driver is handling a multi-part operation that is in
 * process. They can thus change freely without consequence for stored keys. */
#if defined(PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT)
/* Other drivers get unique IDs assigned in sequence */
#define SLI_SE_TRANSPARENT_DRIVER_ID (4)
#define SLI_SE_OPAQUE_DRIVER_ID (5)
#define SLI_CRYPTOACC_TRANSPARENT_DRIVER_ID (6)
#define SLI_CRYPTO_TRANSPARENT_DRIVER_ID (7)
#endif /* PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT */

/* Support the 'old' SE interface when asked to */
#if defined(MBEDTLS_PSA_CRYPTO_SE_C)
/* PSA_CRYPTO_DRIVER_PRESENT is defined when either a new-style or old-style
 * SE driver is present, to avoid unused argument errors at compile time. */
#ifndef PSA_CRYPTO_DRIVER_PRESENT
#define PSA_CRYPTO_DRIVER_PRESENT
#endif
#include "psa_crypto_se.h"
#endif

/* Start delegation functions */

/*
 * Init function
 */
psa_status_t psa_driver_wrapper_init( void )
{
#if defined(PSA_CRYPTO_DRIVER_PRESENT)
    psa_status_t status;
#if defined(MBEDTLS_PSA_CRYPTO_SE_C)
    status = psa_init_all_se_drivers( );
    if( status != PSA_SUCCESS )
        return status;
    /* continue to driver init calls */
#endif /* MBEDTLS_PSA_CRYPTO_SE_C */
#if defined(PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT)
    status = PSA_SUCCESS;
#if defined(SEMAILBOX_PRESENT)
    status = sli_se_transparent_driver_init();
    if( status != PSA_SUCCESS )
        return status;
#if (_SILICON_LABS_SECURITY_FEATURE == _SILICON_LABS_SECURITY_FEATURE_VAULT) || defined(MBEDTLS_PSA_CRYPTO_BUILTIN_KEYS)
    status = sli_se_opaque_driver_init();
#endif /* Vault */
#endif /* SEMAILBOX_PRESENT */
#if defined(CRYPTOACC_PRESENT)
    status = sli_cryptoacc_transparent_driver_init();
#endif /* CRYPTOACC_PRESENT */
#if defined(CRYPTO_PRESENT)
    status = sli_crypto_transparent_driver_init();
#endif /* CRYPTO_PRESENT */
    return( status );
#endif /* PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT */
#else
    return( PSA_SUCCESS );
#endif /* PSA_CRYPTO_DRIVER_PRESENT */
}

/*
 * Signature functions
 */
psa_status_t psa_driver_wrapper_sign_message(
    const psa_key_attributes_t *attributes,
    const uint8_t *key_buffer,
    size_t key_buffer_size,
    psa_algorithm_t alg,
    const uint8_t *input,
    size_t input_length,
    uint8_t *signature,
    size_t signature_size,
    size_t *signature_length )
{
    psa_status_t status = PSA_ERROR_CORRUPTION_DETECTED;
    psa_key_location_t location =
        PSA_KEY_LIFETIME_GET_LOCATION( attributes->core.lifetime );

    switch( location )
    {
        case PSA_KEY_LOCATION_LOCAL_STORAGE:
            /* Key is stored in the slot in export representation, so
             * cycle through all known transparent accelerators */
#if defined(PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT)
#if defined(PSA_CRYPTO_DRIVER_TEST)
            status = mbedtls_test_transparent_signature_sign_message(
                        attributes,
                        key_buffer,
                        key_buffer_size,
                        alg,
                        input,
                        input_length,
                        signature,
                        signature_size,
                        signature_length );
            /* Declared with fallback == true */
            if( status != PSA_ERROR_NOT_SUPPORTED )
                return( status );
#else /* PSA_CRYPTO_DRIVER_TEST */
#if defined(SEMAILBOX_PRESENT)
            status = sli_se_transparent_sign_message( attributes,
                                                      key_buffer,
                                                      key_buffer_size,
                                                      alg,
                                                      input,
                                                      input_length,
                                                      signature,
                                                      signature_size,
                                                      signature_length );
            /* Declared with fallback == true */
            if( status != PSA_ERROR_NOT_SUPPORTED )
                return( status );
#endif /* SEMAILBOX_PRESENT */
#endif /* PSA_CRYPTO_DRIVER_TEST */
#endif /* PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT */
            break;

        /* Add cases for opaque driver here */
#if defined(PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT)
#if defined(PSA_CRYPTO_DRIVER_TEST)
        case PSA_CRYPTO_TEST_DRIVER_LOCATION:
            status = mbedtls_test_opaque_signature_sign_message(
                        attributes,
                        key_buffer,
                        key_buffer_size,
                        alg,
                        input,
                        input_length,
                        signature,
                        signature_size,
                        signature_length );
            if( status != PSA_ERROR_NOT_SUPPORTED )
                return( status );
            break;
#endif /* PSA_CRYPTO_DRIVER_TEST */
#if defined(SEMAILBOX_PRESENT) && \
  ( (_SILICON_LABS_SECURITY_FEATURE == _SILICON_LABS_SECURITY_FEATURE_VAULT) || \
    defined(MBEDTLS_PSA_CRYPTO_BUILTIN_KEYS) )
        case PSA_KEY_LOCATION_SLI_SE_OPAQUE:
            status = sli_se_opaque_sign_message( attributes,
                                                 key_buffer,
                                                 key_buffer_size,
                                                 alg,
                                                 input,
                                                 input_length,
                                                 signature,
                                                 signature_size,
                                                 signature_length );
            /* No fallback for opaque */
            return( status );
#endif /* VAULT */
#endif /* PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT */
        default:
            /* Key is declared with a lifetime not known to us */
            (void)status;
            break;
    }

    return( psa_sign_message_builtin( attributes,
                                      key_buffer,
                                      key_buffer_size,
                                      alg,
                                      input,
                                      input_length,
                                      signature,
                                      signature_size,
                                      signature_length ) );
}

psa_status_t psa_driver_wrapper_verify_message(
    const psa_key_attributes_t *attributes,
    const uint8_t *key_buffer,
    size_t key_buffer_size,
    psa_algorithm_t alg,
    const uint8_t *input,
    size_t input_length,
    const uint8_t *signature,
    size_t signature_length )
{
    psa_status_t status = PSA_ERROR_CORRUPTION_DETECTED;
    psa_key_location_t location =
        PSA_KEY_LIFETIME_GET_LOCATION( attributes->core.lifetime );

    switch( location )
    {
        case PSA_KEY_LOCATION_LOCAL_STORAGE:
            /* Key is stored in the slot in export representation, so
             * cycle through all known transparent accelerators */
#if defined(PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT)
#if defined(PSA_CRYPTO_DRIVER_TEST)
            status = mbedtls_test_transparent_signature_verify_message(
                        attributes,
                        key_buffer,
                        key_buffer_size,
                        alg,
                        input,
                        input_length,
                        signature,
                        signature_length );
            /* Declared with fallback == true */
            if( status != PSA_ERROR_NOT_SUPPORTED )
                return( status );
#else /* PSA_CRYPTO_DRIVER_TEST */
#if defined(SEMAILBOX_PRESENT)
            status = sli_se_transparent_verify_message( attributes,
                                                        key_buffer,
                                                        key_buffer_size,
                                                        alg,
                                                        input,
                                                        input_length,
                                                        signature,
                                                        signature_length );
            /* Declared with fallback == true */
            if( status != PSA_ERROR_NOT_SUPPORTED )
                return( status );
#endif /* SEMAILBOX_PRESENT */
#endif /* PSA_CRYPTO_DRIVER_TEST */
#endif /* PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT */
            break;

        /* Add cases for opaque driver here */
#if defined(PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT)
#if defined(PSA_CRYPTO_DRIVER_TEST)
        case PSA_CRYPTO_TEST_DRIVER_LOCATION:
            return( mbedtls_test_opaque_signature_verify_message(
                        attributes,
                        key_buffer,
                        key_buffer_size,
                        alg,
                        input,
                        input_length,
                        signature,
                        signature_length ) );
#endif /* PSA_CRYPTO_DRIVER_TEST */
#if defined(SEMAILBOX_PRESENT) && \
  ( (_SILICON_LABS_SECURITY_FEATURE == _SILICON_LABS_SECURITY_FEATURE_VAULT) || \
    defined(MBEDTLS_PSA_CRYPTO_BUILTIN_KEYS) )
        case PSA_KEY_LOCATION_SLI_SE_OPAQUE:
            status = sli_se_opaque_verify_message( attributes,
                                                   key_buffer,
                                                   key_buffer_size,
                                                   alg,
                                                   input,
                                                   input_length,
                                                   signature,
                                                   signature_length );
            /* No fallback for opaque */
            return( status );
#endif /* VAULT */
#endif /* PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT */
        default:
            /* Key is declared with a lifetime not known to us */
            (void)status;
            break;
    }

    return( psa_verify_message_builtin( attributes,
                                        key_buffer,
                                        key_buffer_size,
                                        alg,
                                        input,
                                        input_length,
                                        signature,
                                        signature_length ) );
}

psa_status_t psa_driver_wrapper_sign_hash(
    const psa_key_attributes_t *attributes,
    const uint8_t *key_buffer, size_t key_buffer_size,
    psa_algorithm_t alg, const uint8_t *hash, size_t hash_length,
    uint8_t *signature, size_t signature_size, size_t *signature_length )
{
    /* Try dynamically-registered SE interface first */
#if defined(MBEDTLS_PSA_CRYPTO_SE_C)
    const psa_drv_se_t *drv;
    psa_drv_se_context_t *drv_context;

    if( psa_get_se_driver( attributes->core.lifetime, &drv, &drv_context ) )
    {
        if( drv->asymmetric == NULL ||
            drv->asymmetric->p_sign == NULL )
        {
            /* Key is defined in SE, but we have no way to exercise it */
            return( PSA_ERROR_NOT_SUPPORTED );
        }
        return( drv->asymmetric->p_sign(
                    drv_context, *( (psa_key_slot_number_t *)key_buffer ),
                    alg, hash, hash_length,
                    signature, signature_size, signature_length ) );
    }
#endif /* PSA_CRYPTO_SE_C */

    psa_status_t status = PSA_ERROR_CORRUPTION_DETECTED;
    psa_key_location_t location =
        PSA_KEY_LIFETIME_GET_LOCATION( attributes->core.lifetime );

    switch( location )
    {
        case PSA_KEY_LOCATION_LOCAL_STORAGE:
            /* Key is stored in the slot in export representation, so
             * cycle through all known transparent accelerators */
#if defined(PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT)
#if defined(PSA_CRYPTO_DRIVER_TEST)
            status = mbedtls_test_transparent_signature_sign_hash( attributes,
                                                           key_buffer,
                                                           key_buffer_size,
                                                           alg,
                                                           hash,
                                                           hash_length,
                                                           signature,
                                                           signature_size,
                                                           signature_length );
            /* Declared with fallback == true */
            if( status != PSA_ERROR_NOT_SUPPORTED )
                return( status );
#endif /* PSA_CRYPTO_DRIVER_TEST */
#if defined(SEMAILBOX_PRESENT)
            status = sli_se_transparent_sign_hash( attributes,
                                                   key_buffer,
                                                   key_buffer_size,
                                                   alg,
                                                   hash,
                                                   hash_length,
                                                   signature,
                                                   signature_size,
                                                   signature_length );
            /* Declared with fallback == true */
            if( status != PSA_ERROR_NOT_SUPPORTED )
                return( status );
#elif defined(CRYPTOACC_PRESENT)
            status = sli_cryptoacc_transparent_sign_hash( attributes,
                                                          key_buffer,
                                                          key_buffer_size,
                                                          alg,
                                                          hash,
                                                          hash_length,
                                                          signature,
                                                          signature_size,
                                                          signature_length );
            /* Declared with fallback == true */
            if( status != PSA_ERROR_NOT_SUPPORTED )
                return( status );
#endif /* SEMAILBOX_PRESENT CRYPTOACC_PRESENT */
#endif /* PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT */
            /* Fell through, meaning no accelerator supports this operation */
            return( psa_sign_hash_builtin( attributes,
                                           key_buffer,
                                           key_buffer_size,
                                           alg,
                                           hash,
                                           hash_length,
                                           signature,
                                           signature_size,
                                           signature_length ) );

        /* Add cases for opaque driver here */
#if defined(PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT)
#if defined(PSA_CRYPTO_DRIVER_TEST)
        case PSA_CRYPTO_TEST_DRIVER_LOCATION:
            return( mbedtls_test_opaque_signature_sign_hash( attributes,
                                                             key_buffer,
                                                             key_buffer_size,
                                                             alg,
                                                             hash,
                                                             hash_length,
                                                             signature,
                                                             signature_size,
                                                             signature_length ) );
#endif /* PSA_CRYPTO_DRIVER_TEST */
#if defined(SEMAILBOX_PRESENT) && \
  ( (_SILICON_LABS_SECURITY_FEATURE == _SILICON_LABS_SECURITY_FEATURE_VAULT) || \
    defined(MBEDTLS_PSA_CRYPTO_BUILTIN_KEYS) )
        case PSA_KEY_LOCATION_SLI_SE_OPAQUE:
            status = sli_se_opaque_sign_hash( attributes,
                                              key_buffer,
                                              key_buffer_size,
                                              alg,
                                              hash,
                                              hash_length,
                                              signature,
                                              signature_size,
                                              signature_length );
            /* No fallback for opaque */
            return( status );
#endif /* VAULT */
#endif /* PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT */
        default:
            /* Key is declared with a lifetime not known to us */
            (void)status;
            return( PSA_ERROR_INVALID_ARGUMENT );
    }
}

psa_status_t psa_driver_wrapper_verify_hash(
    const psa_key_attributes_t *attributes,
    const uint8_t *key_buffer, size_t key_buffer_size,
    psa_algorithm_t alg, const uint8_t *hash, size_t hash_length,
    const uint8_t *signature, size_t signature_length )
{
    /* Try dynamically-registered SE interface first */
#if defined(MBEDTLS_PSA_CRYPTO_SE_C)
    const psa_drv_se_t *drv;
    psa_drv_se_context_t *drv_context;

    if( psa_get_se_driver( attributes->core.lifetime, &drv, &drv_context ) )
    {
        if( drv->asymmetric == NULL ||
            drv->asymmetric->p_verify == NULL )
        {
            /* Key is defined in SE, but we have no way to exercise it */
            return( PSA_ERROR_NOT_SUPPORTED );
        }
        return( drv->asymmetric->p_verify(
                    drv_context, *( (psa_key_slot_number_t *)key_buffer ),
                    alg, hash, hash_length,
                    signature, signature_length ) );
    }
#endif /* PSA_CRYPTO_SE_C */

    psa_status_t status = PSA_ERROR_CORRUPTION_DETECTED;
    psa_key_location_t location =
        PSA_KEY_LIFETIME_GET_LOCATION( attributes->core.lifetime );

    switch( location )
    {
        case PSA_KEY_LOCATION_LOCAL_STORAGE:
            /* Key is stored in the slot in export representation, so
             * cycle through all known transparent accelerators */
#if defined(PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT)
#if defined(PSA_CRYPTO_DRIVER_TEST)
            status = mbedtls_test_transparent_signature_verify_hash(
                         attributes,
                         key_buffer,
                         key_buffer_size,
                         alg,
                         hash,
                         hash_length,
                         signature,
                         signature_length );
            /* Declared with fallback == true */
            if( status != PSA_ERROR_NOT_SUPPORTED )
                return( status );
#endif /* PSA_CRYPTO_DRIVER_TEST */
#if defined(SEMAILBOX_PRESENT)
            status = sli_se_transparent_verify_hash( attributes,
                                                     key_buffer,
                                                     key_buffer_size,
                                                     alg,
                                                     hash,
                                                     hash_length,
                                                     signature,
                                                     signature_length );
            /* Declared with fallback == true */
            if( status != PSA_ERROR_NOT_SUPPORTED )
                return( status );
#elif defined(CRYPTOACC_PRESENT)
            status = sli_cryptoacc_transparent_verify_hash( attributes,
                                                            key_buffer,
                                                            key_buffer_size,
                                                            alg,
                                                            hash,
                                                            hash_length,
                                                            signature,
                                                            signature_length );
            /* Declared with fallback == true */
            if( status != PSA_ERROR_NOT_SUPPORTED )
                return( status );
#endif /* SEMAILBOX_PRESENT CRYPTOACC_PRESENT*/
#endif /* PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT */

            return( psa_verify_hash_builtin( attributes,
                                             key_buffer,
                                             key_buffer_size,
                                             alg,
                                             hash,
                                             hash_length,
                                             signature,
                                             signature_length ) );

        /* Add cases for opaque driver here */
#if defined(PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT)
#if defined(PSA_CRYPTO_DRIVER_TEST)
        case PSA_CRYPTO_TEST_DRIVER_LOCATION:
            return( mbedtls_test_opaque_signature_verify_hash( attributes,
                                                               key_buffer,
                                                               key_buffer_size,
                                                               alg,
                                                               hash,
                                                               hash_length,
                                                               signature,
                                                               signature_length ) );
#endif /* PSA_CRYPTO_DRIVER_TEST */
#if defined(SEMAILBOX_PRESENT) && \
  ( (_SILICON_LABS_SECURITY_FEATURE == _SILICON_LABS_SECURITY_FEATURE_VAULT) || \
    defined(MBEDTLS_PSA_CRYPTO_BUILTIN_KEYS) )
        case PSA_KEY_LOCATION_SLI_SE_OPAQUE:
            status = sli_se_opaque_verify_hash( attributes,
                                                key_buffer,
                                                key_buffer_size,
                                                alg,
                                                hash,
                                                hash_length,
                                                signature,
                                                signature_length );
            /* No fallback for opaque */
            return( status );
#endif /* VAULT */
#endif /* PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT */
        default:
            /* Key is declared with a lifetime not known to us */
            (void)status;
            return( PSA_ERROR_INVALID_ARGUMENT );
    }
}

/** Get the key buffer size required to store the key material of a key
 *  associated with an opaque driver without storage.
 *
 * \param[in] attributes  The key attributes.
 * \param[out] key_buffer_size  Minimum buffer size to contain the key material
 *
 * \retval #PSA_SUCCESS
 *         The minimum size for a buffer to contain the key material has been
 *         returned successfully.
 * \retval #PSA_ERROR_INVALID_ARGUMENT
 *         The size in bits of the key is not valid.
 * \retval #PSA_ERROR_NOT_SUPPORTED
 *         The type and/or the size in bits of the key or the combination of
 *         the two is not supported.
 */
psa_status_t psa_driver_wrapper_get_key_buffer_size(
    const psa_key_attributes_t *attributes,
    size_t *key_buffer_size )
{
    psa_key_location_t location = PSA_KEY_LIFETIME_GET_LOCATION( attributes->core.lifetime );
    psa_key_type_t key_type = attributes->core.type;
    size_t key_bits = attributes->core.bits;
    size_t buffer_size = 0;

    switch( location )
    {
        case PSA_KEY_LOCATION_LOCAL_STORAGE:
            buffer_size = PSA_EXPORT_KEY_OUTPUT_SIZE( key_type, key_bits );
            if( buffer_size == 0 ||
                ( PSA_KEY_TYPE_IS_ECC_PUBLIC_KEY(key_type) && buffer_size == 1 ) )
                return( PSA_ERROR_NOT_SUPPORTED );
            *key_buffer_size = buffer_size;
            return( PSA_SUCCESS );
#if defined(SEMAILBOX_PRESENT) \
     && ((_SILICON_LABS_SECURITY_FEATURE == _SILICON_LABS_SECURITY_FEATURE_VAULT) \
         || defined(MBEDTLS_PSA_CRYPTO_BUILTIN_KEYS))
        case PSA_KEY_LOCATION_SLI_SE_OPAQUE:
            buffer_size = PSA_EXPORT_KEY_OUTPUT_SIZE( key_type, key_bits );
            if( buffer_size == 0 ||
                ( PSA_KEY_TYPE_IS_ECC_PUBLIC_KEY(key_type) && buffer_size == 1 ) )
                buffer_size = *key_buffer_size;
            // Remove public key format byte
            if( PSA_KEY_TYPE_IS_ECC_PUBLIC_KEY( key_type ) ) {
                buffer_size--;
            }
            // Compensate for word alignment demands
            buffer_size = sli_se_word_align( buffer_size );
            if( PSA_BITS_TO_BYTES( key_bits ) & 0x3 || PSA_BITS_TO_BYTES( key_bits ) == 0 ) {
                if( PSA_KEY_TYPE_IS_ECC_PUBLIC_KEY(key_type) ) {
                    // Allocate extra word for public keys, since alignment constrains
                    // May require that
                    buffer_size += sizeof(uint32_t);
                }
            }
            // Add wrapped context overhead
            buffer_size += sizeof(sli_se_opaque_wrapped_key_context_t);
            *key_buffer_size = buffer_size;
            return ( PSA_SUCCESS );
#endif // VAULT || MBEDTLS_PSA_CRYPTO_BUILTIN_KEYS
#if defined(PSA_CRYPTO_DRIVER_TEST)
        case PSA_CRYPTO_TEST_DRIVER_LOCATION:
#if defined(MBEDTLS_PSA_CRYPTO_BUILTIN_KEYS)
            /* Emulate property 'builtin_key_size' */
            if( psa_key_id_is_builtin(
                    MBEDTLS_SVC_KEY_ID_GET_KEY_ID(
                        psa_get_key_id( attributes ) ) ) )
            {
                *key_buffer_size = sizeof( psa_drv_slot_number_t );
                return( PSA_SUCCESS );
            }
#endif /* MBEDTLS_PSA_CRYPTO_BUILTIN_KEYS */
            *key_buffer_size = mbedtls_test_size_function( key_type, key_bits );
            return( ( *key_buffer_size != 0 ) ?
                    PSA_SUCCESS : PSA_ERROR_NOT_SUPPORTED );
#endif /* PSA_CRYPTO_DRIVER_TEST */

        default:
            (void)key_type;
            (void)key_bits;
            *key_buffer_size = 0;
            return( PSA_ERROR_INVALID_ARGUMENT );
    }
}

psa_status_t psa_driver_wrapper_generate_key(
    const psa_key_attributes_t *attributes,
    uint8_t *key_buffer, size_t key_buffer_size, size_t *key_buffer_length )
{
    psa_status_t status = PSA_ERROR_CORRUPTION_DETECTED;
    psa_key_location_t location =
        PSA_KEY_LIFETIME_GET_LOCATION(attributes->core.lifetime);

    /* Try dynamically-registered SE interface first */
#if defined(MBEDTLS_PSA_CRYPTO_SE_C)
    const psa_drv_se_t *drv;
    psa_drv_se_context_t *drv_context;

    if( psa_get_se_driver( attributes->core.lifetime, &drv, &drv_context ) )
    {
        size_t pubkey_length = 0; /* We don't support this feature yet */
        if( drv->key_management == NULL ||
            drv->key_management->p_generate == NULL )
        {
            /* Key is defined as being in SE, but we have no way to generate it */
            return( PSA_ERROR_NOT_SUPPORTED );
        }
        return( drv->key_management->p_generate(
            drv_context,
            *( (psa_key_slot_number_t *)key_buffer ),
            attributes, NULL, 0, &pubkey_length ) );
    }
#endif /* MBEDTLS_PSA_CRYPTO_SE_C */

    switch( location )
    {
        case PSA_KEY_LOCATION_LOCAL_STORAGE:
#if defined(PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT)
            /* Transparent drivers are limited to generating asymmetric keys */
            if( PSA_KEY_TYPE_IS_ASYMMETRIC( attributes->core.type ) )
            {
            /* Cycle through all known transparent accelerators */
#if defined(PSA_CRYPTO_DRIVER_TEST)
                status = mbedtls_test_transparent_generate_key(
                    attributes, key_buffer, key_buffer_size,
                    key_buffer_length );
                /* Declared with fallback == true */
                if( status != PSA_ERROR_NOT_SUPPORTED )
                    break;
#endif /* PSA_CRYPTO_DRIVER_TEST */
#if defined(SEMAILBOX_PRESENT)
                status = sli_se_transparent_generate_key( attributes,
                                                          key_buffer,
                                                          key_buffer_size,
                                                          key_buffer_length );
                /* Declared with fallback == true */
                if( status != PSA_ERROR_NOT_SUPPORTED )
                    break;
#endif /* SEMAILBOX_PRESENT */
#if defined(CRYPTOACC_PRESENT)
                status = sli_cryptoacc_transparent_generate_key( attributes,
                                                                 key_buffer,
                                                                 key_buffer_size,
                                                                 key_buffer_length );
                /* Declared with fallback == true */
                if( status != PSA_ERROR_NOT_SUPPORTED )
                    break;
#endif /* CRYPTOACC_PRESENT */
            }
#endif /* PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT */

            /* Software fallback */
            status = psa_generate_key_internal(
                attributes, key_buffer, key_buffer_size, key_buffer_length );
            break;

        /* Add cases for opaque driver here */
#if defined(PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT)
#if defined(PSA_CRYPTO_DRIVER_TEST)
        case PSA_CRYPTO_TEST_DRIVER_LOCATION:
            status = mbedtls_test_opaque_generate_key(
                attributes, key_buffer, key_buffer_size, key_buffer_length );
            break;
#endif /* PSA_CRYPTO_DRIVER_TEST */
#if defined(SEMAILBOX_PRESENT) && (_SILICON_LABS_SECURITY_FEATURE == _SILICON_LABS_SECURITY_FEATURE_VAULT)
        case PSA_KEY_LOCATION_SLI_SE_OPAQUE:
            // Call opaque driver API
            status = sli_se_opaque_generate_key( attributes,
                                                 key_buffer,
                                                 key_buffer_size,
                                                 key_buffer_length );
            /* No fallback for opaque drivers */
            return status;
#endif /* SEMAILBOX_PRESENT && VAULT*/
#endif /* PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT */

        default:
            /* Key is declared with a lifetime not known to us */
            status = PSA_ERROR_INVALID_ARGUMENT;
            break;
    }

    return( status );
}

psa_status_t psa_driver_wrapper_import_key(
    const psa_key_attributes_t *attributes,
    const uint8_t *data,
    size_t data_length,
    uint8_t *key_buffer,
    size_t key_buffer_size,
    size_t *key_buffer_length,
    size_t *bits )
{
    psa_status_t status = PSA_ERROR_CORRUPTION_DETECTED;
    psa_key_location_t location = PSA_KEY_LIFETIME_GET_LOCATION(
                                      psa_get_key_lifetime( attributes ) );

    /* Try dynamically-registered SE interface first */
#if defined(MBEDTLS_PSA_CRYPTO_SE_C)
    const psa_drv_se_t *drv;
    psa_drv_se_context_t *drv_context;

    if( psa_get_se_driver( attributes->core.lifetime, &drv, &drv_context ) )
    {
        if( drv->key_management == NULL ||
            drv->key_management->p_import == NULL )
            return( PSA_ERROR_NOT_SUPPORTED );

        /* The driver should set the number of key bits, however in
         * case it doesn't, we initialize bits to an invalid value. */
        *bits = PSA_MAX_KEY_BITS + 1;
        status = drv->key_management->p_import(
            drv_context,
            *( (psa_key_slot_number_t *)key_buffer ),
            attributes, data, data_length, bits );

        if( status != PSA_SUCCESS )
            return( status );

        if( (*bits) > PSA_MAX_KEY_BITS )
            return( PSA_ERROR_NOT_SUPPORTED );

        return( PSA_SUCCESS );
    }
#endif /* PSA_CRYPTO_SE_C */

    switch( location )
    {
        case PSA_KEY_LOCATION_LOCAL_STORAGE:
            /* Key is stored in the slot in export representation, so
             * cycle through all known transparent accelerators */
#if defined(PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT)
#if defined(PSA_CRYPTO_DRIVER_TEST)
            status = mbedtls_test_transparent_import_key(
                         attributes,
                         data, data_length,
                         key_buffer, key_buffer_size,
                         key_buffer_length, bits );
            /* Declared with fallback == true */
            if( status != PSA_ERROR_NOT_SUPPORTED )
                return( status );
#endif /* PSA_CRYPTO_DRIVER_TEST */
#if defined(CRYPTOACC_PRESENT)
            status = sli_cryptoacc_transparent_import_key( attributes,
                                                           data, data_length,
                                                           key_buffer,
                                                           key_buffer_size,
                                                           key_buffer_length,
                                                           bits );
            /* Declared with fallback == true */
            if( status != PSA_ERROR_NOT_SUPPORTED )
                return( status );
#endif /* CRYPTOACC_PRESENT */
#if defined(SEMAILBOX_PRESENT)
            status = sli_se_transparent_import_key( attributes,
                                                    data, data_length,
                                                    key_buffer,
                                                    key_buffer_size,
                                                    key_buffer_length,
                                                    bits );
            /* Declared with fallback == true */
            if( status != PSA_ERROR_NOT_SUPPORTED )
                return( status );
#endif /* SEMAILBOX_PRESENT */
#endif /* PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT */
            /* Fell through, meaning no accelerator supports this operation */
            return( psa_import_key_into_slot( attributes,
                                              data, data_length,
                                              key_buffer, key_buffer_size,
                                              key_buffer_length, bits ) );

#if defined(SEMAILBOX_PRESENT) && (_SILICON_LABS_SECURITY_FEATURE == _SILICON_LABS_SECURITY_FEATURE_VAULT) \
  && defined(PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT)
        case PSA_KEY_LOCATION_SLI_SE_OPAQUE:
            {
                status = sli_se_opaque_import_key( attributes,
                                                   data, data_length,
                                                   key_buffer,
                                                   key_buffer_size,
                                                   key_buffer_length,
                                                   bits);
                return status;
            }
#endif // VAULT && PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT

        default:
            /* Importing a key with external storage in not yet supported.
             * Return in error indicating that the lifetime is not valid. */
            (void)status;
            return( PSA_ERROR_INVALID_ARGUMENT );
    }

}

psa_status_t psa_driver_wrapper_export_key(
    const psa_key_attributes_t *attributes,
    const uint8_t *key_buffer, size_t key_buffer_size,
    uint8_t *data, size_t data_size, size_t *data_length )

{
    psa_status_t status = PSA_ERROR_INVALID_ARGUMENT;
    psa_key_location_t location = PSA_KEY_LIFETIME_GET_LOCATION(
                                      psa_get_key_lifetime( attributes ) );

    /* Try dynamically-registered SE interface first */
#if defined(MBEDTLS_PSA_CRYPTO_SE_C)
    const psa_drv_se_t *drv;
    psa_drv_se_context_t *drv_context;

    if( psa_get_se_driver( attributes->core.lifetime, &drv, &drv_context ) )
    {
        if( ( drv->key_management == NULL   ) ||
            ( drv->key_management->p_export == NULL ) )
        {
            return( PSA_ERROR_NOT_SUPPORTED );
        }

        return( drv->key_management->p_export(
                     drv_context,
                     *( (psa_key_slot_number_t *)key_buffer ),
                     data, data_size, data_length ) );
    }
#endif /* PSA_CRYPTO_SE_C */

    switch( location )
    {
        case PSA_KEY_LOCATION_LOCAL_STORAGE:
            return( psa_export_key_internal( attributes,
                                             key_buffer,
                                             key_buffer_size,
                                             data,
                                             data_size,
                                             data_length ) );

        /* Add cases for opaque driver here */
#if defined(PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT)
#if defined(PSA_CRYPTO_DRIVER_TEST)
        case PSA_CRYPTO_TEST_DRIVER_LOCATION:
            return( mbedtls_test_opaque_export_key( attributes,
                                                    key_buffer,
                                                    key_buffer_size,
                                                    data,
                                                    data_size,
                                                    data_length ) );
#endif /* PSA_CRYPTO_DRIVER_TEST */
#if defined(SEMAILBOX_PRESENT) && (_SILICON_LABS_SECURITY_FEATURE == _SILICON_LABS_SECURITY_FEATURE_VAULT)
        case PSA_KEY_LOCATION_SLI_SE_OPAQUE:
            // We are on a vault device, call opaque driver
            return( sli_se_opaque_export_key( attributes,
                                              key_buffer,
                                              key_buffer_size,
                                              data,
                                              data_size,
                                              data_length ) );
#endif // VAULT
#endif /* PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT */
        default:
            /* Key is declared with a lifetime not known to us */
            return( status );
    }
}

psa_status_t psa_driver_wrapper_export_public_key(
    const psa_key_attributes_t *attributes,
    const uint8_t *key_buffer, size_t key_buffer_size,
    uint8_t *data, size_t data_size, size_t *data_length )

{
    psa_status_t status = PSA_ERROR_INVALID_ARGUMENT;
    psa_key_location_t location = PSA_KEY_LIFETIME_GET_LOCATION(
                                      psa_get_key_lifetime( attributes ) );

    /* Try dynamically-registered SE interface first */
#if defined(MBEDTLS_PSA_CRYPTO_SE_C)
    const psa_drv_se_t *drv;
    psa_drv_se_context_t *drv_context;

    if( psa_get_se_driver( attributes->core.lifetime, &drv, &drv_context ) )
    {
        if( ( drv->key_management == NULL ) ||
            ( drv->key_management->p_export_public == NULL ) )
        {
            return( PSA_ERROR_NOT_SUPPORTED );
        }

        return( drv->key_management->p_export_public(
                    drv_context,
                    *( (psa_key_slot_number_t *)key_buffer ),
                    data, data_size, data_length ) );
    }
#endif /* MBEDTLS_PSA_CRYPTO_SE_C */

    switch( location )
    {
        case PSA_KEY_LOCATION_LOCAL_STORAGE:
            /* Key is stored in the slot in export representation, so
             * cycle through all known transparent accelerators */
#if defined(PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT)
#if defined(PSA_CRYPTO_DRIVER_TEST)
            status = mbedtls_test_transparent_export_public_key(
                         attributes,
                         key_buffer,
                         key_buffer_size,
                         data,
                         data_size,
                         data_length );
            /* Declared with fallback == true */
            if( status != PSA_ERROR_NOT_SUPPORTED )
                return( status );
#endif /* PSA_CRYPTO_DRIVER_TEST */
#if defined(SEMAILBOX_PRESENT)
            status = sli_se_transparent_export_public_key( attributes,
                                                           key_buffer,
                                                           key_buffer_size,
                                                           data,
                                                           data_size,
                                                           data_length );
            /* Declared with fallback == true */
            if( status != PSA_ERROR_NOT_SUPPORTED )
                return( status );
#endif // SEMAILBOX_PRESENT
#if defined(CRYPTOACC_PRESENT)
            status = sli_cryptoacc_transparent_export_public_key( attributes,
                                                                  key_buffer,
                                                                  key_buffer_size,
                                                                  data,
                                                                  data_size,
                                                                  data_length );
            /* Declared with fallback == true */
            if( status != PSA_ERROR_NOT_SUPPORTED )
                return( status );
#endif // CRYPTOACC_PRESENT
#endif /* PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT */
            /* Fell through, meaning no accelerator supports this operation */
            return( psa_export_public_key_internal( attributes,
                                                    key_buffer,
                                                    key_buffer_size,
                                                    data,
                                                    data_size,
                                                    data_length ) );

        /* Add cases for opaque driver here */
#if defined(PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT)
#if defined(PSA_CRYPTO_DRIVER_TEST)
        case PSA_CRYPTO_TEST_DRIVER_LOCATION:
            return( mbedtls_test_opaque_export_public_key( attributes,
                                                           key_buffer,
                                                           key_buffer_size,
                                                           data,
                                                           data_size,
                                                           data_length ) );
#endif /* PSA_CRYPTO_DRIVER_TEST */
#if defined(SEMAILBOX_PRESENT) && \
  ( (_SILICON_LABS_SECURITY_FEATURE == _SILICON_LABS_SECURITY_FEATURE_VAULT) || \
    defined(MBEDTLS_PSA_CRYPTO_BUILTIN_KEYS) )
        case PSA_KEY_LOCATION_SLI_SE_OPAQUE:
            return( sli_se_opaque_export_public_key( attributes,
                                                     key_buffer,
                                                     key_buffer_size,
                                                     data,
                                                     data_size,
                                                     data_length ) );
#endif // SEMAILBOX_PRESENT & VAULT
#endif /* PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT */
        default:
            /* Key is declared with a lifetime not known to us */
            return( status );
    }
}

psa_status_t psa_driver_wrapper_get_builtin_key(
    psa_drv_slot_number_t slot_number,
    psa_key_attributes_t *attributes,
    uint8_t *key_buffer, size_t key_buffer_size, size_t *key_buffer_length )
{
    psa_key_location_t location = PSA_KEY_LIFETIME_GET_LOCATION( attributes->core.lifetime );
    switch( location )
    {
#if defined(PSA_CRYPTO_DRIVER_TEST)
        case PSA_CRYPTO_TEST_DRIVER_LOCATION:
            return( mbedtls_test_opaque_get_builtin_key(
                        slot_number,
                        attributes,
                        key_buffer, key_buffer_size, key_buffer_length ) );
#endif /* PSA_CRYPTO_DRIVER_TEST */
#if defined(SEMAILBOX_PRESENT) && \
  ( (_SILICON_LABS_SECURITY_FEATURE == _SILICON_LABS_SECURITY_FEATURE_VAULT) || \
    defined(MBEDTLS_PSA_CRYPTO_BUILTIN_KEYS) )
        case PSA_KEY_LOCATION_SLI_SE_OPAQUE:
            return( sli_se_opaque_get_builtin_key(
                        slot_number,
                        attributes,
                        key_buffer, key_buffer_size, key_buffer_length ) );
#endif // SEMAILBOX_PRESENT & VAULT
        default:
            (void) slot_number;
            (void) key_buffer;
            (void) key_buffer_size;
            (void) key_buffer_length;
            return( PSA_ERROR_DOES_NOT_EXIST );
    }
}

/*
 * Cipher functions
 */
psa_status_t psa_driver_wrapper_cipher_encrypt(
    psa_key_slot_t *slot,
    psa_algorithm_t alg,
    const uint8_t *input,
    size_t input_length,
    uint8_t *output,
    size_t output_size,
    size_t *output_length )
{
#if defined(PSA_CRYPTO_DRIVER_PRESENT) && defined(PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT)
    psa_status_t status = PSA_ERROR_INVALID_ARGUMENT;
    psa_key_location_t location = PSA_KEY_LIFETIME_GET_LOCATION(slot->attr.lifetime);
    psa_key_attributes_t attributes = {
      .core = slot->attr
    };

    switch( location )
    {
        case PSA_KEY_LOCATION_LOCAL_STORAGE:
            /* Key is stored in the slot in export representation, so
             * cycle through all known transparent accelerators */
#if defined(PSA_CRYPTO_DRIVER_TEST)
            status = mbedtls_test_transparent_cipher_encrypt( &attributes,
                                                              slot->key.data,
                                                              slot->key.bytes,
                                                              alg,
                                                              input,
                                                              input_length,
                                                              output,
                                                              output_size,
                                                              output_length );
            /* Declared with fallback == true */
            if( status != PSA_ERROR_NOT_SUPPORTED )
                return( status );
#endif /* PSA_CRYPTO_DRIVER_TEST */
#if defined(MBEDTLS_PSA_CRYPTO_DRIVERS)
#if defined(SEMAILBOX_PRESENT)
            status = sli_se_transparent_cipher_encrypt(
                        &attributes,
                        slot->key.data,
                        slot->key.bytes,
                        alg,
                        input,
                        input_length,
                        output,
                        output_size,
                        output_length );

            if( status != PSA_ERROR_NOT_SUPPORTED )
                return( status );
#endif /* SEMAILBOX_PRESENT */
#if defined(CRYPTOACC_PRESENT)
            status = sli_cryptoacc_transparent_cipher_encrypt(
                        &attributes,
                        slot->key.data,
                        slot->key.bytes,
                        alg,
                        input,
                        input_length,
                        output,
                        output_size,
                        output_length );

            if( status != PSA_ERROR_NOT_SUPPORTED )
                return( status );
#endif /* CRYPTOACC_PRESENT */
#if defined(CRYPTO_PRESENT)
            status = sli_crypto_transparent_cipher_encrypt(
                        &attributes,
                        slot->key.data,
                        slot->key.bytes,
                        alg,
                        input,
                        input_length,
                        output,
                        output_size,
                        output_length );

            if( status != PSA_ERROR_NOT_SUPPORTED )
                return( status );
#endif /* CRYPTO_PRESENT */
#endif
            /* Fell through, meaning no accelerator supports this operation */
            return( PSA_ERROR_NOT_SUPPORTED );
        /* Add cases for opaque driver here */
#if defined(PSA_CRYPTO_DRIVER_TEST)
        case PSA_CRYPTO_TEST_DRIVER_LOCATION:
            return( mbedtls_test_opaque_cipher_encrypt( &attributes,
                                                        slot->key.data,
                                                        slot->key.bytes,
                                                        alg,
                                                        input,
                                                        input_length,
                                                        output,
                                                        output_size,
                                                        output_length ) );
#endif /* PSA_CRYPTO_DRIVER_TEST */
#if defined(SEMAILBOX_PRESENT) && (_SILICON_LABS_SECURITY_FEATURE == _SILICON_LABS_SECURITY_FEATURE_VAULT) || \
    defined(MBEDTLS_PSA_CRYPTO_BUILTIN_KEYS)
        case PSA_KEY_LOCATION_SLI_SE_OPAQUE:
            return ( sli_se_opaque_cipher_encrypt(
                        &attributes,
                        slot->key.data,
                        slot->key.bytes,
                        alg,
                        input,
                        input_length,
                        output,
                        output_size,
                        output_length ) );
#endif // VAULT
        default:
            /* Key is declared with a lifetime not known to us */
            return( status );
    }
#else /* PSA_CRYPTO_DRIVER_PRESENT */
    (void) slot;
    (void) alg;
    (void) input;
    (void) input_length;
    (void) output;
    (void) output_size;
    (void) output_length;

    return( PSA_ERROR_NOT_SUPPORTED );
#endif /* PSA_CRYPTO_DRIVER_PRESENT */
}

psa_status_t psa_driver_wrapper_cipher_decrypt(
    psa_key_slot_t *slot,
    psa_algorithm_t alg,
    const uint8_t *input,
    size_t input_length,
    uint8_t *output,
    size_t output_size,
    size_t *output_length )
{
#if defined(PSA_CRYPTO_DRIVER_PRESENT) && defined(PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT)
    psa_status_t status = PSA_ERROR_INVALID_ARGUMENT;
    psa_key_location_t location = PSA_KEY_LIFETIME_GET_LOCATION(slot->attr.lifetime);
    psa_key_attributes_t attributes = {
      .core = slot->attr
    };

    switch( location )
    {
        case PSA_KEY_LOCATION_LOCAL_STORAGE:
            /* Key is stored in the slot in export representation, so
             * cycle through all known transparent accelerators */
#if defined(PSA_CRYPTO_DRIVER_TEST)
            status = mbedtls_test_transparent_cipher_decrypt( &attributes,
                                                              slot->key.data,
                                                              slot->key.bytes,
                                                              alg,
                                                              input,
                                                              input_length,
                                                              output,
                                                              output_size,
                                                              output_length );
            /* Declared with fallback == true */
            if( status != PSA_ERROR_NOT_SUPPORTED )
                return( status );
#endif /* PSA_CRYPTO_DRIVER_TEST */
#if defined(MBEDTLS_PSA_CRYPTO_DRIVERS)
#if defined(SEMAILBOX_PRESENT)
            status = sli_se_transparent_cipher_decrypt(
                        &attributes,
                        slot->key.data,
                        slot->key.bytes,
                        alg,
                        input,
                        input_length,
                        output,
                        output_size,
                        output_length );

            if( status != PSA_ERROR_NOT_SUPPORTED )
                return( status );
#endif /* SEMAILBOX_PRESENT */
#if defined(CRYPTOACC_PRESENT)
            status = sli_cryptoacc_transparent_cipher_decrypt(
                        &attributes,
                        slot->key.data,
                        slot->key.bytes,
                        alg,
                        input,
                        input_length,
                        output,
                        output_size,
                        output_length );

            if( status != PSA_ERROR_NOT_SUPPORTED )
                return( status );
#endif /* CRYPTOACC_PRESENT */
#if defined(CRYPTO_PRESENT)
            status = sli_crypto_transparent_cipher_decrypt(
                        &attributes,
                        slot->key.data,
                        slot->key.bytes,
                        alg,
                        input,
                        input_length,
                        output,
                        output_size,
                        output_length );

            if( status != PSA_ERROR_NOT_SUPPORTED )
                return( status );
#endif /* CRYPTO_PRESENT */
#endif
            /* Fell through, meaning no accelerator supports this operation */
            return( PSA_ERROR_NOT_SUPPORTED );
        /* Add cases for opaque driver here */
#if defined(PSA_CRYPTO_DRIVER_TEST)
        case PSA_CRYPTO_TEST_DRIVER_LOCATION:
            return( mbedtls_test_opaque_cipher_decrypt( &attributes,
                                                        slot->key.data,
                                                        slot->key.bytes,
                                                        alg,
                                                        input,
                                                        input_length,
                                                        output,
                                                        output_size,
                                                        output_length ) );
#endif /* PSA_CRYPTO_DRIVER_TEST */
#if defined(SEMAILBOX_PRESENT) && (_SILICON_LABS_SECURITY_FEATURE == _SILICON_LABS_SECURITY_FEATURE_VAULT) || \
    defined(MBEDTLS_PSA_CRYPTO_BUILTIN_KEYS)
        case PSA_KEY_LOCATION_SLI_SE_OPAQUE:
            return ( sli_se_opaque_cipher_decrypt(
                        &attributes,
                        slot->key.data,
                        slot->key.bytes,
                        alg,
                        input,
                        input_length,
                        output,
                        output_size,
                        output_length ) );
#endif // VAULT
        default:
            /* Key is declared with a lifetime not known to us */
            return( status );
    }
#else /* PSA_CRYPTO_DRIVER_PRESENT */
    (void) slot;
    (void) alg;
    (void) input;
    (void) input_length;
    (void) output;
    (void) output_size;
    (void) output_length;

    return( PSA_ERROR_NOT_SUPPORTED );
#endif /* PSA_CRYPTO_DRIVER_PRESENT */
}

psa_status_t psa_driver_wrapper_cipher_encrypt_setup(
    psa_cipher_operation_t *operation,
    const psa_key_attributes_t *attributes,
    const uint8_t *key_buffer, size_t key_buffer_size,
    psa_algorithm_t alg )
{
    psa_status_t status = PSA_ERROR_CORRUPTION_DETECTED;
    psa_key_location_t location =
        PSA_KEY_LIFETIME_GET_LOCATION( attributes->core.lifetime );

    switch( location )
    {
        case PSA_KEY_LOCATION_LOCAL_STORAGE:
            /* Key is stored in the slot in export representation, so
             * cycle through all known transparent accelerators */
#if defined(PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT)
#if defined(PSA_CRYPTO_DRIVER_TEST)
            status = mbedtls_test_transparent_cipher_encrypt_setup(
                &operation->ctx.transparent_test_driver_ctx,
                attributes,
                key_buffer,
                key_buffer_size,
                alg );
            /* Declared with fallback == true */
            if( status == PSA_SUCCESS )
                operation->id = PSA_CRYPTO_TRANSPARENT_TEST_DRIVER_ID;

            if( status != PSA_ERROR_NOT_SUPPORTED )
                return( status );
#endif /* PSA_CRYPTO_DRIVER_TEST */
#if defined(SEMAILBOX_PRESENT)
            status = sli_se_transparent_cipher_encrypt_setup(
                        &operation->ctx.sli_se_transparent_ctx,
                        attributes,
                        key_buffer,
                        key_buffer_size,
                        alg );
            if( status == PSA_SUCCESS )
                operation->id = SLI_SE_TRANSPARENT_DRIVER_ID;

            if( status != PSA_ERROR_NOT_SUPPORTED )
                return( status );
#endif /* SEMAILBOX_PRESENT */
#if defined(CRYPTOACC_PRESENT)
            status = sli_cryptoacc_transparent_cipher_encrypt_setup(
                        &operation->ctx.sli_cryptoacc_transparent_ctx,
                        attributes,
                        key_buffer,
                        key_buffer_size,
                        alg );
            if( status == PSA_SUCCESS )
                operation->id = SLI_CRYPTOACC_TRANSPARENT_DRIVER_ID;

            if( status != PSA_ERROR_NOT_SUPPORTED )
                return( status );
#endif /* CRYPTOACC_PRESENT */
#if defined(CRYPTO_PRESENT)
            status = sli_crypto_transparent_cipher_encrypt_setup(
                        &operation->ctx.sli_crypto_transparent_ctx,
                        attributes,
                        key_buffer,
                        key_buffer_size,
                        alg );
            if( status == PSA_SUCCESS )
                operation->id = SLI_CRYPTO_TRANSPARENT_DRIVER_ID;

            if( status != PSA_ERROR_NOT_SUPPORTED )
                return( status );
#endif /* CRYPTO_PRESENT */
#endif /* PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT */
#if defined(MBEDTLS_PSA_BUILTIN_CIPHER)
            /* Fell through, meaning no accelerator supports this operation */
            status = mbedtls_psa_cipher_encrypt_setup( &operation->ctx.mbedtls_ctx,
                                                       attributes,
                                                       key_buffer,
                                                       key_buffer_size,
                                                       alg );
            if( status == PSA_SUCCESS )
                operation->id = PSA_CRYPTO_MBED_TLS_DRIVER_ID;

            if( status != PSA_ERROR_NOT_SUPPORTED )
                return( status );
#endif /* MBEDTLS_PSA_BUILTIN_CIPHER */
            return( PSA_ERROR_NOT_SUPPORTED );

        /* Add cases for opaque driver here */
#if defined(PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT)
#if defined(PSA_CRYPTO_DRIVER_TEST)
        case PSA_CRYPTO_TEST_DRIVER_LOCATION:
            status = mbedtls_test_opaque_cipher_encrypt_setup(
                &operation->ctx.opaque_test_driver_ctx,
                attributes,
                key_buffer, key_buffer_size,
                alg );

            if( status == PSA_SUCCESS )
                operation->id = PSA_CRYPTO_OPAQUE_TEST_DRIVER_ID;

            return( status );
#endif /* PSA_CRYPTO_DRIVER_TEST */
#if defined(SEMAILBOX_PRESENT) && (_SILICON_LABS_SECURITY_FEATURE == _SILICON_LABS_SECURITY_FEATURE_VAULT) || \
    defined(MBEDTLS_PSA_CRYPTO_BUILTIN_KEYS)
        case PSA_KEY_LOCATION_SLI_SE_OPAQUE:
            status = sli_se_opaque_cipher_encrypt_setup(
                        &operation->ctx.sli_se_opaque_ctx,
                        attributes,
                        key_buffer,
                        key_buffer_size,
                        alg );
            if( status == PSA_SUCCESS )
                operation->id = SLI_SE_OPAQUE_DRIVER_ID;

            return( status );
#endif // VAULT
#endif /* PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT */
        default:
            /* Key is declared with a lifetime not known to us */
            (void)status;
            (void)key_buffer;
            (void)key_buffer_size;
            (void)alg;
            return( PSA_ERROR_INVALID_ARGUMENT );
    }
}

psa_status_t psa_driver_wrapper_cipher_decrypt_setup(
    psa_cipher_operation_t *operation,
    const psa_key_attributes_t *attributes,
    const uint8_t *key_buffer, size_t key_buffer_size,
    psa_algorithm_t alg )
{
    psa_status_t status = PSA_ERROR_INVALID_ARGUMENT;
    psa_key_location_t location =
        PSA_KEY_LIFETIME_GET_LOCATION( attributes->core.lifetime );

    switch( location )
    {
        case PSA_KEY_LOCATION_LOCAL_STORAGE:
            /* Key is stored in the slot in export representation, so
             * cycle through all known transparent accelerators */
#if defined(PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT)
#if defined(PSA_CRYPTO_DRIVER_TEST)
            status = mbedtls_test_transparent_cipher_decrypt_setup(
                &operation->ctx.transparent_test_driver_ctx,
                attributes,
                key_buffer,
                key_buffer_size,
                alg );
            /* Declared with fallback == true */
            if( status == PSA_SUCCESS )
                operation->id = PSA_CRYPTO_TRANSPARENT_TEST_DRIVER_ID;

            if( status != PSA_ERROR_NOT_SUPPORTED )
                return( status );
#endif /* PSA_CRYPTO_DRIVER_TEST */
#if defined(SEMAILBOX_PRESENT)
            status = sli_se_transparent_cipher_decrypt_setup(
                        &operation->ctx.sli_se_transparent_ctx,
                        attributes,
                        key_buffer,
                        key_buffer_size,
                        alg );
            if( status == PSA_SUCCESS )
                operation->id = SLI_SE_TRANSPARENT_DRIVER_ID;

            if( status != PSA_ERROR_NOT_SUPPORTED )
                return( status );
#endif /* SEMAILBOX_PRESENT */
#if defined(CRYPTOACC_PRESENT)
            status = sli_cryptoacc_transparent_cipher_decrypt_setup(
                        &operation->ctx.sli_cryptoacc_transparent_ctx,
                        attributes,
                        key_buffer,
                        key_buffer_size,
                        alg );
            if( status == PSA_SUCCESS )
                operation->id = SLI_CRYPTOACC_TRANSPARENT_DRIVER_ID;

            if( status != PSA_ERROR_NOT_SUPPORTED )
                return( status );
#endif /* CRYPTOACC_PRESENT */
#if defined(CRYPTO_PRESENT)
            status = sli_crypto_transparent_cipher_decrypt_setup(
                        &operation->ctx.sli_crypto_transparent_ctx,
                        attributes,
                        key_buffer,
                        key_buffer_size,
                        alg );
            if( status == PSA_SUCCESS )
                operation->id = SLI_CRYPTO_TRANSPARENT_DRIVER_ID;

            if( status != PSA_ERROR_NOT_SUPPORTED )
                return( status );
#endif /* CRYPTO_PRESENT */
#endif /* PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT */
#if defined(MBEDTLS_PSA_BUILTIN_CIPHER)
            /* Fell through, meaning no accelerator supports this operation */
            status = mbedtls_psa_cipher_decrypt_setup( &operation->ctx.mbedtls_ctx,
                                                       attributes,
                                                       key_buffer,
                                                       key_buffer_size,
                                                       alg );
            if( status == PSA_SUCCESS )
                operation->id = PSA_CRYPTO_MBED_TLS_DRIVER_ID;

            if( status != PSA_ERROR_NOT_SUPPORTED )
                return( status );
#endif /* MBEDTLS_PSA_BUILTIN_CIPHER */
            return( PSA_ERROR_NOT_SUPPORTED );

        /* Add cases for opaque driver here */
#if defined(PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT)
#if defined(PSA_CRYPTO_DRIVER_TEST)
        case PSA_CRYPTO_TEST_DRIVER_LOCATION:
            status = mbedtls_test_opaque_cipher_decrypt_setup(
                         &operation->ctx.opaque_test_driver_ctx,
                         attributes,
                         key_buffer, key_buffer_size,
                         alg );

            if( status == PSA_SUCCESS )
                operation->id = PSA_CRYPTO_OPAQUE_TEST_DRIVER_ID;

            return( status );
#endif /* PSA_CRYPTO_DRIVER_TEST */
#if defined(SEMAILBOX_PRESENT) && (_SILICON_LABS_SECURITY_FEATURE == _SILICON_LABS_SECURITY_FEATURE_VAULT) || \
    defined(MBEDTLS_PSA_CRYPTO_BUILTIN_KEYS)
        case PSA_KEY_LOCATION_SLI_SE_OPAQUE:
            status = sli_se_opaque_cipher_decrypt_setup(
                        &operation->ctx.sli_se_opaque_ctx,
                        attributes,
                        key_buffer,
                        key_buffer_size,
                        alg );
            if( status == PSA_SUCCESS )
                operation->id = SLI_SE_OPAQUE_DRIVER_ID;

            return( status );
#endif // VAULT
#endif /* PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT */
        default:
            /* Key is declared with a lifetime not known to us */
            (void)status;
            (void)key_buffer;
            (void)key_buffer_size;
            (void)alg;
            return( PSA_ERROR_INVALID_ARGUMENT );
    }
}

psa_status_t psa_driver_wrapper_cipher_set_iv(
    psa_cipher_operation_t *operation,
    const uint8_t *iv,
    size_t iv_length )
{
    switch( operation->id )
    {
#if defined(MBEDTLS_PSA_BUILTIN_CIPHER)
        case PSA_CRYPTO_MBED_TLS_DRIVER_ID:
            return( mbedtls_psa_cipher_set_iv( &operation->ctx.mbedtls_ctx,
                                               iv,
                                               iv_length ) );
#endif /* MBEDTLS_PSA_BUILTIN_CIPHER */

#if defined(PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT)
#if defined(PSA_CRYPTO_DRIVER_TEST)
        case PSA_CRYPTO_TRANSPARENT_TEST_DRIVER_ID:
            return( mbedtls_test_transparent_cipher_set_iv(
                        &operation->ctx.transparent_test_driver_ctx,
                        iv, iv_length ) );

        case PSA_CRYPTO_OPAQUE_TEST_DRIVER_ID:
            return( mbedtls_test_opaque_cipher_set_iv(
                        &operation->ctx.opaque_test_driver_ctx,
                        iv, iv_length ) );
#endif /* PSA_CRYPTO_DRIVER_TEST */
#if defined(SEMAILBOX_PRESENT)
        case SLI_SE_TRANSPARENT_DRIVER_ID:
            return( sli_se_transparent_cipher_set_iv(
                        &operation->ctx.sli_se_transparent_ctx,
                        iv, iv_length ) );
#endif /* SEMAILBOX_PRESENT */
#if defined(CRYPTOACC_PRESENT)
        case SLI_CRYPTOACC_TRANSPARENT_DRIVER_ID:
            return( sli_cryptoacc_transparent_cipher_set_iv(
                        &operation->ctx.sli_cryptoacc_transparent_ctx,
                        iv, iv_length ) );
#endif /* CRYPTOACC_PRESENT */
#if defined(CRYPTO_PRESENT)
        case SLI_CRYPTO_TRANSPARENT_DRIVER_ID:
            return( sli_crypto_transparent_cipher_set_iv(
                        &operation->ctx.sli_crypto_transparent_ctx,
                        iv, iv_length ) );
#endif /* CRYPTO_PRESENT */
#if defined(SEMAILBOX_PRESENT) && (_SILICON_LABS_SECURITY_FEATURE == _SILICON_LABS_SECURITY_FEATURE_VAULT) || \
    defined(MBEDTLS_PSA_CRYPTO_BUILTIN_KEYS)
        case SLI_SE_OPAQUE_DRIVER_ID:
            return( sli_se_opaque_cipher_set_iv(
                        &operation->ctx.sli_se_opaque_ctx,
                        iv, iv_length ) );
#endif // VAULT
#endif /* PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT */
    }

    (void)iv;
    (void)iv_length;

    return( PSA_ERROR_INVALID_ARGUMENT );
}

psa_status_t psa_driver_wrapper_cipher_update(
    psa_cipher_operation_t *operation,
    const uint8_t *input,
    size_t input_length,
    uint8_t *output,
    size_t output_size,
    size_t *output_length )
{
    switch( operation->id )
    {
#if defined(MBEDTLS_PSA_BUILTIN_CIPHER)
        case PSA_CRYPTO_MBED_TLS_DRIVER_ID:
            return( mbedtls_psa_cipher_update( &operation->ctx.mbedtls_ctx,
                                               input,
                                               input_length,
                                               output,
                                               output_size,
                                               output_length ) );
#endif /* MBEDTLS_PSA_BUILTIN_CIPHER */

#if defined(PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT)
#if defined(PSA_CRYPTO_DRIVER_TEST)
        case PSA_CRYPTO_TRANSPARENT_TEST_DRIVER_ID:
            return( mbedtls_test_transparent_cipher_update(
                        &operation->ctx.transparent_test_driver_ctx,
                        input, input_length,
                        output, output_size, output_length ) );

        case PSA_CRYPTO_OPAQUE_TEST_DRIVER_ID:
            return( mbedtls_test_opaque_cipher_update(
                        &operation->ctx.opaque_test_driver_ctx,
                        input, input_length,
                        output, output_size, output_length ) );
#endif /* PSA_CRYPTO_DRIVER_TEST */
#if defined(SEMAILBOX_PRESENT)
        case SLI_SE_TRANSPARENT_DRIVER_ID:
            return( sli_se_transparent_cipher_update(
                        &operation->ctx.sli_se_transparent_ctx,
                        input, input_length,
                        output, output_size, output_length ) );
#endif /* SEMAILBOX_PRESENT */
#if defined(CRYPTOACC_PRESENT)
        case SLI_CRYPTOACC_TRANSPARENT_DRIVER_ID:
            return( sli_cryptoacc_transparent_cipher_update(
                        &operation->ctx.sli_cryptoacc_transparent_ctx,
                        input, input_length,
                        output, output_size, output_length ) );
#endif /* CRYPTOACC_PRESENT */
#if defined(CRYPTO_PRESENT)
        case SLI_CRYPTO_TRANSPARENT_DRIVER_ID:
            return( sli_crypto_transparent_cipher_update(
                        &operation->ctx.sli_crypto_transparent_ctx,
                        input, input_length,
                        output, output_size, output_length ) );
#endif /* CRYPTO_PRESENT */
#if defined(SEMAILBOX_PRESENT) && (_SILICON_LABS_SECURITY_FEATURE == _SILICON_LABS_SECURITY_FEATURE_VAULT)|| \
    defined(MBEDTLS_PSA_CRYPTO_BUILTIN_KEYS)
        case SLI_SE_OPAQUE_DRIVER_ID:
            return( sli_se_opaque_cipher_update(
                        &operation->ctx.sli_se_opaque_ctx,
                        input, input_length,
                        output, output_size, output_length ) );
#endif // VAULT
#endif /* PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT */
    }

    (void)input;
    (void)input_length;
    (void)output;
    (void)output_size;
    (void)output_length;

    return( PSA_ERROR_INVALID_ARGUMENT );
}

psa_status_t psa_driver_wrapper_cipher_finish(
    psa_cipher_operation_t *operation,
    uint8_t *output,
    size_t output_size,
    size_t *output_length )
{
    switch( operation->id )
    {
#if defined(MBEDTLS_PSA_BUILTIN_CIPHER)
        case PSA_CRYPTO_MBED_TLS_DRIVER_ID:
            return( mbedtls_psa_cipher_finish( &operation->ctx.mbedtls_ctx,
                                               output,
                                               output_size,
                                               output_length ) );
#endif /* MBEDTLS_PSA_BUILTIN_CIPHER */

#if defined(PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT)
#if defined(PSA_CRYPTO_DRIVER_TEST)
        case PSA_CRYPTO_TRANSPARENT_TEST_DRIVER_ID:
            return( mbedtls_test_transparent_cipher_finish(
                        &operation->ctx.transparent_test_driver_ctx,
                        output, output_size, output_length ) );

        case PSA_CRYPTO_OPAQUE_TEST_DRIVER_ID:
            return( mbedtls_test_opaque_cipher_finish(
                        &operation->ctx.opaque_test_driver_ctx,
                        output, output_size, output_length ) );
#endif /* PSA_CRYPTO_DRIVER_TEST */
#if defined(SEMAILBOX_PRESENT)
        case SLI_SE_TRANSPARENT_DRIVER_ID:
            return( sli_se_transparent_cipher_finish(
                        &operation->ctx.sli_se_transparent_ctx,
                        output, output_size, output_length ) );
#endif /* SEMAILBOX_PRESENT */
#if defined(CRYPTOACC_PRESENT)
        case SLI_CRYPTOACC_TRANSPARENT_DRIVER_ID:
            return( sli_cryptoacc_transparent_cipher_finish(
                        &operation->ctx.sli_cryptoacc_transparent_ctx,
                        output, output_size, output_length ) );
#endif /* CRYPTOACC_PRESENT */
#if defined(CRYPTO_PRESENT)
        case SLI_CRYPTO_TRANSPARENT_DRIVER_ID:
            return( sli_crypto_transparent_cipher_finish(
                        &operation->ctx.sli_crypto_transparent_ctx,
                        output, output_size, output_length ) );
#endif /* CRYPTO_PRESENT */
#if defined(SEMAILBOX_PRESENT) && (_SILICON_LABS_SECURITY_FEATURE == _SILICON_LABS_SECURITY_FEATURE_VAULT) || \
    defined(MBEDTLS_PSA_CRYPTO_BUILTIN_KEYS)
        case SLI_SE_OPAQUE_DRIVER_ID:
            return( sli_se_opaque_cipher_finish(
                        &operation->ctx.sli_se_opaque_ctx,
                        output, output_size, output_length ) );
#endif // VAULT
#endif /* PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT */
    }

    (void)output;
    (void)output_size;
    (void)output_length;

    return( PSA_ERROR_INVALID_ARGUMENT );
}

psa_status_t psa_driver_wrapper_cipher_abort(
    psa_cipher_operation_t *operation )
{
    psa_status_t status = PSA_ERROR_CORRUPTION_DETECTED;

    switch( operation->id )
    {
#if defined(MBEDTLS_PSA_BUILTIN_CIPHER)
        case PSA_CRYPTO_MBED_TLS_DRIVER_ID:
            return( mbedtls_psa_cipher_abort( &operation->ctx.mbedtls_ctx ) );
#endif /* MBEDTLS_PSA_BUILTIN_CIPHER */

#if defined(PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT)
#if defined(PSA_CRYPTO_DRIVER_TEST)
        case PSA_CRYPTO_TRANSPARENT_TEST_DRIVER_ID:
            status = mbedtls_test_transparent_cipher_abort(
                         &operation->ctx.transparent_test_driver_ctx );
            mbedtls_platform_zeroize(
                &operation->ctx.transparent_test_driver_ctx,
                sizeof( operation->ctx.transparent_test_driver_ctx ) );
            return( status );

        case PSA_CRYPTO_OPAQUE_TEST_DRIVER_ID:
            status = mbedtls_test_opaque_cipher_abort(
                         &operation->ctx.opaque_test_driver_ctx );
            mbedtls_platform_zeroize(
                &operation->ctx.opaque_test_driver_ctx,
                sizeof( operation->ctx.opaque_test_driver_ctx ) );
            return( status );
#endif /* PSA_CRYPTO_DRIVER_TEST */
#if defined(SEMAILBOX_PRESENT)
        case SLI_SE_TRANSPARENT_DRIVER_ID:
            return( sli_se_transparent_cipher_abort(
                        &operation->ctx.sli_se_transparent_ctx ) );
#endif /* SEMAILBOX_PRESENT */
#if defined(CRYPTOACC_PRESENT)
        case SLI_CRYPTOACC_TRANSPARENT_DRIVER_ID:
            return( sli_cryptoacc_transparent_cipher_abort(
                        &operation->ctx.sli_cryptoacc_transparent_ctx ) );
#endif /* CRYPTOACC_PRESENT */
#if defined(CRYPTO_PRESENT)
        case SLI_CRYPTO_TRANSPARENT_DRIVER_ID:
            return( sli_crypto_transparent_cipher_abort(
                        &operation->ctx.sli_crypto_transparent_ctx  ) );
#endif /* CRYPTO_PRESENT */
#if defined(SEMAILBOX_PRESENT) && (_SILICON_LABS_SECURITY_FEATURE == _SILICON_LABS_SECURITY_FEATURE_VAULT)
        case SLI_SE_OPAQUE_DRIVER_ID:
            return( sli_se_opaque_cipher_abort(
                        &operation->ctx.sli_se_opaque_ctx ) );
#endif // VAULT
#endif /* PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT */
    }

    (void)status;
    return( PSA_ERROR_INVALID_ARGUMENT );
}

/*
 * Hashing functions
 */
psa_status_t psa_driver_wrapper_hash_compute(
    psa_algorithm_t alg,
    const uint8_t *input,
    size_t input_length,
    uint8_t *hash,
    size_t hash_size,
    size_t *hash_length)
{
    psa_status_t status = PSA_ERROR_CORRUPTION_DETECTED;

    /* Try accelerators first */
#if defined(PSA_CRYPTO_DRIVER_TEST)
    status = mbedtls_test_transparent_hash_compute(
                alg, input, input_length, hash, hash_size, hash_length );
    if( status != PSA_ERROR_NOT_SUPPORTED )
        return( status );
#endif

#if defined(MBEDTLS_PSA_CRYPTO_DRIVERS)
#if defined(SEMAILBOX_PRESENT)
    status = sli_se_transparent_hash_compute(
                alg, input, input_length, hash, hash_size, hash_length );
    if( status != PSA_ERROR_NOT_SUPPORTED )
        return( status );
#endif /* SEMAILBOX_PRESENT */
#if defined(CRYPTOACC_PRESENT)
    status = sli_cryptoacc_transparent_hash_compute(
                alg, input, input_length, hash, hash_size, hash_length );
    if( status != PSA_ERROR_NOT_SUPPORTED )
        return( status );
#endif /* CRYPTOACC_PRESENT */
#if defined(CRYPTO_PRESENT)
    status = sli_crypto_transparent_hash_compute(
                alg, input, input_length, hash, hash_size, hash_length );
    if( status != PSA_ERROR_NOT_SUPPORTED )
        return( status );
#endif /* CRYPTO_PRESENT */
#endif /* MBEDTLS_PSA_CRYPTO_DRIVERS */

    /* If software fallback is compiled in, try fallback */
#if defined(MBEDTLS_PSA_BUILTIN_HASH)
    status = mbedtls_psa_hash_compute( alg, input, input_length,
                                       hash, hash_size, hash_length );
    if( status != PSA_ERROR_NOT_SUPPORTED )
        return( status );
#endif
    (void) status;
    (void) alg;
    (void) input;
    (void) input_length;
    (void) hash;
    (void) hash_size;
    (void) hash_length;

    return( PSA_ERROR_NOT_SUPPORTED );
}

psa_status_t psa_driver_wrapper_hash_setup(
    psa_hash_operation_t *operation,
    psa_algorithm_t alg )
{
    psa_status_t status = PSA_ERROR_CORRUPTION_DETECTED;

    /* Try setup on accelerators first */
#if defined(PSA_CRYPTO_DRIVER_TEST)
    status = mbedtls_test_transparent_hash_setup(
                &operation->ctx.test_driver_ctx, alg );
    if( status == PSA_SUCCESS )
        operation->id = PSA_CRYPTO_TRANSPARENT_TEST_DRIVER_ID;

    if( status != PSA_ERROR_NOT_SUPPORTED )
        return( status );
#endif

#if defined(MBEDTLS_PSA_CRYPTO_DRIVERS)
#if defined(SEMAILBOX_PRESENT)
    status = sli_se_transparent_hash_setup(
                &operation->ctx.sli_se_transparent_ctx, alg );
    if( status == PSA_SUCCESS )
        operation->id = SLI_SE_TRANSPARENT_DRIVER_ID;

    if( status != PSA_ERROR_NOT_SUPPORTED )
        return( status );
#endif /* SEMAILBOX_PRESENT */
#if defined(CRYPTOACC_PRESENT)
    status = sli_cryptoacc_transparent_hash_setup(
                &operation->ctx.sli_cryptoacc_transparent_ctx, alg );
    if( status == PSA_SUCCESS )
        operation->id = SLI_CRYPTOACC_TRANSPARENT_DRIVER_ID;

    if( status != PSA_ERROR_NOT_SUPPORTED )
        return( status );
#endif /* CRYPTOACC_PRESENT */
#if defined(CRYPTO_PRESENT)
    status = sli_crypto_transparent_hash_setup(
                &operation->ctx.sli_crypto_transparent_ctx, alg );
    if( status == PSA_SUCCESS )
        operation->id = SLI_CRYPTO_TRANSPARENT_DRIVER_ID;

    if( status != PSA_ERROR_NOT_SUPPORTED )
        return( status );
#endif /* CRYPTO_PRESENT */
#endif

    /* If software fallback is compiled in, try fallback */
#if defined(MBEDTLS_PSA_BUILTIN_HASH)
    status = mbedtls_psa_hash_setup( &operation->ctx.mbedtls_ctx, alg );
    if( status == PSA_SUCCESS )
        operation->id = PSA_CRYPTO_MBED_TLS_DRIVER_ID;

    if( status != PSA_ERROR_NOT_SUPPORTED )
        return( status );
#endif
    /* Nothing left to try if we fall through here */
    (void) status;
    (void) operation;
    (void) alg;
    return( PSA_ERROR_NOT_SUPPORTED );
}

psa_status_t psa_driver_wrapper_hash_clone(
    const psa_hash_operation_t *source_operation,
    psa_hash_operation_t *target_operation )
{
    switch( source_operation->id )
    {
#if defined(MBEDTLS_PSA_BUILTIN_HASH)
        case PSA_CRYPTO_MBED_TLS_DRIVER_ID:
            target_operation->id = PSA_CRYPTO_MBED_TLS_DRIVER_ID;
            return( mbedtls_psa_hash_clone( &source_operation->ctx.mbedtls_ctx,
                                            &target_operation->ctx.mbedtls_ctx ) );
#endif
#if defined(PSA_CRYPTO_DRIVER_TEST)
        case PSA_CRYPTO_TRANSPARENT_TEST_DRIVER_ID:
            target_operation->id = PSA_CRYPTO_TRANSPARENT_TEST_DRIVER_ID;
            return( mbedtls_test_transparent_hash_clone(
                        &source_operation->ctx.test_driver_ctx,
                        &target_operation->ctx.test_driver_ctx ) );
#endif
#if defined(MBEDTLS_PSA_CRYPTO_DRIVERS)
#if defined(SEMAILBOX_PRESENT)
        case SLI_SE_TRANSPARENT_DRIVER_ID:
            target_operation->id = SLI_SE_TRANSPARENT_DRIVER_ID;
            return( sli_se_transparent_hash_clone(
                        &source_operation->ctx.sli_se_transparent_ctx,
                        &target_operation->ctx.sli_se_transparent_ctx ) );
#endif /* SEMAILBOX_PRESENT */
#if defined(CRYPTOACC_PRESENT)
        case SLI_CRYPTOACC_TRANSPARENT_DRIVER_ID:
            target_operation->id = SLI_CRYPTOACC_TRANSPARENT_DRIVER_ID;
            return( sli_cryptoacc_transparent_hash_clone(
                        &source_operation->ctx.sli_cryptoacc_transparent_ctx,
                        &target_operation->ctx.sli_cryptoacc_transparent_ctx ) );
#endif /* CRYPTOACC_PRESENT */
#if defined(CRYPTO_PRESENT)
        case SLI_CRYPTO_TRANSPARENT_DRIVER_ID:
            target_operation->id = SLI_CRYPTO_TRANSPARENT_DRIVER_ID;
            return( sli_crypto_transparent_hash_clone(
                        &source_operation->ctx.sli_crypto_transparent_ctx,
                        &target_operation->ctx.sli_crypto_transparent_ctx ) );
#endif /* CRYPTO_PRESENT */
#endif /* MBEDTLS_PSA_CRYPTO_DRIVERS */
        default:
            (void) target_operation;
            return( PSA_ERROR_BAD_STATE );
    }
}

psa_status_t psa_driver_wrapper_hash_update(
    psa_hash_operation_t *operation,
    const uint8_t *input,
    size_t input_length )
{
    switch( operation->id )
    {
#if defined(MBEDTLS_PSA_BUILTIN_HASH)
        case PSA_CRYPTO_MBED_TLS_DRIVER_ID:
            return( mbedtls_psa_hash_update( &operation->ctx.mbedtls_ctx,
                                             input, input_length ) );
#endif
#if defined(PSA_CRYPTO_DRIVER_TEST)
        case PSA_CRYPTO_TRANSPARENT_TEST_DRIVER_ID:
            return( mbedtls_test_transparent_hash_update(
                        &operation->ctx.test_driver_ctx,
                        input, input_length ) );
#endif
#if defined(MBEDTLS_PSA_CRYPTO_DRIVERS)
#if defined(SEMAILBOX_PRESENT)
        case SLI_SE_TRANSPARENT_DRIVER_ID:
            return( sli_se_transparent_hash_update(
                        &operation->ctx.sli_se_transparent_ctx,
                        input, input_length ) );
#endif /* SEMAILBOX_PRESENT */
#if defined(CRYPTOACC_PRESENT)
        case SLI_CRYPTOACC_TRANSPARENT_DRIVER_ID:
            return( sli_cryptoacc_transparent_hash_update(
                        &operation->ctx.sli_cryptoacc_transparent_ctx,
                        input, input_length ) );
#endif /* CRYPTOACC_PRESENT */
#if defined(CRYPTO_PRESENT)
        case SLI_CRYPTO_TRANSPARENT_DRIVER_ID:
            return( sli_crypto_transparent_hash_update(
                        &operation->ctx.sli_crypto_transparent_ctx,
                        input, input_length ) );
#endif /* CRYPTO_PRESENT */
#endif /* MBEDTLS_PSA_CRYPTO_DRIVERS */
        default:
            (void) input;
            (void) input_length;
            return( PSA_ERROR_BAD_STATE );
    }
}

psa_status_t psa_driver_wrapper_hash_finish(
    psa_hash_operation_t *operation,
    uint8_t *hash,
    size_t hash_size,
    size_t *hash_length )
{
    switch( operation->id )
    {
#if defined(MBEDTLS_PSA_BUILTIN_HASH)
        case PSA_CRYPTO_MBED_TLS_DRIVER_ID:
            return( mbedtls_psa_hash_finish( &operation->ctx.mbedtls_ctx,
                                             hash, hash_size, hash_length ) );
#endif
#if defined(PSA_CRYPTO_DRIVER_TEST)
        case PSA_CRYPTO_TRANSPARENT_TEST_DRIVER_ID:
            return( mbedtls_test_transparent_hash_finish(
                        &operation->ctx.test_driver_ctx,
                        hash, hash_size, hash_length ) );
#endif
#if defined(MBEDTLS_PSA_CRYPTO_DRIVERS)
#if defined(SEMAILBOX_PRESENT)
        case SLI_SE_TRANSPARENT_DRIVER_ID:
            return( sli_se_transparent_hash_finish(
                        &operation->ctx.sli_se_transparent_ctx,
                        hash, hash_size, hash_length ) );
#endif /* SEMAILBOX_PRESENT */
#if defined(CRYPTOACC_PRESENT)
        case SLI_CRYPTOACC_TRANSPARENT_DRIVER_ID:
            return( sli_cryptoacc_transparent_hash_finish(
                        &operation->ctx.sli_cryptoacc_transparent_ctx,
                        hash, hash_size, hash_length ) );
#endif /* CRYPTOACC_PRESENT */
#if defined(CRYPTO_PRESENT)
        case SLI_CRYPTO_TRANSPARENT_DRIVER_ID:
            return( sli_crypto_transparent_hash_finish(
                        &operation->ctx.sli_crypto_transparent_ctx,
                        hash, hash_size, hash_length ) );
#endif /* CRYPTO_PRESENT */
#endif /* MBEDTLS_PSA_CRYPTO_DRIVERS */
        default:
            (void) hash;
            (void) hash_size;
            (void) hash_length;
            return( PSA_ERROR_BAD_STATE );
    }
}

psa_status_t psa_driver_wrapper_hash_abort(
    psa_hash_operation_t *operation )
{
    switch( operation->id )
    {
#if defined(MBEDTLS_PSA_BUILTIN_HASH)
        case PSA_CRYPTO_MBED_TLS_DRIVER_ID:
            return( mbedtls_psa_hash_abort( &operation->ctx.mbedtls_ctx ) );
#endif
#if defined(PSA_CRYPTO_DRIVER_TEST)
        case PSA_CRYPTO_TRANSPARENT_TEST_DRIVER_ID:
            return( mbedtls_test_transparent_hash_abort(
                        &operation->ctx.test_driver_ctx ) );
#endif
#if defined(MBEDTLS_PSA_CRYPTO_DRIVERS)
#if defined(SEMAILBOX_PRESENT)
        case SLI_SE_TRANSPARENT_DRIVER_ID:
            return( sli_se_transparent_hash_abort(
                        &operation->ctx.sli_se_transparent_ctx ) );
#endif /* SEMAILBOX_PRESENT */
#if defined(CRYPTOACC_PRESENT)
        case SLI_CRYPTOACC_TRANSPARENT_DRIVER_ID:
            return( sli_cryptoacc_transparent_hash_abort(
                        &operation->ctx.sli_cryptoacc_transparent_ctx ) );
#endif /* CRYPTOACC_PRESENT */
#if defined(CRYPTO_PRESENT)
        case SLI_CRYPTO_TRANSPARENT_DRIVER_ID:
            return( sli_crypto_transparent_hash_abort(
                        &operation->ctx.sli_crypto_transparent_ctx ) );
#endif /* CRYPTO_PRESENT */
#endif /* MBEDTLS_PSA_CRYPTO_DRIVERS */
        default:
            return( PSA_ERROR_BAD_STATE );
    }
}

psa_status_t psa_driver_wrapper_aead_encrypt(
    const psa_key_attributes_t *attributes,
    const uint8_t *key_buffer, size_t key_buffer_size,
    psa_algorithm_t alg,
    const uint8_t *nonce, size_t nonce_length,
    const uint8_t *additional_data, size_t additional_data_length,
    const uint8_t *plaintext, size_t plaintext_length,
    uint8_t *ciphertext, size_t ciphertext_size, size_t *ciphertext_length )
{
    psa_status_t status = PSA_ERROR_CORRUPTION_DETECTED;
    psa_key_location_t location =
        PSA_KEY_LIFETIME_GET_LOCATION( attributes->core.lifetime );

    switch( location )
    {
        case PSA_KEY_LOCATION_LOCAL_STORAGE:
            /* Key is stored in the slot in export representation, so
             * cycle through all known transparent accelerators */

#if defined(PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT)
#if defined(PSA_CRYPTO_DRIVER_TEST)
            status = mbedtls_test_transparent_aead_encrypt(
                         attributes, key_buffer, key_buffer_size,
                         alg,
                         nonce, nonce_length,
                         additional_data, additional_data_length,
                         plaintext, plaintext_length,
                         ciphertext, ciphertext_size, ciphertext_length );
            /* Declared with fallback == true */
            if( status != PSA_ERROR_NOT_SUPPORTED )
                return( status );
#endif /* PSA_CRYPTO_DRIVER_TEST */
#if defined(MBEDTLS_PSA_CRYPTO_DRIVERS)
#if defined(SEMAILBOX_PRESENT)
            status = sli_se_transparent_aead_encrypt(
                        attributes, key_buffer, key_buffer_size,
                        alg,
                        nonce, nonce_length,
                        additional_data, additional_data_length,
                        plaintext, plaintext_length,
                        ciphertext, ciphertext_size, ciphertext_length );
            if( status != PSA_ERROR_NOT_SUPPORTED )
                return( status );
#endif /* SEMAILBOX_PRESENT */
#if defined(CRYPTOACC_PRESENT)
            status = sli_cryptoacc_transparent_aead_encrypt(
                        attributes, key_buffer, key_buffer_size,
                        alg,
                        nonce, nonce_length,
                        additional_data, additional_data_length,
                        plaintext, plaintext_length,
                        ciphertext, ciphertext_size, ciphertext_length );
            if( status != PSA_ERROR_NOT_SUPPORTED )
                return( status );
#endif /* CRYPTOACC_PRESENT */
#if defined(CRYPTO_PRESENT)
            status = sli_crypto_transparent_aead_encrypt(
                        attributes, key_buffer, key_buffer_size,
                        alg,
                        nonce, nonce_length,
                        additional_data, additional_data_length,
                        plaintext, plaintext_length,
                        ciphertext, ciphertext_size, ciphertext_length );
            if( status != PSA_ERROR_NOT_SUPPORTED )
                return( status );
#endif /* CRYPTO_PRESENT */
#endif
#endif /* PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT */

            /* Fell through, meaning no accelerator supports this operation */
            return( mbedtls_psa_aead_encrypt(
                        attributes, key_buffer, key_buffer_size,
                        alg,
                        nonce, nonce_length,
                        additional_data, additional_data_length,
                        plaintext, plaintext_length,
                        ciphertext, ciphertext_size, ciphertext_length ) );

        /* Add cases for opaque driver here */
#if defined(SEMAILBOX_PRESENT) && (_SILICON_LABS_SECURITY_FEATURE == _SILICON_LABS_SECURITY_FEATURE_VAULT) || \
    defined(MBEDTLS_PSA_CRYPTO_BUILTIN_KEYS)
        case PSA_KEY_LOCATION_SLI_SE_OPAQUE:
            return ( sli_se_opaque_aead_encrypt(
                        attributes, key_buffer, key_buffer_size,
                        alg,
                        nonce, nonce_length,
                        additional_data, additional_data_length,
                        plaintext, plaintext_length,
                        ciphertext, ciphertext_size, ciphertext_length ) );
#endif // VAULT
        default:
            /* Key is declared with a lifetime not known to us */
            (void)status;
            return( PSA_ERROR_INVALID_ARGUMENT );
    }
}

psa_status_t psa_driver_wrapper_aead_decrypt(
    const psa_key_attributes_t *attributes,
    const uint8_t *key_buffer, size_t key_buffer_size,
    psa_algorithm_t alg,
    const uint8_t *nonce, size_t nonce_length,
    const uint8_t *additional_data, size_t additional_data_length,
    const uint8_t *ciphertext, size_t ciphertext_length,
    uint8_t *plaintext, size_t plaintext_size, size_t *plaintext_length )
{
    psa_status_t status = PSA_ERROR_CORRUPTION_DETECTED;
    psa_key_location_t location =
        PSA_KEY_LIFETIME_GET_LOCATION( attributes->core.lifetime );

    switch( location )
    {
        case PSA_KEY_LOCATION_LOCAL_STORAGE:
            /* Key is stored in the slot in export representation, so
             * cycle through all known transparent accelerators */

#if defined(PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT)
#if defined(PSA_CRYPTO_DRIVER_TEST)
            status = mbedtls_test_transparent_aead_decrypt(
                        attributes, key_buffer, key_buffer_size,
                        alg,
                        nonce, nonce_length,
                        additional_data, additional_data_length,
                        ciphertext, ciphertext_length,
                        plaintext, plaintext_size, plaintext_length );
            /* Declared with fallback == true */
            if( status != PSA_ERROR_NOT_SUPPORTED )
                return( status );
#endif /* PSA_CRYPTO_DRIVER_TEST */
#if defined(MBEDTLS_PSA_CRYPTO_DRIVERS)
#if defined(SEMAILBOX_PRESENT)
            status = sli_se_transparent_aead_decrypt(
                        attributes, key_buffer, key_buffer_size,
                        alg,
                        nonce, nonce_length,
                        additional_data, additional_data_length,
                        ciphertext, ciphertext_length,
                        plaintext, plaintext_size, plaintext_length );
            if( status != PSA_ERROR_NOT_SUPPORTED )
                return( status );
#endif /* SEMAILBOX_PRESENT */
#if defined(CRYPTOACC_PRESENT)
            status = sli_cryptoacc_transparent_aead_decrypt(
                        attributes, key_buffer, key_buffer_size,
                        alg,
                        nonce, nonce_length,
                        additional_data, additional_data_length,
                        ciphertext, ciphertext_length,
                        plaintext, plaintext_size, plaintext_length );
            if( status != PSA_ERROR_NOT_SUPPORTED )
                return( status );
#endif /* CRYPTOACC_PRESENT */
#if defined(CRYPTO_PRESENT)
            status = sli_crypto_transparent_aead_decrypt(
                        attributes, key_buffer, key_buffer_size,
                        alg,
                        nonce, nonce_length,
                        additional_data, additional_data_length,
                        ciphertext, ciphertext_length,
                        plaintext, plaintext_size, plaintext_length );
            if( status != PSA_ERROR_NOT_SUPPORTED )
                return( status );
#endif /* CRYPTO_PRESENT */
#endif
#endif /* PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT */

            /* Fell through, meaning no accelerator supports this operation */
            return( mbedtls_psa_aead_decrypt(
                        attributes, key_buffer, key_buffer_size,
                        alg,
                        nonce, nonce_length,
                        additional_data, additional_data_length,
                        ciphertext, ciphertext_length,
                        plaintext, plaintext_size, plaintext_length ) );

        /* Add cases for opaque driver here */
#if defined(SEMAILBOX_PRESENT) && (_SILICON_LABS_SECURITY_FEATURE == _SILICON_LABS_SECURITY_FEATURE_VAULT) || \
    defined(MBEDTLS_PSA_CRYPTO_BUILTIN_KEYS)
        case PSA_KEY_LOCATION_SLI_SE_OPAQUE:
            return ( sli_se_opaque_aead_decrypt(
                        attributes, key_buffer, key_buffer_size,
                        alg,
                        nonce, nonce_length,
                        additional_data, additional_data_length,
                        ciphertext, ciphertext_length,
                        plaintext, plaintext_size, plaintext_length ) );
#endif // VAULT
        default:
            /* Key is declared with a lifetime not known to us */
            (void)status;
            return( PSA_ERROR_INVALID_ARGUMENT );
    }
}


/*
 * MAC functions
 */
psa_status_t psa_driver_wrapper_mac_compute(
    const psa_key_attributes_t *attributes,
    const uint8_t *key_buffer,
    size_t key_buffer_size,
    psa_algorithm_t alg,
    const uint8_t *input,
    size_t input_length,
    uint8_t *mac,
    size_t mac_size,
    size_t *mac_length )
{
    psa_status_t status = PSA_ERROR_CORRUPTION_DETECTED;
    psa_key_location_t location =
        PSA_KEY_LIFETIME_GET_LOCATION( attributes->core.lifetime );

    switch( location )
    {
        case PSA_KEY_LOCATION_LOCAL_STORAGE:
            /* Key is stored in the slot in export representation, so
             * cycle through all known transparent accelerators */
#if defined(PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT)
#if defined(PSA_CRYPTO_DRIVER_TEST)
            status = mbedtls_test_transparent_mac_compute(
                attributes, key_buffer, key_buffer_size, alg,
                input, input_length,
                mac, mac_size, mac_length );
            /* Declared with fallback == true */
            if( status != PSA_ERROR_NOT_SUPPORTED )
                return( status );
#endif /* PSA_CRYPTO_DRIVER_TEST */
#if defined(MBEDTLS_PSA_CRYPTO_DRIVERS)
#if defined(SEMAILBOX_PRESENT)
            status = sli_se_transparent_mac_compute(
                        attributes, key_buffer, key_buffer_size, alg,
                        input, input_length,
                        mac, mac_size, mac_length );
            if( status != PSA_ERROR_NOT_SUPPORTED )
                return( status );
#endif /* SEMAILBOX_PRESENT */
#if defined(CRYPTOACC_PRESENT)
            status = sli_cryptoacc_transparent_mac_compute(
                        attributes, key_buffer, key_buffer_size, alg,
                        input, input_length,
                        mac, mac_size, mac_length );
            if( status != PSA_ERROR_NOT_SUPPORTED )
                return( status );
#endif /* CRYPTOACC_PRESENT */
#if defined(CRYPTO_PRESENT)
            status = sli_crypto_transparent_mac_compute(
                        attributes, key_buffer, key_buffer_size, alg,
                        input, input_length,
                        mac, mac_size, mac_length );
            if( status != PSA_ERROR_NOT_SUPPORTED )
                return( status );
#endif /* CRYPTO_PRESENT */
#endif
#endif /* PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT */
#if defined(MBEDTLS_PSA_BUILTIN_MAC)
            /* Fell through, meaning no accelerator supports this operation */
            status = mbedtls_psa_mac_compute(
                attributes, key_buffer, key_buffer_size, alg,
                input, input_length,
                mac, mac_size, mac_length );
            if( status != PSA_ERROR_NOT_SUPPORTED )
                return( status );
#endif /* MBEDTLS_PSA_BUILTIN_MAC */
            return( PSA_ERROR_NOT_SUPPORTED );

        /* Add cases for opaque driver here */
#if defined(PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT)
#if defined(PSA_CRYPTO_DRIVER_TEST)
        case PSA_CRYPTO_TEST_DRIVER_LOCATION:
            status = mbedtls_test_opaque_mac_compute(
                attributes, key_buffer, key_buffer_size, alg,
                input, input_length,
                mac, mac_size, mac_length );
            return( status );
#endif /* PSA_CRYPTO_DRIVER_TEST */
#endif /* PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT */
        default:
            /* Key is declared with a lifetime not known to us */
            (void) key_buffer;
            (void) key_buffer_size;
            (void) alg;
            (void) input;
            (void) input_length;
            (void) mac;
            (void) mac_size;
            (void) mac_length;
            (void) status;
            return( PSA_ERROR_INVALID_ARGUMENT );
    }
}

psa_status_t psa_driver_wrapper_mac_sign_setup(
    psa_mac_operation_t *operation,
    const psa_key_attributes_t *attributes,
    const uint8_t *key_buffer,
    size_t key_buffer_size,
    psa_algorithm_t alg )
{
    psa_status_t status = PSA_ERROR_CORRUPTION_DETECTED;
    psa_key_location_t location =
        PSA_KEY_LIFETIME_GET_LOCATION( attributes->core.lifetime );

    switch( location )
    {
        case PSA_KEY_LOCATION_LOCAL_STORAGE:
            /* Key is stored in the slot in export representation, so
             * cycle through all known transparent accelerators */
#if defined(PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT)
#if defined(PSA_CRYPTO_DRIVER_TEST)
            status = mbedtls_test_transparent_mac_sign_setup(
                &operation->ctx.transparent_test_driver_ctx,
                attributes,
                key_buffer, key_buffer_size,
                alg );
            /* Declared with fallback == true */
            if( status == PSA_SUCCESS )
                operation->id = PSA_CRYPTO_TRANSPARENT_TEST_DRIVER_ID;

            if( status != PSA_ERROR_NOT_SUPPORTED )
                return( status );
#endif /* PSA_CRYPTO_DRIVER_TEST */
#if defined(MBEDTLS_PSA_CRYPTO_DRIVERS)
#if defined(SEMAILBOX_PRESENT)
            status = sli_se_transparent_mac_sign_setup(
                &operation->ctx.sli_se_transparent_ctx,
                attributes,
                key_buffer, key_buffer_size,
                alg );
            /* Declared with fallback == true */
            if( status == PSA_SUCCESS )
                operation->id = SLI_SE_TRANSPARENT_DRIVER_ID;

            if( status != PSA_ERROR_NOT_SUPPORTED )
                return( status );
#endif /* SEMAILBOX_PRESENT */
#if defined(CRYPTOACC_PRESENT)
            status = sli_cryptoacc_transparent_mac_sign_setup(
                &operation->ctx.sli_cryptoacc_transparent_ctx,
                attributes,
                key_buffer, key_buffer_size,
                alg );
            /* Declared with fallback == true */
            if( status == PSA_SUCCESS )
                operation->id = SLI_CRYPTOACC_TRANSPARENT_DRIVER_ID;

            if( status != PSA_ERROR_NOT_SUPPORTED )
                return( status );
#endif /* CRYPTOACC_PRESENT */
#if defined(CRYPTO_PRESENT)
            status = sli_crypto_transparent_mac_sign_setup(
                &operation->ctx.sli_crypto_transparent_ctx,
                attributes,
                key_buffer, key_buffer_size,
                alg );
            /* Declared with fallback == true */
            if( status == PSA_SUCCESS )
                operation->id = SLI_CRYPTO_TRANSPARENT_DRIVER_ID;

            if( status != PSA_ERROR_NOT_SUPPORTED )
                return( status );
#endif /* CRYPTO_PRESENT */
#endif
#endif /* PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT */
#if defined(MBEDTLS_PSA_BUILTIN_MAC)
            /* Fell through, meaning no accelerator supports this operation */
            status = mbedtls_psa_mac_sign_setup( &operation->ctx.mbedtls_ctx,
                                                 attributes,
                                                 key_buffer, key_buffer_size,
                                                 alg );
            if( status == PSA_SUCCESS )
                operation->id = PSA_CRYPTO_MBED_TLS_DRIVER_ID;

            if( status != PSA_ERROR_NOT_SUPPORTED )
                return( status );
#endif /* MBEDTLS_PSA_BUILTIN_MAC */
            return( PSA_ERROR_NOT_SUPPORTED );

        /* Add cases for opaque driver here */
#if defined(PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT)
#if defined(PSA_CRYPTO_DRIVER_TEST)
        case PSA_CRYPTO_TEST_DRIVER_LOCATION:
            status = mbedtls_test_opaque_mac_sign_setup(
                &operation->ctx.opaque_test_driver_ctx,
                attributes,
                key_buffer, key_buffer_size,
                alg );

            if( status == PSA_SUCCESS )
                operation->id = PSA_CRYPTO_OPAQUE_TEST_DRIVER_ID;

            return( status );
#endif /* PSA_CRYPTO_DRIVER_TEST */
#if defined(SEMAILBOX_PRESENT) && (_SILICON_LABS_SECURITY_FEATURE == _SILICON_LABS_SECURITY_FEATURE_VAULT) || \
    defined(MBEDTLS_PSA_CRYPTO_BUILTIN_KEYS)
        case PSA_KEY_LOCATION_SLI_SE_OPAQUE:
            status = sli_se_opaque_mac_sign_setup(
                        &operation->ctx.sli_se_opaque_ctx,
                        attributes,
                        key_buffer, key_buffer_size,
                        alg );
            if( status == PSA_SUCCESS )
                operation->id = SLI_SE_OPAQUE_DRIVER_ID;
            return( status );
#endif // VAULT
#endif /* PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT */
        default:
            /* Key is declared with a lifetime not known to us */
            (void) operation;
            (void) status;
            (void) key_buffer;
            (void) key_buffer_size;
            (void) alg;
            return( PSA_ERROR_INVALID_ARGUMENT );
    }
}

psa_status_t psa_driver_wrapper_mac_verify_setup(
    psa_mac_operation_t *operation,
    const psa_key_attributes_t *attributes,
    const uint8_t *key_buffer,
    size_t key_buffer_size,
    psa_algorithm_t alg )
{
    psa_status_t status = PSA_ERROR_CORRUPTION_DETECTED;
    psa_key_location_t location =
        PSA_KEY_LIFETIME_GET_LOCATION( attributes->core.lifetime );

    switch( location )
    {
        case PSA_KEY_LOCATION_LOCAL_STORAGE:
            /* Key is stored in the slot in export representation, so
             * cycle through all known transparent accelerators */
#if defined(PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT)
#if defined(PSA_CRYPTO_DRIVER_TEST)
            status = mbedtls_test_transparent_mac_verify_setup(
                &operation->ctx.transparent_test_driver_ctx,
                attributes,
                key_buffer, key_buffer_size,
                alg );
            /* Declared with fallback == true */
            if( status == PSA_SUCCESS )
                operation->id = PSA_CRYPTO_TRANSPARENT_TEST_DRIVER_ID;

            if( status != PSA_ERROR_NOT_SUPPORTED )
                return( status );
#endif /* PSA_CRYPTO_DRIVER_TEST */
#if defined(MBEDTLS_PSA_CRYPTO_DRIVERS)
#if defined(SEMAILBOX_PRESENT)
            status = sli_se_transparent_mac_verify_setup(
                &operation->ctx.sli_se_transparent_ctx,
                attributes,
                key_buffer, key_buffer_size,
                alg );
            /* Declared with fallback == true */
            if( status == PSA_SUCCESS )
                operation->id = SLI_SE_TRANSPARENT_DRIVER_ID;

            if( status != PSA_ERROR_NOT_SUPPORTED )
                return( status );
#endif /* SEMAILBOX_PRESENT */
#if defined(CRYPTOACC_PRESENT)
            status = sli_cryptoacc_transparent_mac_verify_setup(
                &operation->ctx.sli_cryptoacc_transparent_ctx,
                attributes,
                key_buffer, key_buffer_size,
                alg );
            /* Declared with fallback == true */
            if( status == PSA_SUCCESS )
                operation->id = SLI_CRYPTOACC_TRANSPARENT_DRIVER_ID;

            if( status != PSA_ERROR_NOT_SUPPORTED )
                return( status );
#endif /* CRYPTOACC_PRESENT */
#if defined(CRYPTO_PRESENT)
            status = sli_crypto_transparent_mac_verify_setup(
                &operation->ctx.sli_crypto_transparent_ctx,
                attributes,
                key_buffer, key_buffer_size,
                alg );
            /* Declared with fallback == true */
            if( status == PSA_SUCCESS )
                operation->id = SLI_CRYPTO_TRANSPARENT_DRIVER_ID;

            if( status != PSA_ERROR_NOT_SUPPORTED )
                return( status );
#endif /* CRYPTO_PRESENT */
#endif
#endif /* PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT */
#if defined(MBEDTLS_PSA_BUILTIN_MAC)
            /* Fell through, meaning no accelerator supports this operation */
            status = mbedtls_psa_mac_verify_setup( &operation->ctx.mbedtls_ctx,
                                                   attributes,
                                                   key_buffer, key_buffer_size,
                                                   alg );
            if( status == PSA_SUCCESS )
                operation->id = PSA_CRYPTO_MBED_TLS_DRIVER_ID;

            if( status != PSA_ERROR_NOT_SUPPORTED )
                return( status );
#endif /* MBEDTLS_PSA_BUILTIN_MAC */
            return( PSA_ERROR_NOT_SUPPORTED );

        /* Add cases for opaque driver here */
#if defined(PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT)
#if defined(PSA_CRYPTO_DRIVER_TEST)
        case PSA_CRYPTO_TEST_DRIVER_LOCATION:
            status = mbedtls_test_opaque_mac_verify_setup(
                &operation->ctx.opaque_test_driver_ctx,
                attributes,
                key_buffer, key_buffer_size,
                alg );

            if( status == PSA_SUCCESS )
                operation->id = PSA_CRYPTO_OPAQUE_TEST_DRIVER_ID;

            return( status );
#endif /* PSA_CRYPTO_DRIVER_TEST */
#if defined(SEMAILBOX_PRESENT) && (_SILICON_LABS_SECURITY_FEATURE == _SILICON_LABS_SECURITY_FEATURE_VAULT) || \
    defined(MBEDTLS_PSA_CRYPTO_BUILTIN_KEYS)
        case PSA_KEY_LOCATION_SLI_SE_OPAQUE:
            status = sli_se_opaque_mac_verify_setup(
                        &operation->ctx.sli_se_opaque_ctx,
                        attributes,
                        key_buffer, key_buffer_size,
                        alg );
            if( status == PSA_SUCCESS )
                operation->id = SLI_SE_OPAQUE_DRIVER_ID;
            return( status );
#endif // VAULT
#endif /* PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT */
        default:
            /* Key is declared with a lifetime not known to us */
            (void) operation;
            (void) status;
            (void) key_buffer;
            (void) key_buffer_size;
            (void) alg;
            return( PSA_ERROR_INVALID_ARGUMENT );
    }
}

psa_status_t psa_driver_wrapper_mac_update(
    psa_mac_operation_t *operation,
    const uint8_t *input,
    size_t input_length )
{
    switch( operation->id )
    {
#if defined(MBEDTLS_PSA_BUILTIN_MAC)
        case PSA_CRYPTO_MBED_TLS_DRIVER_ID:
            return( mbedtls_psa_mac_update( &operation->ctx.mbedtls_ctx,
                                            input, input_length ) );
#endif /* MBEDTLS_PSA_BUILTIN_MAC */

#if defined(PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT)
#if defined(PSA_CRYPTO_DRIVER_TEST)
        case PSA_CRYPTO_TRANSPARENT_TEST_DRIVER_ID:
            return( mbedtls_test_transparent_mac_update(
                        &operation->ctx.transparent_test_driver_ctx,
                        input, input_length ) );

        case PSA_CRYPTO_OPAQUE_TEST_DRIVER_ID:
            return( mbedtls_test_opaque_mac_update(
                        &operation->ctx.opaque_test_driver_ctx,
                        input, input_length ) );
#endif /* PSA_CRYPTO_DRIVER_TEST */
#if defined(SEMAILBOX_PRESENT)
        case SLI_SE_TRANSPARENT_DRIVER_ID:
            return( sli_se_transparent_mac_update(
                        &operation->ctx.sli_se_transparent_ctx,
                        input, input_length ) );
#endif /* SEMAILBOX_PRESENT */
#if defined(CRYPTOACC_PRESENT)
        case SLI_CRYPTOACC_TRANSPARENT_DRIVER_ID:
            return( sli_cryptoacc_transparent_mac_update(
                        &operation->ctx.sli_cryptoacc_transparent_ctx,
                        input, input_length ) );
#endif /* CRYPTOACC_PRESENT */
#if defined(CRYPTO_PRESENT)
        case SLI_CRYPTO_TRANSPARENT_DRIVER_ID:
            return( sli_crypto_transparent_mac_update(
                        &operation->ctx.sli_crypto_transparent_ctx,
                        input, input_length ) );
#endif /* CRYPTO_PRESENT */
#if defined(SEMAILBOX_PRESENT) && (_SILICON_LABS_SECURITY_FEATURE == _SILICON_LABS_SECURITY_FEATURE_VAULT) || \
    defined(MBEDTLS_PSA_CRYPTO_BUILTIN_KEYS)
        case SLI_SE_OPAQUE_DRIVER_ID:
            return( sli_se_opaque_mac_update(
                        &operation->ctx.sli_se_opaque_ctx,
                        input, input_length ) );
#endif // VAULT
#endif /* PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT */
        default:
            (void) input;
            (void) input_length;
            return( PSA_ERROR_INVALID_ARGUMENT );
    }
}

psa_status_t psa_driver_wrapper_mac_sign_finish(
    psa_mac_operation_t *operation,
    uint8_t *mac,
    size_t mac_size,
    size_t *mac_length )
{
    switch( operation->id )
    {
#if defined(MBEDTLS_PSA_BUILTIN_MAC)
        case PSA_CRYPTO_MBED_TLS_DRIVER_ID:
            return( mbedtls_psa_mac_sign_finish( &operation->ctx.mbedtls_ctx,
                                                 mac, mac_size, mac_length ) );
#endif /* MBEDTLS_PSA_BUILTIN_MAC */

#if defined(PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT)
#if defined(PSA_CRYPTO_DRIVER_TEST)
        case PSA_CRYPTO_TRANSPARENT_TEST_DRIVER_ID:
            return( mbedtls_test_transparent_mac_sign_finish(
                        &operation->ctx.transparent_test_driver_ctx,
                        mac, mac_size, mac_length ) );

        case PSA_CRYPTO_OPAQUE_TEST_DRIVER_ID:
            return( mbedtls_test_opaque_mac_sign_finish(
                        &operation->ctx.opaque_test_driver_ctx,
                        mac, mac_size, mac_length ) );
#endif /* PSA_CRYPTO_DRIVER_TEST */
#if defined(SEMAILBOX_PRESENT)
        case SLI_SE_TRANSPARENT_DRIVER_ID:
            return( sli_se_transparent_mac_sign_finish(
                        &operation->ctx.sli_se_transparent_ctx,
                        mac, mac_size, mac_length ) );
#endif /* SEMAILBOX_PRESENT */
#if defined(CRYPTOACC_PRESENT)
        case SLI_CRYPTOACC_TRANSPARENT_DRIVER_ID:
            return( sli_cryptoacc_transparent_mac_sign_finish(
                        &operation->ctx.sli_cryptoacc_transparent_ctx,
                        mac, mac_size, mac_length ) );
#endif /* CRYPTOACC_PRESENT */
#if defined(CRYPTO_PRESENT)
        case SLI_CRYPTO_TRANSPARENT_DRIVER_ID:
            return( sli_crypto_transparent_mac_sign_finish(
                        &operation->ctx.sli_crypto_transparent_ctx,
                        mac, mac_size, mac_length ) );
#endif /* CRYPTO_PRESENT */
#if defined(SEMAILBOX_PRESENT) && (_SILICON_LABS_SECURITY_FEATURE == _SILICON_LABS_SECURITY_FEATURE_VAULT) || \
    defined(MBEDTLS_PSA_CRYPTO_BUILTIN_KEYS)
        case SLI_SE_OPAQUE_DRIVER_ID:
            return( sli_se_opaque_mac_sign_finish(
                        &operation->ctx.sli_se_opaque_ctx,
                        mac, mac_size, mac_length ) );
#endif // VAULT
#endif /* PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT */
        default:
            (void) mac;
            (void) mac_size;
            (void) mac_length;
            return( PSA_ERROR_INVALID_ARGUMENT );
    }
}

psa_status_t psa_driver_wrapper_mac_verify_finish(
    psa_mac_operation_t *operation,
    const uint8_t *mac,
    size_t mac_length )
{
    switch( operation->id )
    {
#if defined(MBEDTLS_PSA_BUILTIN_MAC)
        case PSA_CRYPTO_MBED_TLS_DRIVER_ID:
            return( mbedtls_psa_mac_verify_finish( &operation->ctx.mbedtls_ctx,
                                                   mac, mac_length ) );
#endif /* MBEDTLS_PSA_BUILTIN_MAC */

#if defined(PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT)
#if defined(PSA_CRYPTO_DRIVER_TEST)
        case PSA_CRYPTO_TRANSPARENT_TEST_DRIVER_ID:
            return( mbedtls_test_transparent_mac_verify_finish(
                        &operation->ctx.transparent_test_driver_ctx,
                        mac, mac_length ) );

        case PSA_CRYPTO_OPAQUE_TEST_DRIVER_ID:
            return( mbedtls_test_opaque_mac_verify_finish(
                        &operation->ctx.opaque_test_driver_ctx,
                        mac, mac_length ) );
#endif /* PSA_CRYPTO_DRIVER_TEST */
#if defined(SEMAILBOX_PRESENT)
        case SLI_SE_TRANSPARENT_DRIVER_ID:
            return( sli_se_transparent_mac_verify_finish(
                        &operation->ctx.sli_se_transparent_ctx,
                        mac, mac_length ) );
#endif /* SEMAILBOX_PRESENT */
#if defined(CRYPTOACC_PRESENT)
        case SLI_CRYPTOACC_TRANSPARENT_DRIVER_ID:
            return( sli_cryptoacc_transparent_mac_verify_finish(
                        &operation->ctx.sli_cryptoacc_transparent_ctx,
                        mac, mac_length ) );
#endif /* CRYPTOACC_PRESENT */
#if defined(CRYPTO_PRESENT)
        case SLI_CRYPTO_TRANSPARENT_DRIVER_ID:
            return( sli_crypto_transparent_mac_verify_finish(
                        &operation->ctx.sli_crypto_transparent_ctx,
                        mac, mac_length ) );
#endif /* CRYPTO_PRESENT */
#if defined(SEMAILBOX_PRESENT) && (_SILICON_LABS_SECURITY_FEATURE == _SILICON_LABS_SECURITY_FEATURE_VAULT) || \
    defined(MBEDTLS_PSA_CRYPTO_BUILTIN_KEYS)
        case SLI_SE_OPAQUE_DRIVER_ID:
            return( sli_se_opaque_mac_verify_finish(
                        &operation->ctx.sli_se_opaque_ctx,
                        mac, mac_length ) );
#endif // VAULT
#endif /* PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT */
        default:
            (void) mac;
            (void) mac_length;
            return( PSA_ERROR_INVALID_ARGUMENT );
    }
}

psa_status_t psa_driver_wrapper_mac_abort(
    psa_mac_operation_t *operation )
{
    switch( operation->id )
    {
#if defined(MBEDTLS_PSA_BUILTIN_MAC)
        case PSA_CRYPTO_MBED_TLS_DRIVER_ID:
            return( mbedtls_psa_mac_abort( &operation->ctx.mbedtls_ctx ) );
#endif /* MBEDTLS_PSA_BUILTIN_MAC */

#if defined(PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT)
#if defined(PSA_CRYPTO_DRIVER_TEST)
        case PSA_CRYPTO_TRANSPARENT_TEST_DRIVER_ID:
            return( mbedtls_test_transparent_mac_abort(
                        &operation->ctx.transparent_test_driver_ctx ) );
        case PSA_CRYPTO_OPAQUE_TEST_DRIVER_ID:
            return( mbedtls_test_opaque_mac_abort(
                        &operation->ctx.opaque_test_driver_ctx ) );
#endif /* PSA_CRYPTO_DRIVER_TEST */
#if defined(SEMAILBOX_PRESENT)
        case SLI_SE_TRANSPARENT_DRIVER_ID:
            return( sli_se_transparent_mac_abort(
                        &operation->ctx.sli_se_transparent_ctx ) );
#endif /* SEMAILBOX_PRESENT */
#if defined(CRYPTOACC_PRESENT)
        case SLI_CRYPTOACC_TRANSPARENT_DRIVER_ID:
            return( sli_cryptoacc_transparent_mac_abort(
                        &operation->ctx.sli_cryptoacc_transparent_ctx ) );
#endif /* CRYPTOACC_PRESENT */
#if defined(CRYPTO_PRESENT)
        case SLI_CRYPTO_TRANSPARENT_DRIVER_ID:
            return( sli_crypto_transparent_mac_abort(
                        &operation->ctx.sli_crypto_transparent_ctx ) );
#endif /* CRYPTO_PRESENT */
#if defined(SEMAILBOX_PRESENT) && (_SILICON_LABS_SECURITY_FEATURE == _SILICON_LABS_SECURITY_FEATURE_VAULT) || \
    defined(MBEDTLS_PSA_CRYPTO_BUILTIN_KEYS)
        case SLI_SE_OPAQUE_DRIVER_ID:
            return( sli_se_opaque_mac_abort(
                        &operation->ctx.sli_se_opaque_ctx ) );
#endif // VAULT
#endif /* PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT */
        default:
            return( PSA_ERROR_INVALID_ARGUMENT );
    }
}

/*
 * Key agreement functions
 */
psa_status_t psa_driver_wrapper_key_agreement(
    psa_algorithm_t alg,
    psa_key_slot_t *private_key,
    const uint8_t *peer_key,
    size_t peer_key_length,
    uint8_t *shared_secret,
    size_t shared_secret_size,
    size_t *shared_secret_length )
{
    (void) alg;
    (void) private_key;
    (void) peer_key;
    (void) peer_key_length;
    (void) shared_secret;
    (void) shared_secret_size;
    (void) shared_secret_length;
#if defined(PSA_CRYPTO_DRIVER_PRESENT) && defined(PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT)
    psa_status_t status = PSA_ERROR_INVALID_ARGUMENT;
    psa_key_location_t location = PSA_KEY_LIFETIME_GET_LOCATION(private_key->attr.lifetime);
    psa_key_attributes_t attributes = {
      .core = private_key->attr
    };
    (void) attributes;
    switch( location )
    {
        case PSA_KEY_LOCATION_LOCAL_STORAGE:
            /* Key is stored in the slot in export representation, so
             * cycle through all known transparent accelerators */
#if defined(SEMAILBOX_PRESENT)
            status = sli_se_transparent_key_agreement( alg,
                                                       &attributes,
                                                       private_key->key.data,
                                                       private_key->key.bytes,
                                                       peer_key,
                                                       peer_key_length,
                                                       shared_secret,
                                                       shared_secret_size,
                                                       shared_secret_length );
            /* Declared with fallback == true */
            if( status != PSA_ERROR_NOT_SUPPORTED )
                return( status );
#endif /* SEMAILBOX_PRESENT */
#if defined(CRYPTOACC_PRESENT)
            status = sli_cryptoacc_transparent_key_agreement( alg,
                                                              &attributes,
                                                              private_key->key.data,
                                                              private_key->key.bytes,
                                                              peer_key,
                                                              peer_key_length,
                                                              shared_secret,
                                                              shared_secret_size,
                                                              shared_secret_length );
            /* Declared with fallback == true */
            if( status != PSA_ERROR_NOT_SUPPORTED )
                return( status );
#endif /* CRYPTOACC_PRESENT */
            /* Fell through, meaning no accelerator supports this operation */
            return( PSA_ERROR_NOT_SUPPORTED );
#if defined(SEMAILBOX_PRESENT) && \
  ( (_SILICON_LABS_SECURITY_FEATURE == _SILICON_LABS_SECURITY_FEATURE_VAULT) || \
    defined(MBEDTLS_PSA_CRYPTO_BUILTIN_KEYS) )
        case PSA_KEY_LOCATION_SLI_SE_OPAQUE:
            status = sli_se_opaque_key_agreement( alg,
                                                  &attributes,
                                                  private_key->key.data,
                                                  private_key->key.bytes,
                                                  peer_key,
                                                  peer_key_length,
                                                  shared_secret,
                                                  shared_secret_size,
                                                  shared_secret_length );
            // Cannot have fallback for opaque drivers:
            return ( status );
#endif /* SEMAILBOX_PRESENT && VAULT */
        default:
            /* Key is declared with a lifetime not known to us */
            return( status );
    }
#else
    return( PSA_ERROR_NOT_SUPPORTED );
#endif /* PSA_CRYPTO_DRIVER_PRESENT && PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT */
}
/*
 * Asymmetric functions
 */
psa_status_t psa_driver_wrapper_asymmetric_encrypt(
    psa_key_slot_t *slot,
    psa_algorithm_t alg,
    const uint8_t *input,
    size_t input_length,
    const uint8_t *salt,
    size_t salt_length,
    uint8_t *output,
    size_t output_size,
    size_t *output_length )
{
    (void) slot;
    (void) alg;
    (void) input;
    (void) input_length;
    (void) salt;
    (void) salt_length;
    (void) output;
    (void) output_size;
    (void) output_length;
#if defined(PSA_CRYPTO_DRIVER_PRESENT) && defined(PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT)
    return( PSA_ERROR_NOT_SUPPORTED );
#else
    return( PSA_ERROR_NOT_SUPPORTED );
#endif /* PSA_CRYPTO_DRIVER_PRESENT && PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT */
}
psa_status_t psa_driver_wrapper_asymmetric_decrypt(
    psa_key_slot_t *slot,
    psa_algorithm_t alg,
    const uint8_t *input,
    size_t input_length,
    const uint8_t *salt,
    size_t salt_length,
    uint8_t *output,
    size_t output_size,
    size_t *output_length )
{
    (void) slot;
    (void) alg;
    (void) input;
    (void) input_length;
    (void) salt;
    (void) salt_length;
    (void) output;
    (void) output_size;
    (void) output_length;
#if defined(PSA_CRYPTO_DRIVER_PRESENT) && defined(PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT)
    return( PSA_ERROR_NOT_SUPPORTED );
#else
    return( PSA_ERROR_NOT_SUPPORTED );
#endif /* PSA_CRYPTO_DRIVER_PRESENT && PSA_CRYPTO_ACCELERATOR_DRIVER_PRESENT */
}

#endif /* MBEDTLS_PSA_CRYPTO_C */
/* End of automatically generated file. */
