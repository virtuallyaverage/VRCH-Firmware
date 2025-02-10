#include "Arduino.h"
#include "PWM/PCA/pca.h"
#include "globals.hpp"

namespace Haptics {
namespace PwmUtils {
  void setMotorArray(uint16_t state) {
    for (int i = 0; i < totalMotors; i++) {
      allMotorVals[i] = state;
    }
  }

  /// @brief Ramp PCA pwm up and down for testing
  void rampTesting() {
    LOG_INFO("Entering Motor Ramp");
    for (uint16_t state = 0; state < 1024; state++) {
      PCA::setAllPcaDuty(state*4);
      delay(1);
    }
    for (uint16_t state = 1024; state > 0; state--) {
      PCA::setAllPcaDuty(state*4);
      delay(1);
    }
  }

}
}
