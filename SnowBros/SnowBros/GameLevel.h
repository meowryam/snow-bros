#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Botom.h"
#include "FlyingFoogaFoog.h"
#include "Tornado.h"
#include "Mogera.h"
#include "Gamakichi.h"
#include "Snowball.h"
#include "LevelLayout.h"
#include "LevelsManager.h"
#include "PlayerData.h"
#include "ScoreSystem.h"
#include "GemSystem.h"
#include "KeyBindings.h"
#include "EventBus.h"
#include "Entity.h"
using namespace std;

class GameLevel {
private:
    // platforms
    Platform platforms[LevelLayout::MAX_PLATFORMS];
    int      platformCount;

    // enemies — fixed arrays, no STL
    static const int MAX_ENEMIES = 16;
    Botom* botoms[MAX_ENEMIES];
    int              botomCount;
    FlyingFoogaFoog* foogas[MAX_ENEMIES];
    int              foogaCount;
    Tornado* tornados[MAX_ENEMIES];
    int              tornadoCount;
    Mogera* mogera;
    bool             hasMogera;
    Gamakichi* gamakichi;
    bool             hasGamakichi;

    // snowballs
    static const int MAX_SNOWBALLS = 8;
    Snowball* snowballs[MAX_SNOWBALLS];
    int       snowballCount;

    // level bg
    sf::Texture bgTexture;
    optional<sf::Sprite> bgSprite;
    bool        bgLoaded;

    // debug
    bool showHitboxes;

    // refs
    PlayerData& playerData;
    ScoreSystem& scoreSystem;
    GemSystem& gemSystem;
    EventBus& eventBus;
    KeyBindings& keyBindings;

    // asset path helper
    string assetPath;

    void clearEnemies() {
        for (int i = 0; i < botomCount; i++) { delete botoms[i];   botoms[i] = nullptr; }
        for (int i = 0; i < foogaCount; i++) { delete foogas[i];   foogas[i] = nullptr; }
        for (int i = 0; i < tornadoCount; i++) { delete tornados[i]; tornados[i] = nullptr; }
        if (hasMogera) { delete mogera;    mogera = nullptr;    hasMogera = false; }
        if (hasGamakichi) { delete gamakichi; gamakichi = nullptr; hasGamakichi = false; }
        botomCount = 0; foogaCount = 0; tornadoCount = 0;
    }

    void clearSnowballs() {
        for (int i = 0; i < snowballCount; i++) { delete snowballs[i]; snowballs[i] = nullptr; }
        snowballCount = 0;
    }

    void spawnSnowball(double x, double y, double dir, bool powerful, bool longRange) {
        if (snowballCount >= MAX_SNOWBALLS) return;
        Snowball* sb = new Snowball(x, y, dir);
        if (powerful)   sb->addSnow(); // start already powered
        if (longRange)  sb->setxspeed(dir * 600.0); // faster = longer range
        snowballs[snowballCount++] = sb;
    }

