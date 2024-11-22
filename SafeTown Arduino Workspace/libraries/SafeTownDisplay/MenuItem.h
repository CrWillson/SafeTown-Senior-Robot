////////////////////////////////////////////////////////////////////////////////
// Filename: MenuItem.h
// Created: 10-1-2024
// Modified: 11-22-2024
// Author: Benjamin Kinard (SafeTown)
// Purpose: Header file for the MenuItem class
////////////////////////////////////////////////////////////////////////////////

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
    // Setup and constructors
    MenuItem();
    MenuItem(String content);
    MenuItem(String content, bool boolData);
    MenuItem(String content, int intData, int minIntData, int maxIntData, int incIntData);

    // Create new MenuItems and add to the descendant vector (public)
    void addSubMenuItem(String newContent);
    void addSubMenuItem(String newContent, bool boolData);
    void addSubMenuItem(String newContent, int intData, int minIntData, int maxIntData, int incIntData);

    // Getters and setters
    int getNumItems();
    void setIndex(int index);
    int getIndex();
    void setParent(MenuItem *parent);
    MenuItem* getParent();
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
    int getIncIntData();
    void setIncIntData(int setIncIntData);
    void decrementIntData();
    void incrementIntData();

  private:
    // Create new MenuItems and add to the descendant vector (private)
    void createMenuItem(String newContent);
    void createMenuItem(String newContent, bool boolData);
    void createMenuItem(String newContent, int intData, int minIntData, int maxIntData, int incIntData);

    // MenuItem Attributes
    std::vector<MenuItem *> SubMenuItems; // Store SubMenuItems
    MenuItem* parent = nullptr; // Parent pointer
    String content = ""; // Content to display
    int index = -1; // A MenuItem's index within its parent
    bool action = false; // True when MenuItem has an action
    bool back = false; // True when MenuItem is a "Back" item
    void (*selectAction)(SafeTownDisplay*) = nullptr; // The action a MenuItem does (only when there are no SubMenuItems)
    
    // Stored data variables
    bool boolData = false; // Whatever boolean data value the MenuItem needs to hold
    bool hasBoolData = false; // True if the MenuItem has boolean data
    bool hasIntData = false; // True if the MenuItem  has integer data
    int intData = -1; // Integer data
    int minIntData = -1; // Minimum value for integer data
    int maxIntData = -1; // Maximum value for integer data
    int incIntData = -1; // Increment value for integer data
    
};
#endif