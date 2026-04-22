#pragma once
#include <SFML/Graphics.hpp>
#include "GameStateManager.h"
#include "InputManager.h"

// This is the main Game class
// It owns everything — the window, the loop, all the managers
class Game {
private:
    sf::RenderWindow window;        // the actual game window
    GameStateManager stateManager;  // tracks current state
    InputManager inputManager;      // handles keyboard

    // 30 FPS means each frame lasts 1/30th of a second
    const float TIME_PER_FRAME = 1.f / 30.f;

    void processInput();            // step 1 of game loop
    void update(float deltaTime);   // step 2 of game loop
    void draw();                    // step 3 of game loop

public:
    Game();   // sets up the window
    void run(); // starts the game loop
};