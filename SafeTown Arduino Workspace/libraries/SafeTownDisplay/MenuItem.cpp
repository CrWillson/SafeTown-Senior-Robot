/*
  SafeTownDisplay.cpp - Revamped library for displaying system values (based on DisplayOurValues)
  Created by Benjamin Kinard, October 1, 2024.
  Released into the public domain.
*/

// libraries and inclusions
#include "Arduino.h"
#include "MenuItem.h"
#include "SafeTownDisplay.h"

// Constructor
MenuItem::MenuItem()
{
}

// Constructor for setting content
MenuItem::MenuItem(String content)
{
  setContent(content);
}

// Constructor for setting content
MenuItem::MenuItem(String content, bool boolData)
{
  setContent(content);
  setBoolData(boolData);
  hasBoolData = true;
}

// Constructor for setting content
MenuItem::MenuItem(String content, int intData, int minIntData, int maxIntData)
{
  setContent(content);
  setIntData(intData);
  setMinIntData(minIntData);
  setMaxIntData(maxIntData);
  hasIntData = true;
}

// Unnecessary?
// Give the MenuItem access to the display
// void MenuItem::SetDisplay(Adafruit_SSD1306& display)
// {
//   this->display = &display;
// }

// Return the number of SubMenuItems a MenuItem has (including "Back" item)
int MenuItem::getNumItems()
{
  return SubMenuItems.size();
}

// Set the MenuItem's parent
void MenuItem::setParent(MenuItem *parent)
{
  this->parent = parent;
}

// Get the MenuItem's parent pointer
MenuItem* MenuItem::getParent()
{
  return parent;
}

// Create new MenuItem with this item as its parent
void MenuItem::createMenuItem(String newContent)
{
  MenuItem* newItem = new MenuItem(newContent);
  newItem->setParent(this);
  newItem->setIndex(getNumItems());
  if (newContent == BACK_TEXT) {
    newItem->makeBack();
  }
  SubMenuItems.push_back(newItem);
}

// Create new MenuItem with this item as its parent
void MenuItem::createMenuItem(String newContent, bool boolData)
{
  MenuItem* newItem = new MenuItem(newContent, boolData);
  newItem->setParent(this);
  newItem->setIndex(getNumItems());
  SubMenuItems.push_back(newItem);
}

// Create new MenuItem with this item as its parent
void MenuItem::createMenuItem(String newContent, int intData, int minIntData, int maxIntData)
{
  MenuItem* newItem = new MenuItem(newContent, intData, minIntData, maxIntData);
  newItem->setParent(this);
  newItem->setIndex(getNumItems());
  SubMenuItems.push_back(newItem);
}

// Add a SubMenuItem to MenuItem's vector
void MenuItem::addSubMenuItem(String newContent)
{
  if (getNumItems() == 0 && !isRoot()) {
    // If this is the first SubMenuItem, then create a "Back" item
    createMenuItem(BACK_TEXT);
  }
  createMenuItem(newContent);
}

// Add a SubMenuItem to MenuItem's vector
void MenuItem::addSubMenuItem(String newContent, bool boolData)
{
  if (getNumItems() == 0 && !isRoot()) {
    // If this is the first SubMenuItem, then create a "Back" item
    createMenuItem(BACK_TEXT);
  }
  createMenuItem(newContent, boolData);
}

// Add a SubMenuItem to MenuItem's vector
void MenuItem::addSubMenuItem(String newContent, int intData, int minIntData, int maxIntData)
{
  if (getNumItems() == 0 && !isRoot()) {
    // If this is the first SubMenuItem, then create a "Back" item
    createMenuItem(BACK_TEXT);
  }
  createMenuItem(newContent, intData, minIntData, maxIntData);
}

// Set the MenuItem's index
void MenuItem::setIndex(int index) {
  this->index = index;
}

// Get the MenuItem's index
int MenuItem::getIndex() {
  return index;
}

// Get the MenuItem's content
String MenuItem::getContent()
{
  if (hasIntData) {
    return content + ": " + String(intData);
  } else if (hasBoolData) {
    if (boolData) {
      return content + ": TRUE";
    } else {
      return content + ": FALSE";
    }
  }
  return content;
}

// Set the MenuItem's content
void MenuItem::setContent(String content)
{
  this->content = content;
}

// Get a SubMenuItem
MenuItem* MenuItem::getSubMenuItem(int index)
{
  if (index >= 0 && index < getNumItems()) {
    return SubMenuItems.at(index);
  }
  return nullptr;
}

// Get a SubMenuItem's content
String MenuItem::getSubMenuItemContent(int index)
{
  MenuItem* subMenuItem = getSubMenuItem(index);
  if (subMenuItem != nullptr) {
    String subContent = subMenuItem->getContent();
    return subContent;
  }
  return "NULL";
}

// Check if the MenuItem is selectable (i.e. it has SubMenuItems)
bool MenuItem::isParent()
{
  return (getNumItems() > 0);
}

// Check if the MenuItem is the root
bool MenuItem::isRoot()
{
  return (parent == nullptr);
}

// Check if the MenuItem is a "Back" item
bool MenuItem::isBack()
{
  return back;
}

// Make the MenuItem a "Back" item
void MenuItem::makeBack()
{
  back = true;
}

// Set the MenuItem's action
void MenuItem::setAction(void (*selectAction)(SafeTownDisplay*))
{
  this->selectAction = selectAction;
  action = true;
}

// Check if a MenuItem has an action
bool MenuItem::hasAction()
{
  return action;
}

// Do the MenuItem's action
void MenuItem::doAction(SafeTownDisplay* displayLibInst)
{
  if (selectAction != nullptr) {
    selectAction(displayLibInst);
  }
}

// Get the MenuItem's bool data
bool MenuItem::getBoolData()
{
  return boolData;
}

// Set the MenuItem's bool data
void MenuItem::setBoolData(bool boolData)
{
  this->boolData = boolData;
}

// Toggle the MenuItem's bool data
void MenuItem::toggleBoolData()
{
  setBoolData(!getBoolData());
}

// Get the MenuItem's int data
int MenuItem::getIntData()
{
  return intData;
}

// Set the MenuItem's int data
void MenuItem::setIntData(int intData)
{
  this->intData = intData;
}

// Get the MenuItem's min int data
int MenuItem::getMinIntData()
{
  return minIntData;
}

// Set the MenuItem's min int data
void MenuItem::setMinIntData(int minIntData)
{
  this->minIntData = minIntData;
}

// Get the MenuItem's max int data
int MenuItem::getMaxIntData()
{
  return maxIntData;
}

// Set the MenuItem's max int data
void MenuItem::setMaxIntData(int maxIntData)
{
  this->maxIntData = maxIntData;
}

// Decrement (if above min) the MenuItem's int data
void MenuItem::decrementIntData()
{
  if (intData > minIntData) {
    intData--;
  }
}

// Increment (if below max) the MenuItem's int data
void MenuItem::incrementIntData()
{
  if (intData < maxIntData) {
    intData++;
  }
}