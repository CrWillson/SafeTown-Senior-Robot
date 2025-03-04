#pragma once

#include "event_manager.hpp"
#include "communication_types.hpp"
#include <cstdint>
#include <cstring>
#include <string>
#include "Arduino.h"
#include <LittleFS.h>


class ESP32 {
public:
    static ESP32& getInstance() {
        static ESP32 instance;
        return instance;
    }

    void sendPacket(const EspCommand cmd, const char* lbl, const int16_t d);
    EspToPicoPacket receivePacket();
    bool receiveAck(const char label[6], unsigned long timeout = 1000);

    void init(const std::string& imagedir);

    void onValueChange(const Event::ValueChangedEvent& e);

private:
    ESP32() = default;
    ~ESP32() = default;

    ESP32(const ESP32&) = delete;
    ESP32& operator=(const ESP32&) = delete;

    EventManager* eventManager;

    std::string imageDir;

    uint8_t imageCount = 0;

    // TODO: Update to dynamically not allow more than a certain percentage of the total memory to be used
    // FSInfo fs_info;
    // LittleFS.info(fs_info);
    static constexpr uint8_t MAX_IMAGE_COUNT = 10;     // Set to limit the number of images it can save to the local FS

};