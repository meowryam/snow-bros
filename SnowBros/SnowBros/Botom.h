#pragma once
#include "Enemy.h"
#include "Platform.h"
#include <optional>        // ← add this
#include <string> 
using namespace sf;
class Botom : public Enemy {

protected:
    float jumpTimer;
    float jumpInterval;   // how often it jumps
    bool  isJumping;
    static constexpr float JUMP_FORCE = -600.f;
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
    static constexpr int HITS_TO_ENCASE = 4;

    bool showDebug;
    bool rolling;
    double rollSpeed;
    // Second texture for snow encasing frames (from Nick.png)
    sf::Texture              snowTexture;
    std::optional<sf::Sprite> snowSprite;
    bool                     snowTextureLoaded = false;

    // Escape timer — how long encased before reviving
    float escapeTimer = 0.f;
    static constexpr float ESCAPE_DURATION = 5.f;

    // All 16 snow frames
    sf::IntRect snow_r1_s1{ {  85, 3388}, {249, 198} };
    sf::IntRect snow_r1_s2{ { 374, 3309}, {275, 273} };
    sf::IntRect snow_r1_s3{ { 677, 3295}, {289, 281} };
    sf::IntRect snow_r1_s4{ { 983, 3255}, {287, 316} };
    sf::IntRect snow_r2_s1{ {  64, 3571}, {284, 314} };
    sf::IntRect snow_r2_s2{ { 361, 3584}, {300, 298} };
    sf::IntRect snow_r2_s3{ { 674, 3575}, {278, 300} };
    sf::IntRect snow_r2_s4{ { 976, 3574}, {303, 304} };
    sf::IntRect snow_r3_s1{ {  78, 3881}, {252, 302} };
    sf::IntRect snow_r3_s2{ { 352, 3872}, {315, 311} };
    sf::IntRect snow_r3_s3{ { 680, 3872}, {282, 311} };
    sf::IntRect snow_r3_s4{ { 964, 3870}, {323, 313} };
    sf::IntRect snow_r4_s1{ {  64, 4183}, {288, 317} };
    sf::IntRect snow_r4_s2{ { 371, 4184}, {293, 316} };
    sf::IntRect snow_r4_s3{ { 671, 4178}, {284, 309} };
    sf::IntRect snow_r4_s4{ { 971, 4184}, {308, 298} };
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
    void loadSnowTexture(const std::string& path);
};