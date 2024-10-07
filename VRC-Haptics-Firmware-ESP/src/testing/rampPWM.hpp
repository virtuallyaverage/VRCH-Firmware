#include "Arduino.h"
#include "PWM/PCA/pca.h"

/// @brief Ramp PCA pwm up and down for testing
void rampTesting() {
  Serial.println("Entering Fade");
  for (uint16_t state = 0; state < 1024; state++) {
    setAllPcaDuty(state*4);
    setAllLedcDuty(state * 4);
    delay(1);
  }
  for (uint16_t state = 1024; state > 0; state--) {
    setAllPcaDuty(state*4);
    setAllLedcDuty(state*4);
    delay(1);
  }
}