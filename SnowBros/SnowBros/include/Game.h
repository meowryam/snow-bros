
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

#include "SplashScreen.h"
#include "LoginScreen.h"
#include "SignupScreen.h"   
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

class Game {
protected:
    sf::RenderWindow window;       
    GameState currentState;  
    InputManager inputManager;      

    PlayerData   playerData;
    PlayerData   player2Data;
    Player     player1;
    Player     player2;
    GameLevel  gameLevel;
    bool       twoPlayerMode;
    KeyBindings  keyBindings;
    Leaderboard  leaderboard;

    ScoreSystem        scoreSystem;
    GemSystem          gemSystem;
    EventBus           eventBus;
    SoundManager       soundManager;
    LevelsManager      levelsManager;

    ShopSystem   shopSystem;

    SplashScreen splashScreen;
    LoginScreen        loginScreen;
    SignupScreen       signupScreen;    
    MainMenu           mainMenu;
    HUD                hud;
    PauseScreen        pauseScreen;
    GameOverScreen     gameOverScreen;
    LeaderboardScreen  leaderboardScreen;
    KeyRemapScreen     keyRemapScreen;
    ShopScreen         shopScreen;
    LevelSelectScreen levelSelectScreen;

    StarLevelScreen    starLevelScreen;
    int shopOpenedByPlayer = 1; 

    GameState prevState = GameState::MENU;

    
    const float TIME_PER_FRAME = 1.f / 30.f;

 
    void loadAllFonts();
    void loadAllSounds();
    void processInput();            
    void update(float deltaTime);   
    void draw(); 

 
    void handleLoginEvents(sf::Event& event);
    void handleSignupEvents(sf::Event& event);  
    void handleMainMenuEvents(sf::Event& event);
     void handlePauseEvents(sf::Event& event);
     void handleGameOverEvents(sf::Event& event);
     void handleStarLevelEvents(sf::Event& event);
    void saveAndSubmitScore();

public:
    Game();   
    void run(); 
};