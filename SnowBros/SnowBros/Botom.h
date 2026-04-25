#pragma once
#include "Enemy.h"
#include "Platform.h"
#include <SFML/Graphics.hpp>
#include <optional>
using namespace std;

class Botom : public Enemy {
private:
    sf::Texture          texture;
    optional<sf::Sprite> sprite;
    bool        textureLoaded;
    bool        facingRight;
    float       animTimer;
    int         currentFrame;
    int         snowHits;
    float       escapeTimer;

    static const float   FRAME_TIME;
    static const double  WALK_SPEED;

    static const int WALK_Y = 228, WALK_H = 175, WALK_FW = 140, WALK_FRAMES = 2;
    static const int TRAP_Y = 803, TRAP_H = 108, TRAP_FW = 160;

    void updateAnim(float dt) {
        if (!textureLoaded) return;
        animTimer += dt;
        if (animTimer >= FRAME_TIME) {
            animTimer = 0.f;
            currentFrame = (currentFrame + 1) % WALK_FRAMES;
        }
        if (!trap) {
            sprite->setTextureRect(sf::IntRect(
                sf::Vector2i(currentFrame * WALK_FW, WALK_Y),
                sf::Vector2i(WALK_FW, WALK_H)));
        }
        else {
            int tf = min(snowHits - 1, 1);
            sprite->setTextureRect(sf::IntRect(
                sf::Vector2i(tf * TRAP_FW, TRAP_Y),
                sf::Vector2i(TRAP_FW, TRAP_H)));
        }
        float sc = 0.5f;
        sprite->setScale({ facingRight ? sc : -sc, sc });
        if (!facingRight) sprite->setOrigin({ (float)WALK_FW, 0.f });
        else              sprite->setOrigin({ 0.f, 0.f });
    }

public:
    bool fullyTrapped;
    bool rolling;
    double rollDir;

    Botom(double x, double y)
        : Enemy(x, y, 2, WALK_SPEED),
        textureLoaded(false), facingRight(true),
        animTimer(0.f), currentFrame(0),
        snowHits(0), escapeTimer(0.f),
        fullyTrapped(false), rolling(false), rollDir(1.0)
    {
        setxspeed(WALK_SPEED);
    }

    bool loadTexture(const string& path) {
        if (!texture.loadFromFile(path)) return false;
        sprite.emplace(texture);
        textureLoaded = true;
        return true;
    }

    double getx() { return x; }

    void hitBySnow() {
        snowHits++;
        if (snowHits >= 2) {
            fullyTrapped = true;
            settrap(true);
            setxspeed(0);
            escapeTimer = 5.f;
        }
        else {
            setxspeed(xspeed * 0.5);
        }
    }

    void startRolling(double dir) {
        rolling = true;
        rollDir = dir;
        setxspeed(dir * 300.0);
    }

    sf::FloatRect getHitbox() const {
        return sf::FloatRect(sf::Vector2f((float)x + 4, (float)y + 4), sf::Vector2f(44.f, 44.f));
    }

    void resolvePlatforms(Platform platforms[], int count) {
        bool onGround = false;
        for (int i = 0; i < count; i++) {
            sf::FloatRect& p = platforms[i].rect;
            if (yspeed >= 0 &&
                (float)x + 44 > p.position.x && (float)x < p.position.x + p.size.x &&
                (float)y + 44 >= p.position.y && (float)y + 44 <= p.position.y + p.size.y + 8.f) {
                y = p.position.y - 44;
                yspeed = 0;
                onGround = true;
            }
        }
        if (!onGround) yspeed += 900.0 * 0.033;
    }

    void update(double deltaTime) override {
        if (!alive) return;

        if (fullyTrapped && !rolling) {
            escapeTimer -= (float)deltaTime;
            if (escapeTimer <= 0) {
                fullyTrapped = false;
                settrap(false);
                snowHits = 0;
                setxspeed(facingRight ? WALK_SPEED : -WALK_SPEED);
            }
            updateAnim((float)deltaTime);
            return;
        }

        yspeed += 900.0 * deltaTime;
        x += xspeed * deltaTime;
        y += yspeed * deltaTime;

        if (x > 800) x = -40;
        if (x < -40) x = 800;

        if (!rolling) {
            if (x <= 0) { setxspeed(WALK_SPEED);  facingRight = true; }
            if (x >= 756) { setxspeed(-WALK_SPEED); facingRight = false; }
        }

        updateAnim((float)deltaTime);
    }

    void draw(sf::RenderWindow& window) override {
        if (!alive) return;
        if (!textureLoaded) {
            sf::RectangleShape r(sf::Vector2f(48.f, 48.f));
            r.setPosition({ (float)x,(float)y });
            r.setFillColor(sf::Color(200, 100, 50));
            window.draw(r);
            return;
        }
        sprite->setPosition({ (float)x,(float)y });
        window.draw(*sprite);
    }
};

