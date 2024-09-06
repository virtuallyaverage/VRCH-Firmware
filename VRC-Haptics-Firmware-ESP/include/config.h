//Wifi Credentials
#define WIFI_SSID "SlimeServer"
#define WIFI_PASSWORD "95815480"

//board
#define ESP32C3

#define MOTOR_MAP 15, 14, 1, 0, 13, 12, 3, 2, 8, 9, 5, 6, 10, 11, 7, 4,25,24,16,17,27,26,18,19,29,30,22,21,28,31,20,23

//IP Addresses
#define OSC_SERVER_IP 192, 168, 1, 104
#define OSC_SERVER_IP_STR "192.168.1.104"
#define OSC_LOCAL_IP 192, 168, 1, 245
#define OSC_GATEWAY_IP 192, 168, 1, 1

//OSC Setup
#define OSC_IN_PORT 1025
#define OSC_OUT_PORT 1026
#define OSC_MOTOR_ADDRESS "/h"

// motor setu
#define OSC_MOTOR_BYTE_NUM 4


//Timing stuff (ms)
#define INTERVAL_SHOW_VALS 1000
#define INTERVAL_OSC_TICK 1
#define INTERVAL_OVERRUN_CHECK 100
