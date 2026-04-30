#include "Player.h"
Player::Player()
    : playerData(*(new PlayerData())),  
    sprite(texture)
{
    
    playerNumber = 1;
    screenWidth = 800.f;
    screenHeight = 600.f;
    position = Vector2f(100.f, 500.f);
    velocity = Vector2f(0.f, 0.f);
    speed = BASE_SPEED;
    invincibleTimer = 3.f;  // 3 seconds of spawn protection
    wantsToThrow = false;
    isOnGround = false;
    canJump = true;
    isAlive = true;
    state = PlayerState::FALLING;
    facing = Direction::RIGHT;
    speedBoostActive = balloonModeActive = snowballPowerActive = distanceIncreaseActive = false;
    speedBoostTimer = balloonModeTimer = 0.f;
    showDebug = false;
    keyLeft = Keyboard::Key::Left;
    keyRight = Keyboard::Key::Right;
    keyJump = Keyboard::Key::Up;
    keyThrow = Keyboard::Key::Space;
    debugBox.setSize(Vector2f(36.f, 44.f));
    debugBox.setFillColor(Color::Transparent);
    debugBox.setOutlineColor(Color::Green);
    debugBox.setOutlineThickness(1.f);
    hitbox = FloatRect(Vector2f(position.x, position.y), Vector2f(36.f, 44.f));
}

Player::Player(PlayerData& data, int playerNum, float screenW, float screenH)
    : playerData(data),        
    playerNumber(playerNum),
    screenWidth(screenW),
    screenHeight(screenH),
    sprite(texture)          
{
    if (playerNumber == 1) {  position = Vector2f(100.f, 500.f); }
    else { position = Vector2f(screenWidth - 150.f, 500.f);  }
    velocity = Vector2f(0.f, 0.f);
    speed = BASE_SPEED; 
    isOnGround = false;
    canJump = true;

    if (playerNumber == 1) {
        //p1 keys
        keyLeft = Keyboard::Key::Left;
        keyRight = Keyboard::Key::Right;
        keyJump = Keyboard::Key::Up;
        keyThrow = Keyboard::Key::Space;
    }
    else {
        //p2 keys
        keyLeft = Keyboard::Key::A;
        keyRight = Keyboard::Key::D;
        keyJump = Keyboard::Key::W;
        keyThrow = Keyboard::Key::Space;
    }
    state = PlayerState::FALLING;
    facing = Direction::RIGHT;     
    isAlive = true;

    hitbox = FloatRect(Vector2f(position.x, position.y), Vector2f(36.f, 44.f));
    //(position, size) — both Vector2f.

    debugBox.setSize(Vector2f(36.f, 44.f));
    debugBox.setFillColor(Color::Transparent);     
    debugBox.setOutlineColor(Color::Green);        
    debugBox.setOutlineThickness(1.f);
    showDebug = false;

    speedBoostActive = false;
    speedBoostTimer = 0.f;
    balloonModeActive = false;
    balloonModeTimer = 0.f;
    snowballPowerActive = false;
    distanceIncreaseActive = false;

    sprite.setPosition(position);
   }
/*
bool Player::loadTexture(const string& path) {

    if (!texture.loadFromFile(path)) {  return false; }
    sprite.setTexture(texture);
    sprite.setScale(Vector2f(1.5f, 1.5f));
    return true;
} */
bool Player::loadTexture(const string& path) {
    if (!texture.loadFromFile(path)) return false;
    sprite.setTexture(texture);
    sprite.setTextureRect(walk1);
    sprite.setOrigin({ 0.f, 0.f });   // default: no flip, faces LEFT naturally
    return true;
}

