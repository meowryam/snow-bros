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

    sf::IntRect unused_frame1{ {  8,  22}, { 93,  94} };
    sf::IntRect unused_frame2{ { 99,  22}, { 93,  94} };
    sf::IntRect idle{ {  8, 136}, { 86,  87} };
    sf::IntRect charging_frame1{ {  2, 250}, { 94,  87} };
    sf::IntRect charging_frame2{ { 97, 254}, { 94,  87} };
    sf::IntRect walking{ {  3, 366}, { 94,  87} };
    sf::IntRect walking_frame2{ { 95, 364}, { 94,  87} };
    sf::IntRect walking_frame3{ {192, 367}, { 94,  87} };
    sf::IntRect falling{ {  5, 481}, { 94,  95} };
    sf::IntRect topped_frame1{ {  4, 596}, { 93,  94} };
    sf::IntRect topped_frame2{ {  4, 712}, { 93,  91} };
    sf::IntRect topped_frame3{ { 98, 711}, { 93,  91} };
    sf::IntRect unsheathing_frame1{ {  1, 824}, { 93,  91} };
    sf::IntRect unsheathing_frame2{ { 97, 824}, { 93,  91} };
    sf::IntRect unsheathing_frame3{ {195, 823}, { 93,  91} };
    sf::IntRect staring{ {  3, 939}, { 93,  91} };
    sf::IntRect dying_frame1{ {  3,1055}, { 93,  93} };
    sf::IntRect dying_frame2{ { 97,1052}, { 93,  93} };
    sf::IntRect dying_frame3{ {190,1054}, { 93,  93} };
    sf::IntRect dying_frame4{ {280,1047}, {106, 100} };
    sf::IntRect dying_frame5{ {388,1044}, { 88, 113} };
    sf::IntRect dying_frame6{ {475,1056}, { 94,  95} };
    sf::IntRect dying_frame7{ {571,1047}, { 96, 100} };
    sf::IntRect dying_frame8{ {667,1066}, { 92,  85} };
    sf::IntRect dying_frame9{ {759,1088}, { 95,  62} };
    sf::IntRect frozen{ {  4,1167}, { 96,  91} };
   

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