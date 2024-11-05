/*
  SafeTownDisplay.cpp - Revamped library for displaying system values (based on DisplayOurValues)
  Created by Benjamin Kinard, October 1, 2024.
  Released into the public domain.
*/

// libraries and inclusions
#include "Arduino.h"
#include "MenuItem.h"

// Constructor
MenuItem::MenuItem(Adafruit_SSD1306& display)
{
  this->display = &display;
}

void MenuItem::select()
{
  display->println(content + " selected");
}