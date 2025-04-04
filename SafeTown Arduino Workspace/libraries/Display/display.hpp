#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <vector>
#include <string>
#include <array>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "event_types.hpp"

/**
 * @brief Class to handle drawing to the display.
 * 
 */
class Display {
public:
    Display() = default;
    ~Display() = default;

    /**
     * @brief Initialize the display object
     * 
     */
    void init();

    /**
     * @brief Clear the screen
     * 
     */
    void clearDisplay();

    /**
     * @brief Write the contents of the stored lines onto the display
     * 
     */
    void draw();

    /**
     * @brief Set the line number to the supplied string
     * 
     * @param str - The text to set
     * @param lineNum - The line number to change. Ranges from 0-7.
     */
    void setLineText(std::string str, uint8_t lineNum);

    /**
     * @brief Set the line number to the supplied string with a T/F following
     * 
     * @param str - The text to set
     * @param value - The bool value appended to the line
     * @param lineNum - The line number to change. Ranges from 0-7.
     */
    void setLineText(std::string str, int value, uint8_t lineNum);
    
    /**
     * @brief Set the line number to the supplied string with an 8-bit value following
     * 
     * @param str - The text to set
     * @param value - The value to append to the line
     * @param lineNum - The line number to change. Ranges from 0-7.
     */
    void setLineText(std::string str, bool value, uint8_t lineNum); 

    /**
     * @brief Set the line number to the supplied string with a second string appended
     * 
     * @param str - The text to set
     * @param value - The string to append to the line
     * @param lineNum - The line number to change. Ranges from 0-7.
     */
    void setLineText(std::string str, std::string value, uint8_t lineNum);

    /**
     * @brief Print a set of lines to the screen
     * 
     * @param lines - The lines to print
     */
    void printLines(std::vector<std::string> lines);
    
    
private:
    Adafruit_SSD1306* screen;
    
    std::array<std::string, 8> _lines;
    
    void _onPageUpdated(const Event::PageChangedEvent& e);
};

#endif //DISPLAY_HPP