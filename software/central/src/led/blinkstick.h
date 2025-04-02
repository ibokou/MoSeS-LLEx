#ifndef TWINCENTRAL_LED_BLINKSTICK_INCLUDED
#define TWINCENTRAL_LED_BLINKSTICK_INCLUDED

#define SLOW_BLINK 500
#define FAST_BLINK 100
#define LED_CHANNEL_NUM 0

#include "StateMachine_.h"
#include "blinkstick/blinkstick.h"
#include <atomic>
#include <thread>

namespace twincentral::led {

  struct color_t {
    mutable uint8_t r;
    mutable uint8_t g;
    mutable uint8_t b;

    auto operator<<(const color_t &other) const {
      auto shift = [](uint8_t &val1, uint8_t &val2) -> uint8_t {
        uint16_t _val1 = val1;
        uint16_t _val2 = val2;
        
        if (_val1 + _val2 == _val2) {
          return val2;
        }

        return 0;
      };
      r = shift(r, other.r);
      g = shift(g, other.g);
      b = shift(b, other.b);
    }

    auto operator=(const color_t &other) const {
      r = other.r;
      g = other.g;
      b = other.b;
      return *this;
    }
  };

  class BlinkStick : public StateMachine {
  public:
    BlinkStick()
        : StateMachine(ST_MAX_STATES),
          device(blinkstick::device{nullptr, blinkstick::device_type::unknown}),
          suspendExecution(false) {}

    void init();
    void shutdown();
    void notifyIsStarting();
    void notifyIsBLEDiscoveringConnecting();
    void notifyIsBLEDAdvertising();
    void notifyHasReceivedBadControlInput();
    void notifyIsRunningCalc();
    void notifyIsRunningOffsetting();
    void notifyHasTriggeredNoraxon();
    void notifyIsRestarting();
    void notifyHasFailedSetup();
    void forceStateTransition(BYTE newState, const EventData *pData);
    static std::string TAG;

  private:
    enum States {
      ST_IDLE,
      ST_INIT,
      ST_STARTUP,
      ST_BLE_DISCOVER_CONNECT,
      ST_BLE_ADVERTISE,
      ST_BLE_BAD_CTRL_INPUT,
      ST_RUN,
      ST_RUN_OFFSET,
      ST_TRIGGERED_NORAXON,
      ST_RESTART,
      ST_FAILED_SETUP,
      ST_MAX_STATES
    };

    STATE_DECLARE(BlinkStick, Idle, NoEventData)
    STATE_DECLARE(BlinkStick, Init, NoEventData)

    STATE_DECLARE(BlinkStick, StartUp, NoEventData)
    EXIT_DECLARE(BlinkStick, ExitStartUp)

    STATE_DECLARE(BlinkStick, BLEDiscoverConnect, NoEventData)
    EXIT_DECLARE(BlinkStick, ExitBLEDiscoverConnect)

    STATE_DECLARE(BlinkStick, BLEAdvertise, NoEventData)
    EXIT_DECLARE(BlinkStick, ExitBLEAdvertise)

    STATE_DECLARE(BlinkStick, BLEBadControlInput, NoEventData)
    EXIT_DECLARE(BlinkStick, ExitBLEBadControlInput)

    STATE_DECLARE(BlinkStick, Run, NoEventData)
    EXIT_DECLARE(BlinkStick, ExitRun)

    STATE_DECLARE(BlinkStick, RunOffset, NoEventData)
    EXIT_DECLARE(BlinkStick, ExitRunOffset)

    STATE_DECLARE(BlinkStick, TriggeredNoraxon, NoEventData)
    EXIT_DECLARE(BlinkStick, ExitTriggeredNoraxon)

    STATE_DECLARE(BlinkStick, Restart, NoEventData)
    EXIT_DECLARE(BlinkStick, ExitRestart)

    STATE_DECLARE(BlinkStick, FailedSetup, NoEventData)

    BEGIN_STATE_MAP_EX
    STATE_MAP_ENTRY_EX(&Idle)
    STATE_MAP_ENTRY_EX(&Init)
    STATE_MAP_ENTRY_ALL_EX(&StartUp, 0, 0, &ExitStartUp)
    STATE_MAP_ENTRY_ALL_EX(&BLEDiscoverConnect, 0, 0, &ExitBLEDiscoverConnect)
    STATE_MAP_ENTRY_ALL_EX(&BLEAdvertise, 0, 0, &ExitBLEAdvertise)
    STATE_MAP_ENTRY_ALL_EX(&BLEBadControlInput, 0, 0, &ExitBLEBadControlInput)
    STATE_MAP_ENTRY_ALL_EX(&Run, 0, 0, &ExitRun)
    STATE_MAP_ENTRY_ALL_EX(&RunOffset, 0, 0, &ExitRunOffset)
    STATE_MAP_ENTRY_ALL_EX(&TriggeredNoraxon, 0, 0, &ExitTriggeredNoraxon)
    STATE_MAP_ENTRY_ALL_EX(&Restart, 0, 0, &ExitRestart)
    STATE_MAP_ENTRY_EX(&FailedSetup)
    END_STATE_MAP_EX

    const color_t colorRed{.r = 255, .g = 0, .b = 0};
    const color_t colorGreen{.r = 0, .g = 255, .b = 0};
    const color_t colorBlue{.r = 0, .g = 0, .b = 255};
    const color_t colorYellow{.r = 255, .g = 255, .b = 0};
    void setColor(color_t color);
    void spawnBlinkingTask(color_t color, uint32_t waitDuration);
    void stopCurrentBlinkingTasks();
    blinkstick::device device;
    std::atomic_bool suspendExecution;
  };
}

#endif