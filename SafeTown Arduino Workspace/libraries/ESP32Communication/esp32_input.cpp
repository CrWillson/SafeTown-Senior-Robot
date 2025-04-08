#include "esp32_input.hpp"
#include <LittleFS.h>


void ESP32::init(const std::string& imagedir)
{
    Serial1.setPollingMode(true);
    Serial1.begin(BAUD_RATE);  

    EventManager::getInstance().subscribe<Event::ValueChangedEvent>([this](const auto& event) {
        this->_onValueChange(event);
    });
    EventManager::getInstance().subscribe<Event::RequestPhotoEvent>([this](const auto& event) {
        this->sendPacket(EspCommand::CMD_REQUEST_IMAGE);
    });

    imageDir = imagedir;
    LittleFS.mkdir(imageDir.c_str());

    imageNumber = _getNextImageNumber();
}

void ESP32::sendPacket(const EspCommand cmd, const PacketLabel label, const int16_t d)
{
    PicoToEspPacket packet;
    packet.command = cmd;
    packet.label = label;
    packet.data = d;

    LOGLN("Sending packet: ");
    LOGF("Command: %d, Label: %d, Data: %d\n", static_cast<int>(packet.command), static_cast<int>(packet.label), packet.data);
    LOGF("Packet size: %d bytes\n", sizeof(PicoToEspPacket));

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
        _saveNextImageNumber(imageNumber);
        LOG("Saved image as: ");
        LOGLN(fileName.c_str());

    }
    
    return packet;
}


void ESP32::_onValueChange(const Event::ValueChangedEvent& e)
{
    PacketLabel label;
    bool validLabel = true;

    if (e.valueId == "WhiteRedLimit") {
        label = PacketLabel::PARAM_WHITE_RED_LIMIT;
    } else if (e.valueId == "WhiteGreenLimit") {
        label = PacketLabel::PARAM_WHITE_GREEN_LIMIT;
    } else if (e.valueId == "WhiteBlueLimit") {
        label = PacketLabel::PARAM_WHITE_BLUE_LIMIT;
    } else if (e.valueId == "PercentStop") {
        label = PacketLabel::PARAM_PERCENT_TO_STOP;
    } else if (e.valueId == "StopGreenToler") {
        label = PacketLabel::PARAM_STOP_GREEN_TOLERANCE;
    } else if (e.valueId == "StopBlueToler") {
        label = PacketLabel::PARAM_STOP_BLUE_TOLERANCE;
    } else if (e.valueId == "RedBoxTLX") {
        label = PacketLabel::PARAM_STOPBOX_TL_X;
    } else if (e.valueId == "RedBoxTLY") {
        label = PacketLabel::PARAM_STOPBOX_TL_Y;
    } else if (e.valueId == "RedBoxBRX") {
        label = PacketLabel::PARAM_STOPBOX_BR_X;
    } else if (e.valueId == "RedBoxBRY") {
        label = PacketLabel::PARAM_STOPBOX_BR_Y;
    } else if (e.valueId == "WhiteCenter") {
        label = PacketLabel::PARAM_WHITE_CENTER_POS;
    } else {
        validLabel = false;
    }

    if (validLabel) {
        sendPacket(EspCommand::CMD_SET_PARAM, label, static_cast<int16_t>(stoi(e.newValue)));
    }
}

bool ESP32::_spaceAvailable(uint32_t numBytes, uint32_t bufferSpace)
{
    FSInfo fs_info;
    LittleFS.info(fs_info);

    return (fs_info.totalBytes - fs_info.usedBytes - bufferSpace) > numBytes;
}

uint8_t ESP32::_getNextImageNumber()
{
    // Check if the file exists
    if (!LittleFS.exists(CONFIG_FILE)) {
        // If not, create it and initialize the imageNumber to 0
        File configFile = LittleFS.open(CONFIG_FILE, "w");
        if (configFile) {
            configFile.println("0");
            configFile.close();
            return 0;
        }
    }

    File configFile = LittleFS.open(CONFIG_FILE, "r");
    if (configFile) {
        String line = configFile.readStringUntil('\n');
        configFile.close();
        return static_cast<uint8_t>(line.toInt());
    }
    return 0;
}

void ESP32::_saveNextImageNumber(const uint8_t imgNum) {
    File configFile = LittleFS.open(CONFIG_FILE, "w");
    if (configFile) {
        std::string numStr = std::to_string(imgNum);
        configFile.println(numStr.c_str());
        configFile.close();
    }
}