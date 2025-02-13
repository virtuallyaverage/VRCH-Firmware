#include <Arduino.h>
#include "Wire.h"
#include "LittleFS.h"

// main config files
#include "globals.h"
#include "config/config.h"
#include "logging/Logger.h"

// import modules
#include "OSC/osc.h"
#include "PWM/PCA/pca.h"
#include "PWM/LEDC/ledc.h"
#include "serial/serial.h"

// testing
#include "testing/rampPWM.hpp"
#include "PWM/PWMUtils.hpp"

// Main Logger instance
Haptics::Logging::Logger logger("Main");

void setup() {
  Serial.begin(115200);

  #ifdef DEV_MODE
  //wait for serial if we are developing
  delay(700);
  #endif

  // Initialize LittleFS
  if (!LittleFS.begin(true)) {
    logger.error("LittleFS mount failed, please restart");
    return;
  }

  Haptics::loadConfig();
  Haptics::initGlobals();

  Haptics::Wireless::Start(&Haptics::conf);
  Haptics::PCA::start(&Haptics::conf);
  Haptics::LEDC::start(&Haptics::conf);
}

uint32_t ticks = 0;
unsigned long startMillis = millis();

void loop() {
  if (Haptics::globals.reinitLEDC == true) { // prevents not defined error
    Haptics::LEDC::start(&Haptics::conf);
    logger.debug("Restarted LEDC");
    Haptics::globals.reinitLEDC = false;
  }

  Haptics::Wireless::Tick();
  Haptics::LEDC::tick();
  Haptics::PCA::setPcaDuty(&Haptics::globals, &Haptics::conf);
  Haptics::SerialComm::tick();

  // rampTesting(); // uncomment this to continually ramp up and down (USED FOR MOTOR TESTING)
  
  ticks += 1;
  if (millis() - startMillis >= 1000) {
    logger.debug("Loop/sec: %d", ticks);
    Haptics::PwmUtils::printAllDuty();
    Haptics::Wireless::printRawPacket();

    startMillis = millis();
    ticks = 0;
  }
}