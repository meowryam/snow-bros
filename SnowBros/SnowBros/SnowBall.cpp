#include "Snowball.h"


Snowball::Snowball(double startX, double startY, double dir,bool isPowerful, bool isLongRange, float scrW, float scrH)
    : Entity(startX, startY)
{
    direction = dir;  
    powerful = isPowerful;
    longRange = isLongRange;
    screenWidth = scrW;
    screenHeight = scrH;
    xspeed = direction * (longRange ? LONG_RANGE_SPEED : BASE_SPEED);
    yspeed = 0.0;
}



void Snowball::update(double deltaTime) {
    if (!alive) return;

    x += xspeed * deltaTime;
//screen wrap
    if (x + snowballhitboxsize   < 0.f) { x = screenWidth; }
    else if (x > screenWidth) { x = -snowballhitboxsize;}
}



void Snowball::draw(RenderWindow& window) {
    if (!alive) return;

    RectangleShape rect(Vector2f(snowballhitboxsize, snowballhitboxsize));
    rect.setPosition(Vector2f(static_cast<float>(x), static_cast<float>(y)));
    if (powerful) {rect.setFillColor(Color(180, 220, 255));}
    else {rect.setFillColor(sf::Color::White);}
    window.draw(rect);
}


void Snowball::resolvePlatforms(Platform platforms[], int count) {\
    (void)platforms;  
    (void)count;
}


bool Snowball::isRolling() const { return false;}



void Snowball::addSnow() {
    powerful = true;
    xspeed = direction * (LONG_RANGE_SPEED * 0.8);
}

void Snowball::setLongRange() {
    longRange = true;
    xspeed = direction * LONG_RANGE_SPEED;
}

FloatRect Snowball::getHitbox() const {
    return FloatRect(Vector2f(static_cast<float>(x), static_cast<float>(y)), Vector2f(snowballhitboxsize, snowballhitboxsize) );
}

float Snowball::getSize() const { return snowballhitboxsize;}