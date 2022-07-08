/***************************************************************************//**
 * @file
 * @brief
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
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

// This callback file is created for your convenience. You may add application
// code to this file. If you regenerate this file over a previous version, the
// previous version will be overwritten and any code you have added will be
// lost.

#include "app/framework/include/af.h"
#include "app/framework/util/af-main.h"
#include "app/framework/plugin/green-power-common/green-power-common.h"

#include EMBER_AF_API_NETWORK_CREATOR
#include EMBER_AF_API_NETWORK_CREATOR_SECURITY
#include EMBER_AF_API_NETWORK_STEERING
#include EMBER_AF_API_FIND_AND_BIND_TARGET

#ifdef EMBER_AF_PLUGIN_MULTIRAIL_DEMO
  #include EMBER_AF_API_MULTIRAIL_DEMO
#endif

#define LIGHT_ENDPOINT (1)

/** @brief Event control definitions
 */
EmberEventControl commissioningLedEventControl;
EmberEventControl findingAndBindingEventControl;
EmberEventControl sinkCommissioningModeEventControl;

void commissioningLedEventHandler(void)
{
  emberEventControlSetInactive(commissioningLedEventControl);

  if (emberAfNetworkState() == EMBER_JOINED_NETWORK) {
    uint16_t identifyTime;
    emberAfReadServerAttribute(LIGHT_ENDPOINT,
                               ZCL_IDENTIFY_CLUSTER_ID,
                               ZCL_IDENTIFY_TIME_ATTRIBUTE_ID,
                               (uint8_t *)&identifyTime,
                               sizeof(identifyTime));
    if (identifyTime > 0) {
      halToggleLed(COMMISSIONING_STATUS_LED);
      emberEventControlSetDelayMS(commissioningLedEventControl,
                                  LED_BLINK_PERIOD_MS << 1);
    } else {
      halSetLed(COMMISSIONING_STATUS_LED);
    }
  } else {
    EmberStatus status = emberAfPluginNetworkSteeringStart();
    emberAfCorePrintln("%p network %p: 0x%X", "Join", "start", status);
  }
}

void findingAndBindingEventHandler()
{
  if (emberAfNetworkState() == EMBER_JOINED_NETWORK) {
    emberEventControlSetInactive(findingAndBindingEventControl);
    emberAfCorePrintln("Find and bind target start: 0x%X",
                       emberAfPluginFindAndBindTargetStart(LIGHT_ENDPOINT));
  }
}

/** An automatic scheduled response using a secondary RAIL instance.
 *
 * Only applicable if EMBER_AF_PLUGIN_MULTIRAIL_DEMO is #defined.
 */
#ifdef EMBER_AF_PLUGIN_MULTIRAIL_DEMO

// Outgoing Bi-directional GPDF queue and routines
#ifndef EMBER_APPL_GP_BIDIRECTIONAL_TX_QUEUE_SIZE
  #define EMBER_APPL_GP_BIDIRECTIONAL_TX_QUEUE_SIZE 1
#endif

// The time between receiving a GP packet and sending a scheduled response
#ifndef GP_RX_OFFSET_USEC
  #define GP_RX_OFFSET_USEC 20500
#endif

extern uint8_t sl_mac_lower_mac_get_radio_channel(uint8_t mac_index);
extern EmberMessageBuffer emGpdfMakeHeader(bool useCca,
                                           EmberGpAddress *src,
                                           EmberGpAddress *dst);
extern void emberDGpSentHandler(EmberStatus status,
                                uint8_t gpepHandle);

// Frees one entry
static void emAppFreeGpTxQueueEntry(EmberGpTxQueueEntry* entry)
{
  emberGpRemoveFromTxQueue(entry);
}

// Resets the entire queue
static void emAppInitGpTxQueue(void)
{
  emberGpClearTxQueue();
}

