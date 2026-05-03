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
    jumpTimer = randFloat(2.f, 4.f);
    jumpInterval = jumpTimer;
    isJumping = false;
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
                alive = false;  
                return;
            }
        
        if (!isOnGround) {
            yspeed += GRAVITY * deltaTime;
            if (yspeed > MAX_FALL) yspeed = MAX_FALL;
        }
        y += yspeed * deltaTime;
        return;
    }

    if (trap && !rolling) {
        escapeTimer -= static_cast<float>(deltaTime);
        if (escapeTimer <= 0.f) {
            trap = false;
            snowballHits = 0;
            escapeTimer = 0.f;
            xspeed = (rand() % 2 == 0) ? speed : -speed;
        }
       
        animTimer += static_cast<float>(deltaTime);
        if (animTimer >= FRAME_DURATION) {
            animTimer = 0.f;
            animFrame++;
        }
        return;
    }

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


//animate
    animTimer += static_cast<float>(deltaTime);
    if (animTimer >= FRAME_DURATION) {
        animTimer = 0.f;
        animFrame++;
    }

    if (!textureLoaded || !sprite) return;

    if (!alive) {
        IntRect dyingFrames[9] = {
            dying_frame1, dying_frame2, dying_frame3,
            dying_frame4, dying_frame5, dying_frame6,
            dying_frame7, dying_frame8, dying_frame9
        };
        int idx = std::min(animFrame, 8);   
        sprite->setTextureRect(dyingFrames[idx]);
    }
    else if (trap && rolling) { sprite->setTextureRect(animFrame % 2 == 0 ? topped_frame1 : topped_frame2);  }
    else if (trap) { sprite->setTextureRect(snow_r2_s4);}
    else if (snowballHits == 1) {
        IntRect encaseFrames[4] = { snow_r1_s1, snow_r1_s2, snow_r1_s3, snow_r1_s4 };
        sprite->setTextureRect(encaseFrames[animFrame % 4]);
    }
    else if (std::abs(xspeed) > speed * 1.5) { sprite->setTextureRect(animFrame % 2 == 0 ? charging_frame1 : charging_frame2); }
    else {
        IntRect walkFrames[3] = { idle, walking, walking_frame2 };
        sprite->setTextureRect(walkFrames[animFrame % 3]);
    }
    jumpTimer -= static_cast<float>(deltaTime);
    if (jumpTimer <= 0.f && isOnGround) {
        yspeed = JUMP_FORCE;
        isOnGround = false;
        isJumping = true;
        jumpTimer = randFloat(2.f, 4.f);  
    }

    if (yspeed >= 0.f) isJumping = false;  // falling, not jumping anymore
    float scaleX = hitboxbotom_width / static_cast<float>(idle.size.x);
    float scaleY = hitboxbotom_height / static_cast<float>(idle.size.y);

    if (xspeed < 0)
        sprite->setScale({ -scaleX, scaleY });
    else
        sprite->setScale({ scaleX, scaleY });
}



void Botom::draw(RenderWindow& window) {
    if (!alive) return;

    bool showingSnow = (trap && !rolling) || snowballHits > 0;

    if (showingSnow && snowTextureLoaded && snowSprite) {
        sf::IntRect cur;

        if (trap && !rolling) {
            sf::IntRect fullFrames[8] = {
                snow_r3_s1, snow_r3_s2, snow_r3_s3, snow_r3_s4,
                snow_r4_s1, snow_r4_s2, snow_r4_s3, snow_r4_s4
            };
            if (escapeTimer < 1.5f) {
                sf::IntRect warnFrames[4] = { snow_r4_s1, snow_r4_s2, snow_r4_s3, snow_r4_s4 };
                cur = warnFrames[animFrame % 4];
            }
            else {
                cur = fullFrames[animFrame % 8];
            }
        }
        else {
            sf::IntRect partialFrames[8] = {
                snow_r1_s1, snow_r1_s2, snow_r1_s3, snow_r1_s4,
                snow_r2_s1, snow_r2_s2, snow_r2_s3, snow_r2_s4
            };
            float progress = (float)snowballHits / (float)(HITS_TO_ENCASE - 1);
            int frameIdx = std::min((int)(progress * 7.f), 7);
            cur = partialFrames[frameIdx];
        }

        snowSprite->setTextureRect(cur);
        float scaleX = hitboxbotom_width / static_cast<float>(cur.size.x);
        float scaleY = hitboxbotom_height / static_cast<float>(cur.size.y);
        snowSprite->setScale({ scaleX, scaleY });
        snowSprite->setPosition({ static_cast<float>(x), static_cast<float>(y) });
        window.draw(*snowSprite);

    }
    else if (textureLoaded && sprite) {
        float drawX = static_cast<float>(x);
        if (xspeed < 0)
            drawX += hitboxbotom_width;
        sprite->setPosition({ drawX, static_cast<float>(y) });
        window.draw(*sprite);

    }
    else {
        RectangleShape rect(Vector2f(hitboxbotom_width, hitboxbotom_height));
        rect.setPosition(Vector2f(static_cast<float>(x), static_cast<float>(y)));
        if (trap) rect.setFillColor(Color::White);
        else if (hitFlashTimer > 0.f) rect.setFillColor(Color::Yellow);
        else rect.setFillColor(Color::Red);
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
        escapeTimer = ESCAPE_DURATION;   
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
        float scaleX = hitboxbotom_width / static_cast<float>(idle.size.x);
        float scaleY = hitboxbotom_height / static_cast<float>(idle.size.y);
        sprite->setScale({ scaleX, scaleY });
    }
}
void Botom::loadSnowTexture(const std::string& path) {
    snowTextureLoaded = snowTexture.loadFromFile(path);
    if (snowTextureLoaded) {
        snowSprite.emplace(snowTexture);
    }
}