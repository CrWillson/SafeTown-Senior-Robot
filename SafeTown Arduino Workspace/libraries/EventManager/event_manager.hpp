#ifndef EVENTMANAGER_HPP
#define EVENTMANAGER_HPP

#include "event_types.hpp"

#include <functional>
#include <vector>
#include <cstddef>

// FreeRTOS includes
#include "FreeRTOS.h"
#include "semphr.h"

namespace {
    static int getNextID() {
        static int counter = 0;
        return counter++;
    }

    template <typename T>
    int getID() {
        static int id = ::getNextID();
        return id;
    }
};



class EventManager {
public:
    EventManager();
    ~EventManager();

    template <typename EventType>
    void subscribe(std::function<void(const EventType&)> handler) {
        std::size_t typeID = getID<EventType>();
        // Ensure the top-level handlers vector is large enough.
        if (typeID >= handlers.size()) {
            handlers.resize(typeID + 1);
        }
        // Append the handler (wrapped as a lambda that casts from void* to EventType*).
        handlers[typeID].push_back([handler](const void* event) {
            handler(*reinterpret_cast<const EventType*>(event));
        });
    }

    template <typename EventType>
    void publish(const EventType& event) {
        std::size_t typeID = getID<EventType>();
        // Only publish if at least one handler is registered for this event type.
        if (typeID >= handlers.size() || handlers[typeID].empty()) {
            return;
        }
        auto eventCopy = event;  // Make a local copy.
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

    // Process and dispatch events.
    // This method is intended to run in a dedicated FreeRTOS task (or on the second core).
    void processEvents();

private:
    using HandlerFunc = std::function<void(const void*)>;
    
    // Mapping from unique event type IDs to their list of handler functions.
    std::vector<std::vector<HandlerFunc>> handlers;

    // The event queue holds lambdas that, when executed, dispatch an event.
    std::vector<std::function<void()>> eventQueue;

    // FreeRTOS synchronization primitives.
    SemaphoreHandle_t queueMutex;    // Protects access to the event queue.
    SemaphoreHandle_t eventSemaphore;  // Signals that one or more events are available.
};

#endif