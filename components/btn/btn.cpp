#include "btn/btn.hpp"

twinperipheral::button::DFRobotButton::DFRobotButton(const gpio_num_t &BTN_GPIO)
    : BTN_GPIO(BTN_GPIO) {
  ESP_LOGI(TAG.c_str(), "Configuring GPIO: %d", BTN_GPIO);
  gpio_reset_pin(BTN_GPIO);
  gpio_set_direction(BTN_GPIO, GPIO_MODE_INPUT);
  ESP_LOGI(TAG.c_str(), "Configuraton complete GPIO: %d", BTN_GPIO);
}

std::string twinperipheral::button::DFRobotButton::TAG = "DFRobotButton";

void twinperipheral::button::DFRobotButton::checkForBtnPressWithDuration(
    const int64_t ms, std::function<void()> cb) {
  int lastState = 0;
  std::chrono::steady_clock::time_point pressedTime;
  std::chrono::steady_clock::time_point releasedTime;

  while (1) {
    auto currentState = gpio_get_level(BTN_GPIO);
    //  lastState = LOW; currentState = LOW
    if (lastState == 0 && currentState > 0) // button is pressed
      pressedTime = std::chrono::steady_clock::now();
    // lastState = HIGH; currentState = LOW
    else if (lastState > 0 && currentState == 0) { // button is released
      releasedTime = std::chrono::steady_clock::now();

      auto pressDuration =
          std::chrono::duration_cast<std::chrono::milliseconds>(releasedTime -
                                                                pressedTime)
              .count();

      if (pressDuration > ms) {
        ESP_LOGI(TAG.c_str(), "Btn press detected");
        cb();
      }
    }

    lastState = currentState;
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

gpio_num_t twinperipheral::button::DFRobotButton::getGPIONum() {
  return BTN_GPIO;
}