#pragma once

enum class EventType {
    SPEED_UP,
    MAGIC_PIECE,
    EARTHQUAKE
};

struct GameEvent {
    EventType type;
    float triggerTime;

    bool operator<(const GameEvent& other) const {
        return triggerTime < other.triggerTime;
    }
};

struct EventManagerImpl;

class EventManager {
private:
    EventManagerImpl* pImpl;

public:
    EventManager();
    ~EventManager();

    void scheduleEvent(EventType type, float triggerTime);
    bool pollEvent(float currentTime, GameEvent& outEvent);
    bool peekNextEvent(GameEvent& outEvent) const;
    void reset();
    void clear();
    int getCount() const;
    bool isEmpty() const;
};
