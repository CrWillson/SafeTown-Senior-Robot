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

void MenuPage::scrollUp()
{
    if (selectedLine > 0) {
        selectedLine--;
    }
    if (selectedLine == topLine + 1 && topLine > 0) {
        topLine--;
        botLine--;
    }
}

void MenuPage::scrollDown()
{
    if (selectedLine < numLines - 1) {
        selectedLine++;
    }
    if (selectedLine == botLine - 1 && botLine < numLines - 1) {
        botLine++;
        topLine++;
    }
}

void MenuPage::select()
{
    lines.at(selectedLine)->onSelect();
}
