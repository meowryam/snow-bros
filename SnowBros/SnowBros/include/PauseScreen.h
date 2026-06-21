#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace std;
using namespace sf;
enum class PauseResult {
    NONE,
    RESUME,
    OPEN_SHOP,
    OPEN_KEYREMAP,
    QUIT_TO_MENU
};

class PauseScreen {
    sf::Font fontTitle;
    sf::Font fontButtons;
    bool extraFontsLoaded = false;
public:
    PauseScreen() : selectedOption(0) {}

    PauseScreen(const PauseScreen&) = delete;
    PauseScreen& operator=(const PauseScreen&) = delete;

    bool loadFont(const string& path, const string& titlePath, const string& btnPath) {

        if (!font.openFromFile(path)) return false;
        extraFontsLoaded = fontTitle.openFromFile(titlePath);
        if (!extraFontsLoaded) cout << "fontTitle not loaded" << endl;
        bool fontButtonsLoaded = fontButtons.openFromFile(btnPath);
        if (!fontButtonsLoaded) cout << "fontButtons not loaded" << endl;

        bgLoaded = bgTex.loadFromFile("assets\\images\\Pause_bg.png");
        if (bgLoaded) {
            bgSprite.emplace(bgTex);
            Vector2u ts = bgTex.getSize();
            bgSprite->setScale({ 800.f / static_cast<float>(ts.x), 600.f / static_cast<float>(ts.y) });
        }


        txtTitle.emplace(extraFontsLoaded ? fontTitle : font, "PAUSED", 32u);
        txtHint.emplace(font, "arrows to navigate  |  Enter to select  |  Esc to resume", 10u);
        txtMessage.emplace(font, "", 10u);
        for (int i = 0; i < OPT_COUNT; i++)
            txtOptions[i].emplace(fontButtons, "", 14u);
        layoutReady = false;
        return true;
    }

    void reset() {
        selectedOption = 0;
        layoutReady = false;
    }

    PauseResult handleEvent(const Event& event) {
        if (const auto* kp = event.getIf<Event::KeyPressed>()) {
            if (kp->code == Keyboard::Key::Up) {
                selectedOption = (selectedOption - 1 + OPT_COUNT) % OPT_COUNT;
                refreshOptions();
            }
            else if (kp->code == Keyboard::Key::Down) {
                selectedOption = (selectedOption + 1) % OPT_COUNT;
                refreshOptions();
            }
            else if (kp->code == Keyboard::Key::Escape)
                return PauseResult::RESUME;
            else if (kp->code == Keyboard::Key::Enter) {
                if (selectedOption == 0) return PauseResult::RESUME;
                if (selectedOption == 1) return PauseResult::OPEN_SHOP;
                if (selectedOption == 2) return PauseResult::OPEN_KEYREMAP;
                if (selectedOption == 3) return PauseResult::QUIT_TO_MENU;
            }
        }
        if (const auto* mb = event.getIf<Event::MouseButtonPressed>()) {
            if (mb->button == Mouse::Button::Left)
                handleClick({ static_cast<float>(mb->position.x),
                              static_cast<float>(mb->position.y) });
        }
        return PauseResult::NONE;
    }

