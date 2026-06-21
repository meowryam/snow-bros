#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "SoundManager.h"  
using namespace std;

enum class MainMenuResult {
    NONE,
    START_GAME,
    START_2PLAYER,
    OPEN_SHOP,
    OPEN_LEADERBOARD,
    OPEN_KEYREMAP,
    LOGOUT,
    QUIT
};

class MainMenu {
private:
    sf::Font font;
    sf::Font fontTitle;    // Cinzel-Bold
    sf::Font fontBody;     // Montserrat-Medium
    sf::Font fontButtons;  // Montserrat-SemiBoldItalic
    bool extraFontsLoaded = false;
    static constexpr int OPT_COUNT = 7;

    optional<sf::Text> txtTitle;
    optional<sf::Text> txtSub;
    optional<sf::Text> txtOptions[OPT_COUNT];
    optional<sf::Text> txtHint;

    sf::RectangleShape optBtns[OPT_COUNT];
    sf::RectangleShape dividerTop;
    sf::RectangleShape dividerBot;

    int    selectedOption = 0;
    string username;
    SoundManager& soundManager;   // ADD

    sf::Color titleCol = sf::Color(200, 240, 255, 255);   // bright ice white-blue
    sf::Color subCol = sf::Color(160, 215, 255, 230);   // softer ice blue
    sf::Color hintCol = sf::Color(100, 160, 220, 170);   // dim ice hint


    sf::Color btnNormal = sf::Color(10, 30, 80, 120);
    sf::Color btnSel = sf::Color(40, 120, 220, 160);
    sf::Color btnOutNrm = sf::Color(60, 140, 220, 80);
    sf::Color btnOutSel = sf::Color(140, 220, 255, 255);


    sf::Color txtNormal = sf::Color(160, 210, 255, 220);
    sf::Color txtSel = sf::Color(255, 255, 255, 255);


    sf::Texture bgTex;
    bool        bgLoaded = false;
    optional<sf::Sprite> bgSprite;


    static constexpr float CX = 400.f;
    static constexpr float BTN_W = 280.f;
    static constexpr float BTN_H = 38.f;
    static constexpr float BTN_GAP = 9.f;
    static constexpr float BX = CX - BTN_W * 0.5f;    // 260
    static constexpr float BTNS_TOP = 210.f;

    sf::Text& TX(optional<sf::Text>& t) { return t.value(); }

    void centreText(sf::Text& t, float y) {
        auto lb = t.getLocalBounds();
        t.setPosition({ CX - lb.size.x * 0.5f, y });
    }

public:
    MainMenu(SoundManager& sm) : selectedOption(0), soundManager(sm) {}

    bool loadFont(const string& path, const string& titlePath,
        const string& bodyPath, const string& btnPath) {
        if (!font.openFromFile(path)) return false;
        extraFontsLoaded = fontTitle.openFromFile(titlePath);
        if (!extraFontsLoaded) cout << "fontTitle not loaded" << endl;
        bool fontBodyLoaded = fontBody.openFromFile(bodyPath);
        if (!fontBodyLoaded) cout << "fontBody not loaded" << endl;
        bool fontButtonsLoaded = fontButtons.openFromFile(btnPath);
        if (!fontButtonsLoaded) cout << "fontButtons not loaded" << endl;

        bgLoaded = bgTex.loadFromFile("assets\\images\\Login_bg.png");
        if (bgLoaded) {
            bgSprite.emplace(bgTex);
            sf::Vector2u ts = bgTex.getSize();
            bgSprite->setScale({
                800.f / static_cast<float>(ts.x),
                600.f / static_cast<float>(ts.y)
                });
        }
        txtTitle.emplace(extraFontsLoaded ? fontTitle : font, "SNOW  BROS", 30u);
        txtSub.emplace(fontBody, "", 13u);
        txtHint.emplace(fontBody, "", 10u);
        for (int i = 0; i < OPT_COUNT; i++)
            txtOptions[i].emplace(fontButtons, "", 13u);

        return true;
    }

    void setUsername(const string& name) { username = name; }
    void reset() { selectedOption = 0; }

