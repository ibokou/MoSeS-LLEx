#ifndef TWINCENTRAL_GAIT_DETECTION_FUZZY_LOGIC_RULES_INCLUDED
#define TWINCENTRAL_GAIT_DETECTION_FUZZY_LOGIC_RULES_INCLUDED
#include "memFunc.h"
#include "vars.h"

enum FSRPlacement {
  Heel_L,
  Heel_R,
  FifthMetatarsal_L,
  FifthMetatarsal_R,
  FirstMetatarsal_L,
  FirstMetatarsal_R,
};

namespace twincentral::gait_detection::fuzzy_logic::rules {

  class HeelStrike
      : public fuzzy::tsukamoto::inference::Rule<
            twincentral::gait_detection::fuzzy_logic::vars::FSRPressure, 6> {
    using Rule<twincentral::gait_detection::fuzzy_logic::vars::FSRPressure,
               6>::Rule;
    fuzzy::tsukamoto::inference::RuleOutputTuple
    fire(std::array<twincentral::gait_detection::fuzzy_logic::vars::FSRPressure,
                    6>
             sets) const override {
      auto resultSet = fuzzy::tsukamoto::inference::AND(
          sets[Heel_L].IsHigh(), sets[FifthMetatarsal_L].IsLow(),
          sets[FirstMetatarsal_L].IsLow());
      return lookUpTable[resultSet[0].membershipValue];
    }
  };

  class EarlyStance
      : public fuzzy::tsukamoto::inference::Rule<
            twincentral::gait_detection::fuzzy_logic::vars::FSRPressure, 6> {
    using Rule<twincentral::gait_detection::fuzzy_logic::vars::FSRPressure,
               6>::Rule;
    fuzzy::tsukamoto::inference::RuleOutputTuple
    fire(std::array<twincentral::gait_detection::fuzzy_logic::vars::FSRPressure,
                    6>
             sets) const override {
      auto resultSet = fuzzy::tsukamoto::inference::AND(
          sets[Heel_L].IsHigh(), sets[FifthMetatarsal_L].IsHigh(),
          sets[FirstMetatarsal_L].IsLow());
      return lookUpTable[resultSet[0].membershipValue];
    }
  };

  class MidStance
      : public fuzzy::tsukamoto::inference::Rule<
            twincentral::gait_detection::fuzzy_logic::vars::FSRPressure, 6> {
    using Rule<twincentral::gait_detection::fuzzy_logic::vars::FSRPressure,
               6>::Rule;
    fuzzy::tsukamoto::inference::RuleOutputTuple
    fire(std::array<twincentral::gait_detection::fuzzy_logic::vars::FSRPressure,
                    6>
             sets) const override {
      auto resultSet = fuzzy::tsukamoto::inference::AND(
          sets[Heel_L].IsHigh(), sets[FifthMetatarsal_L].IsHigh(),
          sets[FirstMetatarsal_L].IsHigh());
      return lookUpTable[resultSet[0].membershipValue];
    }
  };

  class TerminalStance
      : public fuzzy::tsukamoto::inference::Rule<
            twincentral::gait_detection::fuzzy_logic::vars::FSRPressure, 6> {
    using Rule<twincentral::gait_detection::fuzzy_logic::vars::FSRPressure,
               6>::Rule;
    fuzzy::tsukamoto::inference::RuleOutputTuple
    fire(std::array<twincentral::gait_detection::fuzzy_logic::vars::FSRPressure,
                    6>
             sets) const override {
      auto resultSet = fuzzy::tsukamoto::inference::AND(
          sets[Heel_L].IsLow(), sets[FifthMetatarsal_L].IsHigh(),
          sets[FirstMetatarsal_L].IsHigh());
      return lookUpTable[resultSet[0].membershipValue];
    }
  };

  class PreSwing
      : public fuzzy::tsukamoto::inference::Rule<
            twincentral::gait_detection::fuzzy_logic::vars::FSRPressure, 6> {
    using Rule<twincentral::gait_detection::fuzzy_logic::vars::FSRPressure,
               6>::Rule;
    fuzzy::tsukamoto::inference::RuleOutputTuple
    fire(std::array<twincentral::gait_detection::fuzzy_logic::vars::FSRPressure,
                    6>
             sets) const override {
      auto resultSet = fuzzy::tsukamoto::inference::AND(
          sets[Heel_L].IsLow(), sets[FifthMetatarsal_L].IsLow(),
          sets[FirstMetatarsal_L].IsHigh());
      return lookUpTable[resultSet[0].membershipValue];
    }
  };

  class Swing
      : public fuzzy::tsukamoto::inference::Rule<
            twincentral::gait_detection::fuzzy_logic::vars::FSRPressure, 6> {
    using Rule<twincentral::gait_detection::fuzzy_logic::vars::FSRPressure,
               6>::Rule;
    fuzzy::tsukamoto::inference::RuleOutputTuple
    fire(std::array<twincentral::gait_detection::fuzzy_logic::vars::FSRPressure,
                    6>
             sets) const override {
      auto resultSet = fuzzy::tsukamoto::inference::AND(
          sets[Heel_L].IsLow(), sets[FifthMetatarsal_L].IsLow(),
          sets[FirstMetatarsal_L].IsLow());
      return lookUpTable[resultSet[0].membershipValue];
    }
  };
}
#endif