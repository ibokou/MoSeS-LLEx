#ifndef TWINCENTRAL_ACT_EXPERIMENT_INCLUDED
#define TWINCENTRAL_ACT_EXPERIMENT_INCLUDED

#include "../collect/collector.h"
#include "../io/file.h"
#include "../statemachine/central.h"
#include <memory>

namespace twincentral::act {
  namespace util {
    bool triggerNoraxon(twincentral::ble::PeerList &peerList);

    void accumulateDatapoints(json &accumHolder, const json &data);

    void calculateOffsets(json &offsetsHolder, const json &accumHolder,
                          const std::size_t &numDataPoints);

    const json getNormalizedData(const json &data, const json &offsetsHolder);
  }

  void startExperiment(
      twincentral::statemachine::TwinCentralStateMachine &machine,
      std::shared_ptr<twincentral::io::file::SynchronizedBioMetricsFile>
          resultFile);
}

#endif