// Gets an entry from the Gp Stub queue if available and RAIL 2 is ready
static EmberGpTxQueueEntry* emAppGetGpStubTxQueue(EmberGpAddress* addr)
{
  // Steal the GP Queue to send it through the additional RAIL handle
  // Check if RAIL 2 handle available and there is anything in GP Stub queue
  EmberGpTxQueueEntry emGpTxQueue;
  MEMCOPY(&emGpTxQueue.addr, addr, sizeof(EmberGpAddress));
  uint8_t data[128];
  uint16_t dataLength;
  if (emberAfPluginMultirailDemoGetHandle()
      && emberGpGetTxQueueEntryFromQueue(&emGpTxQueue,
                                         data,
                                         &dataLength,
                                         128) != EMBER_NULL_MESSAGE_BUFFER) {
    // Allocate a buffer and prepare a outgoing MAC header using gpd address in the emGpTxQueue
    EmberMessageBuffer header = emGpdfMakeHeader(true, NULL, &(emGpTxQueue.addr));
    // Add the command Id from the queue to the buffer
    uint8_t len = emberMessageBufferLength(header) + 1;
    emberAppendToLinkedBuffers(header, &(emGpTxQueue.gpdCommandId), 1);
    // Copy Command Payload from the queue to the buffer and update the length
    emberSetLinkedBuffersLength(header,
                                emberMessageBufferLength(header)
                                + dataLength);
    // Add the payload
    emberCopyToLinkedBuffers(data,
                             header,
                             len,
                             dataLength);
    // Clear the Stub queue because everything is serialised in header
    emberGpRemoveFromTxQueue(&emGpTxQueue);

    // Prepare a RAIL frame to be transported using the additional handle
    uint8_t outPktLength = emberMessageBufferLength(header);
    uint8_t outPkt[128]; //128 = MAX size
    // RAIL Frame : [Total Length (excludes itself) | <-----MAC FRAME ---->| 2 byte CRC]
    outPkt[0] = outPktLength + 2;
    // Copy the data from the buffer
    emberCopyFromLinkedBuffers(header,
                               0,
                               &outPkt[1],
                               outPktLength);
    // Free the header as the rail frame will be submitted with a new buffer asdu
    emberReleaseMessageBuffer(header);

    // This entry is exempt from marking (see emberAfMarkBuffersCallback below),
    // since it is allocated and released within the context of the same function
    // call (appGpScheduleOutgoingGpdf).
    static EmberGpTxQueueEntry copyOfGpStubTxQueue;
    copyOfGpStubTxQueue.inUse = true;
    copyOfGpStubTxQueue.asdu = emberFillLinkedBuffers(outPkt, (outPkt[0] + 1));
    MEMCOPY(&(copyOfGpStubTxQueue.addr), addr, sizeof(EmberGpAddress));
    return &copyOfGpStubTxQueue;
  }
  return NULL;
}

/** CLI functions exposing the above GpTx APIs
 */
static void gpAppGpTxQueueInit(void)
{
  emAppInitGpTxQueue();
  RAIL_Handle_t h = emberAfPluginMultirailDemoInit(NULL,
                                                   NULL,
                                                   true,
                                                   RAIL_GetTxPowerDbm(emberGetRailHandle()),
                                                   NULL,
                                                   0,
                                                   0xFFFF,
                                                   NULL);
  emberAfCorePrintln("Additional RAIL handle %sintialized", h ? "" : "not ");
}

static void gpAppGpTxQueueSet(void)
{
  EmberGpAddress addr = { 0 };

  addr.applicationId = emberUnsignedCommandArgument(0);
  if (addr.applicationId != EMBER_GP_APPLICATION_SOURCE_ID) {
    emberAfCorePrintln("Invalid application ID");
    return;
  }
  addr.id.sourceId = emberUnsignedCommandArgument(1);

  uint8_t gpdCommandId = emberUnsignedCommandArgument(2);

  uint8_t gpdAsduLength;
  uint8_t *gpdAsdu = emberStringCommandArgument(3, &gpdAsduLength);

  if (emberDGpSend(true,//bool action,
                   false,//bool useCca,
                   &addr,
                   gpdCommandId,
                   gpdAsduLength,
                   gpdAsdu,
                   0,
                   0) != EMBER_SUCCESS) {
    emberAfCorePrintln("Failed to add entry");
  }
}

