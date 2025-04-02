#include "sense/bno055.hpp"

twinperipheral::sense::TwinBNO055::TwinBNO055(i2c_port_t port, uint8_t i2cAddr,
                                              int sda_io_num, int scl_io_num,
                                              int timeout)
    : bno(port, i2cAddr), isCalibrated(false) {
  i2c_config_t conf;
  conf.mode = I2C_MODE_MASTER;
  conf.sda_io_num = sda_io_num;
  conf.scl_io_num = scl_io_num;
  conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
  conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
  conf.master.clk_speed = 100000;
  conf.clk_flags = 0;
  i2c_param_config(port, &conf);
  i2c_driver_install(port, I2C_MODE_MASTER, 0, 0, 0);
  i2c_set_timeout(port, timeout);

  gpio_reset_pin(STEMMA_QT_PWR);
  gpio_set_direction(STEMMA_QT_PWR, GPIO_MODE_OUTPUT);
}

std::string twinperipheral::sense::TwinBNO055::TAG = "TwinBNO055";

void twinperipheral::sense::TwinBNO055::powerOn(uint32_t &&wait) {
  gpio_set_level(STEMMA_QT_PWR, (uint32_t)1);
  vTaskDelay(wait / portTICK_PERIOD_MS);
}

void twinperipheral::sense::TwinBNO055::powerOff() {
  gpio_set_level(STEMMA_QT_PWR, (uint32_t)0);
}

void twinperipheral::sense::TwinBNO055::run() {
  bno.begin();
  bno.enableExternalCrystal();
}

bool twinperipheral::sense::TwinBNO055::isSelfTestSuccessful() {
  bno055_self_test_result_t res = bno.getSelfTestResult();
  ESP_LOGI(TAG.c_str(), "Self-Test Results: MCU: %u, GYR:%u, MAG:%u, ACC: %u",
           res.mcuState, res.gyrState, res.magState, res.accState);
  return ((res.mcuState == 1) && (res.gyrState == 1) && (res.magState == 1) &&
          (res.accState == 1));
}

bool twinperipheral::sense::TwinBNO055::runCalibration() {
  try {
    bno.setOprModeNdof();
    while (1) {
      // Calibration 3 = fully calibrated, 0 = not calibrated
      bno055_calibration_t cal = bno.getCalibration();
      bno055_vector_t v = bno.getVectorEuler();
      ESP_LOGI(TAG.c_str(),
               "Euler: X: %.1f Y: %.1f Z: %.1f || Calibration SYS: %u GYRO: %u "
               "ACC:%u MAG:%u",
               v.x, v.y, v.z, cal.sys, cal.gyro, cal.accel, cal.mag);
      if (cal.gyro == 3 && cal.accel == 3 && cal.mag == 3) {
        ESP_LOGI(TAG.c_str(), "Fully Calibrated.");
        bno.setOprModeConfig(); // Change to OPR_MODE
        vTaskDelay(200 / portTICK_PERIOD_MS);
        isCalibrated = true;
        return isCalibrated;
      }
      vTaskDelay(100 / portTICK_PERIOD_MS); // in fusion mode max output rate is
                                            // 100hz (actual rate: 100ms (10hz))
    }
  } catch (BNO055BaseException &ex) {
    ESP_LOGE(TAG.c_str(), "I2C Communcation failed: %s", ex.what());
    vTaskDelay(100 / portTICK_PERIOD_MS);
    return false;
  } catch (std::exception &ex) {
    ESP_LOGE(TAG.c_str(), "Something bad happened: %s", ex.what());
    vTaskDelay(100 / portTICK_PERIOD_MS);
    return false;
  }
}

void twinperipheral::sense::TwinBNO055::checkForCalibration() {
  if (!isCalibrated) {
    throw twinperipheral::sense::BNO055NotCalibratedException(
        "call runCalibration() before calling this method");
  }
}

bno055_vector_t twinperipheral::sense::TwinBNO055::getEulerAngles() {
  checkForCalibration();
  bno.setOprModeNdof();
  return bno.getVectorEuler();
}

bno055_vector_t twinperipheral::sense::TwinBNO055::getLinearAccel() {
  checkForCalibration();
  bno.setOprModeNdof();
  return bno.getVectorLinearAccel();
}

bno055_quaternion_t twinperipheral::sense::TwinBNO055::getQuaternion() {
  checkForCalibration();
  bno.setOprModeNdof();
  return bno.getQuaternion();
}

bno055_calibration_t twinperipheral::sense::TwinBNO055::getCalibrationStatus() {
  return bno.getCalibration();
}
