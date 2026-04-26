#pragma once
#include "FlyingFoogaFoog.h"
#include "sfmlcolours.h"
using namespace sfmlcolours;
// ============================================================
//  FlyingFoogaFoog Color Variants — for levels 6-9
//
//  All variants inherit FlyingFoogaFoog completely.
//  Flight behavior, ground walk, gravity, encasing, rolling
//  are all unchanged. We only adjust:
//  - Speed (ground walk speed — flight speed scales with it)
//  - Hits to encase
//  - Draw color
// ============================================================


// ============================================================
//  GreenFooga — Levels 6-7
//  1.25x speed, 3 hits to encase
// ============================================================
class GreenFooga : public FlyingFoogaFoog {
private:
    static constexpr int   GREEN_HITS = 3;
    static constexpr float GREEN_SPEED = 80.f * 1.25f;

    int variantHits;

public:
    GreenFooga(double startX, double startY,
        float scrW = 800.f, float scrH = 600.f)
        : FlyingFoogaFoog(startX, startY, scrW, scrH)
    {
        speed = GREEN_SPEED;
        xspeed = (rand() % 2 == 0) ? speed : -speed;
        variantHits = 0;
    }

    bool receiveSnowballHit() {
        if (trap || !alive) return false;

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

    void draw(sf::RenderWindow& window) override {
        if (!alive) return;

        sf::RectangleShape rect(sf::Vector2f(hitboxbotom_width, hitboxbotom_height));
        rect.setPosition(sf::Vector2f(static_cast<float>(x), static_cast<float>(y)));

        if (trap)                     rect.setFillColor(sf::Color::White);
        else if (hitFlashTimer > 0.f) rect.setFillColor(PaleGreen);
        else if (inFlight)            rect.setFillColor(Lime); // bright green in flight
        else                          rect.setFillColor(DarkGreen);   // dark green on ground

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


// ============================================================
//  BlueFooga — Levels 7-8
//  1.5x speed, 4 hits to encase
// ============================================================
class BlueFooga : public FlyingFoogaFoog {
private:
    static constexpr int   BLUE_HITS = 4;
    static constexpr float BLUE_SPEED = 80.f * 1.5f;

    int variantHits;

public:
    BlueFooga(double startX, double startY,
        float scrW = 800.f, float scrH = 600.f)
        : FlyingFoogaFoog(startX, startY, scrW, scrH)
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
        else if (inFlight)            rect.setFillColor(LightBlue); // light blue in flight
        else                          rect.setFillColor(DarkBlue);   // dark blue on ground

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


// ============================================================
//  PurpleFooga — Levels 8-9
//  1.75x speed, 5 hits to encase
// ============================================================
class PurpleFooga : public FlyingFoogaFoog {
private:
    static constexpr int   PURPLE_HITS = 5;
    static constexpr float PURPLE_SPEED = 80.f * 1.75f;

    int variantHits;

public:
    PurpleFooga(double startX, double startY,
        float scrW = 800.f, float scrH = 600.f)
        : FlyingFoogaFoog(startX, startY, scrW, scrH)
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

        sf::RectangleShape rect(sf::Vector2f(hitboxbotom_width, hitboxbotom_height));
        rect.setPosition(sf::Vector2f(static_cast<float>(x), static_cast<float>(y)));

        if (trap)                     rect.setFillColor(sf::Color::White);
        else if (hitFlashTimer > 0.f) rect.setFillColor(Lilac);
        else if (inFlight)            rect.setFillColor(LightPurple);//ight purple in flight
        else                          rect.setFillColor(DarkPurple);   // dark purple on ground

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