static void gpAppGpTxQueueGet(void)
{
  EmberGpTxQueueEntry txQueue;
  txQueue.addr.applicationId = emberUnsignedCommandArgument(0);
  if (txQueue.addr.applicationId != EMBER_GP_APPLICATION_SOURCE_ID) {
    emberAfCorePrintln("Invalid application ID");
    return;
  }
  txQueue.addr.id.sourceId = emberUnsignedCommandArgument(1);

  uint8_t buffer[128];
  uint16_t length = 0;

  if (emberGpGetTxQueueEntryFromQueue(&txQueue,
                                      buffer,
                                      &length,
                                      128) != EMBER_NULL_MESSAGE_BUFFER) {
    emberAfCorePrintln("");
    emberAfCorePrintBuffer(buffer, length, true);
    emberAfCorePrintln("");
  } else {
    emberAfCorePrintln("No entry found");
  }
}

static void gpAppGpTxQueueRemove(void)
{
  EmberGpAddress addr = { 0 };

  addr.applicationId = emberUnsignedCommandArgument(0);
  if (addr.applicationId != EMBER_GP_APPLICATION_SOURCE_ID) {
    emberAfCorePrintln("Invalid application ID");
    return;
  }
  addr.id.sourceId = emberUnsignedCommandArgument(1);

  if (emberDGpSend(false,//bool action,
                   false,//bool useCca,
                   &addr,
                   0,
                   0,
                   EMBER_NULL_MESSAGE_BUFFER,
                   0,
                   0) != EMBER_SUCCESS) {
    emberAfCorePrintln("No entry found");
  }
}

static void gpAppGpSetTxQueueSize(void)
{
  uint8_t size = emberUnsignedCommandArgument(0);
  emberGpSetMaxTxQueueEntry((uint16_t)size);
}

static void gpAppGpGetTxQueueSize(void)
{
  emberAfCorePrintln("Max Tx Queue Size = %d", emberGetGpMaxTxQListCount());
}

static void gpAppGpGetTxQueueCount(void)
{
  emberAfCorePrintln("Tx Queue Count = %d", emberGetGpTxQListCount());
}

static void gpAppGpPrintTxQueue(void)
{
  uint16_t listCount = emberGetGpTxQListCount();
  emberAfCorePrintln("\nNumber of Gp Tx Queue entries : %d", listCount);
  int i = 0;
  if (listCount != 0) {
    Buffer finger = emBufferQueueHead(emberGpGetTxQueueHead());
    while (finger != NULL_BUFFER) {
      emberAfCorePrintln("Entry [%d] :", i++);
      EmberGpTxQueueEntry *entry = (EmberGpTxQueueEntry *)emberMessageBufferContents(finger);
      emberAfCorePrintln("  App Id =  %d", entry->addr.applicationId);
      emberAfCorePrintln("  Src Id = 0x%4X", entry->addr.id.sourceId);
      uint8_t *data = (uint8_t *) (emberMessageBufferContents(finger) + sizeof(EmberGpTxQueueEntry));
      uint16_t dataLength = emberMessageBufferLength(finger) - sizeof(EmberGpTxQueueEntry);
      emberAfCorePrintln("  Data Length = %d", dataLength);
      emberAfCorePrint("  Data = [");
      emberAfCorePrintBuffer(data, dataLength, true);
      emberAfCorePrint("]\n");
      finger = emBufferQueueNext(emberGpGetTxQueueHead(), finger);
    }
  }
}

#define macToAppDelay(macTimeStamp) ((RAIL_GetTime() & 0x00FFFFFF) - (macTimeStamp))

