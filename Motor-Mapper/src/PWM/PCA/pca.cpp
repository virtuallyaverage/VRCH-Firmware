#include <Arduino.h>

#include "config.hpp"
#include "globals.hpp"

#include "pca.h"
#include "macros.h"

#include "Adafruit_PWMServoDriver.h"
#include <vector> // I give up trying to get the other method working



Adafruit_PWMServoDriver pcaModule1 = Adafruit_PWMServoDriver(PCA_1, Wire);//, Wire);
Adafruit_PWMServoDriver pcaModule2 = Adafruit_PWMServoDriver(PCA_2, Wire);//, Wire);

bool firstPCAConnected = false;
bool secondPCAConnected = false;

std::vector<uint16_t> debounceBuffer(pcaMapLen, 0);  // Dynamically sized array

/// @brief Start pca module communication
void startPCA() {
  Wire.begin(SDA_PIN, SCL_PIN, I2C_CLOCK);

  // init modules
  if (pcaModule1.begin()) {
    Serial.println("PCA1 Connected.");
    firstPCAConnected = true;
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
  if(firstPCAConnected) {
    pcaModule1.setPWMFreq(PCA_FREQUENCY);
    pcaModule1.setOscillatorFrequency(27000000);
    Serial.println(pcaModule1.readPrescale());
  }

  if (secondPCAConnected) {
    pcaModule2.setPWMFreq(PCA_FREQUENCY);
    pcaModule2.setOscillatorFrequency(27000000);
    Serial.println(pcaModule2.readPrescale());
  }

  
  
  //chime
  setAllPcaDuty(4095);
  delay(100);
  setAllPcaDuty(0);

}

/// @brief Sets PCA motors to the values from the pcaMotorVals array
void setPcaDuty() {
    for(uint8_t i = 0; i < 16; i++) {
        const uint16_t value = pcaMotorVals[i];
        const uint16_t value2 = pcaMotorVals[i+16];
        if (value != debounceBuffer[i] && firstPCAConnected) {
          pcaModule1.setPin(pcaMap[i], value);
          debounceBuffer[i] = value;
        }
        if (value2 != debounceBuffer[i+16] && secondPCAConnected) { //only send to second if it is connected
          pcaModule2.setPin(pcaMap[i+16]-16, value2);// 3 HOURS JUST TO FIND THE -16..... I WANT TO DIE
          debounceBuffer[i+16] = value2;
        }
    }
}

/// @brief Sets all motors to the specified duty cycle, mapped to the PCA_MAP defined in config.h
/// @param dutyCycle The list of each motors duty cycle
void setAllPcaDuty(uint16_t duty) {
    for(uint8_t i = 0; i < pcaMapLen; i++) {
        setPCAMotorDuty(i, duty);
    }
}

void setPCAMotorDuty(uint8_t motorIndex, uint16_t dutyCycle){
  if (motorIndex < 16) {
    //only push updates if they are different
    if (debounceBuffer[motorIndex] != dutyCycle && firstPCAConnected) {
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
