#ifndef EVENTTYPES_HPP
#define EVENTTYPES_HPP

#include <string>
#include <cstdint>
#include <array>

/**
 * @brief Namespace containing several common event types shared among all components.
 * @ingroup CoreUtilities
 */
namespace Event {
    //-----------------------------------------------------------------
    // Functions to statically assign a unique id to each event type
    //-----------------------------------------------------------------

    /** @cond DOXYGEN_EXCLUDE */
    inline int getNextID() {
        static int counter = 0;
        return counter++;
    }

    template <typename T>
    inline int getID() {
        static int id = getNextID();
        return id;
    }
    /** @endcond */


    //-----------------------------------------------------------------
    // Base Event type
    //-----------------------------------------------------------------
    
    /// @brief  Pure vitual base event type
    struct Event {   
        virtual ~Event() = default;  
    };

    //-----------------------------------------------------------------
    // User Input Events
    //-----------------------------------------------------------------

    /// @brief Event for the encoder spinning to the right
    struct EncoderRight : public Event {};
    /// @brief Event for the encoder spinning to the left
    struct EncoderLeft : public Event {};
    /// @brief Event for the encoder being pressed
    struct EncoderPress : public Event {};

    //-----------------------------------------------------------------
    // Menu Events
    //-----------------------------------------------------------------

    /// @brief Event for the current page being visually updated in some way
    struct PageChangedEvent : public Event {
        std::array<std::string, 8> lines;
        PageChangedEvent(const std::array<std::string, 8>& pageLines) 
            : lines(pageLines) {}
    };

    //-----------------------------------------------------------------
    // Information Transfer events
    //-----------------------------------------------------------------

    /// @brief Event to signal that a value has changed
    struct ValueChangedEvent : public Event {
        std::string valueId;
        std::string newValue;
        ValueChangedEvent(const std::string& id, const std::string& val)
          : valueId(id), newValue(val) {}
    };

    /// @brief Event to request the updated state of some value
    struct ValueRequestEvent : public Event {
        std::string valueId;
        ValueRequestEvent(const std::string& id) : valueId(id) {}
    };


    //-----------------------------------------------------------------
    // ESP32 Camera Events
    //-----------------------------------------------------------------

    /// @brief Event to signal that a photo has been requested from the ESP32
    struct RequestPhotoEvent : public Event {};

}

#endif