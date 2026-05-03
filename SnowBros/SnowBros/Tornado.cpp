#include "Tornado.h"
#include <cstdlib>   
#include <cmath>     

static float randFloatT(float low, float high)
{
    return low + static_cast<float>(rand()) / RAND_MAX * (high - low);
}

Tornado::Tornado(double startX, double startY, float scrW, float scrH) : FlyingFoogaFoog(startX, startY, scrW, scrH)
{
    playerX = 400.0;
    playerY = 300.0;
    knifeTimer = randFloatT(0.5f, KNIFE_INTERVAL);
    currentFlightSpeed = randFloatT(MIN_FLIGHT_SPEED, MAX_FLIGHT_SPEED);
}


void Tornado::setPlayerPos(double px, double py) {
    playerX = px;
    playerY = py;
}


void Tornado::randomizeFlightSpeed() {
    currentFlightSpeed = randFloatT(MIN_FLIGHT_SPEED, MAX_FLIGHT_SPEED);
    float cx = static_cast<float>(xspeed);
    float cy = static_cast<float>(yspeed);
    float len = sqrtf(cx * cx + cy * cy);

    if (len > 0.f) {
        xspeed = (cx / len) * currentFlightSpeed;
        yspeed = (cy / len) * currentFlightSpeed;
    }
}


void Tornado::fireKnife() {
    for (int i = 0; i < MAX_KNIVES; i++) {
        if (knives[i].alive) continue;
        knives[i].x = x + hitboxbotom_width * 0.5 - Knife::knifehitboxwidth * 0.5;
        knives[i].y = y + hitboxbotom_height * 0.5 - Knife::knifehitboxheight * 0.5;

        double dx = playerX - knives[i].x;
        double dy = playerY - knives[i].y;

        double len = sqrtf(static_cast<float>(dx * dx + dy * dy));
        if (len < 1.0) len = 1.0;

        knives[i].xspeed = (dx / len) * KNIFE_SPEED;
        knives[i].yspeed = (dy / len) * KNIFE_SPEED;
        knives[i].alive = true;

        return;
    }
}

void Tornado::update(double deltaTime) {
    if (!alive) return;
    if (trap || snowballHits > 0) {
        inFlight = false;
// Kill any in-flight knives when encased
        for (int i = 0; i < MAX_KNIVES; i++)
            knives[i].alive = false;
        FlyingFoogaFoog::update(deltaTime);
        return;
    }


    bool wasInFlight = inFlight;
    FlyingFoogaFoog::update(deltaTime);
    if (inFlight && !wasInFlight) { randomizeFlightSpeed(); }

    knifeTimer -= static_cast<float>(deltaTime);
    if (knifeTimer <= 0.f) {
        fireKnife();
        knifeTimer = KNIFE_INTERVAL + randFloatT(-0.3f, 0.3f);
    }

    for (int i = 0; i < MAX_KNIVES; i++) {
        knives[i].update(deltaTime);
        if (knives[i].alive) {
            if (knives[i].x < -50 || knives[i].x > screenWidth + 50 ||
                knives[i].y < -50 || knives[i].y > screenHeight + 50) {
                knives[i].alive = false;
            }
        }
    }
}


bool Tornado::receiveSnowballHit() {
    if (trap || !alive) return false;

    snowballHits++;
    hitFlashTimer = HIT_FLASH_DURATION;

    if (snowballHits == 1) {
        xspeed *= 0.5;
    }

    if (snowballHits >= HITS_TO_ENCASE) {   
        trap = true;
        rolling = false;
        xspeed = 0.0;
        yspeed = 0.0;
        inFlight = false;   //gravity
        escapeTimer = ESCAPE_DURATION;
        return true;
    }
    return false;
}

void Tornado::loadTexture(const std::string& path) {
    tornadoTextureLoaded = tornadoTexture.loadFromFile(path);
    if (tornadoTextureLoaded) {
        tornadoSprite.emplace(tornadoTexture);
        tornadoSprite->setTextureRect(t_idle);
        float scaleX = hitboxbotom_width / static_cast<float>(t_idle.size.x);
        float scaleY = hitboxbotom_height / static_cast<float>(t_idle.size.y);
        tornadoSprite->setScale({ scaleX, scaleY });
    }
}


void Tornado::draw(sf::RenderWindow& window) {
    if (!alive) return;

    if (trap || snowballHits > 0) {
        Botom::draw(window);
        return;  
    }
    if (tornadoTextureLoaded && tornadoSprite) {
       IntRect chosenFrame = t_idle;

        if (inFlight) {
            IntRect spinFrames[9] = {
                t_spin1, t_spin2, t_spin3, t_spin4,
                t_spin5, t_spin6, t_spin7, t_spin8, t_spin9
            };
            chosenFrame = spinFrames[animFrame % 9];
        }
        else if (knifeTimer < 0.3f) {
            chosenFrame = (animFrame % 2 == 0) ? t_throw1 : t_throw2;
        }
        else {
            IntRect runFrames[3] = { t_run1, t_run2, t_run3 };
            chosenFrame = runFrames[animFrame % 3];
        }

        tornadoSprite->setTextureRect(chosenFrame);
        float scaleX = hitboxbotom_width / static_cast<float>(chosenFrame.size.x);
        float scaleY = hitboxbotom_height / static_cast<float>(chosenFrame.size.y);

        float drawX = static_cast<float>(x);
        if (xspeed < 0) {
            tornadoSprite->setScale({ -scaleX, scaleY });
            drawX += hitboxbotom_width;
        }
        else {
            tornadoSprite->setScale({scaleX, scaleY});
        }
        tornadoSprite->setPosition({ drawX, static_cast<float>(y) });
        window.draw(*tornadoSprite);
    }

    else {
        RectangleShape rect(Vector2f(hitboxbotom_width, hitboxbotom_height));
        rect.setPosition(Vector2f(static_cast<float>(x), static_cast<float>(y)));
        if (hitFlashTimer > 0.f) rect.setFillColor(Color(220, 180, 255));
        else if (inFlight) rect.setFillColor(Color(200, 0, 200));
        else rect.setFillColor(Color(120, 0, 180));
        window.draw(rect);
    }

    if (showDebug) {
        RectangleShape outline(Vector2f(hitboxbotom_width, hitboxbotom_height));
        outline.setPosition(Vector2f(static_cast<float>(x), static_cast<float>(y)));
        outline.setFillColor(Color::Transparent);
        outline.setOutlineThickness(1.f);
        outline.setOutlineColor(Color::Red);
        window.draw(outline);
    }

// Draw knives only when not encased
    for (int i = 0; i < MAX_KNIVES; i++)
        knives[i].draw(window);
}