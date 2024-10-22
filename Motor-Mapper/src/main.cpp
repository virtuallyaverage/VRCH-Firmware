#include <Arduino.h>
#include "PWM/PCA/pca.h"
#include "globals.hpp"

void activateMotorChannel();

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

// Function to read input from the serial monitor and activate the corresponding motor channel
void activateMotorChannel() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n'); // Read the entire line of input
    int motorChannel = input.toInt(); // Convert the input to an integer

    if (motorChannel >= 0 && motorChannel < pcaMapLen) {
      setAllPcaDuty(0);
      setPCAMotorDuty(motorChannel, 4095); // Set the duty cycle to maximum to turn on the motor
      Serial.print("Motor channel ");
      Serial.print(motorChannel);
      Serial.println(" activated.");
    } else {
      setAllPcaDuty(0);
      Serial.println("Invalid motor channel. Please enter a number between 0 and 31.");
    }
  }
}


void setup() {
  Serial.begin(115200);
  startPCA(); // Initialize PCA modules
}

void loop() {
  activateMotorChannel(); // Continuously check for input and activate motor channel
}