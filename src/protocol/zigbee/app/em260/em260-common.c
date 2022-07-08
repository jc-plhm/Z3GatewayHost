/***************************************************************************//**
 * @file
 * @brief Public common code for NCP applications.
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

#include PLATFORM_HEADER

#include "stack/include/ember.h"

#include "hal/hal.h"

#include "app/util/ezsp/ezsp-enum.h"
#include "app/util/ezsp/ezsp-frame-utilities.h"

#define DEFAULT_TC_POLICY (EZSP_DECISION_ALLOW_JOINS | EZSP_DECISION_ALLOW_UNSECURED_REJOINS)
extern uint8_t emNumMultiPanForks;
extern uint16_t trustCenterPolicies[];

//------------------------------------------------------------------------------
// Maximum configuration values.

// Certain configuration values cannot be set to more than the size of the
// stack token for that value. We define these values here so that we can use
// the compile time token size.
const uint8_t emberNcpBindingTableTokenSize = EMBER_BINDING_TABLE_TOKEN_SIZE;
const uint8_t emberNcpChildTableTokenSize = EMBER_CHILD_TABLE_TOKEN_SIZE;
const uint8_t emberNcpKeyTableTokenSize = EMBER_KEY_TABLE_TOKEN_SIZE;
const uint8_t emberNcpNeighborTableTokenSize = EMBER_MAX_NEIGHBOR_TABLE_SIZE;

//------------------------------------------------------------------------------
// Common APIs

void emXncpInit(void)
{
#ifdef EMBER_MULTI_NETWORK_STRIPPED
  trustCenterPolicies[0] = DEFAULT_TC_POLICY;
#else // EMBER_MULTI_NETWORK_STRIPPED
  for (uint8_t i = 0; i < emNumMultiPanForks; i++) {
    trustCenterPolicies[i] = DEFAULT_TC_POLICY;
  }
#endif  // EMBER_MULTI_NETWORK_STRIPPED
}

//------------------------------------------------------------------------------
// GPIO configuration.

void emProcessSetGpioCurrentConfiguration(void)
{
  appendInt8u(EZSP_ERROR_INVALID_CALL);
}

void emProcessSetGpioPowerUpDownConfiguration(void)
{
  appendInt8u(EZSP_ERROR_INVALID_CALL);
}

void emProcessSetGpioRadioPowerMask(void)
{
  appendInt8u(EZSP_ERROR_INVALID_CALL);
}

// *****************************************
// Convenience Stubs
// *****************************************

#ifndef __NCP_CONFIG__

#ifndef EMBER_APPLICATION_HAS_SET_OR_GET_EZSP_TOKEN_HANDLER
void emberSetOrGetEzspTokenCommandHandler(bool isSet)
{
  appendInt8u(EMBER_INVALID_CALL);
}
#endif

#endif /* __NCP_CONFIG__ */
