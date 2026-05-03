#pragma once
#include <SFML/Graphics.hpp>
#include "PlayerData.h"
using namespace std;

enum class GameOverResult {
    NONE,
    RETRY,
    QUIT_TO_MENU
};

class GameOverScreen {
public:
    GameOverScreen() : selectedOption(0) {}
    GameOverScreen(const GameOverScreen&) = delete;
    GameOverScreen& operator=(const GameOverScreen&) = delete;

    bool loadFont(const string& path) {
        if (!font.openFromFile(path)) return false;

        bgLoaded = bgTex.loadFromFile("assets\\images\\GameOver_bg.png");
        if (bgLoaded) {
            bgSprite.emplace(bgTex);
            sf::Vector2u ts = bgTex.getSize();
            bgSprite->setScale({
                800.f / static_cast<float>(ts.x),
                600.f / static_cast<float>(ts.y)
                });
        }
        titleText.emplace(font, "GAME  OVER", 30u);
        statsText.emplace(font, "", 14u);
        hintText.emplace(font, "Arrows to navigate  |  Enter to select", 9u);
        for (int i = 0; i < 2; i++)
            optionTexts[i].emplace(font, "", 20u);

        layoutReady = false;
        return true;
    }

    void reset() {
        selectedOption = 0;
        layoutReady = false;
    }

    GameOverResult handleEvent(sf::Event& event) {
        if (auto* kp = event.getIf<sf::Event::KeyPressed>()) {
            if (kp->code == sf::Keyboard::Key::Up ||
                kp->code == sf::Keyboard::Key::Down)
            {
                selectedOption = 1 - selectedOption;
                refreshOptions();
            }
            else if (kp->code == sf::Keyboard::Key::Enter) {
                if (selectedOption == 0) return GameOverResult::RETRY;
                if (selectedOption == 1) return GameOverResult::QUIT_TO_MENU;
            }
        }
        if (const auto* mb = event.getIf<sf::Event::MouseButtonPressed>()) {
            if (mb->button == sf::Mouse::Button::Left)
                handleClick({ static_cast<float>(mb->position.x),
                              static_cast<float>(mb->position.y) });
        }
        return GameOverResult::NONE;
    }

    void draw(sf::RenderWindow& window, const PlayerData& data) {
        if (!layoutReady) setupLayout(data);

        // Refresh stats each draw in case data changed
        string stats = "Level: " + to_string(data.getCurrentLevel()) +
            "   Gems: " + to_string(data.getGemCount()) +
            "   Best: " + to_string(data.getHighscore());
        TX(statsText).setString(stats);
        {
            auto lb = TX(statsText).getLocalBounds();
            TX(statsText).setPosition({ 400.f - lb.size.x * 0.5f, STATS_Y });
        }

        // Background
        if (bgLoaded)
            window.draw(*bgSprite);
        else {
            sf::RectangleShape fb({ 800.f, 600.f });
            fb.setFillColor(sf::Color(20, 0, 5, 255));
            window.draw(fb);
        }

        // Subtle panel
        window.draw(panel);
        window.draw(panelOutline);

        // Title
        window.draw(TX(titleText));

        // Divider
        window.draw(divider);

        // Stats
        window.draw(TX(statsText));

        // Buttons
        for (int i = 0; i < 2; i++) {
            window.draw(optBtns[i]);
            window.draw(TX(optionTexts[i]));
        }

        // Hint
        window.draw(TX(hintText));
    }

private:

    sf::Font  font;
    // ── layout constants ───────────────────────────────────────
    static constexpr float PANEL_W = 360.f;
    static constexpr float PANEL_H = 310.f;
    static constexpr float PANEL_X = (800.f - PANEL_W) / 2.f;   // 220
    static constexpr float PANEL_Y = (600.f - PANEL_H) / 2.f;   // 145
    static constexpr float BTN_W = 280.f;
    static constexpr float BTN_H = 42.f;
    static constexpr float BTN_GAP = 14.f;
    static constexpr float BTN_LEFT = PANEL_X + (PANEL_W - BTN_W) / 2.f;
    static constexpr float BTNS_TOP = PANEL_Y + 168.f;
    static constexpr float STATS_Y = PANEL_Y + 108.f;
    const sf::Color PANEL_FILL{ 10,  2,   4, 140 };    // lighter touch, less coverage
    const sf::Color PANEL_OUTLINE{ 160,  25,  25, 180 };

    const sf::Color DIV_COL{ 200,  50,  30, 120 };

    const sf::Color COL_TITLE{ 255,  80,  50, 255 };

    const sf::Color COL_STATS{ 255, 220, 200, 255 };

