#ifndef TWINPERIPHERAL_BLE_STACK_INCLUDED
#define TWINPERIPHERAL_BLE_STACK_INCLUDED

// clang-format off
#include <string>
#include <atomic>
#include <mutex>
#include <memory>

// clang-format on
#include "esp_event.h"
#include "esp_log.h"
#include "esp_nimble_hci.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "host/ble_hs.h"

#include "nimble/nimble_port.h"
#include "nimble/nimble_port_freertos.h"
#include "nvs_flash.h"
#include "sdkconfig.h"
#include "services/gap/ble_svc_gap.h"
#include "services/gatt/ble_svc_gatt.h"
#include "store/config/ble_store_config.h"

namespace twinperipheral::ble {
  class Stack {
  public:
    static void start(const std::string bleName,
                      ble_gap_event_fn *gapEventHandler,
                      void *gapEventHandlerArg,
                      const ble_gatt_svc_def *gattSvcDefs = nullptr);
    static void stop();
    static uint8_t BLE_ADDR_TYPE;
    static bool IS_RUNNING;
    class Peripheral {
    public:
      static void placeMessageToSend(std::string);
      static std::string fetchPendingMessage();
      static void purgePendingMessage();
      static void configureGattServices(const ble_gatt_svc_def *gattSvcDefs);
      static void startAdvertising(ble_gap_event_fn *gapEventHandler,
                                   void *gapEventHandlerArg);
      static void stopAdvertising();

    public:
      static std::atomic_bool IS_FOR_SENDING_MEASUREMENT;

    private:
      static std::string pendingMessage;
      static std::mutex mtx;
    };

  private:
    static void syncCallback(void);
    static void hostTask(void *params);
    static ble_gap_event_fn *_gapEventHandler;
    static void *_gapEventHandlerArg;
    static std::string TAG;
  };
}

#endif