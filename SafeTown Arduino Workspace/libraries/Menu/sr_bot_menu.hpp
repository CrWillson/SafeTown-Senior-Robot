#pragma once

#include "menu.hpp"
#include "file_menu_page.hpp"
#include "menu_line.hpp"

class SrMenu : public Menu {
protected:
    inline void buildMenu() override {
        // HOME PAGE
        addPage(new MenuPage("Home"));
        allPages.at("Home")->addLine(new TextMenuLine("Home Page"));
        allPages.at("Home")->addLine(new TextMenuLine("Sr Robot Menu"));
        allPages.at("Home")->addLine(new SpacerMenuLine());
        allPages.at("Home")->addLine(new ButtonMenuLine("Sensor values", [this]{ 
            this->setCurrentPage("Sensors"); 
        }));
        allPages.at("Home")->addLine(new ButtonMenuLine("ESP32 settings", [this]{ 
            this->setCurrentPage("ESP32"); 
        }));
        allPages.at("Home")->addLine(new ButtonMenuLine("File explorer", [this]{ 
            this->setCurrentPage("FileMenu"); 
        }));
        
        // SENSOR READINGS PAGE
        addPage(new MenuPage("Sensors"));
        allPages.at("Sensors")->addLine(new TextMenuLine("Sensor Values"));
        allPages.at("Sensors")->addLine(new SpacerMenuLine());
        allPages.at("Sensors")->addLine(new ValueMenuLine("Dist:", "whiteDist"));
        allPages.at("Sensors")->addLine(new ValueMenuLine("Stop:", "stopDetect"));
        allPages.at("Sensors")->addLine(new ButtonMenuLine("Go to home page", [this]{ 
            this->setCurrentPage("Home"); 
        }));

        // FILE SYSTEM PAGES
        addPage(new FileMenuPage("FileMenu", "Home"));
        addPage(new FileStatPage("FileStats", "Home"));

        // ESP32 CONTROL MAIN PAGE
        addPage(new MenuPage("ESP32"));
        allPages.at("ESP32")->addLine(new TextMenuLine("ESP32 Settings"));
        allPages.at("ESP32")->addLine(new SpacerMenuLine());
        allPages.at("ESP32")->addLine(new ButtonMenuLine("Edit Parameters", [this]{ 
            this->setCurrentPage("ESP32Params"); 
        }));
        allPages.at("ESP32")->addLine(new ButtonMenuLine("Go to home page", [this]{ 
            this->setCurrentPage("Home"); 
        }));

        // ESP32 PARAMETER EDIT PAGE
        addPage(new MenuPage("ESP32Params"));
        allPages.at("ESP32Params")->addLine(new TextMenuLine("ESP32 Parameters"));
        allPages.at("ESP32Params")->addLine(new SpacerMenuLine());
        allPages.at("ESP32Params")->addLine(new TextMenuLine("White Parmeters"));
        allPages.at("ESP32Params")->addLine(new SliderMenuLine("Red Limit:", "WRLIM", 200, 0, 255));
        allPages.at("ESP32Params")->addLine(new SliderMenuLine("Green Limit:", "WGLIM", 200, 0, 255));
        allPages.at("ESP32Params")->addLine(new SliderMenuLine("Blue Limit:", "WBLIM", 180, 0, 255));
        allPages.at("ESP32Params")->addLine(new SpacerMenuLine());
        allPages.at("ESP32Params")->addLine(new ButtonMenuLine("Back", [this]{ 
            this->setCurrentPage("ESP32"); 
        }));
        

    }

};