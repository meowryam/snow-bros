#pragma once
#include "Enemy.h"
#include <SFML/Graphics.hpp>
#include <optional>
#include <string>
using namespace std;

class Gamakichi : public Enemy {
public:
    static const int MAX_ROCKETS = 8;  // keeping for GameLevel.h compatibility

    // ?? Hitbox for collision with player ?????????????????????
    sf::FloatRect getHitbox() const {
        return sf::FloatRect(
            sf::Vector2f((float)x, (float)y),
            sf::Vector2f(DISPLAY_W, DISPLAY_H)
        );
    }

    void setPlayerPos(double px, double py) { playerX = px; playerY = py; }

private:
    // ?? Display sizes ?????????????????????????????????????????
    static constexpr float DISPLAY_W = 420.f;   // head width on screen
    static constexpr float DISPLAY_H = 230.f;   // head height on screen
    static constexpr float SCREEN_W = 800.f;
    static constexpr float SCREEN_H = 600.f;
    static constexpr float HEAD_X = (SCREEN_W - DISPLAY_W) / 2.f;  // centered
    static constexpr float HEAD_Y = -40.f;   // slightly off top edge, looming

    // ?? Health / phase ????????????????????????????????????????
    static constexpr int   MAX_HEALTH = 50;
    static constexpr int   PHASE2_HP = 25;

    // ?? Timing constants ?????????????????????????????????????
    static constexpr float HEAD_FRAME_TIME = 0.6f;   // idle breathe speed
    static constexpr float EYE_FRAME_TIME = 0.08f;  // eye charge speed
    static constexpr float EYE_FRAME_TIME_P2 = 0.04f;  // faster in phase 2
    static constexpr float ORB_SPEED = 180.f;
    static constexpr float ORB_SPEED_P2 = 280.f;
    static constexpr float FOAM_SPEED = 140.f;
    static constexpr float FOAM_FRAME_TIME = 0.1f;

    // attack cycle timings (seconds)
    static constexpr float IDLE_DURATION = 1.8f;
    static constexpr float EYE_DURATION = 6 * 0.08f; // 6 frames * frame time
    static constexpr float FIRE_PAUSE = 0.4f;
    static constexpr float FOAM_DURATION = 2.5f;

    // ?? Sprite rects ?? Head ?????????????????????????????????
    sf::IntRect head_closed{ {  16,    0}, {1240, 659} };
    sf::IntRect head_open{ {1340,    0}, {1210, 668} };

    // ?? Sprite rects ?? Eye charge (6 frames) ????????????????
    sf::IntRect eye_frame1{ { 883,  703}, { 217, 373} };
    sf::IntRect eye_frame2{ {1195,  715}, { 235, 364} };
    sf::IntRect eye_frame3{ {1534,  700}, { 223, 382} };
    sf::IntRect eye_frame4{ {1864,  697}, { 214, 391} };
    sf::IntRect eye_frame5{ {2194,  703}, { 217, 391} };
    sf::IntRect eye_frame6{ {2518,  709}, { 232, 373} };

    // ?? Sprite rects ?? Explosions ????????????????????????????
    sf::IntRect explosion_1{ {  34,  643}, { 274, 322} };
    sf::IntRect explosion_2{ { 337,  655}, { 343, 334} };

    // ?? Sprite rects ?? Foam (4 frames) ??????????????????????
    sf::IntRect foam_1{ {2737,  688}, { 340, 229} };
    sf::IntRect foam_2{ {3073,  667}, { 334, 250} };
    sf::IntRect foam_3{ {3409,  616}, { 325, 319} };
    sf::IntRect foam_4{ {3733,  571}, { 327, 334} };

    // ?? Sprite rects ?? Orbs row 3 (12 frames, phase 1) ??????
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

    // ?? Sprite rects ?? Orbs row 4 (7 frames, phase 2) ???????
    sf::IntRect orb_b1{ {  31, 1483}, {277, 234} };
    sf::IntRect orb_b2{ { 319, 1507}, {238, 210} };
    sf::IntRect orb_b3{ { 628, 1522}, {178, 195} };
    sf::IntRect orb_b4{ { 811, 1474}, {232, 243} };
    sf::IntRect orb_b5{ {1096, 1492}, {274, 225} };
    sf::IntRect orb_b6{ {1387, 1498}, {247, 219} };
    sf::IntRect orb_b7{ {1663, 1495}, {274, 222} };