static void appGpScheduleOutgoingGpdf(EmberZigbeePacketType packetType,
                                      int8u* packetData,
                                      int8u size_p)
{
  //MAC Frame  : [<---------------MAC Header------------->||<------------------------------------NWK Frame----------------------------------->]
  //              FC(2) | Seq(1) | DstPan(2) | DstAddr(2) || FC(1) | ExtFC(0/1) | SrcId(0/4) | SecFc(0/4) | MIC(0/4) | <------GPDF(1/n)------>
  //              FC    : ExtFC Present(b7)=1| AC(b6)=0| Protocol Ver(b5-b2)=3 GP frames| Frame Type(b1-b0) = 0
  //              ExtFC :  rxAfteTX (b6) = 1 |  AppId(b2-b0) = 0
  if (packetType == EMBER_ZIGBEE_PACKET_TYPE_RAW_MAC
      && size_p > 9 // minimum
      && ((packetData[7] & 0x3C) == 0x0C)            // FC : GP Frames has Protocol Version = 3 ?
      && (((packetData[7] & 0xC3) == 0x01)           // FC : ExtFC(b7) = 0 | AC(b6) = 0 | Frame type(b1-b0) = 1 = Maint, Frame
          || (((packetData[7] & 0xC3) == 0x80)     // FC : ExtFC(b7) = 1 | AC(b6) = 0 | Frame type(b1-b0) = 0 = Data frame
              && ((packetData[8] & 0xC0) == 0x40)))) { // ExtFC :  Direction(b7) = 0 | rxAfteTX (b6) = 1
    // The last 3 bytes of packetData contain the MAC time stamp
    const uint32_t macTimeStamp = ((uint32_t)packetData[size_p - 3] << 16)
                                  + ((uint32_t)packetData[size_p - 2] << 8)
                                  + ((uint32_t)packetData[size_p - 1]);

    // Do we have enough time to schedule the response?
    if (macToAppDelay(macTimeStamp) < GP_RX_OFFSET_USEC) {
      EmberGpAddress gpdAddr;
      gpdAddr.applicationId = EMBER_GP_APPLICATION_SOURCE_ID;
      gpdAddr.id.sourceId = 0;
      if (((packetData[7] & 0xC3) == 0x80)   // FC : ExtFC(b7) = 1 | AC(b6) = 0 | Frame type(b1-b0) = 0 = Data frame
          && ((packetData[8] & 0x07) == EMBER_GP_APPLICATION_SOURCE_ID)) {// ExtFC :  App Id (b2-b0) = 0
        (void) memcpy(&gpdAddr.id.sourceId, &packetData[9], sizeof(EmberGpSourceId));
      }
      // Is there a queued response for this source ID?
      EmberGpTxQueueEntry* entry = emAppGetGpStubTxQueue(&gpdAddr);
      if (entry) {
        uint8_t outPktLength = emberMessageBufferLength(entry->asdu);
        uint8_t outPkt[128];
        emberCopyFromLinkedBuffers(entry->asdu,
                                   0,
                                   outPkt,
                                   outPktLength);

        // Schedule sending the response.
        RAIL_SchedulerInfo_t schedulerInfo = {
          .priority = 50,
          .slipTime = 2000,
          .transactionTime = 5000
        };
        RAIL_ScheduleTxConfig_t scheduledTxConfig = {
          .mode = RAIL_TIME_DELAY,
          // We could reuse macToAppDelay here, but recalculating the delay
          // will give us the most up-to-date timings:
          .when = GP_RX_OFFSET_USEC - macToAppDelay(macTimeStamp)
        };

        RAIL_Status_t UNUSED status = emberAfPluginMultirailDemoSend(outPkt,
                                                                     outPktLength,
                                                                     sl_mac_lower_mac_get_radio_channel(0),
                                                                     &scheduledTxConfig,
                                                                     &schedulerInfo);
        emAppFreeGpTxQueueEntry(entry);
      }
    }
  }
}

/** CLI descriptions.
 *
 * Only applicable if EMBER_AF_PLUGIN_MULTIRAIL_DEMO is #defined.
 */
static PGM_P PGM gpAppGpTxQueueSetArguments[] = {
  "GPD application ID (must be 0)",
  "GPD source ID",
  "GPD CommandId",
  "GPD command payload",
  NULL
};

static PGM_P PGM gpAppGpTxQueueGetOrRemoveArguments[] = {
  "GPD application ID (must be 0)",
  "GPD source ID",
  NULL
};

static PGM_P PGM gpAppGpSetTxQueueSizeArguments[] = {
  "Maximum Size of Gp Tx Queue",
  NULL
};

#endif // EMBER_AF_PLUGIN_MULTIRAIL_DEMO

/** Main custom CLI definition
 */
