#ifndef BLE_APP_H
#define BLE_APP_H

// clang-format off
#include <string>
#include <iostream>
#include "lib/json.hpp"

// clang-format on
#include "driver/adc.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>

#include "events.hpp"
#include "freertos/queue.h"

#include "esp_event.h"
#include "esp_nimble_hci.h"
#include "freertos/event_groups.h"
#include "host/ble_hs.h"

#include "nimble/nimble_port.h"
#include "nimble/nimble_port_freertos.h"
#include "nvs_flash.h"
#include "sdkconfig.h"
#include "services/gap/ble_svc_gap.h"
#include "services/gatt/ble_svc_gatt.h"

extern QueueHandle_t INBOUND_QUEUE;
extern QueueHandle_t OUTBOUND_QUEUE;

namespace {
  using json = nlohmann::json;

  auto const LOG_BLE_INFO_TAG = std::string("BLE_INFO");
  auto const DEVICE_NAME = std::string("BLE-TwinInsole");
  auto constexpr MAX_INBOUND_SEND_ATTEMPTS = 5;

  uint8_t ble_addr_type;

  auto ble_app_advertise() -> void;

  auto sendEventToInboundQueue(BLE::events::Event event) -> void {
    std::size_t attempt = 0;
    while (attempt < MAX_INBOUND_SEND_ATTEMPTS) {
      if (xQueueSend(INBOUND_QUEUE, (void *)&event, 10) != pdTRUE) {
        ESP_LOGI(LOG_BLE_INFO_TAG.c_str(), "INBOUND_QUEUE is full");
        vTaskDelay(500);
        ++attempt;
      } else {
        return;
      }
    }
  }

  auto handleClientMessage(const char *const msg) -> void {
    auto data = json::parse(msg);
    auto msg_type = static_cast<std::string>(data.at("type"));
    if (msg_type.compare("command") == 0) {
      std::cout << static_cast<BLE::events::Event>(data.at("mode"))
                << std::endl;
      switch (static_cast<BLE::events::Event>(data.at("mode"))) {
        case BLE::events::REQ_BENCHMARK1:
          sendEventToInboundQueue(BLE::events::REQ_BENCHMARK1);
          break;
        case BLE::events::REQ_BENCHMARK2:
          sendEventToInboundQueue(BLE::events::REQ_BENCHMARK1);
          break;
        case BLE::events::REQ_BENCHMARK3:
          sendEventToInboundQueue(BLE::events::REQ_BENCHMARK1);
          break;
        case BLE::events::REQ_STOP_BENCHMARK:
          sendEventToInboundQueue(BLE::events::REQ_STOP_BENCHMARK);
          break;
        default:
          return;
      }
    }
  }

  auto device_write(uint16_t conn_handle, uint16_t attr_handle,
                    struct ble_gatt_access_ctxt *ctxt, void *arg) -> int {

    handleClientMessage((char *)ctxt->om->om_data);
    // printf("Data from the client: %.*s\n", ctxt->om->om_len,
    // ctxt->om->om_data);
    return 0;
  }

  auto device_read(uint16_t con_handle, uint16_t attr_handle,
                   struct ble_gatt_access_ctxt *ctxt, void *arg) -> int {

    char *outboundMessage;
    if (xQueueReceive(OUTBOUND_QUEUE, &outboundMessage, portMAX_DELAY) ==
        pdTRUE) {
      std::cout << outboundMessage << std::endl;
      os_mbuf_append(ctxt->om, outboundMessage, strlen(outboundMessage));
    }

    return 0;
  }

