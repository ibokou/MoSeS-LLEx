#include "blinkstick.h"

std::string twincentral::led::BlinkStick::TAG = "BlinkStick";

void twincentral::led::BlinkStick::init() {
  BEGIN_TRANSITION_MAP
  TRANSITION_MAP_ENTRY(ST_INIT)       // ST_IDLE
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_INIT
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_STARTUP
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_BLE_DISCOVER_CONNECT
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_BLE_ADVERTISE
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_BLE_BAD_CTRL_INPUT
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_RUN
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_RUN_OFFSET
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_TRIGGERED_NORAXON
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_RESTART
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_FAILED_SETUP
  END_TRANSITION_MAP(NULL)
}

void twincentral::led::BlinkStick::notifyIsStarting() {
  BEGIN_TRANSITION_MAP
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_IDLE
  TRANSITION_MAP_ENTRY(ST_STARTUP)    // ST_INIT
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_STARTUP
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_BLE_DISCOVER_CONNECT
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_BLE_ADVERTISE
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_BLE_BAD_CTRL_INPUT
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_RUN
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_RUN_OFFSET
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_TRIGGERED_NORAXON
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_RESTART
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_FAILED_SETUP
  END_TRANSITION_MAP(NULL)
}

void twincentral::led::BlinkStick::notifyIsBLEDiscoveringConnecting() {
  BEGIN_TRANSITION_MAP
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)           // ST_IDLE
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)           // ST_INIT
  TRANSITION_MAP_ENTRY(ST_BLE_DISCOVER_CONNECT) // ST_STARTUP
  TRANSITION_MAP_ENTRY(EVENT_IGNORED)           // ST_BLE_DISCOVER_CONNECT
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)           // ST_BLE_ADVERTISE
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)           // ST_BLE_BAD_CTRL_INPUT
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)           // ST_RUN
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)           // ST_RUN_OFFSET
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)           // ST_TRIGGERED_NORAXON
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)           // ST_RESTART
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)           // ST_FAILED_SETUP
  END_TRANSITION_MAP(NULL)
}

void twincentral::led::BlinkStick::notifyIsBLEDAdvertising() {
  BEGIN_TRANSITION_MAP
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)    // ST_IDLE
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)    // ST_INIT
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)    // ST_STARTUP
  TRANSITION_MAP_ENTRY(ST_BLE_ADVERTISE) // ST_BLE_DISCOVER_CONNECT
  TRANSITION_MAP_ENTRY(EVENT_IGNORED)    // ST_BLE_ADVERTISE
  TRANSITION_MAP_ENTRY(ST_BLE_ADVERTISE) // ST_BLE_BAD_CTRL_INPUT
  TRANSITION_MAP_ENTRY(ST_BLE_ADVERTISE) // ST_RUN
  TRANSITION_MAP_ENTRY(ST_BLE_ADVERTISE) // ST_RUN_OFFSET
  TRANSITION_MAP_ENTRY(ST_BLE_ADVERTISE) // ST_TRIGGERED_NORAXON
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)    // ST_RESTART
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)    // ST_FAILED_SETUP
  END_TRANSITION_MAP(NULL)
}

void twincentral::led::BlinkStick::notifyHasReceivedBadControlInput() {
  BEGIN_TRANSITION_MAP
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)         // ST_IDLE
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)         // ST_INIT
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)         // ST_STARTUP
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)         // ST_BLE_DISCOVER_CONNECT
  TRANSITION_MAP_ENTRY(ST_BLE_BAD_CTRL_INPUT) // ST_BLE_ADVERTISE
  TRANSITION_MAP_ENTRY(EVENT_IGNORED)         // ST_BLE_BAD_CTRL_INPUT
  TRANSITION_MAP_ENTRY(ST_BLE_BAD_CTRL_INPUT) // ST_RUN
  TRANSITION_MAP_ENTRY(EVENT_IGNORED)         // ST_RUN_OFFSET
  TRANSITION_MAP_ENTRY(EVENT_IGNORED)         // ST_TRIGGERED_NORAXON
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)         // ST_RESTART
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)         // ST_FAILED_SETUP
  END_TRANSITION_MAP(NULL)
}

void twincentral::led::BlinkStick::notifyIsRunningCalc() {
  BEGIN_TRANSITION_MAP
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_IDLE
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_INIT
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_STARTUP
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_BLE_DISCOVER_CONNECT
  TRANSITION_MAP_ENTRY(ST_RUN)        // ST_BLE_ADVERTISE
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_BLE_BAD_CTRL_INPUT
  TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_RUN
  TRANSITION_MAP_ENTRY(ST_RUN)        // ST_RUN_OFFSET
  TRANSITION_MAP_ENTRY(ST_RUN)        // ST_TRIGGERED_NORAXON
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_RESTART
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_FAILED_SETUP
  END_TRANSITION_MAP(NULL)
}

