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

/**
 * @brief Handles the publishing of and subcribing to events between functionally independent components.
 * 
 */
class EventManager {
public:
    ~EventManager();
    EventManager(const EventManager&) = delete;
    EventManager& operator=(const EventManager&) = delete;

    // Make the EventManager a Meyers singleton class
    static EventManager& getInstance() {
        static EventManager instance;
        return instance;
    }
    
    /**
     * @brief Register a call-back function to be execution upon the publishing of a specific event type.
     * All call-back functions for a specific event type will be executed in the order that they are registered. 
     * 
     * @tparam EventType - The event type to subscribe to
     * @param handler  - The call-back function to be executed
     */
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
    
    /**
     * @brief Publish an event to the event queue. All call-back functions registered to that
     * event type will be executed in the order that they were registered. 
     * @details If no call-back functions are registered to the event type passed, the event will
     * be discarded. 
     *
     * @param event - The event to publish
     */
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
    
    /**
     * @brief Begins the event processing loop. Should be executed on the second core to prevent
     * the main loop from blocking.
     * @details This function starts an infinite loop and will never return.
     * 
     */
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