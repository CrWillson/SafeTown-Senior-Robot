#include "fs_manager.hpp"

void FSManager::init()
{
    eventManager = &EventManager::getInstance(); // Get the singleton instance of EventManager

    if (!LittleFS.begin()) {
        Serial.println("LittleFS mount failed!");
        return;
    }

}

void FSManager::createFile(const std::string &fileName)
{
    File file = LittleFS.open(fileName.c_str(), "w");
    if (!file) {
        Serial.println("Failed to create file!");
        return;
    }
    file.close();
}

void FSManager::deleteFile(const std::string &fileName)
{
    if (!LittleFS.remove(fileName.c_str())) {
        Serial.println("Failed to delete file!");
    }
}

void FSManager::printFile(const std::string &fileName)
{
    File file = LittleFS.open(fileName.c_str(), "r");
    if (!file) {
        Serial.println("Failed to open file for reading!");
        return;
    }

    while (file.available()) {
        Serial.write(file.read());
    }
    file.close();
}
