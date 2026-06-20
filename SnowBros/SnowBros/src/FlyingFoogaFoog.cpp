#include "FlyingFoogaFoog.h"
#include <cstdlib>   
#include <cmath>     
#include "sfmlcolours.h"
using namespace sfmlcolours;

FlyingFoogaFoog::FlyingFoogaFoog(double startX, double startY, float scrW, float scrH) : Botom(startX, startY, scrW, scrH)
{
    inFlight = false;
    groundTimer = GROUND_DURATION;
    flightTimer = 0.f;
    flightDirTimer = 0.f;
}

void FlyingFoogaFoog::pickRandomFlightDirection() {
    int dir = rand() % 8;
    float dx = 0.f, dy = 0.f;

    switch (dir) {
    case 0:  dx = 1.f; dy = 0.f; break;  // right
    case 1:  dx = -1.f; dy = 0.f; break;  // left
    case 2:  dx = 0.f; dy = -1.f; break;  // up
    case 3:  dx = 0.f; dy = 1.f; break;  // down
    case 4:  dx = 1.f; dy = -1.f; break;  // up-right
    case 5:  dx = -1.f; dy = -1.f; break;  // up-left
    case 6:  dx = 1.f; dy = 1.f; break;  // down-right
    case 7:  dx = -1.f; dy = 1.f; break;  // down-left
    }
// Normalise so diagonals aren't faster than cardinals
    float len = sqrtf(dx * dx + dy * dy);
    if (len > 0.f) { dx /= len; dy /= len; }

    xspeed = dx * FLIGHT_SPEED;
    yspeed = dy * FLIGHT_SPEED;
}

void FlyingFoogaFoog::update(double deltaTime) {
    if (!alive) return;

    if (trap || snowballHits > 0) {
        inFlight = false;
        Botom::update(deltaTime);
        return;
    }

    
    if (inFlight) {
        flightTimer -= static_cast<float>(deltaTime);
        flightDirTimer -= static_cast<float>(deltaTime);

        if (flightDirTimer <= 0.f) {
            pickRandomFlightDirection();
            flightDirTimer = FLIGHT_DIR_INTERVAL;
        }

        x += xspeed * deltaTime;
        y += yspeed * deltaTime;

       
        if (x + hitboxbotom_width < 0.f)  x = screenWidth;
        else if (x > screenWidth)         x = -hitboxbotom_width;

        
        float topLimit = 0.f;
        float bottomLimit = screenHeight - 24.f - hitboxbotom_height;
        if (y < topLimit) { y = topLimit;      yspeed = -yspeed; }
        if (y > bottomLimit) { y = bottomLimit;   yspeed = -yspeed; }

        if (hitFlashTimer > 0.f)
            hitFlashTimer -= static_cast<float>(deltaTime);

        
        animTimer += static_cast<float>(deltaTime);
        if (animTimer >= FRAME_DURATION) {
            animTimer = 0.f;
            animFrame++;
        }

       
        if (flightTimer <= 0.f) {
            inFlight = false;
            groundTimer = GROUND_DURATION;
            yspeed = 0.0;
        }

        return;
    }

    Botom::update(deltaTime);

    if (isOnGround) {
        groundTimer -= static_cast<float>(deltaTime);
        if (groundTimer <= 0.f) {
            inFlight = true;
            flightTimer = FLIGHT_DURATION;
            flightDirTimer = 0.f;
        }
    }
}

void FlyingFoogaFoog::draw(sf::RenderWindow& window) {
    if (!alive) return;

    if (trap || snowballHits > 0) {
        Botom::draw(window);
        return;
    }

    if (textureLoaded && sprite) {
        sf::IntRect cur;

        if (inFlight) {
            sf::IntRect flyFrames[3] = { fly_frame1, fly_frame2, fly_frame3 };
            cur = flyFrames[animFrame % 3];
        }
        else {
            sf::IntRect glideFrames[3] = { glide_frame1, glide_frame2, glide_frame3 };
            cur = glideFrames[animFrame % 3];
        }

        sprite->setTextureRect(cur);

        float scaleX = hitboxbotom_width / static_cast<float>(cur.size.x);
        float scaleY = hitboxbotom_height / static_cast<float>(cur.size.y);

        float drawX = static_cast<float>(x);
        if (xspeed < 0) {
            sprite->setScale({ -scaleX, scaleY });
            drawX += hitboxbotom_width;
        }
        else {
            sprite->setScale({ scaleX, scaleY });
        }

        sprite->setPosition({ drawX, static_cast<float>(y) });
        window.draw(*sprite);
    }
    else {

        sf::RectangleShape rect(sf::Vector2f(hitboxbotom_width, hitboxbotom_height));
        rect.setPosition(sf::Vector2f(static_cast<float>(x), static_cast<float>(y)));
        if (hitFlashTimer > 0.f) rect.setFillColor(sf::Color(173, 216, 230));
        else if (inFlight)            rect.setFillColor(sf::Color(0, 150, 255));
        else                          rect.setFillColor(sf::Color(65, 105, 225));
        window.draw(rect);
    }

    if (showDebug) {
        sf::RectangleShape outline(sf::Vector2f(hitboxbotom_width, hitboxbotom_height));
        outline.setPosition(sf::Vector2f(static_cast<float>(x), static_cast<float>(y)));
        outline.setFillColor(sf::Color::Transparent);
        outline.setOutlineThickness(1.f);
        outline.setOutlineColor(sf::Color::Red);
        window.draw(outline);
    }
}


bool FlyingFoogaFoog::receiveSnowballHit() {
    if (trap || !alive) return false;

    snowballHits++;
    hitFlashTimer = HIT_FLASH_DURATION;

    if (snowballHits == 1) {
        xspeed *= 0.5;
    }

    if (snowballHits >= HITS_TO_ENCASE) {   // 6 
        trap = true;
        rolling = false;
        xspeed = 0.0;
        yspeed = 0.0;
        inFlight = false;
        escapeTimer = ESCAPE_DURATION;
        return true;
    }
    return false;
}

void FlyingFoogaFoog::loadTexture(const std::string& path) {
    textureLoaded = texture.loadFromFile(path);
    if (textureLoaded) {
        sprite.emplace(texture);
        sprite->setTextureRect(fly_frame1);
        float scaleX = hitboxbotom_width / static_cast<float>(fly_frame1.size.x);
        float scaleY = hitboxbotom_height / static_cast<float>(fly_frame1.size.y);
        sprite->setScale({ scaleX, scaleY });
    }
}