#ifndef BLE_MSG_H
#define BLE_MSG_H
#define JSON_NO_IO

#include "lib/json.hpp"
#include <string>
#include <unordered_map>

namespace {
  using json = nlohmann::json;
}

namespace BLE::messages {

  auto generateOutboundMessage(
      const int64_t &timestamp, const std::string &memFuncName,
      const std::unordered_map<std::string, float> &maxMinInfMap,
      const std::unordered_map<std::string, float> &maxProdInfMap)
      -> std::string {

    json msg = {{"timestamp", timestamp},
                {"membershipFunction", memFuncName},
                {"MaxMinInference",
                 {{"MeanMaxima", maxMinInfMap.at("MeanMaxima")},
                  {"CenterofGravity", maxMinInfMap.at("CenterofGravity")},
                  {"WeightedAverage", maxMinInfMap.at("WeightedAverage")}}},
                {"MaxProductInference",
                 {{"MeanMaxima", maxProdInfMap.at("MeanMaxima")},
                  {"CenterofGravity", maxProdInfMap.at("CenterofGravity")},
                  {"WeightedAverage", maxProdInfMap.at("WeightedAverage")}}}};

    return msg.dump();
  }

}

namespace BLE::messages::constants {
  static const auto messageOutboundSize = sizeof(char *);
}
#endif