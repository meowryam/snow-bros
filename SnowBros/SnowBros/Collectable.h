#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"

// ============================================================
//  Collectable  �  abstract base for all pickups
//  Spec section 8: power-ups + gems drop from defeated enemies
// ============================================================
class Collectable {
protected:
    double x, y;
    bool   alive;

    // Gentle bob animation
    float  bobTimer = 0.f;
    float  bobOffset = 0.f;

    // Auto-despawn after a while so the screen doesn't fill up
    float  lifetime;
    static constexpr float DEFAULT_LIFETIME = 8.f;   // seconds

    sf::RectangleShape shape;   // fallback if no texture

public:
    Collectable(double spawnX, double spawnY, float life = DEFAULT_LIFETIME)
        : x(spawnX), y(spawnY), alive(true), lifetime(life)
    {
    }

    virtual ~Collectable() = default;

    // ── core interface ─────────────────────────────────────
    // Returns true if the player overlaps this pickup
    bool checkCollect(Player& player) {
        if (!alive) return false;
        sf::FloatRect myBox(sf::Vector2f((float)x, (float)y), getSize());
        if (myBox.findIntersection(player.getHitbox())) {
            onCollect(player);
            alive = false;
            return true;
        }
        return false;
    }

    virtual void update(double deltaTime) {
        if (!alive) return;

        // bob up/down
        bobTimer += (float)deltaTime;
        bobOffset = std::sin(bobTimer * 4.f) * 4.f;

        // auto-despawn
        lifetime -= (float)deltaTime;
        if (lifetime <= 0.f) alive = false;
    }

    virtual void draw(sf::RenderWindow& window) {
        if (!alive) return;
        shape.setPosition(sf::Vector2f((float)x, (float)y + bobOffset));
        window.draw(shape);
    }

    bool   isAlive() const { return alive; }
    double getX()    const { return x; }
    double getY()    const { return y; }

protected:
    // Subclasses implement: what size is the hitbox?
    virtual sf::Vector2f getSize() const = 0;
    // Subclasses implement: what happens when collected?
    virtual void onCollect(Player& player) = 0;
};


// ============================================================
//  GemCollectable  �  drops 1 gem, worth 1-5 gems via GemSystem
//  Kept simple: just adds directly to player data via callback.
//  Caller (GameLevel) handles gem accounting through GemSystem.
// ============================================================
class GemCollectable : public Collectable {
public:
    GemCollectable(double spawnX, double spawnY)
        : Collectable(spawnX, spawnY, 6.f)
    {
        shape.setSize({ 14.f, 14.f });
        shape.setFillColor(sf::Color(80, 200, 255));   // bright cyan gem
        shape.setOutlineColor(sf::Color::White);
        shape.setOutlineThickness(1.f);
    }

protected:
    sf::Vector2f getSize() const override { return { 14.f, 14.f }; }
    void onCollect(Player&) override {}  // GameLevel calls gemSystem.enemykilled()
};


// ============================================================
//  SpeedBoostPickup  �  spec 8.2: +50% speed for 15 seconds
// ============================================================
class SpeedBoostPickup : public Collectable {
public:
    SpeedBoostPickup(double spawnX, double spawnY)
        : Collectable(spawnX, spawnY)
    {
        shape.setSize({ 18.f, 18.f });
        shape.setFillColor(sf::Color(255, 165, 0));   // orange shoe colour
        shape.setOutlineColor(sf::Color::Yellow);
        shape.setOutlineThickness(1.f);
    }

protected:
    sf::Vector2f getSize() const override { return { 18.f, 18.f }; }
    void onCollect(Player& player) override {
        player.activateSpeedBoost(15.f);   // spec: 15 seconds
    }
};


