#ifndef TWINPERIPHERAL_SHARED_CONFIG_INCLUDED
#define TWINPERIPHERAL_SHARED_CONFIG_INCLUDED

#define TWINCRUTCH_R_SVC_SENSOR_DATA_UUID                                      \
  ble_uuid16_t { BLE_UUID_TYPE_16, 0xB181 }
#define TWINCRUTCH_L_SVC_SENSOR_DATA_UUID                                      \
  ble_uuid16_t { BLE_UUID_TYPE_16, 0xB181 }
#define TWININSOLE_R_SVC_SENSOR_DATA_UUID                                      \
  ble_uuid16_t { BLE_UUID_TYPE_16, 0xB181 }
#define TWININSOLE_L_SVC_SENSOR_DATA_UUID                                      \
  ble_uuid16_t { BLE_UUID_TYPE_16, 0xB181 }

#define TWINPERIPHERAL_CHR_READ_SENSOR_DATA_UUID                               \
  ble_uuid16_t { BLE_UUID_TYPE_16, 0xCBF6 }

#include <chrono>
#include <string>
#include <tuple>
#include <unordered_map>

struct BLEAddrHash {
  auto operator()(const ble_addr_t &key) const -> size_t {
    uint64_t val = (uint64_t)(static_cast<uint64_t>(key.val[5]) << 40 |
                              static_cast<uint64_t>(key.val[4]) << 32 |
                              static_cast<uint64_t>(key.val[3]) << 24 |
                              static_cast<uint64_t>(key.val[2]) << 16 |
                              static_cast<uint64_t>(key.val[1]) << 8 |
                              static_cast<uint64_t>(key.val[0]));

    return std::hash<uint64_t>{}(val);
  }
};

class BleAddrEqual {
public:
  bool operator()(const ble_addr_t &addr1, const ble_addr_t &addr2) const {
    if (addr1.type != addr2.type)
      return false;
    for (int i = 0; i < 6; ++i) {
      if (addr1.val[i] != addr2.val[i])
        return false;
    }
    return true;
  }
};

static const std::string twinCrutchRightBleName = "TwinCrutch-R";
static const std::string twinCrutchLeftBleName = "TwinCrutch-L";
static const std::string twinInsoleRightBleName = "TwinInsole-R";
static const std::string twinInsoleLeftBleName = "TwinInsole-L";

static const ble_addr_t centralAddr{BLE_OWN_ADDR_RANDOM,
                                    {0xCC, 0x23, 0xFB, 0x45, 0xBF, 0x11}};

static const std::unordered_map<ble_addr_t,
                                std::tuple<std::string, ble_uuid16_t>,
                                BLEAddrHash, BleAddrEqual>
    bleTwinPeripheralsByAddr{
        {ble_addr_t{BLE_OWN_ADDR_RANDOM, {0x2B, 0x23, 0xFB, 0x45, 0xA1, 0x11}},
         std::make_tuple(twinCrutchRightBleName,
                         TWINCRUTCH_R_SVC_SENSOR_DATA_UUID)},
        {ble_addr_t{BLE_OWN_ADDR_RANDOM, {0x2B, 0x23, 0xFB, 0x45, 0xA2, 0x11}},
         std::make_tuple(twinCrutchLeftBleName,
                         TWINCRUTCH_L_SVC_SENSOR_DATA_UUID)},
        {ble_addr_t{BLE_OWN_ADDR_RANDOM, {0x2C, 0x23, 0xFB, 0x45, 0xA1, 0x11}},
         std::make_tuple(twinInsoleRightBleName,
                         TWININSOLE_R_SVC_SENSOR_DATA_UUID)},
        {ble_addr_t{BLE_OWN_ADDR_RANDOM, {0x2C, 0x23, 0xFB, 0x45, 0xA2, 0x11}},
         std::make_tuple(twinInsoleLeftBleName,
                         TWININSOLE_L_SVC_SENSOR_DATA_UUID)}};

static const std::unordered_map<std::string,
                                std::tuple<ble_addr_t, ble_uuid16_t>>
    bleTwinPeripheralsByName{
        {"TwinCrutch-R",
         std::make_tuple(ble_addr_t{BLE_OWN_ADDR_RANDOM,
                                    {0x2B, 0x23, 0xFB, 0x45, 0xA1, 0x11}},
                         TWINCRUTCH_R_SVC_SENSOR_DATA_UUID)},
        {"TwinCrutch-L",
         std::make_tuple(ble_addr_t{BLE_OWN_ADDR_RANDOM,
                                    {0x2B, 0x23, 0xFB, 0x45, 0xA2, 0x11}},
                         TWINCRUTCH_L_SVC_SENSOR_DATA_UUID)},
        {"TwinInsole-R",
         std::make_tuple(ble_addr_t{BLE_OWN_ADDR_RANDOM,
                                    {0x2C, 0x23, 0xFB, 0x45, 0xA1, 0x11}},
                         TWININSOLE_R_SVC_SENSOR_DATA_UUID)},
        {"TwinInsole-L",
         std::make_tuple(ble_addr_t{BLE_OWN_ADDR_RANDOM,
                                    {0x2C, 0x23, 0xFB, 0x45, 0xA2, 0x11}},
                         TWININSOLE_L_SVC_SENSOR_DATA_UUID)},
    };

#endif