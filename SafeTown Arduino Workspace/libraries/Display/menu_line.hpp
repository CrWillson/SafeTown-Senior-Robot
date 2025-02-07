#pragma once

#include <string>
#include <functional>
#include "Arduino.h"
#include "event_types.hpp"
#include "event_manager.hpp"

class enum LineType { Text, Value, Button, Toggle, Slider };

class MenuLine {
public:
    MenuLine(const std::string& text) : label(text) {}
    virtual ~MenuLine() = default;

    // Render the line. The parameter 'selected' indicates if this line is currently selected.
    virtual std::string getText(bool selected) const = 0;

    // Called when this line is selected (encoder pressed).
    virtual void onSelect() = 0;

    virtual LineType getType() = 0;

    std::string label;
};



class TextMenuLine : public MenuLine {
public:
    TextMenuLine(const std::string& text) 
        : MenuLine(text) {}

    virtual std::string getText(bool selected) const override;
    virtual void onSelect() override { /* do nothing */ }
    virtual LineType getType() { return LineType::Text; }
private:
    friend class MenuPage;
};


class ValueMenuLine : public MenuLine {
public:
    ValueMenuLine(const std::string& text, const std::string valLbl)
        : MenuLine(text), valueLabel(valLbl) {}

    virtual std::string getText(bool selected) const override;
    virtual void onSelect() override { /* do nothing */ }
    virtual LineType getType() { return LineType::Value; }
private:
    std::string value;
    std::string valueLabel;
    friend class MenuPage;
};


class ButtonMenuLine : public MenuLine {
public:
    ButtonMenuLine(const std::string& text, std::function<void()> act)
        : MenuLine(text), action(act) {}

    virtual std::string getText(bool selected) const override;
    virtual void onSelect() override { action; };
    virtual LineType getType() { return LineType::Button; }
private:
    std::function<void()> action;
    friend class MenuPage;
};


class ToggleMenuLine : public MenuLine {
public:
    ToggleMenuLine(const std::string& text, bool initialState)
      : MenuLine(text), state(initialState) {}
    virtual std::string getText(bool selected) const override;
    virtual void onSelect() override { state = !state; };
    virtual LineType getType() { return LineType::Toggle; }
private:
    bool state;
    friend class MenuPage;
};


class SliderMenuLine : public MenuLine {
public:
    SliderMenuLine(const std::string& text, int initialValue, int minValue, int maxValue)
      : MenuLine(text), value(initialValue), minVal(minValue), maxVal(maxValue), editing(false) {}
    virtual std::string getText(bool selected) const override;
    virtual void onSelect() override { editing = !editing; };
    virtual LineType getType() { return LineType::Slider; }
private:
    int value;
    int minVal;
    int maxVal;
    bool editing;
    friend class MenuPage;
};


