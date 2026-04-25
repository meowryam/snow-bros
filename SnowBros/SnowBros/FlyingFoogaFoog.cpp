#include "FlyingFoogaFoog.h"
#include <cstdlib>   
#include <cmath>     
FlyingFoogaFoog::FlyingFoogaFoog(double startX, double startY, float scrW, float scrH)
    : Botom(startX, startY, scrW, scrH)
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



void FlyingFoogaFoog::draw(RenderWindow& window) {
    if (!alive) return;

    RectangleShape rect(Vector2f(hitboxbotom_width, hitboxbotom_height));
    rect.setPosition(Vector2f(static_cast<float>(x), static_cast<float>(y)));

    if (trap) {rect.setFillColor(Color::White); }
    else if (hitFlashTimer > 0.f) {  rect.setFillColor(sf::Color(200, 200, 255));}
    else if (inFlight) {rect.setFillColor(sf::Color(0, 220, 255));}
    else { rect.setFillColor(Color(50, 100, 220));}

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