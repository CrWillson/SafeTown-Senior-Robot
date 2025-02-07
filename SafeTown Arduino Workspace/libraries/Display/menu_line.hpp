#pragma once

#include <string>
#include <functional>
#include "Arduino.h"
#include "event_types.hpp"
#include "event_manager.hpp"


class MenuLine {
public:
    MenuLine(const std::string& text)
        : label(text) {}
    virtual ~MenuLine() = default;

    // Render the line. The parameter 'selected' indicates if this line is currently selected.
    virtual std::string getText(bool selected) const = 0;

    // Called when this line is selected (encoder pressed).
    virtual void onSelect() = 0;

    // Called when an external event is received (e.g. a value changed externally)
    // so that the line can update its displayed value.
    virtual void onEvent(const Event::ValueChangedEvent& e) = 0;

    std::string label; // Unique identifier for this line.

    EventManager* eventManager;
};



class TextMenuLine : public MenuLine {
public:
    TextMenuLine(const std::string& text) 
        : MenuLine(text) {}

    virtual std::string getText(bool selected) const override;
    virtual void onSelect() override { /* do nothing */ }
    virtual void onEvent(const Event::ValueChangedEvent& e) override { /* do nothing */ }
private:
    friend class MenuPage;
};


class ValueMenuLine : public MenuLine {
public:
    ValueMenuLine(const std::string& text, const std::string valLbl)
        : MenuLine(text), valueLabel(valLbl) {
            eventManager->subscribe<Event::ValueChangedEvent>([this](const auto& event) {
                this->onEvent(event);
            });
        }

    virtual std::string getText(bool selected) const override;
    virtual void onSelect() override { /* do nothing */ }
    virtual void onEvent(const Event::ValueChangedEvent& e) override;
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
    virtual void onEvent(const Event::ValueChangedEvent& e) override { /* do nothing */ }
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
    virtual void onEvent(const Event::ValueChangedEvent& e) override { /* do nothing */ };
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
    virtual void onEvent(const Event::ValueChangedEvent& e) override { /* do nothing */ };
private:
    int value;
    int minVal;
    int maxVal;
    bool editing;
    friend class MenuPage;
};


