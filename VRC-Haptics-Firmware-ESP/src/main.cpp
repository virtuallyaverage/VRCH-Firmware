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

  WirelessStart();
  startPCA();

}

uint32_t ticks = 0;
bool lightState = false;
unsigned long start = millis();
void loop() {

  unsigned long start_task = micros();
  WirelessTick();
  setAllDuty();


  //rampTesting(); //uncomment this to continually ramp up and down 
  // (USED FOR MOTOR TESTING)
  
  ticks += 1;
  if (millis()-start >= 1000) {
    Serial.print("Loop/sec: ");
    Serial.println(ticks);
    //printMotorDuty();

    start = millis();
    ticks = 0;
  }

}
