#pragma once
#include "Entity.h"
#include "Platform.h"
using namespace sf;
enum class SnowballState { FLYING, ROLLING };
class Snowball : public Entity {
private:
    
    Texture texture;
    optional<Sprite> sprite;
    bool textureLoaded = false;
    float animTimer = 0.f;
    int animFrame = 0;
    static constexpr float FRAME_DURATION = 0.12f;
    IntRect proj1{ { 16, 814}, {63, 63} };
    IntRect proj2{ { 101, 821}, {65, 56} };
    IntRect proj3{ {178, 834}, {69, 43} };
    IntRect proj4{ {253, 843}, {80, 33} };
    IntRect proj5{ {  338, 854}, {84, 21} };
    IntRect flame_blue{ {331, 826}, {123, 134} };
    IntRect flame_red{ {482, 818}, {145, 144} };
    IntRect roll1{ {13, 919}, {57, 49} }; 
    IntRect roll2 { {83, 903}, {65, 65} }; 
    IntRect roll3 { {176, 899}, {72, 70} }; 
    IntRect roll4 { {260, 883}, {69, 86} }; 
    IntRect roll5 { {6, 968}, {74, 84} }; 
    IntRect roll6 { {88, 971}, {75, 81} }; 
    IntRect roll7 { {172, 968}, {70, 82} }; 
    IntRect roll8 { {256, 968}, {74, 82} }; 
    IntRect roll9 { {10, 1054}, {65, 82} }; 
    IntRect roll10{ {86, 1052}, {74, 84} }; 
    IntRect roll11{ {173, 1052}, {75,84} }; 
    IntRect roll12{ {253, 1052}, {82,84} }; 
    IntRect break1{ {29, 1284}, { 66, 55} };
    IntRect break2{ {151, 1277}, { 75, 61} };
    IntRect break3{ {267, 1259}, { 96,  81} };
    IntRect break4{ {378, 1236}, {115, 101} };
    IntRect break5{ {4, 1339}, {123, 127} };
    IntRect break6{ {129, 1342}, {123, 123} };
    IntRect break7{ {253, 1341}, {114, 122} };

    float screenWidth;
    float screenHeight;
    static constexpr float snowballhitboxsize = 24.f;  
    SnowballState snowState;
    bool powerful;
    bool longRange;
    int snowHits; 
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