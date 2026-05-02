#pragma once
#include "Enemy.h"
#include "Platform.h"
#include <SFML/Graphics.hpp>
#include <optional>
#include <string>
#include <cmath>
using namespace std;

// ?? Orb Bomb ?????????????????????????????????????????????????
struct OrbBomb {
    double x, y, vx, vy;
    bool   alive;
    bool   exploding;
    float  explodeTimer;
    float  fuseTimer;
    float  animTimer;
    int    animFrame;
    bool   onGround;
    float  flashTimer;

    static constexpr float FUSE_TIME = 4.f;
    static constexpr float EXPLODE_DURATION = 0.4f;
    static constexpr float EXPLODE_RADIUS = 80.f;
    static constexpr float WALK_SPEED = 55.f;
    static constexpr float GRAVITY = 1200.f;
    static constexpr float MAX_FALL = 800.f;
    static constexpr float W = 40.f;
    static constexpr float H = 44.f;
    static constexpr float FRAME_T = 0.08f;
    static constexpr float FLASH_START = 1.5f; // start flashing this many secs before explode

    OrbBomb() : x(0), y(0), vx(0), vy(0),
        alive(false), exploding(false),
        explodeTimer(0.f), fuseTimer(0.f),
        animTimer(0.f), animFrame(0),
        onGround(false), flashTimer(0.f) {
    }

    void spawn(double sx, double sy, double dir) {
        x = sx; y = sy;
        vx = dir * WALK_SPEED;
        vy = 0.0;
        alive = true;
        exploding = false;
        explodeTimer = 0.f;
        fuseTimer = 0.f;
        animTimer = 0.f;
        animFrame = 0;
        onGround = false;
        flashTimer = 0.f;
    }

    void startExplode() {
        exploding = true;
        explodeTimer = 0.f;
        vx = 0.0;
        vy = 0.0;
    }

    sf::FloatRect getHitbox() const {
        return sf::FloatRect(sf::Vector2f((float)x, (float)y),
            sf::Vector2f(W, H));
    }

    sf::FloatRect getBlastBox() const {
        return sf::FloatRect(
            sf::Vector2f((float)x - EXPLODE_RADIUS, (float)y - EXPLODE_RADIUS),
            sf::Vector2f(W + EXPLODE_RADIUS * 2.f, H + EXPLODE_RADIUS * 2.f));
    }

    void update(double dt, Platform platforms[], int platformCount,
        float screenWidth) {
        if (!alive) return;

        if (exploding) {
            explodeTimer += (float)dt;
            animTimer += (float)dt;
            if (animTimer >= FRAME_T) { animTimer = 0.f; animFrame++; }
            if (explodeTimer >= EXPLODE_DURATION) alive = false;
            return;
        }

        fuseTimer += (float)dt;

        // flash when close to exploding
        if (fuseTimer >= FUSE_TIME - FLASH_START) {
            flashTimer += (float)dt;
        }

        if (fuseTimer >= FUSE_TIME) {
            startExplode();
            return;
        }

        // gravity
        if (!onGround) {
            vy += GRAVITY * dt;
            if (vy > MAX_FALL) vy = MAX_FALL;
        }

        x += vx * dt;
        y += vy * dt;

        // screen wrap horizontal
        if (x + W < 0)           x = screenWidth;
        else if (x > screenWidth) x = -W;

        // platform resolution
        onGround = false;
        for (int i = 0; i < platformCount; i++) {
            sf::FloatRect pr = platforms[i].rect;
            sf::FloatRect hb = getHitbox();
            if (!hb.findIntersection(pr)) continue;
            float feet = (float)y + H;
            float platTop = pr.position.y;
            if (vy >= 0.0 && feet - vy * 0.05 <= platTop + 6.f) {
                y = platTop - H;
                vy = 0.0;
                onGround = true;
                break;
            }
        }

        // floor clamp
        float floorY = 600.f - 24.f - H;
        if (y >= floorY) {
            y = floorY;
            vy = 0.0;
            onGround = true;
        }

        // reverse direction at screen edges
        if (x <= 0) { x = 0;           vx = WALK_SPEED; }
        if (x + W >= screenWidth) { x = screenWidth - W; vx = -WALK_SPEED; }

        // animation
        animTimer += (float)dt;
        if (animTimer >= FRAME_T) { animTimer = 0.f; animFrame++; }
    }
};

