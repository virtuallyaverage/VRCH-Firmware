#ifndef GLOBALS_H
#define GLOBALS_H

#include "Arduino.h"
#include "software_defines.h"

namespace Haptics {
    // Volatile, non-static, user-denied variables
    struct Globals {
        uint16_t ledcMotorVals[MAX_LEDC_MOTORS];
        uint16_t pcaMotorVals[MAX_I2C_MOTORS];
        uint16_t allMotorVals[MAX_LEDC_MOTORS + MAX_I2C_MOTORS];
    };

    inline Globals initGlobals() {
        Globals g = {};
        return g;
    }

    // Declare a global instance of Globals.
    // (C++17 inline variables ensure one definition across translation units.)
    inline Globals globals = initGlobals();
} // namespace Haptics

#endif // GLOBALS_H