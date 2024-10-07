#include <Arduino.h>
#include "Wire.h"

//main config files
#include "globals.hpp"
#include "config.hpp"

//import modules
#include "OSC/osc.h"
#include "PWM/PCA/pca.h"
#include "PWM/LEDC/ledc.hpp"

//testing
#include "testing/rampPWM.hpp"
#include "PWM/PWMUtils.hpp"

//init globals here'
#ifdef LEDC_MAP
const uint8_t  ledcMap[]     =   {LEDC_MAP};
#else
const uint8_t ledcMap[]      = {0};
#endif

#ifdef PCA_MAP
const uint8_t  pcaMap[]     =   {PCA_MAP};
#else
const uint8_t pcaMap[]      = {0};
#endif

const uint8_t  pcaMapLen    =   sizeof(pcaMap) / sizeof(pcaMap[0]);
const uint8_t  ledcMapLen    =  sizeof(ledcMap) / sizeof(ledcMap[0]);
const uint8_t  totalMotors   =   ledcMapLen + pcaMapLen;

uint16_t allMotorVals[totalMotors]  = {0};
uint16_t ledcMotorVals[ledcMapLen]  = {0};
uint16_t pcaMotorVals[pcaMapLen]    = {0};

void setup() {
  Serial.begin(115200);

  WirelessStart();
  startPCA();
  startLEDC();

}

uint32_t ticks = 0;
unsigned long start = millis();
void loop() {

  WirelessTick();
  setLedcDuty();
  setPcaDuty();

  // rampTesting(); //uncomment this to continually ramp up and down 
  // (USED FOR MOTOR TESTING)
  
  ticks += 1;
  if (millis()-start >= 1000) {
    Serial.print("Loop/sec: ");
    Serial.println(ticks);
    printMotorDuty();
    printRawPacket();

    start = millis();
    ticks = 0;
  }

}
