#include "central.h"
#include "../act/experiment.h"
#include "../os/os.h"
#include "regex"
#include <sdbus-c++/sdbus-c++.h>
#include <simplebluez/Bluez.h>

std::string twincentral::statemachine::TwinCentralStateMachine::TAG =
    "TwinCentralStateMachine";

void twincentral::statemachine::TwinCentralStateMachine::run() {
  BEGIN_TRANSITION_MAP
  TRANSITION_MAP_ENTRY(ST_STARTUP)    // ST_IDLE
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_STARTUP
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_BLE_DISCOVER_CONNECT
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_BLE_ADVERTISE
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_BLE_CTRL_CONN
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_BLE_CTRL_DISCONN
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_BLE_BAD_CTRL_INPUT
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_RUN_EXPERIMENT
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_RUN_AUTO
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_STOP_RUN
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_RESTART
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_SHUTDOWN
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_FAILED_SETUP
  END_TRANSITION_MAP(NULL)
}

void twincentral::statemachine::TwinCentralStateMachine::
    notifyBLEControllerAppHasConnected() {
  BEGIN_TRANSITION_MAP
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)    // ST_IDLE
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)    // ST_STARTUP
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)    // ST_BLE_DISCOVER_CONNECT
  TRANSITION_MAP_ENTRY(ST_BLE_CTRL_CONN) // ST_BLE_ADVERTISE
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)    // ST_BLE_CTRL_CONN
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)    // ST_BLE_CTRL_DISCONN
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)    // ST_BLE_BAD_CTRL_INPUT
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)    // ST_RUN_EXPERIMENT
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)    // ST_RUN_AUTO
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)    // ST_STOP_RUN
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)    // ST_RESTART
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)    // ST_SHUTDOWN
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)    // ST_FAILED_SETUP
  END_TRANSITION_MAP(NULL)
};

void twincentral::statemachine::TwinCentralStateMachine::
    notifyBLEControllerAppHasDisconnected() {
  BEGIN_TRANSITION_MAP
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)       // ST_IDLE
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)       // ST_STARTUP
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)       // ST_BLE_DISCOVER_CONNECT
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)       // ST_BLE_ADVERTISE
  TRANSITION_MAP_ENTRY(ST_BLE_CTRL_DISCONN) // ST_BLE_CTRL_CONN
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)       // ST_BLE_CTRL_DISCONN
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)       // ST_BLE_BAD_CTRL_INPUT
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)       // ST_RUN_EXPERIMENT
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)       // ST_RUN_AUTO
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)       // ST_STOP_RUN
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)       // ST_RESTART
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)       // ST_SHUTDOWN
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)       // ST_FAILED_SETUP
  END_TRANSITION_MAP(NULL)
}

void twincentral::statemachine::TwinCentralStateMachine::startExperiment(
    ExperimentData *data) {
  BEGIN_TRANSITION_MAP
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)     // ST_IDLE
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)     // ST_STARTUP
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)     // ST_BLE_DISCOVER_CONNECT
  TRANSITION_MAP_ENTRY(ST_RUN_EXPERIMENT) // ST_BLE_ADVERTISE
  TRANSITION_MAP_ENTRY(ST_RUN_EXPERIMENT) // ST_BLE_CTRL_CONN
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)     // ST_BLE_CTRL_DISCONN
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)     // ST_BLE_BAD_CTRL_INPUT
  TRANSITION_MAP_ENTRY(EVENT_IGNORED)     // ST_RUN_EXPERIMENT
  TRANSITION_MAP_ENTRY(EVENT_IGNORED)     // ST_RUN_AUTO
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)     // ST_STOP_RUN
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)     // ST_RESTART
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)     // ST_SHUTDOWN
  TRANSITION_MAP_ENTRY(EVENT_IGNORED)     // ST_FAILED_SETUP
  END_TRANSITION_MAP(data)
}

void twincentral::statemachine::TwinCentralStateMachine::startAutoMode() {
  BEGIN_TRANSITION_MAP
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_IDLE
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_STARTUP
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_BLE_DISCOVER_CONNECT
  TRANSITION_MAP_ENTRY(ST_RUN_AUTO)   // ST_BLE_ADVERTISE
  TRANSITION_MAP_ENTRY(ST_RUN_AUTO)   // ST_BLE_CTRL_CONN
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_BLE_CTRL_DISCONN
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_BLE_BAD_CTRL_INPUT
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_RUN_EXPERIMENT
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_RUN_AUTO
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_STOP_RUN
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_RESTART
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_SHUTDOWN
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_FAILED_SETUP
  END_TRANSITION_MAP(NULL)
}

