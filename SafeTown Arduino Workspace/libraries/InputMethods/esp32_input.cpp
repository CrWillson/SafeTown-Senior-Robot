#include "esp32_input.hpp"

void ESP32Input::init()
{
    Serial1.setPollingMode(true);
    Serial1.begin(500000);  //serial for the UART connection to the ESP32 CAM

}