#ifndef TWINCENTRAL_GAIT_DETECTION_FUZZY_LOGIC_MEM_FUNC_INCLUDED
#define TWINCENTRAL_GAIT_DETECTION_FUZZY_LOGIC_MEM_FUNC_INCLUDED

#include "cmath"

#define CAUCHY_B 2.00f
#define EPSILON 0.000001f

namespace twincentral::gait_detection::fuzzy_logic {

  constexpr static auto trapzMemFunc(const float &crisp, const float &start,
                                     const float &end) -> float {
    if (std::fabs((start - end)) < EPSILON) {
      return 0.0f;
    }
    return (crisp - start) / (end - start);
  }

  constexpr static auto cauchyMemFunc(const float &crisp, const float &start,
                                      const float &end) -> float {

    if (std::fabs((start - end)) < EPSILON) {
      return 1.0;
    }

    /*
      mid point defined as point with membershipValue 0.5
      mid point = end - a  -> end - mid_point
    */
    const auto mid_point = start + ((end - start) / (2.0f));
    const auto a = end - mid_point;

    return 1 / (1 + std::pow(std::fabs(((crisp - end) / a)), 2.0f * CAUCHY_B));
  }

  constexpr static auto sigmoidMemFunc(const float &crisp, const float &start,
                                       const float &end) -> float {

    const auto sigmoid_a = 20.0f;
    const auto c = 0.1f;

    return 1 / (1 + exp(-sigmoid_a * (crisp - c)));
  }

  constexpr static auto outSigmoidMemFunc(const float &crisp,
                                          const float &start, const float &end)
      -> float {

    if (crisp > end || std::fabs((start - end)) < EPSILON) {
      return 1.0f;
    }

    auto c = start + ((end - start) / (2.0f));
    auto sigmoid_a = 1.0f;

    if (2.5f > end) {
      c = 0.5;
      sigmoid_a = 5;
    }

    return 1 / (1 + exp(-sigmoid_a * (crisp - c)));
  }
}
#endif