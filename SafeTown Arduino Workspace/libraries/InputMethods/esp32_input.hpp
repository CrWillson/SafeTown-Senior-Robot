#pragma once

#include <string>
#include "event_manager.hpp"

struct ESP32RecvPacket {
    int8_t whiteDist;
    bool stopDetected;
    bool carDetected;
    uint16_t image[96][96];
};

struct ESP32SendPacket {
    char command[6];
    char label[6];
    int16_t data;
};

class ESP32Camera {
public:
    static ESP32Camera& getInstance() {
        static ESP32Camera instance;
        return instance;
    }
    ESP32Camera(const ESP32Camera&) = delete;
    ESP32Camera& operator=(const ESP32Camera&) = delete;

    void init();

    void onValueChanged(const Event::ValueChangedEvent& e);

    template<typename T>
    void sendData(const T& packet) {
        Serial1.write((const uint8_t*)&packet, sizeof(T));
        Serial1.flush();
    }

    template<typename T>
    bool recvData(T& packet) {
        if (Serial1.available() >= sizeof(T)) {
            Serial1.readBytes((char*)&packet, sizeof(T));
            return true;
        }
        return false;
    }

private:
    EventManager* eventManager;

    ESP32Camera() = default;
    ~ESP32Camera() = default;

};