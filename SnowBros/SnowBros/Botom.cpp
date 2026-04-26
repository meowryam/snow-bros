 #include "Botom.h"
#include <cstdlib>   


static float randFloat(float low, float high) {
    return low + static_cast<float>(rand()) / RAND_MAX * (high - low);
}

Botom::Botom(double startX, double startY, float scrW, float scrH)
    : Enemy(startX, startY, 2, 80.0)
{
    screenWidth = scrW;
    screenHeight = scrH;
    isOnGround = false;
    xspeed = (rand() % 2 == 0) ? speed : -speed; //random direction(L or R)
    yspeed = 0.0;
    directionTimer = randFloat(1.5f, 3.5f);
    directionInterval = directionTimer;
    shakeoutTimer = SHAKEOUT_DURATION;
    hitFlashTimer = 0.f;
    snowballHits = 0;
    showDebug = false;
    rolling = false;
    rollSpeed = 300.0;  
}



void Botom::update(double deltaTime) {
    if (!alive) return;

    if (trap && rolling) {
        x += xspeed * deltaTime;
        shakeoutTimer -= static_cast<float>(deltaTime);
        if (shakeoutTimer <= 0.f) {
            trap = false;
            snowballHits = 0;
            shakeoutTimer = SHAKEOUT_DURATION;
            xspeed = speed;  
        }
        if (!isOnGround) {
            yspeed += GRAVITY * deltaTime;
            if (yspeed > MAX_FALL) yspeed = MAX_FALL;
        }
        y += yspeed * deltaTime;
        return;
    }

    if (trap) return;  

//gravity
    if (!isOnGround) {
        yspeed += GRAVITY * deltaTime;
        if (yspeed > MAX_FALL) yspeed = MAX_FALL;
    }
//veclocity
    x += xspeed * deltaTime;
    y += yspeed * deltaTime;
//screenwrap
    if (x + hitboxbotom_width < 0.f)       x = screenWidth;
    else if (x > screenWidth)  x = -hitboxbotom_width;

 //floor clamp
    float floorY = screenHeight - 24.f - hitboxbotom_height;  // 24 = floor platform height
    if (y >= floorY) {
        y = floorY;
        yspeed = 0.0;
        isOnGround = true;
    }

 
    directionTimer -= static_cast<float>(deltaTime);
    if (directionTimer <= 0.f) {
        xspeed = -xspeed;  
        directionTimer = randFloat(1.5f, 3.5f);   
    }

   
    if (hitFlashTimer > 0.f)
        hitFlashTimer -= static_cast<float>(deltaTime);
}



void Botom::draw(RenderWindow& window) {
    if (!alive) return;

    RectangleShape rect(Vector2f(hitboxbotom_width, hitboxbotom_height));
    rect.setPosition(Vector2f(static_cast<float>(x), static_cast<float>(y)));

    if (trap) {
        rect.setFillColor(Color::White);
    }
    else if (hitFlashTimer > 0.f) {
        rect.setFillColor(Color(255, 255, 200));
    }
    else {
        rect.setFillColor(Color(200, 50, 50));
    }
    window.draw(rect);

    
    if (showDebug) {
        sf::RectangleShape outline(sf::Vector2f(hitboxbotom_width, hitboxbotom_height));
        outline.setPosition(sf::Vector2f(static_cast<float>(x), static_cast<float>(y)));
        outline.setFillColor(sf::Color::Transparent);
        outline.setOutlineThickness(1.f);
        outline.setOutlineColor(sf::Color::Red);
        window.draw(outline);
    }
}


bool Botom::receiveSnowballHit() {
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
        return true;
    }
    return false;
}



void Botom::resolvePlatforms(Platform platforms[], int count) {
    isOnGround = false;

   FloatRect myBox = getHitbox();

    for (int i = 0; i < count; i++) {
        FloatRect pRect = platforms[i].rect;
        if (!myBox.findIntersection(pRect)) continue;
        float feet = static_cast<float>(y) + hitboxbotom_height;
        float platTop = pRect.position.y;
        if (yspeed >= 0.0 && feet - yspeed * 0.05 <= platTop + 6.f) {
            y = static_cast<double>(platTop) - hitboxbotom_height;
            yspeed = 0.0;
            isOnGround = true;
            break;
        }
    }
}


FloatRect Botom::getHitbox() const {  return FloatRect( Vector2f(static_cast<float>(x), static_cast<float>(y)), Vector2f(hitboxbotom_width, hitboxbotom_height));}
void Botom::setShowDebug(bool show) { showDebug = show;}
void Botom::startRolling(double dir) {
    rolling = true;
    xspeed = dir * rollSpeed;  
    yspeed = 0.0;
}

bool Botom::isRolling() const {
    return rolling;
}