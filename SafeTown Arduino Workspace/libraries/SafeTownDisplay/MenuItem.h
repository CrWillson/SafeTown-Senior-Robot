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
    int index = -1;
    void setIndex(int index);
    int getIndex();
    int getNumItems();
    void setParent(MenuItem *parent);
    MenuItem* getParent();
    void addSubMenuItem(String newContent);
    String getContent();
    MenuItem* getSubMenuItem(int index);
    String getSubMenuItemContent(int index);
    bool selectable = false;
    bool isSelectable();
    void makeSelectable();
    bool root = false;
    bool isRoot();
    void makeRoot();
    bool back = false;
    bool isBack();
    void makeBack();
    void (*selectAction)() = nullptr;
    bool action = false;
    void setAction(void (*selectAction)());
    bool hasAction();
    void doAction();
};
#endif