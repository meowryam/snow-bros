#pragma once
#include "FlyingFoogaFoog.h"

struct Knife {

    double x, y;           
    double xspeed, yspeed; 
    bool   alive;          
    static constexpr float knifehitboxwidth = 16.f;
    static constexpr float knifehitboxheight = 8.f;

    Knife() : x(0), y(0), xspeed(0), yspeed(0), alive(false) {}
    void update(double deltaTime) {
        if (!alive) return;
        x += xspeed * deltaTime;
        y += yspeed * deltaTime;
    }

    FloatRect getHitbox() const {
        return FloatRect(
            Vector2f(static_cast<float>(x), static_cast<float>(y)),
            Vector2f(knifehitboxwidth, knifehitboxheight)
        );
    }
    void draw(sf::RenderWindow& window) const {
        if (!alive) return;
        RectangleShape rect(Vector2f(knifehitboxwidth, knifehitboxheight));
        rect.setPosition(Vector2f(static_cast<float>(x), static_cast<float>(y)));
        rect.setFillColor(Color(255, 220, 50));  
        window.draw(rect);
    }
};

class Tornado : public FlyingFoogaFoog {
public:
    static const int MAX_KNIVES = 6;  
    Knife knives[MAX_KNIVES];
private:
    double playerX, playerY;
    float knifeTimer;
    static constexpr float KNIFE_INTERVAL = 2.5f;  
    static constexpr float KNIFE_SPEED = 350.f;  
    float currentFlightSpeed;
    static constexpr float MIN_FLIGHT_SPEED = 60.f;   
    static constexpr float MAX_FLIGHT_SPEED = 320.f;  
    void fireKnife();
    void randomizeFlightSpeed();

public:

    Tornado(double startX, double startY,
        float scrW = 800.f, float scrH = 600.f);
    void setPlayerPos(double px, double py);
    void update(double deltaTime) override;
    void draw(sf::RenderWindow& window) override;
};