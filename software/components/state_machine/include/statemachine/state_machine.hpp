#ifndef TWINPERIPHERAL_STATE_MACHINE_INCLUDED
#define TWINPERIPHERAL_STATE_MACHINE_INCLUDED

#include "StateMachine.h"
#include "ble/app/gatt_svcs.hpp"
#include "ble/msg.hpp"
#include "ble/stack.hpp"
#include "config/app/peripheral.hpp"
#include "led/led.hpp"
#include "sense/adc.hpp"
#include "sense/bno055.hpp"
#include "system/sys.hpp"
#include <iostream>
#include <string>

#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

namespace twinperipheral::statemachine {

  template <typename... Channels>
  class TwinPeripheralStateMachine : public StateMachine {

  public:
    TwinPeripheralStateMachine(SemaphoreHandle_t stateMutex,
                               twinperipheral::system::SystemController &sys,
                               twinperipheral::sense::TwinBNO055 &bno,
                               twinperipheral::sense::TwinADC<Channels...> &adc,
                               twinperipheral::led::RGBLED &led);

    void performStartUpRoutine();
    void notifyBLEConnection();
    void notifyBLEDisconnection();
    void startBLEAdvertisement();
    void startBLECalibrationAdvertisement();
    void notifyIMUCalibrationSuccess();
    void notifyIMUCalibrationFailure();
    void sleep();
    void runSleepRoutine();
    void sendData();
    static std::string TAG;

  private:
    enum States {
      ST_IDLE,
      ST_STARTUP,
      ST_IMU_CALIBRATION,
      ST_BLE_ADV_CALIBRATION,
      ST_OPERATIONAL,
      ST_BLE_ADV,
      ST_BLE_CONNECTION,
      ST_BLE_DISCONNECTION,
      ST_SEND_DATA,
      ST_FAILED_WIRING,
      ST_SLEEP,
      ST_MAX_STATES
    };

    // Define the state machine state functions with event data type
    STATE_DECLARE(TwinPeripheralStateMachine, Idle, NoEventData)
    STATE_DECLARE(TwinPeripheralStateMachine, StartUp, NoEventData)

    ENTRY_DECLARE(TwinPeripheralStateMachine, EntryIMUCalibration, NoEventData)
    STATE_DECLARE(TwinPeripheralStateMachine, IMUCalibration, NoEventData)
    EXIT_DECLARE(TwinPeripheralStateMachine, ExitIMUCalibration)

    ENTRY_DECLARE(TwinPeripheralStateMachine, EntryBLECalibrationAdv,
                  NoEventData)
    STATE_DECLARE(TwinPeripheralStateMachine, BLECalibrationAdv, NoEventData)
    EXIT_DECLARE(TwinPeripheralStateMachine, ExitBLECalibrationAdv)

    STATE_DECLARE(TwinPeripheralStateMachine, Operational, NoEventData)

    ENTRY_DECLARE(TwinPeripheralStateMachine, EntryBLEAdv, NoEventData)
    STATE_DECLARE(TwinPeripheralStateMachine, BLEAdv, NoEventData)
    EXIT_DECLARE(TwinPeripheralStateMachine, ExitBLEAdv)

    ENTRY_DECLARE(TwinPeripheralStateMachine, EntryBLEConnection, NoEventData)
    STATE_DECLARE(TwinPeripheralStateMachine, BLEConnection, NoEventData)
    EXIT_DECLARE(TwinPeripheralStateMachine, ExitBLEConnection)

    STATE_DECLARE(TwinPeripheralStateMachine, BLEDisconnection, NoEventData)
    STATE_DECLARE(TwinPeripheralStateMachine, SendData, NoEventData)

    ENTRY_DECLARE(TwinPeripheralStateMachine, EntryFailedWiring, NoEventData)
    STATE_DECLARE(TwinPeripheralStateMachine, FailedWiring, NoEventData)
    EXIT_DECLARE(TwinPeripheralStateMachine, ExitFailedWiring)

    STATE_DECLARE(TwinPeripheralStateMachine, Sleep, NoEventData)

