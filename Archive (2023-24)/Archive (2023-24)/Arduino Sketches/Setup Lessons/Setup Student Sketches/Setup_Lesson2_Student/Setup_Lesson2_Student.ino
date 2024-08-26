/** Setup Lesson 2 Student Sketch: Writing a Sketch, and Programming the Junior Robot
* This sketch blinks the red LED on the Junior PCB as a first sketch and 
* to ensure the student can program the RP2040.
*/

// LIBRARIES & INCLUSIONS
#include "Arduino.h"
//_________________________________________________________________________________________________________________________________

// CONSTANTS & CLASS INSTANTIATIONS
// this variable holds the GPIO pin value of the red LED on the Junior PCB
//***********************************************************************************************************************
const int  redLED = /** insert the red LED GPIO pin number here*/ ;
//***********************************************************************************************************************
//_________________________________________________________________________________________________________________________________

// MAIN FUNCTIONS
// the setup function runs once when you press reset or power the board.
void setup() {
  // put your setup code here, to run once:

  // initialize the red_LED GPIO pin as an output.
  pinMode(redLED, OUTPUT);
}

// the loop function runs over and over again forever.
void loop() {
  // put your main code here, to run repeatedly:

  // turn the red LED on, wait, and then turn it off
  //***********************************************************************************************************************
  // insert your own code here to turn the red LED on, wait, and then turn it off
  //***********************************************************************************************************************
}
//_________________________________________________________________________________________________________________________________ 