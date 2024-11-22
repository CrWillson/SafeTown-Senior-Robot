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
int rightTurnPin = 25; // could also just use "LED_BUILTIN" in place of all instances of rightTurnPin, since LED_BUILTIN is attached to pin 25 by default
// THIS IS AN OLD PIN NUMBER SO IT WILL NEED UPDATED WHEN THE NEW PCBs ARRIVE
int servoPin = 14;
int center = 90; // THIS IS SPECIFIC TO OUR ONE ROBOT'S SERVO - FINDING THIS VALUE IS PART OF THE EXERCISE
int range = 50; // THIS IS TENTATIVE AND COULD BE DIFFERENT DEPENDING ON THE USER'S SERVO'S CENTER
//***********************************************************************************************************************
//_________________________________________________________________________________________________________________________________

// MAIN FUNCTIONS
void setup() {
  // put your setup code here, to run once:

  // set the mode of the GPIO pin you will use
  pinMode(rightTurnPin, OUTPUT);

  // attach the servo pin to the servo instance, and initialize the servo with the position that commands it to point to the left
  //***********************************************************************************************************************
  servo.attach(servoPin);
  servo.write(center - range); // when we first begin, set the servo's position all the way to the left
  //***********************************************************************************************************************
}

void loop() {
  // put your main code here, to run repeatedly:

  // Section 1
  // make the LED dim down and then dim up
  // IMPORTANT NOTE: Comment out the following code when programming for Section 2.
  //***********************************************************************************************************************
  // set the brightness of the right turn signal
  // analogWrite(rightTurnPin, brightness);

  // // change the brightness for next time through the loop
  // brightness = brightness + fadeAmount;

  // // reverse the direction of the fading at the ends of the fade
  // if (brightness <= 0 || brightness >= 255) {
  //   fadeAmount = -fadeAmount;
  // }

  // // wait for 30 milliseconds to see the dimming effect
  // delay(30);
  //***********************************************************************************************************************

  // Section 2
  // IMPORTANT NOTE: Comment out the following code when programming for Section 1.
  //***********************************************************************************************************************
  // move the servo all the way to the right
  for (int i = -range; i < range; i++) {
    servo.write(center + i);
    delay(5);
  }
  // move the servo all the way to the left
  for (int i = range; i >= -range; i--) {
    servo.write(center + i);
    delay(5);
  }
  //***********************************************************************************************************************

}
//_________________________________________________________________________________________________________________________________