void twincentral::statemachine::TwinCentralStateMachine::stopCurrentRun() {
  BEGIN_TRANSITION_MAP
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_IDLE
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_STARTUP
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_BLE_DISCOVER_CONNECT
  TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_BLE_ADVERTISE
  TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_BLE_CTRL_CONN
  TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_BLE_CTRL_DISCONN
  TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_BLE_BAD_CTRL_INPUT
  TRANSITION_MAP_ENTRY(ST_STOP_RUN)   // ST_RUN_EXPERIMENT
  TRANSITION_MAP_ENTRY(ST_STOP_RUN)   // ST_RUN_AUTO
  TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_STOP_RUN
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_RESTART
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_SHUTDOWN
  TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_FAILED_SETUP
  END_TRANSITION_MAP(NULL)
}

void twincentral::statemachine::TwinCentralStateMachine::restart() {
  BEGIN_TRANSITION_MAP
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_IDLE
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_STARTUP
  TRANSITION_MAP_ENTRY(ST_RESTART)    // ST_BLE_DISCOVER_CONNECT
  TRANSITION_MAP_ENTRY(ST_RESTART)    // ST_BLE_ADVERTISE
  TRANSITION_MAP_ENTRY(ST_RESTART)    // ST_BLE_CTRL_CONN
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_BLE_CTRL_DISCONN
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_BLE_BAD_CTRL_INPUT
  TRANSITION_MAP_ENTRY(ST_RESTART)    // ST_RUN_EXPERIMENT
  TRANSITION_MAP_ENTRY(ST_RESTART)    // ST_RUN_AUTO
  TRANSITION_MAP_ENTRY(ST_RESTART)    // ST_STOP_RUN
  TRANSITION_MAP_ENTRY(ST_RESTART)    // ST_RESTART
  TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SHUTDOWN
  TRANSITION_MAP_ENTRY(ST_RESTART)    // ST_FAILED_SETUP
  END_TRANSITION_MAP(NULL)
}

void twincentral::statemachine::TwinCentralStateMachine::shutdown() {
  BEGIN_TRANSITION_MAP
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_IDLE
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_STARTUP
  TRANSITION_MAP_ENTRY(ST_SHUTDOWN)   // ST_BLE_DISCOVER_CONNECT
  TRANSITION_MAP_ENTRY(ST_SHUTDOWN)   // ST_BLE_ADVERTISE
  TRANSITION_MAP_ENTRY(ST_SHUTDOWN)   // ST_BLE_CTRL_CONN
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_BLE_CTRL_DISCONN
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_BLE_BAD_CTRL_INPUT
  TRANSITION_MAP_ENTRY(ST_SHUTDOWN)   // ST_RUN_EXPERIMENT
  TRANSITION_MAP_ENTRY(ST_SHUTDOWN)   // ST_RUN_AUTO
  TRANSITION_MAP_ENTRY(ST_SHUTDOWN)   // ST_STOP_RUN
  TRANSITION_MAP_ENTRY(ST_SHUTDOWN)   // ST_RESTART
  TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SHUTDOWN
  TRANSITION_MAP_ENTRY(ST_SHUTDOWN)   // ST_FAILED_SETUP
  END_TRANSITION_MAP(NULL)
}

void twincentral::statemachine::TwinCentralStateMachine::hasFailedSetup() {
  BEGIN_TRANSITION_MAP
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)   // ST_IDLE
  TRANSITION_MAP_ENTRY(ST_FAILED_SETUP) // ST_STARTUP
  TRANSITION_MAP_ENTRY(ST_FAILED_SETUP) // ST_BLE_DISCOVER_CONNECT
  TRANSITION_MAP_ENTRY(ST_FAILED_SETUP) // ST_BLE_ADVERTISE
  TRANSITION_MAP_ENTRY(ST_FAILED_SETUP) // ST_BLE_CTRL_CONN
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)   // ST_BLE_CTRL_DISCONN
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)   // ST_BLE_BAD_CTRL_INPUT
  TRANSITION_MAP_ENTRY(ST_FAILED_SETUP) // ST_RUN_EXPERIMENT
  TRANSITION_MAP_ENTRY(ST_FAILED_SETUP) // ST_RUN_AUTO
  TRANSITION_MAP_ENTRY(ST_FAILED_SETUP) // ST_STOP_RUN
  TRANSITION_MAP_ENTRY(ST_FAILED_SETUP) // ST_RESTART
  TRANSITION_MAP_ENTRY(ST_FAILED_SETUP) // ST_SHUTDOWN
  TRANSITION_MAP_ENTRY(ST_FAILED_SETUP) // ST_FAILED_SETUP
  END_TRANSITION_MAP(NULL)
}

