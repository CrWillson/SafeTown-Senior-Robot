#ifndef MenuItem_h
#define MenuItem_h

#include "Arduino.h"
#include <vector>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

class MenuItem {
  public:
    MenuItem(Adafruit_SSD1306& display);
    Adafruit_SSD1306* display;
    std::vector<MenuItem *> SubMenuItems;
    String content = "";
    void select();
};
#endif