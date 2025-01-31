#include "display.hpp"

Display::Display() : screen(std::make_unique<Adafruit_SSD1306>(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET))
{
    Serial.begin(115200);
    if(!screen->begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("SSD1306 allocation failed"));
        while(true); // Don't proceed, loop forever
    }
    clearDisplay();
}

void Display::clearDisplay() {
    screen->clearDisplay(); // reset the display
    screen->setTextSize(1); // 7 pixels tall
    screen->setTextColor(SSD1306_WHITE); // draw white text    
    screen->setCursor(0,0); // start printing at the beginning of the first line
}

void Display::printLines(std::vector<std::string> lines)
{
    clearDisplay();
    for (const auto line : lines) {
        screen->println(line.c_str());
    }
    screen->display();
}
