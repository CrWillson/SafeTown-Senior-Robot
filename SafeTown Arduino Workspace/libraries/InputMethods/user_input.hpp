#pragma once

#define ENC_B    20
#define ENC_A    18
#define ENC_S    19

#include "event_manager.hpp"
#include "Arduino.h"

class UIManager {
public:
    UIManager(EventManager* manager);

    static void EncoderLeft() {
        Event::EncoderLeft event(true);
        eventManager->publish(event);
    }

    static void EncoderRight() {
        Event::EncoderRight event(true);
        eventManager->publish(event);
    }

    static void EncoderPress() {
        eventManager->publish(Event::EncoderPress{});
    }

    static void EncoderRelease() {
        eventManager->publish(Event::EncoderRelease{});
    }


private:
    static EventManager* eventManager;
};