// ?? Gamakichi ????????????????????????????????????????????????
class Gamakichi : public Enemy {
public:
    static const int MAX_ROCKETS = 1; // stub for GameLevel.h compat
    static const int MAX_CHILDREN = 1; // stub for GameLevel.h compat
    static const int MAX_BOMBS = 10;

    struct RocketStub {
        bool alive = false;
        sf::FloatRect getHitbox() const { return {}; }
    };
    struct ChildStub {
        bool alive = false;
        sf::FloatRect getHitbox() const { return {}; }
    };

    RocketStub rockets[MAX_ROCKETS];
    ChildStub  children[MAX_CHILDREN];
    OrbBomb    bombs[MAX_BOMBS];

    sf::FloatRect getHitbox() const {
        return sf::FloatRect(
            sf::Vector2f(HEAD_X, HEAD_Y),
            sf::Vector2f(DISPLAY_W, DISPLAY_H));
    }

    void setPlayerPos(double px, double py) { playerX = px; playerY = py; }
    bool isDying() const { return dying; }

    void startDying() {
        if (dying || !alive) return;
        dying = true;
        dyingTimer = 0.f;
        flashTimer = 0.f;
        flashVis = true;
        attackState = AttackState::DYING;
        for (int i = 0; i < MAX_BOMBS; i++) bombs[i].alive = false;
    }

private:
    // ?? Layout ???????????????????????????????????????????????
    static constexpr float SCREEN_W = 800.f;
    static constexpr float SCREEN_H = 600.f;
    static constexpr float DISPLAY_W = 380.f;
    static constexpr float DISPLAY_H = 210.f;
    static constexpr float HEAD_X = (SCREEN_W - DISPLAY_W) / 2.f; // 210
    static constexpr float HEAD_Y = 110.f; // sits in upper-center, chin ~y=320

    // ?? Health ???????????????????????????????????????????????
    static constexpr int MAX_HEALTH = 30;
    static constexpr int PHASE2_HP = 15;

    // ?? Timing ???????????????????????????????????????????????
    static constexpr float HEAD_FRAME_TIME = 0.7f;
    static constexpr float HEAD_FRAME_TIME_P2 = 0.3f;
    static constexpr float EYE_FRAME_TIME = 0.09f;
    static constexpr float EYE_FRAME_TIME_P2 = 0.045f;
    static constexpr float IDLE_DURATION = 2.0f;
    static constexpr float IDLE_DURATION_P2 = 1.0f;
    static constexpr float FIRE_PAUSE = 0.3f;
    static constexpr float FOAM_DURATION = 2.8f;
    static constexpr float FOAM_DURATION_P2 = 1.8f;
    static constexpr float FOAM_SPEED = 150.f;
    static constexpr float FOAM_FRAME_TIME = 0.09f;
    static constexpr float BOMB_SPAWN_INTERVAL = 2.2f;
    static constexpr float BOMB_SPAWN_INTERVAL_P2 = 1.2f;
    static constexpr float FIRE_INTERVAL = 0.0f; // unused now
    static constexpr int   BOMBS_PER_ROUND = 2;
    static constexpr int   BOMBS_PER_ROUND_P2 = 3;

    // ?? Death ????????????????????????????????????????????????
    static constexpr float DYING_DURATION = 3.5f;
    static constexpr float FLASH_INTERVAL_DEATH = 0.1f;
    static constexpr float DEATH_EXPLOSION_INTERVAL = 0.22f;

    // ?? Sprite rects ?????????????????????????????????????????
    sf::IntRect head_closed{ {  16,    0}, {1240, 659} };
    sf::IntRect head_open{ {1340,    0}, {1210, 668} };

    sf::IntRect eye_frame1{ { 883,  703}, { 217, 373} };
    sf::IntRect eye_frame2{ {1195,  715}, { 235, 364} };
    sf::IntRect eye_frame3{ {1534,  700}, { 223, 382} };
    sf::IntRect eye_frame4{ {1864,  697}, { 214, 391} };
    sf::IntRect eye_frame5{ {2194,  703}, { 217, 391} };
    sf::IntRect eye_frame6{ {2518,  709}, { 232, 373} };

