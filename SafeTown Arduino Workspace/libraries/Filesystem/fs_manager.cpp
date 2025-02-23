#include "fs_manager.hpp"

void FSManager::init()
{
    eventManager = &EventManager::getInstance(); // Get the singleton instance of EventManager

    if (!LittleFS.begin()) {
        Serial.println("LittleFS mount failed!");
        return;
    }

}
