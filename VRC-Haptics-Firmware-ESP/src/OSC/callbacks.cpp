#include "callbacks.h"


namespace Haptics  {
namespace Wireless { 
    bool first_packet = true;

    bool printNext = false;

    inline void printRaw() {
        printNext = true;
    }

    inline void updateMotorVals(){
        for (uint8_t i = 0; i < totalMotors; i++) {
            pcaMotorVals[i] = allMotorVals[i];
            continue;
            if (i < ledcMapLen) {
                ledcMotorVals[i] = allMotorVals[i];
            } else {
                #ifndef LEDC_MAP
                pcaMotorVals[i-ledcMapLen] = allMotorVals[i]; //assign to offset array
                #else 
                pcaMotorVals[i-ledcMapLen] = allMotorVals[i]; //assign to offset array
                #endif
            }
        }
    }

    void motorMessage_callback(const OscMessage& message){
        if (first_packet){
            LOG_INFO("FIRST PACKET");
            first_packet = false;
        }

        // create char array
        String msg_str = message.arg<String>(0);
        int msg_length = msg_str.length();
        char msg_char[msg_length + 1]; // +1 for null terminator
        msg_str.toCharArray(msg_char, msg_length + 1);

        const uint8_t numElements = msg_length / OSC_MOTOR_CHAR_NUM;

        // process each hex number
        char snippet[OSC_MOTOR_CHAR_NUM + 1]; // +1 for null terminator
        for (uint16_t i = 0; i < numElements; i++) {
            memcpy(snippet, &msg_char[OSC_MOTOR_CHAR_NUM * i], OSC_MOTOR_CHAR_NUM);
            snippet[OSC_MOTOR_CHAR_NUM] = '\0'; // null terminate the snippet
            // convert a section of the input string into an integer number
            allMotorVals[i] = strtol(snippet, NULL, 16);
        }

        updateMotorVals();
        
    }


    void printOSCMessage(const OscMessage& message) {
        Serial.print("Address: ");
        Serial.println(message.address());
        for (size_t i = 0; i < message.size(); i++) {
            Serial.print("Index: ");
            Serial.print((int)i);
            Serial.print("Content: ");
            Serial.print(message.arg<String>(i));
            Serial.println("");
        }
        Serial.print("\n");
    }

} // namespace Wireless
} // namespace Haptics
