#pragma once
#include "Entity.h"
#include "KeyBindings.h"
#include "Platform.h"
#include <SFML/Graphics.hpp>
#include <optional>
#include <string>
using namespace std;

enum class PlayerAnim {
    IDLE, WALK, JUMP, FALL, THROW, DEAD, BALLOON
};

class Player : public Entity {
private:
    sf::Texture               texture;
    optional<sf::Sprite>      sprite;
    bool                      textureLoaded;
    bool                      facingRight;

    PlayerAnim animState;
    float      animTimer;
    int        currentFrame;
    static const float FRAME_TIME;

    struct AnimRow { int y, h, frames, fw; };
    static const AnimRow ANIM_IDLE;
    static const AnimRow ANIM_WALK;
    static const AnimRow ANIM_JUMP;
    static const AnimRow ANIM_THROW;
    static const AnimRow ANIM_DEAD;
    static const AnimRow ANIM_BALLOON;

    static const double GRAVITY;
    static const double JUMP_FORCE;
    static const double MOVE_SPEED;

    bool  onGround;
    bool  isJumping;
    bool  jumpKeyHeld; // prevent auto-repeat jump

    bool  speedBoostActive;
    bool  balloonActive;
    bool  snowballPowerActive;
    bool  distanceIncreaseActive;
    float speedBoostTimer;
    float balloonTimer;
    float invincTimer;
    bool  invincible;

    const AnimRow* getAnimRow() const {
        switch (animState) {
        case PlayerAnim::WALK:    return &ANIM_WALK;
        case PlayerAnim::JUMP:    return &ANIM_JUMP;
        case PlayerAnim::FALL:    return &ANIM_JUMP;
        case PlayerAnim::THROW:   return &ANIM_THROW;
        case PlayerAnim::DEAD:    return &ANIM_DEAD;
        case PlayerAnim::BALLOON: return &ANIM_BALLOON;
        default:                  return &ANIM_IDLE;
        }
    }

    void updateAnimation(float dt) {
        if (!textureLoaded) return;
        animTimer += dt;
        const AnimRow* row = getAnimRow();
        if (animTimer >= FRAME_TIME) {
            animTimer = 0.f;
            currentFrame = (currentFrame + 1) % row->frames;
        }
        sprite->setTextureRect(sf::IntRect(
            sf::Vector2i(currentFrame * row->fw, row->y),
            sf::Vector2i(row->fw, row->h)
        ));
        float sc = 0.28f;
        sprite->setScale({ facingRight ? sc : -sc, sc });
        if (!facingRight)
            sprite->setOrigin({ (float)row->fw, 0.f });
        else
            sprite->setOrigin({ 0.f, 0.f });
    }

public:
    bool wantsToThrow;
    bool isDead;

    // Default constructor needed for Game member
    Player()
        : Entity(100.0, 200.0),
        textureLoaded(false), facingRight(true),
        animState(PlayerAnim::IDLE), animTimer(0.f), currentFrame(0),
        onGround(false), isJumping(false), jumpKeyHeld(false),
        speedBoostActive(false), balloonActive(false),
        snowballPowerActive(false), distanceIncreaseActive(false),
        speedBoostTimer(0.f), balloonTimer(0.f),
        invincTimer(0.f), invincible(false),
        wantsToThrow(false), isDead(false)
    {
    }

    Player(double startX, double startY)
        : Entity(startX, startY),
        textureLoaded(false), facingRight(true),
        animState(PlayerAnim::IDLE), animTimer(0.f), currentFrame(0),
        onGround(false), isJumping(false), jumpKeyHeld(false),
        speedBoostActive(false), balloonActive(false),
        snowballPowerActive(false), distanceIncreaseActive(false),
        speedBoostTimer(0.f), balloonTimer(0.f),
        invincTimer(0.f), invincible(false),
        wantsToThrow(false), isDead(false)
    {
    }

    bool loadTexture(const string& path) {
        if (!texture.loadFromFile(path)) return false;
        sprite.emplace(texture);
        textureLoaded = true;
        return true;
    }

    void applySpeedBoost() { speedBoostActive = true; speedBoostTimer = 15.f; }
    void applyBalloon() { balloonActive = true; balloonTimer = 10.f; }
    void applySnowballPower() { snowballPowerActive = true; }
    void applyDistanceIncrease() { distanceIncreaseActive = true; }

