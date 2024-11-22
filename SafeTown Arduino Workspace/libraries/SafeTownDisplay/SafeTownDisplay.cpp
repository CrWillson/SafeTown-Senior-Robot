////////////////////////////////////////////////////////////////////////////////
// Filename: SafeTownDisplay.cpp
// Created: 10-1-2024
// Modified: 11-22-2024
// Author: Benjamin Kinard (SafeTown)
// Purpose: Handles all interactions with the OLED display for the SafeTown 
//          robot. Creates and displays a menu of MenuItem objects, and provides
//          capabilities to add child MenuItem objects or to pass function 
//          pointers in as an "action" for a MenuItem object.
// Instructions for adding new menu items:
// 1. In the setup() function, add a new MenuItem by calling addSubMenuItem() on 
//    the parent MenuItem.
// 2. If adding child elements, repeat #1 as necessary.
// 3. If adding an action, then create a function to be passed to the MenuItem 
//    object. Use the following function format:
//      void SafeTownDisplay::<function name>(SafeTownDisplay* displayLibInst) {}
// 4. NOTE: unless the action is to be terminated by pressing the encoder,
//    the action needs to set runSelectedAction to false to exit the action state.
// 5. Call setAction() on the object to have the action and pass it the function 
//    created in #3.
// 6. NOTE: if the action is to be a setting containing boolean or integer data,
//    then use toggleData() or changeIntData() instead of creating a custom action
//    function.
// 7. See existing menu items and action functions for usage examples.
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Setup and constructors
////////////////////////////////////////////////////////////////////////////////

// Include header file
#include "SafeTownDisplay.h"

// Display variables and instances
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels (set to 32 for 4 lines, 64 for 8 lines)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 adaSSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); // The display object

// Constructor (default)
SafeTownDisplay::SafeTownDisplay() {
  numLines = SCREEN_HEIGHT/8;
}

