#include <Arduino.h>

#include "globals.hpp"

void printMotorDuty() {
    Serial.print("Motor Duty : ");
    
    for (uint8_t i = 0; i < totalMotors; i++) {
        Serial.print(allMotorVals[i]);
        Serial.print(", ");
    }

    Serial.println("");
}