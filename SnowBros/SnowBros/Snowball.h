//#include "Projectile.h"
//
//// Player's thrown snowball — grows with hits, becomes rollable when full.
//// This is yours fully (ties to player throw mechanic in T-A4).
//class Snowball : public Projectile {
//private:
//    int snowLevel;   // 0=small, 1=medium, 2=full (can encase enemy)
//    bool rolling;
//    double direction; // +1.0 = right, -1.0 = left
//
//public:
//    Snowball(double x, double y, double dir)
//        : Projectile(x, y, 1), snowLevel(0), rolling(false), direction(dir) {
//        xspeed = dir * 300.0;
//    }
//
//    void addSnow() {
//        if (snowLevel < 2) snowLevel++;
//    }
//
//    bool canEncase()    const { return snowLevel >= 2; }
//    bool isRolling()    const { return rolling; }
//    void startRolling() { rolling = true; xspeed = direction * 200.0; }
//    int  getSnowLevel() const { return snowLevel; }
//
//    void update(double deltaTime) override {
//        if (!alive) return;
//        x += xspeed * deltaTime;
//        y += yspeed * deltaTime;
//        // TODO: add gravity, platform collision, rolling behaviour
//    }
//
//    void draw(sf::RenderWindow& window) override {
//        // TODO: draw correct frame from Items.png based on snowLevel
//    }
//};


#pragma once
#include "Projectile.h"
#include "Platform.h"
#include <optional>

class Snowball : public Projectile {
private:
    int    snowLevel;
    bool   rolling;
    double direction;

    sf::Texture              texture;
    std::optional<sf::Sprite> sprite;
    bool                     textureLoaded;

public:
    Snowball(double x, double y, double dir)
        : Projectile(x, y, 1),
        snowLevel(0), rolling(false), direction(dir),
        textureLoaded(false)
    {
        xspeed = dir * 300.0;
    }

    bool loadTexture(const string& path) {
        if (!texture.loadFromFile(path)) return false;
        sprite.emplace(texture);
        textureLoaded = true;
        return true;
    }

    void addSnow() { if (snowLevel < 2) snowLevel++; }
    bool canEncase()    const { return snowLevel >= 2; }
    bool isRolling()    const { return rolling; }
    int  getSnowLevel() const { return snowLevel; }

    void startRolling() {
        rolling = true;
        xspeed = direction * 200.0;
        yspeed = 0;
    }

    void resolvePlatforms(Platform platforms[], int count) {
        for (int i = 0; i < count; i++) {
            sf::FloatRect& p = platforms[i].rect;
            float sx = (float)x, sy = (float)y;
            float sw = 24.f, sh = 24.f;
            if (yspeed >= 0 &&
                sx + sw > p.position.x &&
                sx < p.position.x + p.size.x &&
                sy + sh >= p.position.y &&
                sy + sh <= p.position.y + p.size.y + 8.f) {
                y = p.position.y - sh;
                yspeed = 0;
                if (!rolling) startRolling();
            }
        }
    }

    void update(double deltaTime) override {
        if (!alive) return;
        yspeed += 900.0 * deltaTime; // gravity
        x += xspeed * deltaTime;
        y += yspeed * deltaTime;

        // screen wrap (spec 7.1)
        if (x > 800) x = -24;
        if (x < -24) x = 800;

        // die if falls off bottom
        if (y > 620) alive = false;
    }

    void draw(sf::RenderWindow& window) override {
        if (!alive) return;

        if (!textureLoaded) {
            sf::RectangleShape r(sf::Vector2f(24.f, 24.f));
            r.setPosition({ (float)x, (float)y });
            r.setFillColor(rolling ? sf::Color::White : sf::Color(200, 220, 255));
            window.draw(r);
            return;
        }

        // frame based on snowLevel (0=small,1=medium,2=full)
        sprite->setTextureRect(sf::IntRect(
            sf::Vector2i(snowLevel * 80, 0),
            sf::Vector2i(80, 80)
        ));
        sprite->setPosition({ (float)x, (float)y });
        sprite->setScale({ 0.3f, 0.3f });
        window.draw(*sprite);
    }
};