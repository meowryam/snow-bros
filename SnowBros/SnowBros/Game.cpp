#include "Game.h"
#include "MainMenu.h"
#include <ctime>

// Sets up the window at 800x600 pixels, titled "Snow Bros"
//The constructor's initializer list. Creates the window at 800x600,sets the starting state to LOGIN
Game::Game()    
    : window(sf::VideoMode({ 800u, 600u }), "Snow Bros"),
    currentState(GameState::LOGIN),
    scoreSystem(playerData),
    gemSystem(playerData),
    shopSystem(playerData),
    leaderboardScreen(leaderboard),
    keyRemapScreen(keyBindings),
    shopScreen(shopSystem),
    gameLevel(playerData, scoreSystem, gemSystem, eventBus, keyBindings,
        "assets\\"),
    twoPlayerMode(false)
{
    // Initialize player 2 data with same lives
    player2Data = PlayerData();
    player2Data.setUsername("Player2");
    player2Data.setLives(2);

    //player1.setShowDebug(true);
   // player2.setShowDebug(true);

    window.setFramerateLimit(30); // lock to 30 FPS
    leaderboard.load(); // loads the leaderboard from disk
    loadAllFonts(); // loads all fonts and sounds.
    loadAllSounds();
}



void Game::loadAllFonts() //Loads the same font into every screen that displays text, so they all share a consistent look.
{
    string fontPath = "assets\\fonts\\PressStart2P-Regular.ttf";

    loginScreen.loadFont(fontPath);
    mainMenu.loadFont(fontPath);
    hud.loadFont(fontPath);
    pauseScreen.loadFont(fontPath);
    gameOverScreen.loadFont(fontPath);
    leaderboardScreen.loadFont(fontPath);
    keyRemapScreen.loadFont(fontPath);
    shopScreen.loadFont(fontPath);
}

void Game::loadAllSounds() {
    // soundManager.loadSound("throw",      "D:\\Fast\\oop\\SnowBros\\SnowBros\\SnowBros\\assets\\sounds\\throw.wav");
    // soundManager.loadSound("encase",     "D:\\Fast\\oop\\SnowBros\\SnowBros\\SnowBros\\assets\\sounds\\encase.wav");
    // soundManager.loadSound("death",      "D:\\Fast\\oop\\SnowBros\\SnowBros\\SnowBros\\assets\\sounds\\death.wav");
    // soundManager.loadSound("levelup",    "D:\\Fast\\oop\\SnowBros\\SnowBros\\SnowBros\\assets\\sounds\\levelup.wav");
    // soundManager.loadSound("gemcollect", "D:\\Fast\\oop\\SnowBros\\SnowBros\\SnowBros\\assets\\sounds\\gem.wav");
    // soundManager.loadMusic("D:\\Fast\\oop\\SnowBros\\SnowBros\\SnowBros\\assets\\sounds\\bgm.ogg");
    // soundManager.playMusic();
}





void Game::run() //The main game loop setup
{
    sf::Clock clock; //clock measures elapsed time
    float timeSinceLastUpdate = 0.f;

    while (window.isOpen()) 
    {
        float deltaTime = clock.restart().asSeconds(); //Each frame, gets how much real time passed since last frame and
        timeSinceLastUpdate += deltaTime; //adds it to the accumulator.
       

        while (auto event = window.pollEvent()) 
        //Checks for any pending window events (key presses, mouse clicks, window close, etc.) and processes them one by one.
        {
            if (event->is<sf::Event::Closed>()) 
             //If the user clicks the X button, saves the score and closes the game cleanly.
            {
                saveAndSubmitScore();
                window.close();
                return;
            }
            switch (currentState)
                //Routes each event to the correct handler depending on which screen is currently active.
            {
            case GameState::LOGIN:
                handleLoginEvents(*event); break;
            case GameState::MENU:
                handleMainMenuEvents(*event); break;
            case GameState::PAUSED:
                handlePauseEvents(*event); break;
            case GameState::GAME_OVER:
                handleGameOverEvents(*event); break;
            case GameState::KEY_REMAP:
                keyRemapScreen.handleEvent(*event);
                if (keyRemapScreen.done) {
                    keyBindings.saveToFile(playerData.getUsername());
                    currentState = GameState::PAUSED;
                    keyRemapScreen.done = false;
                }
                break;
                // Game.cpp — inside run(), in the switch(currentState) block
// ADD this case, right before the default: break

            case GameState::SHOP:
                shopScreen.handleEvent(*event);
                if (shopScreen.done) {
                    shopScreen.done = false;
                    currentState = GameState::MENU;
                }
                break;
            default: break;
            }
        }

        while (timeSinceLastUpdate >= TIME_PER_FRAME)
        //A fixed timestep loop — only updates the game logic when enough time has accumulated.
         //This keeps physics and movement consistent regardless of how fast the computer is running
        {
            processInput();
            update(TIME_PER_FRAME);
            timeSinceLastUpdate -= TIME_PER_FRAME;
        }

        draw();
    }
}



