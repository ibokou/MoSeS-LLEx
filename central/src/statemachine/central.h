#ifndef TWINCENTRAL_STATEMACHINE_CENTRAL_INCLUDED
#define TWINCENTRAL_STATEMACHINE_CENTRAL_INCLUDED

#include "../ble/peer.h"
#include "../ble/stack.h"
#include "../io/file.h"
#include "../led/blinkstick.h"
#include "../plan/gait_estimation.h"
#include "StateMachine_.h"
#include <atomic>
#include <cstdio>
#include <thread>

namespace twincentral::statemachine {

  namespace ble {
    enum class Cmd { START, STOP, TRIGGER_NORAXON, RESTART, SHUTDOWN };

    class ExpMode {
    public:
      enum Value : uint8_t { WITHOUT, EXO_AUTO };

      ExpMode() = default;
      constexpr ExpMode(Value mode) : value(mode) {}

      constexpr operator Value() const { return value; }

      std::string toString() const {
        switch (value) {
          case ExpMode::WITHOUT:
            return "without_exo";
          case ExpMode::EXO_AUTO:
            return "exo_auto";
          default:
            assert(false);
        }
      }

    private:
      Value value;
    };
  }

  struct ExperimentData : public EventData {
    ExperimentData(const std::string &patient,
                   twincentral::statemachine::ble::ExpMode mode)
        : patient(patient), mode(mode){};
    std::string patient;
    twincentral::statemachine::ble::ExpMode mode;
  };

  class TwinCentralStateMachine : public StateMachine {

  public:
    TwinCentralStateMachine(
        twincentral::led::BlinkStick &blinkStickLed,
        twincentral::plan::GaitEstimator &gaitEstimator,
        std::unique_ptr<sdbus::IConnection> systemBusConnection,
        std::string biometricsFolder = "biometrics/")
        : StateMachine(ST_MAX_STATES), blinkStickLed(blinkStickLed),
          gaitEstimator(gaitEstimator), bleStack(), peerList(),
          systemBusConnection(std::move(systemBusConnection)),
          biometricsFolder(biometricsFolder), suspendRunExecution(true) {}

    void run();
    void startExperiment(ExperimentData *data);
    void startAutoMode();
    void stopCurrentRun();
    void hasFailedSetup();
    void notifyBLEControllerAppHasConnected();
    void notifyBLEControllerAppHasDisconnected();
    void hasReceivedBadControlInput();
    void restart();
    void shutdown();
    bool isRunExecutionSuspended();
    twincentral::ble::PeerList peerList;
    twincentral::plan::GaitEstimator &gaitEstimator;
    twincentral::led::BlinkStick &blinkStickLed;
    static std::string TAG;

  private:
    enum States {
      ST_IDLE,
      ST_STARTUP,
      ST_BLE_DISCOVER_CONNECT,
      ST_BLE_ADVERTISE,
      ST_BLE_CTRL_CONN,    // App (BLE Scanner) connects to Raspi
      ST_BLE_CTRL_DISCONN, // App (BLE Scanner) disconnects
      ST_BLE_BAD_CTRL_INPUT,
      ST_RUN_EXPERIMENT,
      ST_RUN_AUTO,
      ST_STOP_RUN,
      ST_RESTART,
      ST_SHUTDOWN,
      ST_FAILED_SETUP,
      ST_MAX_STATES
    };

    // Define the state machine state functions with event data type
    STATE_DECLARE(TwinCentralStateMachine, Idle, NoEventData)
    STATE_DECLARE(TwinCentralStateMachine, StartUp, NoEventData)

    ENTRY_DECLARE(TwinCentralStateMachine, EntryBLEDiscoverConnect, NoEventData)
    STATE_DECLARE(TwinCentralStateMachine, BLEDiscoverConnect, NoEventData)
    EXIT_DECLARE(TwinCentralStateMachine, ExitBLEDiscoverConnect)

    ENTRY_DECLARE(TwinCentralStateMachine, EntryBLEAdvertise, NoEventData)
    STATE_DECLARE(TwinCentralStateMachine, BLEAdvertise, NoEventData)
    EXIT_DECLARE(TwinCentralStateMachine, ExitBLEAdvertise)

    STATE_DECLARE(TwinCentralStateMachine, BLEControllerConnection, NoEventData)
    STATE_DECLARE(TwinCentralStateMachine, BLEControllerDisconnection,
                  NoEventData)

