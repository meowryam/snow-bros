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
// Replace proj1-5 in Snowball.h with these round snowball frames
    sf::IntRect proj1{ { 13, 919}, {57, 49} };
    sf::IntRect proj2{ { 83, 903}, {65, 65} };
    sf::IntRect proj3{ {176, 899}, {72, 70} };
    sf::IntRect proj4{ {260, 883}, {69, 86} };
    sf::IntRect proj5{ {  6, 968}, {74, 84} };

    // Rolling snowball frames (ROW 9 fat/rolling — grows as it rolls)
  // CORRECT — actual snowball growing frames
    sf::IntRect roll1{ { 13, 919}, {57, 49} };
    sf::IntRect roll2{ { 83, 903}, {65, 65} };
    sf::IntRect roll3{ {176, 899}, {72, 70} };
    sf::IntRect roll4{ {260, 883}, {69, 86} };
    sf::IntRect roll5{ {  6, 968}, {74, 84} };
    sf::IntRect roll6{ { 88, 971}, {75, 81} };

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