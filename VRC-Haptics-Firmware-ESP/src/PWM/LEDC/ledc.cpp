#include "ledc.h"

namespace Haptics {
namespace LEDC {
Logging::Logger logger("LEDC");

inline int setChannel(const uint8_t channel, const uint16_t duty){ // throws an error if void
    ledcWrite(channel, duty);
    //logger.debug("Set Channel: ", channel, " to duty:", duty);
    return 0;
}

int setAllLedcDuty(const uint16_t duty, Config *conf) {
    for (int i = 0; i < conf->motor_map_ledc_num; i++) {
        setChannel(i, duty);
    }
    return 0;
}

/// Sets ledc attached motors to the global configuration
int setLedcDuty(Globals *globals, Config *conf) {
    for (uint8_t i = 0; i < conf->motor_map_ledc_num; i++) {
        setChannel(i, globals->ledcMotorVals[i]);
    }

    return 0;
}

int start(Config *conf) {

    uint16_t ledcMapLen = conf->motor_map_ledc_num;

    // TODO: FIX THESE
    for (uint8_t i = 0; i < ledcMapLen; i++) {
        ledcSetup(i, LEDC_FREQUENCY, LEDC_RESOLUTION);
        pinMode(conf->motor_map_ledc[i], OUTPUT);
        ledcAttachPin(conf->motor_map_ledc[i], i);
        logger.debug("Setting pin: ", conf->motor_map_ledc[i], "to output, on channel:", i);
    }

    //chime
    setAllLedcDuty(4095, conf);
    delay(100);
    setAllLedcDuty(0, conf);

    return 0;
}

} // namespace LEDC
} // namespace Haptics