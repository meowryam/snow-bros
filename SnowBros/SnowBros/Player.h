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
    
    bool isOnGround;
    bool canJump;
    float screenWidth;
    float screenHeight;
  
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
};