#include "Tornado.h"
#include <cstdlib>   // rand()
#include <cmath>     // sqrtf()

// ============================================================
//  Helper: random float between lo and hi
//  (same pattern as Botom — keeping things consistent)
// ============================================================
static float randFloatT(float lo, float hi) {
    return lo + static_cast<float>(rand()) / RAND_MAX * (hi - lo);
}

// ============================================================
//  Constructor
// ============================================================
Tornado::Tornado(double startX, double startY, float scrW, float scrH)
    : FlyingFoogaFoog(startX, startY, scrW, scrH)
    // FlyingFoogaFoog's constructor already sets up everything:
    //   Botom ground walk, gravity, encasing, rolling,
    //   flight timers, direction change timer
{
    playerX = 400.0;  // default to screen centre until GameLevel sets it
    playerY = 300.0;

    // Start knife timer at a random offset so multiple Tornados on screen
    // don't all throw at the exact same moment — staggers the attacks
    knifeTimer = randFloatT(0.5f, KNIFE_INTERVAL);

    // Pick an initial flight speed
    currentFlightSpeed = randFloatT(MIN_FLIGHT_SPEED, MAX_FLIGHT_SPEED);

    // All knives start inactive — Knife default constructor handles this
}

// ============================================================
//  setPlayerPos — called by GameLevel each frame before update()
// ============================================================
void Tornado::setPlayerPos(double px, double py) {
    playerX = px;
    playerY = py;
}

// ============================================================
//  randomizeFlightSpeed
//  Picks a new random speed and re-applies it to xspeed/yspeed
//  while keeping the same direction.
//  Called each time Tornado enters flight mode.
// ============================================================
void Tornado::randomizeFlightSpeed() {
    currentFlightSpeed = randFloatT(MIN_FLIGHT_SPEED, MAX_FLIGHT_SPEED);

    // Re-scale current velocity to the new speed.
    // We extract the current direction (unit vector) and multiply by new speed.
    float cx = static_cast<float>(xspeed);
    float cy = static_cast<float>(yspeed);
    float len = sqrtf(cx * cx + cy * cy);

    if (len > 0.f) {
        // Normalise then scale to new speed
        xspeed = (cx / len) * currentFlightSpeed;
        yspeed = (cy / len) * currentFlightSpeed;
    }
    // If len == 0 (no movement yet), pickRandomFlightDirection()
    // in FlyingFoogaFoog will set a direction on the next frame anyway
}

// ============================================================
//  fireKnife
//  Finds a free knife slot and launches it toward the player's
//  last known position in a straight line.
// ============================================================
void Tornado::fireKnife() {

    // Find an inactive knife slot
    for (int i = 0; i < MAX_KNIVES; i++) {
        if (knives[i].alive) continue;  // slot in use — skip

        // Found a free slot — set it up

        // Spawn at Tornado's centre so the knife comes from the middle
        // of the enemy rather than its top-left corner
        knives[i].x = x + hitboxbotom_width * 0.5 - Knife::WIDTH * 0.5;
        knives[i].y = y + hitboxbotom_height * 0.5 - Knife::HEIGHT * 0.5;

        // Calculate direction vector from Tornado to player
        double dx = playerX - knives[i].x;
        double dy = playerY - knives[i].y;

        // Normalise so the knife always travels at exactly KNIFE_SPEED
        // regardless of the distance to the player
        double len = sqrtf(static_cast<float>(dx * dx + dy * dy));
        if (len < 1.0) len = 1.0;  // safety: avoid divide by zero

        knives[i].xspeed = (dx / len) * KNIFE_SPEED;
        knives[i].yspeed = (dy / len) * KNIFE_SPEED;
        knives[i].alive = true;

        return;  // only fire one knife per call
    }
    // If all slots are full, skip silently — don't fire
}

