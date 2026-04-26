#pragma once
#include "Botom.h"
#include "sfmlcolours.h"
using namespace sfmlcolours;

//l6-7, 3 hits, 1.25x
class GreenBotom : public Botom {
private:
    static constexpr int   GREEN_HITS = 3;
    static constexpr float GREEN_SPEED = 80.f * 1.25f;  
    int variantHits;

public:
    GreenBotom(double startX, double startY, float scrW = 800.f, float scrH = 600.f)
        : Botom(startX, startY, scrW, scrH)
    {
        speed = GREEN_SPEED;
        xspeed = (rand() % 2 == 0) ? speed : -speed;
        variantHits = 0;
    }

    bool receiveSnowballHit() { //override
        if (trap || !alive) return false;
        variantHits++;
        hitFlashTimer = HIT_FLASH_DURATION;

        if (variantHits == 1) {xspeed *= 0.75;} //slow down

        if (variantHits >= GREEN_HITS) {
            trap = true;
            xspeed = 0.0;
            yspeed = 0.0;
            rolling = false;
            return true;
        }
        return false;
    }

    void draw(RenderWindow& window) override {
        if (!alive) return;

        RectangleShape rect(Vector2f(hitboxbotom_width, hitboxbotom_height));
        rect.setPosition(Vector2f(static_cast<float>(x), static_cast<float>(y)));
        if (trap) rect.setFillColor(Color::White);
        else if (hitFlashTimer > 0.f) rect.setFillColor(PaleGreen); 
        else rect.setFillColor(Green);      
        window.draw(rect);

        if (showDebug) {
            RectangleShape outline(Vector2f(hitboxbotom_width, hitboxbotom_height));
            outline.setPosition(Vector2f(static_cast<float>(x), static_cast<float>(y)));
            outline.setFillColor(Color::Transparent);
            outline.setOutlineThickness(1.f);
            outline.setOutlineColor(sf::Color::Red);
            window.draw(outline);
        }
    }
};


//l7-7, 4 hits, 1.5x
class BlueBotom : public Botom {
private:
    static constexpr int   BLUE_HITS = 4;
    static constexpr float BLUE_SPEED = 80.f * 1.5f; 
    int variantHits;

public:
    BlueBotom(double startX, double startY, float scrW = 800.f, float scrH = 600.f)
        : Botom(startX, startY, scrW, scrH)
    {
        speed = BLUE_SPEED;
        xspeed = (rand() % 2 == 0) ? speed : -speed;
        variantHits = 0;
    }

    bool receiveSnowballHit() {
        if (trap || !alive) return false;

        variantHits++;
        hitFlashTimer = HIT_FLASH_DURATION;

        if (variantHits == 1) xspeed *= 0.75;

        if (variantHits >= BLUE_HITS) {
            trap = true;
            xspeed = 0.0;
            yspeed = 0.0;
            rolling = false;
            return true;
        }
        return false;
    }

    void draw(sf::RenderWindow& window) override {
        if (!alive) return;

        sf::RectangleShape rect(sf::Vector2f(hitboxbotom_width, hitboxbotom_height));
        rect.setPosition(sf::Vector2f(static_cast<float>(x), static_cast<float>(y)));

        if (trap) rect.setFillColor(sf::Color::White);
        else if (hitFlashTimer > 0.f) rect.setFillColor(PaleBlue); 
        else rect.setFillColor(Blue); 

        window.draw(rect);

        if (showDebug) {
            sf::RectangleShape outline(sf::Vector2f(hitboxbotom_width, hitboxbotom_height));
            outline.setPosition(sf::Vector2f(static_cast<float>(x), static_cast<float>(y)));
            outline.setFillColor(sf::Color::Transparent);
            outline.setOutlineThickness(1.f);
            outline.setOutlineColor(sf::Color::Red);
            window.draw(outline);
        }
    }
};


//l8-9, 5 hits, 1.75x
class PurpleBotom : public Botom {
private:
    static constexpr int   PURPLE_HITS = 5;
    static constexpr float PURPLE_SPEED = 80.f * 1.75f;  // 140 px/s

    int variantHits;

public:
    PurpleBotom(double startX, double startY,
        float scrW = 800.f, float scrH = 600.f)
        : Botom(startX, startY, scrW, scrH)
    {
        speed = PURPLE_SPEED;
        xspeed = (rand() % 2 == 0) ? speed : -speed;
        variantHits = 0;
    }

    bool receiveSnowballHit() {
        if (trap || !alive) return false;

        variantHits++;
        hitFlashTimer = HIT_FLASH_DURATION;

        if (variantHits == 1) xspeed *= 0.75;

        if (variantHits >= PURPLE_HITS) {
            trap = true;
            xspeed = 0.0;
            yspeed = 0.0;
            rolling = false;
            return true;
        }
        return false;
    }

    void draw(RenderWindow& window) override {
        if (!alive) return;

        RectangleShape rect(Vector2f(hitboxbotom_width, hitboxbotom_height));
        rect.setPosition(Vector2f(static_cast<float>(x), static_cast<float>(y)));

        if (trap) rect.setFillColor(Color::White);
        else if (hitFlashTimer > 0.f) rect.setFillColor(Lilac);
        else rect.setFillColor(Purple);   
        window.draw(rect);

        if (showDebug) {
            RectangleShape outline(Vector2f(hitboxbotom_width, hitboxbotom_height));
            outline.setPosition(Vector2f(static_cast<float>(x), static_cast<float>(y)));
            outline.setFillColor(Color::Transparent);
            outline.setOutlineThickness(1.f);
            outline.setOutlineColor(Color::Red);
            window.draw(outline);
        }
    }
};