PGM EmberCommandEntry emberAfCustomCommands[] = {
#ifdef EMBER_AF_PLUGIN_MULTIRAIL_DEMO
  emberCommandEntryActionWithDetails("gpAppTxQInit", gpAppGpTxQueueInit, "", "Initialises the Application Gp Tx Queue", NULL),
  emberCommandEntryActionWithDetails("gpAppTxQSet", gpAppGpTxQueueSet, "uwub", "Sets a packet on Application Gp Tx Queue", gpAppGpTxQueueSetArguments),
  emberCommandEntryActionWithDetails("gpAppTxQGet", gpAppGpTxQueueGet, "uw", "Gets a packet from Application Gp Tx Queue", gpAppGpTxQueueGetOrRemoveArguments),
  emberCommandEntryActionWithDetails("gpAppTxQRemove", gpAppGpTxQueueRemove, "uw", "Removes a packet from Application Gp Tx Queue", gpAppGpTxQueueGetOrRemoveArguments),
  emberCommandEntryActionWithDetails("gpAppTxQSetMaxSize", gpAppGpSetTxQueueSize, "u", "Sets the Gp Queue Max Size", gpAppGpSetTxQueueSizeArguments),
  emberCommandEntryActionWithDetails("gpAppTxQGetMaxSize", gpAppGpGetTxQueueSize, "", "Gets the Gp Queue Max Size", NULL),
  emberCommandEntryActionWithDetails("gpAppTxQGetCount", gpAppGpGetTxQueueCount, "", "Gets the Gp Queue Counts", NULL),
  emberCommandEntryActionWithDetails("gpAppPrintTxQ", gpAppGpPrintTxQueue, "", "Prints Gp Tx Queue", NULL),
#endif //EMBER_AF_PLUGIN_MULTIRAIL_DEMO
  emberCommandEntryTerminator()
};

/** @brief Stack Status
 *
 * This function is called by the application framework from the stack status
 * handler.  This callbacks provides applications an opportunity to be notified
 * of changes to the stack status and take appropriate action.  The return code
 * from this callback is ignored by the framework.  The framework will always
 * process the stack status after the callback returns.
 *
 * @param status   Ver.: always
 */
bool emberAfStackStatusCallback(EmberStatus status)
{
  // Make sure to change the ZLL factory new state based on whether or not
  // we are on a network.
  if (status == EMBER_NETWORK_DOWN) {
    halClearLed(COMMISSIONING_STATUS_LED);
  } else if (status == EMBER_NETWORK_UP) {
    halSetLed(COMMISSIONING_STATUS_LED);

    emberEventControlSetActive(findingAndBindingEventControl);
  }

  // This value is ignored by the framework.
  return false;
}

/** @brief Main Init
 *
 * This function is called from the application's main function. It gives the
 * application a chance to do any initialization required at system startup.
 * Any code that you would normally put into the top of the application's
 * main() routine should be put into this function.
        Note: No callback
 * in the Application Framework is associated with resource cleanup. If you
 * are implementing your application on a Unix host where resource cleanup is
 * a consideration, we expect that you will use the standard Posix system
 * calls, including the use of atexit() and handlers for signals such as
 * SIGTERM, SIGINT, SIGCHLD, SIGPIPE and so on. If you use the signal()
 * function to register your signal handler, please mind the returned value
 * which may be an Application Framework function. If the return value is
 * non-null, please make sure that you call the returned function from your
 * handler to avoid negating the resource cleanup of the Application Framework
 * itself.
 *
 */
void emberAfMainInitCallback(void)
{
  emberEventControlSetActive(commissioningLedEventControl);
}

/** @brief Complete
 *
 * This callback is fired when the Network Steering plugin is complete.
 *
 * @param status On success this will be set to EMBER_SUCCESS to indicate a
 * network was joined successfully. On failure this will be the status code of
 * the last join or scan attempt. Ver.: always
 * @param totalBeacons The total number of 802.15.4 beacons that were heard,
 * including beacons from different devices with the same PAN ID. Ver.: always
 * @param joinAttempts The number of join attempts that were made to get onto
 * an open Zigbee network. Ver.: always
 * @param finalState The finishing state of the network steering process. From
 * this, one is able to tell on which channel mask and with which key the
 * process was complete. Ver.: always
 */
