#include "event_manager.h"
#include "lib/priority_queue.h"

struct EventManagerImpl {
    PriorityQueue<GameEvent> queue;
};

EventManager::EventManager() : pImpl(new EventManagerImpl()) {
    reset();
}

EventManager::~EventManager() {
    clear();
    delete pImpl;
}

void EventManager::scheduleEvent(EventType type, float triggerTime) {
    GameEvent ev{type, triggerTime};
    enqueue(pImpl->queue, ev);
}

bool EventManager::pollEvent(float currentTime, GameEvent& outEvent) {
    if (::isEmpty(pImpl->queue)) {
        return false;
    }
    GameEvent nextEv;
    if (peek(pImpl->queue, nextEv) && nextEv.triggerTime <= currentTime) {
        dequeue(pImpl->queue, outEvent);
        return true;
    }
    return false;
}

bool EventManager::peekNextEvent(GameEvent& outEvent) const {
    return peek(pImpl->queue, outEvent);
}

void EventManager::reset() {
    clear();
    scheduleEvent(EventType::MAGIC_PIECE, 30.0f);
    scheduleEvent(EventType::SPEED_UP, 45.0f);
    scheduleEvent(EventType::EARTHQUAKE, 60.0f);
}

void EventManager::clear() {
    ::clear(pImpl->queue);
}

int EventManager::getCount() const {
    return pImpl->queue.count;
}

bool EventManager::isEmpty() const {
    return ::isEmpty(pImpl->queue);
}
