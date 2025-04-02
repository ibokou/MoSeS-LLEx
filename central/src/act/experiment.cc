#include "experiment.h"
#include <iostream>

void twincentral::act::startExperiment(
    twincentral::statemachine::TwinCentralStateMachine &machine,
    std::shared_ptr<twincentral::io::file::SynchronizedBioMetricsFile>
        resultFile) {
  std::thread([&machine, resultFile]() {
    twincentral::collect::DataCollector collector(machine.peerList);
    try {
      auto start = std::chrono::steady_clock::now();
      std::chrono::time_point<std::chrono::steady_clock> experimentStartTime;
      json dataAccum;
      json offset;
      std::size_t numDataPoints = 0;
      bool noraxonHasBeenTriggered = false;
      machine.blinkStickLed.notifyIsRunningOffsetting();
      while (!(machine.isRunExecutionSuspended())) {
        const auto data =
            collector.collect(twincentral::app::config::dataCollectionTimeout);

        auto current = std::chrono::steady_clock::now();
        auto delta = std::chrono::duration_cast<std::chrono::milliseconds>(
            current - start);

        if (data.empty()) { // If empty that means that peripheral sent garbage
          continue;
        }

        if (delta < twincentral::app::config::offsettingDuration) {
          twincentral::act::util::accumulateDatapoints(dataAccum, data);
          ++numDataPoints;
          continue;
        } else {
          if (!noraxonHasBeenTriggered) {
            twincentral::act::util::calculateOffsets(offset, dataAccum,
                                                     numDataPoints);
            try {
              auto res =
                  twincentral::act::util::triggerNoraxon(machine.peerList);
              if (res) {
                noraxonHasBeenTriggered = res;
                machine.blinkStickLed.notifyHasTriggeredNoraxon();
                std::this_thread::sleep_for(std::chrono::milliseconds(
                    twincentral::app::config::
                        noraxonTriggerSuccessNotificationDuration));
                experimentStartTime = std::chrono::steady_clock::now();
              }
            } catch (std::exception &e) {
            };
          } else {
            machine.blinkStickLed.notifyIsRunningCalc();
          }
        }

        const auto normData =
            twincentral::act::util::getNormalizedData(data, offset);

        std::thread([&machine, normData, resultFile, experimentStartTime]() {
          auto gait = machine.gaitEstimator.fuzzyEstimateGaitPctWithInsoleFSRs(
              normData[twincentral::app::config::twinInsoleLeftBleName],
              normData[twincentral::app::config::twinInsoleRightBleName]);
          auto cop =
              machine.gaitEstimator.calculateCenterOfPressureWithInsoleFSRs(
                  normData[twincentral::app::config::twinInsoleLeftBleName],
                  normData[twincentral::app::config::twinInsoleRightBleName]);
          auto resultObject = normData;

          resultObject["GaitPct"] = gait;
          resultObject["COP"]["x"] = std::get<0>(cop);
          resultObject["COP"]["y"] = std::get<1>(cop);

          resultFile->write(resultObject, experimentStartTime);
        }).detach();
      }
    } catch (std::exception &e) {
      printf("[%s] %s: %s\n", "DataCollector", "Collection failed, reason",
             e.what());
      machine.hasFailedSetup();
    }
  }).detach();
}

bool twincentral::act::util::triggerNoraxon(
    twincentral::ble::PeerList &peerList) {
  if (auto elem =
          peerList.find(twincentral::app::config::noraxonTriggerBleName);
      elem != peerList.end()) {
    auto noraxonTrigger = elem->second;

    auto entry =
        twincentral::app::config::bleTwinPeripheralsByName.find(elem->first);

    auto svcUUID = std::get<0>(entry->second);
    auto chrUUID = std::get<1>(entry->second);

    twincentral::ble::readFromPeer(noraxonTrigger, svcUUID, chrUUID);

    return true;
  }

  return false;
}

void twincentral::act::util::accumulateDatapoints(json &accumHolder,
                                                  const json &data) {
  for (auto &name : twincentral::app::config::bleCollectableTwinPeripherals) {

    if (!accumHolder.contains(name)) {
      accumHolder[name][twincentral::app::config::eulerAnglesJsonKey]["x"] = 0.0f;
      accumHolder[name][twincentral::app::config::eulerAnglesJsonKey]["y"] = 0.0f;
      accumHolder[name][twincentral::app::config::eulerAnglesJsonKey]["z"] = 0.0f;

      accumHolder[name][twincentral::app::config::quaternionsJsonKey]["w"] = 0.0f;
      accumHolder[name][twincentral::app::config::quaternionsJsonKey]["x"] = 0.0f;
      accumHolder[name][twincentral::app::config::quaternionsJsonKey]["y"] = 0.0f;
      accumHolder[name][twincentral::app::config::quaternionsJsonKey]["z"] = 0.0f;
    }

    accumHolder[name][twincentral::app::config::eulerAnglesJsonKey]["x"] =
        static_cast<double>(accumHolder[name][twincentral::app::config::eulerAnglesJsonKey]["x"]) +
        static_cast<double>(data[name][twincentral::app::config::eulerAnglesJsonKey]["x"]);
    accumHolder[name][twincentral::app::config::eulerAnglesJsonKey]["y"] =
        static_cast<double>(accumHolder[name][twincentral::app::config::eulerAnglesJsonKey]["y"]) +
        static_cast<double>(data[name][twincentral::app::config::eulerAnglesJsonKey]["y"]);

    accumHolder[name][twincentral::app::config::eulerAnglesJsonKey]["z"] =
        static_cast<double>(accumHolder[name][twincentral::app::config::eulerAnglesJsonKey]["z"]) +
        static_cast<double>(data[name][twincentral::app::config::eulerAnglesJsonKey]["z"]);

    accumHolder[name][twincentral::app::config::quaternionsJsonKey]["w"] =
        static_cast<double>(accumHolder[name][twincentral::app::config::quaternionsJsonKey]["w"]) +
        static_cast<double>(data[name][twincentral::app::config::quaternionsJsonKey]["w"]);

    accumHolder[name][twincentral::app::config::quaternionsJsonKey]["x"] =
        static_cast<double>(accumHolder[name][twincentral::app::config::quaternionsJsonKey]["x"]) +
        static_cast<double>(data[name][twincentral::app::config::quaternionsJsonKey]["x"]);

    accumHolder[name][twincentral::app::config::quaternionsJsonKey]["y"] =
        static_cast<double>(accumHolder[name][twincentral::app::config::quaternionsJsonKey]["y"]) +
        static_cast<double>(data[name][twincentral::app::config::quaternionsJsonKey]["y"]);

    accumHolder[name][twincentral::app::config::quaternionsJsonKey]["z"] =
        static_cast<double>(accumHolder[name][twincentral::app::config::quaternionsJsonKey]["z"]) +
        static_cast<double>(data[name][twincentral::app::config::quaternionsJsonKey]["z"]);
  }
}

