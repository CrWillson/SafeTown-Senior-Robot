#include "esp32_input.hpp"

void ESP32::init()
{
    Serial1.setPollingMode(true);
    Serial1.begin(500000);  //serial for the UART connection to the ESP32 CAM

    eventManager = &EventManager::getInstance();
    eventManager->subscribe<Event::ValueChangedEvent>([this](const auto& event) {
        this->onValueChange(event);
    });
}

void ESP32::sendPacket(const char* cmd, const char* lbl, const int16_t d)
{
    SendPacket packet;
    strncpy(packet.command, cmd, sizeof(packet.command) - 1);
    packet.command[sizeof(packet.command) - 1] = '\0'; // Ensure null-termination
    strncpy(packet.label, lbl, sizeof(packet.label) - 1);
    packet.label[sizeof(packet.label) - 1] = '\0'; // Ensure null-termination
    packet.data = d;

    Serial.println("Sending packet:");
    Serial.write((uint8_t*)&packet, sizeof(SendPacket));
    Serial.println("");

    Serial1.write((uint8_t*)&packet, sizeof(SendPacket));
}

ESP32::RecvPacket ESP32::receivePacket()
{
    RecvPacket packet;
    if (Serial1.available() >= sizeof(RecvPacket)) {
        Serial1.readBytes((char*)&packet, sizeof(RecvPacket));
    }
    return packet;
}

void ESP32::onValueChange(const Event::ValueChangedEvent& e)
{
    if (e.valueId == "WRLIM") {
        sendPacket("SETPR", "WRLIM", static_cast<int16_t>(stoi(e.newValue)));
    }
    else if (e.valueId == "WGLIM") {
        sendPacket("SETPR", "WGLIM", static_cast<int16_t>(stoi(e.newValue)));
    }
    else if (e.valueId == "WBLIM") {
        sendPacket("SETPR", "WBLIM", static_cast<int16_t>(stoi(e.newValue)));
    }
}