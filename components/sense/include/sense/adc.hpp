#ifndef TWINPERIPHERAL_SENSE_ADC_INCLUDED
#define TWINPERIPHERAL_SENSE_ADC_INCLUDED

// clang-format off

#include "esp_adc/adc_oneshot.h"
#include "hx711.h"

// clang-format on
#include "driver/gpio.h"
#include <array>
#include <type_traits>
#include <vector>

namespace twinperipheral::sense {
  template <typename... Channels>
  class TwinADC {
  public:
#if TWIN_CRUTCH_LEFT || TWIN_CRUTCH_RIGHT
    TwinADC() = default;
#else
    TwinADC(Channels... adcChannels) : channels{{adcChannels...}} {
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wmissing-field-initializers"
      adc_oneshot_unit_init_cfg_t initConfig1{
          .unit_id = ADC_UNIT_1,
          .ulp_mode = ADC_ULP_MODE_DISABLE,
      };
      adc_oneshot_unit_init_cfg_t initConfig2{
          .unit_id = ADC_UNIT_2,
          .ulp_mode = ADC_ULP_MODE_DISABLE,
      };
#  pragma GCC diagnostic pop
      ESP_ERROR_CHECK(adc_oneshot_new_unit(&initConfig1, &adc1Handle));
      ESP_ERROR_CHECK(adc_oneshot_new_unit(&initConfig2, &adc2Handle));
    };
#endif

#if TWIN_CRUTCH_LEFT || TWIN_CRUTCH_RIGHT
    void init(gpio_num_t DOUT, gpio_num_t SCK) {
      dev = hx711_t{
          .dout = DOUT,
          .pd_sck = SCK,
          .gain = HX711_GAIN_A_64,
          .offset = 0,
          .scale = twincrutches::config::app::
              scaleCalibrationFactor, // calibration factor from config
      };
      ESP_ERROR_CHECK(hx711_init(&dev));
      tare();
    }

    void tare() {
      if(dev.dout != 0) {
        ESP_ERROR_CHECK(hx711_tare(
            &dev, twincrutches::config::app::readAvgTimes)); // sets offset
      }
    }

    std::vector<float> readADCValues() {
      std::vector<float> vals{};
      float scale;
      hx711_get_units(&dev, twincrutches::config::app::readAvgTimes, &scale);
      vals.push_back(scale);
      return vals;
    }

#endif

#if TWIN_INSOLE_LEFT || TWIN_INSOLE_RIGHT
    void configure() {
      adc_oneshot_chan_cfg_t defaultConfig(ADC_ATTEN_DB_11,
                                           ADC_BITWIDTH_DEFAULT);
      for (auto &channel : channels) {
        configureADCChannel(adc2Handle, channel, &defaultConfig);
      }
    }

    std::vector<int> readADCValues() {
      std::vector<int> vals{};
      for (auto &channel : channels) {
        int val;
        adc_oneshot_read(adc2Handle, channel, &val);
        vals.push_back(val);
      }
      return vals;
    }
#endif

  private:
#if TWIN_CRUTCH_LEFT || TWIN_CRUTCH_RIGHT
    hx711_t dev;
#else
    void configureADCChannel(adc_oneshot_unit_handle_t &handle,
                             const adc_channel_t &chan,
                             adc_oneshot_chan_cfg_t *chanCfg) {
      ESP_ERROR_CHECK(adc_oneshot_config_channel(handle, chan, chanCfg));
    }

    adc_oneshot_unit_handle_t adc1Handle;
    adc_oneshot_unit_handle_t adc2Handle;
    std::array<adc_channel_t, sizeof...(Channels)> channels;
#endif
  };
}
#endif