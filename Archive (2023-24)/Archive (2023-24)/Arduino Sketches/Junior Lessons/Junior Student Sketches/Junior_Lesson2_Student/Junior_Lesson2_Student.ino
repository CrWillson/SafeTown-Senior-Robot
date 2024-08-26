/** Junior Lesson 2 Solution Sketch: PWM and Servos
* This sketch shows how to use PWM to dim an LED and to control the position of a servo.
*/

// LIBRARIES & INCLUSIONS
#include <Servo.h>
//_________________________________________________________________________________________________________________________________

// CONSTANTS & CLASS INSTANTIATIONS
int brightness = 0;  // how bright the LED is
int fadeAmount = 5;  // how many points to fade the LED by
Servo servo; // an instance of the servo class so we can use our servo
//***********************************************************************************************************************
int rightTurnPin = /** insert the right turn signal GPIO pin number here*/ ; 
int servoPin = /** insert the servo GPIO pin number here*/ ;
int center = /** insert the STOP GPIO pin number here*/ ; // this value is specific to your robot's servo. The first time you run this program, set this value as 90. Adjust it based on the results
int range = /** insert the STOP GPIO pin number here*/ ; // the degree range on either side of the servo's center. This is also an experimental value. Start with 60 the first time you run this program. Adjust it based on the results
//***********************************************************************************************************************
//_________________________________________________________________________________________________________________________________

// MAIN FUNCTIONS
void setup() {
  // put your setup code here, to run once:

  // set the mode of the right turn signal's GPIO pin
  pinMode(rightTurnPin, OUTPUT);

  // attach the servo pin to the servo instance, and initialize the servo with the position that commands it to point to the left
  //***********************************************************************************************************************
  // insert your own code here to attach the servo pin and initiatlize the servo's position
  //***********************************************************************************************************************
}

void loop() {
  // put your main code here, to run repeatedly:

  // Section 1
  // make the LED dim down and then dim up
  // IMPORTANT NOTE: Comment out the following code when programming for Section 2.
  //***********************************************************************************************************************
  // set the brightness of the right turn signal

  // change the brightness for next time through the loop
  brightness = brightness + fadeAmount;

  // reverse the direction of the fading at the ends of the fade
  if (brightness <= /** insert brightness condition here for when the LED has dimmed all the way off*/ || brightness >= /** insert brightness condition here for when the LED has brightened all the way*/) {
    fadeAmount = -fadeAmount;
  }

  // wait for 30 milliseconds to see the dimming effect
  //***********************************************************************************************************************

  // Section 2
  // IMPORTANT NOTE: Comment out the following code when programming for Section 1.
  //***********************************************************************************************************************
  // move the servo all the way to the right

  // move the servo all the way to the left
  //***********************************************************************************************************************

}
//_________________________________________________________________________________________________________________________________
