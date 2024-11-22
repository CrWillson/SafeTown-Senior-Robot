
/*void motor_speed(int pin, unsigned char speed) {
  Digital
}*/
unsigned char t;
void setup() {
  pinMode(23, OUTPUT); //int2 //in2A
  pinMode(24, OUTPUT); //int1 //in2B
  pinMode(21, OUTPUT); //int4 //in1A
  pinMode(22, OUTPUT); //int3  //in1B

  //digitalWrite(23, HIGH); // To guarantee forward mode
  //digitalWrite(21, HIGH); // To guarantee forward mode
  //t = 1000;
}

void loop() {
  digitalWrite(24, LOW); 
  digitalWrite(22, LOW); 
  digitalWrite(21, HIGH); // to go in reverse
  digitalWrite(23, HIGH); 
  delay(1000);
  digitalWrite(21, HIGH); // to brake
  digitalWrite(23, HIGH); 
  digitalWrite(24, HIGH); 
  digitalWrite(22, HIGH); 
  delay(1000);
  digitalWrite(24, HIGH); 
  digitalWrite(22, HIGH); 
  digitalWrite(21, LOW); // to go in reverse
  digitalWrite(23, LOW); // to go in reverse
  delay(1000);
  digitalWrite(21, HIGH); // to brake
  digitalWrite(23, HIGH); 
  digitalWrite(24, HIGH); 
  digitalWrite(22, HIGH); 
  delay(1000);

  
}
