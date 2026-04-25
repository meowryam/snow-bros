#pragma once
#include <string>
using namespace std;

enum class GameEvent {
    ENEMY_KILLED,
    BOSS_KILLED,
    PLAYER_DIED,
    LEVEL_COMPLETE,
    POWERUP_COLLECTED
};

class IEventListener {
public:
    virtual void onEvent(GameEvent event) = 0;
    virtual ~IEventListener() {}
};

class EventBus {
private:
    IEventListener* listeners[10];
    int listenerCount;

    // queued events for poll-style access

    GameEvent queue[50];
    int queueCount;

public:
    EventBus() : listenerCount(0), queueCount(0) {
        for (int i = 0; i < 10; i++) listeners[i] = nullptr;
        for (int i = 0; i < 50; i++) queue[i] = GameEvent::ENEMY_KILLED;
    }
    void subscribe(IEventListener* listener) {
        if (listenerCount < 10)
            listeners[listenerCount++] = listener;
    }

    void publish(GameEvent event) {
        for (int i = 0; i < listenerCount; i++)
            listeners[i]->onEvent(event);
    }

    // post = publish + queue (used by GameLevel)
    void post(GameEvent event) {
        publish(event);
        if (queueCount < 50)
            queue[queueCount++] = event;
    }

    bool hasEvent(GameEvent event) {
        for (int i = 0; i < queueCount; i++)
            if (queue[i] == event) return true;
        return false;
    }

    void clear() { queueCount = 0; }
};