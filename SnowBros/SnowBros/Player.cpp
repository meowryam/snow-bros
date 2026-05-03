
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
    deathTimer = 0.f;
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
        
        keyLeft = Keyboard::Key::A;
        keyRight = Keyboard::Key::D;
        keyJump = Keyboard::Key::W;
        keyThrow = Keyboard::Key::S;
    }
    else {
        
        keyLeft = Keyboard::Key::Left;
        keyRight = Keyboard::Key::Right;
        keyJump = Keyboard::Key::Up;
        keyThrow = Keyboard::Key::Space;
    }
    state = PlayerState::FALLING;
    facing = Direction::RIGHT;     
    isAlive = true;

    hitbox = FloatRect(Vector2f(position.x, position.y), Vector2f(36.f, 44.f));
    

    debugBox.setSize(Vector2f(36.f, 44.f));
    debugBox.setFillColor(Color::Transparent);     
    debugBox.setOutlineColor(Color::Green);        
    debugBox.setOutlineThickness(1.f);
    showDebug = false;
    invincibleTimer = 3.f;
    deathTimer = 0.f;
    speedBoostActive = false;
    speedBoostTimer = 0.f;
    balloonModeActive = false;
    balloonModeTimer = 0.f;
    snowballPowerActive = false;
    distanceIncreaseActive = false;

    sprite.setPosition(position);
   }

bool Player::loadTexture(const string& path) {
    if (!texture.loadFromFile(path)) return false;
    sprite.setTexture(texture);
    sprite.setTextureRect(walk1);
    sprite.setOrigin({ 0.f, 0.f });   
    return true;
}

void Player::handleInput() {
    if (!isAlive) return;
    velocity.x = 0.f; 

    if (Keyboard::isKeyPressed(keyLeft)) {
        velocity.x = -speed; 
        facing = Direction::LEFT;
      
    }
    else if (Keyboard::isKeyPressed(keyRight)) {
        velocity.x = speed; 
        facing = Direction::RIGHT;
       
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
    if (!isAlive) {
        if (deathTimer > 0.f) {
            deathTimer -= deltaTime;
          
            animTimer += deltaTime;
            if (animTimer >= FRAME_DURATION) { animTimer = 0.f; animFrame++; }
            sf::IntRect hurtFrames[7] = { hurt1, hurt2, hurt3, hurt4, hurt5, hurt6, hurt7 };
            sprite.setTextureRect(hurtFrames[std::min(animFrame, 6)]);
        }
        return;
    }
    if (invincibleTimer > 0.f)
        invincibleTimer -= deltaTime;
    if (!isOnGround && !balloonModeActive) {
        velocity.y += GRAVITY * deltaTime;
        if (velocity.y > MAX_FALL_SPEED)
            velocity.y = MAX_FALL_SPEED;
        if (velocity.y > 50.f && (state == PlayerState::IDLE || state == PlayerState::WALKING))
            state = PlayerState::FALLING;
    }
 // balloon
    if (balloonModeActive) {
        velocity.y = -80.f;
        balloonModeTimer -= deltaTime;
        if (balloonModeTimer <= 0.f) {
            balloonModeActive = false;
            balloonModeTimer = 0.f;
            velocity.y = 0.f;
        }
    }
// speedboost
    if (speedBoostActive) {
        speedBoostTimer -= deltaTime;
        if (speedBoostTimer <= 0.f) {
            speedBoostActive = false;
            speedBoostTimer = 0.f;
            speed = BASE_SPEED;
        }
    }

    position.x += velocity.x * deltaTime;
    position.y += velocity.y * deltaTime;
    if (position.x + 36.f < 0.f)        position.x = screenWidth;
    if (position.x > screenWidth)        position.x = -36.f;

// animation state reset
    if (state != prevState) {
        
        bool groundedTransition =
            (prevState == PlayerState::IDLE || prevState == PlayerState::WALKING) &&
            (state == PlayerState::IDLE || state == PlayerState::WALKING);

        if (!groundedTransition) {
            animFrame = 0;
            animTimer = 0.f;
        }
        prevState = state;
    }
    animTimer += deltaTime;
    if (animTimer >= FRAME_DURATION) {
        animTimer = 0.f;
        animFrame++;
    }

 // Pick frame
    if (state == PlayerState::DEAD) {
        sf::IntRect hurtFrames[7] = { hurt1, hurt2, hurt3, hurt4, hurt5, hurt6, hurt7 };
        sprite.setTextureRect(hurtFrames[std::min(animFrame, 6)]);
    }
    else if (state == PlayerState::THROWING) {
        sf::IntRect throwFrames[7] = { throw1, throw2, throw3, throw4, throw5, throw6, throw7 };
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
        sprite.setTextureRect(walk1);
    }

    static constexpr float REF_W = 220.f;
    static constexpr float REF_H = 275.f;
    static constexpr float SCALE_X = 36.f / REF_W;
    static constexpr float SCALE_Y = 44.f / REF_H;

    sf::IntRect cur = sprite.getTextureRect();
    float originX = REF_W / 2.f; 
    float originY = REF_H;
    float footX = position.x + 18.f;
    float footY = position.y + 44.f;

    if (facing == Direction::LEFT) {
        sprite.setOrigin({ originX, originY });
        sprite.setScale({ SCALE_X, SCALE_Y });
    }
    else {
        sprite.setOrigin({ originX, originY });
        sprite.setScale({ -SCALE_X, SCALE_Y });
    }
    sprite.setPosition({ footX, footY });

    hitbox = FloatRect(Vector2f(position.x, position.y), Vector2f(36.f, 44.f));
    debugBox.setPosition(Vector2f(hitbox.position.x, hitbox.position.y));
}

void Player::resolvePlatforms(Platform platforms[], int count) {
    
    isOnGround = false;

    for (int i = 0; i < count; i++) {
        FloatRect pRect = platforms[i].rect;
        if (!hitbox.findIntersection(pRect)) continue;

        float platTop = pRect.position.y;

        if (velocity.y >= 0.f) {
           
            position.y = platTop - 44.f;
            velocity.y = 0.f;
            isOnGround = true;
            if (state == PlayerState::JUMPING || state == PlayerState::FALLING)
                state = (velocity.x != 0.f) ? PlayerState::WALKING : PlayerState::IDLE;
          
            hitbox = FloatRect(Vector2f(position.x, position.y), Vector2f(36.f, 44.f));
            break;
        }
    }
}
void Player::draw(RenderWindow& window) const {
  
    window.draw(sprite);

    if (showDebug) {
        window.draw(debugBox);
         }
}
void Player::landOnPlatform(float topOfPlatform) {
    position.y = topOfPlatform - 44.f; 
    velocity.y = 0.f;
    isOnGround = true;
    if (state == PlayerState::JUMPING || state == PlayerState::FALLING) {
        state = (velocity.x != 0.f) ? PlayerState::WALKING : PlayerState::IDLE;
    }
}
void Player::leaveGround() {
    isOnGround = false;
}
void Player::takeDamage()
{
    if (!isAlive) return;
    if (invincibleTimer > 0.f) return;
    int currentLives = playerData.getLives();
    if (currentLives <= 0) return;
    playerData.setLives(currentLives - 1);

    state = PlayerState::DEAD;
    isAlive = false;

    if (playerData.getLives() > 0) {
        deathTimer = 2.0f;   
    }
    else {
        deathTimer = 0.f;    
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
    deathTimer = 0.f;
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

