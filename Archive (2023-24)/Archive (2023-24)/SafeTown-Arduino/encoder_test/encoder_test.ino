#include "pio_encoder.h"

#define CLK 14
#define DT  15
#define SW  2
#define LED1 22
#define LED2 21
#define LED3 20

PioEncoder encoder(14);


long pos = -999;
long new_pos;

void setup() {
  encoder.begin();
  pinMode(SW, INPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  Serial.begin(115200);
  while(!Serial);
}

void loop() {
  new_pos = encoder.getCount()  >> 1;
  if (new_pos != pos) {
    digitalWrite(LED3, HIGH);
    Serial.println(String(new_pos) + "  |  " + String(pos));
    if (new_pos > pos) {
      digitalWrite(LED2, HIGH);
    } else {
      digitalWrite(LED2, LOW);
    }
    pos = new_pos;
  }
  if (digitalRead(SW)) {
    digitalWrite(LED1, HIGH);
    digitalWrite(LED3, LOW);
  } else {
    digitalWrite(LED1, LOW);
  }
  delay(10);
}
