/** Junior Lesson 1 Student Sketch: Brake Lights and Turn Signals
* This sketch implements basic brake light and turn signal functionality on the Junior robot.
*/

// LIBRARIES & INCLUSIONS
#include "Arduino.h"
//_________________________________________________________________________________________________________________________________

// CONSTANTS & CLASS INSTANTIATIONS
// these hold the GPIO pin values of the stop and start buttons and the left and right turn signals
//***********************************************************************************************************************
const int stopPin = /** insert the STOP GPIO pin number here*/ ;
const int startPin = /** insert the START GPIO pin number here*/ ;
const int leftSignal = /** insert the left turn signal GPIO pin number here*/ ;
const int rightSignal = /** insert the right turn signal GPIO pin number here*/ ;
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
  // insert your own code here to check the stop and start buttons and to change the brake lights accordingly
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
      // insert your own code here to change the rightLedState to reflect which state it now needs to be given its previous state
      //***********************************************************************************************************************

      // set the left LEDs with the new leftLedState
      digitalWrite(rightSignal, rightLedState);
    };
  }
  else {
    // if the stop button isn't pushed, make sure the right LEDs are off
    //***********************************************************************************************************************
    // insert your own code here to turn the right LEDs off
    //***********************************************************************************************************************
  };

  // update the left signals
  if (digitalRead(startPin)) { // check if the condition "digitalRead(startPin)" is true. If the start button is pushed, this value will be true, or 1. If not, it will be false, or 0.

    if (currentMillis - leftPreviousMillis >= interval) { // only update the left LEDs if enough time has passed since the last time you updated the leftPreviousMillis 
                                                          // (a.k.a the last time you updated the left LEDs)
      
      // save the time (the last time you updated the left LEDs)
      leftPreviousMillis = currentMillis;

      // if the left LEDs are off, turn them on, and vice-versa
      //***********************************************************************************************************************
      // Insert your own code here to change the leftLedState to reflect which state it now needs to be given its previous state
      //***********************************************************************************************************************

      // set the left LEDs with the new leftLedState
      digitalWrite(leftSignal, leftLedState);
    };
  }
  else {
    // if the start button isn't pushed, make sure the left LEDs are off
    //***********************************************************************************************************************
    // insert your own code here to turn the left LEDs off
    //***********************************************************************************************************************
  };
}
//_________________________________________________________________________________________________________________________________ 
