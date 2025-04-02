#ifndef TWINCENTRAL_GAIT_DETECTION_FUZZY_LOGIC_INFERENCE_INCLUDED
#define TWINCENTRAL_GAIT_DETECTION_FUZZY_LOGIC_INFERENCE_INCLUDED

#include "memFunc.h"
#include "rules.h"
#include "vars.h"
#include <vector>

namespace twincentral::gait_detection::fuzzy_logic::inference {
  const static auto createRules() {
    constexpr static auto heelStrikeRule =
        twincentral::gait_detection::fuzzy_logic::rules::HeelStrike(
            fuzzy::tsukamoto::inference::MaxMinInference, 0.0f, 2.0f,
            &twincentral::gait_detection::fuzzy_logic::outSigmoidMemFunc);

    constexpr static auto earlyStanceRule =
        twincentral::gait_detection::fuzzy_logic::rules::EarlyStance(
            fuzzy::tsukamoto::inference::MaxMinInference, 2.0f, 12.0f,
            &twincentral::gait_detection::fuzzy_logic::outSigmoidMemFunc);

    constexpr static auto midStanceRule =
        twincentral::gait_detection::fuzzy_logic::rules::MidStance(
            fuzzy::tsukamoto::inference::MaxMinInference, 12.0f, 31.0f,
            &twincentral::gait_detection::fuzzy_logic::outSigmoidMemFunc);

    constexpr static auto terminalStanceRule =
        twincentral::gait_detection::fuzzy_logic::rules::TerminalStance(
            fuzzy::tsukamoto::inference::MaxMinInference, 31.0f, 50.0f,
            &twincentral::gait_detection::fuzzy_logic::outSigmoidMemFunc);

    constexpr static auto preSwingRule =
        twincentral::gait_detection::fuzzy_logic::rules::PreSwing(
            fuzzy::tsukamoto::inference::MaxMinInference, 50.0f, 62.0f,
            &twincentral::gait_detection::fuzzy_logic::outSigmoidMemFunc);

    constexpr static auto swingRule =
        twincentral::gait_detection::fuzzy_logic::rules::Swing(
            fuzzy::tsukamoto::inference::MaxMinInference, 62.0f, 100.0f,
            &twincentral::gait_detection::fuzzy_logic::outSigmoidMemFunc);

    return std::array<
        const fuzzy::tsukamoto::inference::Rule<
            twincentral::gait_detection::fuzzy_logic::vars::FSRPressure, 6> *,
        6>{&heelStrikeRule,     &earlyStanceRule, &midStanceRule,
           &terminalStanceRule, &preSwingRule,    &swingRule};
  }

  const static auto createSystem() {
    constexpr static auto system =
        fuzzy::tsukamoto::inference::UnaryFuzzyInferenceSystem<
            twincentral::gait_detection::fuzzy_logic::vars::FSRPressure, 10000,
            6, 6>(fuzzy::algebra::CrispTuple{0.0f},
                  fuzzy::algebra::CrispTuple{1.0f},
                  fuzzy::algebra::CrispTuple{0.001f}, &sigmoidMemFunc);
    return system;
  }
}

#endif