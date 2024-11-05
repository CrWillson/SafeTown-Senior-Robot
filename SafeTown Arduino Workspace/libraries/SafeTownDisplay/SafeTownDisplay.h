#ifndef SafeTownDisplay_h
#define SafeTownDisplay_h

#include "Arduino.h"
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
    MenuItem menu = MenuItem(adaSSD1306);
    int c;
};
#endif