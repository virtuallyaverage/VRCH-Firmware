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
time_t timeLastLow[MAX_BITBANG_MOTORS] = { 0 }; // the last time a motor indices was turned on
const double_t UpdateFrequency = LEDC_FREQUENCY;
const uint64_t updatePeriod = (1./UpdateFrequency)*1000000; // microseconds

void tick() {
    uint16_t num_motors = Haptics::conf.motor_map_ledc_num;
    for(uint16_t motor = 0; motor < num_motors; motor++) {
        if (toggleStates[motor]) {
            // if the difference means we hit our %ge of on time on
            if (micros() - timeLastLow[motor] > updatePeriod*(Haptics::globals.ledcMotorVals[motor]/65535.0)) { //floating point bad
                digitalWrite(Haptics::conf.motor_map_ledc[motor], LOW);
                toggleStates[motor] = false;
            } 
        } else { // motor has been turned off
            // time period has passed.
            if (micros() - timeLastLow[motor] > updatePeriod) {
                digitalWrite(Haptics::conf.motor_map_ledc[motor], HIGH);
                toggleStates[motor] = true;
                timeLastLow[motor] = micros();
            }
        }
    }
}

/// Sets ledc attached motors to the global configuration
int setLedcDuty() {
    // TODO: CONVERT FROM 16 bit to 12 bit(4096)
    for (uint8_t i = 0; i < Haptics::conf.motor_map_ledc_num; i++) {
        ledcWrite(i, Haptics::globals.ledcMotorVals[i]);
    }

    return 0;
}

int start(Config *conf) {
    uint16_t ledcMapLen = conf->motor_map_ledc_num;

    logger.debug("Period set to: %d", updatePeriod);

    for (uint8_t i = 0; i < ledcMapLen; i++) {
        // Stop the channel if it was previously configured.
        //ledcDetachPin(i);
        
        // Reinitialize the channel.
        uint8_t pin = conf->motor_map_ledc[i];
        //ledcSetup(i, LEDC_FREQUENCY, LEDC_RESOLUTION);
        pinMode(pin, OUTPUT);
        digitalWrite(pin, HIGH); //Cycling this seems to get it to work, idk why, pinmode should just work
        digitalWrite(pin, LOW);
        //ledcAttachPin(pin, i);
        logger.debug("Setting pin: %d to output on channel: %d", pin, i);
    }
    return 0;
}

} // namespace LEDC
} // namespace Haptics