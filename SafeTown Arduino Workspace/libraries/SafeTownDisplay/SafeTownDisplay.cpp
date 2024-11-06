/*
  SafeTownDisplay.cpp - Temporary library for displaying system values (based on DisplayOurValues)
  Created by Benjamin Kinard, October 1, 2024.
  Released into the public domain.
*/

// libraries and inclusions
#include "Arduino.h"
#include "SafeTownDisplay.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// variables and instances
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels (set to 32 for 4 lines, 64 for 8 lines)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 adaSSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//NOTE: Size 1 font seems to be 7 pixels tall

// Constructor
SafeTownDisplay::SafeTownDisplay()
{
  numLines = SCREEN_HEIGHT/8;
}

// setup function. Must be run in any sketch that writes to the OLED - taken from Adafruit
void SafeTownDisplay::setup() {
  Serial.begin(115200);
  if(!adaSSD1306.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  adaSSD1306.clearDisplay();
  menu.SetDisplay(adaSSD1306);
  menuSetup();
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

void SafeTownDisplay::menuSetup() {
  menu = MenuItem("MAIN");
  menu.addSubMenuItem("1. Item #1");
  menu.addSubMenuItem("2. Item #2");
  menu.addSubMenuItem("3. Item #3");
  menu.addSubMenuItem("4. Item #4");
  menu.addSubMenuItem("5. Item #5");
}

void SafeTownDisplay::displayMenu() {
  String menuTitle = currentMenu->getContent();

  adaSSD1306.clearDisplay();
  adaSSD1306.setTextSize(1);
  adaSSD1306.setTextColor(SSD1306_WHITE);
  adaSSD1306.setCursor(0, 0);
  adaSSD1306.println(currentMenu->getContent());

  int firstItem = 0;
  int lastItem = firstItem + numLines - 2;
  
  for (int i = firstItem; i <= lastItem; i++) {
    adaSSD1306.setTextColor(SSD1306_WHITE);
    if (i == currentIndex) {
      adaSSD1306.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
    }
    adaSSD1306.println((currentMenu->getSubMenuItemContent(i)).c_str());
  }
  adaSSD1306.display();
}