/*
  SafeTownDisplay.cpp - Temporary library for displaying system values (based on DisplayOurValues)
  Created by Benjamin Kinard, October 1, 2024.
  Released into the public domain.
*/

// libraries and inclusions
#include "Arduino.h"
#include "SafeTownDisplay.h"
#include "MenuItem.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// variables and instances
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 adaSSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//NOTE: Size 1 font seems to be 7 pixels tall

// Constructor
SafeTownDisplay::SafeTownDisplay()
{

}

// setup function. Must be run in any sketch that writes to the OLED - taken from Adafruit
void SafeTownDisplay::setup() {
  Serial.begin(9600);
  if(!adaSSD1306.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
}

void SafeTownDisplay::loop() {
  adaSSD1306.clearDisplay();
  adaSSD1306.setTextSize(1);
  adaSSD1306.setTextColor(SSD1306_WHITE);
  adaSSD1306.setCursor(0, 0);
  adaSSD1306.println("Hello, World! Library");
  adaSSD1306.println(c);
  adaSSD1306.display();
}

void SafeTownDisplay::select() {
  menu.select();
}