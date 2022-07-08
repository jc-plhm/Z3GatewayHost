#ifdef EMBER_AF_NCP
#include "app/framework/include/af-ncp.h"
#else // !EMBER_AF_NCP
#include "app/framework/include/af.h"
#endif // EMBER_AF_NCP
#include <em_common.h>
#include "sl_bluetooth.h"
#include "sl_bt_stack_init.h"
#include "sl_bt_api.h"
#include "ble-interface.h"
#include "sli_bt_gattdb_def.h"

extern const sli_bt_gattdb_t gattdb;
static sl_bt_configuration_t config = SL_BT_CONFIG_DEFAULT;

/** @brief Table of used BGAPI classes */
static const struct sli_bgapi_class * const bt_class_table[] =
{
  SL_BT_BGAPI_CLASS(dfu),
  SL_BT_BGAPI_CLASS(system),
  SL_BT_BGAPI_CLASS(advertiser),
  SL_BT_BGAPI_CLASS(gap),
  SL_BT_BGAPI_CLASS(scanner),
  SL_BT_BGAPI_CLASS(connection),
  SL_BT_BGAPI_CLASS(gatt),
  SL_BT_BGAPI_CLASS(gatt_server),
  SL_BT_BGAPI_CLASS(nvm),
  SL_BT_BGAPI_CLASS(sm),
  NULL
};

void sl_bt_init()
{
  emberAfPluginBleGetConfig(&config);
  sl_bt_init_stack(&config);
  sl_bt_init_classes(bt_class_table);
  sl_bt_init_multiprotocol();
}

SL_WEAK void sl_bt_on_event(sl_bt_msg_t* evt)
{
  (void)(evt);
}
void sl_bt_process_event(sl_bt_msg_t *evt)
{
  sl_bt_on_event(evt);
}
void sl_bt_step()
{
  sl_bt_msg_t evt;
  /* Pop (non-blocking) a Bluetooth stack event from event queue. */
  sl_status_t status = sl_bt_pop_event(&evt);
  if (status != SL_STATUS_OK) {
    return;
  }
  sl_bt_process_event(&evt);
}
