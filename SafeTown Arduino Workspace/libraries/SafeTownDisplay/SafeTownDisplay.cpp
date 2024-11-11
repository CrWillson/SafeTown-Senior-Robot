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
  // menu.SetDisplay(adaSSD1306);
  menuSetup();
}

void SafeTownDisplay::displayIRValues() {
  // clear the OLED buffer
	adaSSD1306.clearDisplay();
	
	// choose text settings
	adaSSD1306.setTextSize(1);         
	adaSSD1306.setTextColor(SSD1306_WHITE); // Draw white text    
	adaSSD1306.setCursor(0,0); 
	
	// read the IR sensors
	int down = analogRead(down_ir_pin);
	int front = analogRead(front_ir_pin);
	int inner = analogRead(in_ir_pin);
	int outer = analogRead(out_ir_pin);
	
	// post the IR ADC values to the OLED buffer
	adaSSD1306.print("Down IR: ");
	adaSSD1306.println(down);
	adaSSD1306.print("Front IR: ");
	adaSSD1306.println(front);
	adaSSD1306.print("Inner Left IR: ");
	adaSSD1306.println(inner);
	adaSSD1306.print("Outer Left IR: ");
	adaSSD1306.println(outer);

	// post the OLED buffer (which now holds the IR ADC values) to the OLED (write to the screen)
	adaSSD1306.display();
}

void SafeTownDisplay::menuSetup() {
  menu = MenuItem("MAIN");
  menu.addSubMenuItem("View IR Sensor Values");
  menu.getSubMenuItem(0)->setAction(displayIRValues);
  menu.addSubMenuItem("Item #2");
  menu.getSubMenuItem(1)->addSubMenuItem("Item #2.1");
  menu.getSubMenuItem(1)->getSubMenuItem(1)->addSubMenuItem("Item #2.1.1");
  menu.getSubMenuItem(1)->getSubMenuItem(1)->addSubMenuItem("Item #2.1.2");
  menu.getSubMenuItem(1)->getSubMenuItem(1)->addSubMenuItem("Item #2.1.3");
  menu.getSubMenuItem(1)->getSubMenuItem(1)->getSubMenuItem(3)->addSubMenuItem("Item #2.1.3.1");
  menu.getSubMenuItem(1)->getSubMenuItem(1)->getSubMenuItem(3)->addSubMenuItem("Item #2.1.3.2");
  menu.getSubMenuItem(1)->getSubMenuItem(1)->getSubMenuItem(3)->addSubMenuItem("Item #2.1.3.3");
  menu.getSubMenuItem(1)->getSubMenuItem(1)->getSubMenuItem(3)->addSubMenuItem("Item #2.1.3.4");
  menu.getSubMenuItem(1)->getSubMenuItem(1)->getSubMenuItem(3)->addSubMenuItem("Item #2.1.3.5");
  menu.getSubMenuItem(1)->addSubMenuItem("Item #2.2");
  menu.getSubMenuItem(1)->addSubMenuItem("Item #2.3");
  menu.getSubMenuItem(1)->addSubMenuItem("Item #2.4");
  menu.getSubMenuItem(1)->addSubMenuItem("Item #2.5");
  menu.getSubMenuItem(1)->addSubMenuItem("Item #2.6");
  menu.addSubMenuItem("Item #3");
  menu.getSubMenuItem(2)->addSubMenuItem("Item #3.1");
  menu.getSubMenuItem(2)->addSubMenuItem("Item #3.2");
  menu.getSubMenuItem(2)->addSubMenuItem("Item #3.3");
  menu.getSubMenuItem(2)->addSubMenuItem("Item #3.4");
  menu.getSubMenuItem(2)->addSubMenuItem("Item #3.5");
  menu.getSubMenuItem(2)->addSubMenuItem("Item #3.6");
  menu.addSubMenuItem("Item #4");
  menu.getSubMenuItem(3)->addSubMenuItem("Item #4.1");
  menu.getSubMenuItem(3)->addSubMenuItem("Item #4.2");
  menu.getSubMenuItem(3)->addSubMenuItem("Item #4.3");
  menu.getSubMenuItem(3)->addSubMenuItem("Item #4.4");
  menu.getSubMenuItem(3)->addSubMenuItem("Item #4.5");
  menu.getSubMenuItem(3)->addSubMenuItem("Item #4.6");
  menu.addSubMenuItem("Item #5");
  menu.getSubMenuItem(4)->addSubMenuItem("Item #5.1");
  menu.getSubMenuItem(4)->addSubMenuItem("Item #5.2");
  menu.getSubMenuItem(4)->addSubMenuItem("Item #5.3");
  menu.getSubMenuItem(4)->addSubMenuItem("Item #5.4");
  menu.getSubMenuItem(4)->addSubMenuItem("Item #5.5");
  menu.getSubMenuItem(4)->addSubMenuItem("Item #5.6");
  menu.addSubMenuItem("Item #6");
  menu.addSubMenuItem("Item #7");
  menu.addSubMenuItem("Item #8");
  menu.addSubMenuItem("Item #9");
  menu.addSubMenuItem("Item #10");
}

