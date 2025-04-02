/*
To prevent naming conflics in context of min max functions with C++-STL, we
include the headers first that use STL. After that headers are included that
include ESP-IDF specific header files that define min max macro which causes the
naming conflict

See:
https://stackoverflow.com/questions/41093090/esp8266-error-macro-min-passed-3-arguments-but-takes-just-2
*/

#include "ble/stack.hpp"
#include "esp_err.h"
#include "led/led.hpp"
#include "sense/adc.hpp"
#include "sense/bno055.hpp"
#include "statemachine/state_machine.hpp"
#include "system/sys.hpp"

#define R_PIN GPIO_NUM_5
#define G_PIN GPIO_NUM_6
#define B_PIN GPIO_NUM_9

extern "C" {
void app_main(void);
}

auto const LOG_APP_INFO_TAG = std::string("STATE_INFO");

auto app_main(void) -> void {
  static SemaphoreHandle_t stateMutex = xSemaphoreCreateMutex();
  static SemaphoreHandle_t ledMutex = xSemaphoreCreateMutex();

  twinperipheral::led::RGBLED led(ledMutex, R_PIN, G_PIN, B_PIN);
  twinperipheral::sense::TwinBNO055 bno((i2c_port_t)I2C_NUM_0, 0x28, GPIO_NUM_3,
                                        GPIO_NUM_4, 20);

  twinperipheral::sense::TwinADC adc(ADC_CHANNEL_7, ADC_CHANNEL_6,
                                     ADC_CHANNEL_5);
  adc.configure();

  twinperipheral::system::SystemController sys(1023123, GPIO_NUM_10, led);

  twinperipheral::statemachine::TwinPeripheralStateMachine stateMachine(
      stateMutex, sys, bno, adc, led);

  stateMachine.performStartUpRoutine();

  while (1) {
    vTaskDelay(10);
  };
}
