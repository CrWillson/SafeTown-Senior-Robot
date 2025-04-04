#pragma once

#include "menu.hpp"
#include "file_menu_page.hpp"
#include "file_stats_page.hpp"
#include "esp32_input.hpp"

/**
 * @brief Concrete child class of the Menu type. Specifies the construction of a menu for the Safe Town Senior Robot. 
 * 
 */
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
        allPages.at("Home")->addLine(new ButtonMenuLine("File stats", [this]{ 
            this->setCurrentPage("FileStats"); 
        }));
        
        // SENSOR READINGS PAGE
        addPage(new MenuPage("Sensors"));
        allPages.at("Sensors")->addLine(new TextMenuLine("Sensor Values"));
        allPages.at("Sensors")->addLine(new SpacerMenuLine());
        allPages.at("Sensors")->addLine(new ValueMenuLine("Dist:", "whiteDist"));
        allPages.at("Sensors")->addLine(new ValueMenuLine("Stop:", "stopDetect"));
        allPages.at("Sensors")->addLine(new ValueMenuLine("Steer:", "steerAngle"));
        allPages.at("Sensors")->addLine(new ValueMenuLine("Next Turn:", "nextTurn"));
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
        allPages.at("ESP32")->addLine(new ButtonMenuLine("Take Photo", []{ 
            EventManager::getInstance().publish(Event::RequestPhotoEvent());
        }));
        allPages.at("ESP32")->addLine(new ButtonMenuLine("Reset Img Num", []{
            ESP32::getInstance().setImageNumber(0);
        }));
        allPages.at("ESP32")->addLine(new ButtonMenuLine("Edit Parameters", [this]{ 
            this->setCurrentPage("ESP32Params"); 
        }));
        allPages.at("ESP32")->addLine(new ButtonMenuLine("Go to home page", [this]{ 
            this->setCurrentPage("Home"); 
        }));

        // ESP32 PARAMETER EDIT PAGE
        auto esppage = addPage(new MenuPage("ESP32Params"));
        esppage->addLine(new TextMenuLine("ESP32 Parameters"));
        esppage->addLine(new SpacerMenuLine());
        esppage->addLine(new TextMenuLine("White Parmeters"));
        esppage->addLine(new SliderMenuLine("Red Limit:", "WhiteRedLimit", 200, 0, 255));
        esppage->addLine(new SliderMenuLine("Green Limit:", "WhiteGreenLimit", 200, 0, 255));
        esppage->addLine(new SliderMenuLine("Blue Limit:", "WhiteBlueLimit", 180, 0, 255));
        esppage->addLine(new SpacerMenuLine());
        esppage->addLine(new TextMenuLine("Red Parmeters"));
        esppage->addLine(new SliderMenuLine("Percent Stop:", "PercentStop", 20, 1, 100));
        esppage->addLine(new SliderMenuLine("Green Thresh:", "StopGreenToler", 15, 0, 100));
        esppage->addLine(new SliderMenuLine("Blue Thresh:", "StopBlueToler", 20, 0, 100));
        esppage->addLine(new SliderMenuLine("Stop Delay:", "StopDelay", 6, 0, 100));
        esppage->addLine(new SpacerMenuLine());
        esppage->addLine(new TextMenuLine("Red Box Params"));
        esppage->addLine(new SliderMenuLine("Red Box TL_X:", "RedBoxTLX", 15, 1, 95));
        esppage->addLine(new SliderMenuLine("Red Box TL_Y:", "RedBoxTLY", 75, 1, 95));
        esppage->addLine(new SliderMenuLine("Red Box BR_X:", "RedBoxBRX", 40, 1, 95));
        esppage->addLine(new SliderMenuLine("Red Box BR_Y:", "RedBoxBRY", 85, 1, 95));
        esppage->addLine(new SpacerMenuLine());
        esppage->addLine(new ButtonMenuLine("Back", [this]{ 
            this->setCurrentPage("ESP32"); 
        }));
        

    }

};