#pragma once
#include "Entity.h"
#include "Platform.h"
using namespace sf;
enum class SnowballState { FLYING, ROLLING };
class Snowball : public Entity {
private:
    // Sprite
    sf::Texture texture;
    std::optional<sf::Sprite> sprite;
    bool textureLoaded = false;

    // Animation
    float animTimer = 0.f;
    int   animFrame = 0;
    static constexpr float FRAME_DURATION = 0.12f;

    // Flying snowball frames (from Player_Blue.png ROW 10)
    sf::IntRect proj1{ { 16, 814}, {63, 63} };
    sf::IntRect proj2{ {101, 821}, {63, 56} };
    sf::IntRect proj3{ {178, 834}, {69, 43} };
    sf::IntRect proj4{ {253, 843}, {80, 33} };
    sf::IntRect proj5{ {338, 854}, {84, 21} };

    // Rolling snowball frames (ROW 9 fat/rolling — grows as it rolls)
    sf::IntRect roll1{ {  8, 683}, { 70, 112} };
    sf::IntRect roll2{ { 77, 675}, { 91, 115} };
    sf::IntRect roll3{ {172, 674}, {115, 123} };
    sf::IntRect roll4{ {292, 672}, {119, 131} };
    sf::IntRect roll5{ {419, 673}, {122, 127} };
    sf::IntRect roll6{ {544, 669}, {125, 129} };

    float screenWidth;
    float screenHeight;
    static constexpr float snowballhitboxsize = 24.f;  
    SnowballState snowState;
    bool powerful;
    bool longRange;
    int snowHits;  // how many times this snowball has hit a rolling enemy
    static constexpr double BASE_SPEED = 350.0; 
    static constexpr double LONG_RANGE_SPEED = 600.0; 
    double direction;
public:
    Snowball(double startX, double startY, double dir, bool isPowerful = false, bool isLongRange = false, float scrW = 800.f, float scrH = 600.f);
    void update(double deltaTime) override;
    void draw(RenderWindow& window) override;
    void resolvePlatforms(Platform platforms[], int count);
    bool isRolling() const;
    void addSnow();         
    void setLongRange();     
    FloatRect getHitbox() const;
    float getSize() const;
    void startRolling(double dir);
    void addSnowHit();
    int getSnowHits() const;
    void loadTexture(const std::string& path);
};