#include "event_manager.hpp"



//------------------------------------------------------------------------------
// Non-Templated Methods
//------------------------------------------------------------------------------
EventManager::EventManager() {
    // Create a mutex to protect the event queue.
    queueMutex = xSemaphoreCreateMutex();
    eventSemaphore = xSemaphoreCreateCounting(50, 0);
}

EventManager::~EventManager() {
    if (queueMutex) {
        vSemaphoreDelete(queueMutex);
    }
    if (eventSemaphore) {
        vSemaphoreDelete(eventSemaphore);
    }
}

void EventManager::processEvents() {
    while (true) {
        // Wait indefinitely until an event is available.
        if (xSemaphoreTake(eventSemaphore, portMAX_DELAY) == pdTRUE) {
            // Lock the event queue.
            if (xSemaphoreTake(queueMutex, portMAX_DELAY) == pdTRUE) {
                if (!eventQueue.empty()) {
                    // Retrieve and remove the first event in the queue.
                    auto action = eventQueue.front();
                    eventQueue.erase(eventQueue.begin());
                    xSemaphoreGive(queueMutex);
                    // Dispatch the event by executing its lambda.
                    action();
                } else {
                    xSemaphoreGive(queueMutex);
                }
            }
        }
    }
}
