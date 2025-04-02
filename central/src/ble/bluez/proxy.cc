#include "proxy.h"
#include <functional>
#include <iostream>

void LEAdvertisingManager1_proxy::RegisterAdvertisement(
    sdbus::ObjectPath &&objectPath, sdbus::Dict dict) {
  proxy_.callMethodAsync("RegisterAdvertisement")
      .onInterface(INTERFACE_NAME)
      .withArguments(objectPath, dict)
      .uponReplyInvoke(
          [](const sdbus::Error *error, const sdbus::Variant &value) {
            if (error != nullptr) {
              std::cout << "Register Advertisement failed" << std::endl;
              std::cout << error->getName() << std::endl;
              std::cout << error->getMessage() << std::endl;
            } else {
              std::cout << "Advertisement registered" << std::endl;
            }
          });
}

void LEAdvertisingManager1_proxy::UnregisterAdvertisement(
    sdbus::ObjectPath &&objectPath) {
  proxy_.callMethodAsync("UnregisterAdvertisement")
      .onInterface(INTERFACE_NAME)
      .withArguments(objectPath)
      .uponReplyInvoke(
          [](const sdbus::Error *error, const sdbus::Variant &value) {
            if (error != nullptr) {
              std::cout << "Unregister Advertisement failed" << std::endl;
              std::cout << error->getName() << std::endl;
              std::cout << error->getMessage() << std::endl;
            } else {
              std::cout << "Advertisement unregistered" << std::endl;
            }
          });
}

void GATTManager1_proxy::RegisterApplication(sdbus::ObjectPath &&objectPath,
                                             sdbus::Dict dict) {
  proxy_.callMethodAsync("RegisterApplication")
      .onInterface(INTERFACE_NAME)
      .withArguments(objectPath, dict)
      .uponReplyInvoke(
          [](const sdbus::Error *error, const sdbus::Variant &value) {
            if (error != nullptr) {
              std::cout << "Register Application failed" << std::endl;
              std::cout << error->getName() << std::endl;
              std::cout << error->getMessage() << std::endl;
            } else {
              std::cout << "Application registered" << std::endl;
            }
          });
}

void GATTManager1_proxy::UnregisterApplication(sdbus::ObjectPath &&objectPath) {
  proxy_.callMethodAsync("UnregisterApplication")
      .onInterface(INTERFACE_NAME)
      .withArguments(objectPath)
      .uponReplyInvoke(
          [](const sdbus::Error *error, const sdbus::Variant &value) {
            if (error != nullptr) {
              std::cout << "Unregister Application failed" << std::endl;
              std::cout << error->getName() << std::endl;
              std::cout << error->getMessage() << std::endl;
            } else {
              std::cout << "Application unregistered" << std::endl;
            }
          });
}
