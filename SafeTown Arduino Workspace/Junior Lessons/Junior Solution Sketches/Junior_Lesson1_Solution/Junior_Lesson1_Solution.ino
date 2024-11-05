/** Junior Lesson 1 Solution Sketch: Brake Lights and Turn Signals
* This sketch implements basic brake light and turn signal functionality on the Junior robot.
*/

// LIBRARIES & INCLUSIONS
#include "Arduino.h"
//_________________________________________________________________________________________________________________________________

// CONSTANTS & CLASS INSTANTIATIONS
// these hold the GPIO pin values of the brake lights, the stop and start buttons, and the left and right turn signals
//***********************************************************************************************************************
const int brakeLightsPin = 25; 
const int stopPin = 16; 
const int startPin = 17;
const int leftSignal = 3; // THE LAST I CHECKED, THESE WERE THE CORRECT NUMS, BUT THEY'RE LISTED AS THE OPPOSITE IN THE GPIO TABLE  
const int rightSignal = 2;
//***********************************************************************************************************************

// this stores a reasonable amount of time (ms) for the duration of a blink or an off between blinks. We chose 1000ms = 1s. You can change this if you want
const long interval = 1000;
//_________________________________________________________________________________________________________________________________

// GLOBAL VARIABLES
int leftLedState = LOW;  // ledStates used to set the LEDs
int rightLedState = LOW; 

// generally, you should use "unsigned long" for variables that hold time
// the value will quickly become too large for an int to store
unsigned long leftPreviousMillis = 0;  // will store last time the left LEDs were updated
unsigned long rightPreviousMillis = 0; // will store last time the right LEDs were updated
//_________________________________________________________________________________________________________________________________

// MAIN FUNCTIONS
void setup() {
  // put your setup code here, to run once:

  // set the modes of the GPIO pins you will use
  pinMode(brakeLightsPin, OUTPUT);
  pinMode(stopPin, INPUT);
  pinMode(startPin, INPUT);
  pinMode(leftSignal, OUTPUT);
  pinMode(rightSignal, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

  // Section 1
  // update the brake lights
  //***********************************************************************************************************************
  if (digitalRead(stopPin) && digitalRead(startPin)) { // check if both the conditions "digitalRead(stopPin)" and "digitalRead(startPin)" are true. If both buttons are pushed, both conditions will be true, or 1
    // turn the brake lights on
    digitalWrite(brakeLightsPin, HIGH);
  } else {
    // if the start and stop buttons are not both being pushed, make sure the brake lights are off
    digitalWrite(brakeLightsPin, LOW);
  }
  //***********************************************************************************************************************

  // Section 2
  unsigned long currentMillis = millis(); // will store the current time since the program began running (ms) and will be updated every loop

  // update the right signals
  if (digitalRead(stopPin)) { // check if the condition "digitalRead(stopPin)" is true. If the stop button is pushed, this value will be true, or 1. If not, it will be false, or 0

    if (currentMillis - rightPreviousMillis >= interval) { // only update the right LEDs if at least an interval's worth of time has passed since the last time you updated the rightPreviousMillis 
                                                           // (a.k.a the last time you updated the right LEDs)
      
      // save the current time (the last time you updated the right LEDs)
      rightPreviousMillis = currentMillis;

      // if the right LEDs are off, turn them on, and vice-versa
      //***********************************************************************************************************************
      if (rightLedState == LOW) {
        rightLedState = HIGH;
      } else {
        rightLedState = LOW;
      }
      //***********************************************************************************************************************

      // set the left LEDs with the new leftLedState
      digitalWrite(rightSignal, rightLedState);
    };
  }
  else {
    // if the stop button isn't pushed, make sure the right LEDs are off
    //***********************************************************************************************************************
    digitalWrite(rightSignal, LOW);
    //***********************************************************************************************************************
  };

  // update the left signals
  if (digitalRead(startPin)) { // check if the condition "digitalRead(startPin)" is true. If the start button is pushed, this value will be true, or 1. If not, it will be false, or 0.

    if (currentMillis - leftPreviousMillis >= interval) { // only update the left LEDs if enough time has passed since the last time you updated the leftPreviousMillis (a.k.a the last time you updated the left LEDs)
      
      // save the time (the last time you updated the left LEDs)
      leftPreviousMillis = currentMillis;

      // if the left LEDs are off, turn them on, and vice-versa
      //***********************************************************************************************************************
      if (leftLedState == LOW) {
        leftLedState = HIGH;
      } else {
        leftLedState = LOW;
      }
      //***********************************************************************************************************************

      // set the left LEDs with the new leftLedState
      digitalWrite(leftSignal, leftLedState);
    };
  }
  else {
    // if the start button isn't pushed, make sure the left LEDs are off
    //***********************************************************************************************************************
    digitalWrite(leftSignal, LOW);
    //***********************************************************************************************************************
  };
}
//_________________________________________________________________________________________________________________________________ 
