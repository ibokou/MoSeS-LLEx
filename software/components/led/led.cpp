#include "led/led.hpp"

twinperipheral::led::RGBLED::RGBLED(SemaphoreHandle_t stateMutex, int ledRedPin,
                                    int ledGreenPin, int ledBluePin,
                                    BaseType_t app_cpu)
    : StateMachine(stateMutex, ST_MAX_STATES), LED_RED_PIN(ledRedPin),
      LED_GREEN_PIN(ledGreenPin), LED_BLUE_PIN(ledBluePin),
      lowFreqBlinkSimTaskHandle(NULL) {
  SLOW_BLINK_EVENT_QUEUE = xQueueCreate(LED_SLOW_BLINK_EVENT_QUEUE_LEN,
                                        LED_SLOW_BLINK_EVENT_QUEUE_MSG_SIZE);
  ledc_timer_config_t ledc_timer_conf = {
      .speed_mode = LEDC_LOW_SPEED_MODE,
      .duty_resolution = LEDC_TIMER_13_BIT,
      .timer_num = ledc_timer,
      .freq_hz = LED_STEADY_FREQ, // Set output frequency at 1 kHz
      .clk_cfg = LEDC_AUTO_CLK};
  ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer_conf));

  static auto lowFreqBlinkSimTask = [](void *_this) {
    auto ctx = static_cast<twinperipheral::led::RGBLED *>(_this);
    static color_t color = ctx->colorBlank;
    while (1) {
      xQueueReceive(ctx->SLOW_BLINK_EVENT_QUEUE, &color, 0);
      ctx->simulateLowFreqBlink(color);
    }
  };
  xTaskCreatePinnedToCore(lowFreqBlinkSimTask, "lowFreqBlinkSimTasK", 1024,
                          (void *)this, 3, &lowFreqBlinkSimTaskHandle, app_cpu);
  vTaskSuspend(lowFreqBlinkSimTaskHandle);
}

std::string twinperipheral::led::RGBLED::TAG = "RGBLED";

void twinperipheral::led::RGBLED::init() {
  BEGIN_TRANSITION_MAP
  TRANSITION_MAP_ENTRY(ST_INIT)       // ST_IDLE
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_INIT
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_STARTUP
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_IMU_CALIBRATION
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_OPERATIONAL
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_BLE_READY
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_BLE_CONNECTION
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_SEND_DATA
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_FAILED_WIRING
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_STOP
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_BATTERY_INDICATION
  END_TRANSITION_MAP(NULL)
}

void twinperipheral::led::RGBLED::stop() {
  BEGIN_TRANSITION_MAP
  TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_IDLE
  TRANSITION_MAP_ENTRY(ST_STOP)       // ST_INIT
  TRANSITION_MAP_ENTRY(ST_STOP)       // ST_STARTUP
  TRANSITION_MAP_ENTRY(ST_STOP)       // ST_IMU_CALIBRATION
  TRANSITION_MAP_ENTRY(ST_STOP)       // ST_OPERATIONAL
  TRANSITION_MAP_ENTRY(ST_STOP)       // ST_BLE_READY
  TRANSITION_MAP_ENTRY(ST_STOP)       // ST_BLE_CONNECTION
  TRANSITION_MAP_ENTRY(ST_STOP)       // ST_SEND_DATA
  TRANSITION_MAP_ENTRY(ST_STOP)       // ST_FAILED_WIRING
  TRANSITION_MAP_ENTRY(ST_STOP)       // ST_STOP
  TRANSITION_MAP_ENTRY(ST_STOP)       // ST_BATTERY_INDICATION
  END_TRANSITION_MAP(NULL)
}

void twinperipheral::led::RGBLED::notifyIsStarting() {
  BEGIN_TRANSITION_MAP
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_IDLE
  TRANSITION_MAP_ENTRY(ST_STARTUP)    // ST_INIT
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_STARTUP
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_IMU_CALIBRATION
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_OPERATIONAL
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_BLE_READY
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_BLE_CONNECTION
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_SEND_DATA
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_FAILED_WIRING
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_STOP
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_BATTERY_INDICATION
  END_TRANSITION_MAP(NULL)
}