void twincentral::act::util::calculateOffsets(
    json &offsetsHolder, const json &accumHolder,
    const std::size_t &numDataPoints) {
  for (auto &name : twincentral::app::config::bleCollectableTwinPeripherals) {
    offsetsHolder[name][twincentral::app::config::eulerAnglesJsonKey]["x"] =
        static_cast<double>(accumHolder[name][twincentral::app::config::eulerAnglesJsonKey]["x"]) /
        static_cast<double>(numDataPoints);

    offsetsHolder[name][twincentral::app::config::eulerAnglesJsonKey]["y"] =
        static_cast<double>(accumHolder[name][twincentral::app::config::eulerAnglesJsonKey]["y"]) /
        static_cast<double>(numDataPoints);

    offsetsHolder[name][twincentral::app::config::eulerAnglesJsonKey]["z"] =
        static_cast<double>(accumHolder[name][twincentral::app::config::eulerAnglesJsonKey]["z"]) /
        static_cast<double>(numDataPoints);

    offsetsHolder[name][twincentral::app::config::quaternionsJsonKey]["w"] =
        static_cast<double>(accumHolder[name][twincentral::app::config::quaternionsJsonKey]["w"]) /
        static_cast<double>(numDataPoints);

    offsetsHolder[name][twincentral::app::config::quaternionsJsonKey]["x"] =
        static_cast<double>(accumHolder[name][twincentral::app::config::quaternionsJsonKey]["x"]) /
        static_cast<double>(numDataPoints);

    offsetsHolder[name][twincentral::app::config::quaternionsJsonKey]["y"] =
        static_cast<double>(accumHolder[name][twincentral::app::config::quaternionsJsonKey]["y"]) /
        static_cast<double>(numDataPoints);

    offsetsHolder[name][twincentral::app::config::quaternionsJsonKey]["z"] =
        static_cast<double>(accumHolder[name][twincentral::app::config::quaternionsJsonKey]["z"]) /
        static_cast<double>(numDataPoints);
  }
}

const json
twincentral::act::util::getNormalizedData(const json &data,
                                          const json &offsetsHolder) {
  json normData = data;
  for (auto &name : twincentral::app::config::bleCollectableTwinPeripherals) {
    normData[name][twincentral::app::config::eulerAnglesJsonKey]["x"] =
        static_cast<double>(data[name][twincentral::app::config::eulerAnglesJsonKey]["x"]) -
        static_cast<double>(offsetsHolder[name][twincentral::app::config::eulerAnglesJsonKey]["x"]);

    normData[name][twincentral::app::config::eulerAnglesJsonKey]["y"] =
        static_cast<double>(data[name][twincentral::app::config::eulerAnglesJsonKey]["y"]) -
        static_cast<double>(offsetsHolder[name][twincentral::app::config::eulerAnglesJsonKey]["y"]);

    normData[name][twincentral::app::config::eulerAnglesJsonKey]["z"] =
        static_cast<double>(data[name][twincentral::app::config::eulerAnglesJsonKey]["z"]) -
        static_cast<double>(offsetsHolder[name][twincentral::app::config::eulerAnglesJsonKey]["z"]);

    normData[name][twincentral::app::config::quaternionsJsonKey]["w"] =
        static_cast<double>(data[name][twincentral::app::config::quaternionsJsonKey]["w"]) -
        static_cast<double>(offsetsHolder[name][twincentral::app::config::quaternionsJsonKey]["w"]);

    normData[name][twincentral::app::config::quaternionsJsonKey]["x"] =
        static_cast<double>(data[name][twincentral::app::config::quaternionsJsonKey]["x"]) -
        static_cast<double>(offsetsHolder[name][twincentral::app::config::quaternionsJsonKey]["x"]);

    normData[name][twincentral::app::config::quaternionsJsonKey]["y"] =
        static_cast<double>(data[name][twincentral::app::config::quaternionsJsonKey]["y"]) -
        static_cast<double>(offsetsHolder[name][twincentral::app::config::quaternionsJsonKey]["y"]);

    normData[name][twincentral::app::config::quaternionsJsonKey]["z"] =
        static_cast<double>(data[name][twincentral::app::config::quaternionsJsonKey]["z"]) -
        static_cast<double>(offsetsHolder[name][twincentral::app::config::quaternionsJsonKey]["z"]);
  }

  return normData;
}