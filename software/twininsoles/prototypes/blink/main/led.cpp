#include "led.hpp"

TwinInsoleRGDLED::TwinInsoleRGDLED(int ledRedPin, int ledGreenPin,
                                   int ledBluePin)
    : LED_RED_PIN(ledRedPin), LED_GREEN_PIN(ledGreenPin),
      LED_BLUE_PIN(ledBluePin) {
  ledc_timer_config_t ledc_timer_conf = {
      .speed_mode = LEDC_LOW_SPEED_MODE,
      .duty_resolution = LEDC_TIMER_13_BIT,
      .timer_num = ledc_timer,
      .freq_hz = LED_STEADY, // Set output frequency at 1 kHz
      .clk_cfg = LEDC_AUTO_CLK};
  ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer_conf));
}

void TwinInsoleRGDLED::init() {
  ledc_channels[0].channel = LEDC_CHANNEL_0;
  ledc_channels[0].gpio_num = LED_RED_PIN;
  ledc_channels[1].channel = LEDC_CHANNEL_1;
  ledc_channels[1].gpio_num = LED_GREEN_PIN;

  ledc_channels[2].channel = LEDC_CHANNEL_2;
  ledc_channels[2].gpio_num = LED_BLUE_PIN;
  for (int i = 0; i < 3; i++) {
    ledc_channels[i].speed_mode = LEDC_LOW_SPEED_MODE;
    ledc_channels[i].timer_sel = LEDC_TIMER_0;
    ledc_channels[i].intr_type = LEDC_INTR_DISABLE;
    ledc_channels[i].duty = 0;
    ledc_channels[i].hpoint = 0;

    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channels[i]));
  }
}

void TwinInsoleRGDLED::notifyIsOperational() {
  setColor(colorGreen);
  setTimerFrequency(LED_STEADY);
}

void TwinInsoleRGDLED::setColor(color_t color) {
  uint32_t red_duty = 8191 * color.r / 255;
  uint32_t green_duty = 8191 * color.g / 255;
  uint32_t blue_duty = 8191 * color.b / 255;
  ESP_ERROR_CHECK(ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, red_duty));
  ESP_ERROR_CHECK(ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0));
  ESP_ERROR_CHECK(
      ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, green_duty));
  ESP_ERROR_CHECK(ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1));
  ESP_ERROR_CHECK(
      ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_2, blue_duty));
  ESP_ERROR_CHECK(ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_2));
}

void TwinInsoleRGDLED::setTimerFrequency(uint32_t freq_hz) {
  ledc_set_freq(LEDC_LOW_SPEED_MODE, ledc_timer, freq_hz);
}
