// INSTALL arduino-pico BOARD CORE
// https://arduino-pico.readthedocs.io/en/latest/install.html 

void setup() {
  pinMode(22, OUTPUT);

}

void loop() {
  digitalWrite(22, HIGH);
  delay(500);
  digitalWrite(22, LOW);
  delay(500);
}

void setup1() {
  pinMode(26, OUTPUT);
}

void loop1() {
  for (int i = 0; i < 255; i++) {
    analogWrite(26, i);
    delay(5);
  }
  
}
