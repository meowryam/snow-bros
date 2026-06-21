#include "Game.h"
#include <ctime>

Game::Game()
    : window(sf::VideoMode({ 800u, 600u }), "Snow Bros"),
    currentState(GameState::SPLASH),
    player2Data(),
    player1(playerData, 1, 800.f, 600.f),
    player2(player2Data, 2, 800.f, 600.f),


    scoreSystem(playerData),
    gemSystem(playerData),
    shopSystem(playerData),
    leaderboardScreen(leaderboard),
    keyRemapScreen(keyBindings),
    shopScreen(shopSystem),
    gameLevel(playerData, scoreSystem, gemSystem, eventBus, keyBindings,
        "assets\\"),
    twoPlayerMode(false)
    , levelSelectScreen(playerData)
    , loginScreen(soundManager)      // ADD
    , mainMenu(soundManager)
    //, signupScreen(soundManager)
{

    srand(static_cast<unsigned int>(time(nullptr)));
    player2Data = PlayerData();

    player2Data.setUsername("Player2");
    player2Data.setLives(2);
    player2Data.setGemCount(0);


    window.setFramerateLimit(30);
    leaderboard.load();
    loadAllFonts();
    splashScreen.load(
        "assets\\fonts\\Cinzel-Bold.ttf",
        "assets\\fonts\\CormorantGaramond-Regular.ttf",
        "assets\\images\\splash_logo.png"
    );
    loadAllSounds();

    soundManager.playSplashMusic();    // ADD
}







void Game::loadAllFonts() {
    //  string fontPath = "assets\\fonts\\PressStart2P-Regular.ttf";
    string fontTitle = "assets\\fonts\\Cinzel-Bold.ttf";
    string fontBody = "assets\\fonts\\Montserrat-Medium.ttf";
    string fontButtons = "assets\\fonts\\Montserrat-SemiBoldItalic.ttf";
    string fontSubtitle = "assets\\fonts\\CormorantGaramond-Regular.ttf";
    string fontOrbitron = "assets\\fonts\\Orbitron Light.ttf";

    loginScreen.loadFont(fontOrbitron, fontTitle, fontBody);
    signupScreen.loadFont(fontOrbitron, fontTitle, fontBody);
    mainMenu.loadFont(fontOrbitron, fontTitle, fontBody, fontButtons);
    hud.loadFont(fontOrbitron);
    pauseScreen.loadFont(fontOrbitron, fontTitle, fontButtons);
    gameOverScreen.loadFont(fontOrbitron);
    leaderboardScreen.loadFont(fontOrbitron, fontTitle, fontBody, fontButtons);
    keyRemapScreen.loadFont(fontOrbitron);
    shopScreen.loadFont(fontOrbitron);
    levelSelectScreen.loadAssets(fontOrbitron, "assets\\images\\LevelSelect_bg.png");
    starLevelScreen.loadAssets(fontOrbitron, "assets\\images\\StarLevel_bg.png");
}








void Game::loadAllSounds() {
    soundManager.loadLoginMusic("assets\\sounds\\snowbros_theme.ogg");
    soundManager.loadSplashMusic("assets\\sounds\\snowman.ogg");   // ADD
    soundManager.loadSound("gamestart", "assets\\sounds\\gamestart.wav");  // #2 game start jingle
    soundManager.loadSound("levelstart", "assets\\sounds\\levelstart.wav"); // #4 next level fanfare

    // UI sounds for login/signup screens
    soundManager.loadSound("ui_confirm", "assets\\sounds\\Menu Confirm.ogg");
    soundManager.loadSound("ui_error", "assets\\sounds\\Menu Error.ogg");
    soundManager.loadSound("ui_navigate", "assets\\sounds\\Menu Move.ogg");

    soundManager.loadMenuMusic("assets\\sounds\\menu_bgm.wav");      // #
    // soundManager.loadMenuMusic("assets\\sounds\\snowman.mp3");      // #

    soundManager.loadGameMusic("assets\\sounds\\game_bgm.wav");      // #3
    soundManager.loadGameOverMusic("assets\\sounds\\gameover_bgm.wav"); // #5
}






