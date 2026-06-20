#pragma once
#include "GameState.h"


class GameStateManager {
private:
    GameState currentState; 

public:
    GameStateManager() : currentState(GameState::MENU) {}

    GameState getState() const { return currentState; }

    void setState(GameState newState) { currentState = newState; }

    bool isMenu()     const { return currentState == GameState::MENU; }
    bool isPlaying()  const { return currentState == GameState::PLAYING; }
    bool isPaused()   const { return currentState == GameState::PAUSED; }
    bool isGameOver() const { return currentState == GameState::GAME_OVER; }
};