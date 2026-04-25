#pragma once
#include "Enemy.h"
#include <SFML/Graphics.hpp>
using namespace std;

// MogeraChild — small enemy spawned by Mogera
struct MogeraChild {
    double x, y, vx;
    bool alive;
    sf::RectangleShape shape;
    MogeraChild() : x(0), y(0), vx(80), alive(false) {
        shape.setSize({ 28.f,28.f });
        shape.setFillColor(sf::Color(150, 60, 60));
    }
    void spawn(double sx, double sy, double dir) {
        x = sx; y = sy; vx = dir * 80; alive = true;
    }
    void update(double dt) {
        if (!alive) return;
        x += vx * dt;
        y += 900.0 * dt * 0.01; // slight gravity
        if (x < 0 || x > 800) alive = false;
        shape.setPosition({ (float)x,(float)y });
    }
    void draw(sf::RenderWindow& w) { if (alive) w.draw(shape); }
    sf::FloatRect getHitbox() const {
        return sf::FloatRect(sf::Vector2f((float)x, (float)y), sf::Vector2f(28.f, 28.f));
    }
};

class Mogera : public Enemy {
private:
    sf::Texture          texture;
    optional<sf::Sprite> sprite;
    bool        textureLoaded;
    float       animTimer;
    int         currentFrame;
    double      spawnTimer;

    // Mogera.png: 2470x1260, 3 idle frames in top row ~190px each, ~350px tall
    static const int IDLE_Y = 0, IDLE_H = 350, IDLE_FW = 600, IDLE_FRAMES = 3;
    static const float FRAME_TIME;
    static const double SPAWN_INTERVAL;

    void updateAnim(float dt) {
        animTimer += dt;
        if (animTimer >= FRAME_TIME) {
            animTimer = 0.f;
            currentFrame = (currentFrame + 1) % IDLE_FRAMES;
        }
        sprite->setTextureRect(sf::IntRect(
            sf::Vector2i(currentFrame * IDLE_FW, IDLE_Y),
            sf::Vector2i(IDLE_FW, IDLE_H)
        ));
        sprite->setScale({ 0.35f, 0.35f });
    }

public:
    static const int MAX_CHILDREN = 8;
    MogeraChild children[MAX_CHILDREN];
    sf::RectangleShape healthBarBg, healthBar;

    Mogera(double x, double y)
        : Enemy(x, y, 20, 0.0),
        textureLoaded(false),
        animTimer(0.f), currentFrame(0),
        spawnTimer(0.0)
    {
        healthBarBg.setSize({ 200.f, 16.f });
        healthBarBg.setFillColor(sf::Color(80, 0, 0));
        healthBar.setSize({ 200.f, 16.f });
        healthBar.setFillColor(sf::Color(220, 30, 30));
    }

    bool loadTexture(const string& path) {
        if (!texture.loadFromFile(path)) return false;
        sprite.emplace(texture);
        textureLoaded = true;
        return true;
    }

    sf::FloatRect getHitbox() const {
        return sf::FloatRect(sf::Vector2f((float)x, (float)y), sf::Vector2f(210.f, 120.f));
    }

    void update(double deltaTime) override {
        if (!alive) return;

        for (int i = 0; i < MAX_CHILDREN; i++)
            children[i].update(deltaTime);

        spawnTimer += deltaTime;
        if (spawnTimer >= SPAWN_INTERVAL) {
            spawnTimer = 0;
            for (int i = 0; i < MAX_CHILDREN; i++) {
                if (!children[i].alive) {
                    double dir = (rand() % 2) ? 1.0 : -1.0;
                    children[i].spawn(x + 100, y + 80, dir);
                    break;
                }
            }
        }
        updateAnim((float)deltaTime);
    }

    void draw(sf::RenderWindow& window) override {
        if (!alive) return;

        for (int i = 0; i < MAX_CHILDREN; i++) children[i].draw(window);

        if (!textureLoaded) {
            sf::RectangleShape r(sf::Vector2f(210.f, 120.f));
            r.setPosition({ (float)x,(float)y });
            r.setFillColor(sf::Color(80, 160, 80));
            window.draw(r);
        }
        else {
            sprite->setPosition({ (float)x,(float)y });
            window.draw(*sprite);
        }

        // Health bar
        float barX = (float)x - 20.f;
        float barY = (float)y - 24.f;
        healthBarBg.setPosition({ barX, barY });
        window.draw(healthBarBg);
        float pct = (float)gethealth() / 20.f;
        healthBar.setSize({ 200.f * pct, 16.f });
        healthBar.setPosition({ barX, barY });
        window.draw(healthBar);
    }
};

