#pragma once
#include "Enemy.h"
#include "Platform.h"
using namespace sf;
class Botom : public Enemy {

protected:

    float screenWidth;
    float screenHeight;
    static constexpr float GRAVITY = 1200.f;
    static constexpr float MAX_FALL = 800.f;   
    bool isOnGround;   
    float directionTimer;      
    float directionInterval;   // how long before next flip (randomized)
    float hitFlashTimer;      
    static constexpr float HIT_FLASH_DURATION = 0.15f;
    float shakeoutTimer;
    static constexpr float SHAKEOUT_DURATION = 5.0f;  
    static constexpr float hitboxbotom_width = 36.f;
    static constexpr float hitboxbotom_height = 44.f;
    int snowballHits;
    static constexpr int HITS_TO_ENCASE = 2;

    bool showDebug;
    bool rolling;
    double rollSpeed;
public:

    Botom(double startX, double startY, float scrW = 800.f, float scrH = 600.f);
    void update(double deltaTime) override;
    void draw(sf::RenderWindow& window) override;
    bool receiveSnowballHit();
    void resolvePlatforms(Platform platforms[], int count);
    FloatRect getHitbox() const;
    void setShowDebug(bool show);
    void startRolling(double dir);
    bool isRolling() const;
};