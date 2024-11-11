/*
  SafeTownDisplay.cpp - Revamped library for displaying system values (based on DisplayOurValues)
  Created by Benjamin Kinard, October 1, 2024.
  Released into the public domain.
*/

// libraries and inclusions
#include "Arduino.h"
#include "MenuItem.h"

// Constructor
MenuItem::MenuItem()
{
}
MenuItem::MenuItem(String content)
{
  this->content = content;
}

void MenuItem::SetDisplay(Adafruit_SSD1306& display)
{
  this->display = &display;
}

void MenuItem::select()
{
  // c++;
  // display->clearDisplay();
  // display->setTextSize(1);
  // display->setTextColor(SSD1306_WHITE);
  // display->setCursor(0, 0);
  // display->println(c);
  // display->display();
}

int MenuItem::getNumItems()
{
  return SubMenuItems.size();
}

void MenuItem::setParent(MenuItem *parent)
{
  this->parent = parent;
}

MenuItem* MenuItem::getParent()
{
  return parent;
}

void MenuItem::addSubMenuItem(String newContent)
{
  if (getNumItems() == 0 && !isRoot()) {
    MenuItem* newItem = new MenuItem("<- Back");
    newItem->setParent(this);
    newItem->makeBack();
    SubMenuItems.push_back(newItem);
  }
  MenuItem* newItem = new MenuItem(newContent);
  newItem->setParent(this);
  newItem->setIndex(getNumItems());
  SubMenuItems.push_back(newItem);
  makeSelectable();
}

void MenuItem::setIndex(int index) {
  this->index = index;
}

int MenuItem::getIndex() {
  return index;
}

String MenuItem::getContent()
{
  return content;
}

MenuItem* MenuItem::getSubMenuItem(int index)
{
  if (index >= 0 && index < getNumItems()) {
    return SubMenuItems.at(index);
  }
  return nullptr;
}

String MenuItem::getSubMenuItemContent(int index)
{
  MenuItem* subMenuItem = getSubMenuItem(index);
  if (subMenuItem != nullptr) {
    String subContent = subMenuItem->getContent();
    return subContent;
  }
  return "NULL";
}

bool MenuItem::isSelectable()
{
  return selectable;
}

void MenuItem::makeSelectable()
{
  selectable = true;
}

bool MenuItem::isRoot()
{
  return root;
}

void MenuItem::makeRoot()
{
  root = true;
}

bool MenuItem::isBack()
{
  return back;
}

void MenuItem::makeBack()
{
  back = true;
}

void MenuItem::setAction(void (*selectAction)())
{
  this->selectAction = selectAction;
  action = true;
}
bool MenuItem::hasAction()
{
  return action;
}
void MenuItem::doAction()
{
  selectAction();
}