void twinperipheral::led::RGBLED::notifyIsCalibratingIMU() {
  BEGIN_TRANSITION_MAP
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)      // ST_IDLE
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)      // ST_INIT
  TRANSITION_MAP_ENTRY(ST_IMU_CALIBRATION) // ST_STARTUP
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)      // ST_IMU_CALIBRATION
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)      // ST_OPERATIONAL
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)      // ST_BLE_READY
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)      // ST_BLE_CONNECTION
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)      // ST_SEND_DATA
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)      // ST_FAILED_WIRING
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)      // ST_STOP
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)      // ST_BATTERY_INDICATION
  END_TRANSITION_MAP(NULL)
}

void twinperipheral::led::RGBLED::notifyIsOperational() {
  BEGIN_TRANSITION_MAP
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)  // ST_IDLE
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)  // ST_INIT
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)  // ST_STARTUP
  TRANSITION_MAP_ENTRY(ST_OPERATIONAL) // ST_IMU_CALIBRATION
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)  // ST_OPERATIONAL
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)  // ST_BLE_READY
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)  // ST_BLE_CONNECTION
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)  // ST_SEND_DATA
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)  // ST_FAILED_WIRING
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)  // ST_STOP
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)  // ST_BATTERY_INDICATION
  END_TRANSITION_MAP(NULL)
}

void twinperipheral::led::RGBLED::notifyIsBLEReady() {
  BEGIN_TRANSITION_MAP
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_IDLE
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_INIT
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_STARTUP
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_IMU_CALIBRATION
  TRANSITION_MAP_ENTRY(ST_BLE_READY)  // ST_OPERATIONAL
  TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_BLE_READY
  TRANSITION_MAP_ENTRY(ST_BLE_READY)  // ST_BLE_CONNECTION
  TRANSITION_MAP_ENTRY(ST_BLE_READY)  // ST_SEND_DATA
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_FAILED_WIRING
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_STOP
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_BATTERY_INDICATION
  END_TRANSITION_MAP(NULL)
}

void twinperipheral::led::RGBLED::notifyIsBLEConnected() {
  BEGIN_TRANSITION_MAP
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)     // ST_IDLE
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)     // ST_INIT
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)     // ST_STARTUP
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)     // ST_IMU_CALIBRATION
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)     // ST_OPERATIONAL
  TRANSITION_MAP_ENTRY(ST_BLE_CONNECTION) // ST_BLE_READY
  TRANSITION_MAP_ENTRY(EVENT_IGNORED)     // ST_BLE_CONNECTION
  TRANSITION_MAP_ENTRY(EVENT_IGNORED)     // ST_SEND_DATA
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)     // ST_FAILED_WIRING
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)     // ST_STOP
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)     // ST_BATTERY_INDICATION
  END_TRANSITION_MAP(NULL)
}

void twinperipheral::led::RGBLED::notifyIsSendingData() {
  BEGIN_TRANSITION_MAP
  TRANSITION_MAP_ENTRY(ST_INIT)       // ST_IDLE
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_INIT
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_STARTUP
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_IMU_CALIBRATION
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_OPERATIONAL
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_BLE_READY
  TRANSITION_MAP_ENTRY(ST_SEND_DATA)  // ST_BLE_CONNECTION
  TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SEND_DATA
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_FAILED_WIRING
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_STOP
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_BATTERY_INDICATION
  END_TRANSITION_MAP(NULL)
}

void twinperipheral::led::RGBLED::notifyhasFailedWiring() {
  BEGIN_TRANSITION_MAP
  TRANSITION_MAP_ENTRY(ST_INIT)          // ST_IDLE
  TRANSITION_MAP_ENTRY(ST_FAILED_WIRING) // ST_INIT
  TRANSITION_MAP_ENTRY(EVENT_IGNORED)    // ST_STARTUP
  TRANSITION_MAP_ENTRY(ST_FAILED_WIRING) // ST_IMU_CALIBRATION
  TRANSITION_MAP_ENTRY(EVENT_IGNORED)    // ST_OPERATIONAL
  TRANSITION_MAP_ENTRY(ST_FAILED_WIRING) // ST_BLE_READY
  TRANSITION_MAP_ENTRY(EVENT_IGNORED)    // ST_BLE_CONNECTION
  TRANSITION_MAP_ENTRY(EVENT_IGNORED)    // ST_SEND_DATA
  TRANSITION_MAP_ENTRY(EVENT_IGNORED)    // ST_FAILED_WIRING
  TRANSITION_MAP_ENTRY(EVENT_IGNORED)    // ST_STOP
  TRANSITION_MAP_ENTRY(EVENT_IGNORED)    // ST_BATTERY_INDICATION
  END_TRANSITION_MAP(NULL)
}

