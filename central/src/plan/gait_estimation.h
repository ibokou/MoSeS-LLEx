#ifndef TWINCENTRAL_PLAN_GAIT_ESTIMATION_INCLUDED
#define TWINCENTRAL_PLAN_GAIT_ESTIMATION_INCLUDED

#include "../gait_detection/fuzzy_logic/inference.h"
#include "json/json.hpp"
#include <numeric>

namespace {
  using json = nlohmann::json;
}

namespace twincentral::plan {
  class GaitEstimator {
  public:
    template <class TR, class TS>
    GaitEstimator(TR rules, TS system)
        : fuzzyRules(rules), fuzzySystem(system){};
        float fuzzyEstimateGaitPctWithInsoleFSRs(const json fsrDataL,
                                             const json fsrDataR);
        std::pair<float, float> calculateCenterOfPressureWithInsoleFSRs(const json fsrDataL,
                                             const json fsrDataR);

  private:

    std::array<
        const fuzzy::tsukamoto::inference::Rule<
            twincentral::gait_detection::fuzzy_logic::vars::FSRPressure, 6> *,
        6>
        fuzzyRules;
    fuzzy::tsukamoto::inference::UnaryFuzzyInferenceSystem<
        twincentral::gait_detection::fuzzy_logic::vars::FSRPressure, 10000, 6,
        6>
        fuzzySystem;
  };
}

#endif