#pragma once

#include "event_manager.hpp"
#include <cstdint>
#include <string.h>
#include "Arduino.h"


class ESP32 {
public:
    static ESP32& getInstance() {
        static ESP32 instance;
        return instance;
    }

    struct RecvPacket
    {
        int8_t whiteDist;
        bool stopDetected;
        uint16_t image[96][96];
    };
    
    struct SendPacket
    {
        char command[6];
        char label[6];
        int16_t data;
    };

    struct AckPacket
    {
        bool ack;
        char label[6];
    };


    void sendPacket(const char* cmd, const char* lbl, const int16_t d);
    RecvPacket receivePacket();

    void init();

    void onValueChange(const Event::ValueChangedEvent& e);

private:
    ESP32() = default;
    ~ESP32() = default;

    ESP32(const ESP32&) = delete;
    ESP32& operator=(const ESP32&) = delete;

    EventManager* eventManager;

};