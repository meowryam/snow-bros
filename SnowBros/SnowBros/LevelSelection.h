#pragma once
#include <SFML/Graphics.hpp>
#include "PlayerData.h"
#include <string>
#include <optional>
using namespace std;

class LevelSelectScreen {
public:
    explicit LevelSelectScreen(PlayerData& data) : playerData(data) {}
    LevelSelectScreen(const LevelSelectScreen&) = delete;
    LevelSelectScreen& operator=(const LevelSelectScreen&) = delete;

    bool loadAssets(const string& fontPath,
        const string& bgPath = "assets\\images\\LevelSelect_bg.png");

    void reset();
    void handleEvent(const sf::Event& event);
    void draw(sf::RenderWindow& window);

    int  result = 0;
    bool done = false;

private:
    sf::Font    font;
    static constexpr int   TOTAL_LEVELS = 10;
    static constexpr int   COLS = 5;

    
    static constexpr float POSITIONS[10][2] = {
       
        { 280.f, 220.f },  // 1
        { 420.f, 220.f },  // 2
        { 560.f, 220.f },  // 3
        { 210.f, 315.f },  // 4
        { 350.f, 315.f },  // 5
        { 490.f, 315.f },  // 6
        { 630.f, 315.f },  // 7
        { 280.f, 400.f },  // 8
        { 420.f, 400.f },  // 9
        { 560.f, 400.f },  // 10

    };
    static constexpr float HEX_R = 36.f;   
    static constexpr float TITLE_Y = 28.f;

    PlayerData& playerData;
   
    bool        layoutReady = false;
    int         hoveredIndex = 0;

    sf::Texture          bgTex;
    bool                 bgLoaded = false;
    optional<sf::Sprite> bgSprite;
    sf::CircleShape      hexShape[TOTAL_LEVELS];  
    sf::CircleShape      innerGlow[TOTAL_LEVELS];
    optional<sf::Text>   txtNumber[TOTAL_LEVELS];
    optional<sf::Text>   txtLabel[TOTAL_LEVELS];  
    optional<sf::Text>   txtTitle;
    optional<sf::Text>   txtBack;
    optional<sf::Text>   txtMessage;
    sf::RectangleShape   backBtn;

    float messageTimer = 0.f;
    bool  showMessage = false;

    
    const sf::Color C_HEX_UNLOCK{ 60,  160, 230, 180 };
    const sf::Color C_GLOW_UNLOCK{ 20,   80, 160, 140 };
    const sf::Color C_NUM_UNLOCK{ 220, 245, 255, 255 };
    const sf::Color C_OUT_UNLOCK{ 120, 210, 255, 220 };
    const sf::Color C_HEX_HOVER{ 100, 210, 255, 220 };
    const sf::Color C_GLOW_HOVER{ 30, 120, 220, 200 };
    const sf::Color C_NUM_HOVER{ 255, 255, 255, 255 };
    const sf::Color C_OUT_HOVER{ 200, 240, 255, 255 };
    const sf::Color C_HEX_LOCK{ 20,  35,  60, 160 };
    const sf::Color C_GLOW_LOCK{ 10,  20,  40, 140 };
    const sf::Color C_NUM_LOCK{ 70,  90, 110, 140 };
    const sf::Color C_OUT_LOCK{ 30,  50,  80, 120 };

    sf::Text& TX(optional<sf::Text>& t) { return t.value(); }
    sf::Vector2f centre(int i) const;
    bool isUnlocked(int i) const;
    void setupLayout();
    void refreshMedallion(int i);
    void confirmSelection(int i);
    void postMessage(const string& msg);
    void handleClick(sf::Vector2f mp);
};



inline sf::Vector2f LevelSelectScreen::centre(int i) const {
    return { POSITIONS[i][0], POSITIONS[i][1] };
}

inline bool LevelSelectScreen::isUnlocked(int i) const {
    return (i + 1) <= playerData.getCurrentLevel();
}

