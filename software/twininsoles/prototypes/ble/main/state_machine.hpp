#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

// clang-format off
#include "ble/events.hpp"
#include <iostream>

// clang-format on
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

enum State {
  IDLE,
  IN_BLE_CONN,
  IN_BENCHMARK1,
  IN_BENCHMARK2,
  IN_BENCHMARK3,
  NA
};

static TaskHandle_t xTaskBlinkLedSlowHandle;
static TaskHandle_t xTaskBlinkLedFastHandle;

static uint8_t led_slow_lvl = 0;
static uint8_t led_fast_lvl = 0;

static void configureLedGPIO() {
  gpio_reset_pin(GPIO_NUM_2);
  gpio_set_direction(GPIO_NUM_2, GPIO_MODE_OUTPUT);
}

static auto turnOnLED() -> void { gpio_set_level(GPIO_NUM_2, (uint32_t)1); }

static auto initLED(void *parameters) -> void {
  vTaskSuspend(xTaskBlinkLedSlowHandle);
  vTaskSuspend(xTaskBlinkLedFastHandle);

  configureLedGPIO();
  turnOnLED();
  vTaskDelete(NULL);
}

static auto blinkLEDSlow(void *parameters) -> void {
  while (true) {
    led_slow_lvl = ~led_slow_lvl;
    gpio_set_level(GPIO_NUM_2, led_slow_lvl);
    vTaskDelay(500 / portTICK_PERIOD_MS); // 500ms
  }
}

static auto blinkLEDFast(void *parameters) -> void {
  while (true) {
    led_fast_lvl = ~led_fast_lvl;
    gpio_set_level(GPIO_NUM_2, led_fast_lvl);
    vTaskDelay(100 / portTICK_PERIOD_MS); // 100ms
  }
}

static auto getNextState(State currentState, BLE::events::Event event)
    -> State {
  if (event == BLE::events::BLE_CONN) {
    return IN_BLE_CONN;
  }

  if (event == BLE::events::BLE_DISCONN) {
    return IDLE;
  }

  if (currentState == IN_BLE_CONN) {
    switch (event) {
      case BLE::events::REQ_BENCHMARK1:
        return IN_BENCHMARK1;
      case BLE::events::REQ_BENCHMARK2:
        return IN_BENCHMARK2;
      case BLE::events::REQ_BENCHMARK3:
        return IN_BENCHMARK3;
      default:
        return currentState;
    }
  }

  if (currentState == IN_BENCHMARK1 || currentState == IN_BENCHMARK1 ||
      currentState == IN_BENCHMARK1) {
    if (event == BLE::events::REQ_STOP_BENCHMARK) {
      return IDLE;
    }
  }

  return IDLE;
}
#endif