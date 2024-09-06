#include <Arduino.h>
#include "Wire.h"

//main config files
#include "globals.h"
#include "config.h"

//import modules
#include "OSC/osc.h"
#include "PWM/PCA/pca.h"

//testing
#include "testing/rampPWM.hpp"

//init globals here
uint16_t motorDuty[32] = {0};

void setup() {
  Serial.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);

  startOSCWifi();
  startPCA();

}

uint32_t ticks = 0;
bool lightState = false;
unsigned long start = millis();
void loop() {

  unsigned long start_task = micros();

  oscTick();
  setAllDuty();


  //rampTesting();
  
  ticks += 1;
  if (millis()-start >= 1000) {
    Serial.print("Loop/sec: ");
    Serial.println(ticks);
    //printMotorDuty();

    digitalWrite(LED_BUILTIN, lightState ? HIGH : LOW);
    lightState = !lightState;

    start = millis();
    ticks = 0;
  }

}