    void draw(RenderWindow& window) {
        if (!layoutReady) setupLayout();


        if (bgLoaded) window.draw(*bgSprite);
        else {
            RectangleShape fb({ 800.f, 600.f });
            fb.setFillColor(Color(180, 220, 255, 255));
            window.draw(fb);
        }

        window.draw(panel);
        window.draw(panelOutline);
        window.draw(TX(txtTitle));
        window.draw(divider);


        for (int i = 0; i < OPT_COUNT; i++) {
            window.draw(optBtns[i]);
            window.draw(TX(txtOptions[i]));
        }

        window.draw(TX(txtHint));
    }

private:
    static constexpr int OPT_COUNT = 4;
    static constexpr float PANEL_W = 320.f;
    static constexpr float PANEL_H = 300.f;
    static constexpr float PANEL_X = (800.f - PANEL_W) / 2.f;   // 240
    static constexpr float PANEL_Y = (600.f - PANEL_H) / 2.f;   // 150
    static constexpr float BTN_W = 260.f;
    static constexpr float BTN_H = 38.f;
    static constexpr float BTN_GAP = 10.f;
    static constexpr float BTN_LEFT = PANEL_X + (PANEL_W - BTN_W) / 2.f;
    static constexpr float BTNS_TOP = PANEL_Y + 100.f;
    const Color PANEL_FILL{ 5,  15,  40,  165 };    // CHANGED — was fully transparent
    const Color PANEL_OUTLINE{ 140, 215, 255, 200 };  // CHANGED — was fully transparent
    const Color BTN_NORMAL{ 10,  30,  80, 150 };
    const Color BTN_SEL{ 40, 120, 220, 160 };
    const Color BTN_OUT_NRM{ 60, 140, 220,  80 };
    const Color BTN_OUT_SEL{ 140, 220, 255, 255 };
    const Color COL_TITLE{ 200, 240, 255, 255 };
    const Color COL_OPT_NORM{ 160, 210, 255, 220 };
    const Color COL_OPT_SEL{ 255, 255, 255, 255 };
    const Color COL_HINT{ 100, 160, 220, 160 };

    sf::Font  font;
    bool      layoutReady = false;
    int       selectedOption;

    Texture bgTex;
    bool bgLoaded = false;
    optional<Sprite> bgSprite;

    RectangleShape panel;
    RectangleShape panelOutline;
    RectangleShape divider;
    RectangleShape optBtns[OPT_COUNT];

    optional<Text> txtTitle;
    optional<Text> txtOptions[OPT_COUNT];
    optional<Text> txtHint;
    optional<Text> txtMessage;


    Text& TX(optional<Text>& t) { return t.value(); }

    void setupLayout() {
        panel.setSize({ PANEL_W, PANEL_H });
        panel.setPosition({ PANEL_X, PANEL_Y });
        panel.setFillColor(PANEL_FILL);
        panel.setOutlineThickness(0.f);
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
            TX(txtTitle).setPosition({ PANEL_X + (PANEL_W - lb.size.x) * 0.5f, PANEL_Y + 22.f });
        }


        divider.setSize({ PANEL_W - 40.f, 1.5f });
        divider.setPosition({ PANEL_X + 20.f, PANEL_Y + 72.f });
        divider.setFillColor(Color(140, 215, 255, 130));   // CHANGED — was 60, now visible

        const char* labels[OPT_COUNT] = { "Resume", "Shop", "Key Bindings", "Quit to Menu" };
        for (int i = 0; i < OPT_COUNT; i++) {
            float by = BTNS_TOP + i * (BTN_H + BTN_GAP);

            optBtns[i].setSize({ BTN_W, BTN_H });
            optBtns[i].setPosition({ BTN_LEFT, by });
            optBtns[i].setOutlineThickness(1.5f);

            TX(txtOptions[i]).setString(labels[i]);
        }

        TX(txtHint).setFillColor(COL_HINT);
        {
            auto lb = TX(txtHint).getLocalBounds();
            TX(txtHint).setPosition({ PANEL_X + (PANEL_W - lb.size.x) * 0.5f,PANEL_Y + PANEL_H - 22.f });
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
            TX(txtOptions[i]).setStyle(sel ? Text::Bold : Text::Regular);
            auto lb = TX(txtOptions[i]).getLocalBounds();
            TX(txtOptions[i]).setPosition({ BTN_LEFT + (BTN_W - lb.size.x) * 0.5f,by + (BTN_H - lb.size.y) * 0.5f - 2.f });
        }
    }

    void handleClick(sf::Vector2f mp) {
        if (!layoutReady) return;
        for (int i = 0; i < OPT_COUNT; i++) {
            if (optBtns[i].getGlobalBounds().contains(mp)) {
                selectedOption = i;
                refreshOptions();
            }
        }
    }
};