template <typename... Channels>
twinperipheral::statemachine::TwinPeripheralStateMachine<Channels...>::
    TwinPeripheralStateMachine(SemaphoreHandle_t stateMutex,
                               twinperipheral::system::SystemController &sys,
                               twinperipheral::sense::TwinBNO055 &bno,
                               twinperipheral::sense::TwinADC<Channels...> &adc,
                               twinperipheral::led::RGBLED &led)
    : StateMachine(stateMutex, ST_MAX_STATES), sys(sys), bno(bno), adc(adc),
      led(led){};

template <typename... Channels>
std::string
    twinperipheral::statemachine::TwinPeripheralStateMachine<Channels...>::TAG =
        "TwinPeripheralStateMachine";

template <typename... Channels>
void twinperipheral::statemachine::TwinPeripheralStateMachine<
    Channels...>::performStartUpRoutine() {
  BEGIN_TRANSITION_MAP                // - Current State -
  TRANSITION_MAP_ENTRY(ST_STARTUP)    // ST_Idle
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_StartUp
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_IMU_CALIBRATION
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_BLE_ADV_CALIBRATION
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_OPERATIONAL
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_BLE_ADV
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_BLEConnection
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_BLEDisconnection
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_SendData
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_FailedWiring
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_Sleep
  END_TRANSITION_MAP(NULL)
}

template <typename... Channels>
void twinperipheral::statemachine::TwinPeripheralStateMachine<
    Channels...>::notifyIMUCalibrationSuccess() {
  BEGIN_TRANSITION_MAP                 // - Current State -
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)  // ST_Idle
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)  // ST_StartUp
  TRANSITION_MAP_ENTRY(ST_OPERATIONAL) // ST_IMU_CALIBRATION
  TRANSITION_MAP_ENTRY(ST_OPERATIONAL) // ST_BLE_ADV_CALIBRATION
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)  // ST_OPERATIONAL
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)  // ST_BLE_ADV
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)  // ST_BLEConnection
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)  // ST_BLEDisconnection
  TRANSITION_MAP_ENTRY(ST_OPERATIONAL) // ST_SendData
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)  // ST_FailedWiring
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)  // ST_Sleep
  END_TRANSITION_MAP(NULL)
}

template <typename... Channels>
void twinperipheral::statemachine::TwinPeripheralStateMachine<
    Channels...>::notifyIMUCalibrationFailure() {
  BEGIN_TRANSITION_MAP                   // - Current State -
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)    // ST_Idle
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)    // ST_StartUp
  TRANSITION_MAP_ENTRY(ST_FAILED_WIRING) // ST_IMU_CALIBRATION
  TRANSITION_MAP_ENTRY(ST_FAILED_WIRING) // ST_BLE_ADV_CALIBRATION
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)    // ST_OPERATIONAL
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)    // ST_BLE_ADV
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)    // ST_BLEConnection
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)    // ST_BLEDisconnection
  TRANSITION_MAP_ENTRY(ST_FAILED_WIRING) // ST_SendData
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)    // ST_FailedWiring
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)    // ST_Sleep
  END_TRANSITION_MAP(NULL)
}

template <typename... Channels>
void twinperipheral::statemachine::TwinPeripheralStateMachine<
    Channels...>::startBLECalibrationAdvertisement() {
  BEGIN_TRANSITION_MAP                         // - Current State -
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)          // ST_Idle
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)          // ST_StartUp
  TRANSITION_MAP_ENTRY(ST_BLE_ADV_CALIBRATION) // ST_IMU_CALIBRATION
  TRANSITION_MAP_ENTRY(ST_BLE_ADV_CALIBRATION) // ST_BLE_ADV_CALIBRATION
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)          // ST_OPERATIONAL
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)          // ST_BLE_ADV
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)          // ST_BLEConnection
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)          // ST_BLEDisconnection
  TRANSITION_MAP_ENTRY(ST_BLE_ADV_CALIBRATION) // ST_SendData
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)          // ST_FailedWiring
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)          // ST_Sleep
  END_TRANSITION_MAP(NULL)
}