void twincentral::statemachine::TwinCentralStateMachine::
    hasReceivedBadControlInput() {
  BEGIN_TRANSITION_MAP
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)         // ST_IDLE
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)         // ST_STARTUP
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)         // ST_BLE_DISCOVER_CONNECT
  TRANSITION_MAP_ENTRY(ST_BLE_BAD_CTRL_INPUT) // ST_BLE_ADVERTISE
  TRANSITION_MAP_ENTRY(ST_BLE_BAD_CTRL_INPUT) // ST_BLE_CTRL_CONN
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)         // ST_BLE_CTRL_DISCONN
  TRANSITION_MAP_ENTRY(EVENT_IGNORED)         // ST_BLE_BAD_CTRL_INPUT
  TRANSITION_MAP_ENTRY(EVENT_IGNORED)         // ST_RUN_EXPERIMENT
  TRANSITION_MAP_ENTRY(EVENT_IGNORED)         // ST_RUN_AUTO
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)         // ST_STOP_RUN
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)         // ST_RESTART
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)         // ST_SHUTDOWN
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)         // ST_FAILED_SETUP
  END_TRANSITION_MAP(NULL)
}

bool twincentral::statemachine::TwinCentralStateMachine::
    isRunExecutionSuspended() {
  return suspendRunExecution.load();
}

void twincentral::statemachine::TwinCentralStateMachine::cmdWrite(
    std::string msg) {
  std::regex pattern("^(\\w+)(?: (\\w+))?(?: (\\w+))?$");
  std::smatch match;

  if (std::regex_match(msg, match, pattern)) {
    try {
      const auto mStr = match[1].str();
      const auto cmd = twincentral::statemachine::ble::util::extractCmd(mStr);
      switch (cmd) {
        case twincentral::statemachine::ble::Cmd::START:
          {
            if (match.size() >= 4) {
              std::string patient =
                  twincentral::statemachine::ble::util::str_tolower(
                      match[2].str());
              const auto mode =
                  twincentral::statemachine::ble::util::extractMode(
                      match[3].str());
              ExperimentData data(patient, mode);
              startExperiment(&data);
            } else {
              hasReceivedBadControlInput();
            }
          }
          break;
        case twincentral::statemachine::ble::Cmd::STOP:
          {
            stopCurrentRun();
          }
          break;
        case twincentral::statemachine::ble::Cmd::TRIGGER_NORAXON:
          {
            try {
              auto res = twincentral::act::util::triggerNoraxon(peerList);
              if (res) {
                auto prevState = blinkStickLed.GetCurrentState();
                blinkStickLed.notifyHasTriggeredNoraxon();
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                blinkStickLed.forceStateTransition(prevState, NULL);
              }
            } catch (std::exception &e) {
            };
          }
          break;
        case twincentral::statemachine::ble::Cmd::RESTART:
          {
            restart();
          }
          break;
        case twincentral::statemachine::ble::Cmd::SHUTDOWN:
          {
            shutdown();
          }
          break;
      }
    } catch (
        twincentral::statemachine::ble::exception::OptionUnknownException &e) {
      hasReceivedBadControlInput();
    }
  } else {
    hasReceivedBadControlInput();
  }
}

std::string twincentral::statemachine::TwinCentralStateMachine::statusRead() {
  if (resultFileFromCurrentRun != nullptr) {
    return resultFileFromCurrentRun->getLatest();
  }
  return "Experiment not started yet";
}

