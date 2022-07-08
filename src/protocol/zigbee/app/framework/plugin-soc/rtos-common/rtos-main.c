/***************************************************************************//**
 * @file
 * @brief Main entrypoint when using RTOS.
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
#ifdef EMBER_AF_NCP
#include "app/framework/include/af-ncp.h"
#else // !EMBER_AF_NCP
#include "app/framework/include/af.h"
#endif // EMBER_AF_NCP

#include "cmsis_os2.h"
#include "sl_cmsis_os2_common.h"

#include "rtos-zigbee.h"

#if defined(EMBER_AF_PLUGIN_BLE)
#include "protocol/bluetooth/inc/sl_bt_stack_init.h"
#include EMBER_AF_API_BLE
#include "sl_bt_rtos_adaptation.h"
#include "sl_bluetooth.h"
#endif // EMBER_AF_PLUGIN_BLE

#if defined(EMBER_AF_PLUGIN_MBEDTLS_MULTIPROTOCOL)
#include "sl_mbedtls.h"
#include "sl_bt_mbedtls_context.h"
#endif

#ifdef EMBER_AF_NCP
extern int emberAfMain(MAIN_FUNCTION_PARAMETERS);
#endif // EMBER_AF_NCP
//------------------------------------------------------------------------------
// Tasks variables and defines

#define ZIGBEE_STACK_TASK_PRIORITY              (osPriorityRealtime7 - 6)

#define ZIGBEE_TASK_YIELD 0x0001

static osThreadId_t zigbee_task_id;
__ALIGNED(8) static uint8_t zigbee_task_stack[EMBER_AF_PLUGIN_RTOS_COMMON_ZIGBEE_STACK_SIZE * sizeof(void *)];
__ALIGNED(4) static uint8_t zigbee_task_cb[osThreadCbSize];
static osThreadAttr_t zigbee_task_attr;

#if defined(EMBER_AF_PLUGIN_RTOS_COMMON_APP_TASK1)
static osThreadId_t zigbee_application_task_1_id;
__ALIGNED(8) static uint8_t zigbee_application_task_1_stack[EMBER_AF_PLUGIN_RTOS_COMMON_APP_TASK1_STACK_SIZE * sizeof(void *)];
__ALIGNED(4) static uint8_t zigbee_application_task_1_cb[osThreadCbSize];
static osThreadAttr_t zigbee_application_task_1_attr;
#endif // EMBER_AF_PLUGIN_RTOS_COMMON_APP_TASK1

#if defined(EMBER_AF_PLUGIN_RTOS_COMMON_APP_TASK2)
static osThreadId_t zigbee_application_task_2_id;
__ALIGNED(8) static uint8_t zigbee_application_task_2_stack[EMBER_AF_PLUGIN_RTOS_COMMON_APP_TASK2_STACK_SIZE * sizeof(void *)];
__ALIGNED(4) static uint8_t zigbee_application_task_2_cb[osThreadCbSize];
static osThreadAttr_t zigbee_application_task_2_attr;
#endif // EMBER_AF_PLUGIN_RTOS_COMMON_APP_TASK2

#if defined(EMBER_AF_PLUGIN_RTOS_COMMON_APP_TASK3)
static osThreadId_t zigbee_application_task_3_id;
__ALIGNED(8) static uint8_t zigbee_application_task_3_stack[EMBER_AF_PLUGIN_RTOS_COMMON_APP_TASK3_STACK_SIZE * sizeof(void *)];
__ALIGNED(4) static uint8_t zigbee_application_task_3_cb[osThreadCbSize];
static osThreadAttr_t zigbee_application_task_3_attr;
#endif // EMBER_AF_PLUGIN_RTOS_COMMON_APP_TASK3

//------------------------------------------------------------------------------
// Event flags and semaphores

static osEventFlagsId_t zigbee_task_event_flags_id;
__ALIGNED(4) static uint8_t zigbee_task_event_flags_cb[osEventFlagsCbSize];
static osEventFlagsAttr_t zigbee_task_event_flags_attr = {
  .name = "Zigbee event flags",
  .cb_mem = zigbee_task_event_flags_cb,
  .cb_size = osEventFlagsCbSize,
  .attr_bits = 0
};

//------------------------------------------------------------------------------
// Forward and external declarations

static void zigbee_task(void *p_arg);
static void init_cpu(void);

#ifdef SL_CATALOG_MICRIUMOS_KERNEL_PRESENT
extern void App_OS_SetAllHooks(void);
#endif
//------------------------------------------------------------------------------
// Main

void emAfPluginRtosSleepInit(void);

#ifdef SL_CATALOG_SEGGER_RTT_PRESENT
#include "os_trace_events.h"
#endif //SL_CATALOG_SEGGER_RTT_PRESENT

int main(MAIN_FUNCTION_PARAMETERS)
{
  osStatus_t status;
#ifdef SL_CATALOG_MICRIUMOS_KERNEL_PRESENT
  CPU_Init();
#endif
  halInit();  //
  init_cpu(); //
#ifndef EMBER_AF_NCP
  // Not exist on NCP.
  emberAfMainInit();
#endif
  // Initialize tokens in zigbee before we call emberAfMain because we need to
  // upgrade any potential SimEEv2 data to NVM3 before we call gecko_stack_init
  // (a bluetooth call). If we don't, the bluetooth call only initializes NVM3
  // and wipes any SimEEv2 data present (it does not handle upgrading). The
  // second call of halStackInitTokens in the zigbee stack will be a no-op
  if (EMBER_SUCCESS != halStackInitTokens()) {
    assert(false);
  }

  #ifdef SL_CATALOG_SEGGER_RTT_PRESENT
  SEGGER_SYSVIEW_Conf();
  #endif //SL_CATALOG_SEGGER_RTT_PRESENT

  status = osKernelInitialize();
  assert(status == osOK);

#ifdef SL_CATALOG_MICRIUMOS_KERNEL_PRESENT
  App_OS_SetAllHooks();
#endif

  emAfPluginRtosSleepInit();

  // Create ZigBee task.
  zigbee_task_attr.name = "Zigbee task";
  zigbee_task_attr.stack_mem = &zigbee_task_stack[0];
  zigbee_task_attr.stack_size = sizeof(zigbee_task_stack);
  zigbee_task_attr.cb_mem = zigbee_task_cb;
  zigbee_task_attr.cb_size = osThreadCbSize;
  zigbee_task_attr.priority = ZIGBEE_STACK_TASK_PRIORITY;
  zigbee_task_attr.attr_bits = 0;
  zigbee_task_attr.tz_module = 0;

  zigbee_task_id = osThreadNew(zigbee_task,
                               NULL,
                               &zigbee_task_attr);
  assert(zigbee_task_id != NULL);

  zigbee_task_event_flags_id = osEventFlagsNew(&zigbee_task_event_flags_attr);
  assert(zigbee_task_event_flags_id != NULL);

  // Start the OS
  status = osKernelStart();
  assert(status == osOK);
}

//------------------------------------------------------------------------------
// Internal APIs

// This can be called from ISR.
void emAfPluginRtosWakeUpZigbeeStackTask(void)
{
  uint32_t flags = osEventFlagsSet(zigbee_task_event_flags_id, ZIGBEE_TASK_YIELD);
  assert((flags & osFlagsError) == 0);
}

//------------------------------------------------------------------------------
// Implemented callbacks

bool emberRtosIdleHandler(uint32_t *idleTimeMs)
{
  return emberAfPluginIdleSleepRtosCallback(idleTimeMs, false);
}

void emberRtosStackWakeupIsrHandler(void)
{
  emAfPluginRtosWakeUpZigbeeStackTask();
}

//------------------------------------------------------------------------------
// Static functions

static void zigbee_task(void *p_arg)
{
#if defined(EMBER_AF_PLUGIN_MBEDTLS_MULTIPROTOCOL)
  // Call sl_mbedtls_init to initialize Silicon Labs specifics required for
  // proper mbedTLS operation, e.g. the SE Manager and redirection of mbedTLS's
  // threading subsystem to the RTOS.
  sl_mbedtls_init();
#endif

#if defined(EMBER_AF_PLUGIN_BLE)
  sl_bt_rtos_init();
#endif // EMBER_AF_PLUGIN_BLE

#if defined(EMBER_AF_PLUGIN_RTOS_COMMON_APP_TASK1)
  emberAfPluginRtosAppTask1InitCallback();

  // Create Application Task 1.
  zigbee_application_task_1_attr.name = "Zigbee Application task 1";
  zigbee_application_task_1_attr.stack_mem = &zigbee_application_task_1_stack[0];
  zigbee_application_task_1_attr.stack_size = sizeof(zigbee_application_task_1_stack);
  zigbee_application_task_1_attr.cb_mem = zigbee_application_task_1_cb;
  zigbee_application_task_1_attr.cb_size = osThreadCbSize;
  zigbee_application_task_1_attr.priority = (osPriorityRealtime7 - EMBER_AF_PLUGIN_RTOS_COMMON_APP_TASK1_PRIORITY);
  zigbee_application_task_1_attr.attr_bits = 0;
  zigbee_application_task_1_attr.tz_module = 0;

  zigbee_application_task_1_id = osThreadNew(emberAfPluginRtosAppTask1MainLoopCallback,
                                             NULL,
                                             &zigbee_application_task_1_attr);
  assert(zigbee_application_task_1_id != NULL);

#endif // EMBER_AF_PLUGIN_RTOS_COMMON_APP_TASK1

#if defined(EMBER_AF_PLUGIN_RTOS_COMMON_APP_TASK2)
  emberAfPluginRtosAppTask2InitCallback();

  // Create Application Task 2.
  zigbee_application_task_2_attr.name = "Zigbee Application task 2";
  zigbee_application_task_2_attr.stack_mem = &zigbee_application_task_2_stack[0];
  zigbee_application_task_2_attr.stack_size = sizeof(zigbee_application_task_2_stack);
  zigbee_application_task_2_attr.cb_mem = zigbee_application_task_2_cb;
  zigbee_application_task_2_attr.cb_size = osThreadCbSize;
  zigbee_application_task_2_attr.priority = (osPriorityRealtime7 - EMBER_AF_PLUGIN_RTOS_COMMON_APP_TASK2_PRIORITY);
  zigbee_application_task_2_attr.attr_bits = 0;
  zigbee_application_task_2_attr.tz_module = 0;

  zigbee_application_task_2_id = osThreadNew(emberAfPluginRtosAppTask2MainLoopCallback,
                                             NULL,
                                             &zigbee_application_task_2_attr);
  assert(zigbee_application_task_2_id != NULL);

#endif // EEMBER_AF_PLUGIN_RTOS_COMMON_APP_TASK2

#if defined(EMBER_AF_PLUGIN_RTOS_COMMON_APP_TASK3)
  emberAfPluginRtosAppTask3InitCallback();

  // Create Application Task 3.
  zigbee_application_task_3_attr.name = "Zigbee Application task 3",
  zigbee_application_task_3_attr.stack_mem = &zigbee_application_task_3_stack[0];
  zigbee_application_task_3_attr.stack_size = sizeof(zigbee_application_task_3_stack);
  zigbee_application_task_3_attr.cb_mem = zigbee_application_task_3_cb;
  zigbee_application_task_3_attr.cb_size = osThreadCbSize;
  zigbee_application_task_3_attr.priority = (osPriorityRealtime7 - EMBER_AF_PLUGIN_RTOS_COMMON_APP_TASK3_PRIORITY);
  zigbee_application_task_3_attr.attr_bits = 0;
  zigbee_application_task_3_attr.tz_module = 0;

  zigbee_application_task_3_id = osThreadNew(emberAfPluginRtosAppTask3MainLoopCallback,
                                             NULL,
                                             &zigbee_application_task_3_attr);
  assert(zigbee_application_task_3_id != NULL);

#endif // EMBER_AF_PLUGIN_RTOS_COMMON_APP_TASK3

  emberAfMain();
}

// Register interrupt vectors with the OS
static void init_cpu(void)
{
  // Radio Interrupts can optionally be configured non-kernel aware at this point
}

void rtos_zigbee_pend_on_event(uint32_t timeout)
{
  uint32_t flags = osEventFlagsWait(zigbee_task_event_flags_id, ZIGBEE_TASK_YIELD, osFlagsWaitAny, timeout);
  assert((flags != osFlagsErrorUnknown) && (flags != osFlagsErrorParameter) && (flags != osFlagsErrorResource));
}
