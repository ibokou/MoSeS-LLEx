#ifndef TWINPERIPHERAL_SYSTEM_CONTROLLER_INCLUDED
#define TWINPERIPHERAL_SYSTEM_CONTROLLER_INCLUDED

#include "btn/btn.hpp"
#include "driver/gpio.h"
#include "driver/rtc_io.h"
#include "error/err.hpp"
#include "esp_sleep.h"
#include "led/led.hpp"
#include "sdkconfig.h"

#define BATTERY_HIGH_LIMIT 3.6f
#define BATTERY_LOW_LIMIT 3.3f

#define BATTERY_FULL_INTERVAL                                                  \
  ((uint32_t)(30 * 60 * 1000)) / portTICK_PERIOD_MS // every 30 minutes
#define BATTERY_MEDIUM_INTERVAL                                                \
  ((uint32_t)(15 * 60 * 1000)) / portTICK_PERIOD_MS // every 15 minutes
#define BATTERY_LOW_INTERVAL                                                   \
  ((uint32_t)(5 * 60 * 1000)) / portTICK_PERIOD_MS // every 5 minutes

#define BATTERY_MEDIUM_INDICATION_DURATION                                     \
  ((uint32_t)(10 * 1000)) / portTICK_PERIOD_MS // hold for 10 seconds

#define BATTERY_LOW_INDICATION_DURATION                                        \
  ((uint32_t)(30 * 1000)) / portTICK_PERIOD_MS // hold for 30 seconds

namespace twinperipheral::system {

  class NoRTCPINException : public twinperipheral::exception::BaseException {
    using twinperipheral::exception::BaseException::BaseException;
  };

  class SystemController {
  public:
    SystemController(const int64_t idleTime, const gpio_num_t &&PWR_BTN_GPIO,
                     twinperipheral::led::RGBLED &led, BaseType_t app_cpu = 1);
    void configureDeepSleepWakeUp();
    void pollShutdownSignal(std::function<void()> cb);
    void pollHX711TareSignal(std::function<void()> cb);
    void startDeepSleep();
    void startBatteryMonotoring();

  private:
    int64_t idleTime;
    twinperipheral::button::DFRobotButton powerBtn;
    twinperipheral::led::RGBLED &led;
    float latestBatteryLevel;
    static std::string TAG;
    BaseType_t app_cpu;
    float readBatteryLevel();
  };
}
#endif