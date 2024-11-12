/// @brief public functions for the osc.cpp file

#define RECIEVE_PORT 1027

#define HEARTBEAT_ADDRESS "/hrtbt"
#define PING_ADDRESS "/ping"
#define MOTOR_ADDRESS "/h"

#define MDNS_SERVICE_NAME "haptics"
#define MDNS_SERVICE_PROTOCOL "udp"

void WirelessStart();
bool WiFiConnected();
void WirelessTick();
void printRawPacket();
