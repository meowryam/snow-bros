#pragma once
#include "Entity.h"
#include "Platform.h"
using namespace sf;
class Snowball : public Entity {
private:
    float screenWidth;
    float screenHeight;
    static constexpr float snowballhitboxsize = 24.f;  
    bool powerful;   
    bool longRange;   
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
};