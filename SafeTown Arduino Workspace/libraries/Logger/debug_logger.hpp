#pragma once

#include <Arduino.h>
#include <string>
#include <cstring>
#include <atomic>


#ifdef ENABLE_DEBUG_LOGGING
#define LOGLN(message) DebugLogger::getInstance().logln(message)
#define LOG(message) DebugLogger::getInstance().log(message)
#define LOGF(format, ...) DebugLogger::getInstance().logf(format, __VA_ARGS__)
#define SUPRESS_DEBUG_OUTPUT(enable) DebugLogger::getInstance().supressDebugOutput(enable)
#else
#define LOGLN(message)
#define LOG(message)
#define LOGF(format, ...)
#define SUPRESS_DEBUG_OUTPUT(enable)
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
        _debugEnabled.store(!enable, std::memory_order_relaxed);
    }

    template<typename T>
    void logln(T message) {
        if (_debugEnabled.load(std::memory_order_relaxed)) {
            Serial.println(message);
        }
    }

    // std::string specialization
    void logln(std::string message) {
        if (_debugEnabled.load(std::memory_order_relaxed)) {
            Serial.println(message.c_str());
        }
    }

    template<typename T>
    void log(T message) {
        if (_debugEnabled.load(std::memory_order_relaxed)) {
            Serial.print(message);
        }
    }

    // std::string specialization
    void log(std::string message) {
        if (_debugEnabled.load(std::memory_order_relaxed)) {
            Serial.print(message.c_str());
        }
    }

    template<typename... Args>
    void logf(const char* format, Args... args) {
        if (_debugEnabled.load(std::memory_order_relaxed)) {
            Serial.printf(format, args...);
        }
    }


private:
    DebugLogger() : _debugEnabled(false) {}
    std::atomic<bool> _debugEnabled;
};