#pragma once
#include "GameState.h"

// This class remembers which state we're currently in
// and lets us switch to a different state
class GameStateManager {
private:
    GameState currentState; // stores the current state

public:
    // When the game starts, we begin at the MENU
    GameStateManager() : currentState(GameState::MENU) {}

    // Get whatever state we're currently in
    GameState getState() const { return currentState; }

    // Switch to a new state
    void setState(GameState newState) { currentState = newState; }

    // Easy check functions — instead of writing currentState == MENU every time
    bool isMenu()     const { return currentState == GameState::MENU; }
    bool isPlaying()  const { return currentState == GameState::PLAYING; }
    bool isPaused()   const { return currentState == GameState::PAUSED; }
    bool isGameOver() const { return currentState == GameState::GAME_OVER; }
};