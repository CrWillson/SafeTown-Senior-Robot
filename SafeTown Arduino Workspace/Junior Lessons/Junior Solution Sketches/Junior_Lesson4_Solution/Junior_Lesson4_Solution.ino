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
const int motor1APin = 21;
const int motor1BPin = 22; 
const int motor2APin = 23;
const int motor2BPin = 24;
const int servoPin = 14;
int wait = 750; // this can be whatever value the student desires, within reason (e.g. anything less than 300 is probably too low, and anything more than 1000 might be too long)
int center = 90; // use the center value for your servo that you found in Lesson 2
//***********************************************************************************************************************
Servo steer = Servo(); // create an instance of the Servo library to control the steering servo
//_________________________________________________________________________________________________________________________________

// MAIN FUNCTIONS
void setup() {
  // put your setup code here, to run once:
  
  // set the modes of the GPIO pins you will use
  //***********************************************************************************************************************
  pinMode(motor1APin, OUTPUT);
  pinMode(motor1BPin, OUTPUT);
  pinMode(motor2APin, OUTPUT);
  pinMode(motor2BPin, OUTPUT);
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
  /**
  * Note: Students may also use analogWrite() here. digitalWrite() is acceptable for Section 2.1 
  * because the values involved are only HIGH and LOW, nothing in between.
  * It is also acceptable for the students to command an intermediate speed in driving forward
  * or backward. It is also acceptable if the speeds commanded for driving forward and backward
  * are not the same.
  */
  digitalWrite(motor1APin, HIGH); // for forward: A = H & B = L
  digitalWrite(motor1BPin, LOW);
  digitalWrite(motor2APin, HIGH); 
  digitalWrite(motor2BPin, LOW);
  // wait some amount of time (to allow the robot to drive forward for a little bit)
  delay(wait);
  // brake with both motors
  digitalWrite(motor1APin, HIGH); // to brake: A = H & B = H
  digitalWrite(motor1BPin, HIGH);
  digitalWrite(motor2APin, HIGH);
  digitalWrite(motor2BPin, HIGH);
  // wait some amount of time
  delay(wait);
  // drive backward with both motors
  digitalWrite(motor1APin, LOW); // for reverse: A = L & B = H
  digitalWrite(motor1BPin, HIGH);
  digitalWrite(motor2APin, LOW);
  digitalWrite(motor2BPin, HIGH);
  // wait some amount of time
  delay(wait);
  // brake with both motors
  digitalWrite(motor1APin, HIGH);
  digitalWrite(motor1BPin, HIGH);
  digitalWrite(motor2APin, HIGH);
  digitalWrite(motor2BPin, HIGH);
  // wait some amount of time
  delay(wait);
  //***********************************************************************************************************************

  // Section 2.2
  // IMPORTANT NOTE: Comment out Section 2.2 code when programming for Section 2.1.
  // make the robot drive in a medium-speed figure 8
  //***********************************************************************************************************************
  // turn the servo right and drive forward for about 1 full circle
  /**
  * Note: analogWrite() is required in this section for the intermediate speed commands.
  * For the pins that aren't receiving a PWM signal, using digitalWrite() is acceptable, 
  * but it is easier and more consistent to use only analogWrite(). The number the students
  * write to the servo may differ here. This is fine. The PWM speeds and delay times may 
  * also differ, which is fine. The goal is simply to get the robot to turn in an approximate
  * figure 8 at an intermediate speed. As long as they accomplish that, they have completed the task.
  */
  // steer.write(center + 70);
  // analogWrite(motor1APin, 100); // for forward: A = H & B = L
  // analogWrite(motor1BPin, HIGH);
  // analogWrite(motor2APin, 150); 
  // analogWrite(motor2BPin, HIGH);
  // // wait some amount of time
  // delay(3.4*wait);
  // // turn the servo left and drive forward for about 1 full circle
  // steer.write(center - 70);
  // analogWrite(motor1APin, 150); // for forward: A = H & B = L
  // analogWrite(motor1BPin, HIGH);
  // analogWrite(motor2APin, 100); 
  // analogWrite(motor2BPin, HIGH);
  // // wait for some amount of time
  // delay(3.4*wait);
  //***********************************************************************************************************************
}
//_________________________________________________________________________________________________________________________________