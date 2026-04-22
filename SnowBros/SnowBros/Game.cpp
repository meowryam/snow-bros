#include "Game.h"

// Constructor — this runs once when Game object is created
// Sets up the window at 800x600 pixels, titled "Snow Bros"
Game::Game()    
    : window(sf::VideoMode({ 800u, 600u }), "Snow Bros") 
{
    window.setFramerateLimit(30); // lock to 30 FPS
}

void Game::run()  // THE MAIN GAME LOOP

{
    sf::Clock clock; // a timer
    float timeSinceLastUpdate = 0.f;

    while (window.isOpen())
    {
        
        float deltaTime = clock.restart().asSeconds(); // How long did the last frame take?
        timeSinceLastUpdate += deltaTime;

        
        if (!inputManager.processEvents(window)) // Step 1 — check if window was closed
            break;

        
        while (timeSinceLastUpdate >= TIME_PER_FRAME) // Step 2 — update game logic at exactly 30 FPS
        {
            processInput();
            update(TIME_PER_FRAME);
            timeSinceLastUpdate -= TIME_PER_FRAME;
        }

        
        draw(); // Step 3 — draw everything
    }
}


void Game::processInput() // Check keyboard and change state accordingly
{
    if (stateManager.isPlaying())     // If we're playing and press Escape ? go to PAUSED

    {
        if (inputManager.isKeyHeld(sf::Keyboard::Key::Escape))
            stateManager.setState(GameState::PAUSED);
    }

    if (stateManager.isMenu())      // If we're on MENU and press Enter ? start PLAYING

    {
        if (inputManager.isKeyHeld(sf::Keyboard::Key::Enter))
            stateManager.setState(GameState::PLAYING);
    }
}

// Update game logic — empty for now
// Your partner will plug Player and Enemy logic in here later
void Game::update(float deltaTime) 
{
}

// Draw everything on screen
void Game::draw()
{
    window.clear(sf::Color::Black);     // Default — black screen (MENU state)


    if (stateManager.isPlaying())     // Dark blue when playing

        window.clear(sf::Color(20, 20, 60));

    else if (stateManager.isPaused())     // Dark red when paused

        window.clear(sf::Color(60, 20, 20));

    window.display(); // show the frame
}