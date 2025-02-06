#include "user_input.hpp"

EventManager* UIManager::eventManager = nullptr;

UIManager::UIManager(EventManager *manager)
{
    eventManager = manager;

    // Encoder setup
    pinMode(ENC_B, INPUT);
    pinMode(ENC_A, INPUT);
    pinMode(ENC_S, INPUT);
    attachInterrupt(digitalPinToInterrupt(ENC_B), EncoderLeft, RISING);
    attachInterrupt(digitalPinToInterrupt(ENC_A), EncoderRight, RISING);
    attachInterrupt(digitalPinToInterrupt(ENC_S), EncoderPress, RISING);
    attachInterrupt(digitalPinToInterrupt(ENC_S), EncoderRelease, FALLING);

}