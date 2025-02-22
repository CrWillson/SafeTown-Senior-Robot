#ifndef EVENTTYPES_HPP
#define EVENTTYPES_HPP

#include <string>
#include <cstdint>
#include <array>

namespace Event {
    //-----------------------------------------------------------------
    // Functions to statically assign a unique id to each event type
    //-----------------------------------------------------------------
    inline int getNextID() {
        static int counter = 0;
        return counter++;
    }

    template <typename T>
    inline int getID() {
        static int id = getNextID();
        return id;
    }
    
    //-----------------------------------------------------------------
    // Event types
    //-----------------------------------------------------------------
    struct Event {  // Polymorphic base
        virtual ~Event() = default;  
    };

    //-----------------------------------------------------------------
    // User Input Events
    //-----------------------------------------------------------------
    struct EncoderRight : public Event {};
    struct EncoderLeft : public Event {};
    struct EncoderPress : public Event {};

    //-----------------------------------------------------------------
    // Menu Events
    //-----------------------------------------------------------------
    struct PageChangedEvent : public Event {
        std::array<std::string, 8> lines;
        PageChangedEvent(const std::array<std::string, 8>& pageLines) 
            : lines(pageLines) {}
    };

    //-----------------------------------------------------------------
    // Information Transfer events
    //-----------------------------------------------------------------
    struct ValueChangedEvent : public Event {
        std::string valueId;
        std::string newValue;
        ValueChangedEvent(const std::string& id, const std::string& val)
          : valueId(id), newValue(val) {}
    };

    struct ValueRequestEvent : public Event {
        std::string valueId;
        ValueRequestEvent(const std::string& id) : valueId(id) {}
    };

    //-----------------------------------------------------------------
    // Filesystem Events
    //-----------------------------------------------------------------
    struct FileCreatedEvent : public Event {
        std::string fileName;
        FileCreatedEvent(const std::string& filename) : fileName(filename) {};
    };

    struct FileDeletedEvent : public Event {
        std::string fileName;
        FileDeletedEvent(const std::string& filename) : fileName(filename) {};
    };
}

#endif