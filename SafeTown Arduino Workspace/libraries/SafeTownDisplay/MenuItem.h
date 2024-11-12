#ifndef MenuItem_h
#define MenuItem_h

#include "Arduino.h"
#include <vector>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

class SafeTownDisplay; // forward declaration

class MenuItem {
  public:
    MenuItem();
    MenuItem(String content);
    // void SetDisplay(Adafruit_SSD1306& display);
    // Adafruit_SSD1306* display;
    void setIndex(int index);
    int getIndex();
    int getNumItems();
    void setParent(MenuItem *parent);
    MenuItem* getParent();
    void addSubMenuItem(String newContent);
    String getContent();
    void setContent(String content);
    MenuItem* getSubMenuItem(int index);
    String getSubMenuItemContent(int index);
    bool isParent();
    bool isRoot();
    bool isBack();
    void makeBack();
    void setAction(void (*selectAction)());
    void setAction2(void (*selectAction2)(SafeTownDisplay*));
    bool hasAction();
    void doAction(SafeTownDisplay* displayLibInst);
    bool getBoolData();
    void setBoolData(bool boolData);

  private:
    std::vector<MenuItem *> SubMenuItems; // Store SubMenuItems
    MenuItem* parent = nullptr; // Parent pointer
    String content = ""; // Content to display
    int index = -1; // A MenuItem's index within its parent
    bool action = false;
    bool back = false; // True when MenuItem is a "Back" item
    void (*selectAction)() = nullptr; // The action a MenuItem does (only when there are no SubMenuItems)
    void (*selectAction2)(SafeTownDisplay*) = nullptr;
    bool boolData = true;
};
#endif