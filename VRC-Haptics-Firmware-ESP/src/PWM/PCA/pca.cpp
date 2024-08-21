#include <Arduino.h>

#include "config.h"
#include "globals.h"

#include "PWM/pwmConfig.h"
#include "pca.h"

#include "Adafruit_PWMServoDriver.h"
#include "Wire.h"

Adafruit_PWMServoDriver pcaModule1 = Adafruit_PWMServoDriver(PCA_1, Wire);
Adafruit_PWMServoDriver pcaModule2 = Adafruit_PWMServoDriver(PCA_2, Wire);


uint8_t motorMap[32] = {MOTOR_MAP};

/// @brief Start pca module communication
void startPCA() {
    // init modules
    pcaModule1.begin();
    pcaModule2.begin();

    //set frequencies
    pcaModule1.setPWMFreq(PCA_FREQUENCY);
    pcaModule2.setPWMFreq(PCA_FREQUENCY);

    pcaModule1.begin();
    pcaModule1.setOscillatorFrequency(27000000);
    pcaModule1.setPWMFreq(1600);  // This is the maximum PWM frequency

    pcaModule2.begin();
    pcaModule2.setOscillatorFrequency(27000000);
    pcaModule2.setPWMFreq(1600);  // This is the maximum PWM frequency

    Wire.setClock(400000);

    Serial.println(pcaModule1.getOscillatorFrequency());
    Serial.println(pcaModule2.getOscillatorFrequency());
    //chime
    setToDuty(4095);
    delay(100);
    setToDuty(0);
}

/// @brief Sets all motors to the specified duty cycle, mapped to the MOTOR_MAP defined in config.h
/// @param dutyCycle The list of each motors duty cycle
void setAllDuty() {
    for(uint8_t i = 0; i < 16; i++) {
        pcaModule1.setPin(motorMap[i], motorDuty[i]);
        pcaModule2.setPin(motorMap[i+16]-16, motorDuty[i+16]);// 3 HOURS JUST TO FIND THE -16..... I WANT TO DIE
    }
}
void setToDuty(uint16_t duty) {
    for(uint8_t i = 0; i < 32; i++) {
        setMotorDuty(motorMap[i], duty);
    }
}

void setMotorDuty(uint8_t motorIndex, uint16_t dutyCycle){
  if (motorIndex < 16) {
    pcaModule1.setPin(motorIndex, dutyCycle);
  } else {
    pcaModule2.setPin(motorIndex-16, dutyCycle);
  }
}
