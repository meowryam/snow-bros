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
    hitbox = FloatRect(Vector2f(position.x + 6.f, position.y + 2.f), Vector2f(36.f, 44.f));
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

    hitbox = FloatRect( Vector2f(position.x + 6.f, position.y + 2.f), Vector2f(36.f, 44.f) );
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

bool Player::loadTexture(const string& path) {

    if (!texture.loadFromFile(path)) {  return false; }
    sprite.setTexture(texture);
    sprite.setScale(Vector2f(1.5f, 1.5f));
    return true;
}
void Player::handleInput() {
    if (!isAlive) return;
    velocity.x = 0.f; //no key pressed

    if (Keyboard::isKeyPressed(keyLeft)) {
        velocity.x = -speed; 
        facing = Direction::LEFT;
        sprite.setScale(sf::Vector2f(-1.5f, 1.5f));
    }
    else if (Keyboard::isKeyPressed(keyRight)) {
        velocity.x = speed; 
        facing = Direction::RIGHT;
        sprite.setScale(sf::Vector2f(1.5f, 1.5f));
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
    if (facing == Direction::LEFT) {
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

//animation
    if (state == PlayerState::THROWING) {
        if (!isOnGround) {
            state = (velocity.y < 0.f) ? PlayerState::JUMPING : PlayerState::FALLING;
        }
        else if (velocity.x != 0.f) {
            state = PlayerState::WALKING;
        }
        else {
            state = PlayerState::IDLE;
        }
    }
    else if (!isOnGround) {
     // In the air
        if (velocity.y < 0.f) {
            state = PlayerState::JUMPING;   
        }
        else {
            state = PlayerState::FALLING;  
        }
    }
    else if (velocity.x != 0.f) {
        state = PlayerState::WALKING;  
    }
    else {
        state = PlayerState::IDLE;     
    }
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
