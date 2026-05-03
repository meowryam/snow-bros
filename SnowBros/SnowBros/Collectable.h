#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"

enum class CollectableType {
    GEM,
    FOOD,
    POWERUP,
    CASH_BUNDLE
};

class Collectable {
protected:
    double x, y;
    bool alive;

    float bobTimer = 0.f;
    float bobOffset = 0.f;

    float lifetime;
    static constexpr float DEFAULT_LIFETIME = 8.f;

    sf::Sprite         sprite;
    bool               hasSprite = false;

    // Fallback rectangle (used if texture not loaded)
    sf::RectangleShape shape;

public:
    Collectable(double spawnX, double spawnY, float life = DEFAULT_LIFETIME)
        : x(spawnX), y(spawnY), alive(true), lifetime(life),
        sprite(getDummyTexture())   // SFML 3: Sprite requires a texture at construction
    {
    }

    virtual ~Collectable() = default;

    bool checkCollect(Player& player) {
        if (!alive) return false;
        sf::FloatRect myBox(sf::Vector2f((float)x, (float)y), getSize());
        if (myBox.findIntersection(player.getHitbox())) {
            onCollect(player);
            alive = false;
            return true;
        }
        return false;
    }

    virtual void update(double deltaTime) {
        if (!alive) return;
        bobTimer += (float)deltaTime;
        bobOffset = std::sin(bobTimer * 4.f) * 4.f;
        lifetime -= (float)deltaTime;
        if (lifetime <= 0.f) alive = false;
    }

    virtual void draw(sf::RenderWindow& window) {
        if (!alive) return;
        sf::Vector2f drawPos((float)x, (float)y + bobOffset);
        if (hasSprite) {
            sprite.setPosition(drawPos);
            window.draw(sprite);
        }
        else {
            shape.setPosition(drawPos);
            window.draw(shape);
        }
    }

    bool   isAlive() const { return alive; }
    double getX()    const { return x; }
    double getY()    const { return y; }

    // Type query — no dynamic_cast needed in GameLevel
    virtual CollectableType getType()       const = 0;

    // Point value for food items; 0 for non-food
    virtual int             getPointValue() const { return 0; }

protected:
    virtual sf::Vector2f getSize()          const = 0;
    virtual void         onCollect(Player&) = 0;

    // Sets sprite from a texture + rect, scales it to targetSize
    void initSprite(sf::Texture& tex, sf::IntRect rect, sf::Vector2f targetSize) {
        sprite.setTexture(tex);
        sprite.setTextureRect(rect);
        sf::Vector2f frameSize((float)rect.size.x, (float)rect.size.y);
        sprite.setScale({ targetSize.x / frameSize.x, targetSize.y / frameSize.y });
        hasSprite = true;
    }

private:
    // SFML 3 requires a texture in the Sprite constructor.
    // Returns a reference to a persistent 1x1 dummy so the
    // base constructor compiles cleanly before subclasses set the real texture.
    static sf::Texture& getDummyTexture() {
        static sf::Texture dummy;
        static bool made = false;
        if (!made) {
            sf::Image img({ 1, 1 }, sf::Color::Transparent);
            dummy.loadFromImage(img);
            made = true;
        }
        return dummy;
    }
};


// ============================================================
//  GemCollectable
//  Sprite: face_pink_border  { {309, 639}, {179, 185} }
// ============================================================
class GemCollectable : public Collectable {
private:
    static sf::Texture sharedTex;
    static bool        texLoaded;
public:
    static bool loadSharedTexture(const std::string& path) {
        if (texLoaded) return true;
        texLoaded = sharedTex.loadFromFile(path);
        return texLoaded;
    }

    GemCollectable(double spawnX, double spawnY)
        : Collectable(spawnX, spawnY, 6.f)
    {
        if (texLoaded) {
            initSprite(sharedTex, sf::IntRect({ 309, 639 }, { 179, 185 }), { 24.f, 24.f });
        }
        else {
            shape.setSize({ 14.f, 14.f });
            shape.setFillColor(sf::Color(80, 200, 255));
            shape.setOutlineColor(sf::Color::White);
            shape.setOutlineThickness(1.f);
        }
    }