template <typename... Channels>
void twinperipheral::statemachine::TwinPeripheralStateMachine<
    Channels...>::startBLEAdvertisement() {
  BEGIN_TRANSITION_MAP                // - Current State -
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_Idle
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_StartUp
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_IMU_CALIBRATION
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_BLE_ADV_CALIBRATION
  TRANSITION_MAP_ENTRY(ST_BLE_ADV)    // ST_OPERATIONAL
  TRANSITION_MAP_ENTRY(ST_BLE_ADV)    // ST_BLE_ADV
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_BLEConnection
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_BLEDisconnection
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_SendData
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_FailedWiring
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_Sleep
  END_TRANSITION_MAP(NULL)
}

template <typename... Channels>
void twinperipheral::statemachine::TwinPeripheralStateMachine<
    Channels...>::notifyBLEConnection() {
  BEGIN_TRANSITION_MAP                    // - Current State -
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)     // ST_Idle
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)     // ST_StartUp
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)     // ST_IMU_CALIBRATION
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)     // ST_BLE_ADV_CALIBRATION
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)     // ST_OPERATIONAL
  TRANSITION_MAP_ENTRY(ST_BLE_CONNECTION) // ST_BLE_ADV
  TRANSITION_MAP_ENTRY(EVENT_IGNORED)     // ST_BLEConnection
  TRANSITION_MAP_ENTRY(EVENT_IGNORED)     // ST_BLEDisconnection
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)     // ST_SendData
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)     // ST_FailedWiring
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)     // ST_Sleep
  END_TRANSITION_MAP(NULL)
}

template <typename... Channels>
void twinperipheral::statemachine::TwinPeripheralStateMachine<
    Channels...>::notifyBLEDisconnection() {
  BEGIN_TRANSITION_MAP                       // - Current State -
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)        // ST_Idle
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)        // ST_StartUp
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)        // ST_IMU_CALIBRATION
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)        // ST_BLE_ADV_CALIBRATION
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)        // ST_OPERATIONAL
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)        // ST_BLE_ADV
  TRANSITION_MAP_ENTRY(ST_BLE_DISCONNECTION) // ST_BLEConnection
  TRANSITION_MAP_ENTRY(EVENT_IGNORED)        // ST_BLEDisconnection
  TRANSITION_MAP_ENTRY(ST_BLE_DISCONNECTION) // ST_SendData
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)        // ST_FailedWiring
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)        // ST_Sleep
  END_TRANSITION_MAP(NULL)
}

template <typename... Channels>
void twinperipheral::statemachine::TwinPeripheralStateMachine<
    Channels...>::sendData() {
  BEGIN_TRANSITION_MAP                // - Current State -
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_Idle
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_StartUp
  TRANSITION_MAP_ENTRY(ST_SEND_DATA)  // ST_IMU_CALIBRATION
  TRANSITION_MAP_ENTRY(ST_SEND_DATA)  // ST_BLE_ADV_CALIBRATION
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_OPERATIONAL
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_BLE_ADV
  TRANSITION_MAP_ENTRY(ST_SEND_DATA)  // ST_BLEConnection
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_BLEDisconnection
  TRANSITION_MAP_ENTRY(ST_SEND_DATA)  // ST_SendData
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_FailedWiring
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_Sleep
  END_TRANSITION_MAP(NULL)
}

template <typename... Channels>
void twinperipheral::statemachine::TwinPeripheralStateMachine<
    Channels...>::sleep() {
  BEGIN_TRANSITION_MAP                // - Current State -
  TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_Idle
  TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_StartUp
  TRANSITION_MAP_ENTRY(ST_SLEEP)      // ST_IMU_CALIBRATION
  TRANSITION_MAP_ENTRY(ST_SLEEP)      // ST_BLE_ADV_CALIBRATION
  TRANSITION_MAP_ENTRY(ST_SLEEP)      // ST_OPERATIONAL
  TRANSITION_MAP_ENTRY(ST_SLEEP)      // ST_BLE_ADV
  TRANSITION_MAP_ENTRY(ST_SLEEP)      // ST_BLEConnection
  TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_BLEDisconnection
  TRANSITION_MAP_ENTRY(ST_SLEEP)      // ST_SendData
  TRANSITION_MAP_ENTRY(ST_SLEEP)      // ST_FailedWiring
  TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_Sleep
  END_TRANSITION_MAP(NULL)
}

