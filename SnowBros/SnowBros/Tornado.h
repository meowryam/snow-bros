#pragma once
#include "FlyingFoogaFoog.h"

// ============================================================
//  Knife — a simple projectile thrown by Tornado
//
//  Kept as a struct (not a class) because:
//  - It has no complex behaviour — just moves in a straight line
//  - It only ever exists inside Tornado's fixed array
//  - GameLevel needs direct access to alive and getHitbox()
//    so keeping it simple and public makes sense
// ============================================================
struct Knife {

    double x, y;           // current position
    double xspeed, yspeed; // velocity — set once at throw time, never changes
    bool   alive;          // false = already hit something or left screen

    // Size of the knife hitbox (small — thin projectile)
    static constexpr float WIDTH = 16.f;
    static constexpr float HEIGHT = 8.f;

    // Default constructor — knife starts inactive
    Knife() : x(0), y(0), xspeed(0), yspeed(0), alive(false) {}

    // Move the knife by its velocity each frame.
    // No gravity — knives fly in a straight line as per spec.
    void update(double deltaTime) {
        if (!alive) return;
        x += xspeed * deltaTime;
        y += yspeed * deltaTime;
    }

    // Returns the rectangle used for collision checks in GameLevel
    sf::FloatRect getHitbox() const {
        return sf::FloatRect(
            sf::Vector2f(static_cast<float>(x), static_cast<float>(y)),
            sf::Vector2f(WIDTH, HEIGHT)
        );
    }

    // Draw the knife as a small yellow rectangle (placeholder)
    void draw(sf::RenderWindow& window) const {
        if (!alive) return;
        sf::RectangleShape rect(sf::Vector2f(WIDTH, HEIGHT));
        rect.setPosition(sf::Vector2f(static_cast<float>(x), static_cast<float>(y)));
        rect.setFillColor(sf::Color(255, 220, 50));  // yellow — visible against most backgrounds
        window.draw(rect);
    }
};

// ============================================================
//  Tornado — inherits FlyingFoogaFoog, adds:
//  1. Randomized flight speed (sometimes crawling, sometimes very fast)
//  2. Knife throwing toward player's last known position at intervals
//
//  Everything else (ground walk, gravity, platform resolution,
//  encasing, rolling, flight phase, direction changes) is
//  inherited from FlyingFoogaFoog → Botom → Enemy → Entity.
//  We only override update() and draw().
// ============================================================
class Tornado : public FlyingFoogaFoog {

public:

    // GameLevel loops over this array directly — must be public
    static const int MAX_KNIVES = 6;  // max knives alive at once
    Knife knives[MAX_KNIVES];

private:

    // --- Player position (updated each frame by GameLevel) ---
    // Tornado needs to know where the player is to aim knives.
    // GameLevel calls setPlayerPos() before update() each frame.
    double playerX, playerY;

    // --- Knife throw timer ---
    // Counts down; when it hits 0 a new knife is fired and timer resets.
    float knifeTimer;
    static constexpr float KNIFE_INTERVAL = 2.5f;  // seconds between throws
    static constexpr float KNIFE_SPEED = 350.f;  // pixels/sec

    // --- Randomized flight speed ---
    // Overrides FlyingFoogaFoog's fixed FLIGHT_SPEED.
    // Each time Tornado enters flight, it picks a new random speed.
    // This is what makes its trajectory unpredictable per spec.
    float currentFlightSpeed;
    static constexpr float MIN_FLIGHT_SPEED = 60.f;   // slow crawl
    static constexpr float MAX_FLIGHT_SPEED = 320.f;  // very fast dash

    // --- Try to fire a knife ---
    // Finds an inactive knife slot and launches it toward the player.
    void fireKnife();

    // --- Pick a new randomized flight speed ---
    // Called each time Tornado enters flight mode.
    void randomizeFlightSpeed();

public:

    Tornado(double startX, double startY,
        float scrW = 800.f, float scrH = 600.f);

    // Called by GameLevel every frame before update()
    // so Tornado always has a fresh player position to aim at
    void setPlayerPos(double px, double py);

    void update(double deltaTime) override;
    void draw(sf::RenderWindow& window) override;
};