    // ?? Orb projectile ????????????????????????????????????????
    struct GamakichiOrb {
        double x, y, vx, vy;
        bool   alive;
        bool   isPhase2Type;  // false = row3 frames, true = row4 frames
        float  animTimer;
        int    animFrame;

        GamakichiOrb() : x(0), y(0), vx(0), vy(0),
            alive(false), isPhase2Type(false),
            animTimer(0.f), animFrame(0) {
        }

        static constexpr float ORB_DISPLAY = 48.f;
        static constexpr float FRAME_T = 0.06f;

        sf::FloatRect getHitbox() const {
            return sf::FloatRect(
                sf::Vector2f((float)x, (float)y),
                sf::Vector2f(ORB_DISPLAY, ORB_DISPLAY)
            );
        }
    };

    // ?? Foam wave ?????????????????????????????????????????????
    struct FoamWave {
        double x, y, vx;
        bool   alive;
        float  animTimer;
        int    animFrame;
        static constexpr float FOAM_DISPLAY_W = 80.f;
        static constexpr float FOAM_DISPLAY_H = 50.f;

        FoamWave() : x(0), y(0), vx(0), alive(false),
            animTimer(0.f), animFrame(0) {
        }

        sf::FloatRect getHitbox() const {
            return sf::FloatRect(
                sf::Vector2f((float)x, (float)y),
                sf::Vector2f(FOAM_DISPLAY_W, FOAM_DISPLAY_H)
            );
        }
    };

    // ?? Explosion marker ??????????????????????????????????????
    struct Explosion {
        float x, y;
        float timer;
        bool  alive;
        int   frame;
        Explosion() : x(0), y(0), timer(0.f), alive(false), frame(0) {}
    };

    // ?? Fixed arrays (no STL) ?????????????????????????????????
    static const int MAX_ORBS = 12;
    static const int MAX_FOAM = 4;
    static const int MAX_EXPLOSIONS = 8;

    GamakichiOrb orbs[MAX_ORBS];
    FoamWave     foamWaves[MAX_FOAM];
    Explosion    explosions[MAX_EXPLOSIONS];

    // ?? Texture & sprites ?????????????????????????????????????
    sf::Texture           texture;
    bool                  textureLoaded = false;
    optional<sf::Sprite>  headSprite;
    optional<sf::Sprite>  eyeSprite;
    optional<sf::Sprite>  orbSprite;
    optional<sf::Sprite>  foamSprite;
    optional<sf::Sprite>  explosionSprite;

    // ?? Health bar ????????????????????????????????????????????
    sf::RectangleShape healthBarBg;
    sf::RectangleShape healthBar;

    // ?? State machine ?????????????????????????????????????????
    enum class AttackState {
        IDLE,
        EYE_CHARGE,
        FIRE_PAUSE,
        FIRING,
        FOAM_ATTACK,
        DYING
    };

    AttackState attackState = AttackState::IDLE;
    float       stateTimer = 0.f;
    int         phase = 1;

    // ?? Head animation ????????????????????????????????????????
    float headAnimTimer = 0.f;
    bool  mouthOpen = false;

    // ?? Eye charge animation ??????????????????????????????????
    float eyeAnimTimer = 0.f;
    int   eyeFrame = 0;
    bool  eyeVisible = false;

    // ?? Firing state ??????????????????????????????????????????
    int   orbsFiredThisRound = 0;
    float fireCooldown = 0.f;
    static constexpr float FIRE_INTERVAL = 0.22f;
    static constexpr int   ORBS_PER_ROUND = 5;
    static constexpr int   ORBS_PER_ROUND_P2 = 8;

    // ?? Death ?????????????????????????????????????????????????
    bool  dying = false;
    float dyingTimer = 0.f;
    float flashTimer = 0.f;
    bool  flashVis = true;
    static constexpr float DYING_DURATION = 3.5f;
    static constexpr float FLASH_INTERVAL = 0.1f;
    float deathExplosionTimer = 0.f;
    static constexpr float DEATH_EXPLOSION_INTERVAL = 0.25f;

    // ?? Player target ?????????????????????????????????????????
    double playerX = 400.0;
    double playerY = 300.0;

