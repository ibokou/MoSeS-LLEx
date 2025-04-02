#include "driver/adc.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <chrono>
#include <memory>
#include <stdio.h>
#include <string>

extern "C" {
void app_main(void);
}

auto const LOG_INFO_TAG = std::string("INFO");

auto blink_led(const gpio_num_t &gpio_num, uint32_t &&level) -> void {
  gpio_set_level(gpio_num, level);
}

static void configure_gpio(const gpio_num_t &&BLINK_GPIO,
                           const gpio_mode_t &&mode) {
  ESP_LOGI(LOG_INFO_TAG.c_str(), "Configuring GPIO: %d", BLINK_GPIO);
  gpio_reset_pin(BLINK_GPIO);
  gpio_set_direction(BLINK_GPIO, mode);
}

auto app_main(void) -> void {

  // TURN ON ONBOARD LED
  configure_gpio(GPIO_NUM_2, GPIO_MODE_OUTPUT);
  blink_led(GPIO_NUM_2, (uint32_t)1);

  // READ VOLTAGE FROM FSR (GPIO34/ADC1_6)

  /*
   * ADC1_CHANNEL_0: FSR on Heel
   * ADC1_CHANNEL_4: FSR on lateral side
   * ADC1_CHANNEL_6: FSR ON 5th metatarsal
   * ADC1_CHANNEL_7: FSR ON 1th metatarsal
   */

  adc1_config_width(ADC_WIDTH_12Bit);
  adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_11);
  adc1_config_channel_atten(ADC1_CHANNEL_3, ADC_ATTEN_DB_11);
  adc2_config_channel_atten(ADC2_CHANNEL_3, ADC_ATTEN_DB_11);
  adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_11);
  adc1_config_channel_atten(ADC1_CHANNEL_7, ADC_ATTEN_DB_11);

  for (;;) {
    vTaskDelay(1000);

    std::chrono::steady_clock::time_point begin =
        std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point end =
        std::chrono::steady_clock::now();

    int val_0 = adc1_get_raw(ADC1_CHANNEL_0);
    int val_3 = adc1_get_raw(ADC1_CHANNEL_3);
    int val_2_3 = 0;
    adc2_get_raw(ADC2_CHANNEL_3, ADC_WIDTH_12Bit, &val_3);
    int val_6 = adc1_get_raw(ADC1_CHANNEL_6);
    int val_7 = adc1_get_raw(ADC1_CHANNEL_7);

    auto duration_micros =
        std::chrono::duration_cast<std::chrono::microseconds>(end - begin)
            .count();
    auto duration_ns =
        std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin)
            .count();

    ESP_LOGI(LOG_INFO_TAG.c_str(),
             "Time elapsed for reading in µs %lld and in ns %lld",
             duration_micros, duration_ns);

    ESP_LOGI(LOG_INFO_TAG.c_str(),
             "Voltage values (ADC0, ADC1, ADC6, ADC7): "
             "%d, %d, %d, %d",
             val_0, val_3, val_6, val_7);
  }
}