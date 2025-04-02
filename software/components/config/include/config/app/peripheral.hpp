#ifndef TWINCENTRAL_APP_CONFIG_INCLUDED
#define TWINCENTRAL_APP_CONFIG_INCLUDED

#define TASK_DEFAULT_PRIORITY 1
#define TASK_DEFAULT_STACK NULL
#define TASK_DEFAULT_STACK_SIZE 400

#include "host/ble_hs.h"
#include "host/util/util.h"
#include "nimble/nimble_npl.h"
#include "nimble/nimble_port.h"

#include "../../../shared-config/ble.h"

namespace twinperipheral::config::app {

  static const BaseType_t pro_cpu = 0;
  static const BaseType_t app_cpu = 1;

  static constexpr uint32_t idleTime = (uint32_t)120 * 60 * 1000; // 45 minutes

  static const auto acceptOnlyDirectedAdvertisement = false;
  static const auto directedAdvAddr = centralAddr;

#ifdef TWIN_INSOLE_RIGHT

  static const std::string blePeripheralDeviceName = twinInsoleRightBleName;

#elif TWIN_INSOLE_LEFT

  static const std::string blePeripheralDeviceName = twinInsoleLeftBleName;

#elif TWIN_CRUTCH_RIGHT

  static const std::string blePeripheralDeviceName = twinCrutchRightBleName;

#elif TWIN_CRUTCH_LEFT

  static const std::string blePeripheralDeviceName = twinCrutchLeftBleName;

#else

#  error                                                                       \
      "No target(TWINCRUTCH_RIGHT|TWINCRUTCH_LEFT|TWININSOLE_RIGHT|TWININSOLE_LEFT) defined"

#endif

  static const auto bleAddr =
      std::get<0>(bleTwinPeripheralsByName.at(blePeripheralDeviceName));

  static const auto bleSvcUUID =
      std::get<1>(bleTwinPeripheralsByName.at(blePeripheralDeviceName));

  static const auto bleChrReadUUID = TWINPERIPHERAL_CHR_READ_SENSOR_DATA_UUID;

}
#endif