    // ?? Helpers ???????????????????????????????????????????????
    void spawnOrb(double vx, double vy, bool p2type) {
        for (int i = 0; i < MAX_ORBS; i++) {
            if (!orbs[i].alive) {
                orbs[i].x = HEAD_X + DISPLAY_W * 0.5 - GamakichiOrb::ORB_DISPLAY * 0.5;
                orbs[i].y = HEAD_Y + DISPLAY_H - 10.0;
                orbs[i].vx = vx;
                orbs[i].vy = vy;
                orbs[i].alive = true;
                orbs[i].isPhase2Type = p2type;
                orbs[i].animTimer = 0.f;
                orbs[i].animFrame = 0;
                return;
            }
        }
    }

    void spawnFoam(double startX, double velX) {
        for (int i = 0; i < MAX_FOAM; i++) {
            if (!foamWaves[i].alive) {
                foamWaves[i].x = startX;
                foamWaves[i].y = SCREEN_H - 40.f - FoamWave::FOAM_DISPLAY_H;
                foamWaves[i].vx = velX;
                foamWaves[i].alive = true;
                foamWaves[i].animTimer = 0.f;
                foamWaves[i].animFrame = 0;
                return;
            }
        }
    }

    void spawnExplosion(float ex, float ey) {
        for (int i = 0; i < MAX_EXPLOSIONS; i++) {
            if (!explosions[i].alive) {
                explosions[i].x = ex;
                explosions[i].y = ey;
                explosions[i].timer = 0.f;
                explosions[i].frame = 0;
                explosions[i].alive = true;
                return;
            }
        }
    }

    sf::IntRect orbFrameFor(int idx, bool p2) const {
        if (!p2) {
            sf::IntRect frames[12] = {
                orb_1, orb_2, orb_3, orb_4, orb_5, orb_6,
                orb_7, orb_8, orb_9, orb_10, orb_11, orb_12
            };
            return frames[idx % 12];
        }
        else {
            sf::IntRect frames[7] = {
                orb_b1, orb_b2, orb_b3, orb_b4, orb_b5, orb_b6, orb_b7
            };
            return frames[idx % 7];
        }
    }

    sf::IntRect foamFrameFor(int idx) const {
        sf::IntRect frames[4] = { foam_1, foam_2, foam_3, foam_4 };
        return frames[idx % 4];
    }

    sf::IntRect eyeFrameFor(int idx) const {
        sf::IntRect frames[6] = {
            eye_frame1, eye_frame2, eye_frame3,
            eye_frame4, eye_frame5, eye_frame6
        };
        return frames[idx % 6];
    }

public:
    // keep MAX_ROCKETS stub so GameLevel.h compiles without changes
    struct RocketStub { bool alive = false; sf::FloatRect getHitbox() const { return {}; } };
    RocketStub rockets[MAX_ROCKETS];

    // children array kept for GameLevel.h collision loop compatibility
    static const int MAX_CHILDREN = 1;
    struct ChildStub { bool alive = false; sf::FloatRect getHitbox() const { return {}; } };
    ChildStub children[MAX_CHILDREN];

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

    bool isDying() const { return dying; }

    void startDying() {
        if (dying || !alive) return;
        dying = true;
        dyingTimer = 0.f;
        flashTimer = 0.f;
        flashVis = true;
        attackState = AttackState::DYING;
        // kill all projectiles
        for (int i = 0; i < MAX_ORBS; i++) orbs[i].alive = false;
        for (int i = 0; i < MAX_FOAM; i++) foamWaves[i].alive = false;
    }

