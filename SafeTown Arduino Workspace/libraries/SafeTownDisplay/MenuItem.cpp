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
  c++;
  display->clearDisplay();
  display->setTextSize(1);
  display->setTextColor(SSD1306_WHITE);
  display->setCursor(0, 0);
  display->println(c);
  display->display();
}

int MenuItem::getNumItems()
{
  return SubMenuItems.size();
}

void MenuItem::setParent(MenuItem *parent)
{
  this->parent = parent;
}

void MenuItem::addSubMenuItem(String newContent)
{
  MenuItem newItem = MenuItem(newContent);
  newItem.setParent(this);
  SubMenuItems.push_back(&newItem);
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
  return "ERROR";
}