    sf::IntRect explosion_1{ {  34,  643}, { 274, 322} };
    sf::IntRect explosion_2{ { 337,  655}, { 343, 334} };

    sf::IntRect foam_1{ {2737,  688}, { 340, 229} };
    sf::IntRect foam_2{ {3073,  667}, { 334, 250} };
    sf::IntRect foam_3{ {3409,  616}, { 325, 319} };
    sf::IntRect foam_4{ {3733,  571}, { 327, 334} };

    // orb walking frames — row 3 (12 frames)
    sf::IntRect orb_1{ {  43, 1135}, {262, 358} };
    sf::IntRect orb_2{ { 322, 1162}, {247, 328} };
    sf::IntRect orb_3{ { 613, 1123}, {295, 355} };
    sf::IntRect orb_4{ { 913, 1174}, {223, 298} };
    sf::IntRect orb_5{ {1141, 1219}, {229, 262} };
    sf::IntRect orb_6{ {1378, 1180}, {283, 295} };
    sf::IntRect orb_7{ {1666, 1213}, {220, 271} };
    sf::IntRect orb_8{ {1885, 1168}, {316, 313} };
    sf::IntRect orb_9{ {2206, 1210}, {205, 286} };
    sf::IntRect orb_10{ {2416, 1204}, {253, 271} };
    sf::IntRect orb_11{ {2671, 1222}, {196, 280} };
    sf::IntRect orb_12{ {2866, 1162}, {340, 319} };

    // orb explosion frames — row 4 (7 frames)
    sf::IntRect orb_b1{ {  31, 1483}, {277, 234} };
    sf::IntRect orb_b2{ { 319, 1507}, {238, 210} };
    sf::IntRect orb_b3{ { 628, 1522}, {178, 195} };
    sf::IntRect orb_b4{ { 811, 1474}, {232, 243} };
    sf::IntRect orb_b5{ {1096, 1492}, {274, 225} };
    sf::IntRect orb_b6{ {1387, 1498}, {247, 219} };
    sf::IntRect orb_b7{ {1663, 1495}, {274, 222} };

    // ?? Textures & sprites ????????????????????????????????????
    sf::Texture          texture;
    bool                 textureLoaded = false;
    optional<sf::Sprite> headSprite;
    optional<sf::Sprite> eyeSprite;
    optional<sf::Sprite> orbSprite;
    optional<sf::Sprite> foamSprite;
    optional<sf::Sprite> explosionSprite;

    sf::RectangleShape healthBarBg;
    sf::RectangleShape healthBar;

    // ?? Foam wave ?????????????????????????????????????????????
    struct FoamWave {
        double x, y, vx;
        bool   alive;
        float  animTimer;
        int    animFrame;
        static constexpr float W = 80.f;
        static constexpr float H = 50.f;
        FoamWave() : x(0), y(0), vx(0), alive(false),
            animTimer(0.f), animFrame(0) {
        }
        sf::FloatRect getHitbox() const {
            return sf::FloatRect(sf::Vector2f((float)x, (float)y),
                sf::Vector2f(W, H));
        }
    };

    // ?? Explosion marker ??????????????????????????????????????
    struct Explosion {
        float x, y, timer;
        bool  alive;
        int   frame;
        bool  isBombBlast; // large radius blast circle
        Explosion() : x(0), y(0), timer(0.f),
            alive(false), frame(0), isBombBlast(false) {
        }
    };

    static const int MAX_FOAM = 4;
    static const int MAX_EXPLOSIONS = 16;

    FoamWave  foamWaves[MAX_FOAM];
    Explosion explosions[MAX_EXPLOSIONS];

    // ?? State machine ?????????????????????????????????????????
    enum class AttackState {
        IDLE, EYE_CHARGE, FIRE_PAUSE, SPAWNING_BOMBS, FOAM_ATTACK, DYING
    };

    AttackState attackState = AttackState::IDLE;
    float       stateTimer = 0.f;
    int         phase = 1;

    float headAnimTimer = 0.f;
    bool  mouthOpen = false;

    float eyeAnimTimer = 0.f;
    int   eyeFrame = 0;
    bool  eyeVisible = false;

    int   bombsSpawnedThisRound = 0;
    float bombSpawnCooldown = 0.f;

    bool  dying = false;
    float dyingTimer = 0.f;
    float flashTimer = 0.f;
    bool  flashVis = true;
    float deathExplosionTimer = 0.f;

