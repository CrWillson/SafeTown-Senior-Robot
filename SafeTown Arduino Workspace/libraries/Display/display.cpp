#include "display.hpp"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels (set to 32 for 4 lines, 64 for 8 lines)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 ssd1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); // The display object


void Display::initDisplay(EventManager* manager)
{   
    eventManager = manager;
    screen = &ssd1306;
    
    if(!screen->begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("SSD1306 allocation failed"));
        return;
    }
    clearDisplay();

    eventManager->subscribe<Event::PageChangedEvent>([this](const auto& event) {
        this->onPageUpdated(event);
    });
}

void Display::clearDisplay()
{
    screen->clearDisplay(); // reset the display
    screen->setTextSize(1); // 7 pixels tall
    screen->setTextColor(SSD1306_WHITE); // draw white text    
    screen->setCursor(0,0); // start printing at the beginning of the first line
}

void Display::draw()
{
    clearDisplay();
    for (const auto line : lines) {
        screen->println(line.c_str());
    }
    screen->display();
}

void Display::setLineText(std::string str, uint8_t lineNum)
{
    lines[lineNum] = str;
}

void Display::setLineText(std::string str, int value, uint8_t lineNum)
{
    std::string newLine = str + std::to_string(value);
    lines[lineNum] = newLine;
}

void Display::setLineText(std::string str, bool value, uint8_t lineNum)
{
    std::string newLine = str + (value ? "T" : "F");
    lines[lineNum] = newLine;
}

void Display::setLineText(std::string str, std::string value, uint8_t lineNum)
{
    std::string newLine = str + value;
    lines[lineNum] = newLine;
}

void Display::printLines(std::vector<std::string> lines)
{
    clearDisplay();
    for (const auto line : lines) {
        screen->println(line.c_str());
    }
    screen->display();
}

void Display::onPageUpdated(const Event::PageChangedEvent &e)
{
    Serial.println("Page Change Event registered");
    lines = e.lines;
    draw();
}