template <typename... Channels>
void twinperipheral::statemachine::TwinPeripheralStateMachine<
    Channels...>::waitBeforeStateTransition(int ms) {
  vTaskDelay(ms / portTICK_PERIOD_MS);
}

template <typename... Channels>
STATE_DEFINE(
    twinperipheral::statemachine::TwinPeripheralStateMachine<Channels...>, Idle,
    NoEventData) {
  ESP_LOGI(TAG.c_str(), "Device is in state Idle");
}

template <typename... Channels>
STATE_DEFINE(
    twinperipheral::statemachine::TwinPeripheralStateMachine<Channels...>,
    StartUp, NoEventData) {
  ESP_LOGI(TAG.c_str(), "Device is in state StartUp");
  led.init();
  led.notifyIsStarting();
  sys.configureDeepSleepWakeUp();
  sys.startBatteryMonotoring();

  try {
    bno.powerOn(2000);
    bno.run();
  } catch (BNO055I2CError &ex) {
    InternalEvent(ST_FAILED_WIRING);
  }

  static std::function<void()> pollShutdownSignalCb = [this]() { sleep(); };

  static auto pollShutdownSignalTask = [](void *_this) {
    static_cast<twinperipheral::statemachine::TwinPeripheralStateMachine<
        Channels...> *>(_this)
        ->sys.pollShutdownSignal(pollShutdownSignalCb);
  };

#if TWIN_CRUTCH_LEFT || TWIN_CRUTCH_RIGHT
  static std::function<void()> pollHX711TareSignalCb = [this]() { adc.tare(); };

  static auto pollHX711TareSignalTask = [](void *_this) {
    static_cast<twinperipheral::statemachine::TwinPeripheralStateMachine<
        Channels...> *>(_this)
        ->sys.pollHX711TareSignal(pollHX711TareSignalCb);
  };
#endif

  static auto idleTimeMonitoringCb = [](TimerHandle_t xTimer) {
    auto ctx = pvTimerGetTimerID(xTimer);
    static_cast<twinperipheral::statemachine::TwinPeripheralStateMachine<
        Channels...> *>(ctx)
        ->sleep();
  };

  xTaskCreatePinnedToCore(pollShutdownSignalTask, "pollShutdownSignalTask",
                          8092, this, 1, NULL,
                          twinperipheral::config::app::app_cpu);

#if TWIN_CRUTCH_LEFT || TWIN_CRUTCH_RIGHT
  xTaskCreatePinnedToCore(pollHX711TareSignalTask, "pollHX711TareSignalTask",
                          8092, this, 1, NULL,
                          twinperipheral::config::app::app_cpu);
#endif

  idleTimer = xTimerCreate("Idle Time Monotoring", // Name of timer
                           twinperipheral::config::app::idleTime /
                               portTICK_PERIOD_MS, // Period of timer (in ticks)
                           pdFALSE,                // Auto-reload
                           (void *)this,           // Timer ID
                           idleTimeMonitoringCb);
  xTimerStop(idleTimer, portMAX_DELAY);
  waitBeforeStateTransition(2000); // 2s
  InternalEvent(ST_IMU_CALIBRATION);
}

template <typename... Channels>
ENTRY_DEFINE(
    twinperipheral::statemachine::TwinPeripheralStateMachine<Channels...>,
    EntryIMUCalibration, NoEventData) {
  xTimerReset(idleTimer, portMAX_DELAY);
}

