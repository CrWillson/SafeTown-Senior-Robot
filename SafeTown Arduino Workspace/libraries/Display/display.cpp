#include "display.hpp"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels (set to 32 for 4 lines, 64 for 8 lines)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 screen(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); // The display object

// Display::Display(EventManager* manager)
// {
//     eventManager = manager;

//     // Serial.print("Subscribing to encoder press: ");
//     // Serial.println(Event::getID<Event::EncoderPress>());
//     manager->subscribe<Event::EncoderPress>([this](const auto& event) {
//         this->onSelectPress(event);
//     });

//     // Serial.print("Subscribing to encoder left: ");
//     // Serial.println(Event::getID<Event::EncoderLeft>());
//     manager->subscribe<Event::EncoderLeft>([this](const auto& event) {
//         this->onScrollUp(event);
//     });

//     // Serial.print("Subscribing to encoder right: ");
//     // Serial.println(Event::getID<Event::EncoderRight>());
//     manager->subscribe<Event::EncoderRight>([this](const auto& event) {
//         this->onScrollDown(event);
//     });
// }

void Display::initDisplay(EventManager* manager)
{
    Serial.begin(115200);
    if(!screen.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("SSD1306 allocation failed"));
        return;
    }
    clearDisplay();

    eventManager = manager;

    manager->subscribe<Event::UpdateDisplayText>([this](const auto& event) {
        this->onUpdateDisplay(event);
    });
    manager->subscribe<Event::EncoderPress>([this](const auto& event) {
        this->onSelectPress(event);
    });
    manager->subscribe<Event::EncoderLeft>([this](const auto& event) {
        this->onScrollUp(event);
    });
    manager->subscribe<Event::EncoderRight>([this](const auto& event) {
        this->onScrollDown(event);
    });
}

void Display::clearDisplay()
{
    screen.clearDisplay(); // reset the display
    screen.setTextSize(1); // 7 pixels tall
    screen.setTextColor(SSD1306_WHITE); // draw white text    
    screen.setCursor(0,0); // start printing at the beginning of the first line
}

void Display::draw()
{
    clearDisplay();
    for (const auto line : lines) {
        screen.println(line.c_str());
    }
    screen.display();
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
        screen.println(line.c_str());
    }
    screen.display();
}

void Display::onSelectPress(const Event::Event& event)
{
    Serial.println("Button Press registered by display");
    setLineText("Button Pressed!", 6);
    draw();
    delay(10);
    setLineText(" ", 6);
    draw();
}

void Display::onScrollUp(const Event::Event &event)
{
    Serial.println("Scroll Up registered by display");
    setLineText("Scroll up", 5);
    draw();
}

void Display::onScrollDown(const Event::Event &event)
{
    Serial.println("Scroll Down registered by display");
    setLineText("Scroll down", 5);
    draw();
}

void Display::onUpdateDisplay(const Event::UpdateDisplayText& event)
{
    setLineText(event.text, event.lineNum);
    draw();
}