    void checkCollisions(Player& player1, Player* player2) {
        // Snowball vs enemy
        for (int si = 0; si < snowballCount; si++) {
            Snowball* sb = snowballs[si];
            if (!sb || !sb->getalive()) continue;
            sf::FloatRect sbHB(sf::Vector2f((float)sb->getx(), (float)sb->gety()), sf::Vector2f(24.f, 24.f));

            // vs botoms
            for (int ei = 0; ei < botomCount; ei++) {
                if (!botoms[ei]->getalive()) continue;
                if (!sbHB.findIntersection(botoms[ei]->getHitbox())) continue;
                if (sb->isRolling()) {
                    botoms[ei]->setalive(false);
                    scoreSystem.onBottomKilled();
                    gemSystem.enemykilled();
                    eventBus.post(GameEvent::ENEMY_KILLED);
                }
                else if (!botoms[ei]->gettrap()) {
                    botoms[ei]->receiveSnowballHit();
                    sb->setalive(false);
                }
            }

            // vs foogas
            for (int ei = 0; ei < foogaCount; ei++) {
                if (!foogas[ei]->getalive()) continue;
                if (!sbHB.findIntersection(foogas[ei]->getHitbox())) continue;
                if (sb->isRolling()) {
                    foogas[ei]->setalive(false);
                    scoreSystem.onFoogaKilled();
                    gemSystem.enemykilled();
                    eventBus.post(GameEvent::ENEMY_KILLED);
                }
                else {
                    foogas[ei]->receiveSnowballHit();
                    sb->setalive(false);
                }
            }

            // vs tornados
            for (int ei = 0; ei < tornadoCount; ei++) {
                if (!tornados[ei]->getalive()) continue;
                if (!sbHB.findIntersection(tornados[ei]->getHitbox())) continue;
                if (sb->isRolling()) {
                    tornados[ei]->setalive(false);
                    scoreSystem.onTornadoKilled();
                    gemSystem.enemykilled();
                    eventBus.post(GameEvent::ENEMY_KILLED);
                }
                else {
                    tornados[ei]->receiveSnowballHit();
                    sb->setalive(false);
                }
            }

            // vs mogera
            if (hasMogera && mogera->getalive()) {
                if (sbHB.findIntersection(mogera->getHitbox())) {
                    mogera->healthreduce(1);
                    sb->setalive(false);
                    if (mogera->gethealth() <= 0) {
                        mogera->setalive(false);
                        scoreSystem.onMogeraKilled();
                        gemSystem.Mogerakilled();
                        eventBus.post(GameEvent::ENEMY_KILLED);
                    }
                }
            }

            // vs gamakichi
            if (hasGamakichi && gamakichi->getalive()) {
                if (sbHB.findIntersection(gamakichi->getHitbox())) {
                    gamakichi->healthreduce(1);
                    sb->setalive(false);
                    if (gamakichi->gethealth() <= 0) {
                        gamakichi->setalive(false);
                        scoreSystem.onGamakichiKilled();
                        gemSystem.gamakichikilled();
                        eventBus.post(GameEvent::ENEMY_KILLED);
                    }
                }
            }
        }

        // Player vs trapped enemy — kick to start rolling
        auto checkKick = [&](Player& p) {
            if (!p.getIsAlive()) return;
            sf::FloatRect phb = p.getHitbox();
            for (int ei = 0; ei < botomCount; ei++) {
                if (!botoms[ei]->getalive()) continue;
               if (botoms[ei]->gettrap() && !botoms[ei]->isRolling()) {
                    if (phb.findIntersection(botoms[ei]->getHitbox())) {
                        double dir = (p.getPosition().x < botoms[ei]->getx()) ? 1.0 : -1.0;
                        botoms[ei]->startRolling(dir);
                    }
                }
            }
            };
        checkKick(player1);
        if (player2) checkKick(*player2);

        // Player vs enemy — take damage
        auto checkDamage = [&](Player& p) {
            if (!p.getIsAlive() || p.isInvincible()) return;
            sf::FloatRect phb = p.getHitbox();

            for (int ei = 0; ei < botomCount; ei++) {
                if (!botoms[ei]->getalive() || botoms[ei]->gettrap() )  continue;
                if (phb.findIntersection(botoms[ei]->getHitbox())) { p.takeDamage(); return; }
            }
            for (int ei = 0; ei < foogaCount; ei++) {
                if (!foogas[ei]->getalive() || foogas[ei]->gettrap()) continue;
                if (phb.findIntersection(foogas[ei]->getHitbox())) { p.takeDamage(); return; }
            }
            for (int ei = 0; ei < tornadoCount; ei++) {
                if (!tornados[ei]->getalive() || tornados[ei]->gettrap()) continue;
                if (phb.findIntersection(tornados[ei]->getHitbox())) { p.takeDamage(); return; }
                // knife hits
                for (int k = 0; k < Tornado::MAX_KNIVES; k++) {
                    if (tornados[ei]->knives[k].alive &&
                        phb.findIntersection(tornados[ei]->knives[k].getHitbox())) {
                        p.takeDamage(); return;
                    }
                }
            }
            if (hasMogera && mogera->getalive()) {
                for (int c = 0; c < Mogera::MAX_CHILDREN; c++) {
                    if (mogera->children[c].alive &&
                        phb.findIntersection(mogera->children[c].getHitbox())) {
                        p.takeDamage(); return;
                    }
                }
            }
            if (hasGamakichi && gamakichi->getalive()) {
                for (int r = 0; r < Gamakichi::MAX_ROCKETS; r++) {
                    if (gamakichi->rockets[r].alive &&
                        phb.findIntersection(gamakichi->rockets[r].getHitbox())) {
                        p.takeDamage(); return;
                    }
                }
                for (int c = 0; c < Gamakichi::MAX_CHILDREN; c++) {
                    if (gamakichi->children[c].alive &&
                        phb.findIntersection(gamakichi->children[c].getHitbox())) {
                        p.takeDamage(); return;
                    }
                }
            }
            };
        checkDamage(player1);
        if (player2) checkDamage(*player2);
    }
    /*
    bool allEnemiesDead() {
        for (int i = 0; i < botomCount; i++)   if (botoms[i]->getalive())   return false;
        for (int i = 0; i < foogaCount; i++)   if (foogas[i]->getalive())   return false;
        for (int i = 0; i < tornadoCount; i++) if (tornados[i]->getalive()) return false;
        if (hasMogera && mogera->getalive())    return false;
        if (hasGamakichi && gamakichi->getalive()) return false;
        return true;
    }
    */
    bool allEnemiesDead() {
        for (int i = 0; i < botomCount; i++)   if (botoms[i]->getalive())   return false;
        for (int i = 0; i < foogaCount; i++)   if (foogas[i]->getalive())   return false;
        for (int i = 0; i < tornadoCount; i++) if (tornados[i]->getalive()) return false;
        if (hasMogera && mogera->getalive())    return false;
        if (hasGamakichi && gamakichi->getalive()) return false;
        return true;
    }


public:
    bool levelComplete;

