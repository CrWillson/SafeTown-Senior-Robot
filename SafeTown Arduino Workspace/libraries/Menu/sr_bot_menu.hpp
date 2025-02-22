#pragma once

#include "menu.hpp"
#include "file_menu_page.hpp"

class SrMenu : public Menu {
protected:
    inline void buildMenu() override {
        addPage(new MenuPage("Home"));
        allPages.at("Home")->addLine(new TextMenuLine("Home Page"));
        allPages.at("Home")->addLine(new TextMenuLine("Sr Robot Menu"));
        allPages.at("Home")->addLine(new SliderMenuLine("Slider:", "testSlider", 5, 0, 10));
        allPages.at("Home")->addLine(new ButtonMenuLine("Go to sensor values", [this]{ 
            this->setCurrentPage("Sensors"); 
        }));
        allPages.at("Home")->addLine(new ButtonMenuLine("Go to page 3", [this]{ 
            this->setCurrentPage("Page3"); 
        }));
        
        addPage(new MenuPage("Sensors"));
        allPages.at("Sensors")->addLine(new TextMenuLine("Sensor Values"));
        allPages.at("Sensors")->addLine(new ValueMenuLine("Dist:", "whiteDist"));
        allPages.at("Sensors")->addLine(new ValueMenuLine("Stop:", "stopDetect"));
        allPages.at("Sensors")->addLine(new ValueMenuLine("Slider Value:", "testSlider"));
        allPages.at("Sensors")->addLine(new ButtonMenuLine("Go to home page", [this]{ 
            this->setCurrentPage("Home"); 
        }));

        addPage(new FileMenuPage("FileMenu", "Home"));
    }

};