#include <Arduino.h>

#include "config.h"
#include "variables.h"

#include "pwmConfig.h"
#include "pca.h"

#include "Adafruit_PWMServoDriver.h"

#ifdef PCA_1
Adafruit_PWMServoDriver pcaModule1 = Adafruit_PWMServoDriver(PCA_1);
#endif

#ifdef PCA_2
Adafruit_PWMServoDriver pcaModule2 = Adafruit_PWMServoDriver(PCA_2);
#endif

uint8_t motorMap[32] = {MOTOR_MAP};

/// @brief Start pca module communication
void startPCA() {
    // init modules
    pcaModule1.begin();
    pcaModule2.begin();

    //set frequencies
    pcaModule1.setPWMFreq(PCA_FREQUENCY);
    pcaModule2.setPWMFreq(PCA_FREQUENCY);
}

/// @brief Sets all motors to the specified duty cycle, mapped to the MOTOR_MAP defined in config.h
/// @param dutyCycle The list of each motors duty cycle
void setAllDuty() {
    for(uint8_t i = 0; i < 32; i++) {
        setMotorDuty(motorMap[i], motorDuty[i]);
    }
}

void setMotorDuty(uint8_t motorIndex, uint16_t dutyCycle){
  if (motorIndex < 16) {
    pcaModule1.setPin(motorIndex, dutyCycle);
  } else {
    pcaModule2.setPin(motorIndex-16, dutyCycle);
  }
}