    GameLevel(PlayerData& pd, ScoreSystem& sc, GemSystem& gm,
        EventBus& eb, KeyBindings& kb, const string& assetsPath)
        : playerData(pd), scoreSystem(sc), gemSystem(gm),
        eventBus(eb), keyBindings(kb), assetPath(assetsPath),
        botomCount(0), foogaCount(0), tornadoCount(0),
        hasMogera(false), hasGamakichi(false),
        snowballCount(0), bgLoaded(false),
        showHitboxes(false), platformCount(0),
        levelComplete(false),
        mogera(nullptr), gamakichi(nullptr)
    {
        for (int i = 0; i < MAX_ENEMIES; i++) { botoms[i] = nullptr; foogas[i] = nullptr; tornados[i] = nullptr; }
        for (int i = 0; i < MAX_SNOWBALLS; i++) snowballs[i] = nullptr;
    }

    ~GameLevel() { clearEnemies(); clearSnowballs(); }

    void loadLevel(const Levelblueprint& lvl) {
        clearEnemies();
        clearSnowballs();
        levelComplete = false;

        // Load platform layout
        platformCount = LevelLayout::getLayout(lvl.getLevelno(), platforms);


        // Load background
        //string bgPath = assetPath + "images\\lvl" + to_string(lvl.getLevelno()) + ".png";
        /*bgLoaded = bgTexture.loadFromFile(bgPath);
        if (bgLoaded) {
            bgSprite->setTexture(bgTexture);
            // Scale to fit 800x600
            sf::Vector2u ts = bgTexture.getSize();
            bgSprite->setScale({ 800.f / ts.x, 600.f / ts.y });
        }
        */
        // Load background image for this level
// File naming convention: assets\images\lvl1.png, lvl2.png, etc.
        string bgPath = assetPath + "images\\Level" + to_string(lvl.getLevelno()) + "_bg.png";
        bgLoaded = bgTexture.loadFromFile(bgPath);
        if (bgLoaded) {
            // In SFML 3, Sprite is constructed WITH the texture — not set after
            bgSprite.emplace(bgTexture);
            // Scale whatever image size Gemini/ChatGPT gave us to fit 800x600
            sf::Vector2u ts = bgTexture.getSize();
            bgSprite->setScale({
                800.f / static_cast<float>(ts.x),
                600.f / static_cast<float>(ts.y)
                });
        }


        float speedMult = lvl.getEnemyspeedIncreaser();

        // Spawn enemies based on blueprint
        if (lvl.isBosslevel()) {
            if (lvl.getLevelno() == 5) {
                mogera = new Mogera(295, 420);
              //  mogera->loadTexture(assetPath + "images\\Mogera.png");
                hasMogera = true;
            }
            else {
                gamakichi = new Gamakichi(220, 380);
              //  gamakichi->loadTexture(assetPath + "images\\Gamakichi.png");
                hasGamakichi = true;
            }
        }
        else {
            // Spawn botoms
            for (int i = 0; i < lvl.getBottomcount() && botomCount < MAX_ENEMIES; i++) {
                float sx = 50.f + i * 140.f;
                Botom* b = new Botom(sx, 200);
             //   b->loadTexture(assetPath + "images\\Botom_Blue.png");
                b->setxspeed(80.0 * speedMult * ((i % 2 == 0) ? 1 : -1));
                botoms[botomCount++] = b;
            }
            // Spawn foogas
            for (int i = 0; i < lvl.getFoogacount() && foogaCount < MAX_ENEMIES; i++) {
                float sx = 100.f + i * 160.f;
                FlyingFoogaFoog* f = new FlyingFoogaFoog(sx, 150);
            //    f->loadTexture(assetPath + "images\\FlyingFoogaFoog_Blue.png");
                foogas[foogaCount++] = f;
            }
            // Spawn tornados
            for (int i = 0; i < lvl.getTornadocount() && tornadoCount < MAX_ENEMIES; i++) {
                float sx = 200.f + i * 200.f;
                Tornado* t = new Tornado(sx, 100);
               // t->loadTexture(assetPath + "images\\Tornado_Blue.png");
                tornados[tornadoCount++] = t;
            }
        }
    }

