#pragma once
#include "Enemy.h"
#include "Platform.h"
#include <optional>        // ← add this
#include <string> 
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

    // ── Sprite sheet ──────────────────────────────────────
    sf::Texture              texture;
    std::optional<sf::Sprite> sprite;
    bool                     textureLoaded = false;

    sf::IntRect unused_frame1{ {  3, 238 }, {  97, 106 } };
    sf::IntRect unused_frame2{ { 96, 244 }, {  98,  97 } };
    sf::IntRect idle{ {  5, 363 }, {  95,  93 } };
    sf::IntRect charging_frame1{ { 96, 360 }, {  94,  95 } };
    sf::IntRect charging_frame2{ {190, 395 }, { 102,  96 } };
    sf::IntRect walking{ {  7, 469 }, {  91, 107 } };
    sf::IntRect falling{ {  4, 591 }, {  90, 101 } };
    sf::IntRect topped_frame1{ {  3, 708 }, {  96,  98 } };
    sf::IntRect topped_frame2{ { 99, 702 }, {  95,  98 } };
    sf::IntRect unsheathing_frame1{ {  4, 816 }, {  93,  97 } };
    sf::IntRect unsheathing_frame2{ { 93, 819 }, {  97,  94 } };
    sf::IntRect unsheathing_frame3{ {189, 819 }, {  99,  94 } };
    sf::IntRect staring{ {  3, 935 }, {  93,  93 } };
    sf::IntRect dying_frame1{ {  0, 1048}, { 100, 101 } };
    sf::IntRect dying_frame2{ { 98, 1053}, {  93,  87 } };
    sf::IntRect dying_frame3{ {189, 1056}, {  98,  91 } };
    sf::IntRect dying_frame4{ {281, 1056}, { 101,  84 } };
    sf::IntRect dying_frame5{ {386, 1058}, {  85,  91 } };
    sf::IntRect dying_frame6{ {473, 1064}, {  93,  83 } };
    sf::IntRect dying_frame7{ {570, 1056}, {  96,  90 } };
    sf::IntRect dying_frame8{ {666, 1069}, {  96,  74 } };
    sf::IntRect dying_frame9{ {759, 1090}, {  98,  55 } };
    sf::IntRect frozen{ {  1, 1161}, { 103,  97 } };
    // Frozen (fully encased)
   

    // ── Animation state ───────────────────────────────────
    float animTimer = 0.f;
    int   animFrame = 0;          // current frame index within active anim
    static constexpr float FRAME_DURATION = 0.15f; // seconds per frame


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
    void loadTexture(const std::string& path);
};