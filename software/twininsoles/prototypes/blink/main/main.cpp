/* LEDC (LED Controller) basic example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include "driver/ledc.h"
#include "esp_err.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led.hpp"
#include <stdio.h>

#define R_PIN 2
#define G_PIN 5
#define B_PIN 13

extern "C" {
void app_main(void);
}

void app_main(void) {
  TwinInsoleRGDLED led(R_PIN, G_PIN, B_PIN);
  led.init();
  led.notifyIsOperational();
  while (1) {
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}