    CollectableType getType() const override { return CollectableType::GEM; }

protected:
    sf::Vector2f getSize()          const override { return { 24.f, 24.f }; }
    void         onCollect(Player&)       override {}  // GameLevel handles via getType()
};
inline sf::Texture GemCollectable::sharedTex;
inline bool        GemCollectable::texLoaded = false;


// ============================================================
//  ── FOOD COLLECTABLES ────────────────────────────────────
//
//  All food items use the same shared texture (Items.png).
//  onCollect is intentionally empty — GameLevel awards score
//  via getType() == FOOD and getPointValue().
// ============================================================

// ============================================================
//  FoodTexture  -  one shared texture load for all 6 food classes
// ============================================================
struct FoodTexture {
    static sf::Texture tex;
    static bool        loaded;
    static bool load(const std::string& path) {
        if (loaded) return true;
        loaded = tex.loadFromFile(path);
        return loaded;
    }
};
inline sf::Texture FoodTexture::tex;
inline bool        FoodTexture::loaded = false;


// ============================================================
//  FoodSushiGold  -  sushi_gold_bowl  { {14, 41}, {147, 153} }
//  300 points
// ============================================================
class FoodSushiGold : public Collectable {
public:
    static bool loadSharedTexture(const std::string& path) { return FoodTexture::load(path); }

    FoodSushiGold(double spawnX, double spawnY)
        : Collectable(spawnX, spawnY, 8.f)
    {
        if (FoodTexture::loaded) {
            initSprite(FoodTexture::tex, sf::IntRect({ 14, 41 }, { 147, 153 }), { 28.f, 28.f });
        }
        else {
            shape.setSize({ 28.f, 28.f });
            shape.setFillColor(sf::Color(220, 180, 60));
            shape.setOutlineColor(sf::Color::White);
            shape.setOutlineThickness(1.f);
        }
    }

    CollectableType getType()       const override { return CollectableType::FOOD; }
    int             getPointValue() const override { return 300; }

protected:
    sf::Vector2f getSize()          const override { return { 28.f, 28.f }; }
    void         onCollect(Player&)       override {}
};


// ============================================================
//  FoodSushiRedStrips  -  sushi_red_strips  { {166, 49}, {160, 145} }
//  200 points
// ============================================================
class FoodSushiRedStrips : public Collectable {
public:
    static bool loadSharedTexture(const std::string& path) { return FoodTexture::load(path); }

    FoodSushiRedStrips(double spawnX, double spawnY)
        : Collectable(spawnX, spawnY, 8.f)
    {
        if (FoodTexture::loaded) {
            initSprite(FoodTexture::tex, sf::IntRect({ 166, 49 }, { 160, 145 }), { 28.f, 24.f });
        }
        else {
            shape.setSize({ 28.f, 24.f });
            shape.setFillColor(sf::Color(220, 80, 80));
            shape.setOutlineColor(sf::Color::White);
            shape.setOutlineThickness(1.f);
        }
    }

    CollectableType getType()       const override { return CollectableType::FOOD; }
    int             getPointValue() const override { return 200; }

protected:
    sf::Vector2f getSize()          const override { return { 28.f, 24.f }; }
    void         onCollect(Player&)       override {}
};


// ============================================================
//  FoodSushiDarkBowl  -  sushi_dark_bowl  { {17, 203}, {136, 137} }
//  250 points
// ============================================================
class FoodSushiDarkBowl : public Collectable {
public:
    static bool loadSharedTexture(const std::string& path) { return FoodTexture::load(path); }

    FoodSushiDarkBowl(double spawnX, double spawnY)
        : Collectable(spawnX, spawnY, 8.f)
    {
        if (FoodTexture::loaded) {
            initSprite(FoodTexture::tex, sf::IntRect({ 17, 203 }, { 136, 137 }), { 26.f, 24.f });
        }
        else {
            shape.setSize({ 26.f, 24.f });
            shape.setFillColor(sf::Color(80, 60, 40));
            shape.setOutlineColor(sf::Color::White);
            shape.setOutlineThickness(1.f);
        }
    }

    CollectableType getType()       const override { return CollectableType::FOOD; }
    int             getPointValue() const override { return 250; }

protected:
    sf::Vector2f getSize()          const override { return { 26.f, 24.f }; }
    void         onCollect(Player&)       override {}
};


