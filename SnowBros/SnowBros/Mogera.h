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

    // ── Death animation state ──────────────────────────────────
    bool  dying = false;
    float dyingTimer = 0.f;
    float flashTimer = 0.f;
    bool  flashVisible = true;
    static constexpr float DYING_DURATION = 3.0f;   // total death anim length
    static constexpr float FLASH_INTERVAL = 0.12f;  // how fast it flashes red

private:

    optional<sf::Sprite> legSprite;
    optional<sf::Sprite> bodySprite;  // used for death body frames

    sf::IntRect legs{ {1935, 435}, {528, 357} };

    static constexpr float BOSS_W = 280.f;
    static constexpr float BOSS_H = 200.f;
    static constexpr float LEGS_W = 200.f;
    static constexpr float LEGS_H = 100.f;

    // Full-body death frames (row 2)
    static constexpr float BODY_W = 300.f;   // display size for full body
    static constexpr float BODY_H = 220.f;

    sf::IntRect body_stand{ { 171, 498}, {729, 645} };  // standing — used first
    sf::IntRect body_lying{ { 957, 717}, {840, 435} };  // collapsed — used after

    // Main head/body frames (top row)
    sf::IntRect head_frame1{ {   0,   3}, {576, 504} };
    sf::IntRect head_frame2{ { 576,   3}, {573, 495} };
    sf::IntRect head_frame3{ {1155,   3}, {585, 483} };

    // Tail/legs pieces (right side)
    sf::IntRect tail_top{ {1920,   0}, {543, 192} };
    sf::IntRect tail_mid{ {1950, 180}, {519, 261} };
    sf::IntRect tail_bot{ {1935, 435}, {528, 357} };
    // Display size on screen - big and imposing like original

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
        sprite->setTextureRect(head_frame1);
        sprite->setScale({ BOSS_W / 576.f, BOSS_H / 504.f });

        legSprite.emplace(texture);
        legSprite->setTextureRect(tail_top);
        legSprite->setScale({ LEGS_W / 543.f, LEGS_H / 192.f });

        bodySprite.emplace(texture);
        bodySprite->setTextureRect(body_stand);
        bodySprite->setScale({ BODY_W / 729.f, BODY_H / 645.f });

        textureLoaded = true;
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
        // ── Dying animation (plays after health hits 0) ───────────
        if (dying) {
            dyingTimer += static_cast<float>(deltaTime);
            flashTimer += static_cast<float>(deltaTime);
            if (flashTimer >= FLASH_INTERVAL) {
                flashTimer = 0.f;
                flashVisible = !flashVisible;
            }
            if (dyingTimer >= DYING_DURATION) {
                alive = false;   // NOW actually dead — triggers levelComplete
                dying = false;
            }
            // still update children so they finish running off-screen
            for (int i = 0; i < MAX_CHILDREN; i++)
                children[i].update(deltaTime);
            return;
        }

        if (!alive) return;

        // update children
        for (int i = 0; i < MAX_CHILDREN; i++)
            children[i].update(deltaTime);

        // spawn a child every SPAWN_INTERVAL seconds
        spawnTimer += deltaTime;
        if (spawnTimer >= SPAWN_INTERVAL) {
            spawnTimer = 0.0;
            for (int i = 0; i < MAX_CHILDREN; i++) {
                if (!children[i].alive) {
                    double dir = (rand() % 2) ? 1.0 : -1.0;
                    children[i].spawn(
                        x + BOSS_W * 0.3,
                        y + BOSS_H - MogeraChild::H,
                        dir
                    );
                    break;
                }
            }
        }

        // animate head — cycle through 3 frames
        animTimer += static_cast<float>(deltaTime);
        if (animTimer >= FRAME_DURATION) {
            animTimer = 0.f;
            animFrame = (animFrame + 1) % 3;
        }

        if (!textureLoaded || !sprite || !legSprite) return;

        sf::IntRect headFrames[3] = { head_frame1, head_frame2, head_frame3 };
        sf::IntRect legFrames[3] = { tail_top, tail_mid, tail_bot };

        sprite->setTextureRect(headFrames[animFrame]);
        sprite->setScale({
            BOSS_W / static_cast<float>(headFrames[animFrame].size.x),
            BOSS_H / 504.f
            });

        legSprite->setTextureRect(legFrames[animFrame]);
        legSprite->setScale({
            LEGS_W / static_cast<float>(legFrames[animFrame].size.x),
            LEGS_H / static_cast<float>(legFrames[animFrame].size.y)
            });
    }

    // Call this instead of setalive(false) when health runs out
    void startDying() {
        if (dying || !alive) return;
        dying = true;
        dyingTimer = 0.f;
        flashTimer = 0.f;
        flashVisible = true;
        // kill all children immediately so they don't keep spawning
        for (int i = 0; i < MAX_CHILDREN; i++)
            children[i].alive = false;
    }

    bool isDying() const { return dying; }

    void draw(sf::RenderWindow& window) override {
        // Draw children regardless (they run off during death anim)
        for (int i = 0; i < MAX_CHILDREN; i++)
            children[i].draw(window);

        // ── Dying animation draw ───────────────────────────────
        if (dying && textureLoaded && bodySprite) {
            // First half: body_stand staggering, second half: body_lying collapsed
            if (dyingTimer < DYING_DURATION * 0.5f)
                bodySprite->setTextureRect(body_stand);
            else
                bodySprite->setTextureRect(body_lying);

            sf::IntRect cur = bodySprite->getTextureRect();
            bodySprite->setScale({
                BODY_W / static_cast<float>(cur.size.x),
                BODY_H / static_cast<float>(cur.size.y)
                });

            bodySprite->setPosition({ (float)x, (float)y + BOSS_H - BODY_H });

            // Flash: alternate between red tint and white tint
            if (flashVisible)
                bodySprite->setColor(sf::Color(255, 80, 80, 255));
            else
                bodySprite->setColor(sf::Color(255, 255, 255, 255));

            window.draw(*bodySprite);
            bodySprite->setColor(sf::Color::White);  // reset
            return;
        }

        if (!alive) return;

        if (textureLoaded && sprite && legSprite) {
            sprite->setPosition({ (float)x, (float)y });
            window.draw(*sprite);
            legSprite->setPosition({ (float)x + 20.f, (float)y + BOSS_H - 50.f });
            window.draw(*legSprite);
        }

        // health bar — hide once dying starts
        float barX = 400.f - 150.f;
        float barY = 55.f;
        healthBarBg.setPosition({ barX, barY });
        window.draw(healthBarBg);
        float pct = (float)gethealth() / 20.f;
        healthBar.setSize({ 300.f * pct, 20.f });
        healthBar.setPosition({ barX, barY });
        window.draw(healthBar);
    }
};