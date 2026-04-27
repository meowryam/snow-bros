#include "Snowball.h"

Snowball::Snowball(double startX, double startY, double dir, bool isPowerful, bool isLongRange, float scrW, float scrH)
    : Entity(startX, startY) {
    direction = dir;
    powerful = isPowerful;
    longRange = isLongRange;
    screenWidth = scrW;
    screenHeight = scrH;
    snowState = SnowballState::FLYING;
    snowHits = 0;
    xspeed = direction * (longRange ? LONG_RANGE_SPEED : BASE_SPEED);
    yspeed = 0.0;
}

void Snowball::update(double deltaTime) {
    if (!alive) return;

    if (snowState == SnowballState::ROLLING) {
        yspeed += 1200.0 * deltaTime;  // same gravity as player
        if (yspeed > 800.0) yspeed = 800.0;
        y += yspeed * deltaTime;
    }

    x += xspeed * deltaTime;
    //screen wrap
    animTimer += static_cast<float>(deltaTime);
    if (animTimer >= FRAME_DURATION) {
        animTimer = 0.f;
        animFrame++;
    }

    if (x + snowballhitboxsize < 0.f)  x = screenWidth;
    else if (x > screenWidth)           x = -snowballhitboxsize;
}
/*
void Snowball::draw(RenderWindow& window) {
    if (!alive) return;

    RectangleShape rect(Vector2f(snowballhitboxsize, snowballhitboxsize));
    rect.setPosition(Vector2f(static_cast<float>(x), static_cast<float>(y)));
    if (powerful) {rect.setFillColor(Color(180, 220, 255));}
    else {rect.setFillColor(sf::Color::White);}
    window.draw(rect);
}
*/
void Snowball::draw(sf::RenderWindow& window) {
    if (!alive) return;

    if (textureLoaded && sprite) {
        sf::IntRect cur;
        if (snowState == SnowballState::ROLLING) {
            sf::IntRect rollFrames[6] = { roll1, roll2, roll3, roll4, roll5, roll6 };
            // grow with each enemy hit, clamp to last frame
            int frameIdx = std::min(snowHits, 5);
            cur = rollFrames[frameIdx];
        }
        else {
            sf::IntRect flyFrames[5] = { proj1, proj2, proj3, proj4, proj5 };
            cur = flyFrames[animFrame % 5];
        }
        sprite->setTextureRect(cur);
        float scaleX = snowballhitboxsize / static_cast<float>(cur.size.x);
        float scaleY = snowballhitboxsize / static_cast<float>(cur.size.y);
        sprite->setScale({ scaleX, scaleY });
        sprite->setPosition({ static_cast<float>(x), static_cast<float>(y) });
        window.draw(*sprite);
    }
    else {
        // fallback rectangle
        sf::RectangleShape rect(sf::Vector2f(snowballhitboxsize, snowballhitboxsize));
        rect.setPosition(sf::Vector2f(static_cast<float>(x), static_cast<float>(y)));
        rect.setFillColor(powerful ? sf::Color(180, 220, 255) : sf::Color::White);
        window.draw(rect);
    }
}



void Snowball::resolvePlatforms(Platform platforms[], int count) {
    if (snowState != SnowballState::ROLLING) return;
    for (int i = 0; i < count; i++) {
        sf::FloatRect pRect = platforms[i].rect;
        sf::FloatRect hb = getHitbox();
        if (!hb.findIntersection(pRect)) continue;
        float feet = (float)y + snowballhitboxsize;
        float platTop = pRect.position.y;
        if (yspeed >= 0.f && feet - yspeed * 0.05f <= platTop + 5.f) {
            y = platTop - snowballhitboxsize;
            yspeed = 0.0;
            break;
        }
    }
}

bool Snowball::isRolling() const {
    return snowState == SnowballState::ROLLING;
}



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

void Snowball::startRolling(double dir) {
    snowState = SnowballState::ROLLING;
    xspeed = dir * 300.0;   // rolling speed
    yspeed = 0.0;
}
void Snowball::addSnowHit() { snowHits++; }
int Snowball::getSnowHits() const { return snowHits; }
void Snowball::loadTexture(const std::string& path) {
    textureLoaded = texture.loadFromFile(path);
    if (textureLoaded) {
        sprite.emplace(texture);
        sprite->setTextureRect(proj1);
    }
}