void Player::handleInput() {
    if (!isAlive) return;
    velocity.x = 0.f; //no key pressed

    if (Keyboard::isKeyPressed(keyLeft)) {
        velocity.x = -speed; 
        facing = Direction::LEFT;
       // sprite.setScale(sf::Vector2f(-1.5f, 1.5f));
    }
    else if (Keyboard::isKeyPressed(keyRight)) {
        velocity.x = speed; 
        facing = Direction::RIGHT;
        //sprite.setScale(sf::Vector2f(1.5f, 1.5f));
    }
    if (Keyboard::isKeyPressed(keyJump)) {
        if (isOnGround && canJump) {
            velocity.y = JUMP_FORCE;  
            isOnGround = false;
            canJump = false;      
            state = PlayerState::JUMPING;
        }
    }
    else {
        canJump = true;
    }
    if (Keyboard::isKeyPressed(keyThrow)) {
        wantsToThrow = true;
    }
    else {
        wantsToThrow = false;
    }
}
void Player::update(float deltaTime) {
    if (!isAlive) return;
    if (invincibleTimer > 0.f)
        invincibleTimer -= deltaTime;
    //gravity-validity
    if (!isOnGround && !balloonModeActive) {

        velocity.y += GRAVITY * deltaTime;
        if (velocity.y > MAX_FALL_SPEED) {
            velocity.y = MAX_FALL_SPEED;
        }
    }

    //balloon
    if (balloonModeActive) {
        velocity.y = -80.f;
        balloonModeTimer -= deltaTime;
        if (balloonModeTimer <= 0.f) {
            balloonModeActive = false;
            balloonModeTimer = 0.f;
            velocity.y = 0.f;
        }
    }

    //speedboost
    if (speedBoostActive) {
        speedBoostTimer -= deltaTime;
        if (speedBoostTimer <= 0.f) {
            speedBoostActive = false;
            speedBoostTimer = 0.f;
            speed = BASE_SPEED;
        }
    }

    //playermovement
    position.x += velocity.x * deltaTime;
    position.y += velocity.y * deltaTime;

    //screen wrapping
    const float spriteWidth = 48.f;

    if (position.x + spriteWidth < 0.f) {
        position.x = screenWidth;
    }
    else if (position.x > screenWidth) {
        position.x = -spriteWidth;
    }

    //sync hitbox
      /*  if (facing == Direction::LEFT) {
            sprite.setPosition(Vector2f(position.x + spriteWidth, position.y));
             }
        else {
            sprite.setPosition(position);
        }
        hitbox = FloatRect(
            Vector2f(position.x + 6.f, position.y + 2.f),
            Vector2f(36.f, 44.f)
        );
        debugBox.setPosition(Vector2f(hitbox.position.x, hitbox.position.y));
        */
    if (state != prevState) {
        animFrame = 0;
        animTimer = 0.f;
        prevState = state;
    }

    animTimer += deltaTime;
    if (animTimer >= FRAME_DURATION) {
        animTimer = 0.f;
        animFrame++;
    }

    // Pick frame
    if (state == PlayerState::DEAD) {
        sf::IntRect hurtFrames[7] = {
            hurt1, hurt2, hurt3, hurt4, hurt5, hurt6, hurt7
        };
        sprite.setTextureRect(hurtFrames[std::min(animFrame, 6)]);
    }
    else if (state == PlayerState::THROWING) {
        sf::IntRect throwFrames[7] = {
            throw1, throw2, throw3, throw4, throw5, throw6, throw7
        };
        sprite.setTextureRect(throwFrames[animFrame % 7]);
    }
    else if (state == PlayerState::JUMPING || state == PlayerState::FALLING) {
        sf::IntRect jumpFrames[3] = { jump1, jump2, jump3 };
        sprite.setTextureRect(jumpFrames[animFrame % 3]);
    }
    else if (state == PlayerState::WALKING) {
        sf::IntRect walkFrames[11] = {
            walk1, walk2, walk3, walk4, walk5,
            walk6, walk7, walk8, walk9, walk10, walk11
        };
        sprite.setTextureRect(walkFrames[animFrame % 11]);
    }
    else {
        sprite.setTextureRect(walk1); // idle
    }

    // Scale — use current frame size for correct proportions
    sf::IntRect cur = sprite.getTextureRect();
    float scaleX = 36.f / static_cast<float>(cur.size.x);
    float scaleY = 44.f / static_cast<float>(cur.size.y);

    if (facing == Direction::LEFT) {
        // flip: set origin to right edge of frame, mirror X
        sprite.setOrigin({ static_cast<float>(cur.size.x), 0.f });
        sprite.setScale({ scaleX, scaleY });
        sprite.setPosition(position);   // position is always top-left of hitbox area
    }
    else {
        sprite.setOrigin({ 0.f, 0.f });
        sprite.setScale({ -scaleX, scaleY });
        sprite.setPosition(position);
    }

    hitbox = FloatRect(
        Vector2f(position.x - 33.f, position.y),
        Vector2f(36.f, 44.f)
    );
    debugBox.setPosition(Vector2f(hitbox.position.x, hitbox.position.y));
}
void Player::draw(RenderWindow& window) const {
   // if (!isAlive) return; 
    window.draw(sprite);

    if (showDebug) {
        window.draw(debugBox);
         }
}
void Player::landOnPlatform(float topOfPlatform) {
    position.y = topOfPlatform - 48.f;
    velocity.y = 0.f;
    isOnGround = true;
    if (state == PlayerState::JUMPING || state == PlayerState::FALLING) {
        state = (velocity.x != 0.f) ? PlayerState::WALKING : PlayerState::IDLE;
    }
}
void Player::leaveGround() {
    isOnGround = false;
}
void Player::takeDamage() {
    if (!isAlive) return; 
    int currentLives = playerData.getLives();
    playerData.setLives(currentLives - 1);

    if (playerData.getLives() <= 0) {
        isAlive = false;
        state = PlayerState::DEAD;
    }
    else {
        state = PlayerState::DEAD;
        isAlive = false;
    }
}
void Player::collectLife() {
    playerData.setLives(playerData.getLives() + 1);
}
void Player::activateSpeedBoost(float duration) {
    speedBoostActive = true;
    speedBoostTimer = duration;     
    speed = BASE_SPEED * 1.5f;       
}
void Player::activateBalloonMode(float duration) {
    balloonModeActive = true;
    balloonModeTimer = duration;     
    isOnGround = false;               
}

