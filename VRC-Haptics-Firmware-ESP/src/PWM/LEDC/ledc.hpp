#pragma once
#include <Arduino.h>
#include <esp32-hal-ledc.h>

#include "config.hpp"
#include "globals.hpp"
#include "macros.h"

//defaults from senseshift (THANKS! :-) )
#define LEDC_FREQUENCY 60
#define LEDC_RESOLUTION 12

inline int setChannel(const uint8_t channel, const uint16_t duty){
    #ifndef LEDC_MAP
    return 1;
    #endif

    ledcWrite(channel, duty);
    //LOG_DEBUG("Set Channel: ", channel, " to duty:", duty);
    return 0;
}

int setAllLedcDuty(const uint16_t duty) {
    #ifndef LEDC_MAP
    return 1;
    #endif

    for (int i = 0; i < ledcMapLen; i++) {
        setChannel(i, duty);
    }
    return 0;
}

int setLedcDuty() {
    #ifndef LEDC_MAP
    return 1;
    #endif

    for (uint8_t i = 0; i < ledcMapLen; i++) {
        setChannel(i, ledcMotorVals[i]);
    }

    return 0;
}

int startLEDC() {
    #ifndef LEDC_MAP
    return 1; //avoid errors by returning early (hacky)
    #endif

    for (uint8_t i = 0; i < ledcMapLen; i++) {
        ledcSetup(i, LEDC_FREQUENCY, LEDC_RESOLUTION);
        pinMode(ledcMap[i], OUTPUT);
        ledcAttachPin(ledcMap[i], i);
        LOG_DEBUG("Setting pin: ", ledcMap[i], "to output, on channel:", i);
    }

    //chime
    setAllLedcDuty(4095);
    delay(100);
    setAllLedcDuty(0);

    return 0;
}