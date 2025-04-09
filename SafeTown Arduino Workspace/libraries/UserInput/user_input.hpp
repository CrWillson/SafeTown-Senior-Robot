#pragma once

#define ENC_B    20
#define ENC_A    18
#define ENC_S    19

#include "event_manager.hpp"
#include "Arduino.h"

/**
 * @brief Class to handle gathering user input from the rotary encoder
 * @ingroup CommunicationUtilities
 */
class UIManager {
public:
    UIManager(const UIManager&) = delete;
    UIManager& operator=(const UIManager&) = delete;

    /**
     * @brief Get the Meyers singleton instance of the UIManager
     * 
     * @return UIManager&
     */
    static UIManager& getInstance() {
        static UIManager instance;
        return instance;
    }
    
    /**
     * @brief Initialize the encoder and attach interrupts
     * 
     */
    void init();
    
    /**
     * @brief Called upon the encoder being spun
     * 
     */
    static void updateEncoder();

    /**
     * @brief Called upon the encoder being pressed
     * 
     */
    static void EncoderPress() {
        eventManager->publish(Event::EncoderPress{});
    }
    
private:
    UIManager() = default;
    
    static EventManager* eventManager;

    static volatile uint8_t lastState;
    static constexpr uint8_t debounceDelay = 2;
};