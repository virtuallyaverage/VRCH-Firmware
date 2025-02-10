#include "ledc.h"

namespace Haptics {
namespace LEDC {
Logging::Logger logger;

inline int setChannel(const uint8_t channel, const uint16_t duty){
    //TODO: FIXME

    ledcWrite(channel, duty);
    //logger.debug("Set Channel: ", channel, " to duty:", duty);
    return 0;
}

int setAllLedcDuty(const uint16_t duty) {
    // TODO: FIX THESE

    for (int i = 0; i < ledcMapLen; i++) {
        setChannel(i, duty);
    }
    return 0;
}

int setLedcDuty() {
    // TODO: FIX THESE

    for (uint8_t i = 0; i < ledcMapLen; i++) {
        setChannel(i, ledcMotorVals[i]);
    }

    return 0;
}

int start() {
    logger.setTag("LEDC");

    // TODO: FIX THESE
    for (uint8_t i = 0; i < ledcMapLen; i++) {
        ledcSetup(i, LEDC_FREQUENCY, LEDC_RESOLUTION);
        pinMode(ledcMap[i], OUTPUT);
        ledcAttachPin(ledcMap[i], i);
        logger.debug("Setting pin: ", ledcMap[i], "to output, on channel:", i);
    }

    //chime
    setAllLedcDuty(4095);
    delay(100);
    setAllLedcDuty(0);

    return 0;
}

} // namespace LEDC
} // namespace Haptics