    const sf::Color BTN_NORMAL{ 25,   5,   5, 160 };
    const sf::Color BTN_SEL{ 180,  25,  15, 220 };
    const sf::Color BTN_OUT_NRM{ 160,  50,  40, 180 };   // visible outline now
    const sf::Color BTN_OUT_SEL{ 255,  90,  60, 255 };

    const sf::Color COL_OPT_NORM{ 220, 170, 160, 230 };   // much more visible
    const sf::Color COL_OPT_SEL{ 255, 235, 210, 255 };
    const sf::Color COL_HINT{ 130,  80,  70, 180 };

    // ── state ──────────────────────────────────────────────────
    
    bool      layoutReady = false;
    int       selectedOption;

    sf::Texture          bgTex;
    bool                 bgLoaded = false;
    optional<sf::Sprite> bgSprite;

    sf::RectangleShape panel;
    sf::RectangleShape panelOutline;
    sf::RectangleShape divider;
    sf::RectangleShape optBtns[2];

    optional<sf::Text> titleText;
    optional<sf::Text> statsText;
    optional<sf::Text> optionTexts[2];
    optional<sf::Text> hintText;

    sf::Text& TX(optional<sf::Text>& t) { return t.value(); }

    void setupLayout(const PlayerData& data) {
        // Panel
        panel.setSize({ PANEL_W, PANEL_H });
        panel.setPosition({ PANEL_X, PANEL_Y });
        panel.setFillColor(PANEL_FILL);
        panel.setOutlineThickness(0.f);

        panelOutline.setSize({ PANEL_W, PANEL_H });
        panelOutline.setPosition({ PANEL_X, PANEL_Y });
        panelOutline.setFillColor(sf::Color::Transparent);
        panelOutline.setOutlineThickness(2.f);
        panelOutline.setOutlineColor(PANEL_OUTLINE);

        // Title
        TX(titleText).setFillColor(COL_TITLE);
        TX(titleText).setStyle(sf::Text::Bold);
        TX(titleText).setLetterSpacing(4.f);
        {
            auto lb = TX(titleText).getLocalBounds();
            TX(titleText).setPosition({
                PANEL_X + (PANEL_W - lb.size.x) * 0.5f,
                PANEL_Y + 20.f
                });
        }

        // Divider
        divider.setSize({ PANEL_W - 30.f, 1.5f });
        divider.setPosition({ PANEL_X + 15.f, PANEL_Y + 78.f });
        divider.setFillColor(DIV_COL);

        // Stats
        TX(statsText).setFillColor(COL_STATS);

        // Buttons
        const char* labels[2] = { "Retry", "Quit to Menu" };
        for (int i = 0; i < 2; i++) {
            float by = BTNS_TOP + i * (BTN_H + BTN_GAP);
            optBtns[i].setSize({ BTN_W, BTN_H });
            optBtns[i].setPosition({ BTN_LEFT, by });
            optBtns[i].setOutlineThickness(1.5f);
            TX(optionTexts[i]).setString(labels[i]);
        }

        // Hint
        TX(hintText).setFillColor(COL_HINT);
        {
            auto lb = TX(hintText).getLocalBounds();
            TX(hintText).setPosition({
                PANEL_X + (PANEL_W - lb.size.x) * 0.5f,
                PANEL_Y + PANEL_H - 20.f
                });
        }

        refreshOptions();
        layoutReady = true;
    }

    void refreshOptions() {
        const char* labels[2] = { "Retry", "Quit to Menu" };
        for (int i = 0; i < 2; i++) {
            bool sel = (i == selectedOption);
            float by = BTNS_TOP + i * (BTN_H + BTN_GAP);

            optBtns[i].setFillColor(sel ? BTN_SEL : BTN_NORMAL);
            optBtns[i].setOutlineColor(sel ? BTN_OUT_SEL : BTN_OUT_NRM);

            TX(optionTexts[i]).setString(labels[i]);
            TX(optionTexts[i]).setFillColor(sel ? COL_OPT_SEL : COL_OPT_NORM);
            TX(optionTexts[i]).setStyle(sel ? sf::Text::Bold : sf::Text::Regular);

            auto lb = TX(optionTexts[i]).getLocalBounds();
            TX(optionTexts[i]).setPosition({
                BTN_LEFT + (BTN_W - lb.size.x) * 0.5f,
                by + (BTN_H - lb.size.y) * 0.5f - 2.f
                });
        }
    }

    void handleClick(sf::Vector2f mp) {
        if (!layoutReady) return;
        for (int i = 0; i < 2; i++) {
            if (optBtns[i].getGlobalBounds().contains(mp)) {
                selectedOption = i;
                refreshOptions();
            }
        }
    }
};