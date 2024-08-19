#include <ArduinoOSCWiFi.h>

#include "config.h"
#include "variables.h"
#include "osc.h"

void motorMessage_callback(const OscMessage&);
void serverPing_callback(const OscMessage&);
void printMessage(const OscMessage& message);

void MotorMessage_Callback(const OscMessage& message){

    // create char array
    String msg_str = message.arg<String>(0);
    int msg_length = msg_str.length();
    char msg_char[msg_length + 1]; // +1 for null terminator
    msg_str.toCharArray(msg_char, msg_length + 1);

    const int numElements = msg_length / OSC_MOTOR_BYTE_NUM;

    // process each hex number
    char snippet[OSC_MOTOR_BYTE_NUM + 1]; // +1 for null terminator
    for (uint16_t i = 0; i < numElements; i++) {
        memcpy(snippet, &msg_char[OSC_MOTOR_BYTE_NUM * i], OSC_MOTOR_BYTE_NUM);
        snippet[OSC_MOTOR_BYTE_NUM] = '\0'; // null terminate the snippet
        // convert a section of the input string into an integer number
        motorDuty[i] = strtol(snippet, NULL, 16);
    }
    
}

void serverPing_callback(const OscMessage& message) {
    Serial.println("WIFI: Ping from server");
    
}

void printMessage(const OscMessage& message) {
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