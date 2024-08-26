#include <Servo.h>

// servo value 106 is the center


Servo servo;

#define SERVO 14

int center = 90;
int range = 60;

void setup() {
  servo.attach(SERVO);
  servo.write(center - range);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  for (int i = -range; i < range; i++) {
    servo.write(center + i);
    delay(5);
  }
  digitalWrite(LED_BUILTIN, LOW); 
  for (int i = range; i >= -range; i--) {
    servo.write(center + i);
    delay(5);
  }
}
