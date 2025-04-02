#ifndef TWINPERIPHERAL_BUTTON_BTN_INCLUDED
#define TWINPERIPHERAL_BUTTON_BTN_INCLUDED

#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <chrono>
#include <functional>
#include <string>

namespace twinperipheral::button {
  class DFRobotButton {
  public:
    DFRobotButton(const gpio_num_t &BTN_GPIO);
    void checkForBtnPressWithDuration(const int64_t ms, std::function<void()> cb);
    static std::string TAG;
    gpio_num_t getGPIONum();

  private:
    gpio_num_t BTN_GPIO;
  };
}
#endif