// ============================================================
//  update
//  Layers Tornado-specific logic on top of FlyingFoogaFoog:
//  1. Randomize flight speed when entering flight
//  2. Throw knives at intervals
//  3. Update all active knives
//  4. Kill knives that leave the screen
// ============================================================
void Tornado::update(double deltaTime) {

    if (!alive) return;

    // If encased, delegate to parent chain — no knife throwing while trapped
    if (trap) {
        FlyingFoogaFoog::update(deltaTime);
        // Still update knives that are already in the air
        for (int i = 0; i < MAX_KNIVES; i++) {
            knives[i].update(deltaTime);
            // Kill knives that have left the screen
            if (knives[i].alive) {
                if (knives[i].x < -50 || knives[i].x > screenWidth + 50 ||
                    knives[i].y < -50 || knives[i].y > screenHeight + 50) {
                    knives[i].alive = false;
                }
            }
        }
        return;
    }

    // --- Remember whether we were in flight before this update ---
    // FlyingFoogaFoog::update() may switch inFlight on or off.
    // We check after the call to detect the moment flight starts.
    bool wasInFlight = inFlight;

    // Let FlyingFoogaFoog handle all movement (ground walk, gravity,
    // flight phase, direction changes, platform resolution, encasing)
    FlyingFoogaFoog::update(deltaTime);

    // --- Detect flight start — randomize speed ---
    // If we just entered flight this frame, pick a new random speed.
    // This is the key difference from FlyingFoogaFoog:
    // instead of a fixed FLIGHT_SPEED, Tornado uses a random one each flight.
    if (inFlight && !wasInFlight) {
        // Flight just started this frame
        randomizeFlightSpeed();
    }

    // --- Knife throw timer ---
    // Count down and fire when timer expires.
    // We throw knives both on the ground and in flight — more dangerous.
    knifeTimer -= static_cast<float>(deltaTime);
    if (knifeTimer <= 0.f) {
        fireKnife();
        // Reset with a small random variation so throws aren't perfectly rhythmic
        knifeTimer = KNIFE_INTERVAL + randFloatT(-0.3f, 0.3f);
    }

    // --- Update all active knives ---
    for (int i = 0; i < MAX_KNIVES; i++) {
        knives[i].update(deltaTime);

        // Deactivate knives that have flown off screen
        // We give a 50px margin so knives don't pop out near edges
        if (knives[i].alive) {
            if (knives[i].x < -50 || knives[i].x > screenWidth + 50 ||
                knives[i].y < -50 || knives[i].y > screenHeight + 50) {
                knives[i].alive = false;
            }
        }
    }
}

// ============================================================
//  draw
//  Purple rectangle for Tornado (distinct from red Botom / blue Fooga).
//  Also draws all active knives.
// ============================================================
void Tornado::draw(sf::RenderWindow& window) {
    if (!alive) return;

    sf::RectangleShape rect(sf::Vector2f(hitboxbotom_width, hitboxbotom_height));
    rect.setPosition(sf::Vector2f(static_cast<float>(x), static_cast<float>(y)));

    if (trap) {
        // Encased — white snowball look (same as all enemies)
        rect.setFillColor(sf::Color::White);
    }
    else if (hitFlashTimer > 0.f) {
        // Hit flash — pale purple
        rect.setFillColor(sf::Color(220, 180, 255));
    }
    else if (inFlight) {
        // Flying — bright magenta so player can see it's airborne
        rect.setFillColor(sf::Color(200, 0, 200));
    }
    else {
        // Ground — standard purple
        rect.setFillColor(sf::Color(120, 0, 180));
    }

    window.draw(rect);

    // Debug outline
    if (showDebug) {
        sf::RectangleShape outline(sf::Vector2f(hitboxbotom_width, hitboxbotom_height));
        outline.setPosition(sf::Vector2f(static_cast<float>(x), static_cast<float>(y)));
        outline.setFillColor(sf::Color::Transparent);
        outline.setOutlineThickness(1.f);
        outline.setOutlineColor(sf::Color::Red);
        window.draw(outline);
    }

    // Draw all active knives on top
    for (int i = 0; i < MAX_KNIVES; i++) {
        knives[i].draw(window);
    }
}