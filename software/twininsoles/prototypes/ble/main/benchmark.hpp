#ifndef BENCHMARK_H
#define BENCHMARK_H

// clang-format off
#include <chrono>
#include <string>
#include <map>
#include "ble/msg.hpp"
// clang-format on
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"

auto const LOG_BENCHMARK_INFO_TAG = std::string("BENCHMARK_INFO");

namespace {

  auto constexpr MAX_OUTBOUND_SEND_ATTEMPTS = 5;

  auto
  getCurrentTimestamp(const std::chrono::steady_clock::time_point &startTime)
      -> int64_t {

    std::chrono::steady_clock::time_point currentTime =
        std::chrono::steady_clock::now();
    const auto timestamp =
        std::chrono::duration_cast<std::chrono::milliseconds>(currentTime -
                                                              startTime)
            .count();
    return timestamp;
  }

  auto sendMessageToOutboundQueue(QueueHandle_t &xQueue, const char *const msg)
      -> void {
    std::size_t attempt = 0;
    while (attempt < MAX_OUTBOUND_SEND_ATTEMPTS) {
      if (xQueueSend(xQueue, (void *)&msg, 10) != pdTRUE) {
        ESP_LOGI(LOG_BENCHMARK_INFO_TAG.c_str(), "OUTBOUND_QUEUE is full");
        vTaskDelay(500);
        ++attempt;
      } else {
        return;
      }
    }
  }
}

static auto
startBenchmark1(const std::chrono::steady_clock::time_point &startTime,
                QueueHandle_t &xQueue) -> void {

  const std::unordered_map<std::string, float> maxMinInfMap = {
      {"MeanMaxima", 100.0f},
      {"CenterofGravity", 100.0f},
      {"WeightedAverage", 100.0f}};

  const std::unordered_map<std::string, float> maxProdInfMap = {
      {"MeanMaxima", 100.0f},
      {"CenterofGravity", 100.0f},
      {"WeightedAverage", 100.0f}};

  auto message = BLE::messages::generateOutboundMessage(
      getCurrentTimestamp(startTime), "Trapozoidal MF", maxMinInfMap,
      maxProdInfMap);

  static auto msg_ptr = message.c_str();

  sendMessageToOutboundQueue(xQueue, msg_ptr);
}

static auto
startBenchmark2(const std::chrono::steady_clock::time_point &startTime,
                QueueHandle_t &xQueue) -> void {

  const std::unordered_map<std::string, float> maxMinInfMap = {
      {"MeanMaxima", 100.0f},
      {"CenterofGravity", 100.0f},
      {"WeightedAverage", 100.0f}};

  const std::unordered_map<std::string, float> maxProdInfMap = {
      {"MeanMaxima", 100.0f},
      {"CenterofGravity", 100.0f},
      {"WeightedAverage", 100.0f}};

  auto message = BLE::messages::generateOutboundMessage(
      getCurrentTimestamp(startTime), "Sigmoid MF", maxMinInfMap,
      maxProdInfMap);

  sendMessageToOutboundQueue(xQueue, message.c_str());
}

static auto
startBenchmark3(const std::chrono::steady_clock::time_point &startTime,
                QueueHandle_t &xQueue) -> void {

  const std::unordered_map<std::string, float> maxMinInfMap = {
      {"MeanMaxima", 100.0f},
      {"CenterofGravity", 100.0f},
      {"WeightedAverage", 100.0f}};

  const std::unordered_map<std::string, float> maxProdInfMap = {
      {"MeanMaxima", 100.0f},
      {"CenterofGravity", 100.0f},
      {"WeightedAverage", 100.0f}};

  auto message = BLE::messages::generateOutboundMessage(
      getCurrentTimestamp(startTime), "Cauchy MF", maxMinInfMap, maxProdInfMap);

  sendMessageToOutboundQueue(xQueue, message.c_str());
}

#endif