// ============================================================
//  FoodSushiRedFish  -  sushi_red_fish  { {142, 211}, {207, 142} }
//  350 points
// ============================================================
class FoodSushiRedFish : public Collectable {
public:
    static bool loadSharedTexture(const std::string& path) { return FoodTexture::load(path); }

    FoodSushiRedFish(double spawnX, double spawnY)
        : Collectable(spawnX, spawnY, 8.f)
    {
        if (FoodTexture::loaded) {
            initSprite(FoodTexture::tex, sf::IntRect({ 142, 211 }, { 207, 142 }), { 30.f, 22.f });
        }
        else {
            shape.setSize({ 30.f, 22.f });
            shape.setFillColor(sf::Color(200, 60, 60));
            shape.setOutlineColor(sf::Color::White);
            shape.setOutlineThickness(1.f);
        }
    }

    CollectableType getType()       const override { return CollectableType::FOOD; }
    int             getPointValue() const override { return 350; }

protected:
    sf::Vector2f getSize()          const override { return { 30.f, 22.f }; }
    void         onCollect(Player&)       override {}
};


// ============================================================
//  FoodSushiRedArc  -  sushi_red_arc  { {19, 368}, {140, 131} }
//  200 points
// ============================================================
class FoodSushiRedArc : public Collectable {
public:
    static bool loadSharedTexture(const std::string& path) { return FoodTexture::load(path); }

    FoodSushiRedArc(double spawnX, double spawnY)
        : Collectable(spawnX, spawnY, 8.f)
    {
        if (FoodTexture::loaded) {
            initSprite(FoodTexture::tex, sf::IntRect({ 19, 368 }, { 140, 131 }), { 28.f, 22.f });
        }
        else {
            shape.setSize({ 28.f, 22.f });
            shape.setFillColor(sf::Color(190, 50, 50));
            shape.setOutlineColor(sf::Color::White);
            shape.setOutlineThickness(1.f);
        }
    }

    CollectableType getType()       const override { return CollectableType::FOOD; }
    int             getPointValue() const override { return 200; }

protected:
    sf::Vector2f getSize()          const override { return { 28.f, 22.f }; }
    void         onCollect(Player&)       override {}
};


// ============================================================
//  FoodSushiWhitePair  -  sushi_white_pair  { {166, 389}, {155, 126} }
//  400 points
// ============================================================
class FoodSushiWhitePair : public Collectable {
public:
    static bool loadSharedTexture(const std::string& path) { return FoodTexture::load(path); }

    FoodSushiWhitePair(double spawnX, double spawnY)
        : Collectable(spawnX, spawnY, 8.f)
    {
        if (FoodTexture::loaded) {
            initSprite(FoodTexture::tex, sf::IntRect({ 166, 389 }, { 155, 126 }), { 30.f, 22.f });
        }
        else {
            shape.setSize({ 30.f, 22.f });
            shape.setFillColor(sf::Color(230, 230, 230));
            shape.setOutlineColor(sf::Color::White);
            shape.setOutlineThickness(1.f);
        }
    }

    CollectableType getType()       const override { return CollectableType::FOOD; }
    int             getPointValue() const override { return 400; }

protected:
    sf::Vector2f getSize()          const override { return { 30.f, 22.f }; }
    void         onCollect(Player&)       override {}
};


// ============================================================
//  SpeedBoostPickup  -  spec 8.2: +50% speed for 15 seconds
//  Sprite: potion_red_full  { {325, 17}, {140, 161} }
// ============================================================
class SpeedBoostPickup : public Collectable {
private:
    static sf::Texture sharedTex;
    static bool        texLoaded;
public:
    static bool loadSharedTexture(const std::string& path) {
        if (texLoaded) return true;
        texLoaded = sharedTex.loadFromFile(path);
        return texLoaded;
    }

    SpeedBoostPickup(double spawnX, double spawnY)
        : Collectable(spawnX, spawnY)
    {
        if (texLoaded) {
            initSprite(sharedTex, sf::IntRect({ 325, 17 }, { 140, 161 }), { 20.f, 24.f });
        }
        else {
            shape.setSize({ 18.f, 18.f });
            shape.setFillColor(sf::Color(255, 165, 0));
            shape.setOutlineColor(sf::Color::Yellow);
            shape.setOutlineThickness(1.f);
        }
    }