void Game::run()
{
    sf::Clock clock;
    float timeSinceLastUpdate = 0.f;

    while (window.isOpen())
    {
        float deltaTime = clock.restart().asSeconds();
        timeSinceLastUpdate += deltaTime;


        while (auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                saveAndSubmitScore();
                window.close();
                return;
            }
            switch (currentState)
            {
            case GameState::SPLASH:
                splashScreen.handleEvent(*event);
                break;
            case GameState::LOGIN:
                handleLoginEvents(*event);
                break;
            case GameState::SIGNUP:
                handleSignupEvents(*event);
                break;   // NEW
            case GameState::MENU:
                handleMainMenuEvents(*event);
                break;
            case GameState::LEADERBOARD:
                leaderboardScreen.handleEvent(*event);
                if (leaderboardScreen.done) {
                    leaderboardScreen.done = false;
                    currentState = GameState::MENU;
                }
                break;
            case GameState::PAUSED:
                handlePauseEvents(*event); break;
            case GameState::GAME_OVER:
                handleGameOverEvents(*event); break;
            case GameState::LEVEL_SELECT:
                levelSelectScreen.handleEvent(*event);
                if (levelSelectScreen.done) {
                    if (levelSelectScreen.result > 0) {
                        levelsManager.SpecificLevel(levelSelectScreen.result);
                        gameLevel.loadLevel(levelsManager.getCurrentLevel());
                        if (twoPlayerMode)
                            player2.resetForNewLevel(sf::Vector2f(600.f, 500.f));
                        currentState = GameState::PLAYING;
                        soundManager.playGameMusic();
                    }
                    else {
                        // ESC / Back ? return to menu
                        currentState = GameState::MENU;
                    }
                    levelSelectScreen.reset();
                }
                break;
            case GameState::KEY_REMAP:
                keyRemapScreen.handleEvent(*event);
                if (keyRemapScreen.done) {
                    keyBindings.saveToFile(playerData.getUsername());
                    currentState = GameState::PAUSED;
                    keyRemapScreen.done = false;
                }
                break;
                // Game.cpp ? inside run(), in the switch(currentState) block
// ADD this case, right before the default: break

            /*case GameState::SHOP:
                shopScreen.handleEvent(*event);
                if (shopScreen.done) {
                    shopScreen.done = false;
                    currentState = GameState::MENU;
                }
                break;*/

            case GameState::SHOP:
                shopScreen.handleEvent(*event);
                if (shopScreen.done) {
                    shopScreen.done = false;
                    if (twoPlayerMode) {
                        if (player1.isSnowballPowerActive())
                            player2.activateSnowballPower();
                        if (player1.isDistanceIncreaseActive())
                            player2.activateDistanceIncrease();
                    }
                    currentState = prevState;
                }
                break;
            case GameState::STAR_LEVEL:
                handleStarLevelEvents(*event);
                break;
            default: break;
            }
        }

        while (timeSinceLastUpdate >= TIME_PER_FRAME)

        {
            processInput();
            update(TIME_PER_FRAME);
            timeSinceLastUpdate -= TIME_PER_FRAME;
        }

        draw();
    }
}



void Game::handleLoginEvents(sf::Event& event)
{
    LoginResult result = loginScreen.handleEvent(event);
    if (result == LoginResult::NEW_GAME) {
        saveAndSubmitScore();
        signupScreen.setUsername(loginScreen.getUsername());
        currentState = GameState::SIGNUP;
    }
    else if (result == LoginResult::CONTINUE) {
        saveAndSubmitScore();
        FileManager::loadPlayerData(loginScreen.getUsername(), playerData);
        keyBindings.loadFromFile(playerData.getUsername());
        mainMenu.setUsername(playerData.getUsername());
        mainMenu.reset();
        currentState = GameState::MENU;
        soundManager.playMenuMusic();
    }
    else if (result == LoginResult::QUIT) {
        window.close();
    }
}

