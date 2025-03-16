#include "esp32_input.hpp"
#include "communication_types.hpp"
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
        Serial.println("Image incoming. Waiting for 96x96x2 bytes to save...");

        // Serial1.readBytes(imageBuffer, IMAGE_SIZE);
        // int index = 0;
        // while (index < IMAGE_SIZE) {
        //     if (Serial1.available()) {
        //         imageBuffer[index++] = Serial1.read();
        //     }
        // }
        
        // Skip saving the photo if no space is available
        if (!_spaceAvailable(IMAGE_SIZE)) {
            Serial.println("No space available. Not saving image");
            return packet;
        }
        
        std::string fileName = imageDir + imagePrefix + std::to_string(imageNumber) + imageSuffix;
        File file = LittleFS.open(fileName.c_str(), "w");
        if (!file) {
            Serial.println("Failed to open file for writing");
            return packet;
        }
        Serial.print("Opening file: ");
        Serial.println(fileName.c_str());

        // Read in an entire line until and including a newline char
        Serial.println("|------------------------------------------------------------------------------------------------|");
        Serial.print("|");
        for (int i = 0; i < IMAGE_ROWS; i++) {
            String line = Serial1.readStringUntil('\n');
            file.println(line);
            // Serial.println(line);
            Serial.print(".");
        }
        Serial.println("");

        file.close();
        imageNumber++;
        Serial.print("Saved image as: ");
        Serial.println(fileName.c_str());

        // for (int y = 0; y < IMAGE_ROWS; y++) {
        //     for (int x = 0; x < IMAGE_COLS; x++) {
        //         uint8_t byte1 = Serial1.read();
        //         uint8_t byte2 = Serial1.read();
        //         uint16_t pixel = (byte1 << 8) | byte2;

        //         Serial.print("Read pixel: ");
        //         Serial.println(pixel, HEX);

        //         image[y][x] = pixel;
        //     }
        // }



        // Serial.print("Saving image to file: ");
        // Serial.println(fileName.c_str());

        // file.write((uint8_t*)image, sizeof(image));
        // file.close();

        // imageNumber++;
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

    // Serial.printf("Total size: %u bytes\n", totalBytes);
    // Serial.printf("Used size: %u bytes\n", usedBytes);
    // Serial.printf("Free space: %u bytes\n", remainingBytes);

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
