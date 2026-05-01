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
    sf::IntRect proj1{ { 16, 814}, {63, 63} };
    sf::IntRect proj2{ { 101, 821}, {65, 56} };
    sf::IntRect proj3{ {178, 834}, {69, 43} };
    sf::IntRect proj4{ {253, 843}, {80, 33} };
    sf::IntRect proj5{ {  338, 854}, {84, 21} };

    sf::IntRect roll1{ { 13, 919}, {57, 49} }; // row1 small — 0 hits
    sf::IntRect roll2 { { 83, 903}, {65, 65} }; // row1 med — 1 hit 
    sf::IntRect roll3 { {176, 899}, {72, 70} }; // row1 large — 2 hits
    sf::IntRect roll4 { {260, 883}, {69, 86} }; // row1 xlarge — 3 hits 
    sf::IntRect roll5 { { 6, 968}, {74, 84} }; // row2 small — 4 hits 
    sf::IntRect roll6 { { 88, 971}, {75, 81} }; // row2 med — 5 hits 
    sf::IntRect roll7 { {172, 968}, {70, 82} }; // row2 large — 6 hits 
    sf::IntRect roll8 { {256, 968}, {74, 82} }; // row2 xlarge — 7 hits 
    sf::IntRect roll9 { { 10, 1054}, {65, 82} }; // row3 small — 8 hits
    sf::IntRect roll10{ { 86, 1052}, {74, 84} }; // row3 med — 9 hits 
    sf::IntRect roll11{ {173, 1052}, {75, 84} }; // row3 large — 10 hits 
    sf::IntRect roll12{ {253, 1052}, {82, 84} }; // row3 xlarge — 11+ hits 
    // Break animation (when snowball hits a wall/edge)
    sf::IntRect break1{ { 29, 1284}, { 66,  55} };
    sf::IntRect break2{ {151, 1277}, { 75,  61} };
    sf::IntRect break3{ {267, 1259}, { 96,  81} };
    sf::IntRect break4{ {378, 1236}, {115, 101} };
    sf::IntRect break5{ {  4, 1339}, {123, 127} };
    sf::IntRect break6{ {129, 1342}, {123, 123} };
    sf::IntRect break7{ {253, 1341}, {114, 122} };

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