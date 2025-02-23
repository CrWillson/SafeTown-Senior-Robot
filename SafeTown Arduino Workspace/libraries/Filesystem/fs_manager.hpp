#pragma once

#include <LittleFS.h>
#include <string>
#include "event_manager.hpp"

class FSManager {
public:
    // Delete copy constructor and assignment operator to prevent copying
    FSManager(const FSManager&) = delete;
    FSManager& operator=(const FSManager&) = delete;
    ~FSManager() = default;
    
    // Static method to access the single instance of the class
    static FSManager& getInstance() {
        static FSManager instance;
        return instance;
    }

    void init();

    void createFile(const std::string& fileName);
    void deleteFile(const std::string& fileName);
    void printFile(const std::string& fileName);

private:
    FSManager() = default;

    EventManager* eventManager;
};