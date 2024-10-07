#pragma once

// CONFIG FOR HEAD

//debug log
#define DEBUGLOG_DEFAULT_LOG_LEVEL_TRACE
#include <DebugLog.h>
#include "macros.h"

//Wifi Credentials
#define WIFI_SSID "SlimeServer"
#define WIFI_PASSWORD "95815480"

//PCA Module Pins
#define SDA_PIN -1
#define SCL_PIN -1
/**
 * -1, -1 for default pins on boards
 * ESP32C3 super Mini: 8, 9
 */


//device role
#define ROLE_HEAD
//#define ROLE_VEST

//stuff to do if head configured
#ifdef ROLE_HEAD
#include "head_map.hpp"
#endif 

//stuff to do if vest configured
#ifdef ROLE_VEST 
#include "vest_map.h"
#define SDA_PIN 8
#define SCL_PIN 9
#endif


//Ignore me 
//debug log
#define DEBUGLOG_DEFAULT_LOG_LEVEL_TRACE
#include <DebugLog.h>
#include "macros.h"

//pca communication clock
#define I2C_CLOCK 4000000U