void Game::handleLoginEvents(sf::Event& event) 
//Passes the event to the login screen, then reacts to whatever the user chose — new game resets all data, continue loads saved data, quit closes the window.
{
    LoginResult result = loginScreen.handleEvent(event);
    if (result == LoginResult::NEW_GAME) {
        playerData = PlayerData();
        playerData.setUsername(loginScreen.getUsername());
        keyBindings = KeyBindings();
        mainMenu.setUsername(playerData.getUsername());
        mainMenu.reset();
        currentState = GameState::MENU;
    }
    else if (result == LoginResult::CONTINUE) {
        FileManager::loadPlayerData(loginScreen.getUsername(), playerData);
        keyBindings.loadFromFile(playerData.getUsername());
        mainMenu.setUsername(playerData.getUsername());
        mainMenu.reset();
        currentState = GameState::MENU;
    }
    else if (result == LoginResult::QUIT) {
        window.close();
    }
}

void Game::handleMainMenuEvents(sf::Event& event)
//Handles the main menu choices — starting the game loads a level and switches state to PLAYING. Other options open the shop, key remapping, or log out.
{
    MainMenuResult result = mainMenu.handleEvent(event);
    if (result == MainMenuResult::START_GAME)
    {
        twoPlayerMode = false; // to make sure multiplayer is off
        player1.loadTexture("assets\\images\\Nick.png");
       // player2.loadTexture("assets\\images\\Player_Blue.png");
       levelsManager.SpecificLevel(playerData.getCurrentLevel());
     // levelsManager.SpecificLevel(5);
        gameLevel.loadLevel(levelsManager.getCurrentLevel());
        currentState = GameState::PLAYING;
        soundManager.playMusic();
    }

    else if (result == MainMenuResult::START_2PLAYER)
    {
        twoPlayerMode = true;
        player1.loadTexture("assets\\images\\Nick.png");
        player2.loadTexture("assets\\images\\Nick.png"); // or a different texture
        levelsManager.SpecificLevel(playerData.getCurrentLevel());
        gameLevel.loadLevel(levelsManager.getCurrentLevel());
        currentState = GameState::PLAYING;
        player2.resetForNewLevel(sf::Vector2f(600.f, 500.f));
        soundManager.playMusic();
    }

    else if (result == MainMenuResult::OPEN_SHOP) {
        shopScreen.reset();
        currentState = GameState::SHOP;
    }
    else if (result == MainMenuResult::OPEN_KEYREMAP) {
        keyRemapScreen.done = false;
        currentState = GameState::KEY_REMAP;
    }
    else if (result == MainMenuResult::LOGOUT) {
        FileManager::savePlayerData(playerData);
        currentState = GameState::LOGIN;
    }
    else if (result == MainMenuResult::QUIT) {
        saveAndSubmitScore();
        window.close();
    }
}

void Game::handlePauseEvents(sf::Event& event)
{
    PauseResult result = pauseScreen.handleEvent(event);
    if (result == PauseResult::RESUME) {
        currentState = GameState::PLAYING;
        soundManager.playMusic();
    }
    else if (result == PauseResult::OPEN_SHOP) {
        shopScreen.reset();
        currentState = GameState::SHOP;
    }
    else if (result == PauseResult::OPEN_KEYREMAP) {
        keyRemapScreen.done = false;
        currentState = GameState::KEY_REMAP;
    }
    else if (result == PauseResult::QUIT_TO_MENU) {
        FileManager::savePlayerData(playerData);
        mainMenu.reset();
        currentState = GameState::MENU;
        soundManager.stopMusic();
    }
}

