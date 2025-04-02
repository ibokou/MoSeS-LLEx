#include <cstddef>

namespace twincrutches::config::app {
#if TWIN_CRUTCH_RIGHT
  static constexpr float scaleCalibrationFactor = -21950;
#elif TWIN_CRUTCH_LEFT
  static constexpr float scaleCalibrationFactor = -22150;
#endif

  static constexpr std::size_t readAvgTimes = 10;
}