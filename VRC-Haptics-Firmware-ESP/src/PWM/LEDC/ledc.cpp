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

void tick() {
    // Read the current time once for consistency
    uint32_t now = micros();
    uint16_t num_motors = Haptics::conf.motor_map_ledc_num;
    
    for(uint16_t motor = 0; motor < num_motors; motor++) {
        // Retrieve the duty value once (0 to 65535)
        uint16_t duty = Haptics::globals.ledcMotorVals[motor];
        
        // Special cases: 0% duty -> always off; 100% duty -> always on
        if (duty == 0) {
            digitalWrite(Haptics::conf.motor_map_ledc[motor], LOW);
            // Optionally, force state variables if used later:
            toggleStates[motor] = false;
            continue;
        }
        if (duty == 65535) {
            digitalWrite(Haptics::conf.motor_map_ledc[motor], HIGH);
            toggleStates[motor] = true;
            continue;
        }
        
        // Normal PWM: Calculate on-time based on duty ratio.
        double dutyRatio = duty / 65535.0;
        uint32_t onTime = (uint32_t)(updatePeriod * dutyRatio);

        // Determine elapsed time since the start of the current period.
        // timeLastLow[motor] here represents the start of the current PWM period.
        uint32_t elapsed = now - timeLastLow[motor];

        if (toggleStates[motor]) {
            // Motor is currently HIGH.
            if (elapsed >= onTime) {
                // It’s time to go LOW
                digitalWrite(Haptics::conf.motor_map_ledc[motor], LOW);
                toggleStates[motor] = false;
            }
        } else {
            // Motor is currently LOW.
            if (elapsed >= updatePeriod) {
                // Start of a new PWM cycle: set HIGH and reset period start.
                digitalWrite(Haptics::conf.motor_map_ledc[motor], HIGH);
                toggleStates[motor] = true;
                timeLastLow[motor] = now;
            }
        }
    }
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