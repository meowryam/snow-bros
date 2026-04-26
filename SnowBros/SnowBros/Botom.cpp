 #include "Botom.h"
#include <cstdlib>   

#include "sfmlcolours.h"
using namespace sfmlcolours;

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


    // ── Animate ──────────────────────────────────────────
    animTimer += static_cast<float>(deltaTime);
    if (animTimer >= FRAME_DURATION) {
        animTimer = 0.f;
        animFrame++;
    }

    if (!textureLoaded || !sprite) return;

    if (!alive) {
        // cycle through dying frames
        sf::IntRect dyingFrames[9] = {
            dying_frame1, dying_frame2, dying_frame3,
            dying_frame4, dying_frame5, dying_frame6,
            dying_frame7, dying_frame8, dying_frame9
        };
        int idx = std::min(animFrame, 8);   // clamp so it stays on last frame
        sprite->setTextureRect(dyingFrames[idx]);
    }
    else if (trap && rolling) {
        // alternate between the two topped frames while rolling
        sprite->setTextureRect(animFrame % 2 == 0 ? topped_frame1 : topped_frame2);
    }
    else if (trap) {
        // encased — alternate unsheathing frames (shaking in place)
        sf::IntRect unsheathFrames[3] = { unsheathing_frame1, unsheathing_frame2, unsheathing_frame3 };
        sprite->setTextureRect(unsheathFrames[animFrame % 3]);
    }
    else if (std::abs(xspeed) > speed * 1.5) {
        // charging — flip between the two charge frames
        sprite->setTextureRect(animFrame % 2 == 0 ? charging_frame1 : charging_frame2);
    }
    else {
        // normal walk — alternate idle and walking
        sprite->setTextureRect(animFrame % 2 == 0 ? idle : walking);
    }

    // Flip sprite horizontally based on movement direction
  // In update(), replace the flip block with:
    float scaleX = hitboxbotom_width / static_cast<float>(idle.size.x);
    float scaleY = hitboxbotom_height / static_cast<float>(idle.size.y);

    if (xspeed < 0)
        sprite->setScale({ -scaleX, scaleY });
    else
        sprite->setScale({ scaleX, scaleY });
}


/*
void Botom::draw(RenderWindow& window) {
    if (!alive) return;

    RectangleShape rect(Vector2f(hitboxbotom_width, hitboxbotom_height));
    rect.setPosition(Vector2f(static_cast<float>(x), static_cast<float>(y)));

    if (trap) { rect.setFillColor(Color::White); }
    else if (hitFlashTimer > 0.f) {rect.setFillColor(PaleYellow); }
    else {  rect.setFillColor(Crimson); }
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

*/
void Botom::draw(sf::RenderWindow& window) {
    if (!alive) return;

    if (textureLoaded && sprite) {
        // Position the sprite; account for horizontal flip offset
      ;
        float drawX = static_cast<float>(x);
        if (xspeed < 0)
            drawX += hitboxbotom_width;   // this stays the same, hitbox width is already in pixels
        sprite->setPosition({ drawX, static_cast<float>(y) });

        window.draw(*sprite);
    }
    else {
        // Fallback colored rectangle if texture failed to load
        sf::RectangleShape rect(sf::Vector2f(hitboxbotom_width, hitboxbotom_height));
        rect.setPosition(sf::Vector2f(static_cast<float>(x), static_cast<float>(y)));
        if (trap)                   rect.setFillColor(sf::Color::White);
        else if (hitFlashTimer > 0.f) rect.setFillColor(sf::Color::Yellow);
        else                        rect.setFillColor(sf::Color::Red);
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

void Botom::loadTexture(const std::string& path) {
    textureLoaded = texture.loadFromFile(path);
    if (textureLoaded) {
        sprite.emplace(texture);
        sprite->setTextureRect(idle);

        // Scale sprite so it visually matches the hitbox size
        float scaleX = hitboxbotom_width / static_cast<float>(idle.size.x);
        float scaleY = hitboxbotom_height / static_cast<float>(idle.size.y);
        sprite->setScale({ scaleX, scaleY });
    }
}