void emberAfPluginNetworkSteeringCompleteCallback(EmberStatus status,
                                                  uint8_t totalBeacons,
                                                  uint8_t joinAttempts,
                                                  uint8_t finalState)
{
  emberAfCorePrintln("%p network %p: 0x%X", "Join", "complete", status);

  if (status != EMBER_SUCCESS) {
    status = emberAfPluginNetworkCreatorStart(false); // distributed
    emberAfCorePrintln("%p network %p: 0x%X", "Form", "start", status);
  }
}

/** @brief Complete
 *
 * This callback notifies the user that the network creation process has
 * completed successfully.
 *
 * @param network The network that the network creator plugin successfully
 * formed. Ver.: always
 * @param usedSecondaryChannels Whether or not the network creator wants to
 * form a network on the secondary channels Ver.: always
 */
void emberAfPluginNetworkCreatorCompleteCallback(const EmberNetworkParameters *network,
                                                 bool usedSecondaryChannels)
{
  emberAfCorePrintln("%p network %p: 0x%X",
                     "Form distributed",
                     "complete",
                     EMBER_SUCCESS);
}

/** @brief Server Init
 *
 * On/off cluster, Server Init
 *
 * @param endpoint Endpoint that is being initialized  Ver.: always
 */
void emberAfPluginOnOffClusterServerPostInitCallback(uint8_t endpoint)
{
  // At startup, trigger a read of the attribute and possibly a toggle of the
  // LED to make sure they are always in sync.
  emberAfOnOffClusterServerAttributeChangedCallback(endpoint,
                                                    ZCL_ON_OFF_ATTRIBUTE_ID);
}

/** @brief Server Attribute Changed
 *
 * On/off cluster, Server Attribute Changed
 *
 * @param endpoint Endpoint that is being initialized  Ver.: always
 * @param attributeId Attribute that changed  Ver.: always
 */
void emberAfOnOffClusterServerAttributeChangedCallback(uint8_t endpoint,
                                                       EmberAfAttributeId attributeId)
{
  // When the on/off attribute changes, set the LED appropriately.  If an error
  // occurs, ignore it because there's really nothing we can do.
  if (attributeId == ZCL_ON_OFF_ATTRIBUTE_ID) {
    bool onOff;
    if (emberAfReadServerAttribute(endpoint,
                                   ZCL_ON_OFF_CLUSTER_ID,
                                   ZCL_ON_OFF_ATTRIBUTE_ID,
                                   (uint8_t *)&onOff,
                                   sizeof(onOff))
        == EMBER_ZCL_STATUS_SUCCESS) {
      if (onOff) {
        halSetLed(ON_OFF_LIGHT_LED);
      } else {
        halClearLed(ON_OFF_LIGHT_LED);
      }
    }
  }
}

static bool enterComm;
// Enter or exit sink commissioning mode
void sinkCommissioningModeEventHandler(void)
{
  emberEventControlSetInactive(sinkCommissioningModeEventControl);
  uint8_t options = EMBER_AF_GP_SINK_COMMISSIONING_MODE_OPTIONS_INVOLVE_PROXIES \
                    | ((enterComm) ? EMBER_AF_GP_SINK_COMMISSIONING_MODE_OPTIONS_ACTION : 0);
  emberAfGreenPowerClusterGpSinkCommissioningModeCallback(options,    //options - (Involve Proxy | Enter)
                                                          0xFFFF,     //addr
                                                          0xFFFF,     //addr
                                                          LIGHT_ENDPOINT); //light Endpoint

  if (enterComm) {
    halSetLed(BOARDLED1);
    enterComm = false;
  } else {
    halClearLed(BOARDLED1);
    enterComm = true;
  }
}
/** @brief Hal Button Isr
 *
 * This callback is called by the framework whenever a button is pressed on the
 * device. This callback is called within ISR context.
 *
 * @param button The button which has changed state, either BUTTON0 or BUTTON1
 * as defined in the appropriate BOARD_HEADER.  Ver.: always
 * @param state The new state of the button referenced by the button parameter,
 * either ::BUTTON_PRESSED if the button has been pressed or ::BUTTON_RELEASED
 * if the button has been released.  Ver.: always
 */
