/*
To prevent naming conflics in context of min max functions with C++-STL, we
include the headers first that use STL. After that headers are included that
include ESP-IDF specific header files that define min max macro which causes the
naming conflict

See:
https://stackoverflow.com/questions/41093090/esp8266-error-macro-min-passed-3-arguments-but-takes-just-2
*/

// clang-format off
#include "ble/msg.hpp"
#include <chrono>
#include "state_machine.hpp"
#include "benchmark.hpp"
#include "ble/ble.hpp"
#include "ble/events.hpp"
// clang-format on
#include "esp_log.h"
#include "freertos/queue.h"

#define INBOUND_QUEUE_SIZE 10
#define OUTBOUND_QUEUE_SIZE 10

extern "C" {
void app_main(void);
}

auto const LOG_APP_INFO_TAG = std::string("STATE_INFO");

static SemaphoreHandle_t state_mutex = xSemaphoreCreateMutex();
static SemaphoreHandle_t time_mutex = xSemaphoreCreateMutex();

static constexpr BaseType_t pro_cpu = 0;
static constexpr BaseType_t app_cpu = 1;

static auto currentState = IDLE;

QueueHandle_t INBOUND_QUEUE =
    xQueueCreate(INBOUND_QUEUE_SIZE, sizeof(BLE::events::Event));

QueueHandle_t OUTBOUND_QUEUE = xQueueCreate(
    INBOUND_QUEUE_SIZE, BLE::messages::constants::messageOutboundSize);

std::chrono::steady_clock::time_point BENCHMARK_BEGIN_TIME;

auto initADCPins() -> void {
  adc1_config_width(ADC_WIDTH_12Bit);
  adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_11);
  adc1_config_channel_atten(ADC1_CHANNEL_7, ADC_ATTEN_DB_11);
  adc1_config_channel_atten(ADC1_CHANNEL_4, ADC_ATTEN_DB_11);
  adc1_config_channel_atten(ADC1_CHANNEL_5, ADC_ATTEN_DB_11);
}

auto setBenchmarkStartTime() -> void {
  if (xSemaphoreTake(time_mutex, 0) == pdTRUE) {
    ESP_LOGI(LOG_BENCHMARK_INFO_TAG.c_str(), "Changing benchmark start time");
    BENCHMARK_BEGIN_TIME = std::chrono::steady_clock::now();
    xSemaphoreGive(time_mutex);
  }
}

auto handleEvent(BLE::events::Event &currentEvent) -> void {
  if (currentEvent == BLE::events::BLE_CONN) {
    vTaskResume(xTaskBlinkLedSlowHandle);
    vTaskSuspend(xTaskBlinkLedFastHandle);
  }

  if (currentEvent >= BLE::events::REQ_BENCHMARK1 &&
      !(currentEvent > BLE::events::REQ_BENCHMARK3)) {
    vTaskSuspend(xTaskBlinkLedSlowHandle);
    vTaskResume(xTaskBlinkLedFastHandle);
    setBenchmarkStartTime();
  }

  if (currentEvent == BLE::events::BLE_DISCONN ||
      currentEvent == BLE::events::REQ_STOP_BENCHMARK) {
    vTaskSuspend(xTaskBlinkLedSlowHandle);
    vTaskSuspend(xTaskBlinkLedFastHandle);
    turnOnLED();
    xQueueReset(OUTBOUND_QUEUE);
  }
}

auto setState(BLE::events::Event &currentEvent) -> void {
  if (xSemaphoreTake(state_mutex, 0) == pdTRUE) {
    currentState = getNextState(currentState, currentEvent);
    xSemaphoreGive(state_mutex);
  }
}

auto handleEvents(void *parameters) -> void {
  while (true) {
    BLE::events::Event currentEvent;
    if (xQueueReceive(INBOUND_QUEUE, &currentEvent, (TickType_t)10) == pdTRUE) {
      handleEvent(currentEvent);
      setState(currentEvent);
    }
  }
  vTaskDelay(500);
}

auto doStateMachine(void *parameters) -> void {
  while (true) {
    State state = NA;
    if (xSemaphoreTake(state_mutex, 0) == pdTRUE) {
      state = currentState;
      xSemaphoreGive(state_mutex);
      switch (state) {
        case IDLE:
          ESP_LOGI(LOG_APP_INFO_TAG.c_str(), "Current state: IDLE");
          break;
        case IN_BLE_CONN:
          ESP_LOGI(LOG_APP_INFO_TAG.c_str(), "Current state: IN_BLE_CONN");
          break;
        case IN_BENCHMARK1:
          ESP_LOGI(LOG_APP_INFO_TAG.c_str(), "Current state: IN_BENCHMARK1");
          startBenchmark1(BENCHMARK_BEGIN_TIME, OUTBOUND_QUEUE);
          break;
        case IN_BENCHMARK2:
          ESP_LOGI(LOG_APP_INFO_TAG.c_str(), "Current state: IN_BENCHMARK2");
          startBenchmark2(BENCHMARK_BEGIN_TIME, OUTBOUND_QUEUE);
          break;
        case IN_BENCHMARK3:
          ESP_LOGI(LOG_APP_INFO_TAG.c_str(), "Current state: IN_BENCHMARK3");
          startBenchmark3(BENCHMARK_BEGIN_TIME, OUTBOUND_QUEUE);
          break;
        default:
          ESP_LOGI(LOG_APP_INFO_TAG.c_str(), "Current state: N/A");
      }
      vTaskDelay(100);
    }
  }
}

auto app_main(void) -> void {

  configureLedGPIO();
  turnOnLED();

  initADCPins();

  xTaskCreatePinnedToCore(blinkLEDSlow, "Blink LED slow", 2048, NULL, 4,
                          &xTaskBlinkLedSlowHandle, app_cpu);

  xTaskCreatePinnedToCore(blinkLEDFast, "Blink LED fast", 2048, NULL, 4,
                          &xTaskBlinkLedFastHandle, app_cpu);

  xTaskCreatePinnedToCore(initLED, "Init LED", 2048, NULL, 20, NULL, app_cpu);

  xTaskCreatePinnedToCore(handleEvents, "Handle Events", 2048, NULL, 10, NULL,
                          app_cpu);

  xTaskCreatePinnedToCore(doStateMachine, "State Machine", 20480, NULL, 9, NULL,
                          app_cpu);
  BLE::initBLEService();
  BLE::startBLEService(); // BLE is tied to core 0 (pro_cpu)
}