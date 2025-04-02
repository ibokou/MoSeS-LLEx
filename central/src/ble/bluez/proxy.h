#ifndef PROXY_H
#define PROXY_H

#include "type_def.h"
#include <sdbus-c++/sdbus-c++.h>
#include <tuple>

class LEAdvertisingManager1_proxy {
  static constexpr const char *INTERFACE_NAME =
      "org.bluez.LEAdvertisingManager1";

protected:
  LEAdvertisingManager1_proxy(sdbus::IProxy &proxy) : proxy_(proxy) {}

  ~LEAdvertisingManager1_proxy() = default;

public:
  void RegisterAdvertisement(sdbus::ObjectPath &&objectPath,
                             sdbus::Dict dict = {});
  void UnregisterAdvertisement(sdbus::ObjectPath &&objectPath);

  template <typename _Function>
  void AddSignalHandler(std::string signalName, std::string interfaceName,
                        _Function &callback) {
    proxy_.uponSignal(signalName).onInterface(interfaceName).call(callback);
    proxy_.finishRegistration();
  }

private:
  sdbus::IProxy &proxy_;
};

class GATTManager1_proxy {
  static constexpr const char *INTERFACE_NAME = "org.bluez.GattManager1";

protected:
  GATTManager1_proxy(sdbus::IProxy &proxy) : proxy_(proxy) {}

  ~GATTManager1_proxy() = default;

public:
  void RegisterApplication(sdbus::ObjectPath &&objectPath,
                           sdbus::Dict dict = {});
  void UnregisterApplication(sdbus::ObjectPath &&objectPath);

private:
  sdbus::IProxy &proxy_;
};

class LEAdvertisingManager1Proxy
    : public sdbus::ProxyInterfaces<LEAdvertisingManager1_proxy> {
public:
  LEAdvertisingManager1Proxy(sdbus::IConnection &connection,
                             std::string destination, std::string path)
      : ProxyInterfaces(connection, std::move(destination), std::move(path)) {
    registerProxy();
  }

  ~LEAdvertisingManager1Proxy() { unregisterProxy(); }
};

class GATTManager1Proxy : public sdbus::ProxyInterfaces<GATTManager1_proxy> {
public:
  GATTManager1Proxy(sdbus::IConnection &connection, std::string destination,
                    std::string path)
      : ProxyInterfaces(connection, std::move(destination), std::move(path)) {
    registerProxy();
  }

  ~GATTManager1Proxy() { unregisterProxy(); }
};

#endif