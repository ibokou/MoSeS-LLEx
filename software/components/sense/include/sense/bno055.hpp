#ifndef TWINPERIPHERAL_SENSE_BNO055_INCLUDED
#define TWINPERIPHERAL_SENSE_BNO055_INCLUDED

#include "BNO055ESP32.h"
#include "error/err.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define STEMMA_QT_PWR GPIO_NUM_7

namespace twinperipheral::sense {
  class BNO055NotCalibratedException
      : public twinperipheral::exception::BaseException {
    using twinperipheral::exception::BaseException::BaseException;
  };

  class TwinBNO055 {
  public:
    TwinBNO055(i2c_port_t port, uint8_t i2cAddr, int sda_io_num, int scl_io_num,
               int timeout);

    void run();
    void powerOn(uint32_t &&wait);
    void powerOff();
    bool isSelfTestSuccessful();
    bool runCalibration();
    bno055_quaternion_t getQuaternion();
    bno055_vector_t getEulerAngles();
    bno055_vector_t getLinearAccel();
    bno055_calibration_t getCalibrationStatus();

    static std::string TAG;

  private:
    BNO055 bno{(i2c_port_t)I2C_NUM_0, 0x28};
    void checkForCalibration();
    bool isCalibrated;
  };
}
#endif