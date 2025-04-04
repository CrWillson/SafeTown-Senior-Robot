#pragma once

#include <string>
#include <vector>
#include <memory>
#include <array>
#include "menu_line.hpp"
#include "event_manager.hpp"

// Forward declaration
class Menu;

/**
 * @brief A page in the menu object
 * 
 */
class MenuPage {
public:
    /**
     * @brief Construct a new MenuPage with the given label
     * 
     * @param lbl - The label to give the page
     */
    MenuPage(const std::string& lbl);
    virtual ~MenuPage() = default;
    
    static constexpr uint8_t LINESPERSCREEN = 8;

    /**
     * @brief Get the all of the text visible on the page given the current line
     * 
     * @return std::array<std::string, LINESPERSCREEN> - The lines in string format
     */
    std::array<std::string, LINESPERSCREEN> getVisibleText() const;

    /**
     * @brief Add a line to the page
     * 
     * @param line - The line to be added
     * @return MenuPage* - The page the line was just added to
     */
    virtual MenuPage* addLine(MenuLine* line);

    /**
     * @brief Remove all lines from the page
     * 
     */
    virtual void clearLines();

protected:

    /**
     * @brief Called each time the page is loaded by the menu
     * 
     */
    virtual void onPageLoad();

    /**
     * @brief Called each time the page is exited
     * 
     */
    virtual void onPageExit() { /* do nothing */}
    
    virtual bool onValueChange(const Event::ValueChangedEvent& e);
    virtual bool onValueRequested(const std::string& reqLabel);

    virtual void scrollUp();
    virtual void scrollDown();
    virtual void select();

    /**
     * @brief All of the lines stored in the page
     * 
     */
    std::vector<std::shared_ptr<MenuLine>> lines;

    Menu* parentMenu;
    EventManager* eventManager;
    std::string label;

    uint8_t selectedLine = 0;
    uint8_t topLine = 0;
    uint8_t botLine = 7;
    uint8_t numLines = 0;

    friend class Menu;
};