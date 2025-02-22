#include "menu.hpp"


void Menu::initMenu(EventManager* manager)
{
    eventManager = manager;
    eventManager->subscribe<Event::EncoderRight>([this](const auto& event) {
        this->onScrollUp(event);
    });
    eventManager->subscribe<Event::EncoderLeft>([this](const auto& event) {
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
    allPages[page->label]->eventManager = eventManager;

    // If it's the first page to be added then initialize the current page there
    if (allPages.size() == 1) {
        setCurrentPage(page->label);
    }
}

void Menu::setCurrentPage(const std::string &label)
{
    currentPage = allPages.at(label);
    currentPage->onPageLoad();

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

void Menu::onValueRequest(const Event::ValueRequestEvent &e)
{
    for (const auto& [key, page] : allPages) {
        if (page->onValueRequested(e.valueId)) { break; }
    }
}