void twincentral::led::BlinkStick::notifyIsRunningOffsetting() {
  BEGIN_TRANSITION_MAP
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_IDLE
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_INIT
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_STARTUP
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_BLE_DISCOVER_CONNECT
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_BLE_ADVERTISE
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_BLE_BAD_CTRL_INPUT
  TRANSITION_MAP_ENTRY(ST_RUN_OFFSET) // ST_RUN
  TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_RUN_OFFSET
  TRANSITION_MAP_ENTRY(ST_RUN_OFFSET) // ST_TRIGGERED_NORAXON
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_RESTART
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_FAILED_SETUP
  END_TRANSITION_MAP(NULL)
}

void twincentral::led::BlinkStick::notifyHasTriggeredNoraxon() {
  BEGIN_TRANSITION_MAP
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)        // ST_IDLE
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)        // ST_INIT
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)        // ST_STARTUP
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)        // ST_BLE_DISCOVER_CONNECT
  TRANSITION_MAP_ENTRY(ST_TRIGGERED_NORAXON) // ST_BLE_ADVERTISE
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)        // ST_BLE_BAD_CTRL_INPUT
  TRANSITION_MAP_ENTRY(ST_TRIGGERED_NORAXON) // ST_RUN
  TRANSITION_MAP_ENTRY(ST_TRIGGERED_NORAXON) // ST_RUN_OFFSET
  TRANSITION_MAP_ENTRY(EVENT_IGNORED)        // ST_TRIGGERED_NORAXON
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)        // ST_RESTART
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)        // ST_FAILED_SETUP
  END_TRANSITION_MAP(NULL)
}

void twincentral::led::BlinkStick::notifyIsRestarting() {
  BEGIN_TRANSITION_MAP
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_IDLE
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_INIT
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_STARTUP
  TRANSITION_MAP_ENTRY(ST_RESTART)    // ST_BLE_DISCOVER_CONNECT
  TRANSITION_MAP_ENTRY(ST_RESTART)    // ST_BLE_ADVERTISE
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_BLE_BAD_CTRL_INPUT
  TRANSITION_MAP_ENTRY(ST_RESTART)    // ST_RUN
  TRANSITION_MAP_ENTRY(ST_RESTART)    // ST_RUN_OFFSET
  TRANSITION_MAP_ENTRY(ST_RESTART)    // ST_TRIGGERED_NORAXON
  TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_RESTART
  TRANSITION_MAP_ENTRY(ST_RESTART)    // ST_FAILED_SETUP
  END_TRANSITION_MAP(NULL)
}

void twincentral::led::BlinkStick::notifyHasFailedSetup() {
  BEGIN_TRANSITION_MAP
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)   // ST_IDLE
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)   // ST_INIT
  TRANSITION_MAP_ENTRY(ST_FAILED_SETUP) // ST_STARTUP
  TRANSITION_MAP_ENTRY(ST_FAILED_SETUP) // ST_BLE_DISCOVER_CONNECT
  TRANSITION_MAP_ENTRY(ST_FAILED_SETUP) // ST_BLE_ADVERTISE
  TRANSITION_MAP_ENTRY(ST_FAILED_SETUP) // ST_BLE_BAD_CTRL_INPUT
  TRANSITION_MAP_ENTRY(ST_FAILED_SETUP) // ST_RUN
  TRANSITION_MAP_ENTRY(ST_FAILED_SETUP) // ST_RUN_OFFSET
  TRANSITION_MAP_ENTRY(ST_FAILED_SETUP) // ST_TRIGGERED_NORAXON
  TRANSITION_MAP_ENTRY(ST_FAILED_SETUP) // ST_RESTART
  TRANSITION_MAP_ENTRY(EVENT_IGNORED)   // ST_FAILED_SETUP
  END_TRANSITION_MAP(NULL)
}

void twincentral::led::BlinkStick::forceStateTransition(BYTE newState, const EventData *pData) {
  ExternalEvent(newState, pData);
}

void twincentral::led::BlinkStick::spawnBlinkingTask(color_t color,
                                                     uint32_t waitDuration) {
  this->suspendExecution.store(false);
  const auto currentColor = color;
  std::thread([this, currentColor, waitDuration, color]() {
    while (!(this->suspendExecution.load())) {
      this->setColor(currentColor);
      auto ms = std::chrono::steady_clock::now() +
                std::chrono::milliseconds(waitDuration);
      std::this_thread::sleep_until(ms);
      currentColor << color;
    }
  }).detach();
}

void twincentral::led::BlinkStick::stopCurrentBlinkingTasks() {
  suspendExecution.store(true);
  while (!suspendExecution.load()) {
  };
  std::this_thread::sleep_for(std::chrono::milliseconds(SLOW_BLINK));
}