template <typename... Channels>
STATE_DEFINE(
    twinperipheral::statemachine::TwinPeripheralStateMachine<Channels...>,
    IMUCalibration, NoEventData) {
  ESP_LOGI(TAG.c_str(), "Device is in state IMUCalibration");
  led.notifyIsCalibratingIMU();

  static auto imuCalibrationTask = [](void *_this) {
    auto machine =
        static_cast<twinperipheral::statemachine::TwinPeripheralStateMachine<
            Channels...> *>(_this);
    auto res = machine->bno.runCalibration();
    vTaskDelay(100);
    if (!res) {
      machine->notifyIMUCalibrationFailure();
    } else {
      machine->notifyIMUCalibrationSuccess();
    }
    vTaskDelete(NULL);
  };

  xTaskCreatePinnedToCore(imuCalibrationTask, "imuCalibrationTask", 8092, this,
                          2, NULL, twinperipheral::config::app::app_cpu);
  InternalEvent(ST_BLE_ADV_CALIBRATION);
}

template <typename... Channels>
EXIT_DEFINE(
    twinperipheral::statemachine::TwinPeripheralStateMachine<Channels...>,
    ExitIMUCalibration) {
  xTimerStop(idleTimer, portMAX_DELAY);
}

template <typename... Channels>
ENTRY_DEFINE(
    twinperipheral::statemachine::TwinPeripheralStateMachine<Channels...>,
    EntryBLECalibrationAdv, NoEventData) {
  xTimerReset(idleTimer, portMAX_DELAY);
}

template <typename... Channels>
STATE_DEFINE(
    twinperipheral::statemachine::TwinPeripheralStateMachine<Channels...>,
    BLECalibrationAdv, NoEventData) {
  ESP_LOGI(TAG.c_str(), "Device is in state BLECalibrationAdv");
  auto bleName =
      twinperipheral::config::app::blePeripheralDeviceName + "-IMU_CALIB";
  twinperipheral::ble::Stack::Peripheral::IS_FOR_SENDING_MEASUREMENT.store(
      false);

  if (!twinperipheral::ble::Stack::IS_RUNNING) {
    twinperipheral::ble::Stack::start(
        bleName,
        twinperipheral::statemachine::ble::genGapEventHandler<Channels...>(),
        this,
        twinperipheral::ble::app::gatt_svcs::getGattSvcDefs(
            twinperipheral::statemachine::ble::genReadBLEAccessFunc<
                Channels...>(),
            this));
  } else {
    twinperipheral::ble::Stack::Peripheral::startAdvertising(
        twinperipheral::statemachine::ble::genGapEventHandler<Channels...>(),
        this);
  }
}

template <typename... Channels>
EXIT_DEFINE(
    twinperipheral::statemachine::TwinPeripheralStateMachine<Channels...>,
    ExitBLECalibrationAdv) {
  xTimerStop(idleTimer, portMAX_DELAY);
}

template <typename... Channels>
STATE_DEFINE(
    twinperipheral::statemachine::TwinPeripheralStateMachine<Channels...>,
    Operational, NoEventData) {
  ESP_LOGI(TAG.c_str(), "Device is in state Operational");
  led.notifyIsOperational();
  twinperipheral::ble::Stack::stop();
  waitBeforeStateTransition(2000); // 2s
  InternalEvent(ST_BLE_ADV);
}

template <typename... Channels>
ENTRY_DEFINE(
    twinperipheral::statemachine::TwinPeripheralStateMachine<Channels...>,
    EntryBLEAdv, NoEventData) {
  xTimerReset(idleTimer, portMAX_DELAY);
}

template <typename... Channels>
STATE_DEFINE(
    twinperipheral::statemachine::TwinPeripheralStateMachine<Channels...>,
    BLEAdv, NoEventData) {
  ESP_LOGI(TAG.c_str(), "Device is in state BLEAdv");
  twinperipheral::ble::Stack::Peripheral::IS_FOR_SENDING_MEASUREMENT.store(
      true);

  if (!twinperipheral::ble::Stack::IS_RUNNING) {
    twinperipheral::ble::Stack::start(
        twinperipheral::config::app::blePeripheralDeviceName,
        twinperipheral::statemachine::ble::genGapEventHandler<Channels...>(),
        this,
        twinperipheral::ble::app::gatt_svcs::getGattSvcDefs(
            twinperipheral::statemachine::ble::genReadBLEAccessFunc<
                Channels...>(),
            this));
  } else {
    twinperipheral::ble::Stack::Peripheral::startAdvertising(
        twinperipheral::statemachine::ble::genGapEventHandler<Channels...>(),
        this);
  }
  led.notifyIsBLEReady();
}

