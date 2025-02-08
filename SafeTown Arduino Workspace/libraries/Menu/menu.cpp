#include "menu.hpp"


void Menu::initMenu(EventManager* manager)
{
    eventManager = manager;
    eventManager->subscribe<Event::EncoderLeft>([this](const auto& event) {
        this->onScrollUp(event);
    });
    eventManager->subscribe<Event::EncoderRight>([this](const auto& event) {
        this->onScrollDown(event);
    });
    eventManager->subscribe<Event::EncoderPress>([this](const auto& event) {
        this->onSelect(event);
    });
    eventManager->subscribe<Event::ValueChangedEvent>([this](const auto& event) {
        this->onValueChange(event);
    });

    buildMenu();

    eventManager->publish(Event::PageChangedEvent(currentPage->getVisibleText()));
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

    eventManager->publish(Event::PageChangedEvent(currentPage->getVisibleText()));
}

void Menu::onScrollUp(const Event::Event &e)
{
    Serial.println("Scroll Up Event registered");
    
    if (currentPage->scrollUp()) {
        eventManager->publish(Event::PageChangedEvent(currentPage->getVisibleText()));
    }

}

void Menu::onScrollDown(const Event::Event &e)
{    
    Serial.println("Scroll Down Event registered");

    if (currentPage->scrollDown()) {
        eventManager->publish(Event::PageChangedEvent(currentPage->getVisibleText()));
    }
}

void Menu::onSelect(const Event::Event &e)
{
    Serial.println("Select Event registered");

    if (currentPage->select()) {
        eventManager->publish(Event::PageChangedEvent(currentPage->getVisibleText()));
    }
}

void Menu::onValueChange(const Event::ValueChangedEvent &e)
{  
    if (currentPage->onValueChange(e)) {
        eventManager->publish(Event::PageChangedEvent(currentPage->getVisibleText()));
    }
}
