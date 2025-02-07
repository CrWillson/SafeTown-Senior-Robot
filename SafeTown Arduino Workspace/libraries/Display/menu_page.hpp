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
    MenuPage(Menu* menu, const std::string& lbl) 
        : menu(menu), label(lbl) {}

    static constexpr uint8_t LINESPERSCREEN = 8;

    std::vector<std::shared_ptr<MenuLine>> lines;

    std::array<std::string, LINESPERSCREEN> getVisibleText() const;

    void addLine(MenuLine* line);

    bool onValueChange(const Event::ValueChangedEvent& e);

    bool scrollUp();
    bool scrollDown();
    bool select();

private:
    Menu* menu;
    std::string label;

    uint8_t selectedLine = 0;
    uint8_t topLine = 0;
    uint8_t botLine = 7;
    uint8_t numLines = 0;

    friend class Menu;
};