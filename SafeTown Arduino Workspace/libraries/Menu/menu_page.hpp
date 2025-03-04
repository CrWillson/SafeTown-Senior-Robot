#pragma once

#include <string>
#include <vector>
#include <memory>
#include <array>
#include "menu_line.hpp"
#include "event_manager.hpp"

// Forward declaration
class Menu;

class MenuPage {
public:
    MenuPage(const std::string& lbl);
    virtual ~MenuPage() = default;

    static constexpr uint8_t LINESPERSCREEN = 8;

    std::vector<std::shared_ptr<MenuLine>> lines;

    std::array<std::string, LINESPERSCREEN> getVisibleText() const;

    virtual void addLine(MenuLine* line);
    virtual void clearLines();

    virtual bool onValueChange(const Event::ValueChangedEvent& e);
    virtual void onPageLoad();
    virtual void onPageExit() { /* do nothing */}
    virtual bool onValueRequested(const std::string& reqLabel);

    virtual void scrollUp();
    virtual void scrollDown();
    virtual void select();

protected:
    Menu* parentMenu;
    EventManager* eventManager;
    std::string label;

    uint8_t selectedLine = 0;
    uint8_t topLine = 0;
    uint8_t botLine = 7;
    uint8_t numLines = 0;

    friend class Menu;
};