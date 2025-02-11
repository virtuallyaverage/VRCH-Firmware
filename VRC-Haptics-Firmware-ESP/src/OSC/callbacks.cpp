#include "callbacks.h"


namespace Haptics  {
namespace Wireless { 
    bool first_packet = true;

    bool printNext = false;

    void printRaw() {
        printNext = true;
    }

    /// @brief sets the individual ledc and i2c maps from the global maps
    /// i2c_num -> 4
    /// ledc_num -> 2
    ///  0, 1, 2, 3, 4, 5, (6 numbers transmitted)
    /// [0, 1]              //ledc 
    ///       [2, 3, 4, 5]  //i2c
    inline void updateMotorVals(Globals *globals, Config *conf){
        const uint16_t totalMotors = conf->motor_map_i2c_num+conf->motor_map_ledc_num;
        for (uint16_t i = 0; i < totalMotors; i++) {
            if (conf->motor_map_i2c_num && conf->motor_map_ledc_num) { // both i2c and ledc motors
                // haven't got through the ledc motors
                if (i < conf->motor_map_ledc_num) {
                    globals->ledcMotorVals[i] = globals->allMotorVals[i];
                } else { // past ledc
                    globals->pcaMotorVals[i-conf->motor_map_ledc_num] = globals-> allMotorVals[i];
                }
            } else if (conf->motor_map_i2c_num) {// if only i2c
                globals->pcaMotorVals[i] = globals->allMotorVals[i];
            } else {// assume it's only ledc
                globals->ledcMotorVals[i] = globals->allMotorVals[i];
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
            Haptics::globals.allMotorVals[i] = strtol(snippet, NULL, 16);
        }

        updateMotorVals(&Haptics::globals, &Haptics::conf);
        
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
