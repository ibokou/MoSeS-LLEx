#ifndef TWINCENTRAL_COLLECT_COLLECTOR_INCLUDED
#define TWINCENTRAL_COLLECT_COLLECTOR_INCLUDED

#include "../ble/peer.h"
#include "error/error.h"
#include "json/json.hpp"
#include <atomic>
#include <future>
#include <optional>
#include <string>
#include <thread>

namespace {
  using json = nlohmann::json;
}

namespace twincentral::collect {

  namespace exception {
    class DataCollectionTimeOutException
        : public twincentral::exception::TwincentralBaseException {
      using twincentral::exception::TwincentralBaseException::
          TwincentralBaseException;
    };

    class DataCollectionFailedException
        : public twincentral::exception::TwincentralBaseException {
      using twincentral::exception::TwincentralBaseException::
          TwincentralBaseException;
    };
  }

  struct CollectionSet {};
  class DataCollector {
  public:
    DataCollector(twincentral::ble::PeerList &peerList) : peerList(peerList){};
    const json collect(const std::chrono::milliseconds &timeout);

  private:
    std::tuple<std::string, std::string>
    collectFromPeer(std::shared_ptr<SimpleBluez::Device> peer,
                    const std::chrono::milliseconds &timeout);
    twincentral::ble::PeerList &peerList;
  };
}

#endif