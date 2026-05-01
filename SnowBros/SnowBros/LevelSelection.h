#pragma once
#include <SFML/Graphics.hpp>
#include "PlayerData.h"
#include <string>
#include <optional>
using namespace std;

// ============================================================
//  LevelSelectScreen
//  - Shows 10 level medallions in a 5x2 grid
//  - Unlocked  : level <= playerData.getCurrentLevel()
//  - Locked    : greyed out, click/enter has no effect
//  - Returns   : selectedLevel (1-10) when confirmed, or 0 if back
// ============================================================
class LevelSelectScreen {
public:
    explicit LevelSelectScreen(PlayerData& data) : playerData(data) {}
    LevelSelectScreen(const LevelSelectScreen&) = delete;
    LevelSelectScreen& operator=(const LevelSelectScreen&) = delete;

    // Call once after construction — returns false if font missing
    bool loadAssets(const string& fontPath,
        const string& bgPath = "assets\\images\\LevelSelect_bg.png");

    void reset();
    void handleEvent(const sf::Event& event);
    void draw(sf::RenderWindow& window);

    // After draw() : 0 = still open, -1 = back/ESC, 1-10 = level chosen
    int  result = 0;
    bool done = false;

private:
    // ── constants ──────────────────────────────────────────────
    static constexpr int   TOTAL_LEVELS = 10;
    static constexpr int   COLS = 5;
    static constexpr int   ROWS = 2;

    // Medallion geometry — tuned to the generated background image
    // Row 1 centres ~y=390, Row 2 centres ~y=535  (800x600 window)
    static constexpr float MED_R = 68.f;   // radius of each medallion circle
    static constexpr float MED_GAP_X = 10.f;   // horizontal gap between circles
    // total row width = 5*(2*68) + 4*10 = 720 → left edge = (800-720)/2 = 40
    static constexpr float GRID_LEFT = 40.f + MED_R;   // centre x of first col
    static constexpr float ROW1_Y = 382.f;
    static constexpr float ROW2_Y = 528.f;

    static constexpr float TITLE_Y = 52.f;
    static constexpr float SUBTITLE_Y = 82.f;

    // ── state ──────────────────────────────────────────────────
    PlayerData& playerData;
    sf::Font    font;
    bool        layoutReady = false;
    int         hoveredIndex = 0;   // keyboard cursor (0-based)

    // ── assets ─────────────────────────────────────────────────
    sf::Texture              bgTex;
    bool                     bgLoaded = false;
    optional<sf::Sprite>     bgSprite;

    // Per-medallion shapes & text
    sf::CircleShape          outerRing[TOTAL_LEVELS];
    sf::CircleShape          innerCircle[TOTAL_LEVELS];
    optional<sf::Text>       txtNumber[TOTAL_LEVELS];
    optional<sf::Text>       txtLock[TOTAL_LEVELS];    // "🔒" substitute: "[X]"

    // UI text
    optional<sf::Text>       txtTitle;
    optional<sf::Text>       txtSubtitle;
    optional<sf::Text>       txtBack;
    optional<sf::Text>       txtMessage;

    sf::RectangleShape       backBtn;
    sf::RectangleShape       titleBar;

    float                    messageTimer = 0.f;
    bool                     showMessage = false;

    // ── palette ────────────────────────────────────────────────
    const sf::Color COL_BG_FALLBACK{ 5,  18,  42, 255 };
    const sf::Color COL_TITLE{ 180, 230, 255, 255 };

    // Unlocked medallion
    const sf::Color COL_OUTER_UNLOCK{ 80,  190, 255, 230 };
    const sf::Color COL_INNER_UNLOCK{ 10,  50,  110, 220 };
    const sf::Color COL_NUM_UNLOCK{ 210, 245, 255, 255 };
    // Hovered
    const sf::Color COL_OUTER_HOVER{ 150, 230, 255, 255 };
    const sf::Color COL_INNER_HOVER{ 20,  80,  170, 240 };
    const sf::Color COL_NUM_HOVER{ 255, 255, 255, 255 };
    // Locked medallion
    const sf::Color COL_OUTER_LOCK{ 30,  50,  80,  160 };
    const sf::Color COL_INNER_LOCK{ 10,  20,  40,  180 };
    const sf::Color COL_NUM_LOCK{ 70,  90,  110, 160 };
    // Back button
    const sf::Color COL_BACK_BG{ 10,  30,  60,  215 };
    const sf::Color COL_BACK_OUTLINE{ 120, 220, 255, 210 };
    const sf::Color COL_BACK_TXT{ 220, 245, 255, 255 };
    // Message
    const sf::Color COL_MSG_ERR{ 220, 80,  80,  255 };

    // ── helpers ────────────────────────────────────────────────
    sf::Text& TX(optional<sf::Text>& t) { return t.value(); }
    sf::Vector2f medalCentre(int i) const;
    bool isUnlocked(int i) const;   // i is 0-based
    void setupLayout();
    void refreshMedallion(int i);
    void confirmSelection(int i);
    void postMessage(const string& msg);
    void handleClick(sf::Vector2f mp);
};

