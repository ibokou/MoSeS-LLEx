#ifndef TWINCENTRAL_GAIT_DETECTION_FUZZY_LOGIC_VARS_INCLUDED
#define TWINCENTRAL_GAIT_DETECTION_FUZZY_LOGIC_VARS_INCLUDED
#include "fuzzy/fuzzy.h"

namespace twincentral::gait_detection::fuzzy_logic::vars {

  class FSRPressure : public fuzzy::algebra::FuzzySet<1, float> {
  public:
    FSRPressure IsHigh() const { return *this; }
    FSRPressure IsLow() const {
      const auto complement = ~(*this);
      return (FSRPressure)complement;
    }
  };
}

#endif