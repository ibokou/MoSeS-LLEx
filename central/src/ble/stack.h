#ifndef TWINCENTRAL_BLE_STACK_INCLUDED
#define TWINCENTRAL_BLE_STACK_INCLUDED

// clang-format on
#include <atomic>
#include <future>
#include <optional>
#include <sdbus-c++/sdbus-c++.h>
#include <simplebluez/Bluez.h>

namespace twincentral::ble {

  class Stack {
  public:
    Stack() : asyncBluezActive(true){};
    void setupAdvertisement(sdbus::IConnection &dbusConnection,
                            std::function<std::string(void)> onread,
                            std::function<void(std::string)> onwrite);
    std::shared_ptr<SimpleBluez::Adapter> startBluez(std::size_t adapter = 0);
    void startAdvertisement(sdbus::IConnection &dbusConnection);
    void stopAdvertisement(sdbus::IConnection &dbusConnection);
    void stopBluez();

  private:
    SimpleBluez::Bluez bluez;
    std::atomic_bool asyncBluezActive;
  };
}
#endif