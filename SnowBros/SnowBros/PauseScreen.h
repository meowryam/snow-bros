#pragma once
#include <SFML/Graphics.hpp>
using namespace std;

enum class PauseResult {
    NONE,
    RESUME,
    OPEN_SHOP,
    OPEN_KEYREMAP,
    QUIT_TO_MENU
};

class PauseScreen {
public:
    PauseScreen() : selectedOption(0) {}
    PauseScreen(const PauseScreen&) = delete;
    PauseScreen& operator=(const PauseScreen&) = delete;

    bool loadFont(const string& path) {
        if (!font.openFromFile(path)) return false;

        bgLoaded = bgTex.loadFromFile("assets\\images\\Pause_bg.png");
        if (bgLoaded) {
            bgSprite.emplace(bgTex);
            sf::Vector2u ts = bgTex.getSize();
            bgSprite->setScale({
                800.f / static_cast<float>(ts.x),
                600.f / static_cast<float>(ts.y)
                });
        }

        txtTitle.emplace(font, "PAUSED", 28u);
        txtHint.emplace(font, "arrows to navigate  |  Enter to select  |  Esc to resume", 9u);
        txtMessage.emplace(font, "", 9u);
        for (int i = 0; i < OPT_COUNT; i++)
            txtOptions[i].emplace(font, "", 14u);

        layoutReady = false;
        return true;
    }

    void reset() {
        selectedOption = 0;
        layoutReady = false;
    }

    PauseResult handleEvent(const sf::Event& event) {
        if (const auto* kp = event.getIf<sf::Event::KeyPressed>()) {
            if (kp->code == sf::Keyboard::Key::Up) {
                selectedOption = (selectedOption - 1 + OPT_COUNT) % OPT_COUNT;
                refreshOptions();
            }
            else if (kp->code == sf::Keyboard::Key::Down) {
                selectedOption = (selectedOption + 1) % OPT_COUNT;
                refreshOptions();
            }
            else if (kp->code == sf::Keyboard::Key::Escape)
                return PauseResult::RESUME;
            else if (kp->code == sf::Keyboard::Key::Enter) {
                if (selectedOption == 0) return PauseResult::RESUME;
                if (selectedOption == 1) return PauseResult::OPEN_SHOP;
                if (selectedOption == 2) return PauseResult::OPEN_KEYREMAP;
                if (selectedOption == 3) return PauseResult::QUIT_TO_MENU;
            }
        }
        if (const auto* mb = event.getIf<sf::Event::MouseButtonPressed>()) {
            if (mb->button == sf::Mouse::Button::Left)
                handleClick({ static_cast<float>(mb->position.x),
                              static_cast<float>(mb->position.y) });
        }
        return PauseResult::NONE;
    }

    void draw(sf::RenderWindow& window) {
        if (!layoutReady) setupLayout();

        // Background
        if (bgLoaded)
            window.draw(*bgSprite);
        else {
            sf::RectangleShape fb({ 800.f, 600.f });
            fb.setFillColor(sf::Color(180, 220, 255, 255));
            window.draw(fb);
        }

        // Frosted panel
        window.draw(panel);
        window.draw(panelOutline);

        // Title
        window.draw(TX(txtTitle));

        // Divider line
        window.draw(divider);

        // Options
        for (int i = 0; i < OPT_COUNT; i++) {
            window.draw(optBtns[i]);
            window.draw(TX(txtOptions[i]));
        }

        // Hint
        window.draw(TX(txtHint));
    }

private:

    sf::Font  font;
    static constexpr int OPT_COUNT = 4;

    // ── layout constants ───────────────────────────────────────
    static constexpr float PANEL_W = 320.f;
    static constexpr float PANEL_H = 300.f;
    static constexpr float PANEL_X = (800.f - PANEL_W) / 2.f;   // 240
    static constexpr float PANEL_Y = (600.f - PANEL_H) / 2.f;   // 150
    static constexpr float BTN_W = 260.f;
    static constexpr float BTN_H = 38.f;
    static constexpr float BTN_GAP = 10.f;
    static constexpr float BTN_LEFT = PANEL_X + (PANEL_W - BTN_W) / 2.f;
    static constexpr float BTNS_TOP = PANEL_Y + 100.f;

    const sf::Color PANEL_FILL{ 0,  10,  30,   0 };   // fully transparent — no card
    const sf::Color PANEL_OUTLINE{ 0,   0,   0,   0 };   // invisible

    const sf::Color BTN_NORMAL{ 10,  30,  80, 120 };   // very subtle dark tint
    const sf::Color BTN_SEL{ 40, 120, 220, 160 };   // glowing blue selected
    const sf::Color BTN_OUT_NRM{ 60, 140, 220,  80 };   // faint cyan rim
    const sf::Color BTN_OUT_SEL{ 140, 220, 255, 255 };   // bright crystal glow

