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

    addPage("Home");
    allPages.at("Home")->addLine(new TextMenuLine("Home Page"));
    allPages.at("Home")->addLine(new TextMenuLine("Sr Robot Menu"));
    allPages.at("Home")->addLine(new SliderMenuLine("Slider: ", 5, 0, 10));
    allPages.at("Home")->addLine(new ButtonMenuLine("Go to sensor values", [this]{ 
        this->setCurrentPage("Sensors"); 
    }));
    allPages.at("Home")->addLine(new ButtonMenuLine("Go to page 3", [this]{ 
        this->setCurrentPage("Page3"); 
    }));
    
    addPage("Sensors");
    allPages.at("Sensors")->addLine(new TextMenuLine("Sensor Values"));
    allPages.at("Sensors")->addLine(new ValueMenuLine("Dist: ", "whiteDist"));
    allPages.at("Sensors")->addLine(new ValueMenuLine("Stop: ", "stopDetect"));
    allPages.at("Sensors")->addLine(new ButtonMenuLine("Go to home page", [this]{ 
        this->setCurrentPage("Home"); 
    }));

    addPage("Page3");
    allPages.at("Page3")->addLine(new TextMenuLine("Third Page"));
    allPages.at("Page3")->addLine(new TextMenuLine("It's a menu :)"));
    allPages.at("Page3")->addLine(new ButtonMenuLine("Go to home page", [this]{
        this->setCurrentPage("Home");
    }));
    allPages.at("Page3")->addLine(new ButtonMenuLine("Go to sensor values", [this] {
        this->setCurrentPage("Sensors");
    }));

    eventManager->publish(Event::PageChangedEvent{});
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

void Menu::onScrollUp(const Event::Event &e)
{
    Serial.println("Scroll Up Event registered");
    
    if (currentPage->scrollUp()) {
        eventManager->publish(Event::PageChangedEvent{});
    }

}

void Menu::onScrollDown(const Event::Event &e)
{    
    Serial.println("Scroll Down Event registered");

    if (currentPage->scrollDown()) {
        eventManager->publish(Event::PageChangedEvent{});
    }
}

void Menu::onSelect(const Event::Event &e)
{
    Serial.println("Select Event registered");

    if (currentPage->select()) {
        eventManager->publish(Event::PageChangedEvent{});
    }
}

void Menu::onValueChange(const Event::ValueChangedEvent &e)
{  
    if (currentPage->onValueChange(e)) {
        eventManager->publish(Event::PageChangedEvent{});
    }
}