    CollectableType getType() const override { return CollectableType::POWERUP; }

protected:
    sf::Vector2f getSize()          const override { return { 20.f, 24.f }; }
    void         onCollect(Player& player) override { player.activateSpeedBoost(15.f); }
};
inline sf::Texture SpeedBoostPickup::sharedTex;
inline bool        SpeedBoostPickup::texLoaded = false;


// ============================================================
//  SnowballPowerPickup  -  spec 8.2: 1-hit encase, until level end
//  Sprite: potion_blue_full  { {325, 166}, {137, 166} }
// ============================================================
class SnowballPowerPickup : public Collectable {
private:
    static sf::Texture sharedTex;
    static bool        texLoaded;
public:
    static bool loadSharedTexture(const std::string& path) {
        if (texLoaded) return true;
        texLoaded = sharedTex.loadFromFile(path);
        return texLoaded;
    }

    SnowballPowerPickup(double spawnX, double spawnY)
        : Collectable(spawnX, spawnY)
    {
        if (texLoaded) {
            initSprite(sharedTex, sf::IntRect({ 325, 166 }, { 137, 166 }), { 20.f, 24.f });
        }
        else {
            shape.setSize({ 18.f, 18.f });
            shape.setFillColor(sf::Color(200, 230, 255));
            shape.setOutlineColor(sf::Color(100, 180, 255));
            shape.setOutlineThickness(1.f);
        }
    }

    CollectableType getType() const override { return CollectableType::POWERUP; }

protected:
    sf::Vector2f getSize()          const override { return { 20.f, 24.f }; }
    void         onCollect(Player& player) override { player.activateSnowballPower(); }
};
inline sf::Texture SnowballPowerPickup::sharedTex;
inline bool        SnowballPowerPickup::texLoaded = false;


// ============================================================
//  DistanceIncreasePickup  -  spec 8.2: full-width throw, until level end
//  Sprite: potion_yellow_full  { {333, 326}, {126, 163} }
// ============================================================
class DistanceIncreasePickup : public Collectable {
private:
    static sf::Texture sharedTex;
    static bool        texLoaded;
public:
    static bool loadSharedTexture(const std::string& path) {
        if (texLoaded) return true;
        texLoaded = sharedTex.loadFromFile(path);
        return texLoaded;
    }

    DistanceIncreasePickup(double spawnX, double spawnY)
        : Collectable(spawnX, spawnY)
    {
        if (texLoaded) {
            initSprite(sharedTex, sf::IntRect({ 333, 326 }, { 126, 163 }), { 20.f, 24.f });
        }
        else {
            shape.setSize({ 18.f, 18.f });
            shape.setFillColor(sf::Color(150, 255, 150));
            shape.setOutlineColor(sf::Color(50, 200, 50));
            shape.setOutlineThickness(1.f);
        }
    }

    CollectableType getType() const override { return CollectableType::POWERUP; }

protected:
    sf::Vector2f getSize()          const override { return { 20.f, 24.f }; }
    void         onCollect(Player& player) override { player.activateDistanceIncrease(); }
};
inline sf::Texture DistanceIncreasePickup::sharedTex;
inline bool        DistanceIncreasePickup::texLoaded = false;


// ============================================================
//  BalloonModePickup  -  spec 8.2: float upward for 10 seconds
//  Sprite: potion_green_full  { {349, 485}, {118, 163} }
// ============================================================
class BalloonModePickup : public Collectable {
private:
    static sf::Texture sharedTex;
    static bool        texLoaded;
public:
    static bool loadSharedTexture(const std::string& path) {
        if (texLoaded) return true;
        texLoaded = sharedTex.loadFromFile(path);
        return texLoaded;
    }

    BalloonModePickup(double spawnX, double spawnY)
        : Collectable(spawnX, spawnY)
    {
        if (texLoaded) {
            initSprite(sharedTex, sf::IntRect({ 349, 485 }, { 118, 163 }), { 20.f, 24.f });
        }
        else {
            shape.setSize({ 18.f, 18.f });
            shape.setFillColor(sf::Color(255, 100, 200));
            shape.setOutlineColor(sf::Color(200, 50, 150));
            shape.setOutlineThickness(1.f);
        }
    }

