#pragma once

#define ENC_B    20
#define ENC_A    18
#define ENC_S    19

#include "event_manager.hpp"
#include "Arduino.h"

class UIManager {
public:
    UIManager() = default;

    void initUI(EventManager* manager);

    static void updateEncoder();

    static void EncoderPress() {
        Serial.println("Encoder Press event published");
        eventManager->publish(Event::EncoderPress{});
    }

private:
    static EventManager* eventManager;

    static volatile uint8_t lastState;
    static constexpr uint8_t debounceDelay = 2;
};