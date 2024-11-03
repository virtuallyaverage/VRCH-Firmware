#pragma once

//******************************* SET ME ***************************************

//Wifi Credentials
#define WIFI_SSID "SlimeServer"
#define WIFI_PASSWORD "95815480"

//device role
//#define ROLE_HEAD
//#define ROLE_VEST
//#define ROLE_VEST_F
#define ROLE_VEST_B

/// ******************************* Advanced Configuration *********************


//PCA Module Pins
#define SDA_PIN 8
#define SCL_PIN 9
/**
 * -1, -1 for default (marked) pins on boards
 * ESP32C3 super Mini: 8, 9
*/

#ifndef ROLE_HEAD
#ifndef ROLE_VEST
#pragma message ("No presets are selected. This is fine as long as a custom ledc or PCA map is defined.")
#endif
#endif 

//stuff to do if head configured
#ifdef ROLE_HEAD
#define LEDC_MAP 13, 32, 12, 33, 14, 25, 27, 26
#endif 

//stuff to do if vest configured
#ifdef ROLE_VEST
#ifdef ROLE_HEAD
#error "Both ROLE_VEST and ROLE_HEAD are uncommented"
#endif

#define PCA_MAP 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
#endif

//stuff to do if split vest configured
#if defined(ROLE_VEST_F)
#ifdef ROLE_HEAD
#error "Both ROLE_VEST and ROLE_HEAD are uncommented"
#endif
#define PCA_MAP 0, 3, 14, 15, 1, 2, 12, 13, 7, 6, 9, 8, 5, 4, 11, 10
//#define PCA_MAP 0, 3, 14, 15, 1, 2, 14, 15, 7, 6, 9, 8, 5, 4, 11, 10
#endif

#if defined(ROLE_VEST_B)
#ifdef ROLE_HEAD
#error "Both ROLE_VEST and ROLE_HEAD are uncommented"
#endif
//#define PCA_MAP 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
#define PCA_MAP 6, 7, 15, 14, 4, 5, 13, 12, 9, 9, 8, 9, 9, 9, 9, 9
#endif

//IGNORE ME
//debug log
#define DEBUGLOG_DEFAULT_LOG_LEVEL_TRACE
#include <DebugLog.h>
#include "macros.h"

//pca communication clock
#define I2C_CLOCK 4000000U