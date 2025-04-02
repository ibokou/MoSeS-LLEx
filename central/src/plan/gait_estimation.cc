#include "gait_estimation.h"

float twincentral::plan::GaitEstimator::fuzzyEstimateGaitPctWithInsoleFSRs(
    const json fsrDataL, const json fsrDataR) {
  /*
  std::array<float, 6> fsrValues = {
      fsrDataL["Heel"],          fsrDataR["Heel"],
      fsrDataR["5thmetatarsal"], fsrDataR["5thmetatarsal"],
      fsrDataR["1thmetatarsal"], fsrDataR["1thmetatarsal"]};

  std::array<fuzzy::algebra::CrispTuple<float>, 6> crispValues = {};
  const auto e = 100;
  auto sum = std::accumulate(fsrValues.begin(), fsrValues.end(), 0.0f) + e;

  // Normalize array
  std::transform(
      fsrValues.begin(), fsrValues.end(), crispValues.begin(),
      [sum](float entry) { return fuzzy::algebra::CrispTuple{entry / sum}; });

  auto res = fuzzySystem.fuzzify(crispValues)
                 ->infer(fuzzyRules)
                 ->defuzzify(fuzzy::tsukamoto::inference::CenterofGravity);
  */
  return 0.0f;
}

std::pair<float, float>
twincentral::plan::GaitEstimator::calculateCenterOfPressureWithInsoleFSRs(
    const json fsrDataL, const json fsrDataR) {
  /*
  std::array<float, 6> fsrValues = {
      fsrDataL["Heel"],          fsrDataR["Heel"],
      fsrDataR["5thmetatarsal"], fsrDataR["5thmetatarsal"],
      fsrDataR["1thmetatarsal"], fsrDataR["1thmetatarsal"]};
  */
  return std::make_pair(0.0f, 0.0f);
}