// Setup function
// Note: must be run once during the Arduino setup
void SafeTownDisplay::setup() {
  Serial.begin(115200);
  if(!adaSSD1306.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  menuSetup();
  clearDisplay();
}

////////////////////////////////////////////////////////////////////////////////
// Display and robot hardware helper functions
////////////////////////////////////////////////////////////////////////////////

// Clear the display and prepare to write text
void SafeTownDisplay::clearDisplay() {
  adaSSD1306.clearDisplay(); // reset the display
  adaSSD1306.setTextSize(1); // 7 pixels tall
  adaSSD1306.setTextColor(SSD1306_WHITE); // draw white text    
  adaSSD1306.setCursor(0,0); // start printing at the beginning of the first line
}

// Clear the screen and print a specified string
void SafeTownDisplay::printToScreen(String s)
{
  clearDisplay();
  adaSSD1306.println(s);
  adaSSD1306.display();
}

// Action to perform when the encoder is pressed
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

// Action to perform when the encoder is turned left
void SafeTownDisplay::encoderLeft() {
  if (!runSelectedAction) {
    int numItems = currentMenu->getNumItems();
    currentIndex = (currentIndex + numItems - 1) % numItems;
  } else if (encoderAction) {
    currentMenu->getSubMenuItem(currentIndex)->decrementIntData();
  }
}

// Action to perform when the encoder is turned right
void SafeTownDisplay::encoderRight() {
  if (!runSelectedAction) {
    int numItems = currentMenu->getNumItems();
    currentIndex = (currentIndex + 1) % numItems;
  } else if (encoderAction) {
    currentMenu->getSubMenuItem(currentIndex)->incrementIntData();
  }
}

////////////////////////////////////////////////////////////////////////////////
// Main menu setup and display
////////////////////////////////////////////////////////////////////////////////

// Define the menu structure and actions
// Note: must be run once during the Arduino setup
void SafeTownDisplay::menuSetup() {
  // MAIN MENU
  mainMenu = MenuItem("MAIN MENU");

  // MAIN MENU > View IR Sensor Values
  mainMenu.addSubMenuItem("View IR Sensor Values");
  mainMenu.getSubMenuItem(0)->setAction(displayIRValues);

  // MAIN MENU > Data Collection
  mainMenu.addSubMenuItem("Data Collection");
  MenuItem* dataCollection = mainMenu.getSubMenuItem(1);

  // MAIN MENU > Data Collection > Collect Data
  dataCollection->addSubMenuItem("Collect Data");
  dataCollection->getSubMenuItem(1)->setAction(collectData);

  // MAIN MENU > Data Collection > Output Data
  dataCollection->addSubMenuItem("Output Data");
  dataCollection->getSubMenuItem(2)->setAction(outputData);

  // MAIN MENU > Data Collection > Data Settings
  dataCollection->addSubMenuItem("Data Settings");

  // MAIN MENU > Data Collection > Data Settings > Sample Rate
  dataCollection->getSubMenuItem(3)->addSubMenuItem("Sample Rate (Hz)", 20, 1, 100, 5);
  settingSampleRate = dataCollection->getSubMenuItem(3)->getSubMenuItem(1);
  settingSampleRate->setAction(changeIntData);

  // MAIN MENU > Data Collection > Data Settings > Sample Time
  dataCollection->getSubMenuItem(3)->addSubMenuItem("Sample Time (s)", 5, 1, 10, 1);
  settingSampleTime = dataCollection->getSubMenuItem(3)->getSubMenuItem(2);
  settingSampleTime->setAction(changeIntData);

  // MAIN MENU > Data Collection > Data Settings > Down IR
  dataCollection->getSubMenuItem(3)->addSubMenuItem("Down IR", true);
  settingDownIR = dataCollection->getSubMenuItem(3)->getSubMenuItem(3);
  settingDownIR->setAction(toggleData);

  // MAIN MENU > Data Collection > Data Settings > Front IR
  dataCollection->getSubMenuItem(3)->addSubMenuItem("Front IR", true);
  settingFrontIR = dataCollection->getSubMenuItem(3)->getSubMenuItem(4);
  settingFrontIR->setAction(toggleData);

  // MAIN MENU > Data Collection > Data Settings > Inner Left IR
  dataCollection->getSubMenuItem(3)->addSubMenuItem("Inner Left IR", true);
  settingInnerLeftIR = dataCollection->getSubMenuItem(3)->getSubMenuItem(5);
  settingInnerLeftIR->setAction(toggleData);

  // MAIN MENU > Data Collection > Data Settings > Outer Left IR
  dataCollection->getSubMenuItem(3)->addSubMenuItem("Outer Left IR", true);
  settingOuterLeftIR = dataCollection->getSubMenuItem(3)->getSubMenuItem(6);
  settingOuterLeftIR->setAction(toggleData);
}

// Display the menu and perform any selected actions
// Set updateScreen to true in order to refresh the screen
// Recommendation: refresh the screen every 1000 loop cycles or a couple times per second)
// Note: must be run during each Arduino loop cycle
void SafeTownDisplay::displayMenu(bool updateScreen) {
  this->updateScreen = updateScreen;
  if (updateScreen && (!runSelectedAction || encoderAction)) {
    String menuTitle = currentMenu->getContent();
    int numItems = currentMenu->getNumItems();
    clearDisplay();

    // center the menu title
    adaSSD1306.setCursor((SCREEN_WIDTH - (currentMenu->getContent().length() * 6)) / 2, 0); 
    adaSSD1306.println(currentMenu->getContent());

    // Determine the start and end of the menu window
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
    
    // Print menu items (highlighting the selected item)
    for (int i = firstItem; i <= lastItem; i++) {
      adaSSD1306.setTextColor(SSD1306_WHITE);
      if (i == currentIndex) {
        adaSSD1306.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      }
      adaSSD1306.println(currentMenu->getSubMenuItemContent(i));
    }
    adaSSD1306.display();
  } else if (runSelectedAction) {
    // Run the action of the selected MenuItem
    currentMenu->getSubMenuItem(currentIndex)->doAction(this);
  }
}

////////////////////////////////////////////////////////////////////////////////
// MenuItem action functions
////////////////////////////////////////////////////////////////////////////////

// Display current IR sensor values
void SafeTownDisplay::displayIRValues(SafeTownDisplay* displayLibInst) {
  if (displayLibInst->updateScreen) {
    // clear the OLED buffer
    displayLibInst->clearDisplay();
    
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

// Collect IR sensor data and store in arrays
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
    displayLibInst->clearDisplay();
    adaSSD1306.println("Collecting Data");
    adaSSD1306.print(0);
    adaSSD1306.print("/");
    adaSSD1306.println(displayLibInst->samplesToCollect);
    adaSSD1306.display();
  }

  // collect and store data samples
  double time = millis() - displayLibInst->timeOffset;
  if (time >= displayLibInst->sampleTargetTime + displayLibInst->sampleInterval) {
    int currentSample = displayLibInst->samplePointer;
    int totalSamples = displayLibInst->samplesToCollect;

    // collect data
    displayLibInst->downSamples[currentSample] = analogRead(down_ir_pin);
    displayLibInst->frontSamples[currentSample] = analogRead(front_ir_pin);
    displayLibInst->innerLeftSamples[currentSample] = analogRead(in_ir_pin);
    displayLibInst->outerLeftSamples[currentSample] = analogRead(out_ir_pin);
    displayLibInst->times[currentSample] = time;
    
    // enable to print on serial monitor
    // Serial.println("Collecting data: " + String(currentSample+1) + "/" + String(totalSamples)); 

    displayLibInst->samplePointer++;
    displayLibInst->sampleTargetTime += displayLibInst->sampleInterval;
    if (displayLibInst->samplePointer == totalSamples) {
      displayLibInst->resetData = true;
      displayLibInst->runSelectedAction = false;
    }

    // output result
    if (displayLibInst->samplePointer % (totalSamples/10) == 0 || displayLibInst->samplePointer == totalSamples) {
      displayLibInst->clearDisplay();
      adaSSD1306.println("Collecting Data");
      adaSSD1306.print(displayLibInst->samplePointer);
      adaSSD1306.print("/");
      adaSSD1306.println(totalSamples);
      adaSSD1306.display();
    }
  }
}

// Output the data collected to the serial monitor
// Follows the data format for running with 'monitor.py' to generate a .csv file
void SafeTownDisplay::outputData(SafeTownDisplay* displayLibInst) {
  Serial.println("-----BEGIN DATA-----");
  Serial.println("DATA TYPE: IR_Sensors");
  Serial.print("Sample,Millis");
  if (displayLibInst->settingDownIR->getBoolData()) {
    Serial.print(",Down");
  }
  if (displayLibInst->settingFrontIR->getBoolData()) {
    Serial.print(",Front");
  }
  if (displayLibInst->settingInnerLeftIR->getBoolData()) {
    Serial.print(",InnerLeft");
  }
  if (displayLibInst->settingOuterLeftIR->getBoolData()) {
    Serial.print(",OuterLeft");
  }
  Serial.println();
  for (int i = 0; i < displayLibInst->samplesToCollect; i++) {
    Serial.print(i+1);
    Serial.print(",");
    Serial.print(displayLibInst->times[i]);
    if (displayLibInst->settingDownIR->getBoolData()) {
      Serial.print(",");
      Serial.print(displayLibInst->downSamples[i]);
    }
    if (displayLibInst->settingFrontIR->getBoolData()) {
      Serial.print(",");
      Serial.print(displayLibInst->frontSamples[i]);
    }
    if (displayLibInst->settingInnerLeftIR->getBoolData()) {
      Serial.print(",");
      Serial.print(displayLibInst->innerLeftSamples[i]);
    }
    if (displayLibInst->settingOuterLeftIR->getBoolData()) {
      Serial.print(",");
      Serial.print(displayLibInst->outerLeftSamples[i]);
    }
    Serial.println();
  }
  Serial.println("-----END DATA-----");
  displayLibInst->runSelectedAction = false;
}

// Toggle the boolean data of a MenuItem (pass as action for all boolean settings)
void SafeTownDisplay::toggleData(SafeTownDisplay* displayLibInst) {
  displayLibInst->currentMenu->getSubMenuItem(displayLibInst->currentIndex)->toggleBoolData();
  displayLibInst->runSelectedAction = false;
}

// Change the integer data of a MenuItem (pass as action for all integer settings)
void SafeTownDisplay::changeIntData(SafeTownDisplay* displayLibInst) {
  displayLibInst->encoderAction = true;
}