#pragma once

#include <Arduino.h>
#include <string>
#include <cstring>
#include <atomic>

#define ENABLE_DEBUG_LOGGING

#ifdef ENABLE_DEBUG_LOGGING
#define LOGLN(message) DebugLogger::getInstance().logln(message)                            // Logs a message with a newline
#define LOG(message) DebugLogger::getInstance().log(message)                                // Logs a message without a newline
#define LOGF(format, ...) DebugLogger::getInstance().logf(format, __VA_ARGS__)              // Logs a formatted message
#define SUPRESS_DEBUG_OUTPUT(enable) DebugLogger::getInstance().supressDebugOutput(enable)  // Suppress debug output
#define SCOPED_DEBUG_SUPRESSOR() DebugSupressor _debugSupressor                             // Suppress debug output for the current scope
#else
#define LOGLN(message)
#define LOG(message)
#define LOGF(format, ...)
#define SUPRESS_DEBUG_OUTPUT(enable)
#define SCOPED_DEBUG_SUPRESSOR()
#endif

class DebugLogger {
public:
    static DebugLogger& getInstance() {
        static DebugLogger instance;
        return instance;
    }
    
    ~DebugLogger() = default;
    
    DebugLogger(const DebugLogger&) = delete;
    DebugLogger& operator=(const DebugLogger&) = delete;

    void supressDebugOutput(bool enable) {
        _debugEnabled.store(!enable);
    }

    template<typename T>
    void logln(T message) {
        if (_debugEnabled.load()) {
            Serial.println(message);
        }
    }

    // std::string specialization
    void logln(std::string message) {
        if (_debugEnabled.load()) {
            Serial.println(message.c_str());
        }
    }

    template<typename T>
    void log(T message) {
        if (_debugEnabled.load()) {
            Serial.print(message);
        }
    }

    // std::string specialization
    void log(std::string message) {
        if (_debugEnabled.load()) {
            Serial.print(message.c_str());
        }
    }

    template<typename... Args>
    void logf(const char* format, Args... args) {
        if (_debugEnabled.load()) {
            Serial.printf(format, args...);
        }
    }


private:
    DebugLogger() : _debugEnabled(true) {}
    std::atomic<bool> _debugEnabled;
};


class DebugSupressor {
    public:
        DebugSupressor() {
            SUPRESS_DEBUG_OUTPUT(true);
        }
        
        ~DebugSupressor() {
            SUPRESS_DEBUG_OUTPUT(false);
        }
    };