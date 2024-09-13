#pragma once

//debug log
#define DEBUGLOG_DEFAULT_LOG_LEVEL_TRACE
#include <DebugLog.h>
#include "macros.h"

//Wifi Credentials
#define WIFI_SSID "SlimeServer"
#define WIFI_PASSWORD "95815480"

//device role
//#define ROLE_HEAD
#define ROLE_VEST

//board
//#define ESP32C3

//collider maps
#define PCA_MAP 15, 14, 1, 0, 13, 12, 3, 2, 8, 9, 5, 6, 10, 11, 7, 4, 25,24,16,17,27,26,18,19,29,30,22,21,28,31,20,23
#define LEDC_MAP 27, 26, 25, 33, 32

#define LEDC_AFTER_PCA