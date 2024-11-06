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
    void loop();
    void select();
    MenuItem menu;
    void menuSetup();
    int c;
    int numLines;
    MenuItem* currentMenu = &menu;
    void displayMenu();
    int currentIndex = 0;
};
#endif