// ============================================================
//  Implementation  (header-only, mirrors ShopScreen pattern)
// ============================================================

inline sf::Vector2f LevelSelectScreen::medalCentre(int i) const {
    int col = i % COLS;
    int row = i / COLS;
    float x = GRID_LEFT + col * (2.f * MED_R + MED_GAP_X);
    float y = (row == 0) ? ROW1_Y : ROW2_Y;
    return { x, y };
}

inline bool LevelSelectScreen::isUnlocked(int i) const {
    // i is 0-based; level number is i+1
    return (i + 1) <= playerData.getCurrentLevel();
}

inline bool LevelSelectScreen::loadAssets(const string& fontPath,
    const string& bgPath) {
    if (!font.openFromFile(fontPath)) return false;

    bgLoaded = bgTex.loadFromFile(bgPath);
    if (bgLoaded) {
        bgSprite.emplace(bgTex);
        sf::Vector2u ts = bgTex.getSize();
        bgSprite->setScale({
            800.f / static_cast<float>(ts.x),
            600.f / static_cast<float>(ts.y)
            });
    }

    // Allocate all text objects (SFML 3: must pass font to constructor)
    txtTitle.emplace(font, "SELECT  LEVEL", 16u);
    txtSubtitle.emplace(font, "Choose your destination", 7u);
    txtBack.emplace(font, "BACK  [ESC]", 8u);
    txtMessage.emplace(font, "", 8u);

    for (int i = 0; i < TOTAL_LEVELS; i++) {
        txtNumber[i].emplace(font, to_string(i + 1), 18u);
        txtLock[i].emplace(font, "LOCKED", 5u);
    }

    return true;
}

inline void LevelSelectScreen::reset() {
    hoveredIndex = 0;
    showMessage = false;
    messageTimer = 0.f;
    result = 0;
    done = false;
    layoutReady = false;
}

inline void LevelSelectScreen::setupLayout() {
    // Title bar
    titleBar.setSize({ 800.f, 50.f });
    titleBar.setPosition({ 0.f, 38.f });
    titleBar.setFillColor(sf::Color(5, 15, 35, 170));

    TX(txtTitle).setFillColor(COL_TITLE);
    TX(txtTitle).setStyle(sf::Text::Bold);
    {
        auto lb = TX(txtTitle).getLocalBounds();
        TX(txtTitle).setPosition({ 400.f - lb.size.x * 0.5f, TITLE_Y });
    }

    TX(txtSubtitle).setFillColor(sf::Color(140, 200, 240, 200));
    {
        auto lb = TX(txtSubtitle).getLocalBounds();
        TX(txtSubtitle).setPosition({ 400.f - lb.size.x * 0.5f, SUBTITLE_Y });
    }

    // Back button — bottom centre
    backBtn.setSize({ 140.f, 32.f });
    backBtn.setPosition({ 330.f, 558.f });
    backBtn.setFillColor(COL_BACK_BG);
    backBtn.setOutlineColor(COL_BACK_OUTLINE);
    backBtn.setOutlineThickness(1.5f);
    TX(txtBack).setFillColor(COL_BACK_TXT);
    {
        auto lb = TX(txtBack).getLocalBounds();
        TX(txtBack).setPosition({
            330.f + (140.f - lb.size.x) * 0.5f,
            558.f + (32.f - lb.size.y) * 0.5f - 2.f
            });
    }

    TX(txtMessage).setPosition({ 400.f, 526.f });

    // Medallions
    for (int i = 0; i < TOTAL_LEVELS; i++) {
        sf::Vector2f c = medalCentre(i);

        outerRing[i].setRadius(MED_R);
        outerRing[i].setOrigin({ MED_R, MED_R });
        outerRing[i].setPosition(c);
        outerRing[i].setOutlineThickness(3.f);

        float innerR = MED_R - 7.f;
        innerCircle[i].setRadius(innerR);
        innerCircle[i].setOrigin({ innerR, innerR });
        innerCircle[i].setPosition(c);

        // Number — centred inside medallion
        TX(txtNumber[i]).setStyle(sf::Text::Bold);
        {
            auto lb = TX(txtNumber[i]).getLocalBounds();
            TX(txtNumber[i]).setPosition({
                c.x - lb.size.x * 0.5f - lb.position.x,
                c.y - lb.size.y * 0.5f - lb.position.y
                });
        }

        // "LOCKED" text — just below number
        {
            auto lb = TX(txtLock[i]).getLocalBounds();
            TX(txtLock[i]).setPosition({
                c.x - lb.size.x * 0.5f - lb.position.x,
                c.y + 20.f
                });
            TX(txtLock[i]).setFillColor(COL_NUM_LOCK);
        }

        refreshMedallion(i);
    }

    layoutReady = true;
}

