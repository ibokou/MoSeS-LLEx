#include "stack.h"
#include "../statemachine/central.h"
#include "bluez/advertise.h"
#include <memory>

void twincentral::ble::Stack::setupAdvertisement(
    sdbus::IConnection &dbusConnection, std::function<std::string(void)> onread,
    std::function<void(std::string)> onwrite) {
  std::thread([&dbusConnection, onread, onwrite] {
    GATTApplication application(dbusConnection, GATT_APP_OBJ_PATH);
    ControlAdvertiser advertiser(dbusConnection, GATT_ADV_OBJ_PATH);
    ControlGattService service(dbusConnection, GATT_SVC_OBJ_PATH);
    ControlGattCharacteristic chr(dbusConnection, GATT_CHR_OBJ_PATH, onread,
                                  onwrite);

    dbusConnection.enterEventLoop();
  }).detach();
}

std::shared_ptr<SimpleBluez::Adapter>
twincentral::ble::Stack::startBluez(std::size_t adapter) {
  bluez.init();
  std::thread([this] {
    while (this->asyncBluezActive.load()) {
      this->bluez.run_async();
      std::this_thread::sleep_for(std::chrono::microseconds(100));
    }
  }).detach();

  return bluez.get_adapters()[adapter];
}

void twincentral::ble::Stack::startAdvertisement(
    sdbus::IConnection &dbusConnection) {
  LEAdvertisingManager1Proxy advertiseManager(dbusConnection, "org.bluez",
                                              HCI_BT_ADAPTER_OBJ_PATH);

  GATTManager1Proxy gattManager(dbusConnection, "org.bluez",
                                HCI_BT_ADAPTER_OBJ_PATH);

  // advertiser.registerCallbacks(advertiseManager);

  advertiseManager.RegisterAdvertisement(sdbus::ObjectPath(GATT_ADV_OBJ_PATH));
  gattManager.RegisterApplication(sdbus::ObjectPath(GATT_APP_OBJ_PATH));
}

void twincentral::ble::Stack::stopAdvertisement(
    sdbus::IConnection &dbusConnection) {
  LEAdvertisingManager1Proxy advertiseManager(dbusConnection, "org.bluez",
                                              HCI_BT_ADAPTER_OBJ_PATH);

  GATTManager1Proxy gattManager(dbusConnection, "org.bluez",
                                HCI_BT_ADAPTER_OBJ_PATH);

  advertiseManager.UnregisterAdvertisement(
      sdbus::ObjectPath(GATT_ADV_OBJ_PATH));
  gattManager.UnregisterApplication(sdbus::ObjectPath(GATT_APP_OBJ_PATH));
}

void twincentral::ble::Stack::stopBluez() { asyncBluezActive.store(false); }
