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
  // mainMenu.SetDisplay(adaSSD1306);
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
  mainMenu = MenuItem("MAIN MENU");
  mainMenu.addSubMenuItem("1. View IR Values");
  mainMenu.getSubMenuItem(0)->setAction(displayIRValues);
  mainMenu.addSubMenuItem("Item #2");
  mainMenu.getSubMenuItem(1)->addSubMenuItem("Item #2.1");
  mainMenu.getSubMenuItem(1)->getSubMenuItem(1)->addSubMenuItem("Item #2.1.1");
  mainMenu.getSubMenuItem(1)->getSubMenuItem(1)->addSubMenuItem("Item #2.1.2");
  mainMenu.getSubMenuItem(1)->getSubMenuItem(1)->addSubMenuItem("Item #2.1.3");
  mainMenu.getSubMenuItem(1)->getSubMenuItem(1)->getSubMenuItem(3)->addSubMenuItem("Item #2.1.3.1");
  mainMenu.getSubMenuItem(1)->getSubMenuItem(1)->getSubMenuItem(3)->addSubMenuItem("Item #2.1.3.2");
  mainMenu.getSubMenuItem(1)->getSubMenuItem(1)->getSubMenuItem(3)->addSubMenuItem("Item #2.1.3.3");
  mainMenu.getSubMenuItem(1)->getSubMenuItem(1)->getSubMenuItem(3)->addSubMenuItem("Item #2.1.3.4");
  mainMenu.getSubMenuItem(1)->getSubMenuItem(1)->getSubMenuItem(3)->addSubMenuItem("Item #2.1.3.5");
  mainMenu.getSubMenuItem(1)->addSubMenuItem("Item #2.2");
  mainMenu.getSubMenuItem(1)->addSubMenuItem("Item #2.3");
  mainMenu.getSubMenuItem(1)->addSubMenuItem("Item #2.4");
  mainMenu.getSubMenuItem(1)->addSubMenuItem("Item #2.5");
  mainMenu.getSubMenuItem(1)->addSubMenuItem("Item #2.6");
  mainMenu.addSubMenuItem("Item #3");
  mainMenu.getSubMenuItem(2)->addSubMenuItem("Item #3.1");
  mainMenu.getSubMenuItem(2)->addSubMenuItem("Item #3.2");
  mainMenu.getSubMenuItem(2)->addSubMenuItem("Item #3.3");
  mainMenu.getSubMenuItem(2)->addSubMenuItem("Item #3.4");
  mainMenu.getSubMenuItem(2)->addSubMenuItem("Item #3.5");
  mainMenu.getSubMenuItem(2)->addSubMenuItem("Item #3.6");
  mainMenu.addSubMenuItem("Item #4");
  mainMenu.getSubMenuItem(3)->addSubMenuItem("Item #4.1");
  mainMenu.getSubMenuItem(3)->addSubMenuItem("Item #4.2");
  mainMenu.getSubMenuItem(3)->addSubMenuItem("Item #4.3");
  mainMenu.getSubMenuItem(3)->addSubMenuItem("Item #4.4");
  mainMenu.getSubMenuItem(3)->addSubMenuItem("Item #4.5");
  mainMenu.getSubMenuItem(3)->addSubMenuItem("Item #4.6");
  mainMenu.addSubMenuItem("Item #5");
  mainMenu.getSubMenuItem(4)->addSubMenuItem("Item #5.1");
  mainMenu.getSubMenuItem(4)->addSubMenuItem("Item #5.2");
  mainMenu.getSubMenuItem(4)->addSubMenuItem("Item #5.3");
  mainMenu.getSubMenuItem(4)->addSubMenuItem("Item #5.4");
  mainMenu.getSubMenuItem(4)->addSubMenuItem("Item #5.5");
  mainMenu.getSubMenuItem(4)->addSubMenuItem("Item #5.6");
  mainMenu.addSubMenuItem("Item #6");
  mainMenu.addSubMenuItem("Item #7");
  mainMenu.addSubMenuItem("Item #8");
  mainMenu.addSubMenuItem("Item #9");
  mainMenu.addSubMenuItem("Item #10");
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
  if (!runSelectedAction) {
    MenuItem* selected = currentMenu->getSubMenuItem(currentIndex);
    if (selected->isParent()) {
      currentMenu = selected;
      currentIndex = 1;
    } else if (selected->isBack()) {
      currentIndex = currentMenu->getIndex();
      currentMenu = currentMenu->getParent();
    } else if (selected->hasAction()) {
      runSelectedAction = true;
    }
  } else {
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