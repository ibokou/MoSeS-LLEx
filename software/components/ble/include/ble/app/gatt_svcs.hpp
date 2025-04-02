#ifndef TWINPERIPHERAL_BLE_APP_GATT_SVCS_INCLUDED
#define TWINPERIPHERAL_BLE_APP_GATT_SVCS_INCLUDED

#include "host/ble_hs.h"

namespace twinperipheral::ble::app::gatt_svcs {

  const ble_gatt_svc_def *getGattSvcDefs(ble_gatt_access_fn *readCb, void *arg);
}

#endif