void Player::activateSnowballPower() {
    snowballPowerActive = true;
}

void Player::activateDistanceIncrease() {
    distanceIncreaseActive = true;
}
void Player::resetForNewLevel(Vector2f spawnPosition) {
    position = spawnPosition;
    velocity = Vector2f(0.f, 0.f);
    isOnGround = false;
    canJump = true;
    invincibleTimer = 3.f;  // 3 seconds of s
    isAlive = true;
    state = PlayerState::FALLING; 
    facing = Direction::RIGHT;
    speedBoostActive = false;
    speedBoostTimer = 0.f;
    balloonModeActive = false;
    balloonModeTimer = 0.f;
    snowballPowerActive = false;
    distanceIncreaseActive = false;
    speed = BASE_SPEED; 
    sprite.setPosition(position);
}
void Player::resolvePlatforms(Platform platforms[], int count) {
    isOnGround = false;

    for (int i = 0; i < count; i++) {
        FloatRect pRect = platforms[i].rect;  

        if (!hitbox.findIntersection(pRect)) continue;

        float playerFeet = position.y + 48.f;
        float platTop = pRect.position.y;

        if (velocity.y >= 0.f && playerFeet - velocity.y * 0.05f <= platTop + 5.f) {
            landOnPlatform(platTop);
            break;
        }
    }
}

Vector2f Player::getPosition() const {return position;}
FloatRect Player::getHitbox() const {return hitbox;}
PlayerState Player::getState() const {return state;}
Direction Player::getFacing() const {return facing;}
bool Player::getIsAlive() const {return isAlive;}
bool Player::isSnowballPowerActive() const {return snowballPowerActive;}
bool Player::isDistanceIncreaseActive() const {return distanceIncreaseActive;}
bool Player::isBalloonModeActive() const {return balloonModeActive;}
void Player::setShowDebug(bool show) {showDebug = show; }
void Player::setOnGround(bool onGround) {isOnGround = onGround; }

