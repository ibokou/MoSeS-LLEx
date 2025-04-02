#ifndef TWIN_LED
#define TWIN_LED

#include "driver/ledc.h"
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <array>

#define LED_STEADY (uint32_t)1000
#define LED_BLINK_FAST (uint32_t)15
#define LED_BLINK_SLOW (uint32_t)2

typedef struct {
  uint8_t r;
  uint8_t g;
  uint8_t b;
} color_t;

class TwinInsoleRGDLED {
public:
  TwinInsoleRGDLED(int ledRedPin, int ledGreenPin, int ledBluePin);
  void init();
  void notifyIsOperational();

private:
  void setColor(color_t color);
  void setTimerFrequency(uint32_t freq);
  const color_t colorRed{.r = 255, .g = 0, .b = 0};
  const color_t colorGreen{.r = 0, .g = 255, .b = 0};
  const color_t colorBlue = {.r = 0, .g = 0, .b = 255};
  int LED_RED_PIN;
  int LED_GREEN_PIN;
  int LED_BLUE_PIN;
  ledc_timer_t ledc_timer = LEDC_TIMER_0;
  std::array<ledc_channel_config_t, 3> ledc_channels{};
};

#endif