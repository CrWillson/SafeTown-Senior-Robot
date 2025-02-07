#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <cctype>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <memory>
#include <vector>
#include <string>
#include <array>
#include <string>

#include "menu.hpp"
#include "event_manager.hpp"

class Display {
public:

    // Display(EventManager* manager);
    Display() = default;
    ~Display() = default;

    void initDisplay(EventManager* manager);
    void clearDisplay();

    void draw();
    void setLineText(std::string str, uint8_t lineNum);
    void setLineText(std::string str, int value, uint8_t lineNum); 
    void setLineText(std::string str, bool value, uint8_t lineNum); 
    void setLineText(std::string str, std::string value, uint8_t lineNum);

    void printLines(std::vector<std::string> lines);
    
    void onPageUpdated(const Event::PageChangedEvent& e);

private:
    EventManager* eventManager;
    Adafruit_SSD1306* screen;
    std::unique_ptr<Menu> menu;

    std::array<std::string, MenuPage::LINESPERSCREEN> lines;

};

#endif //DISPLAY_HPP