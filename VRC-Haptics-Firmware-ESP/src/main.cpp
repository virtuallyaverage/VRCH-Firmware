#include <Arduino.h>
#include "Wire.h"
#include "LittleFS.h"

// main config files
#include "globals.h"
#include "config/config.h"
#include "logging/Logger.h"

// switch between board imports
#ifdef BOARD_ESP32C3_SUPERMINI
#include "boards/esp32c3-supermini.h"
#else 
#include "boards/default.h"
#endif

// import modules
#include "OSC/osc.h"
#include "PWM/PCA/pca.h"
#include "PWM/LEDC/ledc.h"

// testing
#include "testing/rampPWM.hpp"
#include "PWM/PWMUtils.hpp"

// Global Logger instance
Haptics::Logging::Logger logger("Main");

void setup() {
  Serial.begin(115200);

  // Initialize LittleFS
  if (!LittleFS.begin(true)) {
    logger.error("LittleFS mount failed, please restart");
    return;
  }
  Haptics::loadConfig(&Haptics::conf);
  Haptics::initGlobals();

  Haptics::Wireless::Start(&Haptics::conf);
  Haptics::PCA::start(&Haptics::conf);
  Haptics::LEDC::start(&Haptics::conf);
}

uint32_t ticks = 0;
unsigned long startMillis = millis();

void loop() {
  Haptics::Wireless::Tick();
  Haptics::LEDC::setLedcDuty(&Haptics::globals, &Haptics::conf);
  Haptics::PCA::setPcaDuty(&Haptics::globals, &Haptics::conf);

  // rampTesting(); // uncomment this to continually ramp up and down (USED FOR MOTOR TESTING)
  
  ticks += 1;
  if (millis() - startMillis >= 1000) {
    LOG_INFO("Loop/sec: ", ticks);
    Haptics::PwmUtils::printAllDuty();
    Haptics::Wireless::printRawPacket();

    startMillis = millis();
    ticks = 0;
  }
}