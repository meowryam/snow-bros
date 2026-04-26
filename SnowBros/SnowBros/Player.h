#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>
#include "PlayerData.h"
#include "LevelLayout.h"
using namespace sf;
enum class PlayerState {
    IDLE,       
    WALKING,    
    JUMPING,    
    FALLING,    
    THROWING,   
    DEAD        
};
enum class Direction {
    LEFT,
    RIGHT
};
class Player {
private:
    Texture texture;
    Sprite sprite;
    Vector2f position;
    Vector2f velocity;
    // Each frame we do: position += velocity * deltaTime
    float speed;
    static constexpr float BASE_SPEED = 200.f;
    static constexpr float JUMP_FORCE = -600.f;
    static constexpr float GRAVITY = 1200.f;
    static constexpr float MAX_FALL_SPEED = 800.f;
    bool wantsToThrow;  // true for exactly one frame when space pressed
    bool isOnGround;
    bool canJump;
    float screenWidth;
    float screenHeight;
    PlayerState prevState = PlayerState::IDLE;
    Keyboard::Key keyLeft;
    Keyboard::Key keyRight;
    Keyboard::Key keyJump;
    Keyboard::Key keyThrow;
  
    PlayerState state;
    Direction facing;
    bool isAlive;
    sf::FloatRect hitbox;
    bool showDebug;
    float invincibleTimer;
    sf::RectangleShape debugBox;
    bool speedBoostActive;
    float speedBoostTimer;      
    bool balloonModeActive;
    float balloonModeTimer;     
    bool snowballPowerActive;
    bool distanceIncreaseActive;
    
    PlayerData& playerData;
    int playerNumber;
   
    // ── Animation ─────────────────────────────────────
    float animTimer = 0.f;
    int   animFrame = 0;
    static constexpr float FRAME_DURATION = 0.12f;

    // Walk Right (5 frames)
    sf::IntRect walk_right1{ {110,  76}, {220, 275} };
    sf::IntRect walk_right2{ {407,  74}, {196, 281} };
    sf::IntRect walk_right3{ {702,  70}, {226, 287} };
    sf::IntRect walk_right4{ {1003, 67}, {213, 276} };
    sf::IntRect walk_right5{ {1287, 55}, {243, 305} };

    // Walk Left (6 frames)
    sf::IntRect walk_left1{ { 61, 367}, {297, 275} };
    sf::IntRect walk_left2{ {381, 354}, {269, 245} };
    sf::IntRect walk_left3{ {672, 364}, {273, 254} };
    sf::IntRect walk_left4{ {1002,369}, {230, 281} };
    sf::IntRect walk_left5{ {1295,373}, {282, 269} };
    sf::IntRect walk_left6{ {1595,376}, {278, 262} };

    // Jump (3 frames)
    sf::IntRect jump1{ {1579,660}, {318, 312} };
    sf::IntRect jump2{ {1891,654}, {293, 336} };
    sf::IntRect jump3{ {2203,656}, {290, 309} };

    // Throw (7 frames)
    sf::IntRect throw1{ {  48, 969}, {322, 312} };
    sf::IntRect throw2{ { 361, 969}, {293, 314} };
    sf::IntRect throw3{ { 664,1029}, {304, 266} };
    sf::IntRect throw4{ { 963,1011}, {309, 263} };
    sf::IntRect throw5{ {1268,1012}, {311, 205} };
    sf::IntRect throw6{ {1577,1014}, {306, 181} };
    sf::IntRect throw7{ {1881, 999}, {316, 159} };

    // Hurt (7 frames)
    sf::IntRect hurt1{ {  73,1275}, {254, 211} };
    sf::IntRect hurt2{ { 336,1287}, {248, 254} };
    sf::IntRect hurt3{ { 755,1302}, {225, 233} };
    sf::IntRect hurt4{ { 980,1271}, {264, 305} };
    sf::IntRect hurt5{ {1280,1274}, {264, 303} };
    sf::IntRect hurt6{ {1598,1283}, {269, 293} };
    sf::IntRect hurt7{ {1897,1289}, {260, 288} };

public:
    Player(); 
    Player(PlayerData& data, int playerNum, float screenW, float screenH);
    bool loadTexture(const std::string& path);
  
    void handleInput();
    void update(float deltaTime);
    void draw(sf::RenderWindow& window) const;
    void landOnPlatform(float topOfPlatform);
    void leaveGround();
    void takeDamage();
    void collectLife();
    void activateSpeedBoost(float duration);
    void activateBalloonMode(float duration);
    void activateSnowballPower();   
    void activateDistanceIncrease(); 
    void resetForNewLevel(sf::Vector2f spawnPosition);
    Vector2f getPosition() const;
    FloatRect getHitbox() const;
    PlayerState getState() const;
    Direction getFacing() const;
    bool getIsAlive() const;
    bool isSnowballPowerActive() const;
    bool isDistanceIncreaseActive() const;
    bool isBalloonModeActive() const;
    void setShowDebug(bool show);
    void setOnGround(bool onGround);
    // remove: bool isInvincible() const { return false; }
    bool isInvincible() const { return invincibleTimer > 0.f; }
    void resolvePlatforms(Platform platforms[], int count);
    bool getWantsToThrow() const { return wantsToThrow; }

};