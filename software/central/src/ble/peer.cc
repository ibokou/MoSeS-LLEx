#include "peer.h"
#include <simplebluez/Exceptions.h>

void twincentral::ble::connectPeer(
    std::shared_ptr<SimpleBluez::Device> device) {
  auto start = std::chrono::high_resolution_clock::now();
  while (true) {
    try {
      printf("[%s] %s [%s]%s \n", "BLEStack",
             "Attempt connection to peripheral :", device->name().c_str(),
             device->address().c_str());
      device->connect();

      std::this_thread::sleep_for(std::chrono::seconds(2));

      if (!device->connected() || !device->services_resolved()) {
        printf("[%s] %s [%s]%s %s\n", "BLEStack",
               "Connection to peripheral :", device->name().c_str(),
               device->address().c_str(), " failed");
        throw twincentral::ble::exception::ConnectionFailedException();
      }

      printf("[%s] %s [%s]%s\n", "BLEStack",
             "Twincentral has connected to peripheral: ",
             device->name().c_str(), device->address().c_str());
      break;
    } catch (SimpleDBus::Exception::SendFailed &e) {
      printf("[%s] %s : %s \n", "BLEStack", "Attempt failed", e.what());
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    auto end = std::chrono::high_resolution_clock::now();

    auto elapsed =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    if (elapsed >= twincentral::app::config::connectTimeout) {
      throw twincentral::ble::exception::ConnectionTimeOutException();
    }
  }
}

void twincentral::ble::disconnectPeer(
    std::shared_ptr<SimpleBluez::Device> device) {
  if (device->connected()) {
    device->disconnect();
  }
}

std::string
twincentral::ble::readFromPeer(std::shared_ptr<SimpleBluez::Device> device,
                               std::string svcUUID, std::string chrUUID) {
  if (device->connected()) {
    for (auto service : device->services()) {
      if (service->uuid().compare(svcUUID) == 0) {
        for (auto characteristic : service->characteristics()) {
          if (characteristic->uuid().compare(chrUUID) == 0) {
            return characteristic->read();
          }
        }
      }
    }
  } else {
    throw twincentral::ble::exception::ConnectionTimeOutException();
  }

  throw twincentral::ble::exception::CharacteristicReadFailedException();
}