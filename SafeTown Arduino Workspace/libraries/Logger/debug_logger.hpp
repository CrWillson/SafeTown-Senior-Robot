#pragma once

#include <Arduino.h>
#include <string>
#include <cstring>
#include <atomic>

/**
 * @defgroup DebugUtilities
 * @brief A collection of utilities for debugging the code
 * 
 */

#define ENABLE_DEBUG_LOGGING

/**
 * @brief Macros to facilitate debug logging. If ENABLE_DEBUG_LOGGING is not defined then all
 * debug log macros will be compiled out of the code.
 * 
 * @addtogroup DebugUtilities
 * @{
 */
#ifdef ENABLE_DEBUG_LOGGING
#define LOGLN(message) DebugLogger::getInstance().logln(message)                            ///< Logs a message with a newline
#define LOG(message) DebugLogger::getInstance().log(message)                                ///< Logs a message without a newline
#define LOGF(format, ...) DebugLogger::getInstance().logf(format, __VA_ARGS__)              ///< Logs a formatted message
#define SUPRESS_DEBUG_OUTPUT(enable) DebugLogger::getInstance().supressDebugOutput(enable)  ///< Suppress debug output
#define SCOPED_DEBUG_SUPRESSOR() DebugSupressor _debugSupressor                             ///< Suppress debug output for the current scope
#else
#define LOGLN(message)
#define LOG(message)
#define LOGF(format, ...)
#define SUPRESS_DEBUG_OUTPUT(enable)
#define SCOPED_DEBUG_SUPRESSOR()
#endif

/** @} */

/**
 * @brief A class for handling printing debug messages to the serial line. Allows for
 * the debug messages to be disabled or suppressed.
 * 
 * @ingroup DebugUtilities
 */
class DebugLogger {
public:
    /**
     * @brief Get the Meyers singleton instance of the debug logger
     * 
     * @return DebugLogger& 
     */
    static DebugLogger& getInstance() {
        static DebugLogger instance;
        return instance;
    }
    
    ~DebugLogger() = default;
    
    DebugLogger(const DebugLogger&) = delete;
    DebugLogger& operator=(const DebugLogger&) = delete;

    /**
     * @brief Supress the debug log output to the serial line
     * 
     * @param enable - If true, disable the debug logger
     */
    void supressDebugOutput(bool enable) {
        _debugEnabled.store(!enable);
    }

    /**
     * @brief Log a message to the serial line with a newline character added
     * 
     * @overload
     * @tparam T - The data type of the message. Must be printable.
     * @param message - The message to print to the serial line
     */
    template<typename T>
    void logln(T message) {
        if (_debugEnabled.load()) {
            Serial.println(message);
        }
    }

    /**
     * @brief Overload of void logln(T message) to handle std::string
     * 
     * @param message - The message to print to the serial line
     */
    void logln(std::string message) {
        if (_debugEnabled.load()) {
            Serial.println(message.c_str());
        }
    }

    /**
     * @brief Log a message to the serial line
     * 
     * @overload
     * @tparam T - The data type of the message. Must be printable.
     * @param message - The message to print to the serial line
     */
    template<typename T>
    void log(T message) {
        if (_debugEnabled.load()) {
            Serial.print(message);
        }
    }

    /**
     * @brief Overload of void log(T message) to handle std::string
     * 
     * @param message - The message to print to the serial line
     */
    void log(std::string message) {
        if (_debugEnabled.load()) {
            Serial.print(message.c_str());
        }
    }

    /**
     * @brief Log a format string to the serial line
     * 
     * @param format - The format string 
     * @param args - The arguments for the format string
     */
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

/** @cond DOXYGEN_EXCLUDE */
class DebugSupressor {
public:
    DebugSupressor() {
        SUPRESS_DEBUG_OUTPUT(true);
    }
    
    ~DebugSupressor() {
        SUPRESS_DEBUG_OUTPUT(false);
    }
};
/** @endcond */

