/** Junior Lesson 4 Student Sketch: Motors and Motor Control
* This sketch shows how to drive the motors using the basic modes
* (forward, reverse, brake) and how to drive the motors using the 
* PWM modes while also tying in a little bit of steering.
*/

// LIBRARIES & INCLUSIONS
#include <Servo.h>
//_________________________________________________________________________________________________________________________________

// CONSTANTS & CLASS INSTANTIATIONS
// these hold the GPIO pin values for the motors and servo & the amount of time to wait between giving new commands to the motor
//***********************************************************************************************************************
const int motor1APin = /** insert the motor 1 GPIO pin A number here*/ ;
const int motor1BPin = /** insert the motor 1 GPIO pin B number here*/ ; 
const int motor2APin = /** insert the motor 2 GPIO pin A number here*/ ;
const int motor2BPin = /** insert the motor 2 GPIO pin B number here*/ ;
const int servoPin = /** insert the servo GPIO pin number here*/ ;
int wait = /** insert the amount of time you want to wait between motor commands here*/ ;
int center = /** insert your servo's center value here*/ ; // use the center value for your servo that you found in Lesson 2
//***********************************************************************************************************************
Servo steer = Servo(); // create an instance of the Servo library to control the steering servo
//_________________________________________________________________________________________________________________________________

// MAIN FUNCTIONS
void setup() {
  // put your setup code here, to run once:
  
  // set the modes of the GPIO pins you will use
  //***********************************************************************************************************************
  // only the motor pins need modes
  //***********************************************************************************************************************

  // set up and initialize the servo with its neutral position (center)
  steer.attach(servoPin);
  steer.write(center);
}

void loop() {
  // put your main code here, to run repeatedly:

  // Section 2.1
  // IMPORTANT NOTE: Comment out Section 2.1 code when programming for Section 2.2.
  // make the robot drive forward, stop, then drive backward, and then stop
  //***********************************************************************************************************************
  // drive forward with both motors
  
  // wait some amount of time (to allow the robot to drive forward for a little bit)

  // brake with both motors
  
  // wait some amount of time

  // drive backward with both motors
 
  // wait some amount of time

  // brake with both motors

  // wait some amount of time

  //***********************************************************************************************************************

  // Section 2.2
  // IMPORTANT NOTE: Comment out Section 2.2 code when programming for Section 2.1.
  // make the robot drive in a medium-speed figure 8
  //***********************************************************************************************************************
  // turn the servo right and drive forward for about 1 full circle

  // wait some amount of time

  // turn the servo left and drive forward for about 1 full circle

  // wait for some amount of time

  //***********************************************************************************************************************
}
//_________________________________________________________________________________________________________________________________