    bool hasSnowballPower()    const { return snowballPowerActive; }
    bool hasDistanceIncrease() const { return distanceIncreaseActive; }
    bool isBalloonActive()     const { return balloonActive; }
    bool isInvincible()        const { return invincible; }
    bool isFacingRight()       const { return facingRight; }
    double getX()              const { return x; }
    double getY()              const { return y; }

    sf::FloatRect getHitboxPublic() const {
        return sf::FloatRect(
            sf::Vector2f((float)x + 20.f, (float)y + 10.f),
            sf::Vector2f(44.f, 70.f)
        );
    }

    void handleInput(const KeyBindings& keys) {
        if (isDead) return;
        double speed = MOVE_SPEED * (speedBoostActive ? 1.5 : 1.0);
        bool moving = false;

        if (sf::Keyboard::isKeyPressed(keys.moveLeft)) {
            xspeed = -speed; facingRight = false; moving = true;
        }
        else if (sf::Keyboard::isKeyPressed(keys.moveRight)) {
            xspeed = speed; facingRight = true; moving = true;
        }
        else {
            xspeed = 0;
        }

        bool jumpPressed = sf::Keyboard::isKeyPressed(keys.jump);
        if (jumpPressed && !jumpKeyHeld && onGround) {
            yspeed = JUMP_FORCE;
            onGround = false;
            isJumping = true;
            jumpKeyHeld = true;
        }
        if (!jumpPressed) jumpKeyHeld = false;

        if (sf::Keyboard::isKeyPressed(keys.throwSnowball))
            wantsToThrow = true;

        if (!onGround)
            animState = (yspeed < 0) ? PlayerAnim::JUMP : PlayerAnim::FALL;
        else if (moving)
            animState = PlayerAnim::WALK;
        else
            animState = PlayerAnim::IDLE;
    }

    void update(double deltaTime) override {
        if (isDead) {
            animState = PlayerAnim::DEAD;
            updateAnimation((float)deltaTime);
            return;
        }

        double grav = balloonActive ? GRAVITY * 0.15 : GRAVITY;
        yspeed += grav * deltaTime;
        x += xspeed * deltaTime;
        y += yspeed * deltaTime;

        // screen wrap
        if (x > 800) x = -40;
        if (x < -40) x = 800;
        if (y < 0) { y = 0; yspeed = 0; }

        // timers
        if (speedBoostActive) { speedBoostTimer -= (float)deltaTime; if (speedBoostTimer <= 0) speedBoostActive = false; }
        if (balloonActive) { balloonTimer -= (float)deltaTime; if (balloonTimer <= 0) balloonActive = false; }
        if (invincible) { invincTimer -= (float)deltaTime; if (invincTimer <= 0) invincible = false; }

        updateAnimation((float)deltaTime);
    }

    void resolvePlatforms(Platform platforms[], int count) {
        onGround = false;
        sf::FloatRect hb = getHitboxPublic();
        for (int i = 0; i < count; i++) {
            sf::FloatRect& p = platforms[i].rect;
            if (yspeed >= 0 &&
                hb.position.x + hb.size.x > p.position.x &&
                hb.position.x < p.position.x + p.size.x &&
                hb.position.y + hb.size.y >= p.position.y &&
                hb.position.y + hb.size.y <= p.position.y + p.size.y + 12.f) {
                y = p.position.y - hb.size.y - 10.f;
                yspeed = 0;
                onGround = true;
                isJumping = false;
            }
        }
    }

    void takeDamage() {
        if (invincible || balloonActive) return;
        isDead = true;
        animState = PlayerAnim::DEAD;
        invincible = true;
        invincTimer = 2.f;
    }

    void respawn(double sx, double sy) {
        x = sx; y = sy;
        xspeed = 0; yspeed = 0;
        isDead = false;
        invincible = true;
        invincTimer = 2.f;
        animState = PlayerAnim::IDLE;
        currentFrame = 0;
        onGround = false;
    }

    //void update(double deltaTime) override; // defined above

    void draw(sf::RenderWindow& window) override {
        if (!alive) return;
        if (invincible && (int)(invincTimer * 10) % 2 == 0) return;

        if (!textureLoaded) {
            sf::RectangleShape r(sf::Vector2f(48.f, 72.f));
            r.setPosition({ (float)x, (float)y });
            r.setFillColor(sf::Color::Cyan);
            window.draw(r);
            return;
        }
        sprite->setPosition({ (float)x, (float)y });
        window.draw(*sprite);
    }
};