    double playerX = 400.0;
    double playerY = 400.0;

    // ?? Platform ref for bombs ????????????????????????????????
    Platform* platformsRef = nullptr;
    int       platformCount = 0;

    // ?? Helpers ???????????????????????????????????????????????
    void spawnFoam(double startX, double velX) {
        for (int i = 0; i < MAX_FOAM; i++) {
            if (!foamWaves[i].alive) {
                foamWaves[i].x = startX;
                foamWaves[i].y = HEAD_Y + DISPLAY_H - FoamWave::H;
                foamWaves[i].vx = velX;
                foamWaves[i].alive = true;
                foamWaves[i].animTimer = 0.f;
                foamWaves[i].animFrame = 0;
                return;
            }
        }
    }

    void spawnExplosion(float ex, float ey, bool bombBlast = false) {
        for (int i = 0; i < MAX_EXPLOSIONS; i++) {
            if (!explosions[i].alive) {
                explosions[i].x = ex;
                explosions[i].y = ey;
                explosions[i].timer = 0.f;
                explosions[i].frame = 0;
                explosions[i].alive = true;
                explosions[i].isBombBlast = bombBlast;
                return;
            }
        }
    }

    sf::IntRect orbWalkFrame(int idx) const {
        sf::IntRect frames[12] = {
            orb_1,orb_2,orb_3,orb_4,orb_5,orb_6,
            orb_7,orb_8,orb_9,orb_10,orb_11,orb_12
        };
        return frames[idx % 12];
    }

    sf::IntRect orbExplodeFrame(int idx) const {
        sf::IntRect frames[7] = {
            orb_b1,orb_b2,orb_b3,orb_b4,orb_b5,orb_b6,orb_b7
        };
        return frames[idx % 7];
    }

    sf::IntRect foamFrameFor(int idx) const {
        sf::IntRect frames[4] = { foam_1, foam_2, foam_3, foam_4 };
        return frames[idx % 4];
    }

    sf::IntRect eyeFrameFor(int idx) const {
        sf::IntRect frames[6] = {
            eye_frame1,eye_frame2,eye_frame3,
            eye_frame4,eye_frame5,eye_frame6
        };
        return frames[idx % 6];
    }

public:
    Gamakichi(double startX, double startY)
        : Enemy(startX, startY, MAX_HEALTH, 0.0)
    {
        x = HEAD_X;
        y = HEAD_Y;

        healthBarBg.setSize({ 400.f, 22.f });
        healthBarBg.setFillColor(sf::Color(60, 0, 0));
        healthBarBg.setOutlineThickness(2.f);
        healthBarBg.setOutlineColor(sf::Color::Red);
        healthBar.setSize({ 400.f, 22.f });
        healthBar.setFillColor(sf::Color(220, 30, 30));
    }

    bool loadTexture(const string& path) {
        if (!texture.loadFromFile(path)) return false;
        headSprite.emplace(texture);
        eyeSprite.emplace(texture);
        orbSprite.emplace(texture);
        foamSprite.emplace(texture);
        explosionSprite.emplace(texture);
        textureLoaded = true;
        return true;
    }

    // call every frame from GameLevel so bombs know platforms
    void setPlatforms(Platform* plats, int count) {
        platformsRef = plats;
        platformCount = count;
    }

    // returns true if bomb i just exploded this frame (for damage check)
    bool isBombExploding(int i) const {
        return i >= 0 && i < MAX_BOMBS &&
            bombs[i].alive && bombs[i].exploding &&
            bombs[i].explodeTimer < 0.05f; // first tick only
    }

    // check if a rect is inside bomb i blast radius
    bool bombBlastHits(int i, sf::FloatRect target) const {
        if (i < 0 || i >= MAX_BOMBS) return false;
        if (!bombs[i].alive || !bombs[i].exploding) return false;
        return bombs[i].getBlastBox().findIntersection(target).has_value();
    }