  auto getGattSvcDefs() -> ble_gatt_svc_def * {
    static const auto svc_uuid = ble_uuid16_t{BLE_UUID_TYPE_16, 0x180};
    static const auto chr_read_uuid = ble_uuid16_t{BLE_UUID_TYPE_16, 0xFEF4};
    static const auto chr_write_uuid = ble_uuid16_t{BLE_UUID_TYPE_16, 0xDFE1};

    ble_gatt_chr_def gattSvcBenchmarkChrRead = {
        (ble_uuid_t *)&chr_read_uuid, device_read, nullptr, nullptr,
        BLE_GATT_CHR_F_READ,          0,           nullptr};
    ble_gatt_chr_def gattSvcBenchmarkChrWrite = {(ble_uuid_t *)&chr_write_uuid,
                                                 device_write,
                                                 nullptr,
                                                 nullptr,
                                                 BLE_GATT_CHR_F_WRITE,
                                                 0,
                                                 nullptr};
    ble_gatt_chr_def gattSvcBenchmarkChrTerm;
    memset(&gattSvcBenchmarkChrTerm, 0, sizeof(gattSvcBenchmarkChrTerm));

    static ble_gatt_chr_def gattSvcBenchmarkChrs[] = {gattSvcBenchmarkChrRead,
                                                      gattSvcBenchmarkChrWrite,
                                                      gattSvcBenchmarkChrTerm};

    ble_gatt_svc_def gattSvcBenchmark = {BLE_GATT_SVC_TYPE_PRIMARY,
                                         (ble_uuid_t *)&svc_uuid, nullptr,
                                         gattSvcBenchmarkChrs

    };
    ble_gatt_svc_def gattSvcTerm; // see definition for ble_gatts_add_svcs()
    memset(&gattSvcTerm, 0, sizeof(gattSvcTerm));

    static ble_gatt_svc_def gatt_svcs[] = {gattSvcBenchmark, gattSvcTerm};
    return gatt_svcs;
  }

  // BLE event handling
  static auto ble_gap_event(struct ble_gap_event *event, void *arg) -> int {
    switch (event->type) {
      // Advertise if connected
      case BLE_GAP_EVENT_CONNECT:
        if (event->connect.status == 0) {
          ESP_LOGI(LOG_BLE_INFO_TAG.c_str(),
                   "Sending BLE_CONN event to INBOUND_QUEUE");
          sendEventToInboundQueue(BLE::events::BLE_CONN);
        } else {
          ble_app_advertise();
        }
        break;
      case BLE_GAP_EVENT_DISCONNECT:
        ble_app_advertise();
        ESP_LOGI(LOG_BLE_INFO_TAG.c_str(),
                 "Sending BLE_DISCONN event to INBOUND_QUEUE");
        sendEventToInboundQueue(BLE::events::BLE_DISCONN);
        break;
      default:
        break;
    }
    return 0;
  }

  auto ble_app_advertise() -> void {
    auto device_name = ble_svc_gap_device_name(); // Read the BLE device name

    struct ble_hs_adv_fields fields;
    memset(&fields, 0, sizeof(fields));
    fields.name = (uint8_t *)device_name;
    fields.name_len = strlen(device_name);
    fields.name_is_complete = 1;

    ble_gap_adv_set_fields(&fields);

    // GAP - device connectivity definition
    struct ble_gap_adv_params adv_params;
    memset(&adv_params, 0, sizeof(adv_params));
    adv_params.conn_mode =
        BLE_GAP_CONN_MODE_UND; // connectable or non-connectable
    adv_params.disc_mode =
        BLE_GAP_DISC_MODE_GEN; // discoverable or non-discoverable
    ble_gap_adv_start(ble_addr_type, NULL, BLE_HS_FOREVER, &adv_params,
                      ble_gap_event, NULL);
  }

  auto ble_app_on_sync() -> void {
    ble_hs_id_infer_auto(
        0, &ble_addr_type); // Determines the best address type automatically
    ble_app_advertise();    // Define the BLE connection
  }

  auto host_task(void *param) -> void {
    nimble_port_run(); // This function will return only when
                       // nimble_port_stop() is executed
  }
}

namespace BLE {
  auto initBLEService() -> void {
    nvs_flash_init();                     // 1 - Initialize NVS flash using
    esp_nimble_hci_and_controller_init(); // 2 - Initialize ESP controller
    nimble_port_init();                   // 3 - Initialize the host stack
    ble_svc_gap_device_name_set(
        DEVICE_NAME
            .c_str());   // 4 - Initialize NimBLE configuration - server name
    ble_svc_gap_init();  // 4 - Initialize NimBLE configuration - gap service
    ble_svc_gatt_init(); // 4 - Initialize NimBLE configuration - gatt service
    const auto gatt_svcs_def = getGattSvcDefs();
    ble_gatts_count_cfg(gatt_svcs_def);   // 4 - Initialize NimBLE configuration
                                          // - config gatt services
    ble_gatts_add_svcs(gatt_svcs_def);    // 4 - Initialize NimBLE configuration
                                          // - queues gatt services.
    ble_hs_cfg.sync_cb = ble_app_on_sync; // 5 - Initialize application
  }

  auto startBLEService() -> void { nimble_port_freertos_init(host_task); }

}
#endif