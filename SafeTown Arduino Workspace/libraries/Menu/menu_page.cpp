#include "menu_page.hpp"

std::array<std::string, MenuPage::LINESPERSCREEN> MenuPage::getVisibleText() const
{
    std::array<std::string, LINESPERSCREEN> result;
    for (int i = 0; i < LINESPERSCREEN; i++) {
        if (topLine + i > numLines - 1) {
            break;
        }
        result[i] = lines[topLine + i]->getText(selectedLine == topLine + i);
    }
    return result;
}

void MenuPage::addLine(MenuLine *line)
{
    lines.push_back(std::shared_ptr<MenuLine>(line));
    numLines++;
}

bool MenuPage::onValueChange(const Event::ValueChangedEvent &e)
{
    for (int i = 0; i < LINESPERSCREEN; i++) {
        if (topLine + i > numLines - 1) {
            break;
        }

        if (lines[topLine + i]->getType() == LineType::Value) {
            auto line = std::static_pointer_cast<ValueMenuLine>(lines.at(topLine + i));

            if (e.valueId == line->valueLabel) {
                line->value = e.newValue;
                return true;
            }
        }
    }
    return false;
}

bool MenuPage::scrollUp()
{
    if (lines.at(selectedLine)->getType() == LineType::Slider) {
        Serial.println("Slider Line detected");
        auto line = std::static_pointer_cast<SliderMenuLine>(lines.at(selectedLine));
        Serial.print("Editing: ");
        Serial.print(line->editing);
        if (line->editing) {
            if (line->value > line->minVal) {
                line->value--;
                return true;
            } else {
                return false;
            }
        }
    }

    bool result = false;
    if (selectedLine > 0) {
        selectedLine--;
        result = true;
    }
    if (selectedLine == topLine + 1 && topLine > 0) {
        topLine--;
        botLine--;
    }
    return result;
}

bool MenuPage::scrollDown()
{
    if (lines.at(selectedLine)->getType() == LineType::Slider) {
        Serial.println("Slider Line detected");
        auto line = std::static_pointer_cast<SliderMenuLine>(lines.at(selectedLine));
        Serial.print("Editing: ");
        Serial.print(line->editing);
        if (line->editing) {
            if (line->value < line->maxVal) {
                line->value++;
                return true;
            } else {
                return false;
            }
        }
    }

    bool result = false;
    if (selectedLine < numLines - 1) {
        selectedLine++;
        result = true;
    }
    if (selectedLine == botLine - 1 && botLine < numLines - 1) {
        botLine++;
        topLine++;
    }
    return result;
}

bool MenuPage::select()
{
    Serial.print("Selecting line: ");
    Serial.println(selectedLine);
    lines.at(selectedLine)->onSelect();

    if (lines.at(selectedLine)->getType() == LineType::Slider) {
        return true;
    }

    return false;
}
