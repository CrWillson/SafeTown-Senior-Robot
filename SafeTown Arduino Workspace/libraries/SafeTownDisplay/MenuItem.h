#ifndef MenuItem_h
#define MenuItem_h

#include "Arduino.h"
#include <vector>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

class MenuItem {
  public:
    MenuItem();
    MenuItem(String content);
    void SetDisplay(Adafruit_SSD1306& display);
    Adafruit_SSD1306* display;
    std::vector<MenuItem *> SubMenuItems;
    MenuItem* parent = nullptr;
    String content = "";
    void select();
    int c = 0;
    int getNumItems();
    void setParent(MenuItem *parent);
    void addSubMenuItem(String newContent);
    String getContent();
    MenuItem* getSubMenuItem(int index);
    String getSubMenuItemContent(int index);
};
#endif