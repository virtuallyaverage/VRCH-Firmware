#pragma once

//******************************* SET ME ***************************************

//Wifi Credentials
#define WIFI_SSID "SlimeServer"
#define WIFI_PASSWORD "95815480"

//device role
//#define ROLE_HEAD
#define ROLE_VEST

/// ******************************* Advanced Configuration *********************


//PCA Module Pins
#define SDA_PIN 8
#define SCL_PIN 9
/**
 * -1, -1 for default (marked) pins on boards
 * ESP32C3 super Mini: 8, 9
*/

//stuff to do if head configured
#ifdef ROLE_HEAD
#define LEDC_MAP 13, 32, 12, 33, 14, 25, 27, 26
#endif 

//stuff to do if vest configured
#ifdef ROLE_VEST 

#ifdef ROLE_HEAD
#error "Both ROLE_VEST and ROLE_HEAD are uncommented"
#endif

#define PCA_MAP 15, 14, 1, 0, 13, 12, 3, 2, 8, 9, 5, 6, 10, 11, 7, 4, 25,24,16,17,27,26,18,19,29,30,22,21,28,31,20,23
#endif

//IGNORE ME
//debug log
#define DEBUGLOG_DEFAULT_LOG_LEVEL_TRACE
#include <DebugLog.h>
#include "macros.h"

//pca communication clock
#define I2C_CLOCK 4000000U