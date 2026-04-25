#pragma once
#include "Enemy.h"
#include <SFML/Graphics.hpp>
using namespace std;

// Knife projectile fired by Tornado
struct Knife {
    double x, y, vx, vy;
    bool alive;
    sf::RectangleShape shape;
    Knife() : x(0), y(0), vx(0), vy(0), alive(false) {
        shape.setSize({ 12.f, 4.f });
        shape.setFillColor(sf::Color(200, 200, 80));
    }
    void launch(double sx, double sy, double tx, double ty) {
        x = sx; y = sy; alive = true;
        double dx = tx - sx, dy = ty - sy;
        double len = sqrt(dx * dx + dy * dy);
        if (len > 0) { vx = dx / len * 300; vy = dy / len * 300; }
    }
    void update(double dt) {
        if (!alive) return;
        x += vx * dt; y += vy * dt;
        if (x < 0 || x > 800 || y < 0 || y > 600) alive = false;
        shape.setPosition({ (float)x, (float)y });
    }
    void draw(sf::RenderWindow& w) { if (alive) w.draw(shape); }
    sf::FloatRect getHitbox() const {
        return sf::FloatRect(sf::Vector2f((float)x, (float)y), sf::Vector2f(12.f, 4.f));
    }
};

class Tornado : public Enemy {
private:
    sf::Texture          texture;
    optional<sf::Sprite> sprite;
    bool        textureLoaded;
    float       animTimer;
    int         currentFrame;
    double      knifeTimer;
    int         snowHits;
    bool        fullyTrapped;
    double      escapeTimer;
    double      targetX, targetY; // last known player pos

    static const double WALK_SPEED;
    static const double KNIFE_INTERVAL;
    static const float  FRAME_TIME;
    static const int    WALK_Y = 0, WALK_H = 170, WALK_FW = 130, WALK_FRAMES = 2;
    static const int    THROW_Y = 500, THROW_H = 170, THROW_FW = 130, THROW_FRAMES = 2;
    static const int    TRAP_Y = 340, TRAP_H = 160, TRAP_FW = 130, TRAP_FRAMES = 2;

    void updateAnim(float dt) {
        animTimer += dt;
        if (animTimer >= FRAME_TIME) {
            animTimer = 0.f;
            currentFrame = (currentFrame + 1) % WALK_FRAMES;
        }
        int ay = fullyTrapped ? TRAP_Y : WALK_Y;
        int ah = fullyTrapped ? TRAP_H : WALK_H;
        int afw = fullyTrapped ? TRAP_FW : WALK_FW;
        sprite->setTextureRect(sf::IntRect(sf::Vector2i(currentFrame * afw, ay), sf::Vector2i(afw, ah)));
        sprite->setScale({ 0.45f, 0.45f });
    }

public:
    static const int MAX_KNIVES = 5;
    Knife knives[MAX_KNIVES];
    bool  firedKnife; // flag for Game to detect
    bool  fullyTrappedPublic;

    Tornado(double x, double y)
        : Enemy(x, y, 3, WALK_SPEED),
        textureLoaded(false),
        animTimer(0.f), currentFrame(0),
        knifeTimer(0.0), snowHits(0),
        fullyTrapped(false), escapeTimer(0.0),
        targetX(400), targetY(300),
        firedKnife(false), fullyTrappedPublic(false)
    {
        xspeed = WALK_SPEED;
    }

    bool loadTexture(const string& path) {
        if (!texture.loadFromFile(path)) return false;
        sprite->setTexture(texture);
        textureLoaded = true;
        return true;
    }

    void setPlayerPos(double px, double py) { targetX = px; targetY = py; }

    void receiveSnowballHit() {
        snowHits++;
        if (snowHits >= 2) {
            fullyTrapped = true; fullyTrappedPublic = true;
            settrap(true); xspeed = 0; yspeed = 0;
            escapeTimer = 5.0;
        }
    }

    sf::FloatRect getHitbox() const {
        return sf::FloatRect(sf::Vector2f((float)x + 4, (float)y + 4), sf::Vector2f(48.f, 48.f));
    }

    void update(double deltaTime) override {
        if (!alive) return;

        for (int i = 0; i < MAX_KNIVES; i++)
            knives[i].update(deltaTime);

        if (fullyTrapped) {
            escapeTimer -= deltaTime;
            if (escapeTimer <= 0) {
                fullyTrapped = false; fullyTrappedPublic = false;
                settrap(false); snowHits = 0; xspeed = WALK_SPEED;
            }
            updateAnim((float)deltaTime);
            return;
        }

        yspeed += 900.0 * deltaTime;
        x += xspeed * deltaTime;
        y += yspeed * deltaTime;
        if (x <= 0 || x >= 756) xspeed *= -1;

        // Throw knife at player
        knifeTimer += deltaTime;
        if (knifeTimer >= KNIFE_INTERVAL) {
            knifeTimer = 0;
            for (int i = 0; i < MAX_KNIVES; i++) {
                if (!knives[i].alive) {
                    knives[i].launch(x + 24, y + 24, targetX, targetY);
                    firedKnife = true;
                    break;
                }
            }
        }
        updateAnim((float)deltaTime);
    }

    void draw(sf::RenderWindow& window) override {
        if (!alive) return;
        for (int i = 0; i < MAX_KNIVES; i++) knives[i].draw(window);
        if (!textureLoaded) {
            sf::RectangleShape r(sf::Vector2f(52.f, 52.f));
            r.setPosition({ (float)x,(float)y });
            r.setFillColor(sf::Color(100, 50, 200));
            window.draw(r);
            return;
        }
        sprite->setPosition({ (float)x,(float)y });
        window.draw(*sprite);
    }
};