void SafeTownDisplay::displayMenu() {
  if (!runSelectedAction) {
    String menuTitle = currentMenu->getContent();
    int numItems = currentMenu->getNumItems();

    adaSSD1306.clearDisplay();
    adaSSD1306.setTextSize(1);
    adaSSD1306.setTextColor(SSD1306_WHITE);
    adaSSD1306.setCursor((SCREEN_WIDTH - (currentMenu->getContent().length() * 6)) / 2, 0); // center the menu name
    adaSSD1306.println(currentMenu->getContent());

    int center = (numLines/2) - 1;
    int firstItem = currentIndex - center;
    if (currentIndex < center || numItems < numLines) {
      firstItem = 0;
    } else if (currentIndex >= numItems - center) {
      firstItem = numItems - (numLines - 1);
    }
    int lastItem = firstItem + numLines - (center-1);
    if (lastItem >= numItems) {
      lastItem = numItems - 1;
    }
    
    for (int i = firstItem; i <= lastItem; i++) {
      adaSSD1306.setTextColor(SSD1306_WHITE);
      if (i == currentIndex) {
        adaSSD1306.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      }
      adaSSD1306.println(currentMenu->getSubMenuItemContent(i));
    }
    adaSSD1306.display();
  } else {
    currentMenu->getSubMenuItem(currentIndex)->doAction();
  }
}

void SafeTownDisplay::selectCurrentItem() {
  // Serial.println("Selected");
  if (!runSelectedAction) {
    // Serial.println("Not running action");
    MenuItem* selected = currentMenu->getSubMenuItem(currentIndex);
    if (selected->isSelectable()) {
      // Serial.println("Is selectable");
      currentMenu = selected;
      currentIndex = 1;
    } else if (selected->isBack()) {
      // Serial.println("Is back");
      currentIndex = currentMenu->getIndex();
      currentMenu = currentMenu->getParent();
    } else if (selected->hasAction()) {
      // Serial.println("Has action");
      runSelectedAction = true;
    }
  } else {
    // Serial.println("Running action");
    runSelectedAction = false;
  }
}

void SafeTownDisplay::decrementMenuIndex() {
  if (!runSelectedAction) {
    int numItems = currentMenu->getNumItems();
    currentIndex = (currentIndex + numItems - 1) % numItems;
    Serial.print("currentIndex: ");
    Serial.println(currentIndex);
  }
}

void SafeTownDisplay::incrementMenuIndex() {
  if (!runSelectedAction) {
    int numItems = currentMenu->getNumItems();
    currentIndex = (currentIndex + 1) % numItems;
    Serial.print("currentIndex: ");
    Serial.println(currentIndex);
  }
}