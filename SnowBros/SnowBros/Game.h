#pragma once
#include <SFML/Graphics.hpp>

#include "GameState.h"
#include "InputManager.h"

#include "PlayerData.h"
#include "FileManager.h"
#include "KeyBindings.h"
#include "Leaderboard.h"

#include "ScoreSystem.h"
#include "GemSystem.h"
#include "EventBus.h"
#include "SoundManager.h"
#include "LevelsManager.h"

#include "LoginScreen.h"
#include "MainMenu.h"
#include "HUD.h"
#include "PauseScreen.h"
#include "GameOverScreen.h"
#include "LeaderboardScreen.h"
#include "KeyRemapScreen.h"
#include "ShopSystem.h"

#include "GameLevel.h"
#include "Player.h"
#include "LevelSelection.h"
#include "StarLevelScreen.h"
// This is the main Game class
// It owns everything — the window, the loop, all the managers
class Game {
private:
   
    sf::RenderWindow window;        // the actual game window
    GameState currentState;  // tracks current state
    InputManager inputManager;      // handles keyboard

    //sf::Font sharedFontUI;    // ADD — one font loaded once
    sf::Font sharedFontTitle; // ADD — one title font loaded once
    sf::Font sharedFontUI;

    // data
    PlayerData   playerData;
    PlayerData   player2Data;
    KeyBindings  keyBindings;
    Leaderboard  leaderboard;


    // systems
    ScoreSystem        scoreSystem;
    GemSystem          gemSystem;
    EventBus           eventBus;
    SoundManager       soundManager;
    LevelsManager      levelsManager;
    // shop (needs playerData ref)
    ShopSystem   shopSystem;

    Player     player1;
    Player     player2;

    GameLevel  gameLevel;
    bool       twoPlayerMode;


    // UI screens
    LoginScreen        loginScreen;
    MainMenu           mainMenu;
    HUD                hud;
    PauseScreen        pauseScreen;
    GameOverScreen     gameOverScreen;
    LeaderboardScreen  leaderboardScreen;
    KeyRemapScreen     keyRemapScreen;
    ShopScreen         shopScreen;
    LevelSelectScreen levelSelectScreen;
    StarLevelScreen starLevelScreen;
   
    int shopOpenedByPlayer = 1; // who opened the shop
    GameState prevState = GameState::MENU;

    // 30 FPS means each frame lasts 1/30th of a second
    const float TIME_PER_FRAME = 1.f / 30.f;

    static const string FONT_TITLE;
    static const string FONT_UI;

    // internal helpers
    void loadAllFonts();
    void loadAllSounds();
    void processInput();            // step 1 of game loop
    void update(float deltaTime);   // step 2 of game loop
    void draw(); // step 3 of game loop

    // state handlers
    void handleLoginEvents(sf::Event& event);
    void handleMainMenuEvents(sf::Event& event);
    void handlePauseEvents(sf::Event& event);
    void handleGameOverEvents(sf::Event& event);

    void saveAndSubmitScore();

    

    //const string FONT_TITLE = "assets\\fonts\\PressStart2P-Regular.ttf";
    //const string FONT_UI = "assets\\fonts\\Orbitron-Regular.ttf";
 
    

public:
    Game();   // sets up the window
    void run(); // starts the game loop
};