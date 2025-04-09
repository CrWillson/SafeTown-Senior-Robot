#pragma once

#include "event_manager.hpp"
#include "communication_types.hpp"
#include <cstdint>
#include <string>

/**
 * @defgroup CommunicationUtilities
 * @brief A group of classes to manage communicating with external sources
 * 
 */


/**
 * @brief Class to manage the communication with the attached ESP32. 
 * @ingroup CommunicationUtilities
 */
class ESP32 {
public:
    ~ESP32() = default;
    ESP32(const ESP32&) = delete;
    ESP32& operator=(const ESP32&) = delete;

    // Make the ESP32 a Meyers singleton class
    static ESP32& getInstance() {
        static ESP32 instance;
        return instance;
    }

    /**
     * @brief Send a command packet to the ESP32
     * 
     * @param cmd - The command to be sent
     * @param label - Additional label for the command
     * @param d - Additional 2 bytes of data
     */
    void sendPacket(const EspCommand cmd, const PacketLabel label = PacketLabel::NULL_LABEL, const int16_t d = 0);

    /**
     * @brief Receive a packet from the ESP32.
     * @details This function blocks until a packet is received. 
     * 
     * @return EspToPicoPacket 
     */
    EspToPicoPacket receivePacket();

    /**
     * @brief Initialize the ESP32.
     * 
     * @param imagedir - The directory in the virtual file system to save any images received from the ESP32.
     */
    void init(const std::string& imagedir);

    /**
     * @brief Set the number to append to the next received image file.
     * 
     * @param imgNum 
     */
    void setImageNumber(const uint8_t imgNum) { 
        imageNumber = imgNum;
        _saveNextImageNumber(imgNum); 
    }
    
private:
    ESP32() = default;
    
    bool _spaceAvailable(uint32_t numBytes, uint32_t bufferSpace = 0);      // Check if there is space in the file system
    uint8_t _getNextImageNumber();                                          // Get the next image number
    void _saveNextImageNumber(const uint8_t imgNum);                        // Save the next image number to a config file
    void _onValueChange(const Event::ValueChangedEvent& e);                 // ValueChangedEvent call-back function
    
    static constexpr char CONFIG_FILE[] = ".config";        // The filepath of the config file
    std::string imageDir;                                   // The directory to store images in
    std::string imagePrefix = "image";                      // image file name prefix
    std::string imageSuffix = ".bin";                       // image file extension
    uint8_t imageNumber;                                    // The number of the next image file

    const int BAUD_RATE = 250000;           //serial for the UART connection to the ESP32 CAM

    const uint8_t IMAGE_ROWS = 96;          // Number of rows in an image
    const uint8_t IMAGE_COLS = 96;          // Number of columns in an image
    const uint8_t IMAGE_BPP  = 4;           // Bytes per pixel in the image
    const uint16_t IMAGE_PADDING = 288;     // Additional bytes in the image file
    const uint16_t IMAGE_SIZE = (IMAGE_ROWS * IMAGE_COLS * IMAGE_BPP) + IMAGE_PADDING;
};