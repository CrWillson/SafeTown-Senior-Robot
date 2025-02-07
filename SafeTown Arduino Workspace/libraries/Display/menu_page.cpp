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
    lines.at(numLines)->eventManager = eventManager;
    numLines++;
}

void MenuPage::onScrollUp(const Event::Event &e)
{
    if (selectedLine > 0) {
        selectedLine--;
        eventManager->publish(Event::PageChangedEvent{});
    }
    if (selectedLine == topLine + 1 && topLine > 0) {
        topLine--;
        botLine--;
        eventManager->publish(Event::PageChangedEvent{});
    }
}

void MenuPage::onScrollDown(const Event::Event &e)
{
    if (selectedLine < numLines - 1) {
        selectedLine++;
        eventManager->publish(Event::PageChangedEvent{});
    }
    if (selectedLine == botLine - 1 && botLine < numLines - 1) {
        botLine++;
        topLine++;
        eventManager->publish(Event::PageChangedEvent{});
    }
}

void MenuPage::onSelect(const Event::Event &e)
{
    lines.at(selectedLine)->onSelect();
}
