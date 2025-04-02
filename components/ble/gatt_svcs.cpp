#include "ble/app/gatt_svcs.hpp"
#include "config/app/peripheral.hpp"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
const ble_gatt_svc_def *
twinperipheral::ble::app::gatt_svcs::getGattSvcDefs(ble_gatt_access_fn *readCb,
                                                    void *arg) {

  static const ble_gatt_chr_def chrDef[]{
      {.uuid = (ble_uuid_t *)&twinperipheral::config::app::
           bleChrReadUUID, // Define UUID for reading
       .access_cb = readCb,
       .arg = arg,
       .flags = BLE_GATT_CHR_F_READ},
      {0}};

  static const ble_gatt_svc_def gattSvcs[] = {
      {.type = BLE_GATT_SVC_TYPE_PRIMARY,
       .uuid = (ble_uuid_t *)&twinperipheral::config::app::bleSvcUUID,
       .characteristics = chrDef},
      {0}};

  return gattSvcs;
};
#pragma GCC diagnostic pop