void emberAfHalButtonIsrCallback(uint8_t button, uint8_t state)
{
  if (state == BUTTON_PRESSED) {
    if (button == BUTTON0) {
      emberEventControlSetActive(sinkCommissioningModeEventControl);
    } else if (button == BUTTON1) {
      // Find and binf Target start
      emberEventControlSetActive(findingAndBindingEventControl);
    }
  }
}

/** @brief Gpd Commissioning
 *
 * This function is called by the Green Power Server upon expiry of either the switch commissioning
 * or multisensor commissioning timer.
 *
 * @param appInfo   Ver.: always
 */
void emberAfGreenPowerServerCommissioningTimeoutCallback(uint8_t commissioningTimeoutType,
                                                         uint8_t numberOfEndpoints,
                                                         uint8_t * endpoints)
{
  // Commissioning exit, clear the state and LED indication.
  enterComm = false;
  halClearLed(BOARDLED1);
}

/** @brief Gpd Commissioning
 *
 * This function is called by the Green Power Server upon the completion of the pairing
 * and to indicate the closure of the pairing session.
 *
 * @param appInfo   Ver.: always
 */
void emberAfGreenPowerServerPairingCompleteCallback(uint8_t numberOfEndpoints,
                                                    uint8_t * endpoints)
{
  emberAfCorePrint("%p : No of Eps = %x EPs[",
                   __FUNCTION__,
                   numberOfEndpoints);
  emberAfCorePrintBuffer(endpoints, numberOfEndpoints, true);
  emberAfCorePrintln("]");
  halClearLed(BOARDLED1);
}

/** @brief Incoming Packet Filter
 *
 * ** REQUIRES INCLUDING THE PACKET-HANDOFF PLUGIN **
 *
 * This is called by the Packet Handoff plugin when the stack receives a packet
 * from one of the protocol layers specified in ::EmberZigbeePacketType.
 *
 * The packetType argument is one of the values of the emberZigbeePacketType enum.
 * If the stack receives an 802.15.4 MAC beacon, it will call this function with
 * the packetType argument set to ::EMBER_ZIGBEE_PACKET_TYPE_BEACON.
 *
 * The implementation of this callback may alter the data contained in packetData,
 * modify options and flags in the auxillary data, or consume the packet itself,
 * either sending the message, or discarding it as it sees fit.
 *
 * @param packetType the type of packet and associated protocol layer  Ver.:
 * always
 * @param packetData flat buffer containing the packet data associated with the
 * packet type  Ver.: always
 * @param size_p a pointer containing the size value of the packet  Ver.: always
 * @param data auxillary data included with the packet  Ver.: always
 */
EmberPacketAction emberAfIncomingPacketFilterCallback(EmberZigbeePacketType packetType,
                                                      int8u* packetData,
                                                      int8u* size_p,
                                                      void* data)
{
#if (defined EMBER_AF_PLUGIN_MULTIRAIL_DEMO)
  appGpScheduleOutgoingGpdf(packetType,
                            packetData,
                            *size_p);
#endif // EMBER_AF_PLUGIN_MULTIRAIL_DEMO
  return EMBER_ACCEPT_PACKET;
}

EmberEventControl emAppGpTransmitCompleteEventControl;
void emAppGpTransmitCompleteEventHandler(void)
{
  emberEventControlSetInactive(emAppGpTransmitCompleteEventControl);
  emberDGpSentHandler(EMBER_SUCCESS, 0);
}

#if (defined EMBER_AF_PLUGIN_MULTIRAIL_DEMO)
/** @brief A callback called whenever a secondary instance RAIL event occurs.
 *
 * @param[in] handle A handle for a RAIL instance.
 * @param[in] events A bit mask of RAIL events (full list in rail_types.h)
 */
void emberAfPluginMultirailDemoRailEventCallback(RAIL_Handle_t handle,
                                                 RAIL_Events_t events)
{
  if (events & RAIL_EVENT_TX_PACKET_SENT) {
    emberEventControlSetDelayMS(emAppGpTransmitCompleteEventControl, 0);
  }
}
#endif // EMBER_AF_PLUGIN_MULTIRAIL_DEMO

/** @brief Mark Buffers
 *
 * This function is called when the garbage collector runs. Any buffers held by
 * the application must be marked.
 */
void emberAfMarkBuffersCallback(void)
{
}