    void update(double deltaTime, Player& player1, Player* player2 = nullptr) {
        if (levelComplete) return;

        // Toggle hitboxes
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F1)) showHitboxes = true;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F2)) showHitboxes = false;

        // Handle player input
        player1.handleInput();
        player1.update(deltaTime);
        player1.resolvePlatforms(platforms, platformCount);

        // Spawn snowball if requested
        if (player1.getWantsToThrow()) {
            bool powerful = player1.isSnowballPowerActive();
            bool longRange = player1.isDistanceIncreaseActive();
            double dir = player1.getFacing() == Direction::RIGHT ? 1.0 : -1.0;
            spawnSnowball(player1.getPosition().x + (dir > 0 ? 48 : 0), player1.getPosition().y + 20, dir, powerful, longRange);
            scoreSystem.resetChain();
        }

        if (player2) {
            // player2 uses arrow keys by default
            player2->update(deltaTime);
            player2->resolvePlatforms(platforms, platformCount);
        }

        // Update snowballs
        for (int i = 0; i < snowballCount; i++) {
            if (!snowballs[i] || !snowballs[i]->getalive()) continue;
            snowballs[i]->update(deltaTime);
            snowballs[i]->resolvePlatforms(platforms, platformCount);
        }

        // Update enemies
        for (int i = 0; i < botomCount; i++) {
            if (!botoms[i]->getalive()) continue;
            botoms[i]->update(deltaTime);
            botoms[i]->resolvePlatforms(platforms, platformCount);
        }
        for (int i = 0; i < foogaCount; i++) {
            if (foogas[i]->getalive()) foogas[i]->update(deltaTime);
        }
        for (int i = 0; i < tornadoCount; i++) {
            if (!tornados[i]->getalive()) continue;
            tornados[i]->setPlayerPos(player1.getPosition().x, player1.getPosition().y);
            tornados[i]->update(deltaTime);
        }
        if (hasMogera && mogera->getalive())    mogera->update(deltaTime);
        if (hasGamakichi && gamakichi->getalive()) {
            gamakichi->setPlayerPos(player1.getPosition().x, player1.getPosition().y);
            gamakichi->update(deltaTime);
        }

        checkCollisions(player1, player2);

        // Handle player death
        if (!player1.getIsAlive()) {
            playerData.setLives(playerData.getLives() - 1);
            if (playerData.getLives() > 0) {
                player1.resetForNewLevel(Vector2f(100.f, 200.f));
            }
        }

       // if (allEnemiesDead()) levelComplete = true;
        if (allEnemiesDead() && (botomCount + foogaCount + tornadoCount > 0 || hasMogera || hasGamakichi))
            levelComplete = true;

        

       eventBus.clear();
    }

    void draw(sf::RenderWindow& window, Player& player1, Player* player2 = nullptr) {
        // Background
        if (bgLoaded) window.draw(*bgSprite);
        else { window.clear(sf::Color(20, 20, 60)); }

        // Platforms
        for (int i = 0; i < platformCount; i++) platforms[i].draw(window, showHitboxes);

        // Enemies
        for (int i = 0; i < botomCount; i++)   botoms[i]->draw(window);
        for (int i = 0; i < foogaCount; i++)   foogas[i]->draw(window);
        for (int i = 0; i < tornadoCount; i++) tornados[i]->draw(window);
        if (hasMogera)    mogera->draw(window);
        if (hasGamakichi) gamakichi->draw(window);

        // Snowballs
        for (int i = 0; i < snowballCount; i++) {
            if (snowballs[i] && snowballs[i]->getalive())
                snowballs[i]->draw(window);
        }

        // Players
        player1.draw(window);
        if (player2) player2->draw(window);

        // Hitbox debug
        if (showHitboxes) {
            auto drawHB = [&](sf::FloatRect hb, sf::Color c) {
                sf::RectangleShape r(hb.size);
                r.setPosition(hb.position);
                r.setFillColor(sf::Color::Transparent);
                r.setOutlineThickness(1.f);
                r.setOutlineColor(c);
                window.draw(r);
                };
            drawHB(player1.getHitbox(), sf::Color::Green);
            for (int i = 0; i < botomCount; i++)
                if (botoms[i]->getalive()) drawHB(botoms[i]->getHitbox(), sf::Color::Red);
        }
    }
};