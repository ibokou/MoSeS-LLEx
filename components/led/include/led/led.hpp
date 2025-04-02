#ifndef TWINPERIPHERAL_LED_RGB_INCLUDED
#define TWINPERIPHERAL_LED_RGB_INCLUDED

#include "StateMachine.h"
#include "driver/ledc.h"
#include "esp_err.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <array>
#include <string>

#define LED_STEADY_FREQ (uint32_t)5000
#define LED_BLINK_FAST_FREQ (uint32_t)10
#define LED_BLINK_SLOW_FREQ (uint32_t)2

#define LED_SLOW_BLINK_EVENT_QUEUE_LEN 10
#define LED_SLOW_BLINK_EVENT_QUEUE_MSG_SIZE sizeof(color_t *)

namespace twinperipheral::led {
  typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
  } color_t;

  struct BatteryIndicationData : public EventData {
    TickType_t hold;
    color_t color;
  };

  class RGBLED : public StateMachine {
  public:
    RGBLED(SemaphoreHandle_t stateMutex, int ledRedPin, int ledGreenPin,
           int ledBluePin, BaseType_t app_cpu = 1);
    void init();
    void stop();
    void notifyIsStarting();
    void notifyIsOperational();
    void notifyIsCalibratingIMU();
    void notifyIsBLEReady();
    void notifyIsBLEConnected();
    void notifyIsSendingData();
    void notifyhasFailedWiring();
    void notifyBatteryLevel(BatteryIndicationData *data);
    void simulateLowFreqBlink(color_t color);

    const color_t colorBlank{.r = 0, .g = 0, .b = 0};        // Duty cyle 100%
    const color_t colorRed{.r = 255, .g = 0, .b = 0};        // Duty cyle 100%
    const color_t colorRedBlink{.r = 153, .g = 0, .b = 0};   // Duty cycle 60%
    const color_t colorGreen{.r = 0, .g = 255, .b = 0};      // Duty cyle 100%
    const color_t colorGreenBlink{.r = 0, .g = 153, .b = 0}; // Duty cyle 60%
    const color_t colorBlue{.r = 0, .g = 0, .b = 255};       // Duty cycle 100%
    const color_t colorBlueBlink{.r = 0, .g = 0, .b = 153};  // Duty cycle 60%
    const color_t colorYellow{.r = 255, .g = 255, .b = 0};   // Duty cycle 100%

    static std::string TAG;

  private:
    enum States {
      ST_IDLE,
      ST_INIT,
      ST_STARTUP,
      ST_IMU_CALIBRATION,
      ST_OPERATIONAL,
      ST_BLE_READY,
      ST_BLE_CONNECTION,
      ST_SEND_DATA,
      ST_FAILED_WIRING,
      ST_STOP,
      ST_BATTERY_INDICATION,
      ST_MAX_STATES
    };

    // Define the state machine state functions with event data type
    STATE_DECLARE(RGBLED, Idle, NoEventData)
    STATE_DECLARE(RGBLED, Init, NoEventData)

    ENTRY_DECLARE(RGBLED, EntryStartUp, NoEventData)
    STATE_DECLARE(RGBLED, StartUp, NoEventData)
    EXIT_DECLARE(RGBLED, ExitStartUp)

    STATE_DECLARE(RGBLED, IMUCalibration, NoEventData)
    STATE_DECLARE(RGBLED, Operational, NoEventData)
    STATE_DECLARE(RGBLED, BLEIdle, NoEventData)

    ENTRY_DECLARE(RGBLED, EntryBLEConnection, NoEventData)
    STATE_DECLARE(RGBLED, BLEConnection, NoEventData)
    EXIT_DECLARE(RGBLED, ExitBLEConnection)

    STATE_DECLARE(RGBLED, SendData, NoEventData)
    STATE_DECLARE(RGBLED, FailedWiring, NoEventData)
    STATE_DECLARE(RGBLED, Stop, NoEventData)

    ENTRY_DECLARE(RGBLED, EntryBatteryIndication, BatteryIndicationData)
    STATE_DECLARE(RGBLED, BatteryIndication, BatteryIndicationData)
    EXIT_DECLARE(RGBLED, ExitBatteryIndication)

    BEGIN_STATE_MAP_EX
    STATE_MAP_ENTRY_EX(&Idle)
    STATE_MAP_ENTRY_EX(&Init)
    STATE_MAP_ENTRY_ALL_EX(&StartUp, 0, &EntryStartUp, &ExitStartUp)
    STATE_MAP_ENTRY_EX(&IMUCalibration)
    STATE_MAP_ENTRY_EX(&Operational)
    STATE_MAP_ENTRY_EX(&BLEIdle)
    STATE_MAP_ENTRY_ALL_EX(&BLEConnection, 0, &EntryBLEConnection,
                           &ExitBLEConnection)
    STATE_MAP_ENTRY_EX(&SendData)
    STATE_MAP_ENTRY_EX(&FailedWiring)
    STATE_MAP_ENTRY_EX(&Stop)
    STATE_MAP_ENTRY_ALL_EX(&BatteryIndication, 0, &EntryBatteryIndication,
                           &ExitBatteryIndication)
    END_STATE_MAP_EX

    void setColor(color_t color);
    void setTimerFrequency(uint32_t freq);
    void sendMessageToQueue(void *message, QueueHandle_t &queue,
                            std::size_t &&maxAttempt);
    void purgeBlinkSimQueue();
    int LED_RED_PIN;
    int LED_GREEN_PIN;
    int LED_BLUE_PIN;
    ledc_timer_t ledc_timer = LEDC_TIMER_0;
    std::array<ledc_channel_config_t, 3> ledc_channels{};
    TaskHandle_t lowFreqBlinkSimTaskHandle;
    QueueHandle_t SLOW_BLINK_EVENT_QUEUE;
    BYTE batteryIndicationEntryState;
  };
}

#endif