void twincentral::statemachine::TwinCentralStateMachine::
    waitBeforeStateTransition(uint32_t ms) {
  std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

STATE_DEFINE(twincentral::statemachine::TwinCentralStateMachine, Idle,
             NoEventData) {
  printf("[%s] %s\n", TAG.c_str(), "Device is in state Idle");
}

STATE_DEFINE(twincentral::statemachine::TwinCentralStateMachine, StartUp,
             NoEventData) {
  printf("[%s] %s\n", TAG.c_str(), "Device is in state StartUp");
  blinkStickLed.init();
  blinkStickLed.notifyIsStarting();

  static std::function<std::string(void)> onread = std::bind(
      &twincentral::statemachine::TwinCentralStateMachine::statusRead, this);
  static std::function<void(std::string)> onwrite =
      std::bind(&twincentral::statemachine::TwinCentralStateMachine::cmdWrite,
                this, std::placeholders::_1);

  try {
    bleStack.setupAdvertisement(*systemBusConnection, onread, onwrite);
    hci0Adapter = bleStack.startBluez(twincentral::app::config::adapter);
  } catch (std::exception &e) {
    InternalEvent(ST_FAILED_SETUP);
  }

  waitBeforeStateTransition(3000);
  InternalEvent(ST_BLE_DISCOVER_CONNECT);
}

ENTRY_DEFINE(twincentral::statemachine::TwinCentralStateMachine,
             EntryBLEDiscoverConnect, NoEventData) {}

STATE_DEFINE(twincentral::statemachine::TwinCentralStateMachine,
             BLEDiscoverConnect, NoEventData) {
  printf("[%s] %s\n", TAG.c_str(), "Device is in state BLEDiscoverConnect");
  blinkStickLed.notifyIsBLEDiscoveringConnecting();

  SimpleBluez::Adapter::DiscoveryFilter filter;
  filter.Transport = SimpleBluez::Adapter::DiscoveryFilter::TransportType::LE;
  filter.DuplicateData = false;
  hci0Adapter->discovery_filter(filter);

  hci0Adapter->set_on_device_updated(
      [this](std::shared_ptr<SimpleBluez::Device> device) {
        printf("[%s] %s [%s]%s\n", TAG.c_str(),
               "Discovered peripheral: ", device->name().c_str(),
               device->address().c_str());
        if ((twincentral::app::config::bleTwinPeripheralsByName.find(
                 device->name()) !=
             twincentral::app::config::bleTwinPeripheralsByName.end()) &&
            !peerList.exists(device->name())) {
          peerList.add(device->name(), device);
          printf("[%s] %s [%s]%s %s\n", TAG.c_str(),
                 "Added peripheral: ", device->name().c_str(),
                 device->address().c_str(), " to list");
        }
      });

  printf("[%s] %s\n", TAG.c_str(), "Starting discovery");
  try {
    hci0Adapter->discovery_start();
    auto discConditionMet = peerList.waitForDiscovery();
    hci0Adapter->discovery_stop();
    std::this_thread::sleep_for(std::chrono::seconds(2));
    printf("[%s] %s\n", TAG.c_str(), "Discovery has been stopped");
    if (!discConditionMet) {
      InternalEvent(ST_FAILED_SETUP);
    } else {
      std::for_each(peerList.begin(), peerList.end(),
                    [this](auto &keyValue) mutable {
                      try {
                        twincentral::ble::connectPeer(keyValue.second);
                      } catch (std::exception &e) {
                        twincentral::os::restartBluetoothService();
                        InternalEvent(ST_FAILED_SETUP);
                      }
                    });

      if (!peerList.isAllConnected()) {
        twincentral::os::restartBluetoothService();
        InternalEvent(ST_FAILED_SETUP);
      } else {
        InternalEvent(ST_BLE_ADVERTISE);
      }
    }
  } catch (std::exception &e) {
    twincentral::os::restartBluetoothService();
    InternalEvent(ST_FAILED_SETUP);
  }
}

EXIT_DEFINE(twincentral::statemachine::TwinCentralStateMachine,
            ExitBLEDiscoverConnect) {}

ENTRY_DEFINE(twincentral::statemachine::TwinCentralStateMachine,
             EntryBLEAdvertise, NoEventData) {
  // Do nothing
}

STATE_DEFINE(twincentral::statemachine::TwinCentralStateMachine, BLEAdvertise,
             NoEventData) {
  printf("[%s] %s\n", TAG.c_str(), "Device is in state BLEAdvertise");
  blinkStickLed.notifyIsBLEDAdvertising();
  bleStack.startAdvertisement(*systemBusConnection);
}

EXIT_DEFINE(twincentral::statemachine::TwinCentralStateMachine,
            ExitBLEAdvertise) {}

STATE_DEFINE(twincentral::statemachine::TwinCentralStateMachine,
             BLEControllerConnection, NoEventData) {
  printf("[%s] %s\n", TAG.c_str(),
         "Device is in state BLEControllerConnection");
  blinkStickLed.notifyIsBLEDAdvertising();
}

STATE_DEFINE(twincentral::statemachine::TwinCentralStateMachine,
             BLEControllerDisconnection, NoEventData) {
  printf("[%s] %s\n", TAG.c_str(),
         "Device is in state BLEControllerDisconnection");
  InternalEvent(ST_BLE_ADVERTISE);
}

STATE_DEFINE(twincentral::statemachine::TwinCentralStateMachine,
             BLEBadControlInput, NoEventData) {
  printf("[%s] %s\n", TAG.c_str(), "Device is in state BLEBadControlInput");
  blinkStickLed.notifyHasReceivedBadControlInput();
  waitBeforeStateTransition(5000);
  InternalEvent(ST_BLE_CTRL_CONN);
}

ENTRY_DEFINE(twincentral::statemachine::TwinCentralStateMachine,
             EntryRunExperiment, ExperimentData) {
  suspendRunExecution.store(false);
}

STATE_DEFINE(twincentral::statemachine::TwinCentralStateMachine, RunExperiment,
             ExperimentData) {
  printf("[%s] %s\n", TAG.c_str(), "Device is in state RunExperiment");
  blinkStickLed.notifyIsRunningCalc();
  std::time_t t = std::time(nullptr);
  std::tm tm = *std::localtime(&t);

  std::stringstream ss;

  ss << std::put_time(&tm, "%Y-%m-%d_%H-%M-%S");
  std::string timestamp = ss.str();

  auto dir = biometricsFolder + "experiment/results/" + data->patient + "/" +
             data->mode.toString() + "/" + timestamp + "/";
  std::string fileName = "result.csv";
  resultFileFromCurrentRun =
      std::make_shared<twincentral::io::file::SynchronizedBioMetricsFile>(
          dir, fileName);
  twincentral::act::startExperiment(*this, resultFileFromCurrentRun);
}

EXIT_DEFINE(twincentral::statemachine::TwinCentralStateMachine,
            ExitRunExperiment) {
  suspendRunExecution.store(true);
  auto res = twincentral::act::util::triggerNoraxon(peerList);
  if (res) {
    auto prevState = blinkStickLed.GetCurrentState();
    blinkStickLed.notifyHasTriggeredNoraxon();
    std::this_thread::sleep_for(std::chrono::milliseconds(
        twincentral::app::config::noraxonTriggerSuccessNotificationDuration));
    blinkStickLed.forceStateTransition(prevState, NULL);
  }
}

// TODO
STATE_DEFINE(twincentral::statemachine::TwinCentralStateMachine, RunAuto,
             NoEventData) {
  printf("[%s] %s\n", TAG.c_str(), "Device is in state RunAuto");
}

STATE_DEFINE(twincentral::statemachine::TwinCentralStateMachine, StopRun,
             NoEventData) {
  printf("[%s] %s\n", TAG.c_str(), "Device is in state StopRun");
  InternalEvent(ST_BLE_CTRL_CONN);
}

STATE_DEFINE(twincentral::statemachine::TwinCentralStateMachine, Restart,
             NoEventData) {
  printf("[%s] %s\n", TAG.c_str(), "Device is in state Restart");
  if (twincentral::os::restart(5)) {
    blinkStickLed.notifyIsRestarting();
  }
}

STATE_DEFINE(twincentral::statemachine::TwinCentralStateMachine, Shutdown,
             NoEventData) {
  printf("[%s] %s\n", TAG.c_str(), "Device is in state Shutdown");
  if (twincentral::os::shutdown(5)) {
    blinkStickLed.notifyIsRestarting();
  }
}

STATE_DEFINE(twincentral::statemachine::TwinCentralStateMachine, FailedSetup,
             NoEventData) {
  printf("[%s] %s\n", TAG.c_str(), "Device is in state FailedSetup");
  blinkStickLed.notifyHasFailedSetup();
  peerList.disconnect();
  waitBeforeStateTransition(5000);
  throw std::exception();
}