    void update(double deltaTime) override {
        float dt = static_cast<float>(deltaTime);

        // ?? Death ?????????????????????????????????????????????
        if (dying) {
            dyingTimer += dt;
            flashTimer += dt;
            deathExplosionTimer += dt;

            if (flashTimer >= FLASH_INTERVAL_DEATH) {
                flashTimer = 0.f;
                flashVis = !flashVis;
            }
            if (deathExplosionTimer >= DEATH_EXPLOSION_INTERVAL) {
                deathExplosionTimer = 0.f;
                float ex = HEAD_X + (float)(rand() % (int)DISPLAY_W);
                float ey = HEAD_Y + (float)(rand() % (int)DISPLAY_H);
                spawnExplosion(ex, ey, false);
            }
            for (int i = 0; i < MAX_EXPLOSIONS; i++) {
                if (!explosions[i].alive) continue;
                explosions[i].timer += dt;
                if (explosions[i].timer >= 0.15f) {
                    explosions[i].timer = 0.f;
                    explosions[i].frame++;
                    if (explosions[i].frame >= 2) explosions[i].alive = false;
                }
            }
            if (dyingTimer >= DYING_DURATION) {
                alive = false;
                dying = false;
            }
            return;
        }

        if (!alive) return;

        // ?? Phase ?????????????????????????????????????????????
        if (health <= PHASE2_HP) phase = 2;

        // ?? Head idle anim ????????????????????????????????????
        headAnimTimer += dt;
        float hft = (phase == 2) ? HEAD_FRAME_TIME_P2 : HEAD_FRAME_TIME;
        if (headAnimTimer >= hft) {
            headAnimTimer = 0.f;
            mouthOpen = !mouthOpen;
        }

        // ?? Update bombs ??????????????????????????????????????
        for (int i = 0; i < MAX_BOMBS; i++) {
            if (!bombs[i].alive) continue;
            bombs[i].update(deltaTime,
                platformsRef ? platformsRef : nullptr,
                platformCount,
                SCREEN_W);

            // chain reaction: if this bomb just started exploding,
            // trigger nearby bombs too
            if (bombs[i].exploding && bombs[i].explodeTimer < dt * 2.f) {
                for (int j = 0; j < MAX_BOMBS; j++) {
                    if (j == i || !bombs[j].alive || bombs[j].exploding) continue;
                    sf::FloatRect blastBox = bombs[i].getBlastBox();
                    if (blastBox.findIntersection(bombs[j].getHitbox()).has_value())
                        bombs[j].startExplode();
                }
                spawnExplosion((float)bombs[i].x, (float)bombs[i].y, true);
            }
        }

        // ?? Update foam ???????????????????????????????????????
        for (int i = 0; i < MAX_FOAM; i++) {
            if (!foamWaves[i].alive) continue;
            foamWaves[i].x += foamWaves[i].vx * deltaTime;
            foamWaves[i].animTimer += dt;
            if (foamWaves[i].animTimer >= FOAM_FRAME_TIME) {
                foamWaves[i].animTimer = 0.f;
                foamWaves[i].animFrame = (foamWaves[i].animFrame + 1) % 4;
            }
            if (foamWaves[i].x < -120 || foamWaves[i].x > SCREEN_W + 120)
                foamWaves[i].alive = false;
        }

        // ?? Update explosions ?????????????????????????????????
        for (int i = 0; i < MAX_EXPLOSIONS; i++) {
            if (!explosions[i].alive) continue;
            explosions[i].timer += dt;
            if (explosions[i].timer >= 0.15f) {
                explosions[i].timer = 0.f;
                explosions[i].frame++;
                if (explosions[i].frame >= 2) explosions[i].alive = false;
            }
        }

        // ?? State machine ?????????????????????????????????????
        stateTimer += dt;

        switch (attackState) {

        case AttackState::IDLE:
            eyeVisible = false;
            {
                float idleDur = (phase == 2) ? IDLE_DURATION_P2 : IDLE_DURATION;
                if (stateTimer >= idleDur) {
                    stateTimer = 0.f;
                    eyeFrame = 0;
                    eyeAnimTimer = 0.f;
                    eyeVisible = true;
                    attackState = AttackState::EYE_CHARGE;
                }
            }
            break;

        case AttackState::EYE_CHARGE: {
            float fet = (phase == 2) ? EYE_FRAME_TIME_P2 : EYE_FRAME_TIME;
            eyeAnimTimer += dt;
            if (eyeAnimTimer >= fet) {
                eyeAnimTimer = 0.f;
                eyeFrame++;
                if (eyeFrame >= 6) {
                    eyeVisible = false;
                    stateTimer = 0.f;
                    attackState = AttackState::FIRE_PAUSE;
                }
            }
            break;
        }

        case AttackState::FIRE_PAUSE:
            if (stateTimer >= FIRE_PAUSE) {
                stateTimer = 0.f;
                bombsSpawnedThisRound = 0;
                bombSpawnCooldown = 0.f;
                attackState = AttackState::SPAWNING_BOMBS;
            }
            break;

        case AttackState::SPAWNING_BOMBS: {
            int bombsThisRound = (phase == 2) ? BOMBS_PER_ROUND_P2 : BOMBS_PER_ROUND;
            float spawnInterval = (phase == 2) ?
                BOMB_SPAWN_INTERVAL_P2 : BOMB_SPAWN_INTERVAL;

            bombSpawnCooldown -= dt;
            if (bombSpawnCooldown <= 0.f &&
                bombsSpawnedThisRound < bombsThisRound) {
                bombSpawnCooldown = 0.3f; // small gap between each spawn

                // find a free slot
                for (int i = 0; i < MAX_BOMBS; i++) {
                    if (!bombs[i].alive) {
                        double dir = (bombsSpawnedThisRound % 2 == 0) ? 1.0 : -1.0;
                        // drop from mouth center
                        bombs[i].spawn(
                            HEAD_X + DISPLAY_W * 0.5 - OrbBomb::W * 0.5,
                            HEAD_Y + DISPLAY_H,
                            dir);
                        bombsSpawnedThisRound++;
                        break;
                    }
                }
            }

            if (bombsSpawnedThisRound >= bombsThisRound &&
                bombSpawnCooldown <= 0.f) {
                stateTimer = 0.f;
                attackState = AttackState::FOAM_ATTACK;
                spawnFoam(HEAD_X + DISPLAY_W * 0.5, FOAM_SPEED);
                spawnFoam(HEAD_X + DISPLAY_W * 0.5, -FOAM_SPEED);
            }
            break;
        }

        case AttackState::FOAM_ATTACK: {
            float foamDur = (phase == 2) ? FOAM_DURATION_P2 : FOAM_DURATION;
            if (stateTimer >= foamDur) {
                stateTimer = 0.f;
                attackState = AttackState::IDLE;
                for (int i = 0; i < MAX_FOAM; i++) foamWaves[i].alive = false;
            }
            break;
        }

        case AttackState::DYING:
            break;
        }
    }

