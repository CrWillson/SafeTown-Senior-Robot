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

#include "event_manager.hpp"

class Display {
public:

    // Display(EventManager* manager);
    Display() = default;
    ~Display() = default;

    void init();
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

    std::array<std::string, 8> lines;

};

#endif //DISPLAY_HPP