void twincentral::led::BlinkStick::setColor(color_t color) {
  device.set_colours(LED_CHANNEL_NUM, color.r, color.g, color.b);
}

STATE_DEFINE(twincentral::led::BlinkStick, Idle, NoEventData) {
  printf("[%s] %s\n", TAG.c_str(), "Device is in state Idle");
}

STATE_DEFINE(twincentral::led::BlinkStick, Init, NoEventData) {
  printf("[%s] %s\n", TAG.c_str(), "Device is in state Idle");
  device = blinkstick::find();
}

STATE_DEFINE(twincentral::led::BlinkStick, StartUp, NoEventData) {
  printf("[%s] %s\n", TAG.c_str(), "Device is in state StartUp");
  spawnBlinkingTask(colorGreen, SLOW_BLINK);
}

EXIT_DEFINE(twincentral::led::BlinkStick, ExitStartUp) {
  printf("[%s] %s\n", TAG.c_str(), "Device is exiting state StartUp");
  stopCurrentBlinkingTasks();
}

STATE_DEFINE(twincentral::led::BlinkStick, BLEDiscoverConnect, NoEventData) {
  printf("[%s] %s\n", TAG.c_str(), "Device is in state BLEDiscoverConnect");
  setColor(colorBlue);
}

EXIT_DEFINE(twincentral::led::BlinkStick, ExitBLEDiscoverConnect) {
  printf("[%s] %s\n", TAG.c_str(),
         "Device is exiting state BLEDiscoverConnect");
  stopCurrentBlinkingTasks();
}

STATE_DEFINE(twincentral::led::BlinkStick, BLEAdvertise, NoEventData) {
  printf("[%s] %s\n", TAG.c_str(), "Device is in state BLEAdvertise");
  spawnBlinkingTask(colorBlue, SLOW_BLINK);
}

EXIT_DEFINE(twincentral::led::BlinkStick, ExitBLEAdvertise) {
  printf("[%s] %s\n", TAG.c_str(), "Device is exiting state BLEAdvertise");
  stopCurrentBlinkingTasks();
}

STATE_DEFINE(twincentral::led::BlinkStick, BLEBadControlInput, NoEventData) {
  printf("[%s] %s\n", TAG.c_str(), "Device is in state BLEBadControlInput");
  spawnBlinkingTask(colorRed, SLOW_BLINK);
}

EXIT_DEFINE(twincentral::led::BlinkStick, ExitBLEBadControlInput) {
  printf("[%s] %s\n", TAG.c_str(),
         "Device is exiting state BLEBadControlInput");
  stopCurrentBlinkingTasks();
}

STATE_DEFINE(twincentral::led::BlinkStick, Run, NoEventData) {
  printf("[%s] %s\n", TAG.c_str(), "Device is in state Run");
  spawnBlinkingTask(colorBlue, FAST_BLINK);
}

EXIT_DEFINE(twincentral::led::BlinkStick, ExitRun) {
  printf("[%s] %s\n", TAG.c_str(), "Device is exiting state Run");
  stopCurrentBlinkingTasks();
}

STATE_DEFINE(twincentral::led::BlinkStick, RunOffset, NoEventData) {
  printf("[%s] %s\n", TAG.c_str(), "Device is in state RunOffset");
  spawnBlinkingTask(colorYellow, FAST_BLINK);
}

EXIT_DEFINE(twincentral::led::BlinkStick, ExitRunOffset) {
  printf("[%s] %s\n", TAG.c_str(), "Device is exiting state RunOffset");
  stopCurrentBlinkingTasks();
}

STATE_DEFINE(twincentral::led::BlinkStick, TriggeredNoraxon, NoEventData) {
  printf("[%s] %s\n", TAG.c_str(), "Device is in state TriggeredNoraxon");
  setColor(colorGreen);
}

EXIT_DEFINE(twincentral::led::BlinkStick, ExitTriggeredNoraxon) {
  printf("[%s] %s\n", TAG.c_str(), "Device is exiting state TriggeredNoraxon");
  stopCurrentBlinkingTasks();
}

STATE_DEFINE(twincentral::led::BlinkStick, Restart, NoEventData) {
  printf("[%s] %s\n", TAG.c_str(), "Device is in state Restart");
  spawnBlinkingTask(colorGreen, FAST_BLINK);
}

EXIT_DEFINE(twincentral::led::BlinkStick, ExitRestart) {
  printf("[%s] %s\n", TAG.c_str(), "Device is exiting state Restart");
  stopCurrentBlinkingTasks();
}

STATE_DEFINE(twincentral::led::BlinkStick, FailedSetup, NoEventData) {
  printf("[%s] %s\n", TAG.c_str(), "Device is in state FailedSetup");
  setColor(colorRed);
}