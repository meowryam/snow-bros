#pragma once
#include "Enemy.h"
#include <SFML/Graphics.hpp>
#include <optional>
#include <string>
using namespace std;

struct MogeraChild {
    double x, y, vx;
    bool alive;
    sf::Texture texture;
    optional<sf::Sprite> sprite;
    bool textureLoaded = false;

    // child uses the small creature frames from Mogera.png
    sf::IntRect child_frame1{ {1797, 798}, {174, 186} };
    sf::IntRect child_frame2{ {1965, 801}, {186, 186} };

    float animTimer = 0.f;
    int   animFrame = 0;

    static constexpr float W = 40.f;
    static constexpr float H = 48.f;

    MogeraChild() : x(0), y(0), vx(0), alive(false) {}

    bool loadTexture(const string& path) {
        if (!texture.loadFromFile(path)) return false;
        sprite.emplace(texture);
        sprite->setTextureRect(child_frame1);
        float sx = W / 174.f;
        float sy = H / 186.f;
        sprite->setScale({ sx, sy });
        textureLoaded = true;
        return true;
    }

    void spawn(double sx, double sy, double dir) {
        x = sx; y = sy;
        vx = dir * 100.0;
        alive = true;
        animFrame = 0;
        animTimer = 0.f;
    }

    void update(double dt) {
        if (!alive) return;
        x += vx * dt;
        if (x < -50 || x > 850) alive = false;

        animTimer += static_cast<float>(dt);
        if (animTimer >= 0.2f) {
            animTimer = 0.f;
            animFrame = (animFrame + 1) % 2;
        }
        if (textureLoaded && sprite) {
            sprite->setTextureRect(animFrame == 0 ? child_frame1 : child_frame2);
            // flip based on direction
            float sx = W / 174.f;
            float sy = H / 186.f;
            if (vx > 0) {
                sprite->setOrigin({ 174.f, 0.f });
                sprite->setScale({ -sx, sy });
            }
            else {
                sprite->setOrigin({ 0.f, 0.f });
                sprite->setScale({ sx, sy });
            }
        }
    }

    void draw(sf::RenderWindow& w) {
        if (!alive) return;
        if (textureLoaded && sprite) {
            sprite->setPosition({ (float)x, (float)y });
            w.draw(*sprite);
        }
        else {
            sf::RectangleShape rect(sf::Vector2f(W, H));
            rect.setPosition({ (float)x, (float)y });
            rect.setFillColor(sf::Color(180, 60, 60));
            w.draw(rect);
        }
    }

    sf::FloatRect getHitbox() const {
        return sf::FloatRect(sf::Vector2f((float)x, (float)y),
            sf::Vector2f(W, H));
    }
};

class Mogera : public Enemy {
private:
    sf::Texture          texture;
    optional<sf::Sprite> sprite;
    bool        textureLoaded = false;
    string      texturePath;  // save path so children can load same texture

    float animTimer = 0.f;
    int   animFrame = 0;
    static constexpr float FRAME_DURATION = 0.25f;

    double spawnTimer = 0.0;
    static constexpr double SPAWN_INTERVAL = 2.5;

    // Top row upright head frames
    sf::IntRect head_frame1{ {   0,   3}, {576, 504} };
    sf::IntRect head_frame2{ { 576,   3}, {573, 495} };
    sf::IntRect head_frame3{ {1155,   3}, {585, 483} };

    // Display size on screen - big and imposing like original

// TO THIS:
    static constexpr float BOSS_W = 300.f;
    static constexpr float BOSS_H = 280.f;

    sf::RectangleShape healthBarBg;
    sf::RectangleShape healthBar;

public:
    static const int MAX_CHILDREN = 8;
    MogeraChild children[MAX_CHILDREN];

    Mogera(double startX, double startY)
        : Enemy(startX, startY, 20, 0.0)
    {
        // health bar at top center of screen
        healthBarBg.setSize({ 300.f, 20.f });
        healthBarBg.setFillColor(sf::Color(60, 0, 0));
        healthBarBg.setOutlineThickness(2.f);
        healthBarBg.setOutlineColor(sf::Color::Red);

        healthBar.setSize({ 300.f, 20.f });
        healthBar.setFillColor(sf::Color(220, 30, 30));
    }

    bool loadTexture(const string& path) {
        texturePath = path;
        if (!texture.loadFromFile(path)) return false;
        sprite.emplace(texture);
        textureLoaded = true;
        sprite->setTextureRect(head_frame1);
        sprite->setScale({
            BOSS_W / 576.f,
            BOSS_H / 504.f
            });
        // load children textures too
        for (int i = 0; i < MAX_CHILDREN; i++)
            children[i].loadTexture(path);
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

        for (int i = 0; i < MAX_CHILDREN; i++)
            children[i].update(deltaTime);

        spawnTimer += deltaTime;
        if (spawnTimer >= SPAWN_INTERVAL) {
            spawnTimer = 0.0;
            for (int i = 0; i < MAX_CHILDREN; i++) {
                if (!children[i].alive) {
                    double dir = (rand() % 2) ? 1.0 : -1.0;
                    // spawn from base of mogera
                    children[i].spawn(
                        x + BOSS_W * 0.3,
                        y + BOSS_H - MogeraChild::H,
                        dir
                    );
                    break;
                }
            }
        }

        // cycle head frames
        animTimer += static_cast<float>(deltaTime);
        if (animTimer >= FRAME_DURATION) {
            animTimer = 0.f;
            animFrame = (animFrame + 1) % 3;
        }

        if (!textureLoaded || !sprite) return;
        sf::IntRect frames[3] = { head_frame1, head_frame2, head_frame3 };
        sprite->setTextureRect(frames[animFrame]);
        sprite->setScale({
            BOSS_W / static_cast<float>(frames[animFrame].size.x),
            BOSS_H / static_cast<float>(frames[animFrame].size.y)
            });
    }

    void draw(sf::RenderWindow& window) override {
        if (!alive) return;

        for (int i = 0; i < MAX_CHILDREN; i++)
            children[i].draw(window);

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

        // Health bar at top-center of screen
        float barX = 400.f - 150.f;  // centered on 800px screen
        float barY = 55.f;           // just below HUD
        healthBarBg.setPosition({ barX, barY });
        window.draw(healthBarBg);

        float pct = (float)gethealth() / 20.f;
        healthBar.setSize({ 300.f * pct, 20.f });
        healthBar.setPosition({ barX, barY });
        window.draw(healthBar);
    }
};