#include "ble/stack.hpp"
#include "config/app/peripheral.hpp"
#include <iostream>

uint8_t twinperipheral::ble::Stack::BLE_ADDR_TYPE = 0;
bool twinperipheral::ble::Stack::IS_RUNNING = false;
ble_gap_event_fn *twinperipheral::ble::Stack::_gapEventHandler = nullptr;
void *twinperipheral::ble::Stack::_gapEventHandlerArg = nullptr;
std::string twinperipheral::ble::Stack::TAG = "BLE stack";

extern "C" {
#include "services/ans/ble_svc_ans.h"
void ble_store_config_init(void);
}

void twinperipheral::ble::Stack::syncCallback(void) {
  /*
  auto rc = ble_hs_id_set_rnd(twinperipheral::config::app::bleAddr.val);
  rc = ble_hs_util_ensure_addr(1);
  rc = ble_hs_id_infer_auto(0, &twinperipheral::ble::Stack::BLE_ADDR_TYPE);
  */
  int rc = ble_hs_util_ensure_addr(0);
  rc = ble_hs_id_infer_auto(0, &twinperipheral::ble::Stack::BLE_ADDR_TYPE);
  assert(rc == 0);
  if (_gapEventHandler != nullptr && _gapEventHandlerArg != nullptr) {
    twinperipheral::ble::Stack::Peripheral::startAdvertising(
        _gapEventHandler, _gapEventHandlerArg);
  }
};

void twinperipheral::ble::Stack::hostTask(void *params) {
  nimble_port_run();
  // Prevent from task return, when nimble_port_stop is called
  while (1) {
    vTaskDelay(10);
  }
}

void twinperipheral::ble::Stack::start(const std::string bleName,
                                       ble_gap_event_fn *gapEventHandler,
                                       void *gapEventHandlerArg,
                                       const ble_gatt_svc_def *gattSvcDefs) {

  twinperipheral::ble::Stack::_gapEventHandler = gapEventHandler;
  twinperipheral::ble::Stack::_gapEventHandlerArg = gapEventHandlerArg;

  nvs_flash_init();
  nimble_port_init();

  ble_store_config_init();

  ble_svc_gap_device_name_set(bleName.c_str());

  /* Activate Peripheral functionality */
  if (gattSvcDefs != nullptr) {
    ble_svc_gap_init();
    ble_svc_gatt_init();
    ble_svc_ans_init();

    twinperipheral::ble::Stack::Peripheral::configureGattServices(gattSvcDefs);
  }

  ble_hs_cfg.sync_cb = twinperipheral::ble::Stack::syncCallback;

  nimble_port_freertos_init(twinperipheral::ble::Stack::hostTask);
  IS_RUNNING = true;
}

void twinperipheral::ble::Stack::stop() {
  if (IS_RUNNING) {
    auto rc = nimble_port_stop();
    if (rc == ESP_OK) {
      nimble_port_deinit();
      nimble_port_freertos_deinit();
      IS_RUNNING = false;
    } else {
      ESP_LOGI(TAG.c_str(), "Stopping BLE server has failed");
    }
  }
}

std::string twinperipheral::ble::Stack::Peripheral::pendingMessage;

std::atomic_bool
    twinperipheral::ble::Stack::Peripheral::IS_FOR_SENDING_MEASUREMENT = false;

std::mutex twinperipheral::ble::Stack::Peripheral::mtx;

void twinperipheral::ble::Stack::Peripheral::placeMessageToSend(
    std::string message) {
  const std::lock_guard<std::mutex> lock(mtx);
  pendingMessage = message;
}

std::string twinperipheral::ble::Stack::Peripheral::fetchPendingMessage() {
  const std::lock_guard<std::mutex> lock(mtx);
  return pendingMessage;
}

void twinperipheral::ble::Stack::Peripheral::purgePendingMessage() {
  const std::lock_guard<std::mutex> lock(mtx);
  pendingMessage = "";
}

void twinperipheral::ble::Stack::Peripheral::configureGattServices(
    const ble_gatt_svc_def *gattSvcDefs) {
  ble_gatts_count_cfg(gattSvcDefs);
  ble_gatts_add_svcs(gattSvcDefs);
}

void twinperipheral::ble::Stack::Peripheral::startAdvertising(
    ble_gap_event_fn *gapEventHandler, void *gapEventHandlerArg) {
  auto deviceName = ble_svc_gap_device_name();
  ble_hs_adv_fields fields;

  memset(&fields, 0, sizeof(fields));

  fields.name = (uint8_t *)deviceName;
  fields.name_len = strlen(deviceName);
  fields.name_is_complete = 1;

  fields.flags = BLE_HS_ADV_F_DISC_GEN | BLE_HS_ADV_F_BREDR_UNSUP;
  fields.tx_pwr_lvl_is_present = 1;
  fields.tx_pwr_lvl = BLE_HS_ADV_TX_PWR_LVL_AUTO;

  ble_gap_adv_set_fields(&fields);

  ble_gap_adv_params adv_params;
  memset(&adv_params, 0, sizeof(adv_params));
  const ble_addr_t *directAddr = NULL;

  if (twinperipheral::config::app::acceptOnlyDirectedAdvertisement) {
    adv_params.conn_mode =
        BLE_GAP_CONN_MODE_DIR; // connectable or non-connectable
    directAddr = &centralAddr;
  } else {
    adv_params.conn_mode =
        BLE_GAP_CONN_MODE_UND; // connectable or non-connectable
  }

  adv_params.disc_mode =
      BLE_GAP_DISC_MODE_GEN; // discoverable or non-discoverable

  ble_gap_adv_start(twinperipheral::ble::Stack::BLE_ADDR_TYPE, directAddr,
                    BLE_HS_FOREVER, &adv_params, gapEventHandler,
                    gapEventHandlerArg);
}

void twinperipheral::ble::Stack::Peripheral::stopAdvertising() {
  ble_gap_adv_stop();
}
