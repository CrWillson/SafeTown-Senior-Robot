#include "menu.hpp"

void Menu::initMenu(EventManager* manager)
{
    eventManager = manager;

    addPage("Home");
    allPages.at("Home")->addLine(new TextMenuLine("Home Page"));
    allPages.at("Home")->addLine(new ButtonMenuLine("Go to page 2", [this]{ this->setCurrentPage("Page2"); }));
    
    addPage("Page2");
    allPages.at("Page2")->addLine(new TextMenuLine("Home Page"));
    allPages.at("Page2")->addLine(new ButtonMenuLine("Go to page 2", [this]{ this->setCurrentPage("Home"); }));
}

void Menu::addPage(const std::string &label)
{
    auto newPage = std::make_shared<MenuPage>(this, label);
    allPages[label] = newPage;

    // If its the first page to be added then initialize the current page there
    if (allPages.size() == 1) {
        auto label = allPages.begin()->first;
        setCurrentPage(label);
    }
}

void Menu::setCurrentPage(const std::string &label)
{
    currentPage = allPages.at(label);

    eventManager->publish(Event::PageChangedEvent{});
}