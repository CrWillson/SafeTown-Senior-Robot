#include "esp32_input.hpp"

void ESP32Camera::init()
{
    Serial1.setPollingMode(true);
    Serial1.begin(500000);  //serial for the UART connection to the ESP32 CAM
    
    eventManager = &EventManager::getInstance();
    eventManager->subscribe<Event::ValueChangedEvent>([this](const Event::ValueChangedEvent& e) {
        this->onValueChanged(e);
    });


}

void ESP32Camera::onValueChanged(const Event::ValueChangedEvent &e)
{
    if (strcmp(e.valueId.c_str(), "WRLIM") == 0 || 
        strcmp(e.valueId.c_str(), "WGLIM") == 0 || 
        strcmp(e.valueId.c_str(), "WBLIM") == 0) 
    {     
        ESP32SendPacket packet = { "SETPR", "", static_cast<int16_t>(std::stoi(e.newValue)) };
        strncpy(packet.label, e.valueId.c_str(), 5);
        packet.label[5] = '\0';
        
        sendData(packet);
    }
}
