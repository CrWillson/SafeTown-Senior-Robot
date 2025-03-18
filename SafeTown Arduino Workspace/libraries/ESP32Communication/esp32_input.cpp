#include "esp32_input.hpp"
#include "communication_types.hpp"
#include "debug_logger.hpp"
#include <vector>

void ESP32::init(const std::string& imagedir)
{
    Serial1.setPollingMode(true);
    Serial1.begin(115200);  //serial for the UART connection to the ESP32 CAM

    eventManager = &EventManager::getInstance();
    eventManager->subscribe<Event::ValueChangedEvent>([this](const auto& event) {
        this->onValueChange(event);
    });

    imageDir = imagedir;
    LittleFS.mkdir(imageDir.c_str());

    imageNumber = 0;
}

void ESP32::sendPacket(const EspCommand cmd, const char* lbl, const int16_t d)
{
    PicoToEspPacket packet;
    packet.command = cmd;
    strncpy(packet.label, lbl, sizeof(packet.label) - 1);
    packet.label[sizeof(packet.label) - 1] = '\0'; // Ensure null-termination
    packet.data = d;

    LOGLN("Sending packet");

    Serial1.write((uint8_t*)&SYNC_BYTES, sizeof(SYNC_BYTES));
    Serial1.write((uint8_t*)&packet, sizeof(PicoToEspPacket));
}


EspToPicoPacket ESP32::receivePacket()
{
    // check for the SYNC_BYTES
    bool syncFound = false;
    uint32_t syncBuffer = 0;
    while (!syncFound) {
        if (Serial1.available() > 0) {
            uint8_t newByte = Serial1.read();
            syncBuffer = (syncBuffer >> 8) | (newByte << 24);
            if (syncBuffer == SYNC_BYTES) {
                syncFound = true;
            }
        }
    }
    
    EspToPicoPacket packet;

    unsigned long startTime = millis();
    while (millis() - startTime < 1000) {
        if (Serial1.available() >= sizeof(EspToPicoPacket)) {
            Serial1.readBytes((char*)&packet, sizeof(EspToPicoPacket));
            break;
        }
    }

    // Check if an image is incoming and save it
    if (packet.imageIncluded) {
        LOGLN("Image incoming. Waiting for 96x96x2 bytes to save...");
        
        // Skip saving the photo if no space is available
        if (!_spaceAvailable(IMAGE_SIZE)) {
            LOGLN("No space available. Not saving image");
            return packet;
        }
        
        std::string fileName = imageDir + imagePrefix + std::to_string(imageNumber) + imageSuffix;
        File file = LittleFS.open(fileName.c_str(), "w");
        if (!file) {
            LOGLN("Failed to open file for writing");
            return packet;
        }
        LOG("Opening file: ");
        LOGLN(fileName.c_str());

        // Read in an entire line until and including a newline char
        LOGLN("|------------------------------------------------------------------------------------------------|");
        LOG("|");
        for (int i = 0; i < IMAGE_ROWS; i++) {
            String line = Serial1.readStringUntil('\n');
            file.println(line);
            // Serial.println(line);
            LOG(".");
        }
        LOGLN("");

        file.close();
        imageNumber++;
        LOG("Saved image as: ");
        LOGLN(fileName.c_str());

    }
    
    return packet;
}


void ESP32::onValueChange(const Event::ValueChangedEvent& e)
{
    if (e.valueId == "WRLIM") {
        sendPacket(EspCommand::CMD_SET_PARAM, "WRLIM", static_cast<int16_t>(stoi(e.newValue)));
    }
    else if (e.valueId == "WGLIM") {
        sendPacket(EspCommand::CMD_SET_PARAM, "WGLIM", static_cast<int16_t>(stoi(e.newValue)));
    }
    else if (e.valueId == "WBLIM") {
        sendPacket(EspCommand::CMD_SET_PARAM, "WBLIM", static_cast<int16_t>(stoi(e.newValue)));
    }
}

bool ESP32::_spaceAvailable(uint32_t numBytes, uint32_t bufferSpace)
{
    FSInfo fs_info;
    LittleFS.info(fs_info);

    return (fs_info.totalBytes - fs_info.usedBytes - bufferSpace) > numBytes;
}