    void draw(sf::RenderWindow& window) override {
        if (!alive && !dying) return;

        // ?? Bombs ?????????????????????????????????????????????
        for (int i = 0; i < MAX_BOMBS; i++) {
            if (!bombs[i].alive) continue;

            bool flash = (bombs[i].fuseTimer >= OrbBomb::FUSE_TIME - OrbBomb::FLASH_START)
                && ((int)(bombs[i].fuseTimer * 10) % 2 == 0);

            if (textureLoaded && orbSprite) {
                sf::IntRect r = bombs[i].exploding
                    ? orbExplodeFrame(bombs[i].animFrame)
                    : orbWalkFrame(bombs[i].animFrame);
                orbSprite->setTextureRect(r);
                orbSprite->setScale({
                    OrbBomb::W / static_cast<float>(r.size.x),
                    OrbBomb::H / static_cast<float>(r.size.y)
                    });
                orbSprite->setPosition({ (float)bombs[i].x, (float)bombs[i].y });
                if (flash)
                    orbSprite->setColor(sf::Color(255, 100, 100, 255));
                else
                    orbSprite->setColor(sf::Color::White);
                window.draw(*orbSprite);
                orbSprite->setColor(sf::Color::White);
            }
            else {
                sf::RectangleShape r(sf::Vector2f(OrbBomb::W, OrbBomb::H));
                r.setPosition({ (float)bombs[i].x, (float)bombs[i].y });
                r.setFillColor(bombs[i].exploding ?
                    sf::Color(255, 140, 0) :
                    (flash ? sf::Color(255, 80, 80) : sf::Color(30, 30, 30)));
                window.draw(r);
            }

            // blast radius circle when exploding
            if (bombs[i].exploding) {
                sf::CircleShape blast(OrbBomb::EXPLODE_RADIUS);
                blast.setOrigin({ OrbBomb::EXPLODE_RADIUS, OrbBomb::EXPLODE_RADIUS });
                blast.setPosition({
                    (float)bombs[i].x + OrbBomb::W * 0.5f,
                    (float)bombs[i].y + OrbBomb::H * 0.5f
                    });
                blast.setFillColor(sf::Color(255, 140, 0, 80));
                blast.setOutlineColor(sf::Color(255, 200, 0, 180));
                blast.setOutlineThickness(2.f);
                window.draw(blast);
            }
        }

        // ?? Foam ??????????????????????????????????????????????
        for (int i = 0; i < MAX_FOAM; i++) {
            if (!foamWaves[i].alive) continue;
            if (textureLoaded && foamSprite) {
                sf::IntRect r = foamFrameFor(foamWaves[i].animFrame);
                foamSprite->setTextureRect(r);
                foamSprite->setScale({
                    FoamWave::W / static_cast<float>(r.size.x),
                    FoamWave::H / static_cast<float>(r.size.y)
                    });
                foamSprite->setPosition({ (float)foamWaves[i].x, (float)foamWaves[i].y });
                window.draw(*foamSprite);
            }
            else {
                sf::RectangleShape r(sf::Vector2f(FoamWave::W, FoamWave::H));
                r.setFillColor(sf::Color(200, 240, 255, 180));
                r.setPosition({ (float)foamWaves[i].x, (float)foamWaves[i].y });
                window.draw(r);
            }
        }

        // ?? Explosions ????????????????????????????????????????
        for (int i = 0; i < MAX_EXPLOSIONS; i++) {
            if (!explosions[i].alive) continue;
            if (textureLoaded && explosionSprite) {
                sf::IntRect r = (explosions[i].frame == 0) ?
                    explosion_1 : explosion_2;
                explosionSprite->setTextureRect(r);
                float sz = explosions[i].isBombBlast ? 100.f : 70.f;
                explosionSprite->setScale({
                    sz / static_cast<float>(r.size.x),
                    sz / static_cast<float>(r.size.y)
                    });
                explosionSprite->setPosition({ explosions[i].x, explosions[i].y });
                window.draw(*explosionSprite);
            }
        }

        // ?? Head ??????????????????????????????????????????????
        if (textureLoaded && headSprite) {
            sf::IntRect hr = mouthOpen ? head_open : head_closed;
            headSprite->setTextureRect(hr);
            headSprite->setScale({
                DISPLAY_W / static_cast<float>(hr.size.x),
                DISPLAY_H / static_cast<float>(hr.size.y)
                });
            headSprite->setPosition({ HEAD_X, HEAD_Y });
            if (dying && !flashVis)
                headSprite->setColor(sf::Color(255, 80, 80, 255));
            else
                headSprite->setColor(sf::Color::White);
            window.draw(*headSprite);
            headSprite->setColor(sf::Color::White);
        }
        else {
            sf::RectangleShape r(sf::Vector2f(DISPLAY_W, DISPLAY_H));
            r.setPosition({ HEAD_X, HEAD_Y });
            r.setFillColor(dying && !flashVis ?
                sf::Color(255, 80, 80) : sf::Color(180, 50, 50));
            window.draw(r);
        }

        // ?? Eye charge overlay ????????????????????????????????
        if (eyeVisible && textureLoaded && eyeSprite) {
            sf::IntRect er = eyeFrameFor(eyeFrame);
            eyeSprite->setTextureRect(er);
            eyeSprite->setScale({
                80.f / static_cast<float>(er.size.x),
                80.f / static_cast<float>(er.size.y)
                });
            eyeSprite->setPosition({
                HEAD_X + DISPLAY_W * 0.5f - 40.f,
                HEAD_Y + DISPLAY_H * 0.35f - 40.f
                });
            window.draw(*eyeSprite);
        }

        if (dying) return;

        // ?? Health bar ????????????????????????????????????????
        float barX = SCREEN_W * 0.5f - 200.f;
        float barY = HEAD_Y + DISPLAY_H + 6.f;
        healthBarBg.setPosition({ barX, barY });
        window.draw(healthBarBg);
        float pct = (float)health / (float)MAX_HEALTH;
        if (pct < 0.f) pct = 0.f;
        healthBar.setSize({ 400.f * pct, 22.f });
        healthBar.setPosition({ barX, barY });
        window.draw(healthBar);
    }
};