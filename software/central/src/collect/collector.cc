#include "collector.h"

const json twincentral::collect::DataCollector::collect(
    const std::chrono::milliseconds &timeout) {

  json resultObject;
  std::vector<std::thread> threads;
  std::vector<std::future<std::tuple<std::string, std::string>>> futures;

  std::for_each(
      peerList.begin(), peerList.end(),
      [this, &threads, &futures, &timeout](auto &keyValue) mutable {
        // Noraxon is not read
        if (keyValue.second->name().compare(
                twincentral::app::config::noraxonTriggerBleName) == 0) {
        } else {
          auto taskF =
              std::bind(&twincentral::collect::DataCollector::collectFromPeer,
                        this, std::placeholders::_1, std::placeholders::_2);
          std::packaged_task<std::tuple<std::string, std::string>(
              std::shared_ptr<SimpleBluez::Device> device,
              const std::chrono::milliseconds &timeout)>
              task(taskF);
          auto future = task.get_future();
          std::thread thd(std::move(task), keyValue.second, timeout);
          threads.push_back(std::move(thd));
          futures.push_back(std::move(future));
        }
      });

  for (auto &thd : threads) {
    thd.join();
  }

  for (auto &future : futures) {
    auto resTuple = future.get();
    auto deviceName = std::get<0>(resTuple);
    auto sData = std::get<1>(resTuple);
    if (sData.empty()) {
      throw twincentral::collect::exception::DataCollectionTimeOutException(
          "Received no value from read operation");
    } else {
      try {
        json jData = json::parse(sData);
        resultObject[deviceName] = jData;
      } catch (std::exception &e) {
        printf("[%s] Peripheral %s sent garbage data\n", "DataCollector",
               deviceName.c_str());
        return json{};
      }
    }
  }
  return resultObject;
}

std::tuple<std::string, std::string>
twincentral::collect::DataCollector::collectFromPeer(
    std::shared_ptr<SimpleBluez::Device> peer,
    const std::chrono::milliseconds &timeout) {
  std::string msg;
  if (auto elem =
          twincentral::app::config::bleTwinPeripheralsByName.find(peer->name());
      elem != twincentral::app::config::bleTwinPeripheralsByName.end()) {
    auto svcUUID = std::get<0>(elem->second);
    auto chrUUID = std::get<1>(elem->second);

    auto start = std::chrono::high_resolution_clock::now();

    while (true) {
      try {
        msg = twincentral::ble::readFromPeer(peer, svcUUID, chrUUID);
        break;
      } catch (std::exception &e) {
      }

      auto end = std::chrono::high_resolution_clock::now();

      auto elapsed =
          std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

      if (elapsed >= timeout) {
        break;
      }
    }
  }
  return std::make_tuple(peer->name(), msg);
}