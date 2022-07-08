/***************************************************************************//**
 * @file
 * @brief Routines to interfact with the BLE stack.
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
#ifdef EMBER_AF_NCP
#include "app/framework/include/af-ncp.h"
#else // !EMBER_AF_NCP
#include "app/framework/include/af.h"
#endif // EMBER_AF_NCP
#include "sl_bt_rtos_config.h"
#include "sl_bt_rtos_adaptation.h"
#include "sl_bt_ll_config.h"
#include "sl_bt_api.h"
#include "sl_bluetooth_config.h"
#include "sl_bluetooth.h"
#include "gatt_db.h"
#include "hal-config.h"

#if defined(EMBER_AF_PLUGIN_BLE_PSSTORE_LIBRARY) || defined (EMBER_AF_PLUGIN_SIM_EEPROM2_TO_NVM3_UPGRADE_RESERVE_PSSTORE)
// This is needed in order to properly place the PSStore space at the top of
// flash (PSStore is not relocatable, so it needs to be at the top of flash).
VAR_AT_SEGMENT(NO_STRIPPING uint8_t psStore[FLASH_PAGE_SIZE * 2], __PSSTORE__);
#endif

void BluetoothLLCallback();
void BluetoothUpdate();

void emberAfPluginBleGetConfig(sl_bt_configuration_t* config)
{
  emberAfPluginBleGetConfigCallback(config);

  // Re-assigning the heap-related fields, just in case.
  config->bluetooth.max_connections = SL_BT_CONFIG_MAX_CONNECTIONS;
  config->bluetooth.max_advertisers = SL_BT_CONFIG_MAX_ADVERTISERS;
  config->bluetooth.max_periodic_sync = SL_BT_CONFIG_MAX_PERIODIC_ADVERTISING_SYNC;
}

sl_bt_bluetooth_ll_priorities *emberAfPluginBleGetLlPrioritiesConfig(void)
{
  return &ll_priorities;
}

bool emberAfPluginBleHasEventPending(void)
{
  return (sl_bt_rtos_has_event_waiting() == SL_STATUS_OK);
}

void emberAfPluginBleInitCallback(void)
{
  // TODO
}

// This is run as part of the Zigbee task main loop.
void emberAfPluginBleTickCallback(void)
{
  if (sl_bt_rtos_event_wait(false) == SL_STATUS_OK) {
    emberAfPluginBleEventCallback((sl_bt_msg_t *)sl_bt_rtos_get_event());
    sl_bt_rtos_set_event_handled();
  }
}
