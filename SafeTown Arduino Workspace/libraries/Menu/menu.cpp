#include "menu.hpp"


void Menu::init()
{
    eventManager = &EventManager::getInstance(); // Get the singleton instance of EventManager
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
    eventManager->subscribe<Event::ValueRequestEvent>([this](const auto& event) {
        this->onValueRequest(event);
    });

    buildMenu();

    eventManager->publish(Event::PageChangedEvent(currentPage->getVisibleText()));
}

void Menu::addPage(MenuPage* page)
{
    allPages[page->label] = std::shared_ptr<MenuPage>(page);
    allPages[page->label]->parentMenu = this;

    // If it's the first page to be added then initialize the current page there
    if (allPages.size() == 1) {
        setCurrentPage(page->label);
    }
}

void Menu::setCurrentPage(const std::string &label)
{
    if (allPages.find(label) != allPages.end()) {
        currentPage = allPages.at(label).get();
        currentPage->onPageLoad();

        eventManager->publish(Event::PageChangedEvent(currentPage->getVisibleText()));
    } else {
        Serial.print("Error: Page with label '");
        Serial.print(label.c_str());
        Serial.println("' not found.");
    }
}

void Menu::onScrollUp(const Event::Event &e)
{
    Serial.println("Scroll Up Event registered");
    
    currentPage->scrollUp();
}

void Menu::onScrollDown(const Event::Event &e)
{    
    Serial.println("Scroll Down Event registered");

    currentPage->scrollDown();
}

void Menu::onSelect(const Event::Event &e)
{
    Serial.println("Select Event registered");

    currentPage->select();
}

void Menu::onValueChange(const Event::ValueChangedEvent &e)
{  
    if (currentPage->onValueChange(e)) {
        eventManager->publish(Event::PageChangedEvent(currentPage->getVisibleText()));
    }
}

void Menu::onValueRequest(const Event::ValueRequestEvent &e)
{
    for (const auto& [key, page] : allPages) {
        if (page->onValueRequested(e.valueId)) { break; }
    }
}
