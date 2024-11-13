#ifndef SafeTownDisplay_h
#define SafeTownDisplay_h

#include "Arduino.h"
#include "MenuItem.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <String.h>

class SafeTownDisplay {
  public:
    SafeTownDisplay();
    void setup();
    MenuItem mainMenu;
    void menuSetup();
    int numLines;
    MenuItem* currentMenu = &mainMenu;
    void displayMenu();
    int currentIndex = 0;
    void encoderPress();
    void encoderLeft();
    void encoderRight();
    bool runSelectedAction = false; // Denotes whether an action is currently being run for a MenuItem
    bool encoderAction = false; // Denotes whether the currently run action is an encoder input action
    static void displayIRValues(SafeTownDisplay* displayLibInst);
    static void collectData(SafeTownDisplay* displayLibInst);
    static void outputData(SafeTownDisplay* displayLibInst);
    static void toggleData(SafeTownDisplay* displayLibInst);
    static void changeIntData(SafeTownDisplay* displayLibInst);

    // IR pin values
    const static int front_ir_pin = 28;
    const static int in_ir_pin = 27;
    const static int out_ir_pin = 26;
    const static int down_ir_pin = 29;

    // Data collection
    MenuItem* settingSampleRate;
    MenuItem* settingSampleTime;
    MenuItem* settingDownIR;
    MenuItem* settingFrontIR;
    MenuItem* settingInnerLeftIR;
    MenuItem* settingOuterLeftIR;
};
#endif