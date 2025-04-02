#ifndef TWINPERIPHERAL_BLE_MSG_INCLUDED
#define TWINPERIPHERAL_BLE_MSG_INCLUDED
#define JSON_NO_IO

#include "BNO055ESP32.h"
#include "cppjson/json.hpp"
#include <string>
#include <cmath>
#include <unordered_map>

namespace {
  using json = nlohmann::json;
}

namespace twinperipheral::ble {
  template <typename T>
  static inline auto
  generateBLEMessage(bno055_vector_t &&euler, bno055_vector_t &&linearAccel,
                     bno055_quaternion_t &&quaternion, std::vector<T> adcValues)
      -> std::string {

    auto roundToFourDecimalPlaces = [](double d) -> double {
      return std::round(d * 10000.0) / 10000.0;
    };

    json msg = {
        {"EA", {{"x", roundToFourDecimalPlaces(euler.x)}, {"y", roundToFourDecimalPlaces(euler.y)}, {"z", roundToFourDecimalPlaces(euler.z)}}},
        {"LA",
         {{"x", roundToFourDecimalPlaces(linearAccel.x)}, {"y", roundToFourDecimalPlaces(linearAccel.y)}, {"z", roundToFourDecimalPlaces(linearAccel.z)}}},
        {"Q",
         {{"w", roundToFourDecimalPlaces(quaternion.w)},
          {"x", roundToFourDecimalPlaces(quaternion.x)},
          {"y", roundToFourDecimalPlaces(quaternion.y)},
          {"z", roundToFourDecimalPlaces(quaternion.z)}}}};

    if (adcValues.size() == 1) {
      msg["LOAD"] = roundToFourDecimalPlaces(static_cast<double>(adcValues[0]));
      msg["FV"] = {{"x", 0.0f}, {"y", 0.0f}, {"z", 0.0f}}; // TODO
    } else {
      msg["FSR"] = {{"H", adcValues[0]},
                    {"5thM", adcValues[1]},
                    {"1thM", adcValues[2]}};
    }

    return msg.dump();
  }

  static inline auto generateBLEMessage(bno055_calibration_t calibrationStatus)
      -> std::string {
    json msg = {{"CalibrationStatus",
                 {{"sys", calibrationStatus.sys},
                  {"gyro", calibrationStatus.gyro},
                  {"mag", calibrationStatus.mag},
                  {"accel", calibrationStatus.accel}}}};

    return msg.dump();
  }
}
#endif