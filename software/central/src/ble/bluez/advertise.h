#include "constants.h"
#include "proxy.h"
#include "type_def.h"
#include <iostream>
#include <sdbus-c++/sdbus-c++.h>

class ControlAdvertiser_adaptor {
protected:
  ControlAdvertiser_adaptor(sdbus::IObject &object) : object_(object) {
    object_.registerMethod("Release")
        .onInterface(BLUEZ_INTERFACE_NAME)
        .implementedAs([this]() { return this->Release(); })
        .withNoReply();

    registerProperties();
  }

  ControlAdvertiser_adaptor(const ControlAdvertiser_adaptor &) = delete;
  ControlAdvertiser_adaptor &
  operator=(const ControlAdvertiser_adaptor &) = delete;
  ControlAdvertiser_adaptor(ControlAdvertiser_adaptor &&) = default;
  ControlAdvertiser_adaptor &operator=(ControlAdvertiser_adaptor &&) = default;

  ~ControlAdvertiser_adaptor() = default;

private:
  sdbus::IObject &object_;

private:
  void registerProperties();
  virtual void Release() = 0;
};

class ControlGattService_adaptor {
protected:
  ControlGattService_adaptor(sdbus::IObject &object) : object_(object) {
    registerProperties();
  }

  ControlGattService_adaptor(const ControlGattService_adaptor &) = delete;
  ControlGattService_adaptor &
  operator=(const ControlGattService_adaptor &) = delete;
  ControlGattService_adaptor(ControlGattService_adaptor &&) = default;
  ControlGattService_adaptor &
  operator=(ControlGattService_adaptor &&) = default;

  ~ControlGattService_adaptor() = default;

private:
  sdbus::IObject &object_;

private:
  void registerProperties();
};

class ControlGattCharacteristic_adaptor {
protected:
  ControlGattCharacteristic_adaptor(sdbus::IObject &object) : object_(object) {

    object_.registerMethod("ReadValue")
        .onInterface(GATT_CHR_INTERFACE_NAME)
        .withInputParamNames("options")
        .withOutputParamNames("value")
        .implementedAs([this](const sdbus::Dict options) {
          return this->ReadValue(options);
        });

    object_.registerMethod("WriteValue")
        .onInterface(GATT_CHR_INTERFACE_NAME)
        .withInputParamNames("value", "options")
        .implementedAs(
            [this](const sdbus::ByteArray value, const sdbus::Dict options) {
              return this->WriteValue(value, options);
            });

    object_.registerMethod("StartNotify")
        .onInterface(GATT_CHR_INTERFACE_NAME)
        .implementedAs([this]() { return this->StartNotify(); });
    object_.registerMethod("StopNotify")
        .onInterface(GATT_CHR_INTERFACE_NAME)
        .implementedAs([this]() { return this->StopNotify(); });

    registerProperties();
  }

  ControlGattCharacteristic_adaptor(const ControlGattCharacteristic_adaptor &) =
      delete;
  ControlGattCharacteristic_adaptor &
  operator=(const ControlGattCharacteristic_adaptor &) = delete;
  ControlGattCharacteristic_adaptor(ControlGattCharacteristic_adaptor &&) =
      default;
  ControlGattCharacteristic_adaptor &
  operator=(ControlGattCharacteristic_adaptor &&) = default;

  ~ControlGattCharacteristic_adaptor() = default;

private:
  sdbus::IObject &object_;

private:
  virtual sdbus::ByteArray ReadValue(const sdbus::Dict dict) = 0;
  virtual void WriteValue(const sdbus::ByteArray value,
                          const sdbus::Dict options) = 0;
  virtual void StartNotify() = 0;
  virtual void StopNotify() = 0;
  void registerProperties();
};

class ControlGattDescriptor_adaptor {
protected:
  ControlGattDescriptor_adaptor(sdbus::IObject &object) : object_(object) {
    registerProperties();
  }

  ControlGattDescriptor_adaptor(const ControlGattDescriptor_adaptor &) = delete;
  ControlGattDescriptor_adaptor &
  operator=(const ControlGattDescriptor_adaptor &) = delete;
  ControlGattDescriptor_adaptor(ControlGattDescriptor_adaptor &&) = default;
  ControlGattDescriptor_adaptor &
  operator=(ControlGattDescriptor_adaptor &&) = default;

  ~ControlGattDescriptor_adaptor() = default;

private:
  sdbus::IObject &object_;

private:
  void registerProperties();
};

// Adaptor wrapper

class GATTApplication
    : public sdbus::AdaptorInterfaces<sdbus::ObjectManager_adaptor> {
public:
  GATTApplication(sdbus::IConnection &connection, std::string path)
      : AdaptorInterfaces(connection, std::move(path)) {
    registerAdaptor();
  }

  ~GATTApplication() { unregisterAdaptor(); }
};

class ControlAdvertiser
    : public sdbus::AdaptorInterfaces<ControlAdvertiser_adaptor,
                                      sdbus::Properties_adaptor> {
public:
  ControlAdvertiser(sdbus::IConnection &connection, std::string objectPath)
      : AdaptorInterfaces(connection, std::move(objectPath)) {
    registerAdaptor();
  }

  ~ControlAdvertiser() { unregisterAdaptor(); }

  void Release() override;

  void registerCallbacks(LEAdvertisingManager1Proxy &advertiseManagerProxy);
  void onConnect(std::function<void(sdbus::Signal)> callback);
  void onDisconnect(std::function<void(sdbus::Signal)> callback);

private:
  std::function<void(sdbus::Signal)> onconnect;
  std::function<void(sdbus::Signal)> ondisconnect;
};

class ControlGattService
    : public sdbus::AdaptorInterfaces<ControlGattService_adaptor,
                                      sdbus::Properties_adaptor> {
public:
  ControlGattService(sdbus::IConnection &connection, std::string objectPath)
      : AdaptorInterfaces(connection, std::move(objectPath)) {
    registerAdaptor();
  }

  ~ControlGattService() { unregisterAdaptor(); }
};

class ControlGattCharacteristic
    : public sdbus::AdaptorInterfaces<ControlGattCharacteristic_adaptor,
                                      sdbus::Properties_adaptor> {
public:
  ControlGattCharacteristic(sdbus::IConnection &connection,
                            std::string objectPath,
                            std::function<std::string(void)> onread,
                            std::function<void(std::string)> onwrite)
      : AdaptorInterfaces(connection, std::move(objectPath)), onread(onread),
        onwrite(onwrite) {
    registerAdaptor();
  }

  ~ControlGattCharacteristic() { unregisterAdaptor(); }

public:
  sdbus::ByteArray ReadValue(const sdbus::Dict dict) override;
  void WriteValue(const sdbus::ByteArray value,
                  const sdbus::Dict options) override;
  void StartNotify() override;
  void StopNotify() override;

private:
  std::function<std::string(void)> onread;
  std::function<void(std::string)> onwrite;
};

class ControlGattDescriptor
    : public sdbus::AdaptorInterfaces<ControlGattDescriptor_adaptor,
                                      sdbus::Properties_adaptor> {
public:
  ControlGattDescriptor(sdbus::IConnection &connection, std::string objectPath)
      : AdaptorInterfaces(connection, std::move(objectPath)) {
    registerAdaptor();
  }

  ~ControlGattDescriptor() { unregisterAdaptor(); }
};