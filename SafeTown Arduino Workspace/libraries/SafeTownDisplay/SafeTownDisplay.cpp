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
SafeTownDisplay::SafeTownDisplay() {
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
  adaSSD1306.clearDisplay();
  adaSSD1306.setTextSize(1);         
  adaSSD1306.setTextColor(SSD1306_WHITE); // Draw white text    
  adaSSD1306.setCursor(0,0);
}

void SafeTownDisplay::displayIRValues(SafeTownDisplay* displayLibInst) {
  if (displayLibInst->updateScreen) {
    // clear the OLED buffer
    adaSSD1306.clearDisplay();
    
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
}

void SafeTownDisplay::printToScreen(String s)
{
  adaSSD1306.clearDisplay();
  adaSSD1306.setTextSize(1);         
  adaSSD1306.setTextColor(SSD1306_WHITE); // Draw white text    
  adaSSD1306.setCursor(0,0);
  adaSSD1306.println(s);
  adaSSD1306.display();
}

void SafeTownDisplay::collectData(SafeTownDisplay* displayLibInst) {
  if (displayLibInst->resetData) {
    displayLibInst->sampleRate = displayLibInst->settingSampleRate->getIntData();
    displayLibInst->sampleTime = displayLibInst->settingSampleTime->getIntData();
    displayLibInst->sampleInterval = 1000.0/displayLibInst->sampleRate;
    displayLibInst->samplePointer = 0;
    displayLibInst->sampleTargetTime = 0;
    displayLibInst->samplesToCollect = displayLibInst->sampleRate*displayLibInst->sampleTime;
    displayLibInst->resetData = false;
    displayLibInst->timeOffset = millis();
  }

  // collect data and store in array
  double time = millis() - displayLibInst->timeOffset;
  if (time >= displayLibInst->sampleTargetTime + displayLibInst->sampleInterval) {
    int currentSample = displayLibInst->samplePointer;
    displayLibInst->downSamples[currentSample] = analogRead(down_ir_pin);
    displayLibInst->frontSamples[currentSample] = analogRead(front_ir_pin);
    displayLibInst->innerLeftSamples[currentSample] = analogRead(in_ir_pin);
    displayLibInst->outerLeftSamples[currentSample] = analogRead(out_ir_pin);
    displayLibInst->times[currentSample] = time;
    Serial.println("Collecting data: " + String(currentSample+1) + "/" + String(displayLibInst->samplesToCollect));
    displayLibInst->samplePointer++;
    displayLibInst->sampleTargetTime += displayLibInst->sampleInterval;
    if (displayLibInst->samplePointer >= displayLibInst->samplesToCollect) {
      displayLibInst->resetData = true;
      displayLibInst->runSelectedAction = false;
    }

    // output result
    adaSSD1306.clearDisplay();
    adaSSD1306.setTextSize(1);         
    adaSSD1306.setTextColor(SSD1306_WHITE); // Draw white text    
    adaSSD1306.setCursor(0,0);
    adaSSD1306.println("Collecting Data");
    adaSSD1306.print(displayLibInst->samplePointer);
    adaSSD1306.print("/");
    adaSSD1306.println(displayLibInst->samplesToCollect);
    adaSSD1306.display();
  }
}

void SafeTownDisplay::outputData(SafeTownDisplay* displayLibInst) {
  
}

void SafeTownDisplay::toggleData(SafeTownDisplay* displayLibInst) {
  displayLibInst->currentMenu->getSubMenuItem(displayLibInst->currentIndex)->toggleBoolData();
  displayLibInst->runSelectedAction = false;
}

void SafeTownDisplay::changeIntData(SafeTownDisplay* displayLibInst) {
  displayLibInst->encoderAction = true;
}

void SafeTownDisplay::menuSetup() {
  // MAIN MENU
  mainMenu = MenuItem("MAIN MENU");

  // MAIN MENU > View IR Sensor Values
  mainMenu.addSubMenuItem("View IR Sensor Values");
  mainMenu.getSubMenuItem(0)->setAction(displayIRValues);

  // MAIN MENU > Data Collection
  mainMenu.addSubMenuItem("Data Collection");

  // MAIN MENU > Data Collection > Collect Data
  mainMenu.getSubMenuItem(1)->addSubMenuItem("Collect Data");
  mainMenu.getSubMenuItem(1)->getSubMenuItem(1)->setAction(collectData);

  // MAIN MENU > Data Collection > Output Data
  mainMenu.getSubMenuItem(1)->addSubMenuItem("Output Data");
  mainMenu.getSubMenuItem(1)->getSubMenuItem(2)->setAction(outputData);

  // MAIN MENU > Data Collection > Data Settings
  mainMenu.getSubMenuItem(1)->addSubMenuItem("Data Settings");

  // MAIN MENU > Data Collection > Data Settings > Sample Rate
  mainMenu.getSubMenuItem(1)->getSubMenuItem(3)->addSubMenuItem("Sample Rate (Hz)", 20, 1, 100);
  settingSampleRate = mainMenu.getSubMenuItem(1)->getSubMenuItem(3)->getSubMenuItem(1);
  settingSampleRate->setAction(changeIntData);

  // MAIN MENU > Data Collection > Data Settings > Sample Time
  mainMenu.getSubMenuItem(1)->getSubMenuItem(3)->addSubMenuItem("Sample Time (s)", 5, 1, 10);
  settingSampleTime = mainMenu.getSubMenuItem(1)->getSubMenuItem(3)->getSubMenuItem(2);
  settingSampleTime->setAction(changeIntData);

  // MAIN MENU > Data Collection > Data Settings > Down IR
  mainMenu.getSubMenuItem(1)->getSubMenuItem(3)->addSubMenuItem("Down IR", true);
  settingDownIR = mainMenu.getSubMenuItem(1)->getSubMenuItem(3)->getSubMenuItem(3);
  settingDownIR->setAction(toggleData);

  // MAIN MENU > Data Collection > Data Settings > Front IR
  mainMenu.getSubMenuItem(1)->getSubMenuItem(3)->addSubMenuItem("Front IR", true);
  settingFrontIR = mainMenu.getSubMenuItem(1)->getSubMenuItem(3)->getSubMenuItem(4);
  settingFrontIR->setAction(toggleData);

  // MAIN MENU > Data Collection > Data Settings > Inner Left IR
  mainMenu.getSubMenuItem(1)->getSubMenuItem(3)->addSubMenuItem("Inner Left IR", true);
  settingInnerLeftIR = mainMenu.getSubMenuItem(1)->getSubMenuItem(3)->getSubMenuItem(5);
  settingInnerLeftIR->setAction(toggleData);

  // MAIN MENU > Data Collection > Data Settings > Outer Left IR
  mainMenu.getSubMenuItem(1)->getSubMenuItem(3)->addSubMenuItem("Outer Left IR", true);
  settingOuterLeftIR = mainMenu.getSubMenuItem(1)->getSubMenuItem(3)->getSubMenuItem(6);
  settingOuterLeftIR->setAction(toggleData);
}

void SafeTownDisplay::displayMenu(bool updateScreen) {
  this->updateScreen = updateScreen;
  if (updateScreen && (!runSelectedAction || encoderAction)) {
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
  } else if (runSelectedAction) {
    currentMenu->getSubMenuItem(currentIndex)->doAction(this);
  }
}

void SafeTownDisplay::encoderPress() {
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
    encoderAction = false;
    resetData = true;
  }
}

void SafeTownDisplay::encoderLeft() {
  if (!runSelectedAction) {
    int numItems = currentMenu->getNumItems();
    currentIndex = (currentIndex + numItems - 1) % numItems;
    Serial.print("currentIndex: ");
    Serial.println(currentIndex);
  } else if (encoderAction) {
    currentMenu->getSubMenuItem(currentIndex)->decrementIntData();
  }
}

void SafeTownDisplay::encoderRight() {
  if (!runSelectedAction) {
    int numItems = currentMenu->getNumItems();
    currentIndex = (currentIndex + 1) % numItems;
    Serial.print("currentIndex: ");
    Serial.println(currentIndex);
  } else if (encoderAction) {
    currentMenu->getSubMenuItem(currentIndex)->incrementIntData();
  }
}