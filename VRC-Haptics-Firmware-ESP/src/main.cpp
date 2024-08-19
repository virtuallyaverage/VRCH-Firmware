#include <Arduino.h>

//main config files
#include "variables.h"
#include "config.h"
#include "main.h"

//import modules
#include "OSC/osc.h"
#include "PCA/pca.h"

//setup scheduler
#include <TaskScheduler.h>

Scheduler TaskScheduler;

//Task task1(INTERVAL_OSC_TICK, TASK_FOREVER, &oscTick, &TaskScheduler, true);
//Task task2(INTERVAL_SHOW_VALS, TASK_FOREVER, &printMotorDuty, &TaskScheduler, true);
//Task task3(INTERVAL_SHOW_VALS, TASK_FOREVER, &overrunCheck, &TaskScheduler, true);

unsigned long previous = millis();
void overrunCheck(){
  unsigned long diff = millis() - previous;
  if (diff > INTERVAL_OVERRUN_CHECK) {
    Serial.print("OVERUN: ");
    Serial.println(diff);
  }
  previous = millis();
}

unsigned long timeTask(Callback timed_function ) {
  unsigned long start = micros();
  timed_function();

  return micros() - start;
}

void setup() {
  Serial.begin(115200);

  startOSCWifi();
  startPCA();

  //time our tasks
  Serial.print("TASK: OSCTick Duration: ");
  Serial.println(timeTask(&oscTick));
  Serial.print("TASK: printMotorDuty Duration: ");
  Serial.println(timeTask(&printMotorDuty));

  //TaskScheduler.startNow();
}

uint32_t ticks = 0;
unsigned long start = millis();
void loop() {
  //TaskScheduler.execute();

  oscTick();
  setAllDuty();
  
  ticks += 1;
  if (millis()-start >= 1000) {
    Serial.print("LoopRate: ");
    Serial.println(ticks);

    start = millis();
    ticks = 0;
  }

}