    const sf::Color COL_TITLE{ 200, 240, 255, 255 };   // bright icy white-cyan
    const sf::Color COL_OPT_NORM{ 160, 210, 255, 220 };   // soft blue-white
    const sf::Color COL_OPT_SEL{ 255, 255, 255, 255 };   // pure white when selected
    const sf::Color COL_HINT{ 100, 160, 220, 160 };   // faint hint

    // ── state ──────────────────────────────────────────────────
   
    bool      layoutReady = false;
    int       selectedOption;

    sf::Texture          bgTex;
    bool                 bgLoaded = false;
    optional<sf::Sprite> bgSprite;

    sf::RectangleShape panel;
    sf::RectangleShape panelOutline;   // drawn on top as border
    sf::RectangleShape divider;
    sf::RectangleShape optBtns[OPT_COUNT];

    optional<sf::Text> txtTitle;
    optional<sf::Text> txtOptions[OPT_COUNT];
    optional<sf::Text> txtHint;
    optional<sf::Text> txtMessage;

    // ── helpers ────────────────────────────────────────────────
    sf::Text& TX(optional<sf::Text>& t) { return t.value(); }

    void setupLayout() {
        // Panel
        panel.setSize({ PANEL_W, PANEL_H });
        panel.setPosition({ PANEL_X, PANEL_Y });
        panel.setFillColor(PANEL_FILL);
        panel.setOutlineThickness(0.f);   // outline drawn separately

        panelOutline.setSize({ PANEL_W, PANEL_H });
        panelOutline.setPosition({ PANEL_X, PANEL_Y });
        panelOutline.setFillColor(sf::Color::Transparent);
        panelOutline.setOutlineThickness(2.5f);
        panelOutline.setOutlineColor(PANEL_OUTLINE);

        TX(txtTitle).setFillColor(COL_TITLE);
        TX(txtTitle).setStyle(sf::Text::Bold);
        TX(txtTitle).setLetterSpacing(5.f);
        {
            auto lb = TX(txtTitle).getLocalBounds();
            TX(txtTitle).setPosition({
                PANEL_X + (PANEL_W - lb.size.x) * 0.5f,
                PANEL_Y + 22.f
                });
        }

        // Thin divider under title
        divider.setSize({ PANEL_W - 40.f, 1.5f });
        divider.setPosition({ PANEL_X + 20.f, PANEL_Y + 72.f });
        divider.setFillColor(sf::Color(100, 180, 255, 60));   // nearly invisible, just a whisper

        // Option buttons
        const char* labels[OPT_COUNT] = { "Resume", "Shop", "Key Bindings", "Quit to Menu" };
        for (int i = 0; i < OPT_COUNT; i++) {
            float by = BTNS_TOP + i * (BTN_H + BTN_GAP);

            optBtns[i].setSize({ BTN_W, BTN_H });
            optBtns[i].setPosition({ BTN_LEFT, by });
            optBtns[i].setOutlineThickness(1.5f);

            TX(txtOptions[i]).setString(labels[i]);
        }

        // Hint — bottom of panel
        TX(txtHint).setFillColor(COL_HINT);
        {
            auto lb = TX(txtHint).getLocalBounds();
            TX(txtHint).setPosition({
                PANEL_X + (PANEL_W - lb.size.x) * 0.5f,
                PANEL_Y + PANEL_H - 22.f
                });
        }

        refreshOptions();
        layoutReady = true;
    }

    void refreshOptions() {
        const char* labels[OPT_COUNT] = { "Resume", "Shop", "Key Bindings", "Quit to Menu" };
        for (int i = 0; i < OPT_COUNT; i++) {
            bool sel = (i == selectedOption);
            float by = BTNS_TOP + i * (BTN_H + BTN_GAP);

            optBtns[i].setFillColor(sel ? BTN_SEL : BTN_NORMAL);
            optBtns[i].setOutlineColor(sel ? BTN_OUT_SEL : BTN_OUT_NRM);

            TX(txtOptions[i]).setString(labels[i]);
            TX(txtOptions[i]).setFillColor(sel ? COL_OPT_SEL : COL_OPT_NORM);
            TX(txtOptions[i]).setStyle(sel ? sf::Text::Bold : sf::Text::Regular);

            auto lb = TX(txtOptions[i]).getLocalBounds();
            TX(txtOptions[i]).setPosition({
                BTN_LEFT + (BTN_W - lb.size.x) * 0.5f,
                by + (BTN_H - lb.size.y) * 0.5f - 2.f
                });
        }
    }

    void handleClick(sf::Vector2f mp) {
        if (!layoutReady) return;
        for (int i = 0; i < OPT_COUNT; i++) {
            if (optBtns[i].getGlobalBounds().contains(mp)) {
                selectedOption = i;
                refreshOptions();
                // simulate Enter on second click on already-selected
                // (single-click selects, double/same click activates — keep simple)
            }
        }
    }
};