// ============================================================
//  SnowballPowerPickup  �  spec 8.2: 1-hit encase, until level end
// ============================================================
class SnowballPowerPickup : public Collectable {
public:
    SnowballPowerPickup(double spawnX, double spawnY)
        : Collectable(spawnX, spawnY)
    {
        shape.setSize({ 18.f, 18.f });
        shape.setFillColor(sf::Color(200, 230, 255));  // icy white-blue
        shape.setOutlineColor(sf::Color(100, 180, 255));
        shape.setOutlineThickness(1.f);
    }

protected:
    sf::Vector2f getSize() const override { return { 18.f, 18.f }; }
    void onCollect(Player& player) override {
        player.activateSnowballPower();    // flag stays until resetForNewLevel
    }
};


// ============================================================
//  DistanceIncreasePickup  �  spec 8.2: full-width throw, until level end
// ============================================================
class DistanceIncreasePickup : public Collectable {
public:
    DistanceIncreasePickup(double spawnX, double spawnY)
        : Collectable(spawnX, spawnY)
    {
        shape.setSize({ 18.f, 18.f });
        shape.setFillColor(sf::Color(150, 255, 150));  // green arrow
        shape.setOutlineColor(sf::Color(50, 200, 50));
        shape.setOutlineThickness(1.f);
    }

protected:
    sf::Vector2f getSize() const override { return { 18.f, 18.f }; }
    void onCollect(Player& player) override {
        player.activateDistanceIncrease();
    }
};


// ============================================================
//  BalloonModePickup  �  spec 8.2: float upward for 10 seconds
// ============================================================
class BalloonModePickup : public Collectable {
public:
    BalloonModePickup(double spawnX, double spawnY)
        : Collectable(spawnX, spawnY)
    {
        shape.setSize({ 18.f, 18.f });
        shape.setFillColor(sf::Color(255, 100, 200));  // pink balloon
        shape.setOutlineColor(sf::Color(200, 50, 150));
        shape.setOutlineThickness(1.f);
    }

protected:
    sf::Vector2f getSize() const override { return { 18.f, 18.f }; }
    void onCollect(Player& player) override {
        player.activateBalloonMode(10.f);  // spec: 10 seconds
    }
};


// ============================================================
//  ExtraLifePickup  �  spec 8.2 / 7.3: +1 life, no cap enforced here
// ============================================================
class ExtraLifePickup : public Collectable {
public:
    ExtraLifePickup(double spawnX, double spawnY)
        : Collectable(spawnX, spawnY)
    {
        shape.setSize({ 18.f, 18.f });
        shape.setFillColor(sf::Color(255, 80, 80));    // red heart
        shape.setOutlineColor(sf::Color(200, 20, 20));
        shape.setOutlineThickness(1.f);
    }

protected:
    sf::Vector2f getSize() const override { return { 18.f, 18.f }; }
    void onCollect(Player& player) override {
        player.collectLife();
    }
};


// ============================================================
//  BonusCashBundle  �  spec 9.2: bonus level rain, +1000pts +10 gems each
//  These fall from the top; GameLevel calls scoreSystem.onBonusBundleCollected()
// ============================================================
class BonusCashBundle : public Collectable {
private:
    double fallSpeed = 180.0;

public:
    BonusCashBundle(double spawnX)
        : Collectable(spawnX, -20.0, 12.f)   // start above screen
    {
        shape.setSize({ 20.f, 16.f });
        shape.setFillColor(sf::Color(255, 215, 0));   // gold
        shape.setOutlineColor(sf::Color(200, 160, 0));
        shape.setOutlineThickness(1.f);
    }

    void update(double deltaTime) override {
        if (!alive) return;
        y += fallSpeed * deltaTime;
        if (y > 640.0) alive = false;   // fell off screen uncollected

        // bob timer still runs (shared base)
        lifetime -= (float)deltaTime;
        if (lifetime <= 0.f) alive = false;
    }

    void draw(sf::RenderWindow& window) override {
        if (!alive) return;
        shape.setPosition(sf::Vector2f((float)x, (float)y));
        window.draw(shape);
    }

protected:
    sf::Vector2f getSize() const override { return { 20.f, 16.f }; }
    void onCollect(Player&) override {}   // GameLevel calls scoreSystem.onBonusBundleCollected()
};