    STATE_DECLARE(TwinCentralStateMachine, BLEBadControlInput, NoEventData)

    ENTRY_DECLARE(TwinCentralStateMachine, EntryRunExperiment, ExperimentData)
    STATE_DECLARE(TwinCentralStateMachine, RunExperiment, ExperimentData)
    EXIT_DECLARE(TwinCentralStateMachine, ExitRunExperiment)

    STATE_DECLARE(TwinCentralStateMachine, RunAuto, NoEventData)

    STATE_DECLARE(TwinCentralStateMachine, StopRun, NoEventData)

    STATE_DECLARE(TwinCentralStateMachine, Restart, NoEventData)

    STATE_DECLARE(TwinCentralStateMachine, Shutdown, NoEventData)

    STATE_DECLARE(TwinCentralStateMachine, FailedSetup, NoEventData)

    BEGIN_STATE_MAP_EX
    STATE_MAP_ENTRY_EX(&Idle)
    STATE_MAP_ENTRY_EX(&StartUp)
    STATE_MAP_ENTRY_ALL_EX(&BLEDiscoverConnect, 0, &EntryBLEDiscoverConnect,
                           &ExitBLEDiscoverConnect)
    STATE_MAP_ENTRY_ALL_EX(&BLEAdvertise, 0, &EntryBLEAdvertise,
                           &ExitBLEAdvertise)
    STATE_MAP_ENTRY_EX(&BLEControllerConnection)
    STATE_MAP_ENTRY_EX(&BLEControllerDisconnection)
    STATE_MAP_ENTRY_EX(&BLEBadControlInput)
    STATE_MAP_ENTRY_ALL_EX(&RunExperiment, 0, &EntryRunExperiment,
                           &ExitRunExperiment)
    STATE_MAP_ENTRY_EX(&RunAuto)
    STATE_MAP_ENTRY_EX(&StopRun)
    STATE_MAP_ENTRY_EX(&Restart)
    STATE_MAP_ENTRY_EX(&Shutdown)
    STATE_MAP_ENTRY_EX(&FailedSetup)
    END_STATE_MAP_EX
    void waitBeforeStateTransition(uint32_t ms);
    void cmdWrite(std::string msg);
    std::string statusRead();
    twincentral::ble::Stack bleStack;
    std::shared_ptr<SimpleBluez::Adapter> hci0Adapter;
    std::string biometricsFolder;
    std::unique_ptr<sdbus::IConnection> systemBusConnection;
    std::atomic_bool suspendRunExecution;
    std::shared_ptr<twincentral::io::file::SynchronizedBioMetricsFile>
        resultFileFromCurrentRun;
  };

  namespace ble {

    namespace exception {
      class OptionUnknownException
          : public twincentral::exception::TwincentralBaseException {
        using twincentral::exception::TwincentralBaseException::
            TwincentralBaseException;
      };
    }

    namespace util {

      static std::string str_tolower(std::string s) {
        std::transform(s.begin(), s.end(), s.begin(),
                       [](unsigned char c) { return std::tolower(c); });
        return s;
      }

      static twincentral::statemachine::ble::Cmd
      extractCmd(const std::string &cmd) {
        auto _cmd = str_tolower(cmd);
        if (_cmd.compare("start") == 0) {
          return twincentral::statemachine::ble::Cmd::START;
        } else if (_cmd.compare("stop") == 0) {
          return twincentral::statemachine::ble::Cmd::STOP;
        } else if (_cmd.compare("trigger") == 0) {
          return twincentral::statemachine::ble::Cmd::TRIGGER_NORAXON;
        } else if (_cmd.compare("restart") == 0) {
          return twincentral::statemachine::ble::Cmd::RESTART;
        } else if (_cmd.compare("shutdown") == 0) {
          return twincentral::statemachine::ble::Cmd::SHUTDOWN;
        } else {
          throw twincentral::statemachine::ble::exception::
              OptionUnknownException();
        }
      }
      static twincentral::statemachine::ble::ExpMode
      extractMode(const std::string &mode) {
        auto _mode = str_tolower(mode);
        if (_mode.compare("noexo") == 0) {
          return twincentral::statemachine::ble::ExpMode::WITHOUT;
        } else if (_mode.compare("exo") == 0) {
          return twincentral::statemachine::ble::ExpMode::EXO_AUTO;
        } else {
          throw twincentral::statemachine::ble::exception::
              OptionUnknownException();
        }
      }
    }
  }
}

#endif
