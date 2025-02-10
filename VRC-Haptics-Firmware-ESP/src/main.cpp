#include <Arduino.h>
#include "Wire.h"
#include "LittleFS.h"

//main config files
#include "globals.hpp"
#include "config/config.h"
#include "logging/Logger.h"

// switch between board imports
#ifdef BOARD_ESP32C3_SUPERMINI
#include "boards/esp32c3-supermini.h"
#else 
#include "boards/default.h"
#endif

//import modules
#include "OSC/osc.h"
#include "PWM/PCA/pca.h"
#include "PWM/LEDC/ledc.h"

//testing
#include "testing/rampPWM.hpp"
#include "PWM/PWMUtils.hpp"

namespace Haptics {

uint16_t allMotorVals[totalMotors]  = {0};
uint16_t ledcMotorVals[ledcMapLen]  = {0};
uint16_t pcaMotorVals[pcaMapLen]    = {0};

// configuration struct
Config conf;
Logging::Logger logger;


void setup() {
  logger.setTag("Main");
  Serial.begin(115200);

  // Initialize LittleFS
  if (!LittleFS.begin(true)) {
    logger.error("LittleFS mount failed, please restart");
    return;
  }
  loadConfig(&conf);

  Wireless::Start(&conf);
  PCA::start(&conf);
  LEDC::start();
}

uint32_t ticks = 0;
unsigned long start = millis();
void loop() {

  Wireless::Tick();
  LEDC::setLedcDuty();
  PCA::setPcaDuty();

  //rampTesting(); //uncomment this to continually ramp up and down 
  // (USED FOR MOTOR TESTING)
  
  ticks += 1;
  if (millis()-start >= 1000) {
    LOG_INFO("Loop/sec: ", ticks);
    PwmUtils::printAllDuty();
    Wireless::printRawPacket();

    start = millis();
    ticks = 0;
  }

}
} // namespace Haptics