    CollectableType getType() const override { return CollectableType::POWERUP; }

protected:
    sf::Vector2f getSize()          const override { return { 20.f, 24.f }; }
    void         onCollect(Player& player) override { player.activateBalloonMode(10.f); }
};
inline sf::Texture BalloonModePickup::sharedTex;
inline bool        BalloonModePickup::texLoaded = false;


// ============================================================
//  ExtraLifePickup  -  spec 8.2 / 7.3: +1 life
//  Sprite: potion_red_select  { {309, 1262}, {177, 171} }
// ============================================================
class ExtraLifePickup : public Collectable {
private:
    static sf::Texture sharedTex;
    static bool        texLoaded;
public:
    static bool loadSharedTexture(const std::string& path) {
        if (texLoaded) return true;
        texLoaded = sharedTex.loadFromFile(path);
        return texLoaded;
    }

    ExtraLifePickup(double spawnX, double spawnY)
        : Collectable(spawnX, spawnY)
    {
        if (texLoaded) {
            initSprite(sharedTex, sf::IntRect({ 309, 1262 }, { 177, 171 }), { 24.f, 24.f });
        }
        else {
            shape.setSize({ 18.f, 18.f });
            shape.setFillColor(sf::Color(255, 80, 80));
            shape.setOutlineColor(sf::Color(200, 20, 20));
            shape.setOutlineThickness(1.f);
        }
    }

    CollectableType getType() const override { return CollectableType::POWERUP; }

protected:
    sf::Vector2f getSize()          const override { return { 24.f, 24.f }; }
    void         onCollect(Player& player) override { player.collectLife(); }
};
inline sf::Texture ExtraLifePickup::sharedTex;
inline bool        ExtraLifePickup::texLoaded = false;


// ============================================================
//  BonusCashBundle  -  spec 9.2 / 9.3: falls from top of screen
//  Used for both bonus-level rain AND boss-kill rain.
//  Sprite: scroll_book  { {17, 974}, {256, 345} }
//
//  Score + gems awarded in GameLevel via getType() == CASH_BUNDLE
//  so that scoreSystem.onBonusBundleCollected() is called there.
// ============================================================
class BonusCashBundle : public Collectable {
private:
    static sf::Texture sharedTex;
    static bool        texLoaded;
    double fallSpeed = 180.0;
public:
    static bool loadSharedTexture(const std::string& path) {
        if (texLoaded) return true;
        texLoaded = sharedTex.loadFromFile(path);
        return texLoaded;
    }

    BonusCashBundle(double spawnX)
        : Collectable(spawnX, -20.0, 12.f)
    {
        if (texLoaded) {
            initSprite(sharedTex, sf::IntRect({ 17, 974 }, { 256, 345 }), { 20.f, 24.f });
        }
        else {
            shape.setSize({ 20.f, 16.f });
            shape.setFillColor(sf::Color(255, 215, 0));
            shape.setOutlineColor(sf::Color(200, 160, 0));
            shape.setOutlineThickness(1.f);
        }
    }

    CollectableType getType() const override { return CollectableType::CASH_BUNDLE; }

    void update(double deltaTime) override {
        if (!alive) return;
        y += fallSpeed * deltaTime;
        if (y > 640.0) alive = false;
        lifetime -= (float)deltaTime;
        if (lifetime <= 0.f) alive = false;
    }

    void draw(sf::RenderWindow& window) override {
        if (!alive) return;
        sf::Vector2f drawPos((float)x, (float)y);
        if (hasSprite) {
            sprite.setPosition(drawPos);
            window.draw(sprite);
        }
        else {
            shape.setPosition(drawPos);
            window.draw(shape);
        }
    }

protected:
    sf::Vector2f getSize()          const override { return { 20.f, 24.f }; }
    void         onCollect(Player&)       override {}  // GameLevel handles via getType()
};
inline sf::Texture BonusCashBundle::sharedTex;
inline bool        BonusCashBundle::texLoaded = false;