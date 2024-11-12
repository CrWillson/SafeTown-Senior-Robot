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
  this->content = content;
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

// Add a SubMenuItem to MenuItem's vector
void MenuItem::addSubMenuItem(String newContent)
{
  if (getNumItems() == 0 && !isRoot()) {
    // If this is the first SubMenuItem, then create a "Back" item
    MenuItem* newItem = new MenuItem("<- Back");
    newItem->setParent(this);
    newItem->makeBack();
    SubMenuItems.push_back(newItem);
  }
  // Create new MenuItem with this item as its parent
  MenuItem* newItem = new MenuItem(newContent);
  newItem->setParent(this);
  newItem->setIndex(getNumItems());
  SubMenuItems.push_back(newItem);
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
void MenuItem::setAction(void (*selectAction)())
{
  this->selectAction = selectAction;
  action = true;
}

// Set the MenuItem's action
void MenuItem::setAction2(void (*selectAction2)(SafeTownDisplay*))
{
  this->selectAction2 = selectAction2;
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
    selectAction();
  }
  if (selectAction2 != nullptr) {
    selectAction2(displayLibInst);
  }
  
}

// Get the MenuItem's bool data
bool MenuItem::getBoolData()
{
  return boolData;
}

void MenuItem::setBoolData(bool boolData)
{
  this->boolData = boolData;
}