    void update(double deltaTime) override {
        float dt = static_cast<float>(deltaTime);

        // ?? Death animation ???????????????????????????????????
        if (dying) {
            dyingTimer += dt;
            flashTimer += dt;
            deathExplosionTimer += dt;

            if (flashTimer >= FLASH_INTERVAL) {
                flashTimer = 0.f;
                flashVis = !flashVis;
            }
            if (deathExplosionTimer >= DEATH_EXPLOSION_INTERVAL) {
                deathExplosionTimer = 0.f;
                float ex = HEAD_X + (rand() % (int)DISPLAY_W);
                float ey = HEAD_Y + (rand() % (int)DISPLAY_H);
                spawnExplosion(ex, ey);
            }
            // update explosions
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

        // ?? Phase check ???????????????????????????????????????
        if (health <= PHASE2_HP) phase = 2;

        // ?? Head idle animation ???????????????????????????????
        headAnimTimer += dt;
        float headFrameTime = (phase == 2) ? HEAD_FRAME_TIME * 0.5f : HEAD_FRAME_TIME;
        if (headAnimTimer >= headFrameTime) {
            headAnimTimer = 0.f;
            mouthOpen = !mouthOpen;
        }

        // ?? Update orbs ???????????????????????????????????????
        for (int i = 0; i < MAX_ORBS; i++) {
            if (!orbs[i].alive) continue;
            orbs[i].x += orbs[i].vx * deltaTime;
            orbs[i].y += orbs[i].vy * deltaTime;

            orbs[i].animTimer += dt;
            if (orbs[i].animTimer >= GamakichiOrb::FRAME_T) {
                orbs[i].animTimer = 0.f;
                orbs[i].animFrame++;
            }
            // kill if off screen or hits floor
            if (orbs[i].y > SCREEN_H - 40.f) {
                spawnExplosion((float)orbs[i].x, (float)orbs[i].y);
                orbs[i].alive = false;
            }
            if (orbs[i].x < -60 || orbs[i].x > SCREEN_W + 60) orbs[i].alive = false;
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
            if (foamWaves[i].x < -100 || foamWaves[i].x > SCREEN_W + 100)
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

        // ?? Attack state machine ??????????????????????????????
        stateTimer += dt;

        switch (attackState) {

        case AttackState::IDLE:
            eyeVisible = false;
            if (stateTimer >= IDLE_DURATION) {
                stateTimer = 0.f;
                eyeFrame = 0;
                eyeAnimTimer = 0.f;
                eyeVisible = true;
                attackState = AttackState::EYE_CHARGE;
            }
            break;

        case AttackState::EYE_CHARGE: {
            float frameTime = (phase == 2) ? EYE_FRAME_TIME_P2 : EYE_FRAME_TIME;
            eyeAnimTimer += dt;
            if (eyeAnimTimer >= frameTime) {
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
                orbsFiredThisRound = 0;
                fireCooldown = 0.f;
                attackState = AttackState::FIRING;
            }
            break;

        case AttackState::FIRING: {
            fireCooldown -= dt;
            int orbsThisRound = (phase == 2) ? ORBS_PER_ROUND_P2 : ORBS_PER_ROUND;
            float speed = (phase == 2) ? ORB_SPEED_P2 : ORB_SPEED;

            if (fireCooldown <= 0.f && orbsFiredThisRound < orbsThisRound) {
                fireCooldown = FIRE_INTERVAL;

                // spread shot — divide 160 degree arc evenly
                double cx = HEAD_X + DISPLAY_W * 0.5;
                double cy = HEAD_Y + DISPLAY_H;
                double dx = playerX - cx;
                double dy = playerY - cy;
                double len = sqrt(dx * dx + dy * dy);
                if (len < 1.0) len = 1.0;
                dx /= len; dy /= len;

                // fire one orb slightly offset from center aim each shot
                // alternate left/right spread
                double spreadAngle = (orbsFiredThisRound % 2 == 0)
                    ? (orbsFiredThisRound / 2) * 18.0
                    : -(orbsFiredThisRound / 2) * 18.0;
                double rad = spreadAngle * 3.14159265 / 180.0;
                double nx = dx * cos(rad) - dy * sin(rad);
                double ny = dx * sin(rad) + dy * cos(rad);

                spawnOrb(nx * speed, ny * speed, phase == 2);
                orbsFiredThisRound++;
            }

            if (orbsFiredThisRound >= orbsThisRound && fireCooldown <= 0.f) {
                stateTimer = 0.f;
                attackState = AttackState::FOAM_ATTACK;
                // spawn two foam waves from center going left and right
                spawnFoam(HEAD_X + DISPLAY_W * 0.5, FOAM_SPEED);
                spawnFoam(HEAD_X + DISPLAY_W * 0.5, -FOAM_SPEED);
            }
            break;
        }

        case AttackState::FOAM_ATTACK:
            if (stateTimer >= FOAM_DURATION) {
                stateTimer = 0.f;
                attackState = AttackState::IDLE;
                // kill any remaining foam
                for (int i = 0; i < MAX_FOAM; i++) foamWaves[i].alive = false;
            }
            break;

        case AttackState::DYING:
            break;
        }
    }

    void draw(sf::RenderWindow& window) override {
        if (!alive && !dying) return;

        // ?? Draw orbs ?????????????????????????????????????????
        for (int i = 0; i < MAX_ORBS; i++) {
            if (!orbs[i].alive) continue;
            if (textureLoaded && orbSprite) {
                sf::IntRect r = orbFrameFor(orbs[i].animFrame, orbs[i].isPhase2Type);
                orbSprite->setTextureRect(r);
                float sc = GamakichiOrb::ORB_DISPLAY / static_cast<float>(r.size.x);
                float sy = GamakichiOrb::ORB_DISPLAY / static_cast<float>(r.size.y);
                orbSprite->setScale({ sc, sy });
                orbSprite->setPosition({ (float)orbs[i].x, (float)orbs[i].y });
                window.draw(*orbSprite);
            }
            else {
                sf::CircleShape c(GamakichiOrb::ORB_DISPLAY * 0.5f);
                c.setFillColor(orbs[i].isPhase2Type ?
                    sf::Color(220, 60, 60) : sf::Color(60, 60, 220));
                c.setPosition({ (float)orbs[i].x, (float)orbs[i].y });
                window.draw(c);
            }
        }

        // ?? Draw foam ?????????????????????????????????????????
        for (int i = 0; i < MAX_FOAM; i++) {
            if (!foamWaves[i].alive) continue;
            if (textureLoaded && foamSprite) {
                sf::IntRect r = foamFrameFor(foamWaves[i].animFrame);
                foamSprite->setTextureRect(r);
                foamSprite->setScale({
                    FoamWave::FOAM_DISPLAY_W / static_cast<float>(r.size.x),
                    FoamWave::FOAM_DISPLAY_H / static_cast<float>(r.size.y)
                    });
                foamSprite->setPosition({ (float)foamWaves[i].x, (float)foamWaves[i].y });
                window.draw(*foamSprite);
            }
            else {
                sf::RectangleShape r(sf::Vector2f(FoamWave::FOAM_DISPLAY_W, FoamWave::FOAM_DISPLAY_H));
                r.setFillColor(sf::Color(100, 200, 255, 180));
                r.setPosition({ (float)foamWaves[i].x, (float)foamWaves[i].y });
                window.draw(r);
            }
        }

        // ?? Draw explosions ???????????????????????????????????
        for (int i = 0; i < MAX_EXPLOSIONS; i++) {
            if (!explosions[i].alive) continue;
            if (textureLoaded && explosionSprite) {
                sf::IntRect r = (explosions[i].frame == 0) ? explosion_1 : explosion_2;
                explosionSprite->setTextureRect(r);
                explosionSprite->setScale({
                    80.f / static_cast<float>(r.size.x),
                    80.f / static_cast<float>(r.size.y)
                    });
                explosionSprite->setPosition({ explosions[i].x, explosions[i].y });
                window.draw(*explosionSprite);
            }
        }

        // ?? Draw head ?????????????????????????????????????????
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

        // ?? Draw eye charge overlay ???????????????????????????
        if (eyeVisible && textureLoaded && eyeSprite) {
            sf::IntRect er = eyeFrameFor(eyeFrame);
            eyeSprite->setTextureRect(er);
            eyeSprite->setScale({
                80.f / static_cast<float>(er.size.x),
                80.f / static_cast<float>(er.size.y)
                });
            // position eye at center of head
            eyeSprite->setPosition({
                HEAD_X + DISPLAY_W * 0.5f - 40.f,
                HEAD_Y + DISPLAY_H * 0.4f - 40.f
                });
            window.draw(*eyeSprite);
        }

        if (dying) return; // no health bar while dying

        // ?? Health bar ????????????????????????????????????????
        float barX = SCREEN_W * 0.5f - 200.f;
        float barY = HEAD_Y + DISPLAY_H + 8.f;
        healthBarBg.setPosition({ barX, barY });
        window.draw(healthBarBg);
        float pct = (float)health / (float)MAX_HEALTH;
        if (pct < 0.f) pct = 0.f;
        healthBar.setSize({ 400.f * pct, 22.f });
        healthBar.setPosition({ barX, barY });
        window.draw(healthBar);
    }
};