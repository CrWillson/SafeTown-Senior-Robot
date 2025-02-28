#include "menu_page.hpp"
#include "menu.hpp"

MenuPage::MenuPage(const std::string& lbl) : label(lbl) 
{
    eventManager = &EventManager::getInstance();
}

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

void MenuPage::clearLines()
{
    lines.clear();
    numLines = 0;
    selectedLine = 0;
    topLine = 0;
    botLine = LINESPERSCREEN - 1;
}

bool MenuPage::onValueChange(const Event::ValueChangedEvent &e)
{
    for (int i = 0; i < numLines; i++) {
        if (auto line = std::dynamic_pointer_cast<ValueMenuLine>(lines.at(i))) {
            if (e.valueId == line->valueLabel) {
                line->value = e.newValue;
                return true;
            }
        }
    }
    return false;
}

bool MenuPage::onPageLoad()
{
    bool result = false;
    for (const auto& menuline : lines) {
        if (auto line = std::dynamic_pointer_cast<ValueMenuLine>(menuline)) {
            auto valReq = Event::ValueRequestEvent(line->valueLabel);
            eventManager->publish(valReq);
            result = true;
        }
    }
    return result;
}

bool MenuPage::onValueRequested(const std::string& reqLabel)
{
    bool result = false;
    for (const auto& menuline : lines) {
        if (auto line = std::dynamic_pointer_cast<SliderMenuLine>(menuline)) {
            if (line->valueLabel == reqLabel) {
                auto valResp = Event::ValueChangedEvent(reqLabel, std::to_string(line->value));
                eventManager->publish(valResp);
                result = true;
            }
        }
    }
    return result;
}

void MenuPage::scrollUp()
{
    if (auto line = std::dynamic_pointer_cast<SliderMenuLine>(lines.at(selectedLine))) {
        if (line->editing) {
            if (line->value > line->minVal) {
                line->value--;
                eventManager->publish(Event::PageChangedEvent(getVisibleText()));
            }
            return;
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

    if (result) { eventManager->publish(Event::PageChangedEvent(getVisibleText())); }
}

void MenuPage::scrollDown()
{
    if (auto line = std::dynamic_pointer_cast<SliderMenuLine>(lines.at(selectedLine))) {
        if (line->editing) {
            if (line->value < line->maxVal) {
                line->value++;
                eventManager->publish(Event::PageChangedEvent(getVisibleText()));
            }
            return;
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

    if (result) { eventManager->publish(Event::PageChangedEvent(getVisibleText())); }
}

void MenuPage::select()
{
    lines.at(selectedLine)->onSelect();

    if (auto line = std::dynamic_pointer_cast<SliderMenuLine>(lines.at(selectedLine))) {
        if (!line->editing) {
            auto valUpdate = Event::ValueChangedEvent(line->valueLabel, std::to_string(line->value));
            eventManager->publish(valUpdate);
        }
        eventManager->publish(Event::PageChangedEvent(getVisibleText()));
    }
}
