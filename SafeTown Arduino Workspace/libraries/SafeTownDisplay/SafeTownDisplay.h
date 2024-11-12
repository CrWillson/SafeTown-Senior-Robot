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
    int c;
    int numLines;
    MenuItem* currentMenu = &mainMenu;
    void displayMenu();
    int currentIndex = 0;
    void selectCurrentItem();
    void decrementMenuIndex();
    void incrementMenuIndex();
    bool runSelectedAction = false;
    static void displayIRValues();
    static void collectData();
    static void outputData();
    static void toggleDownIR();
    static void toggleFrontIR();
    static void toggleInnerLeftIR();
    static void toggleOuterLeftIR(SafeTownDisplay* displayLibInst);

    // IR pin values
    const static int front_ir_pin = 28;
    const static int in_ir_pin = 27;
    const static int out_ir_pin = 26;
    const static int down_ir_pin = 29;
    // static bool outputDownIR = true;
    // static bool outputFrontIR = true;
    // static bool outputInnerLeftIR = true;
    // static bool outputOuterLeftIR = true;
};
#endif