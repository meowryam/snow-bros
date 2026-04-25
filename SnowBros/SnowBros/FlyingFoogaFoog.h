#pragma once
#include "Enemy.h"
#include <SFML/Graphics.hpp>
using namespace std;

class FlyingFoogaFoog : public Enemy {
private:
    sf::Texture          texture;
    optional<sf::Sprite> sprite;
    bool        textureLoaded;

    bool   isFlying;
    double flyTimer;
    double flyDirX, flyDirY;
    int    snowHits;
    bool   fullyTrapped;
    double escapeTimer;
    float  animTimer;
    int    currentFrame;

    static const double GROUND_SPEED;
    static const double FLY_SPEED;
    static const double FLY_INTERVAL;
    static const double FLY_DURATION;
    static const float  FRAME_TIME;

    // FlyingFoogaFoog_Blue.png: ~3 frames per row, rows ~160px tall, frames ~120px wide
    static const int WALK_Y = 0,   WALK_H = 160,  WALK_FW = 120, WALK_FRAMES = 3;
    static const int FLY_Y  = 160, FLY_H  = 160,  FLY_FW  = 120, FLY_FRAMES  = 3;
    static const int TRAP_Y = 480, TRAP_H = 140,  TRAP_FW = 120, TRAP_FRAMES = 2;

    void updateAnim(float dt) {
        animTimer += dt;
        if (animTimer >= FRAME_TIME) {
            animTimer = 0.f;
            int maxF = fullyTrapped ? TRAP_FRAMES : (isFlying ? FLY_FRAMES : WALK_FRAMES);
            currentFrame = (currentFrame + 1) % maxF;
        }
        int ay = fullyTrapped ? TRAP_Y : (isFlying ? FLY_Y : WALK_Y);
        int ah = fullyTrapped ? TRAP_H : (isFlying ? FLY_H  : WALK_H);
        int afw= fullyTrapped ? TRAP_FW: (isFlying ? FLY_FW : WALK_FW);
        sprite->setTextureRect(sf::IntRect(
            sf::Vector2i(currentFrame * afw, ay),
            sf::Vector2i(afw, ah)
        ));
        sprite->setScale({0.45f, 0.45f});
    }

public:
    bool fullyTrappedPublic;

    FlyingFoogaFoog(double x, double y)
        : Enemy(x, y, 2, GROUND_SPEED),
          textureLoaded(false),
          isFlying(false),
          flyTimer(0.0),
          flyDirX(1.0), flyDirY(0.0),
          snowHits(0),
          fullyTrapped(false),
          escapeTimer(0.0),
          animTimer(0.f),
          currentFrame(0),
          fullyTrappedPublic(false)
    {
        xspeed = GROUND_SPEED;
    }

    bool loadTexture(const string& path) {
        if (!texture.loadFromFile(path)) return false;
        sprite.emplace(texture);
        textureLoaded = true;
        return true;
    }

    void hitBySnow() {
        snowHits++;
        if (snowHits >= 2) {
            fullyTrapped = true;
            fullyTrappedPublic = true;
            settrap(true);
            xspeed = 0; yspeed = 0;
            escapeTimer = 5.0;
        }
    }

    sf::FloatRect getHitbox() const {
        return sf::FloatRect(sf::Vector2f((float)x+4,(float)y+4), sf::Vector2f(50.f,50.f));
    }

    void update(double deltaTime) override {
        if (!alive) return;

        if (fullyTrapped) {
            escapeTimer -= deltaTime;
            if (escapeTimer <= 0) {
                fullyTrapped = false; fullyTrappedPublic = false;
                settrap(false); snowHits = 0;
                xspeed = GROUND_SPEED;
            }
            updateAnim((float)deltaTime);
            return;
        }

        flyTimer += deltaTime;
        if (!isFlying && flyTimer >= FLY_INTERVAL) {
            isFlying = true; flyTimer = 0;
            flyDirX = (rand() % 3 - 1) * FLY_SPEED;
            flyDirY = (rand() % 3 - 1) * FLY_SPEED;
            if (flyDirX == 0 && flyDirY == 0) flyDirX = FLY_SPEED;
        }
        if (isFlying && flyTimer >= FLY_DURATION) {
            isFlying = false; flyTimer = 0;
            xspeed = GROUND_SPEED; yspeed = 0;
        }

        if (isFlying) {
            x += flyDirX * deltaTime;
            y += flyDirY * deltaTime;
            // bounce off screen edges
            if (x < 0 || x > 760) flyDirX *= -1;
            if (y < 0 || y > 540) flyDirY *= -1;
        } else {
            yspeed += 900.0 * deltaTime;
            x += xspeed * deltaTime;
            y += yspeed * deltaTime;
            if (x <= 0 || x >= 756) xspeed *= -1;
        }

        updateAnim((float)deltaTime);
    }

    void draw(sf::RenderWindow& window) override {
        if (!alive) return;
        if (!textureLoaded) {
            sf::RectangleShape r(sf::Vector2f(52.f, 52.f));
            r.setPosition({(float)x,(float)y});
            r.setFillColor(sf::Color(50,100,200));
            window.draw(r);
            return;
        }
        sprite->setPosition({(float)x, (float)y});
        window.draw(*sprite);
    }
};

