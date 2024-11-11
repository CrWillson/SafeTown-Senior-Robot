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

// #define SCREEN_WIDTH 128 // OLED display width, in pixels
// #define SCREEN_HEIGHT 64 // OLED display height, in pixels

// #define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
// #define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
// Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// const int in_ir_pin = 27;
// const int out_ir_pin = 28;
// const int front_ir_pin = 26;
// const int down_ir_pin = 29;

#define ENC_B    20
#define ENC_A    18
#define ENC_S    19

SafeTownDisplay display = SafeTownDisplay(); // initialize display
// Encoder variables
bool encoderChange = false; // track encoder changes

// Encoder left turn
void ENC_B_GO() {
  if (encoderChange) {
    display.decrementMenuIndex();
  }
  encoderChange = !encoderChange;
}

// Encoder right turn
void ENC_A_GO() {
  if (encoderChange) {
    display.incrementMenuIndex();
  }
  encoderChange = !encoderChange;
}

// Encoder press
void ENC_S_GO() {
  encoderChange = false;
  display.selectCurrentItem();
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
