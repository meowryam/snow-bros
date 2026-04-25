#include "Snowball.h"

// ============================================================
//  Constructor
// ============================================================
Snowball::Snowball(double startX, double startY, double dir,
    bool isPowerful, bool isLongRange,
    float scrW, float scrH)
    : Entity(startX, startY)
    // Entity sets x, y, xspeed=0, yspeed=0, alive=true
{
    direction = dir;   // +1 = right, -1 = left
    powerful = isPowerful;
    longRange = isLongRange;
    screenWidth = scrW;
    screenHeight = scrH;

    // Set horizontal speed based on whether long range is active.
    // xspeed is inherited from Entity — positive = right, negative = left.
    xspeed = direction * (longRange ? LONG_RANGE_SPEED : BASE_SPEED);
    yspeed = 0.0;  // snowballs travel horizontally only — no gravity
}

// ============================================================
//  update — move the snowball and handle screen wrapping
// ============================================================
void Snowball::update(double deltaTime) {
    if (!alive) return;

    // Move horizontally
    x += xspeed * deltaTime;
    // No vertical movement — snowballs fly straight horizontally per spec

    // --- Screen edge wrapping ---
    // Snowball exits one side and re-enters from the other.
    // This is exactly what the spec describes:
    // "wraps around the screen edges"
    if (x + SIZE < 0.f) {
        // Exited left edge — re-enter from right
        x = screenWidth;
    }
    else if (x > screenWidth) {
        // Exited right edge — re-enter from left
        x = -SIZE;
    }
}

// ============================================================
//  draw — white square placeholder
// ============================================================
void Snowball::draw(sf::RenderWindow& window) {
    if (!alive) return;

    sf::RectangleShape rect(sf::Vector2f(SIZE, SIZE));
    rect.setPosition(sf::Vector2f(static_cast<float>(x), static_cast<float>(y)));

    if (powerful) {
        // Powered snowball — light blue tint so player can see
        // their snowball power is active
        rect.setFillColor(sf::Color(180, 220, 255));
    }
    else {
        // Normal snowball — white
        rect.setFillColor(sf::Color::White);
    }

    window.draw(rect);
}

// ============================================================
//  resolvePlatforms
//  Snowballs travel horizontally and wrap at screen edges.
//  They don't collide with platforms — they fly through them.
//  This method exists because GameLevel calls it on everything,
//  so we provide it as a no-op to keep the interface consistent.
// ============================================================
void Snowball::resolvePlatforms(Platform platforms[], int count) {
    // Intentionally empty — snowballs ignore platforms.
    // If you later want snowballs to bounce off platform edges,
    // add that logic here without touching anything else.
    (void)platforms;  // suppress unused parameter warning
    (void)count;
}

// ============================================================
//  isRolling
//  Always false — rolling is the enemy's responsibility.
//  When a player kicks a fully trapped enemy, startRolling()
//  is called on the enemy directly. The snowball projectile
//  itself never rolls — it dies on contact.
// ============================================================
bool Snowball::isRolling() const {
    return false;
}

// ============================================================
//  addSnow
//  Called by GameLevel's spawnSnowball() when snowball power
//  is active. Makes the snowball powerful — one hit fully encases.
//  The enemy's receiveSnowballHit() checks this via the
//  powerful flag passed from the game loop.
//
//  Note: we store it here so the collision system in GameLevel
//  can check sb->powerful before calling receiveSnowballHit(),
//  or we can expose it via isPowerful() and let the enemy decide.
// ============================================================
void Snowball::addSnow() {
    powerful = true;
    // Also boost speed slightly — powerful snowballs feel heavier
    // and faster, gives good game feel
    xspeed = direction * (LONG_RANGE_SPEED * 0.8);
}

// ============================================================
//  setLongRange
//  Called by spawnSnowball() when distance increase is active.
// ============================================================
void Snowball::setLongRange() {
    longRange = true;
    xspeed = direction * LONG_RANGE_SPEED;
}

// ============================================================
//  getHitbox
//  Returns 24x24 rectangle — matches what GameLevel hardcodes
//  in checkCollisions(): sf::FloatRect(..., sf::Vector2f(24.f, 24.f))
// ============================================================
sf::FloatRect Snowball::getHitbox() const {
    return sf::FloatRect(
        sf::Vector2f(static_cast<float>(x), static_cast<float>(y)),
        sf::Vector2f(SIZE, SIZE)
    );
}

float Snowball::getSize() const {
    return SIZE;
}