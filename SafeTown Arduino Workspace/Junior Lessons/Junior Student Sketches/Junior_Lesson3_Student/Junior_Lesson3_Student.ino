/** Junior Lesson 3 Student Sketch: IR Sensors, Analog-to-Digital Conversion, and Serial Communication
* This sketch shows how to output an ADC IR sensor value to the Serial Monitor and how to
* plot multiple ADC IR sensor values with the Serial Plotter.
*/

// LIBRARIES & INCLUSIONS
#include "Arduino.h"
//_________________________________________________________________________________________________________________________________

// CONSTANTS & CLASS INSTANTIATIONS
// these hold the GPIO pin values of the IR sensors
//***********************************************************************************************************************
const int downIrPin = /** insert the down IR sensor GPIO pin number here*/ ;
const int frontIrPin = /** insert the front IR sensor GPIO pin number here*/ ; 
const int outerIrPin = /** insert the outer IR sensor GPIO pin number here*/ ;
const int innerIrPin = /** insert the inner IR sensor GPIO pin number here*/ ;
//***********************************************************************************************************************
//_________________________________________________________________________________________________________________________________

// MAIN FUNCTIONS
void setup() {
  // put your setup code here, to run once:

  // set the baud rate of serial communication
  Serial.begin(9600);

  // set the modes of the GPIO pins you will use
  pinMode(downIrPin, INPUT);
  pinMode(frontIrPin, INPUT);
  pinMode(outerIrPin, INPUT);
  pinMode(innerIrPin, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

  // Sections 2 & 3
  //***********************************************************************************************************************
  // insert your own code here to read the IR sensors and print them to the Serial Monitor/Plotter
  //***********************************************************************************************************************
}
//_________________________________________________________________________________________________________________________________ 
