/** Junior Lesson 3 Solution Sketch: IR Sensors, Analog-to-Digital Conversion, and Serial Communication
* This sketch shows how to output an ADC IR sensor value to the Serial Monitor and how to
* plot multiple ADC IR sensor values with the Serial Plotter.
*/

// LIBRARIES & INCLUSIONS
#include "Arduino.h"
//_________________________________________________________________________________________________________________________________

// CONSTANTS & CLASS INSTANTIATIONS
// these hold the GPIO pin values of the IR sensors
//***********************************************************************************************************************
const int downIrPin = 29; // THESE ARE OLD PIN NUMBERS SO THEY WILL NEED UPDATED WHEN THE NEW PCBs ARRIVE
const int frontIrPin = 26; 
const int outerIrPin = 27;
const int innerIrPin = 28;
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

  // Sections 2 & 3 (section 3 simply prints the other two IR sensors' values to the display)
  //***********************************************************************************************************************
  int downIrValue = analogRead(downIrPin);
  int frontIrValue = analogRead(frontIrPin);
  int outerIrValue = analogRead(outerIrPin);
  int innerIrValue = analogRead(innerIrPin);

  Serial.print("Down_IR_Value:");
  Serial.print(downIrValue);
  Serial.print(",    Front_IR_Value:");
  Serial.print(frontIrValue);
  Serial.print(",    Outer_IR_Value:");
  Serial.print(outerIrValue);
  Serial.print(",    Inner_IR_Value:");
  Serial.println(innerIrValue);
  //***********************************************************************************************************************
}
//_________________________________________________________________________________________________________________________________ 
