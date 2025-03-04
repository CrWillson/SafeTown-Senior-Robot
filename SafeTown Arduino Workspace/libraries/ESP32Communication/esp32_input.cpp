#include "esp32_input.hpp"

void ESP32::init(const std::string& imagedir)
{
    Serial1.setPollingMode(true);
    Serial1.begin(500000);  //serial for the UART connection to the ESP32 CAM

    eventManager = &EventManager::getInstance();
    eventManager->subscribe<Event::ValueChangedEvent>([this](const auto& event) {
        this->onValueChange(event);
    });

    imageDir = imagedir;
    LittleFS.mkdir(imageDir.c_str());
}

void ESP32::sendPacket(const EspCommand cmd, const char* lbl, const int16_t d)
{
    PicoToEspPacket packet;
    packet.command = cmd;
    strncpy(packet.label, lbl, sizeof(packet.label) - 1);
    packet.label[sizeof(packet.label) - 1] = '\0'; // Ensure null-termination
    packet.data = d;

    Serial.println("Sending packet:");
    Serial.write((uint8_t*)&packet, sizeof(PicoToEspPacket));
    Serial.println("");

    Serial1.write((uint8_t*)&packet, sizeof(PicoToEspPacket));

    if (!receiveAck(lbl)) {
        Serial.println("Failed to receive ack");
    } else {
        Serial.print("Received ack: ");
        Serial.println(lbl);
    }
}

EspToPicoPacket ESP32::receivePacket()
{
    EspToPicoPacket packet;

    if (Serial1.available()) {
        Serial1.readBytes((char*)&packet, sizeof(EspToPicoPacket));
    }
    if (packet.packetType == EspPacketType::IMAGE_PACKET) {
        // Read in 96*96*2 more bytes for the image
        uint16_t image[96][96];
        Serial1.readBytes((char*)image, sizeof(image));

        if (imageCount >= MAX_IMAGE_COUNT) {
            Serial.println("Unable to take more photos, max image count reached");
            return packet;
        }

        // Save the image to a new file in the /images directory
        std::string filename = imageDir + "image" + std::to_string(imageCount) + ".bin";
        File file = LittleFS.open(filename.c_str(), "w");
        if (!file) {
            Serial.println("Failed to open file for writing");
        } else {
            file.write((uint8_t*)image, sizeof(image));
            file.close();
            imageCount++;
        }
    }

    return packet;
}

bool ESP32::receiveAck(const char label[6], unsigned long timeout)
{
    unsigned long startTime = millis();
    while (millis() - startTime < timeout) {
        if (Serial1.available() >= sizeof(AckPacket)) {
            AckPacket packet;
            Serial1.readBytes((char*)&packet, sizeof(AckPacket));
            if (packet.ack && strcmp(packet.label, label) == 0) {
                return true;
            }
        }
    }
    return false;
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