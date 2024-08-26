#include <Servo.h>

#define STOP 2
#define START 3
#define DISP_SDA 4
#define DISP_SCL 5
#define LEFT_A 6
#define LEFT_B 7
#define RIGHT_A 8
#define RIGHT_B 9
#define RED 10
#define GREEN 11
#define YELLOW 12
#define BUZZER 13
#define SERVO 14
#define ENC_A 15
#define ENC_B 16
#define ENC_S 17
#define BRAKE 23
#define TURN_L 24
#define TURN_R 25 //also BUILTIN_LED
#define IR_F 26
#define IR_I 27
#define IR_O 28
#define IR_D 29

Servo servo;
int operation = 0;

void setup() {
  servo.attach(SERVO);
  pinMode(STOP, INPUT);
  pinMode(START, INPUT);
  pinMode(LEFT_A, OUTPUT);
  pinMode(LEFT_B, OUTPUT);
  pinMode(RIGHT_B, OUTPUT);
  pinMode(RIGHT_A, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(ENC_A, INPUT);
  pinMode(ENC_B, INPUT);
  pinMode(ENC_S, INPUT);
  pinMode(BRAKE, OUTPUT);
  pinMode(TURN_L, OUTPUT);
  pinMode(TURN_R, OUTPUT);
  pinMode(IR_F, OUTPUT);
  pinMode(IR_I, OUTPUT);
  pinMode(IR_O, OUTPUT);
  pinMode(IR_D, OUTPUT);
}

void loop() {
}
