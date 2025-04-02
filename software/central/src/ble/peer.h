#ifndef TWINCENTRAL_BLE_PEER_INCLUDED
#define TWINCENTRAL_BLE_PEER_INCLUDED

#include "config/ble.h"
#include "error/error.h"
#include <cstdint>
#include <future>
#include <mutex>
#include <simplebluez/Bluez.h>
#include <string>
#include <unordered_map>

namespace twincentral::ble {

  /*Needs to be used in shared ptr or in reference only*/

  namespace exception {

    class ConnectionTimeOutException
        : public twincentral::exception::TwincentralBaseException {
      using twincentral::exception::TwincentralBaseException::
          TwincentralBaseException;
    };

    class ConnectionFailedException
        : public twincentral::exception::TwincentralBaseException {
      using twincentral::exception::TwincentralBaseException::
          TwincentralBaseException;
    };

    class CharacteristicReadFailedException
        : public twincentral::exception::TwincentralBaseException {
      using twincentral::exception::TwincentralBaseException::
          TwincentralBaseException;
    };
  }

  void connectPeer(std::shared_ptr<SimpleBluez::Device> device);

  void disconnectPeer(std::shared_ptr<SimpleBluez::Device> device);

  std::string readFromPeer(std::shared_ptr<SimpleBluez::Device> device,
                           std::string svcUUID, std::string chrUUID);

  template <typename TKey, class TVal>
  class PeerContainer {
  public:
    PeerContainer() = default;

    bool exists(TKey key) {
      const std::lock_guard<std::mutex> lock(mtx);
      return items.contains(key);
    }

    void add(TKey key, TVal val) {
      const std::lock_guard<std::mutex> lock(mtx);
      items.insert(std::make_pair(key, val));
    }

    void remove(TKey &key) {
      const std::lock_guard<std::mutex> lock(mtx);
      items.erase(key);
    }

    void clear() {
      const std::lock_guard<std::mutex> lock(mtx);
      items.clear();
    };

    std::size_t len() {
      const std::lock_guard<std::mutex> lock(mtx);
      return items.size();
    }

    auto find(TKey key) { return items.find(key); }

    auto begin() {
      const std::lock_guard<std::mutex> lock(mtx);
      return items.begin();
    }

    auto end() {
      const std::lock_guard<std::mutex> lock(mtx);
      return items.end();
    }

  protected:
    std::unordered_map<TKey, TVal> items;

  private:
    std::mutex mtx;
  };

  class PeerList : public PeerContainer<std::string,
                                        std::shared_ptr<SimpleBluez::Device>> {
  public:
    using PeerContainer<std::string,
                        std::shared_ptr<SimpleBluez::Device>>::PeerContainer;
    bool waitForDiscovery() {
      auto start = std::chrono::high_resolution_clock::now();
      while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        if (len() ==
            twincentral::app::config::bleTwinPeripheralsByName.size()) {
          return true;
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed =
            std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        if (elapsed >= twincentral::app::config::discoverTimeout) {
          return false;
        }
      }
    }

    bool isAllConnected() {
      for (auto keyValue : items) {
        auto device = keyValue.second;
        try {
          if (!device->connected()) {
            return false;
          }
        } catch(std::exception &e) {
          return false;
        }
      }

      return true;
    }

    void disconnect() {
      for (auto keyValue : items) {
        try {
          disconnectPeer(keyValue.second);
        } catch (std::exception &e) {
        };
      }
    }
  };

}
#endif
