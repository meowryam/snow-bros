#pragma once
#include "Enemy.h"
#include <SFML/Graphics.hpp>
#include <optional>
#include <string>
using namespace std;

// ?? Child enemy spawned by Mogera ????????????????????????????????
struct MogeraChild {
    double x, y, vx;
    bool alive;
    sf::Texture texture;
    optional<sf::Sprite> sprite;
    bool textureLoaded = false;

    // child sprite frames from Mogera.png
    sf::IntRect child_red{ {1797, 798}, {174, 186} };
    sf::IntRect child_pink{ {1965, 801}, {186, 186} };

    static constexpr float W = 36.f;
    static constexpr float H = 44.f;

    MogeraChild() : x(0), y(0), vx(80), alive(false) {}

    void spawn(double sx, double sy, double dir) {
        x = sx; y = sy;
        vx = dir * 100.0;
        alive = true;
    }

    void update(double dt) {
        if (!alive) return;
        x += vx * dt;
        if (x < -50 || x > 850) alive = false;
    }

    void draw(sf::RenderWindow& w) {
        if (!alive) return;
        sf::RectangleShape rect(sf::Vector2f(W, H));
        rect.setPosition({ (float)x, (float)y });
        rect.setFillColor(sf::Color(200, 60, 60));
        w.draw(rect);
    }

    sf::FloatRect getHitbox() const {
        return sf::FloatRect(sf::Vector2f((float)x, (float)y),
            sf::Vector2f(W, H));
    }
};

// ?? Mogera Boss ???????????????????????????????????????????????????
class Mogera : public Enemy {
private:
    sf::Texture          texture;
    optional<sf::Sprite> sprite;
    bool        textureLoaded = false;

    // Animation
    float animTimer = 0.f;
    int   animFrame = 0;
    static constexpr float FRAME_DURATION = 0.3f;

    // Spawn timer
    double spawnTimer = 0.0;
    static constexpr double SPAWN_INTERVAL = 2.5;

    // Sprite frames from Mogera.png
    sf::IntRect head_frame1{ {   0,   3}, {576, 504} };
    sf::IntRect head_frame2{ { 576,   3}, {573, 495} };
    sf::IntRect head_frame3{ {1155,   3}, {585, 483} };

    // Hitbox size on screen
    static constexpr float BOSS_W = 210.f;
    static constexpr float BOSS_H = 175.f;

    // Health bar shapes
    sf::RectangleShape healthBarBg;
    sf::RectangleShape healthBar;

public:
    static const int MAX_CHILDREN = 8;
    MogeraChild children[MAX_CHILDREN];

    Mogera(double startX, double startY)
        : Enemy(startX, startY, 20, 0.0)
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

        // Scale first frame to boss display size
        sf::Vector2i sz = head_frame1.size;
        sprite->setScale({
            BOSS_W / static_cast<float>(sz.x),
            BOSS_H / static_cast<float>(sz.y)
            });
        sprite->setTextureRect(head_frame1);
        return true;
    }

    sf::FloatRect getHitbox() const {
        return sf::FloatRect(
            sf::Vector2f((float)x, (float)y),
            sf::Vector2f(BOSS_W, BOSS_H)
        );
    }

    void update(double deltaTime) override {
        if (!alive) return;

        // Update children
        for (int i = 0; i < MAX_CHILDREN; i++)
            children[i].update(deltaTime);

        // Spawn a child every SPAWN_INTERVAL seconds
        spawnTimer += deltaTime;
        if (spawnTimer >= SPAWN_INTERVAL) {
            spawnTimer = 0.0;
            for (int i = 0; i < MAX_CHILDREN; i++) {
                if (!children[i].alive) {
                    double dir = (rand() % 2) ? 1.0 : -1.0;
                    children[i].spawn(x + BOSS_W / 2, y + BOSS_H - 44, dir);
                    break;
                }
            }
        }

        // Animate head frames
        animTimer += static_cast<float>(deltaTime);
        if (animTimer >= FRAME_DURATION) {
            animTimer = 0.f;
            animFrame = (animFrame + 1) % 3;
        }

        if (!textureLoaded || !sprite) return;

        sf::IntRect frames[3] = { head_frame1, head_frame2, head_frame3 };
        sprite->setTextureRect(frames[animFrame]);
    }

    void draw(sf::RenderWindow& window) override {
        if (!alive) return;

        // Draw children
        for (int i = 0; i < MAX_CHILDREN; i++)
            children[i].draw(window);

        // Draw boss
        if (textureLoaded && sprite) {
            sprite->setPosition({ (float)x, (float)y });
            window.draw(*sprite);
        }
        else {
            sf::RectangleShape r(sf::Vector2f(BOSS_W, BOSS_H));
            r.setPosition({ (float)x, (float)y });
            r.setFillColor(sf::Color(80, 160, 80));
            window.draw(r);
        }

        // Draw health bar above boss
        float barX = (float)x;
        float barY = (float)y - 24.f;
        healthBarBg.setPosition({ barX, barY });
        window.draw(healthBarBg);

        float pct = (float)gethealth() / 20.f;
        healthBar.setSize({ 200.f * pct, 16.f });
        healthBar.setPosition({ barX, barY });
        window.draw(healthBar);
    }
};