void Game::handleGameOverEvents(sf::Event& event) {
    GameOverResult result = gameOverScreen.handleEvent(event);

    if (result == GameOverResult::RETRY) {
        playerData.setLives(2);
        playerData.setCurrentLevel(1);
        levelsManager.SpecificLevel(1);
        player1.loadTexture("assets\\images\\Nick.png");
        ///player2.loadTexture("assets\\images\\Player_Blue.png");
        gameLevel.loadLevel(levelsManager.getCurrentLevel());
        gameOverScreen.reset();
        currentState = GameState::PLAYING;
        soundManager.playMusic();
    }
    else if (result == GameOverResult::QUIT_TO_MENU) {
        saveAndSubmitScore();
        mainMenu.reset();
        currentState = GameState::MENU;
        soundManager.stopMusic();
    }
}

void Game::processInput()
//Only checks input during gameplay. If the pause key is held, switches to the paused state and pauses the music.
{
    if (currentState == GameState::PLAYING) {
        if (inputManager.isKeyHeld(keyBindings.pause)) {
            currentState = GameState::PAUSED;
            pauseScreen.reset();
            soundManager.pauseMusic();
        }
    }
}

void Game::update(float deltaTime)
//Immediately exits if the game isn't in the PLAYING state — nothing should update while paused or on a menu.
{
    if (currentState != GameState::PLAYING) return;



    if (playerData.getLives() <= 0) {
        saveAndSubmitScore();
        gameOverScreen.reset();
        currentState = GameState::GAME_OVER; //If the player has run out of lives, saves the score and transitions to the game over screen.
        soundManager.stopMusic();
        return;
    }

    if (levelsManager.isGameDone()) {
        saveAndSubmitScore();
        gameOverScreen.reset();
        currentState = GameState::GAME_OVER;
        soundManager.stopMusic();
    }

    if (currentState == GameState::PLAYING) 
    {
        //Updates the current level. Passes player2 only if two-player mode is on, otherwise passes nullptr.
        gameLevel.update(deltaTime, player1, twoPlayerMode ? &player2 : nullptr);
   
        if (gameLevel.levelComplete)
        {
            //When a level is cleared, increments the level counter and loads the next one
            playerData.setCurrentLevel(playerData.getCurrentLevel() + 1);
            levelsManager.NextLevel();
            if (levelsManager.isGameDone()) {
                saveAndSubmitScore();
                currentState = GameState::GAME_OVER;
            }
            else //If there are no more levels, the game ends. 
            {
                gameLevel.loadLevel(levelsManager.getCurrentLevel());
            }
        }
    }

}

void Game::draw() {
    window.clear(sf::Color::Black);//Clears the screen to black

    switch (currentState)  //draws whatever belongs to the current state, then displays it.
        //how you move between menus, gameplay, pause, and game over.
    {
    case GameState::LOGIN:
        loginScreen.draw(window);
        break;
    case GameState::MENU:
        mainMenu.draw(window);
        break;
    case GameState::PLAYING:
        gameLevel.draw(window, player1, twoPlayerMode ? &player2 : nullptr);
        hud.draw(window, playerData);
        break;
    case GameState::PAUSED:
        pauseScreen.draw(window);
        break;
    case GameState::GAME_OVER:
        gameOverScreen.draw(window, playerData);
        break;
    case GameState::KEY_REMAP:
        keyRemapScreen.draw(window);
        break;
    case GameState::SHOP:
        shopScreen.draw(window);
        break;
    default: break;
    }

    window.display(); // pushes the frame to the screen.
}

void Game::saveAndSubmitScore() 
//Saves the player's data to disk and submits their score to the leaderboard with today's date.
{
    FileManager::savePlayerData(playerData);

    time_t t = time(nullptr);
    char buf[11];
    tm timeInfo;
    localtime_s(&timeInfo, &t);
    strftime(buf, sizeof(buf), "%Y-%m-%d", &timeInfo);
    string date(buf);

    leaderboard.submitScore(
        playerData.getUsername(),
        playerData.getHighscore(),
        playerData.getCurrentLevel(),
        date
    );
}