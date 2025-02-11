#include <Arduino.h>

#include "globals.h"

namespace Haptics {
namespace PwmUtils {
    void printMotorDuty() {
        Serial.print("Motor Duty : ");
        const uint16_t totalMotors = conf.motor_map_i2c_num+conf.motor_map_ledc_num;
        for (uint8_t i = 0; i < totalMotors; i++) {
            Serial.print(globals.allMotorVals[i]);
            Serial.print(", ");
        }

        Serial.println("");
    }

    void printPCADuty() {
        Serial.print("PCA Duty : ");
        
        for (uint8_t i = 0; i < conf.motor_map_i2c_num; i++) {
            Serial.print(globals.pcaMotorVals[i]);
            Serial.print(", ");
        }

        Serial.println("");
    }

    void printLEDCDuty() {
        Serial.print("LEDC Duty : ");
        for (uint8_t i = 0; i < conf.motor_map_ledc_num; i++) {
            Serial.print(globals.ledcMotorVals[i]);
            Serial.print(", ");
        }
        Serial.println("");
    }


    void printAllDuty() {
        printMotorDuty();
        printPCADuty();
        printLEDCDuty();
    }

}
}
