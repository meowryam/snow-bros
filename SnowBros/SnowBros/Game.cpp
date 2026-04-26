#include "Game.h"
#include "MainMenu.h"
#include <ctime>


// Constructor — this runs once when Game object is created
// Sets up the window at 800x600 pixels, titled "Snow Bros"
Game::Game()    
    : window(sf::VideoMode({ 800u, 600u }), "Snow Bros"),
    currentState(GameState::LOGIN),
    // initialize systems that need refs
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
    player1.resetForNewLevel(Vector2f(100.f, 200.f));
    player2.resetForNewLevel(Vector2f(200.f, 200.f));




    player1.setShowDebug(true);
    player2.setShowDebug(true);




    window.setFramerateLimit(30); // lock to 30 FPS
    leaderboard.load();
    loadAllFonts();
    loadAllSounds();
}

void Game::loadAllFonts() {
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





void Game::run() {
    sf::Clock clock;
    float timeSinceLastUpdate = 0.f;

    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        timeSinceLastUpdate += deltaTime;

        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                saveAndSubmitScore();
                window.close();
                return;
            }
            switch (currentState) {
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
            default: break;
            }
        }

        while (timeSinceLastUpdate >= TIME_PER_FRAME) {
            processInput();
            update(TIME_PER_FRAME);
            timeSinceLastUpdate -= TIME_PER_FRAME;
        }

        draw();
    }
}



void Game::handleLoginEvents(sf::Event& event) {
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

void Game::handleMainMenuEvents(sf::Event& event) {
    MainMenuResult result = mainMenu.handleEvent(event);
    if (result == MainMenuResult::START_GAME) {
     //   player1.loadTexture("assets\\images\\Nick.png");
       // player2.loadTexture("assets\\images\\Player_Blue.png");
       //
        levelsManager.SpecificLevel(playerData.getCurrentLevel());
    
        gameLevel.loadLevel(levelsManager.getCurrentLevel());
        currentState = GameState::PLAYING;
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

void Game::handlePauseEvents(sf::Event& event) {
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
       // player1.loadTexture("assets\\images\\Nick.png");
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

void Game::processInput() {
    if (currentState == GameState::PLAYING) {
        if (inputManager.isKeyHeld(keyBindings.pause)) {
            currentState = GameState::PAUSED;
            pauseScreen.reset();
            soundManager.pauseMusic();
        }
    }
}

void Game::update(float deltaTime) {
    if (currentState != GameState::PLAYING) return;



    if (playerData.getLives() <= 0) {
        saveAndSubmitScore();
        gameOverScreen.reset();
        currentState = GameState::GAME_OVER;
        soundManager.stopMusic();
        return;
    }

    if (levelsManager.isGameDone()) {
        saveAndSubmitScore();
        gameOverScreen.reset();
        currentState = GameState::GAME_OVER;
        soundManager.stopMusic();
    }

    if (currentState == GameState::PLAYING) {
        gameLevel.update(deltaTime, player1, twoPlayerMode ? &player2 : nullptr);
   
        if (gameLevel.levelComplete) {
            playerData.setCurrentLevel(playerData.getCurrentLevel() + 1);
            levelsManager.NextLevel();
            if (levelsManager.isGameDone()) {
                saveAndSubmitScore();
                currentState = GameState::GAME_OVER;
            }
            else {
                gameLevel.loadLevel(levelsManager.getCurrentLevel());
            }
        }
    }

}

void Game::draw() {
    window.clear(sf::Color::Black);

    switch (currentState) {
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

    window.display();
}

void Game::saveAndSubmitScore() {
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