void Game::handleSignupEvents(sf::Event& event)
{
    SignupResult result = signupScreen.handleEvent(event);
    if (result == SignupResult::DONE) {
        playerData = PlayerData();
        playerData.setUsername(signupScreen.getUsername());
        keyBindings = KeyBindings();
        mainMenu.setUsername(playerData.getUsername());
        mainMenu.reset();
        currentState = GameState::MENU;
        soundManager.playMenuMusic();
    }
    else if (result == SignupResult::BACK) {
        currentState = GameState::LOGIN;
    }
}

void Game::handleMainMenuEvents(sf::Event& event)
{
    MainMenuResult result = mainMenu.handleEvent(event);
    if (result == MainMenuResult::START_GAME)
    {
        twoPlayerMode = false; // to make sure multiplayer is off

        player1.setKeys(
            Keyboard::Key::Left,
            Keyboard::Key::Right,
            Keyboard::Key::Up,
            Keyboard::Key::Space
        );
        player1.loadTexture("assets\\images\\Nick.png");
        levelSelectScreen.reset();
        currentState = GameState::LEVEL_SELECT;
        levelsManager.SpecificLevel(playerData.getCurrentLevel());
        // levelsManager.SpecificLevel(5);
        gameLevel.loadLevel(levelsManager.getCurrentLevel());

        soundManager.playSound("gamestart");
        soundManager.playGameMusic();
    }


    else if (result == MainMenuResult::START_2PLAYER)
    {
        twoPlayerMode = true;
        player2Data.setLives(2);
        player2Data.setGemCount(0);
        player1.loadTexture("assets\\images\\Nick.png");
        player2.loadTexture("assets\\images\\Nick.png"); // or a different texture

        /*player2.setKeys(
            Keyboard::Key::Left,
            Keyboard::Key::Right,
            Keyboard::Key::Up,
            Keyboard::Key::Space
        );*/


        player1.setKeys(
            Keyboard::Key::A,
            Keyboard::Key::D,
            Keyboard::Key::W,
            Keyboard::Key::S
        );
        player2.setKeys(
            Keyboard::Key::Left,
            Keyboard::Key::Right,
            Keyboard::Key::Up,
            Keyboard::Key::Space
        );


        levelSelectScreen.reset();               // ADD
        currentState = GameState::LEVEL_SELECT;  // CHANGE (was PLAYING)
        levelsManager.SpecificLevel(playerData.getCurrentLevel());
        gameLevel.loadLevel(levelsManager.getCurrentLevel());

        player2.resetForNewLevel(sf::Vector2f(600.f, 500.f));

        soundManager.playSound("gamestart");  // ADD
        soundManager.playGameMusic();         // replaces playMusic()
    }

    else if (result == MainMenuResult::OPEN_SHOP) {
        prevState = currentState;
        shopScreen.reset();
        currentState = GameState::SHOP;
    }
    else if (result == MainMenuResult::OPEN_LEADERBOARD) {
        leaderboard.load();
        leaderboardScreen.done = false;
        currentState = GameState::LEADERBOARD;
    }
    else if (result == MainMenuResult::OPEN_KEYREMAP) {
        keyRemapScreen.done = false;
        currentState = GameState::KEY_REMAP;
    }
    else if (result == MainMenuResult::LOGOUT) {
        saveAndSubmitScore();
        FileManager::savePlayerData(playerData);
        currentState = GameState::LOGIN;
        soundManager.stopAll();   // ADD ? silence everything on logout
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
        soundManager.playGameMusic();
    }
    else if (result == PauseResult::OPEN_SHOP) {
        prevState = currentState;
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
        soundManager.playMenuMusic();
    }
}
void Game::handleGameOverEvents(sf::Event& event) {
    GameOverResult result = gameOverScreen.handleEvent(event);
    if (result == GameOverResult::RETRY) {
        playerData.setLives(2);
        playerData.setCurrentLevel(1);
        levelsManager.SpecificLevel(1);
        player1.loadTexture("assets\\images\\Nick.png");
        gameLevel.loadLevel(levelsManager.getCurrentLevel());
        gameOverScreen.reset();
        currentState = GameState::PLAYING;
        // soundManager.playMusic();
    }
    else if (result == GameOverResult::QUIT_TO_MENU) {
        saveAndSubmitScore();
        mainMenu.reset();
        currentState = GameState::MENU;
        //  soundManager.stopMusic();
    }
}
void Game::handleStarLevelEvents(sf::Event& event)
{
    starLevelScreen.handleEvent(event);
    if (!starLevelScreen.done) return;

    string powerUp = starLevelScreen.getChosenPowerUp();
    if (powerUp == "Speed Boost") {
        player1.activateSpeedBoost(15.f);
        if (twoPlayerMode) player2.activateSpeedBoost(15.f);
    }
    else if (powerUp == "Snowball Power") {
        player1.activateSnowballPower();
        if (twoPlayerMode) player2.activateSnowballPower();
    }
    else if (powerUp == "Distance Increase") {
        player1.activateDistanceIncrease();
        if (twoPlayerMode) player2.activateDistanceIncrease();
    }
    else if (powerUp == "Balloon Mode") {
        player1.activateBalloonMode(10.f);
        if (twoPlayerMode) player2.activateBalloonMode(10.f);
    }

    starLevelScreen.done = false;
    currentState = GameState::PLAYING;
}
void Game::processInput()
{
    if (currentState == GameState::PLAYING) {
        if (inputManager.isKeyHeld(keyBindings.pause)) {
            currentState = GameState::PAUSED;
            pauseScreen.reset();
            soundManager.playLoginMusic();
        }
    }

    // TEMP: press G to force game over screen for testing
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::G)) {
        gameOverScreen.reset();
        currentState = GameState::GAME_OVER;
        soundManager.playGameOverMusic();
    }
}

