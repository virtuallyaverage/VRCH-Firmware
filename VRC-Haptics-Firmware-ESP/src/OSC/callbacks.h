#include <ArduinoOSCWiFi.h>

void motorMessage_callback(const OscMessage&);
void serverPing_callback(const OscMessage&);
void printMessage(const OscMessage& message);