#include <Servo.h>
// servo value 106 is the center for old robot
// servo value 86 is the center for new robot


Servo steer;

int pos = 0;
unsigned char speed;

void setup() {
  steer.attach(14);
  steer.write(160);

  pinMode(23, OUTPUT); //int2 //in2A
  pinMode(24, OUTPUT); //int1 //in2B
  pinMode(21, OUTPUT); //int4 //in1A
  pinMode(22, OUTPUT); //int3  //in1B

   analogWrite(21, 255);
  analogWrite(22, 255);
  digitalWrite(23, 1);
  digitalWrite(24, 255);

  speed = 200;
}
// inputs 238-255 are perceived as the same as 255 - therefore inducing brake mode (when the other pin is 255) even though the values aren't exactly the same
//

void loop() {

  for (int i = 100; i < 200; i++) {
    analogWrite(21, speed - i);
    delay(5);
  }

  for (int i = 200; i >= 100; i--) {
    analogWrite(21, speed - i);
    delay(5);
  }
}