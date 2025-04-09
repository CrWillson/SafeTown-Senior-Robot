#pragma once

#include "menu.hpp"
#include "file_menu_page.hpp"
#include "file_stats_page.hpp"
#include "esp32_input.hpp"

/**
 * @brief Concrete child class of the Menu type. Specifies the construction of a menu for the Safe Town Senior Robot. 
 * @ingroup Menu
 */
class SrMenu : public Menu {
protected:
    inline void buildMenu() override {
        // HOME PAGE
        auto homePage = addPage("Home");
        homePage->addLine<TextMenuLine>("Home Page");
        homePage->addLine<TextMenuLine>("Sr Robot Menu");
        homePage->addLine<SpacerMenuLine>();
        homePage->addLine<ButtonMenuLine>("Sensor Values", [this]{
            this->setCurrentPage("Sensors");
        });
        homePage->addLine<ButtonMenuLine>("Esp32 settings", [this]{
            this->setCurrentPage("ESP32");
        });
        homePage->addLine<ButtonMenuLine>("File explorer", [this]{
            this->setCurrentPage("FileMenu");
        });
        homePage->addLine<ButtonMenuLine>("File stats", [this]{
            this->setCurrentPage("FileStats");
        });
        
        // SENSOR READINGS PAGE
        auto sensorPage = addPage("Sensors");
        sensorPage->addLine<TextMenuLine>("Sensor Values");
        sensorPage->addLine<SpacerMenuLine>();
        sensorPage->addLine<ValueMenuLine>("Dist:", "whiteDist");
        sensorPage->addLine<ValueMenuLine>("Stop:", "stopDetect");
        sensorPage->addLine<ValueMenuLine>("Steer:", "steerAngle");
        sensorPage->addLine<ValueMenuLine>("Next Turn:", "nextTurn");
        sensorPage->addLine<ButtonMenuLine>("Go to home page", [this]{
            this->setCurrentPage("Home");
        });

        // FILE SYSTEM PAGES
        addPage<FileMenuPage>("FileMenu", "Home");
        addPage<FileStatPage>("FileStats", "Home");

        // ESP32 CONTROL MAIN PAGE
        auto espPage = addPage("ESP32");
        espPage->addLine<TextMenuLine>("ESP32 Settings");
        espPage->addLine<SpacerMenuLine>();
        espPage->addLine<ButtonMenuLine>("Take Photo", []{ 
            EventManager::getInstance().publish(Event::RequestPhotoEvent());
        });
        espPage->addLine<ButtonMenuLine>("Reset Img Num", []{
            ESP32::getInstance().setImageNumber(0);
        });
        espPage->addLine<ButtonMenuLine>("Edit Parameters", [this]{ 
            this->setCurrentPage("ESP32Params"); 
        });
        espPage->addLine<ButtonMenuLine>("Go to home page", [this]{ 
            this->setCurrentPage("Home"); 
        });

        // ESP32 PARAMETER EDIT PAGE
        auto paramPage = addPage("ESP32Params");
        paramPage->addLine<TextMenuLine>("ESP32 Parameters");
        paramPage->addLine<SpacerMenuLine>();
        paramPage->addLine<TextMenuLine>("White Parmeters");
        paramPage->addLine<SliderMenuLine>("Red Limit:", "WhiteRedLimit", 200, 0, 255);
        paramPage->addLine<SliderMenuLine>("Green Limit:", "WhiteGreenLimit", 200, 0, 255);
        paramPage->addLine<SliderMenuLine>("Blue Limit:", "WhiteBlueLimit", 180, 0, 255);
        paramPage->addLine<SliderMenuLine>("Center Pos: ", "WhiteCenter", 28, 0, 95);
        paramPage->addLine<SpacerMenuLine>();
        paramPage->addLine<TextMenuLine>("Red Parmeters");
        paramPage->addLine<SliderMenuLine>("Percent Stop:", "PercentStop", 20, 1, 100);
        paramPage->addLine<SliderMenuLine>("Green Thresh:", "StopGreenToler", 15, 0, 100);
        paramPage->addLine<SliderMenuLine>("Blue Thresh:", "StopBlueToler", 20, 0, 100);
        paramPage->addLine<SliderMenuLine>("Stop Delay:", "StopDelay", 6, 0, 100);
        paramPage->addLine<SpacerMenuLine>();
        paramPage->addLine<TextMenuLine>("Red Box Params");
        paramPage->addLine<SliderMenuLine>("Red Box TL_X:", "RedBoxTLX", 15, 1, 95);
        paramPage->addLine<SliderMenuLine>("Red Box TL_Y:", "RedBoxTLY", 75, 1, 95);
        paramPage->addLine<SliderMenuLine>("Red Box BR_X:", "RedBoxBRX", 40, 1, 95);
        paramPage->addLine<SliderMenuLine>("Red Box BR_Y:", "RedBoxBRY", 85, 1, 95);
        paramPage->addLine<SpacerMenuLine>();
        paramPage->addLine<ButtonMenuLine>("Back", [this]{ 
            this->setCurrentPage("ESP32"); 
        });
        

    }

};