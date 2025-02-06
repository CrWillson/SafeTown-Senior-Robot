#ifndef EVENTTYPES_HPP
#define EVENTTYPES_HPP

namespace Event {
    struct Event {
        virtual ~Event() = default;  // Polymorphic base
    };

    struct EncoderRight : public Event {
        bool changed;
        explicit EncoderRight(bool c) : changed(c) {}
    };

    struct EncoderLeft : public Event {
        bool changed;
        explicit EncoderLeft(bool c) : changed(c) {}
    };

    struct EncoderPress : public Event {};
    struct EncoderRelease : public Event {};
}

#endif