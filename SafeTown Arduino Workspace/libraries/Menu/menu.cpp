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

void Menu::setCurrentPage(const std::string &label)
{
    if (allPages.find(label) != allPages.end()) {
        if (currentPage) currentPage->onPageExit();
        currentPage = allPages.at(label).get();
        currentPage->onPageLoad();

        eventManager->publish(Event::PageChangedEvent(currentPage->getVisibleText()));
    } else {
        LOG("Error: Page with label '");
        LOG(label.c_str());
        LOGLN("' not found.");
    }
}

void Menu::onScrollUp(const Event::Event &e)
{    
    currentPage->scrollUp();
}

void Menu::onScrollDown(const Event::Event &e)
{    
    currentPage->scrollDown();
}

void Menu::onSelect(const Event::Event &e)
{
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
