#pragma once

#include "event_manager.hpp"
#include "communication_types.hpp"
#include <cstdint>
#include <cstring>
#include <string>
#include "Arduino.h"
#include <LittleFS.h>
#include "debug_logger.hpp"
#include <vector>


class ESP32 {
public:
    static ESP32& getInstance() {
        static ESP32 instance;
        return instance;
    }

    void sendPacket(const EspCommand cmd, const PacketLabel label = PacketLabel::NULL_LABEL, const int16_t d = 0);
    EspToPicoPacket receivePacket();

    void init(const std::string& imagedir);

    void onValueChange(const Event::ValueChangedEvent& e);

    void setImageNumber(const uint8_t imgNum) { 
        imageNumber = imgNum;
        _saveNextImageNumber(imgNum); 
    }

private:
    ESP32() = default;
    ~ESP32() = default;

    ESP32(const ESP32&) = delete;
    ESP32& operator=(const ESP32&) = delete;

    bool _spaceAvailable(uint32_t numBytes, uint32_t bufferSpace = 0);
    uint8_t _getNextImageNumber();
    void _saveNextImageNumber(const uint8_t imgNum);

    EventManager* eventManager;

    static constexpr char CONFIG_FILE[] = ".config";
    std::string imageDir;
    std::string imagePrefix = "image";
    std::string imageSuffix = ".bin";
    uint8_t imageNumber;

    static constexpr uint8_t IMAGE_ROWS = 96;
    static constexpr uint8_t IMAGE_COLS = 96;
    static constexpr uint8_t IMAGE_BPP  = 4;
    static constexpr uint16_t IMAGE_PADDING = 288;
    static constexpr uint16_t IMAGE_SIZE = (IMAGE_ROWS * IMAGE_COLS * IMAGE_BPP) + IMAGE_PADDING;
};