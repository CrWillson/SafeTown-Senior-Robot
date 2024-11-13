#ifndef MenuItem_h
#define MenuItem_h

#include "Arduino.h"
#include <vector>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

class SafeTownDisplay; // forward declaration

const String BACK_TEXT = "<- Back";

class MenuItem {
  public:
    MenuItem();
    MenuItem(String content);
    MenuItem(String content, bool boolData);
    MenuItem(String content, int intData, int minIntData, int maxIntData);
    // void SetDisplay(Adafruit_SSD1306& display);
    // Adafruit_SSD1306* display;
    void setIndex(int index);
    int getIndex();
    int getNumItems();
    void setParent(MenuItem *parent);
    MenuItem* getParent();
    void addSubMenuItem(String newContent);
    void addSubMenuItem(String newContent, bool boolData);
    void addSubMenuItem(String newContent, int intData, int minIntData, int maxIntData);
    String getContent();
    void setContent(String content);
    MenuItem* getSubMenuItem(int index);
    String getSubMenuItemContent(int index);
    bool isParent();
    bool isRoot();
    bool isBack();
    void makeBack();
    void setAction(void (*selectAction)(SafeTownDisplay*));
    bool hasAction();
    void doAction(SafeTownDisplay* displayLibInst);
    bool getBoolData();
    void setBoolData(bool boolData);
    void toggleBoolData();
    int getIntData();
    void setIntData(int intData);
    int getMinIntData();
    void setMinIntData(int minIntData);
    int getMaxIntData();
    void setMaxIntData(int maxIntData);
    void decrementIntData();
    void incrementIntData();

  private:
    std::vector<MenuItem *> SubMenuItems; // Store SubMenuItems
    MenuItem* parent = nullptr; // Parent pointer
    String content = ""; // Content to display
    int index = -1; // A MenuItem's index within its parent
    bool action = false;
    bool back = false; // True when MenuItem is a "Back" item
    void (*selectAction)(SafeTownDisplay*) = nullptr; // The action a MenuItem does (only when there are no SubMenuItems)
    bool boolData = false; // Whatever boolean data value the MenuItem needs to hold
    bool hasBoolData = false;
    bool hasIntData = false;
    int intData = -1;
    int minIntData = -1;
    int maxIntData = -1;
    void createMenuItem(String newContent);
    void createMenuItem(String newContent, bool boolData);
    void createMenuItem(String newContent, int intData, int minIntData, int maxIntData);
};
#endif