#ifndef LEDC_ME_H // idk if just LEDC would interfere
#define LEDC_ME_H

#include <Arduino.h>
#include <esp32-hal-ledc.h>

#include "globals.hpp"
#include "macros.h"
#include "software_defines.h"
#include "logging/Logger.h"

namespace Haptics {
namespace LEDC {
    inline int setChannel(const uint8_t channel, const uint16_t duty);
    int setAllLedcDuty(const uint16_t duty);
    int setLedcDuty();
    int start();
} // namespace LEDC
} // namespace Haptics

#endif // LEDC_ME_H