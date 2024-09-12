#include <Arduino.h>

#include "config.h"
#include "globals.h"

#include "PWM/pwmConfig.hpp"
#include "pca.h"

#include "Adafruit_PWMServoDriver.h"



Adafruit_PWMServoDriver pcaModule1 = Adafruit_PWMServoDriver(PCA_1, Wire);//, Wire);
Adafruit_PWMServoDriver pcaModule2 = Adafruit_PWMServoDriver(PCA_2, Wire);//, Wire);

bool secondPCAConnected = false;

uint8_t motorMap[32] = {MOTOR_MAP};
uint16_t debounceBuffer[32] = {0};

/// @brief Start pca module communication
void startPCA() {
  #ifdef ESP32C3
  Wire.begin(8, 9, 4000000);
  #else 
  Wire.begin();
  Wire.setClock(4000000);
  #endif

  // init modules
  if (pcaModule1.begin()) {
    Serial.println("PCA1 Connected.");
  } else {
    Serial.println("PCA1 Not Found.");
  }

  if (pcaModule2.begin()) {
    Serial.println("PCA2 Connected.");
    secondPCAConnected = true;
  } else {
    Serial.println("PCA2 Not Found.");
  }

  // init modules
  delay(100);
  //pcaModule1.reset();
  if (secondPCAConnected) {
    pcaModule2.reset();
  }
  

  //set frequencies
  delay(100);
  pcaModule1.setPWMFreq(PCA_FREQUENCY);
  pcaModule1.setOscillatorFrequency(27000000);
  Serial.println(pcaModule1.readPrescale());

  if (secondPCAConnected) {
    pcaModule2.setPWMFreq(PCA_FREQUENCY);
    pcaModule2.setOscillatorFrequency(27000000);
    Serial.println(pcaModule2.readPrescale());
  }

  
  
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
        //pcaModule2.setPin(motorMap[i+16]-16, motorDuty[i+16]);// 3 HOURS JUST TO FIND THE -16..... I WANT TO DIE
    }
}
void setToDuty(uint16_t duty) {
    for(uint8_t i = 0; i < 32; i++) {
        setMotorDuty(motorMap[i], duty);
    }
}

void setMotorDuty(uint8_t motorIndex, uint16_t dutyCycle){
  if (motorIndex < 16) {
    //only push updates if they are different
    if (debounceBuffer[motorIndex] != dutyCycle) {
      pcaModule1.setPin(motorIndex, dutyCycle);
      debounceBuffer[motorIndex] = dutyCycle;
      
    }

    
  } else {
    if (debounceBuffer[motorIndex] != dutyCycle && secondPCAConnected) {
      pcaModule2.setPin(motorIndex-16, dutyCycle);
      debounceBuffer[motorIndex] = dutyCycle;
    }
  }
}
