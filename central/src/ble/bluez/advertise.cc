#include "advertise.h"
#include "proxy.h"
#include "util.h"
#include <array>

void ControlAdvertiser_adaptor::registerProperties() {
  object_.registerProperty("Type")
      .onInterface(BLUEZ_INTERFACE_NAME)
      .withGetter([]() { return "peripheral"; });

  object_.registerProperty("LocalName")
      .onInterface(BLUEZ_INTERFACE_NAME)
      .withGetter([]() { return "Twincentral"; });
}

void ControlGattService_adaptor::registerProperties() {
  object_.registerProperty("UUID")
      .onInterface(GATT_SVC_INTERFACE_NAME)
      .withGetter([]() { return GATT_SVC_UUID; });

  object_.registerProperty("Primary")
      .onInterface(GATT_SVC_INTERFACE_NAME)
      .withGetter([]() { return true; });
}

void ControlGattCharacteristic_adaptor::registerProperties() {

  object_.registerProperty("Service")
      .onInterface(GATT_CHR_INTERFACE_NAME)
      .withGetter([]() { return sdbus::ObjectPath(GATT_SVC_OBJ_PATH); });

  object_.registerProperty("UUID")
      .onInterface(GATT_CHR_INTERFACE_NAME)
      .withGetter([]() { return GATT_CHR_UUID; });

  object_.registerProperty("Flags")
      .onInterface(GATT_CHR_INTERFACE_NAME)
      .withGetter([]() {
        return std::array<std::string, 2>{"read", "write-without-response"};
      });

  object_.registerProperty("MTU")
      .onInterface(GATT_CHR_INTERFACE_NAME)
      .withGetter([]() { return (uint16_t)65'536; });
}

void ControlGattDescriptor_adaptor::registerProperties() {
  // TODO
}

void ControlAdvertiser::Release() {
  std::cout << "Advertisement has been released" << std::endl;
}

void ControlAdvertiser::registerCallbacks(
    LEAdvertisingManager1Proxy &advertiseManagerProxy) {

  static auto onPropertyChanged = [this](std::string interfaceName,
                                         sdbus::Dict changedProperties) {
    std::cout << "Callback onPropertyChanged called" << std::endl;
    std::cout << interfaceName << std::endl;
    std::cout << changedProperties["Connected"].get<u_int16_t>() << std::endl;
  };

  static auto onInterfaceAdded =
      [this](std::string objectPath, sdbus::DictDict interfacesAndProperties) {
        std::cout << "onInterfaceAdded" << std::endl;
      };

  advertiseManagerProxy.AddSignalHandler(
      "PropertiesChanged", "org.bluez.Device1", onPropertyChanged);

  advertiseManagerProxy.AddSignalHandler("InterfacesAdded", "org.bluez.Device1",
                                         onInterfaceAdded);
}
void ControlAdvertiser::onConnect(std::function<void(sdbus::Signal)> callback) {
  onconnect = callback;
}

void ControlAdvertiser::onDisconnect(
    std::function<void(sdbus::Signal)> callback) {
  ondisconnect = callback;
}

sdbus::ByteArray ControlGattCharacteristic::ReadValue(sdbus::Dict options) {
  auto val = onread();
  return convertStringToByteArray(val);
}

void ControlGattCharacteristic::WriteValue(sdbus::ByteArray value,
                                           sdbus::Dict options) {
  auto stringVal = convertByteArrayToString(value);
  onwrite(stringVal);
}

void ControlGattCharacteristic::StartNotify() {
  std::cout << "Start Notify" << std::endl;
}

void ControlGattCharacteristic::StopNotify() {
  std::cout << "Stop Notify" << std::endl;
}