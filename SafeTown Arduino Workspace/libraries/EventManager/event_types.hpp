#ifndef EVENTTYPES_HPP
#define EVENTTYPES_HPP

#include <string>
#include <cstdint>

namespace Event {
    inline int getNextID() {
        static int counter = 0;
        return counter++;
    }

    template <typename T>
    inline int getID() {
        static int id = getNextID();
        return id;
    }
    
    struct Event {
        virtual ~Event() = default;  // Polymorphic base
    };

    struct EncoderRight : public Event {};
    struct EncoderLeft : public Event {};
    struct EncoderPress : public Event {};

    template <typename T>
    struct ValueChangedEvent : public Event {
        std::string valueId;
        T newValue;
        ValueChangedEvent(const std::string& id, T val)
          : valueId(id), newValue(val) {}
    };
}

#endif