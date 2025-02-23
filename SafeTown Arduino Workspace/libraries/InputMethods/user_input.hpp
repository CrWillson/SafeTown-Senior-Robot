#pragma once

#define ENC_B    20
#define ENC_A    18
#define ENC_S    19

#include "event_manager.hpp"
#include "Arduino.h"

class UIManager {
public:
    // Delete copy constructor and assignment operator to prevent copying
    UIManager(const UIManager&) = delete;
    UIManager& operator=(const UIManager&) = delete;

    // Static method to access the single instance of the class
    static UIManager& getInstance() {
        static UIManager instance;
        return instance;
    }
    
    void init();
    
    static void updateEncoder();
    
    static void EncoderPress() {
        Serial.println("Encoder Press event published");
        eventManager->publish(Event::EncoderPress{});
    }
    
private:
    UIManager() = default;
    
    static EventManager* eventManager;

    static volatile uint8_t lastState;
    static constexpr uint8_t debounceDelay = 2;
};