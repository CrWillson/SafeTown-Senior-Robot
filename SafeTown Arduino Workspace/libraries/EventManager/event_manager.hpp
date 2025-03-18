#ifndef EVENTMANAGER_HPP
#define EVENTMANAGER_HPP

#include "event_types.hpp"
#include "debug_logger.hpp"

#include <functional>
#include <vector>
#include <cstddef>
#include "Arduino.h"

// FreeRTOS includes
#include "FreeRTOS.h"
#include "semphr.h"

class EventManager {
public:
    // Delete copy constructor and assignment operator to prevent copying
    EventManager(const EventManager&) = delete;
    EventManager& operator=(const EventManager&) = delete;
    ~EventManager();

    // Static method to access the single instance of the class
    static EventManager& getInstance() {
        static EventManager instance;
        return instance;
    }
    
    template <typename EventType>
    void subscribe(std::function<void(const EventType&)> handler) {
        std::size_t typeID = Event::getID<EventType>();
        
        if (typeID >= handlers.size()) {
            handlers.resize(typeID + 1);
        }
        handlers[typeID].push_back([handler](const void* event) {
            handler(*reinterpret_cast<const EventType*>(event));
        });
    }
    
    template <typename EventType>
    void publish(const EventType& event) {
        std::size_t typeID = Event::getID<EventType>();
        
        if (typeID >= handlers.size() || handlers[typeID].empty()) {
            return;
        }
        auto eventCopy = event;
        auto action = [this, typeID, eventCopy]() {
            // Dispatch the event to each registered handler.
            if (typeID < handlers.size()) {
                for (auto &handler : handlers[typeID]) {
                    handler(&eventCopy);
                }
            }
        };
        // Lock the queue, push the event action, and release the lock.
        if (xSemaphoreTake(queueMutex, portMAX_DELAY) == pdTRUE) {
            eventQueue.push_back(action);
            xSemaphoreGive(queueMutex);
        }
        // Signal that a new event is available.
        xSemaphoreGive(eventSemaphore);
    }
    
    void processEvents();
    
private:
    // Private constructor to prevent instantiation
    EventManager();

    using HandlerFunc = std::function<void(const void*)>;
    std::vector<std::vector<HandlerFunc>> handlers;
    std::vector<std::function<void()>> eventQueue;

    SemaphoreHandle_t queueMutex;    // Protects access to the event queue.
    SemaphoreHandle_t eventSemaphore;  // Signals that one or more events are available.
};

#endif