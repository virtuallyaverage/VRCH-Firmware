#include <Arduino.h>

#include "globals.h"

void printMotorDuty() {
    Serial.print("Motor Duty : ");
    
    for (uint8_t i = 0; i < 32; i++) {
        Serial.print(motorDuty[i]);
        Serial.print(", ");
    }

    Serial.println("");
}