#include "esp32_input.hpp"
#include "communication_types.hpp"
#include <vector>

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

    imageNumber = _getNextImgNumber();
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

ESP32::Packet ESP32::receivePacket()
{
    // check for the SYNC_BYTES
    bool syncFound = false;
    uint32_t syncBuffer = 0;
    while (true) {
        if (Serial1.available() > 0) {
            uint8_t newByte = Serial1.read();
            syncBuffer = (syncBuffer << 8) | newByte;
            if (syncBuffer == SYNC_BYTES) {
                syncFound = true;
            }
        }
    }
    
    Serial.println("Found start of packet. Reading contents");

    // Get the type of packet (first byte of incoming packet)
    const PacketType packetType = static_cast<PacketType>(Serial1.peek());
    Serial.print("Found packet of type: ");

    switch(packetType) {
    case (BASIC_PACKET):
        return _processBasicPacket();

    case (IMAGE_PACKET):
        return _processImagePacket();    

    default:
        return {0, false};
        break;
    };

}

bool ESP32::receiveAck(const char label[6], unsigned long timeout)
{
    unsigned long startTime = millis();
    while (millis() - startTime < timeout) {
        if (Serial1.available() >= sizeof(AckPacket)) {
            AckPacket packet;
            Serial1.readBytes((char*)&packet, sizeof(AckPacket));
            if (packet.ack && strcmp(packet.label, label) == 0) {
                Serial.print("Received ack for: ");
                Serial.println(packet.label);
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

bool ESP32::_spaceAvailable(uint32_t numBytes, uint32_t bufferSpace)
{
    FSInfo fs_info;
    LittleFS.info(fs_info);

    const uint32_t totalBytes = fs_info.totalBytes;
    const uint32_t usedBytes = fs_info.usedBytes;
    const uint32_t remainingBytes = totalBytes - usedBytes - bufferSpace;

    Serial.printf("Total size: %u bytes\n", totalBytes);
    Serial.printf("Used size: %u bytes\n", usedBytes);
    Serial.printf("Free space: %u bytes\n", remainingBytes);

    return remainingBytes > numBytes;
}

uint8_t ESP32::_getNextImgNumber()
{
    auto dir = LittleFS.openDir(imageDir.c_str());
    if (!dir.isDirectory()) {
        Serial.println("No image directory found.");
        return -1;
    }

    uint8_t maxIndex = -1;
    while (dir.next()) {
        auto name = dir.fileName();

        if (name.startsWith(String(imageDir.c_str()))) {
            name = name.substring(strlen(imageDir.c_str()));
        }

        if (name.startsWith(imagePrefix.c_str()) && name.endsWith(imageSuffix.c_str())) {
            String numberPart = name.substring(strlen(imagePrefix.c_str()), name.length() - strlen(imageSuffix.c_str()));
            int num = numberPart.toInt();
            
            if (num >= 0) {
                maxIndex = max(maxIndex, num);
            }
        }

    }

    return maxIndex + 1;
}

ESP32::Packet ESP32::_processBasicPacket()
{
    Serial.println("BASIC");

    EspToPicoPacket basicPacket;
    unsigned long startTime = millis();
    while (millis() - startTime < 1000) {
        if (Serial1.available() >= sizeof(EspToPicoPacket)) {
            Serial1.readBytes((char*)&basicPacket, sizeof(EspToPicoPacket));
            break;
        }
    }

    Serial.print("Packet Type: ");
    Serial.print(basicPacket.packetType);
    Serial.print(" White Dist: ");
    Serial.print(basicPacket.whiteDist);
    Serial.print(" Stop Detected: ");
    Serial.println(basicPacket.stopDetected);

    return {basicPacket.whiteDist, basicPacket.stopDetected};
}

ESP32::Packet ESP32::_processImagePacket()
{
    Serial.println("IMAGE");

    EspToPicoPacketImage imagePacket;
    unsigned long startTime = millis();
    while (millis() - startTime < 1000) {
        if (Serial1.available() >= sizeof(EspToPicoPacketImage)) {
            Serial1.readBytes((char*)&imagePacket, sizeof(EspToPicoPacketImage));
            break;
        }
    }

    Serial.print("Packet Type: ");
    Serial.print(imagePacket.packetType);
    Serial.print(" White Dist: ");
    Serial.print(imagePacket.whiteDist);
    Serial.print(" Stop Detected: ");
    Serial.println(imagePacket.stopDetected);

    // Read in packet.numImgBytes more bytes and save it to a file
    uint16_t image[IMAGE_ROWS][IMAGE_COLS];
    Serial1.readBytes((char*)image, sizeof(image));

    // Skip saving the photo if no space is available
    if (!_spaceAvailable(IMAGE_SIZE)) {
        return {imagePacket.whiteDist, imagePacket.stopDetected};
    }

    std::string fileName = imageDir + imagePrefix + std::to_string(imageNumber) + imageSuffix;
    File file = LittleFS.open(fileName.c_str(), "w");
    if (!file) {
        Serial.println("Failed to open file for writing");
        return {imagePacket.whiteDist, imagePacket.stopDetected};
    }

    Serial.print("Saving image to file: ");
    Serial.println(fileName.c_str());

    file.write((uint8_t*)image, sizeof(image));
    file.close();
    
    return {imagePacket.whiteDist, imagePacket.stopDetected};
}
