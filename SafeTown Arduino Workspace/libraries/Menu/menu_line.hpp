#pragma once

#include <string>
#include <functional>
#include "Arduino.h"
#include "event_types.hpp"
#include "event_manager.hpp"


/**
 * @brief Abstract base type for a menu line
 */
class MenuLine {
public:
    MenuLine(const std::string& text) : label(text) {}
    virtual ~MenuLine() = default;

    /**
     * @brief Get the line as a std::string.
     * 
     * @param selected - whether the line is selected or not
     * @return std::string - The menu line in text format
     */
    virtual std::string getText(bool selected) const = 0;

    /**
     * @brief Called upon selecting the menu line
     * 
     */
    virtual void onSelect() = 0;

    /**
     * @brief The text the display as the menu line
     * 
     */
    std::string label;
};

/**
 * @brief Menu line that displays a static, non-interactable separator line
 * 
 */
class SpacerMenuLine : public MenuLine {
public:
    SpacerMenuLine() 
        : MenuLine("-------------------") {}
        
    virtual std::string getText(bool selected) const override;
    virtual void onSelect() override { /* do nothing */ }
};


/**
 * @brief Menu line type that displays static, non-interactable text
 */
class TextMenuLine : public MenuLine {
public:
    TextMenuLine(const std::string& text) 
        : MenuLine(text) {}

    virtual std::string getText(bool selected) const override;
    virtual void onSelect() override { /* do nothing */ }
private:
    friend class MenuPage;
};

/**
 * @brief Menu line that displays a label followed by some value given by the value label
 */
class ValueMenuLine : public MenuLine {
public:
    ValueMenuLine(const std::string& text, const std::string valLbl)
        : MenuLine(text), valueLabel(valLbl) {}

    virtual std::string getText(bool selected) const override;
    virtual void onSelect() override { /* do nothing */ }
private:
    std::string value;
    std::string valueLabel;
    friend class MenuPage;
};

/**
 * @brief Menu line that displays static text. Executes a std::function<void()> upon interaction
 */
class ButtonMenuLine : public MenuLine {
public:
    ButtonMenuLine(const std::string& text, std::function<void()> act)
        : MenuLine(text), action(act) {}

    virtual std::string getText(bool selected) const override;
    virtual void onSelect() override;
private:
    std::function<void()> action;
    friend class MenuPage;
};

/**
 * @brief Menu line that displays static text. Toggles a boolean value upon interaction
 */
class ToggleMenuLine : public MenuLine {
public:
    ToggleMenuLine(const std::string& text, bool initialState)
      : MenuLine(text), state(initialState) {}
    virtual std::string getText(bool selected) const override;
    virtual void onSelect() override { state = !state; };
private:
    bool state;
    friend class MenuPage;
};

/**
 * @brief Menu line that displays a label followed by some user modifiable value.
 * @details Interacting will begin editing. Once editing, scrolling up and down will instead increment or decrement
 * the value between its upper and lower bounds. Interacting again will confirm the selection and broadcast a notification
 * of the value's change.
 */
class SliderMenuLine : public MenuLine {
public:
    SliderMenuLine(const std::string& text, const std::string& label, int initialValue, int minValue, int maxValue)
      : MenuLine(text), valueLabel(label), value(initialValue), minVal(minValue), maxVal(maxValue), editing(false) {}
    virtual std::string getText(bool selected) const override;
    virtual void onSelect() override { editing = !editing; };
private:
    std::string valueLabel;
    int value;
    int minVal;
    int maxVal;
    bool editing;
    friend class MenuPage;
};