inline void LevelSelectScreen::refreshMedallion(int i) {
    bool unlocked = isUnlocked(i);
    bool hovered = (i == hoveredIndex);

    if (!unlocked) {
        outerRing[i].setFillColor(COL_OUTER_LOCK);
        outerRing[i].setOutlineColor(COL_OUTER_LOCK);
        innerCircle[i].setFillColor(COL_INNER_LOCK);
        TX(txtNumber[i]).setFillColor(COL_NUM_LOCK);
    }
    else if (hovered) {
        outerRing[i].setFillColor(COL_OUTER_HOVER);
        outerRing[i].setOutlineColor(sf::Color(200, 240, 255, 255));
        innerCircle[i].setFillColor(COL_INNER_HOVER);
        TX(txtNumber[i]).setFillColor(COL_NUM_HOVER);
    }
    else {
        outerRing[i].setFillColor(COL_OUTER_UNLOCK);
        outerRing[i].setOutlineColor(sf::Color(100, 210, 255, 200));
        innerCircle[i].setFillColor(COL_INNER_UNLOCK);
        TX(txtNumber[i]).setFillColor(COL_NUM_UNLOCK);
    }
}

inline void LevelSelectScreen::confirmSelection(int i) {
    if (!isUnlocked(i)) {
        postMessage("Level " + to_string(i + 1) + " is locked!");
        return;
    }
    result = i + 1;   // 1-based level number
    done = true;
}

inline void LevelSelectScreen::postMessage(const string& msg) {
    showMessage = true;
    messageTimer = 2.5f;
    TX(txtMessage).setString(msg);
    TX(txtMessage).setFillColor(COL_MSG_ERR);
    auto lb = TX(txtMessage).getLocalBounds();
    TX(txtMessage).setPosition({ 400.f - lb.size.x * 0.5f, 526.f });
}

inline void LevelSelectScreen::handleClick(sf::Vector2f mp) {
    if (backBtn.getGlobalBounds().contains(mp)) {
        result = -1;
        done = true;
        return;
    }

    for (int i = 0; i < TOTAL_LEVELS; i++) {
        // Hit-test against the outer circle bounding box
        sf::Vector2f c = medalCentre(i);
        float dx = mp.x - c.x;
        float dy = mp.y - c.y;
        if (dx * dx + dy * dy <= MED_R * MED_R) {
            hoveredIndex = i;
            for (int j = 0; j < TOTAL_LEVELS; j++) refreshMedallion(j);
            confirmSelection(i);
            return;
        }
    }
}

inline void LevelSelectScreen::handleEvent(const sf::Event& event) {
    if (!layoutReady) return;

    if (const auto* kp = event.getIf<sf::Event::KeyPressed>()) {
        switch (kp->code) {

        case sf::Keyboard::Key::Escape:
            result = -1;
            done = true;
            break;

        case sf::Keyboard::Key::Left:
            hoveredIndex = (hoveredIndex - 1 + TOTAL_LEVELS) % TOTAL_LEVELS;
            for (int j = 0; j < TOTAL_LEVELS; j++) refreshMedallion(j);
            break;

        case sf::Keyboard::Key::Right:
            hoveredIndex = (hoveredIndex + 1) % TOTAL_LEVELS;
            for (int j = 0; j < TOTAL_LEVELS; j++) refreshMedallion(j);
            break;

        case sf::Keyboard::Key::Up:
            hoveredIndex = (hoveredIndex - COLS + TOTAL_LEVELS) % TOTAL_LEVELS;
            for (int j = 0; j < TOTAL_LEVELS; j++) refreshMedallion(j);
            break;

        case sf::Keyboard::Key::Down:
            hoveredIndex = (hoveredIndex + COLS) % TOTAL_LEVELS;
            for (int j = 0; j < TOTAL_LEVELS; j++) refreshMedallion(j);
            break;

        case sf::Keyboard::Key::Enter:
        case sf::Keyboard::Key::Space:
            confirmSelection(hoveredIndex);
            break;

        default: break;
        }
        return;
    }

    if (const auto* mb = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mb->button == sf::Mouse::Button::Left)
            handleClick({ static_cast<float>(mb->position.x),
                          static_cast<float>(mb->position.y) });
    }
}

inline void LevelSelectScreen::draw(sf::RenderWindow& window) {
    if (!layoutReady) setupLayout();

    if (showMessage) {
        messageTimer -= 1.f / 30.f;
        if (messageTimer <= 0.f) showMessage = false;
    }

    // Background
    if (bgLoaded)
        window.draw(*bgSprite);
    else {
        sf::RectangleShape fb({ 800.f, 600.f });
        fb.setFillColor(COL_BG_FALLBACK);
        window.draw(fb);
    }

    window.draw(titleBar);
    window.draw(TX(txtTitle));
    window.draw(TX(txtSubtitle));

    for (int i = 0; i < TOTAL_LEVELS; i++) {
        window.draw(outerRing[i]);
        window.draw(innerCircle[i]);
        window.draw(TX(txtNumber[i]));
        if (!isUnlocked(i))
            window.draw(TX(txtLock[i]));
    }

    window.draw(backBtn);
    window.draw(TX(txtBack));

    if (showMessage)
        window.draw(TX(txtMessage));
}