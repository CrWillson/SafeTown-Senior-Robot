////////////////////////////////////////////////////////////////////////////////
// Filename: SafeTownDisplayTest.ino
// Author: SafeTown (2022-2024)
// Purpose: Used for testing the SafeTownDisplayLibrary
////////////////////////////////////////////////////////////////////////////////
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SafeTownDisplay.h>

#define ENC_B    20
#define ENC_A    18
#define ENC_S    19

SafeTownDisplay display = SafeTownDisplay(); // initialize display
// Encoder variables
bool encoderChange = false; // track encoder changes

// Encoder left turn
void ENC_B_GO() {
  if (encoderChange) {
    display.encoderLeft();
  }
  encoderChange = !encoderChange;
}

// Encoder right turn
void ENC_A_GO() {
  if (encoderChange) {
    display.encoderRight();
  }
  encoderChange = !encoderChange;
}

// Encoder press
void ENC_S_GO() {
  encoderChange = false;
  display.encoderPress();
}

void setup() {
  pinMode(ENC_B, INPUT);
  pinMode(ENC_A, INPUT);
  pinMode(ENC_S, INPUT);
  attachInterrupt(digitalPinToInterrupt(ENC_B), ENC_B_GO, RISING);
  attachInterrupt(digitalPinToInterrupt(ENC_A), ENC_A_GO, RISING);
  attachInterrupt(digitalPinToInterrupt(ENC_S), ENC_S_GO, RISING);
  display.setup();
}

void loop() {
  display.displayMenu();
  delay(50);
}
