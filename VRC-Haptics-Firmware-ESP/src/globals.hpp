#pragma once

#include "Arduino.h"

//calcualte map variables
extern const  uint8_t  ledcMap[];
extern const  uint8_t  pcaMap[];
extern const uint8_t pcaMapLen;
extern const uint8_t ledcMapLen;
extern const  uint8_t  totalMotors;

extern uint16_t allMotorVals[];
extern uint16_t ledcMotorVals[];
extern uint16_t pcaMotorVals[];