inline bool LevelSelectScreen::loadAssets(const string& fontPath,
    const string& bgPath)
{
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

    txtTitle.emplace(font, "SELECT  LEVEL", 18u);
    txtBack.emplace(font, "BACK  [ESC]", 9u);
    txtMessage.emplace(font, "", 9u);

    for (int i = 0; i < TOTAL_LEVELS; i++) {
        txtNumber[i].emplace(font, to_string(i + 1), 20u);
        txtLabel[i].emplace(font, isUnlocked(i) ? "" : "LOCKED", 6u);
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
   
    TX(txtTitle).setFillColor(sf::Color(200, 235, 255, 230));
    TX(txtTitle).setStyle(sf::Text::Bold);
    {
        auto lb = TX(txtTitle).getLocalBounds();
        TX(txtTitle).setPosition({ 400.f - lb.size.x * 0.5f, TITLE_Y });
    }

  
    backBtn.setSize({ 140.f, 30.f });
    backBtn.setPosition({ 330.f, 548.f });
    backBtn.setFillColor(sf::Color(5, 18, 45, 200));
    backBtn.setOutlineColor(sf::Color(100, 190, 255, 180));
    backBtn.setOutlineThickness(1.5f);
    TX(txtBack).setFillColor(sf::Color(200, 235, 255, 220));
    {
        auto lb = TX(txtBack).getLocalBounds();
        TX(txtBack).setPosition({
            330.f + (140.f - lb.size.x) * 0.5f,
            548.f + (30.f - lb.size.y) * 0.5f - 1.f
            });
    }

    TX(txtMessage).setPosition({ 400.f, 522.f });

   
    for (int i = 0; i < TOTAL_LEVELS; i++) {
        sf::Vector2f c = centre(i);

        hexShape[i].setPointCount(6);
        hexShape[i].setRadius(HEX_R);
        hexShape[i].setOrigin({ HEX_R, HEX_R });
        hexShape[i].setPosition(c);
        hexShape[i].setOutlineThickness(2.5f);
        hexShape[i].setRotation(sf::degrees(30.f));  // flat-top hex

        float gr = HEX_R - 9.f;
        innerGlow[i].setPointCount(6);
        innerGlow[i].setRadius(gr);
        innerGlow[i].setOrigin({ gr, gr });
        innerGlow[i].setPosition(c);
        innerGlow[i].setRotation(sf::degrees(30.f));

       
        TX(txtNumber[i]).setStyle(sf::Text::Bold);
        {
            auto lb = TX(txtNumber[i]).getLocalBounds();
            TX(txtNumber[i]).setPosition({
                c.x - lb.size.x * 0.5f - lb.position.x,
                c.y - lb.size.y * 0.5f - lb.position.y - 4.f
                });
        }

      
        {
            auto lb = TX(txtLabel[i]).getLocalBounds();
            TX(txtLabel[i]).setPosition({
                c.x - lb.size.x * 0.5f - lb.position.x,
                c.y + 14.f
                });
            TX(txtLabel[i]).setFillColor(C_NUM_LOCK);
        }

        refreshMedallion(i);
    }

    layoutReady = true;
}

inline void LevelSelectScreen::refreshMedallion(int i) {
    bool unlocked = isUnlocked(i);
    bool hovered = (i == hoveredIndex);

    if (hovered && unlocked) {
        hexShape[i].setFillColor(C_HEX_HOVER);
        hexShape[i].setOutlineColor(C_OUT_HOVER);
        innerGlow[i].setFillColor(C_GLOW_HOVER);
        TX(txtNumber[i]).setFillColor(C_NUM_HOVER);
    }
    else if (unlocked) {
        hexShape[i].setFillColor(C_HEX_UNLOCK);
        hexShape[i].setOutlineColor(C_OUT_UNLOCK);
        innerGlow[i].setFillColor(C_GLOW_UNLOCK);
        TX(txtNumber[i]).setFillColor(C_NUM_UNLOCK);
    }
    else {
        hexShape[i].setFillColor(C_HEX_LOCK);
        hexShape[i].setOutlineColor(C_OUT_LOCK);
        innerGlow[i].setFillColor(C_GLOW_LOCK);
        TX(txtNumber[i]).setFillColor(C_NUM_LOCK);
    }

    
    TX(txtLabel[i]).setString(unlocked ? "" : "LCK");
}

inline void LevelSelectScreen::confirmSelection(int i) {
    if (!isUnlocked(i)) {
        postMessage("Level " + to_string(i + 1) + " is locked!");
        return;
    }
    result = i + 1;
    done = true;
}

inline void LevelSelectScreen::postMessage(const string& msg) {
    showMessage = true;
    messageTimer = 2.5f;
    TX(txtMessage).setString(msg);
    TX(txtMessage).setFillColor(sf::Color(220, 80, 80, 255));
    auto lb = TX(txtMessage).getLocalBounds();
    TX(txtMessage).setPosition({ 400.f - lb.size.x * 0.5f, 522.f });
}

inline void LevelSelectScreen::handleClick(sf::Vector2f mp) {
    if (backBtn.getGlobalBounds().contains(mp)) {
        result = -1; done = true; return;
    }
    for (int i = 0; i < TOTAL_LEVELS; i++) {
        sf::Vector2f c = centre(i);
        float dx = mp.x - c.x, dy = mp.y - c.y;
        if (dx * dx + dy * dy <= HEX_R * HEX_R) {
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
            result = -1; done = true; break;
        case sf::Keyboard::Key::Left:
            hoveredIndex = (hoveredIndex - 1 + TOTAL_LEVELS) % TOTAL_LEVELS;
            for (int j = 0; j < TOTAL_LEVELS; j++) refreshMedallion(j); break;
        case sf::Keyboard::Key::Right:
            hoveredIndex = (hoveredIndex + 1) % TOTAL_LEVELS;
            for (int j = 0; j < TOTAL_LEVELS; j++) refreshMedallion(j); break;
        case sf::Keyboard::Key::Up:
            hoveredIndex = (hoveredIndex - COLS + TOTAL_LEVELS) % TOTAL_LEVELS;
            for (int j = 0; j < TOTAL_LEVELS; j++) refreshMedallion(j); break;
        case sf::Keyboard::Key::Down:
            hoveredIndex = (hoveredIndex + COLS) % TOTAL_LEVELS;
            for (int j = 0; j < TOTAL_LEVELS; j++) refreshMedallion(j); break;
        case sf::Keyboard::Key::Enter:
        case sf::Keyboard::Key::Space:
            confirmSelection(hoveredIndex); break;
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
        messageTimer -= 1.f / 60.f;
        if (messageTimer <= 0.f) showMessage = false;
    }

    
    if (bgLoaded)
        window.draw(*bgSprite);
    else {
        sf::RectangleShape fb({ 800.f, 600.f });
        fb.setFillColor(sf::Color(5, 18, 42, 255));
        window.draw(fb);
    }

    sf::RectangleShape topBar({ 800.f, 55.f });
    topBar.setFillColor(sf::Color(0, 5, 20, 140));
    window.draw(topBar);

    window.draw(TX(txtTitle));

    for (int i = 0; i < TOTAL_LEVELS; i++) {
        window.draw(hexShape[i]);
        window.draw(innerGlow[i]);
        window.draw(TX(txtNumber[i]));
        window.draw(TX(txtLabel[i]));
    }

    window.draw(backBtn);
    window.draw(TX(txtBack));

    if (showMessage)
        window.draw(TX(txtMessage));
}