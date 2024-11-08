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
  }
  encoderChange = !encoderChange;
}

// Encoder right turn
void ENC_A_GO() {
  if (encoderChange) {
  }
  encoderChange = !encoderChange;
}

// Encoder press
void ENC_S_GO() {
  encoderChange = false;
  display.select();
}

void setup() {
  // Serial.begin(9600);
  // pinMode(in_ir_pin, INPUT);
  // pinMode(out_ir_pin, INPUT);
  // pinMode(front_ir_pin, INPUT);
  // pinMode(down_ir_pin, INPUT);

  // // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  // if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
  //   Serial.println(F("SSD1306 allocation failed"));
  //   for(;;); // Don't proceed, loop forever
  // }
  pinMode(ENC_B, INPUT);
  pinMode(ENC_A, INPUT);
  pinMode(ENC_S, INPUT);
  attachInterrupt(digitalPinToInterrupt(ENC_B), ENC_B_GO, RISING);
  attachInterrupt(digitalPinToInterrupt(ENC_A), ENC_A_GO, RISING);
  attachInterrupt(digitalPinToInterrupt(ENC_S), ENC_S_GO, RISING);
  display.setup();
  // display.displayMenu();
}

void loop() {
  // display.clearDisplay();
  // display.setTextSize(1);      // Normal 1:1 pixel scale
  // display.setTextColor(SSD1306_WHITE); // Draw white text
  // display.setCursor(0, 0);   
  // display.print("Down IR: ");
	// display.println(analogRead(down_ir_pin));
	// display.print("Front IR: ");
	// display.println(analogRead(front_ir_pin));
	// display.print("Inner Left IR: ");
	// display.println(analogRead(in_ir_pin));
	// display.print("Outer Left IR: ");
	// display.println(analogRead(out_ir_pin));
  // display.display();
  // delay(100);
  // display.loop();
  Serial.println(millis());
  // display.setup();
  display.displayMenu();
  delay(1000);
}
