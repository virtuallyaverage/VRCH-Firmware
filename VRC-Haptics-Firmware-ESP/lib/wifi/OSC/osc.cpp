#include <ArduinoOSCWiFi.h>

#include "config.h"

#include "callbacks.cpp"

const IPAddress ip(OSC_LOCAL_IP);
const IPAddress gateway(OSC_GATEWAY_IP);
const IPAddress subnet(255, 255, 255, 0);


void startOSCWifi() {
    // Start WiFi connection
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.println("WIFI: Connecting");
    
    // Wait for connection
    while (WiFi.status() != WL_CONNECTED) {}

    // Print the IP address
    Serial.print("WIFI: IP: ");
    Serial.println(WiFi.localIP());

    // Setup OSC
    OscWiFi.subscribe(OSC_IN_PORT, OSC_MOTOR_ADDRESS, MotorMessage_Callback);
    OscWiFi.subscribe(OSC_IN_PORT, "/ping/", serverPing_callback);
    

    Serial.println("OSC setup complete.");
}

void oscTick() {
    OscWiFi.update(); // should be called to subscribe + publish osc
    
}

void printMotorDuty() {
    Serial.print("Motor Duty : ");
    
    for (uint8_t i = 0; i < 32; i++) {
        Serial.print(motorDuty[i]);
        Serial.print(", ");
    }

    Serial.println("");
}

//overloads of the send function
void sendOSC(const String& oscAddress, const bool& value)  {
    OscWiFi.send(OSC_SERVER_IP_STR, OSC_OUT_PORT, oscAddress, value);
}
void sendOSC(const String& oscAddress, const uint16_t& value)  {
    OscWiFi.send(OSC_SERVER_IP_STR, OSC_OUT_PORT, oscAddress, value);
}
void sendOSC(const String& oscAddress, const uint8_t& value)  {
    OscWiFi.send(OSC_SERVER_IP_STR, OSC_OUT_PORT, oscAddress, value);
}
void sendOSC(const String& oscAddress, const int16_t& value)  {
    OscWiFi.send(OSC_SERVER_IP_STR, OSC_OUT_PORT, oscAddress, value);
}
void sendOSC(const String& oscAddress, const int8_t& value)  {
    OscWiFi.send(OSC_SERVER_IP_STR, OSC_OUT_PORT, oscAddress, value);
}
void sendOSC(const String& oscAddress, const float_t& value)  {
    OscWiFi.send(OSC_SERVER_IP_STR, OSC_OUT_PORT, oscAddress, value);
}
void sendOSC(const String& oscAddress, char *value, uint16_t size)  {
    OscWiFi.send(OSC_SERVER_IP_STR, OSC_OUT_PORT, oscAddress, value);
}