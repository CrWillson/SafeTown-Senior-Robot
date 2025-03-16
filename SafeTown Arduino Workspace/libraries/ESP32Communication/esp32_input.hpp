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

    bool _spaceAvailable(uint32_t numBytes, uint32_t bufferSpace = 0);
    uint8_t _getNextImgNumber();

    // Packet _processBasicPacket();
    // Packet _processImagePacket();

    EventManager* eventManager;

    std::string imageDir;
    std::string imagePrefix = "image";
    std::string imageSuffix = ".bin";
    uint8_t imageNumber;

    static constexpr uint8_t IMAGE_ROWS = 96;
    static constexpr uint8_t IMAGE_COLS = 96;
    static constexpr uint8_t IMAGE_BPP  = 2;
    static constexpr uint16_t IMAGE_SIZE = IMAGE_ROWS * IMAGE_COLS * IMAGE_BPP;

    char imageBuffer[96*96*4];
};