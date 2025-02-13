#include "ledc.h"

namespace Haptics {
namespace LEDC {
Logging::Logger logger("LEDC");

int setAllTo(const uint16_t duty) {
    for (int i = 0; i < Haptics::conf.motor_map_ledc_num; i++) {
        Haptics::conf.motor_map_ledc[i] = duty;
    }
    return 0;
}

uint16_t toggleStates[MAX_LEDC_MOTORS] = { false }; // state the motor is in currently
time_t timeLastLow[MAX_LEDC_MOTORS] = { 0 }; // the last time a motor indices was turned on
const double_t UpdateFrequency = LEDC_FREQUENCY;
const uint64_t updatePeriod = (1./UpdateFrequency)*1000000; // microseconds

#define TIMER_START uint32_t dwStart = ESP.getCycleCount();
#define TIMER_END Haptics::profiler.digitalWriteCycles += (ESP.getCycleCount() - dwStart);

void tick() {
    uint32_t loopStart = ESP.getCycleCount();
    uint32_t now = micros();
    uint16_t num_motors = Haptics::conf.motor_map_ledc_num;

    for(uint16_t motor = 0; motor < num_motors; motor++) {
        uint16_t duty = Haptics::globals.ledcMotorVals[motor];

        // Special cases: 0% duty -> always off; 100% duty -> always on
        if (duty == 0) {
            if(toggleStates[motor]) {
                digitalWrite(Haptics::conf.motor_map_ledc[motor], LOW);
                toggleStates[motor] = false;
                continue;
            } else {
                continue;
            }
        }
        if (duty == 65535) {
            if(!toggleStates[motor]) {
                digitalWrite(Haptics::conf.motor_map_ledc[motor], HIGH);
                toggleStates[motor] = true;
            } else {
                continue;
            }
        }
        
        double dutyRatio = duty / 65535.0; // floating point operations
        uint32_t onTime = (uint32_t)(updatePeriod * dutyRatio);
        uint32_t elapsed = now - timeLastLow[motor];// floating point operations end

        if (toggleStates[motor]) {
            TIMER_START  // start ticker
            if (elapsed > onTime) { //90.4% of cycles in the function are used in the comparison
                // 0.05% (practically just noise) for entire inside of the if
                digitalWrite(Haptics::conf.motor_map_ledc[motor], LOW);
                toggleStates[motor] = false;
            }
            TIMER_END // end ticker
        } else {
            if (elapsed > updatePeriod) {
                digitalWrite(Haptics::conf.motor_map_ledc[motor], HIGH);
                toggleStates[motor] = true;
                timeLastLow[motor] = now;
            }
        }
        
    }
    Haptics::profiler.loopCycles += (ESP.getCycleCount() - loopStart);
}

int start(Config *conf) {
    uint16_t ledcMapLen = conf->motor_map_ledc_num;

    logger.debug("Period set to: %d", updatePeriod);

    // Update pins and declare pinmodes
    for (uint8_t i = 0; i < ledcMapLen; i++) {
        uint8_t pin = conf->motor_map_ledc[i];
        pinMode(pin, OUTPUT);
        digitalWrite(pin, HIGH); //Cycling this seems to get it to work, idk why, pinmode should just work
        digitalWrite(pin, LOW);
        logger.debug("Setting pin: %d to output on channel: %d", pin, i);
    }
    return 0;
}

} // namespace LEDC
} // namespace Haptics