void Game::update(float deltaTime)
{
    if (currentState == GameState::SPLASH) {
        splashScreen.update(deltaTime);

        if (splashScreen.isDone()) {
            soundManager.stopAll();
            currentState = GameState::LOGIN;
            soundManager.playLoginMusic();   // ADD
        }


        return;
    }
    if (currentState != GameState::PLAYING) return;



    if (playerData.getLives() <= 0) {
        currentState = GameState::GAME_OVER;
        soundManager.playGameOverMusic();   // ADD
        saveAndSubmitScore();
        gameOverScreen.reset();


        /*gameOverScreen.reset();
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
    */
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
                currentState = GameState::GAME_OVER;
                soundManager.playGameOverMusic();   // ADD
                saveAndSubmitScore();
                gameOverScreen.reset();
                currentState = GameState::GAME_OVER;
            }
            else
            {
                if (levelsManager.getCurrentLevel().isStarlevel()) {
                    starLevelScreen.reset();
                    currentState = GameState::STAR_LEVEL;
                }
                gameLevel.loadLevel(levelsManager.getCurrentLevel());
                soundManager.playSound("levelstart");  // ADD ? fanfare
                soundManager.playGameMusic();          // ADD ? BGM restarts
            }
        }
    }

}

void Game::draw() {
    window.clear(sf::Color::Black);//Clears the screen to black

    switch (currentState)  //draws whatever belongs to the current state, then displays it.
        //how you move between menus, gameplay, pause, and game over.
    {
    case GameState::SPLASH:
        splashScreen.draw(window);
        break;
    case GameState::LOGIN:
        loginScreen.draw(window);
        break;
    case GameState::SIGNUP:
        signupScreen.draw(window);   // NEW
        break;
    case GameState::MENU:
        mainMenu.draw(window);
        break;
    case GameState::LEADERBOARD:
        leaderboardScreen.draw(window);
        break;
    case GameState::PLAYING:
        gameLevel.draw(window, player1, twoPlayerMode ? &player2 : nullptr);
        hud.draw(window, playerData, twoPlayerMode ? &player2Data : nullptr);
        //hud.draw(window, playerData); // this was when we had single player only
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
    case GameState::LEVEL_SELECT:
        levelSelectScreen.draw(window);
        break;
    case GameState::STAR_LEVEL:
        starLevelScreen.draw(window);
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