void twinperipheral::led::RGBLED::notifyBatteryLevel(
    twinperipheral::led::BatteryIndicationData *data) {
  BEGIN_TRANSITION_MAP
  TRANSITION_MAP_ENTRY(EVENT_IGNORED)         // ST_IDLE
  TRANSITION_MAP_ENTRY(EVENT_IGNORED)         // ST_INIT
  TRANSITION_MAP_ENTRY(EVENT_IGNORED)         // ST_STARTUP
  TRANSITION_MAP_ENTRY(ST_BATTERY_INDICATION) // ST_IMU_CALIBRATION
  TRANSITION_MAP_ENTRY(ST_BATTERY_INDICATION) // ST_OPERATIONAL
  TRANSITION_MAP_ENTRY(ST_BATTERY_INDICATION) // ST_BLE_READY
  TRANSITION_MAP_ENTRY(ST_BATTERY_INDICATION) // ST_BLE_CONNECTION
  TRANSITION_MAP_ENTRY(ST_BATTERY_INDICATION) // ST_SEND_DATA
  TRANSITION_MAP_ENTRY(ST_BATTERY_INDICATION) // ST_FAILED_WIRING
  TRANSITION_MAP_ENTRY(EVENT_IGNORED)         // ST_STOP
  TRANSITION_MAP_ENTRY(EVENT_IGNORED)         // ST_BATTERY_INDICATION
  END_TRANSITION_MAP(data)
}

STATE_DEFINE(twinperipheral::led::RGBLED, Idle, NoEventData) {
  ESP_LOGI(TAG.c_str(), "LED is in state Idle");
}

