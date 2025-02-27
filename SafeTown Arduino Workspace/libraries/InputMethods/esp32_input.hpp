#pragma once

#include "event_manager.hpp"
#include <cstdint>
#include "Arduino.h"


struct ESP32RecvPacket
{
    int8_t whiteDist;
    bool stopDetected;
    uint16_t image[96][96];
};

struct ESP32SendPacket
{
    char command[6];
    char label[6];
    int16_t data;
};

class ESP32Input {
public:
    static ESP32Input& getInstance() {
        static ESP32Input instance;
        return instance;
    }

    template<typename T>
    void sendPacket(const T& packet);

    template<typename T>
    void receivePacket(T& packet);

    void init();

private:
    ESP32Input() = default;
    ~ESP32Input() = default;

    ESP32Input(const ESP32Input&) = delete;
    ESP32Input& operator=(const ESP32Input&) = delete;

};