    BEGIN_STATE_MAP_EX
    STATE_MAP_ENTRY_EX(&Idle)
    STATE_MAP_ENTRY_EX(&StartUp)
    STATE_MAP_ENTRY_ALL_EX(&IMUCalibration, 0, &EntryIMUCalibration,
                           &ExitIMUCalibration)
    STATE_MAP_ENTRY_ALL_EX(&BLECalibrationAdv, 0, &EntryBLECalibrationAdv,
                           &ExitBLECalibrationAdv)
    STATE_MAP_ENTRY_EX(&Operational)
    STATE_MAP_ENTRY_ALL_EX(&BLEAdv, 0, &EntryBLEAdv, &ExitBLEAdv)
    STATE_MAP_ENTRY_ALL_EX(&BLEConnection, 0, &EntryBLEConnection,
                           &ExitBLEConnection)
    STATE_MAP_ENTRY_EX(&BLEDisconnection)
    STATE_MAP_ENTRY_EX(&SendData)
    STATE_MAP_ENTRY_ALL_EX(&FailedWiring, 0, &EntryFailedWiring,
                           &ExitFailedWiring)
    STATE_MAP_ENTRY_EX(&Sleep)
    END_STATE_MAP_EX

    twinperipheral::system::SystemController &sys;
    twinperipheral::sense::TwinBNO055 &bno;
    twinperipheral::sense::TwinADC<Channels...> &adc;
    twinperipheral::led::RGBLED &led;
    TimerHandle_t idleTimer;
    static std::function<void(void *)> pollShutdownSignalCb;
    void waitBeforeStateTransition(int ms);
  };

  namespace ble {
    // Generate callback function when client requests read operation that is
    // able to parse provided arg to correct templated statemachine

    template <typename... Channels>
    static ble_gatt_access_fn *genReadBLEAccessFunc() {
      static auto measurementRead =
          [](uint16_t con_handle, uint16_t attr_handle,
             struct ble_gatt_access_ctxt *ctxt, void *arg) -> int {
        auto machine =
            static_cast<twinperipheral::statemachine::
                            TwinPeripheralStateMachine<Channels...> *>(arg);
        machine->sendData();
        auto outboundMessage =
            twinperipheral::ble::Stack::Peripheral::fetchPendingMessage();
        if (!(outboundMessage.empty())) {
          os_mbuf_append(ctxt->om, outboundMessage.c_str(),
                         strlen(outboundMessage.c_str()));
        }
        return 0;
      };
      return measurementRead;
    }

    // Generate callback function when client conn status changes that is
    // able to parse provided arg to correct templated statemachine
    template <typename... Channels>
    static ble_gap_event_fn *genGapEventHandler() {
      static auto gapEventHandler = [](struct ble_gap_event *event,
                                       void *arg) -> int {
        auto machine =
            static_cast<twinperipheral::statemachine::
                            TwinPeripheralStateMachine<Channels...> *>(arg);

        switch (event->type) {
          // Advertise if connected
          case BLE_GAP_EVENT_CONNECT:
            if (event->connect.status == 0) {
              if (twinperipheral::ble::Stack::Peripheral::
                      IS_FOR_SENDING_MEASUREMENT.load())
                machine->notifyBLEConnection();
            } else {
              if (twinperipheral::ble::Stack::Peripheral::
                      IS_FOR_SENDING_MEASUREMENT.load()) {
                machine->startBLEAdvertisement();
              } else {
                machine->startBLECalibrationAdvertisement();
              }
            }
            break;
          case BLE_GAP_EVENT_DISCONNECT:
            if (twinperipheral::ble::Stack::Peripheral::
                    IS_FOR_SENDING_MEASUREMENT.load()) {
              machine->notifyBLEDisconnection();
            } else {
              machine->startBLECalibrationAdvertisement();
            }
            break;
          default:
            break;
        }
        return 0;
      };
      return gapEventHandler;
    }
  }
}

// ####################### Implementation #######################

#include "state_machineImpl.hpp"

#endif