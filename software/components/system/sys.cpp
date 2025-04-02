#include "system/sys.hpp"

twinperipheral::system::SystemController::SystemController(
    const int64_t idleTime, const gpio_num_t &&PWR_BTN_GPIO,
    twinperipheral::led::RGBLED &led, BaseType_t app_cpu)
    : idleTime(idleTime), powerBtn(PWR_BTN_GPIO), led(led),
      latestBatteryLevel(4.2f), app_cpu(app_cpu) {
  if (!rtc_gpio_is_valid_gpio(PWR_BTN_GPIO)) {
    throw NoRTCPINException("Power Button GPIO PIN is not a RTC pin");
  }
  ESP_ERROR_CHECK(rtc_gpio_deinit(PWR_BTN_GPIO));
};

std::string twinperipheral::system::SystemController::TAG = "SystemController";

void twinperipheral::system::SystemController::configureDeepSleepWakeUp() {
  esp_sleep_enable_ext0_wakeup(powerBtn.getGPIONum(), 1);

  ESP_LOGI(TAG.c_str(), "Enabling GPIO wakeup on pins GPIO%d\n",
           powerBtn.getGPIONum());
}

void twinperipheral::system::SystemController::pollShutdownSignal(
    std::function<void()> cb) {
  powerBtn.checkForBtnPressWithDuration(3000, cb);
}

void twinperipheral::system::SystemController::pollHX711TareSignal(
    std::function<void()> cb) {
  powerBtn.checkForBtnPressWithDuration(500, cb);
}

void twinperipheral::system::SystemController::startDeepSleep() {
  esp_deep_sleep_start();
}

void twinperipheral::system::SystemController::startBatteryMonotoring() {
  static auto pollBatteryLevelTask = [](void *_this) {
    auto ctx = static_cast<twinperipheral::system::SystemController *>(_this);
    while (1) {
      const auto lvl = ctx->readBatteryLevel();
      ESP_LOGI(TAG.c_str(), "Read Battery Level");
      static twinperipheral::led::BatteryIndicationData data;
      if (lvl < BATTERY_HIGH_LIMIT and lvl > BATTERY_LOW_LIMIT) {
        data.hold = BATTERY_MEDIUM_INDICATION_DURATION;
        data.color = ctx->led.colorYellow;
        ctx->led.notifyBatteryLevel(&data);
        vTaskDelay(BATTERY_MEDIUM_INTERVAL);
        continue;
      }

      if (lvl < BATTERY_LOW_LIMIT) {
        data.hold = BATTERY_LOW_INDICATION_DURATION;
        data.color = ctx->led.colorRed;
        ctx->led.notifyBatteryLevel(&data);
        vTaskDelay(BATTERY_LOW_INTERVAL);
        continue;
      };

      vTaskDelay(BATTERY_FULL_INTERVAL);
    }
  };

  xTaskCreatePinnedToCore(pollBatteryLevelTask, "pollBatteryLevelTask", 4096,
                          this, 4, NULL, app_cpu);
}

float twinperipheral::system::SystemController::readBatteryLevel() {
  return 4.2f;
}