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
    case 4:  dx = 1.f; dy = -1.f; break;  // up-right  (diagonal)
    case 5:  dx = -1.f; dy = -1.f; break;  // up-left   (diagonal)
    case 6:  dx = 1.f; dy = 1.f; break;  // down-right(diagonal)
    case 7:  dx = -1.f; dy = 1.f; break;  // down-left (diagonal)
    }

    // Normalise if diagonal so speed is the same in all 8 directions
    float len = sqrtf(dx * dx + dy * dy);  // 1.0 for cardinal, ~1.414 for diagonal
    if (len > 0.f) {
        dx /= len;
        dy /= len;
    }

  
    xspeed = dx * FLIGHT_SPEED;
    yspeed = dy * FLIGHT_SPEED;
}


void FlyingFoogaFoog::update(double deltaTime) {
    if (!alive) return;
    if (trap) {
        Botom::update(deltaTime);
        return;
    }

 //flight mode
    if (inFlight) {

        flightTimer -= static_cast<float>(deltaTime);
        flightDirTimer -= static_cast<float>(deltaTime);
        if (flightDirTimer <= 0.f) {
            pickRandomFlightDirection();
            flightDirTimer = FLIGHT_DIR_INTERVAL;
        }

        x += xspeed * deltaTime;
        y += yspeed * deltaTime;
//screen wrap
        if (x + hitboxbotom_width < 0.f)      x = screenWidth;
        else if (x > screenWidth)              x = -hitboxbotom_width;

 //vertical clamp
        float topLimit = 0.f;
        float bottomLimit = screenHeight - 24.f - hitboxbotom_height;
        if (y < topLimit) {
            y = topLimit;
            yspeed = -yspeed; 
        }
        if (y > bottomLimit) {
            y = bottomLimit;
            yspeed = -yspeed;  
        }
        if (hitFlashTimer > 0.f)
            hitFlashTimer -= static_cast<float>(deltaTime);

     
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

/*

void FlyingFoogaFoog::draw(RenderWindow& window) {
    if (!alive) return;

    RectangleShape rect(Vector2f(hitboxbotom_width, hitboxbotom_height));
    rect.setPosition(Vector2f(static_cast<float>(x), static_cast<float>(y)));

    if (trap) {rect.setFillColor(Color::White); }
    else if (hitFlashTimer > 0.f) {  rect.setFillColor(PaleBlue);}
    else if (inFlight) {rect.setFillColor(AquaBlue);}
    else { rect.setFillColor(RoyalBlue);}

    window.draw(rect);

    if (showDebug) {
        RectangleShape outline(Vector2f(hitboxbotom_width, hitboxbotom_height));
        outline.setPosition(Vector2f(static_cast<float>(x), static_cast<float>(y)));
        outline.setFillColor(Color::Transparent);
        outline.setOutlineThickness(1.f);
        outline.setOutlineColor(Color::Red);
        window.draw(outline);
    }
}
*/


void FlyingFoogaFoog::draw(sf::RenderWindow& window) {
    if (!alive) return;

    if (textureLoaded && sprite) {
        // Pick correct animation frame
        if (trap) {
            sf::IntRect toppedFrames[3] = {
                topped_fooga_frame1, topped_fooga_frame2, topped_fooga_frame3
            };
            sprite->setTextureRect(toppedFrames[animFrame % 3]);
        }
        else if (inFlight) {
            // alternate between fly frames while airborne
            sf::IntRect flyFrames[3] = { fly_frame1, fly_frame2, fly_frame3 };
            sprite->setTextureRect(flyFrames[animFrame % 3]);
        }
        else {
            // on ground — use glide/hover frames
            sf::IntRect glideFrames[3] = { glide_frame1, glide_frame2, glide_frame3 };
            sprite->setTextureRect(glideFrames[animFrame % 3]);
        }

        // Scale + flip
        float scaleX = hitboxbotom_width / static_cast<float>(fly_frame1.size.x);
        float scaleY = hitboxbotom_height / static_cast<float>(fly_frame1.size.y);

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
        // Fallback rectangle
        sf::RectangleShape rect(sf::Vector2f(hitboxbotom_width, hitboxbotom_height));
        rect.setPosition(sf::Vector2f(static_cast<float>(x), static_cast<float>(y)));
        if (trap)               rect.setFillColor(sf::Color::White);
        else if (hitFlashTimer > 0.f) rect.setFillColor(sf::Color(173, 216, 230));
        else if (inFlight)           rect.setFillColor(sf::Color(0, 150, 255));
        else                         rect.setFillColor(sf::Color(65, 105, 225));
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