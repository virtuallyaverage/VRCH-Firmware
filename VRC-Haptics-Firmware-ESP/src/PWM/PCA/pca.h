#include <Arduino.h>
#include "Adafruit_PWMServoDriver.h"
#include <vector> // I give up trying to get the other method working

#include "globals.hpp"
#include "macros.h"
#include "software_defines.h"
#include "config/config.h"
#include "logging/Logger.h"

#ifndef PCA_ME_H // don't want to intefere with offical library pca moduels
#define PCA_ME_H

#define MAP_OFFSET 1

namespace Haptics {
namespace PCA {
    Logging::Logger logger;

    void start(Config *conf);
    void setPCAMotorDuty(uint8_t motorIndex, uint16_t dutyCycle);
    void setPcaDuty();
    void setAllPcaDuty(uint16_t duty);
} // namespace PCA
} // namespace Haptics



#endif // PCA_ME_H