template <typename... Channels>
EXIT_DEFINE(
    twinperipheral::statemachine::TwinPeripheralStateMachine<Channels...>,
    ExitBLEAdv) {
  xTimerStop(idleTimer, portMAX_DELAY);
}

template <typename... Channels>
ENTRY_DEFINE(
    twinperipheral::statemachine::TwinPeripheralStateMachine<Channels...>,
    EntryBLEConnection, NoEventData) {
  xTimerReset(idleTimer, portMAX_DELAY);
}

template <typename... Channels>
STATE_DEFINE(
    twinperipheral::statemachine::TwinPeripheralStateMachine<Channels...>,
    BLEConnection, NoEventData) {
  ESP_LOGI(TAG.c_str(), "Device is in state BLEConnection");
  led.notifyIsBLEConnected();
}

template <typename... Channels>
EXIT_DEFINE(
    twinperipheral::statemachine::TwinPeripheralStateMachine<Channels...>,
    ExitBLEConnection) {
  xTimerStop(idleTimer, portMAX_DELAY);
}

template <typename... Channels>
STATE_DEFINE(
    twinperipheral::statemachine::TwinPeripheralStateMachine<Channels...>,
    BLEDisconnection, NoEventData) {
  ESP_LOGI(TAG.c_str(), "Device is in state BLEDisconnection");
  twinperipheral::ble::Stack::Peripheral::purgePendingMessage();
  InternalEvent(ST_BLE_ADV);
}

template <typename... Channels>
STATE_DEFINE(
    twinperipheral::statemachine::TwinPeripheralStateMachine<Channels...>,
    SendData, NoEventData) {
  ESP_LOGI(TAG.c_str(), "Device is in state SendData");
  std::string msg;
  try {
    if (twinperipheral::ble::Stack::Peripheral::IS_FOR_SENDING_MEASUREMENT
            .load()) {
      led.notifyIsSendingData();
      msg = twinperipheral::ble::generateBLEMessage(
          bno.getEulerAngles(), bno.getLinearAccel(), bno.getQuaternion(),
          adc.readADCValues());
    } else {
      msg = twinperipheral::ble::generateBLEMessage(bno.getCalibrationStatus());
    }
    waitBeforeStateTransition(10);
    twinperipheral::ble::Stack::Peripheral::placeMessageToSend(msg);
  } catch (std::exception &ex) {
    InternalEvent(ST_FAILED_WIRING);
  }
}

template <typename... Channels>
ENTRY_DEFINE(
    twinperipheral::statemachine::TwinPeripheralStateMachine<Channels...>,
    EntryFailedWiring, NoEventData) {
  xTimerReset(idleTimer, portMAX_DELAY);
}

template <typename... Channels>
STATE_DEFINE(
    twinperipheral::statemachine::TwinPeripheralStateMachine<Channels...>,
    FailedWiring, NoEventData) {
  ESP_LOGI(TAG.c_str(), "Device is in state FailedWiring");
  twinperipheral::ble::Stack::stop();
  led.notifyhasFailedWiring();
}

template <typename... Channels>
EXIT_DEFINE(
    twinperipheral::statemachine::TwinPeripheralStateMachine<Channels...>,
    ExitFailedWiring) {
  xTimerStop(idleTimer, portMAX_DELAY);
}

template <typename... Channels>
STATE_DEFINE(
    twinperipheral::statemachine::TwinPeripheralStateMachine<Channels...>,
    Sleep, NoEventData) {
  ESP_LOGI(TAG.c_str(), "Device is in state Sleep");
  twinperipheral::ble::Stack::stop();
  led.stop();
  bno.powerOff();
  sys.startDeepSleep();
}