#include <ArduinoOSCWiFi.h>
#include <ESPmDNS.h>
#include <WiFi.h>

#include "software_defines.h"
#include "globals.h"
#include "config/config.h"

#ifndef OSC_H
#define OSC_H

namespace Haptics {
namespace Wireless {

// OSC client to send messages back to the hosts
inline OscWiFiClient oscClient;

//publisher references
inline OscPublishElementRef heartbeatPublisher;

// we need to get host ip first
inline String selfIP = "";
inline String selfMac = WiFi.macAddress();
inline String hostIP = "";
inline uint32_t sendPort = 0;
inline uint32_t recvPort = RECIEVE_PORT;

void StartMDNS(Config *conf);
void StartHeartBeat();
void handlePing(const OscMessage& message);

void Start(Config *conf);
bool WiFiConnected();
void Tick();
void printRawPacket();

} // namespace Wireless
} // namespace Haptics

#endif // OSC_H