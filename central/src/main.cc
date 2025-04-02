#include "statemachine/central.h"
#include <thread>

int main() {
  auto rules =
      twincentral::gait_detection::fuzzy_logic::inference::createRules();
  auto system =
      twincentral::gait_detection::fuzzy_logic::inference::createSystem();

  auto gaitEstimator = twincentral::plan::GaitEstimator(rules, system);
  twincentral::led::BlinkStick led;

  auto connection = sdbus::createSystemBusConnection();

  twincentral::statemachine::TwinCentralStateMachine machine(
      led, gaitEstimator, std::move(connection));
  machine.run();

  while (true) {
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
  }
}