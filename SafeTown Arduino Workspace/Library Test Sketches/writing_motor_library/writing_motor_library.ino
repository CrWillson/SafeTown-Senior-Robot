#include <Motor.h>

Motor left(6, 7);
Motor right(9, 8);

void setup() {
  left.begin();
  right.begin();
}

void loop() {
  left.forward(250);
  right.forward(250);
}