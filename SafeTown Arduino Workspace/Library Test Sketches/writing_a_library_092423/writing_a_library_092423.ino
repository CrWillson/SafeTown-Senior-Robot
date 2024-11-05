#include <Morse.h>

Morse morse(3);

void setup() {
  morse.begin();
}

void loop() {
  {
  morse.dot(); morse.dot(); morse.dot();
  morse.dash(); morse.dash(); morse.dash();
  morse.dot(); morse.dot(); morse.dot();
  delay(3000);
}
}
