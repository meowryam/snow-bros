#pragma once
#include "Entity.h"
#include "Platform.h"

// ============================================================
//  Snowball — the player's primary weapon
//
//  Lifecycle:
//  1. Spawned by the game loop when player presses throw
//  2. Travels horizontally in the direction the player faces
//  3. Wraps around screen edges (exits one side, enters other)
//  4. On contact with an unencased enemy:
//       - calls enemy->receiveSnowballHit()
//       - snowball dies (setalive(false))
//  5. isRolling() always returns false — rolling is handled
//     inside the enemy via startRolling(). GameLevel's collision
//     check for rolling is done against the enemy, not the snowball.
//
//  Drawn as a white circle (approximated as a square placeholder
//  until sprites arrive).
// ============================================================

class Snowball : public Entity {

private:

    // --- Screen boundaries for wrapping ---
    float screenWidth;
    float screenHeight;

    // --- Size of the snowball hitbox ---
    static constexpr float SIZE = 24.f;  // 24x24 as GameLevel expects

    // --- Power flags ---
    // Set by spawnSnowball() in GameLevel based on active power-ups
    bool powerful;    // if true, one hit fully encases enemy (snowball power)
    bool longRange;   // if true, travels at higher speed (distance increase)

    // --- Base and boosted speeds ---
    static constexpr double BASE_SPEED = 350.0;  // normal throw speed
    static constexpr double LONG_RANGE_SPEED = 600.0; // with distance increase

    // --- Direction: +1.0 = right, -1.0 = left ---
    double direction;

public:

    // Constructor — pass spawn position and direction
    // powerful and longRange default to false (normal snowball)
    Snowball(double startX, double startY, double dir,
        bool isPowerful = false, bool isLongRange = false,
        float scrW = 800.f, float scrH = 600.f);

    // Called every frame
    void update(double deltaTime) override;

    // Draw placeholder white square
    void draw(sf::RenderWindow& window) override;

    // Resolve collision with platforms —
    // snowball dies if it hits a wall (platforms are vertical boundaries too)
    // For now snowball passes through platforms horizontally,
    // only wraps at screen edges. Keep simple as discussed.
    void resolvePlatforms(Platform platforms[], int count);

    // Always false — rolling is the enemy's responsibility
    // GameLevel calls this but rolling is handled via enemy->startRolling()
    bool isRolling() const;

    // Power flags — set externally by spawnSnowball in GameLevel
    void addSnow();          // makes next hit a full encase (powerful mode)
    void setLongRange();     // boosts speed to long range speed

    // Getters GameLevel needs
    sf::FloatRect getHitbox() const;
    float getSize() const;
};