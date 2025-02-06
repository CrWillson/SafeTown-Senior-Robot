#pragma once

#include <cctype>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <memory>
#include <vector>
#include <string>

#include "event_manager.hpp"

class Display {
public:

    Display(EventManager* manager);
    ~Display() = default;

    void clearDisplay();
    void printLines(std::vector<std::string> lines);

    void onSelectPress(const Event::Event& event);
    void onSelectRelease(const Event::Event& event);

private:

    // Display variables and instances
    static constexpr int SCREEN_WIDTH = 128;       ///< OLED display width, in pixels
    static constexpr int SCREEN_HEIGHT = 32;       ///< OLED display height, in pixels (set to 32 for 4 lines, 64 for 8 lines)
    static constexpr int OLED_RESET = -1;          ///< Reset pin # (or -1 if sharing Arduino reset pin)
    static constexpr int SCREEN_ADDRESS = 0x3C;    ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

    std::unique_ptr<Adafruit_SSD1306> screen;
    EventManager* eventManager;

    // Adafruit_SSD1306 adaSSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); // The display object

};