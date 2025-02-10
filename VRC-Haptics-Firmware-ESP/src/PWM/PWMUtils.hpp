#include <Arduino.h>

#include "globals.hpp"

namespace Haptics {
namespace PwmUtils {
    void printMotorDuty() {
    Serial.print("Motor Duty : ");
    
    for (uint8_t i = 0; i < totalMotors; i++) {
        Serial.print(allMotorVals[i]);
        Serial.print(", ");
    }

    Serial.println("");
    }

    void printPCADuty() {
        Serial.print("PCA Duty : ");
        
        for (uint8_t i = 0; i < pcaMapLen; i++) {
            Serial.print(pcaMotorVals[i]);
            Serial.print(", ");
        }

        Serial.println("");
    }

    void printLEDCDuty() {
        Serial.print("LEDC Duty : ");
        #ifdef LEDC_MAP
        for (uint8_t i = 0; i < pcaMapLen; i++) {
            Serial.print(ledcMotorVals[i]);
            Serial.print(", ");
        }
        #else
        Serial.print(ledcMotorVals[0]);
        Serial.print(", ");
        #endif
        Serial.println("");
    }


    void printAllDuty() {
        printMotorDuty();
        printPCADuty();
        printLEDCDuty();
    }

}
}
