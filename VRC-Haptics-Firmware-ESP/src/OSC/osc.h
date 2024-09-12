/// @brief public functions for the osc.cpp file

#define RECIEVE_PORT 1027

#define HEARTBEAT_ADDRESS "/hrtbt"
#define PING_ADDRESS "/ping"

#define MDNS_SERVICE_NAME "haptics"
#define MDNS_SERVICE_PROTOCOL "osc"

void WirelessStart();

bool WiFiConnected();

void WirelessTick();

