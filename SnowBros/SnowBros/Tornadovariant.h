#pragma once
#include "Tornado.h"
#include "sfmlcolours.h"
using namespace sfmlcolours;


//l 6-7, 1.25x, 3 hits
class GreenTornado : public Tornado {
private:
    static constexpr int   GREEN_HITS = 3;
    static constexpr float GREEN_SPEED = 80.f * 1.25f;
    static constexpr float GREEN_KNIFE_SPEED = 400.f;  

    int variantHits;

public:
    GreenTornado(double startX, double startY, float scrW = 800.f, float scrH = 600.f)
        : Tornado(startX, startY, scrW, scrH)
    {
        speed = GREEN_SPEED;
        xspeed = (rand() % 2 == 0) ? speed : -speed;
        variantHits = 0;
    }

    bool receiveSnowballHit() { if (trap || !alive) return false;
        variantHits++;
        hitFlashTimer = HIT_FLASH_DURATION;

        if (variantHits == 1) xspeed *= 0.75;

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

        if (trap)                     rect.setFillColor(Color::White);
        else if (hitFlashTimer > 0.f) rect.setFillColor(PaleGreen);
        else if (inFlight)            rect.setFillColor(BrightGreen);
        else                          rect.setFillColor(ForestGreen);

        window.draw(rect);

        if (showDebug) {
            sf::RectangleShape outline(sf::Vector2f(hitboxbotom_width, hitboxbotom_height));
            outline.setPosition(sf::Vector2f(static_cast<float>(x), static_cast<float>(y)));
            outline.setFillColor(Color::Transparent);
            outline.setOutlineThickness(1.f);
            outline.setOutlineColor(Color::Red);
            window.draw(outline);
        }
        for (int i = 0; i < MAX_KNIVES; i++) {
            knives[i].draw(window);
        }
    }
};


class BlueTornado : public Tornado {
private:
    static constexpr int   BLUE_HITS = 4;
    static constexpr float BLUE_SPEED = 80.f * 1.5f;
    static constexpr float BLUE_KNIFE_SPEED = 450.f;

    int variantHits;

public:
    BlueTornado(double startX, double startY, float scrW = 800.f, float scrH = 600.f)
        : Tornado(startX, startY, scrW, scrH)
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

        if (trap)                     rect.setFillColor(sf::Color::White);
        else if (hitFlashTimer > 0.f) rect.setFillColor(SoftBlue);
        else if (inFlight)            rect.setFillColor(BrightBlue);
        else                          rect.setFillColor(OceanBlue);

        window.draw(rect);

        if (showDebug) {
            sf::RectangleShape outline(sf::Vector2f(hitboxbotom_width, hitboxbotom_height));
            outline.setPosition(sf::Vector2f(static_cast<float>(x), static_cast<float>(y)));
            outline.setFillColor(sf::Color::Transparent);
            outline.setOutlineThickness(1.f);
            outline.setOutlineColor(sf::Color::Red);
            window.draw(outline);
        }

        for (int i = 0; i < MAX_KNIVES; i++) {
            knives[i].draw(window);
        }
    }
};

//l 8-9, 1.75x, 5 hits
class PurpleTornado : public Tornado {
private:
    static constexpr int   PURPLE_HITS = 5;
    static constexpr float PURPLE_SPEED = 80.f * 1.75f;
    static constexpr float PURPLE_KNIFE_SPEED = 520.f;  

    int variantHits;

public:
    PurpleTornado(double startX, double startY,
        float scrW = 800.f, float scrH = 600.f)
        : Tornado(startX, startY, scrW, scrH)
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

    void draw(sf::RenderWindow& window) override {
        if (!alive) return;

        RectangleShape rect(Vector2f(hitboxbotom_width, hitboxbotom_height));
        rect.setPosition(Vector2f(static_cast<float>(x), static_cast<float>(y)));

        if (trap)                     rect.setFillColor(Color::White);
        else if (hitFlashTimer > 0.f) rect.setFillColor(Lilac);
        else if (inFlight)            rect.setFillColor(BrightPurple);
        else                          rect.setFillColor(DarkestPurple);

        window.draw(rect);

        if (showDebug) {
            sf::RectangleShape outline(Vector2f(hitboxbotom_width, hitboxbotom_height));
            outline.setPosition(Vector2f(static_cast<float>(x), static_cast<float>(y)));
            outline.setFillColor(Color::Transparent);
            outline.setOutlineThickness(1.f);
            outline.setOutlineColor(Color::Red);
            window.draw(outline);
        }

        for (int i = 0; i < MAX_KNIVES; i++) {
            knives[i].draw(window);
        }
    }
};