    MainMenuResult handleEvent(sf::Event& event) {
        if (auto* kp = event.getIf<sf::Event::KeyPressed>()) {
            if (kp->code == sf::Keyboard::Key::Up) {
                selectedOption = (selectedOption - 1 + OPT_COUNT) % OPT_COUNT;
                soundManager.playSound("ui_navigate");   // ADD
            }
            else if (kp->code == sf::Keyboard::Key::Down) {
                selectedOption = (selectedOption + 1) % OPT_COUNT;
                soundManager.playSound("ui_navigate");   // ADD
            }
            else if (kp->code == sf::Keyboard::Key::Enter) {
                soundManager.playSound("ui_confirm");   // ADD
                if (selectedOption == 0) return MainMenuResult::START_GAME;
                if (selectedOption == 1) return MainMenuResult::START_2PLAYER;
                if (selectedOption == 2) return MainMenuResult::OPEN_SHOP;
                if (selectedOption == 3) return MainMenuResult::OPEN_LEADERBOARD;
                if (selectedOption == 4) return MainMenuResult::OPEN_KEYREMAP;
                if (selectedOption == 5) return MainMenuResult::LOGOUT;
                if (selectedOption == 6) return MainMenuResult::QUIT;
            }
        }
        return MainMenuResult::NONE;
    }

    void draw(sf::RenderWindow& window) {


        if (bgLoaded) window.draw(*bgSprite);
        else {
            sf::RectangleShape fb({ 800.f, 600.f });
            fb.setFillColor(sf::Color(5, 12, 35, 255));
            window.draw(fb);
        }


        sf::RectangleShape vignette({ 800.f, 600.f });
        vignette.setFillColor(sf::Color(0, 5, 20, 70));
        window.draw(vignette);


        TX(txtTitle).setString("SNOW  BROS");
        TX(txtTitle).setCharacterSize(34u);
        TX(txtTitle).setStyle(sf::Text::Bold);
        TX(txtTitle).setLetterSpacing(5.f);
        TX(txtTitle).setOutlineThickness(2.f);
        TX(txtTitle).setOutlineColor(sf::Color(50, 130, 200, 200));
        // shadow
        TX(txtTitle).setFillColor(sf::Color(0, 0, 0, 100));
        centreText(TX(txtTitle), 80.f);
        TX(txtTitle).move({ 2.f, 2.f });
        window.draw(TX(txtTitle));
        // main
        TX(txtTitle).setFillColor(titleCol);
        centreText(TX(txtTitle), 78.f);
        window.draw(TX(txtTitle));

        // thin ice divider under title
        dividerTop.setSize({ 260.f, 1.5f });
        dividerTop.setPosition({ CX - 130.f, 130.f });
        dividerTop.setFillColor(sf::Color(120, 200, 255, 80));
        window.draw(dividerTop);

        TX(txtSub).setString("Welcome back,  " + username + "!");
        TX(txtSub).setCharacterSize(14u);
        TX(txtSub).setFillColor(subCol);
        TX(txtSub).setStyle(sf::Text::Regular);
        centreText(TX(txtSub), 148.f);
        window.draw(TX(txtSub));

        const char* opts[OPT_COUNT] = {
            "1 Player", "2 Player", "Shop",
            "Leaderboard", "Key Bindings", "Logout", "Quit"
        };

        for (int i = 0; i < OPT_COUNT; i++) {
            bool sel = (i == selectedOption);
            float by = BTNS_TOP + i * (BTN_H + BTN_GAP);

            optBtns[i].setSize({ BTN_W, BTN_H });
            optBtns[i].setPosition({ BX, by });
            optBtns[i].setFillColor(sel ? btnSel : btnNormal);
            optBtns[i].setOutlineThickness(1.5f);
            optBtns[i].setOutlineColor(sel ? btnOutSel : btnOutNrm);
            window.draw(optBtns[i]);

            TX(txtOptions[i]).setString(opts[i]);
            TX(txtOptions[i]).setCharacterSize(13u);
            TX(txtOptions[i]).setFillColor(sel ? txtSel : txtNormal);
            TX(txtOptions[i]).setStyle(sel ? sf::Text::Bold : sf::Text::Regular);
            auto lb = TX(txtOptions[i]).getLocalBounds();
            TX(txtOptions[i]).setPosition({
                BX + (BTN_W - lb.size.x) * 0.5f,
                by + (BTN_H - lb.size.y) * 0.5f - 2.f
                });
            window.draw(TX(txtOptions[i]));
        }

        // thin divider below buttons
        dividerBot.setSize({ 260.f, 1.5f });
        dividerBot.setPosition({ CX - 130.f, BTNS_TOP + OPT_COUNT * (BTN_H + BTN_GAP) + 4.f });
        dividerBot.setFillColor(sf::Color(120, 200, 255, 60));
        window.draw(dividerBot);

        TX(txtHint).setString("Arrows to navigate   |   Enter to select");
        TX(txtHint).setCharacterSize(11u);
        TX(txtHint).setFillColor(hintCol);
        centreText(TX(txtHint), 570.f);
        window.draw(TX(txtHint));
    }
};