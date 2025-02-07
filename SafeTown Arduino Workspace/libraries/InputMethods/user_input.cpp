#include "user_input.hpp"

EventManager* UIManager::eventManager = nullptr;
volatile uint8_t UIManager::lastState = 0;

void UIManager::init(EventManager *manager)
{
    eventManager = manager;

    // Encoder setup
    pinMode(ENC_B, INPUT);
    pinMode(ENC_A, INPUT);
    pinMode(ENC_S, INPUT);
    attachInterrupt(digitalPinToInterrupt(ENC_B), updateEncoder, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ENC_A), updateEncoder, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ENC_S), EncoderPress, RISING);

}

void UIManager::updateEncoder() {
    uint8_t a = digitalRead(ENC_A);
    uint8_t b = digitalRead(ENC_B);
    uint8_t state = (a << 1) | b; // combine bits
    
    // For simplicity, here’s a very basic example (which might need refinement):
    if (lastState == 0b00) {
        if (state == 0b01) {
            // Possibly turning right
            delay(debounceDelay);
            Serial.println("Encoder Right event published");
            eventManager->publish(Event::EncoderRight{});
        } else if (state == 0b10) {
            // Possibly turning left
            delay(debounceDelay);
            Serial.println("Encoder Left event published");
            eventManager->publish(Event::EncoderLeft{});
        }
    } else if (lastState == 0b11) {
        lastState = 0;
    }
    lastState = state;
}


