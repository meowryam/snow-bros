#pragma once
#include "Enemy.h"
#include "Mogera.h"
#include <SFML/Graphics.hpp>
using namespace std;

struct Rocket {
    double x, y, vx, vy;
    bool alive;
    sf::CircleShape shape;
    Rocket() : x(0), y(0), vx(0), vy(0), alive(false) {
        shape.setRadius(10.f);
        shape.setFillColor(sf::Color(255, 100, 0));
    }
    void launch(double sx, double sy, double tx, double ty) {
        x = sx; y = sy; alive = true;
        double dx = tx - sx, dy = ty - sy;
        double len = sqrt(dx * dx + dy * dy);
        if (len > 0) { vx = dx / len * 250; vy = dy / len * 250; }
    }
    void update(double dt) {
        if (!alive) return;
        x += vx * dt; y += vy * dt;
        if (x < 0 || x>800 || y < 0 || y>600) alive = false;
        shape.setPosition({ (float)x,(float)y });
    }
    void draw(sf::RenderWindow& w) { if (alive) w.draw(shape); }
    sf::FloatRect getHitbox() const {
        return sf::FloatRect(sf::Vector2f((float)x, (float)y), sf::Vector2f(20.f, 20.f));
    }
};

class Gamakichi : public Enemy {
private:
    sf::Texture          texture;
    optional<sf::Sprite> sprite;
    bool        textureLoaded;
    float       animTimer;
    int         currentFrame;
    double      rocketTimer;
    double      spawnTimer;
    int         phase; // 1=normal, 2=enraged (health < 25)

    // Gamakichi.png: 4060x1717 — 2 large idle frames ~1200px wide, ~750px tall
    static const int IDLE_Y = 0, IDLE_H = 750, IDLE_FW = 1200, IDLE_FRAMES = 2;
    static const float FRAME_TIME;
    static const double ROCKET_INTERVAL;
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
        sprite->setScale({ 0.30f, 0.30f });
    }

public:
    static const int MAX_ROCKETS = 8;
    static const int MAX_CHILDREN = 6;
    Rocket      rockets[MAX_ROCKETS];
    MogeraChild children[MAX_CHILDREN]; // reuse child type
    sf::RectangleShape healthBarBg, healthBar;
    double targetX, targetY;

    Gamakichi(double x, double y)
        : Enemy(x, y, 50, 0.0),
        textureLoaded(false),
        animTimer(0.f), currentFrame(0),
        rocketTimer(0.0), spawnTimer(0.0), phase(1),
        targetX(400), targetY(300)
    {
        healthBarBg.setSize({ 300.f,18.f });
        healthBarBg.setFillColor(sf::Color(80, 0, 0));
        healthBar.setSize({ 300.f,18.f });
        healthBar.setFillColor(sf::Color(255, 50, 0));
    }

    bool loadTexture(const string& path) {
        if (!texture.loadFromFile(path)) return false;
        sprite.emplace(texture);
        textureLoaded = true;
        return true;
    }

    void setPlayerPos(double px, double py) { targetX = px; targetY = py; }

    sf::FloatRect getHitbox() const {
        return sf::FloatRect(sf::Vector2f((float)x, (float)y), sf::Vector2f(360.f, 225.f));
    }

    void update(double deltaTime) override {
        if (!alive) return;

        // Phase 2 when health drops below half
        if (gethealth() < 25) phase = 2;

        for (int i = 0; i < MAX_ROCKETS; i++)  rockets[i].update(deltaTime);
        for (int i = 0; i < MAX_CHILDREN; i++) children[i].update(deltaTime);

        double interval = (phase == 2) ? ROCKET_INTERVAL * 0.5 : ROCKET_INTERVAL;
        rocketTimer += deltaTime;
        if (rocketTimer >= interval) {
            rocketTimer = 0;
            // Fire in 4 directions in phase 2, 1 toward player in phase 1
            int shots = (phase == 2) ? 4 : 1;
            int fired = 0;
            for (int i = 0; i < MAX_ROCKETS && fired < shots; i++) {
                if (!rockets[i].alive) {
                    if (fired == 0) rockets[i].launch(x + 180, y + 80, targetX, targetY);
                    else {
                        // extra directions
                        double dirs[3][2] = { {0,300},{300,0},{-300,0} };
                        rockets[i].x = x + 180; rockets[i].y = y + 80;
                        rockets[i].vx = dirs[fired - 1][0];
                        rockets[i].vy = dirs[fired - 1][1];
                        rockets[i].alive = true;
                    }
                    fired++;
                }
            }
        }

        spawnTimer += deltaTime;
        if (spawnTimer >= SPAWN_INTERVAL) {
            spawnTimer = 0;
            for (int i = 0; i < MAX_CHILDREN; i++) {
                if (!children[i].alive) {
                    children[i].spawn(x + 160, y + 120, (rand() % 2) ? 1.0 : -1.0);
                    break;
                }
            }
        }
        updateAnim((float)deltaTime);
    }

    void draw(sf::RenderWindow& window) override {
        if (!alive) return;
        for (int i = 0; i < MAX_ROCKETS; i++)  rockets[i].draw(window);
        for (int i = 0; i < MAX_CHILDREN; i++) children[i].draw(window);

        if (!textureLoaded) {
            sf::RectangleShape r(sf::Vector2f(360.f, 225.f));
            r.setPosition({ (float)x,(float)y });
            r.setFillColor(sf::Color(180, 50, 50));
            window.draw(r);
        }
        else {
            sprite->setPosition({ (float)x,(float)y });
            window.draw(*sprite);
        }

        // Health bar
        float bx = (float)x + 30.f, by = (float)y - 24.f;
        healthBarBg.setPosition({ bx,by });
        window.draw(healthBarBg);
        float pct = (float)gethealth() / 50.f;
        healthBar.setSize({ 300.f * pct,18.f });
        healthBar.setPosition({ bx,by });
        window.draw(healthBar);
    }
};