STATE_DEFINE(twinperipheral::led::RGBLED, Init, NoEventData) {
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

ENTRY_DEFINE(twinperipheral::led::RGBLED, EntryStartUp, NoEventData) {
  vTaskResume(lowFreqBlinkSimTaskHandle);
}

STATE_DEFINE(twinperipheral::led::RGBLED, StartUp, NoEventData) {
  ESP_LOGI(TAG.c_str(), "LED is in state StartUp");
  sendMessageToQueue((void *)&colorGreen, SLOW_BLINK_EVENT_QUEUE, 10);
}

EXIT_DEFINE(twinperipheral::led::RGBLED, ExitStartUp) {
  vTaskSuspend(lowFreqBlinkSimTaskHandle);
  purgeBlinkSimQueue();
}

STATE_DEFINE(twinperipheral::led::RGBLED, IMUCalibration, NoEventData) {
  ESP_LOGI(TAG.c_str(), "LED is in state IMUCalibration");
  setColor(colorGreenBlink);
  setTimerFrequency(LED_BLINK_FAST_FREQ);
}

STATE_DEFINE(twinperipheral::led::RGBLED, Operational, NoEventData) {
  ESP_LOGI(TAG.c_str(), "LED is in state Operational");
  setColor(colorGreen);
  setTimerFrequency(LED_STEADY_FREQ);
}

STATE_DEFINE(twinperipheral::led::RGBLED, BLEIdle, NoEventData) {
  ESP_LOGI(TAG.c_str(), "LED is in state BLEIdle");
  setColor(colorBlue);
  setTimerFrequency(LED_STEADY_FREQ);
}

ENTRY_DEFINE(twinperipheral::led::RGBLED, EntryBLEConnection, NoEventData) {
  vTaskResume(lowFreqBlinkSimTaskHandle);
}

STATE_DEFINE(twinperipheral::led::RGBLED, BLEConnection, NoEventData) {
  ESP_LOGI(TAG.c_str(), "LED is in state BLEConnection");
  sendMessageToQueue((void *)&colorBlue, SLOW_BLINK_EVENT_QUEUE, 10);
}

EXIT_DEFINE(twinperipheral::led::RGBLED, ExitBLEConnection) {
  vTaskSuspend(lowFreqBlinkSimTaskHandle);
  purgeBlinkSimQueue();
}

STATE_DEFINE(twinperipheral::led::RGBLED, SendData, NoEventData) {
  ESP_LOGI(TAG.c_str(), "LED is in state SendData");
  setColor(colorBlueBlink);
  setTimerFrequency(LED_BLINK_FAST_FREQ);
}

STATE_DEFINE(twinperipheral::led::RGBLED, FailedWiring, NoEventData) {
  ESP_LOGI(TAG.c_str(), "LED is in state FailedWiring");
  setColor(colorRed);
  setTimerFrequency(LED_STEADY_FREQ);
}

STATE_DEFINE(twinperipheral::led::RGBLED, Stop, NoEventData) {
  ESP_LOGI(TAG.c_str(), "LED is in state Stop");
  ESP_ERROR_CHECK(ledc_stop(LEDC_LOW_SPEED_MODE, ledc_channels[0].channel, 0));
  ESP_ERROR_CHECK(ledc_stop(LEDC_LOW_SPEED_MODE, ledc_channels[1].channel, 0));
  ESP_ERROR_CHECK(ledc_stop(LEDC_LOW_SPEED_MODE, ledc_channels[2].channel, 0));
  ESP_ERROR_CHECK(ledc_timer_pause(LEDC_LOW_SPEED_MODE, ledc_timer));
}

ENTRY_DEFINE(twinperipheral::led::RGBLED, EntryBatteryIndication,
             twinperipheral::led::BatteryIndicationData) {
  vTaskResume(lowFreqBlinkSimTaskHandle);
  batteryIndicationEntryState = GetCurrentState();
}

STATE_DEFINE(twinperipheral::led::RGBLED, BatteryIndication,
             twinperipheral::led::BatteryIndicationData) {
  ESP_LOGI(TAG.c_str(), "LED is in state BatteryIndication");
  const auto hold = data->hold;
  const auto color = data->color;
  sendMessageToQueue((void *)&color, SLOW_BLINK_EVENT_QUEUE, 10);
  const auto previousState = batteryIndicationEntryState;
  vTaskDelay(hold);
  InternalEvent(previousState);
}

EXIT_DEFINE(twinperipheral::led::RGBLED, ExitBatteryIndication) {
  batteryIndicationEntryState = 0;
  vTaskSuspend(lowFreqBlinkSimTaskHandle);
  purgeBlinkSimQueue();
}

void twinperipheral::led::RGBLED::simulateLowFreqBlink(color_t color) {
  setColor(color);
  vTaskDelay((1000 / LED_BLINK_SLOW_FREQ) / portTICK_PERIOD_MS);
  setColor(colorBlank);
  vTaskDelay((1000 / LED_BLINK_SLOW_FREQ) / portTICK_PERIOD_MS);
}

void twinperipheral::led::RGBLED::setColor(color_t color) {
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

void twinperipheral::led::RGBLED::setTimerFrequency(uint32_t freq_hz) {
  ledc_set_freq(LEDC_LOW_SPEED_MODE, ledc_timer, freq_hz);
}

void twinperipheral::led::RGBLED::sendMessageToQueue(void *message,
                                                     QueueHandle_t &queue,

                                                     std::size_t &&maxAttempt) {
  std::size_t attempt = 0;
  while (attempt < maxAttempt) {
    if (xQueueSend(queue, message, 10) != pdTRUE) {
      ESP_LOGI(TAG.c_str(), "SLOW_BLINK_EVENT_QUEUE is full");
      vTaskDelay(50);
      ++attempt;
    } else {
      return;
    }
  }
}

void twinperipheral::led::RGBLED::purgeBlinkSimQueue() {
  xQueueReset(SLOW_BLINK_EVENT_QUEUE);
}
