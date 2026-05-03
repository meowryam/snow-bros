#pragma once
#include <SFML/Graphics.hpp>
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

    // ── Frozen palette ────────────────────────────────────────────────────
    sf::Color titleCol = sf::Color(200, 240, 255, 255);   // bright ice white-blue
    sf::Color subCol = sf::Color(160, 215, 255, 230);   // softer ice blue
    sf::Color hintCol = sf::Color(100, 160, 220, 170);   // dim ice hint

    // Buttons — same as PauseScreen
    sf::Color btnNormal = sf::Color(10, 30, 80, 120);
    sf::Color btnSel = sf::Color(40, 120, 220, 160);
    sf::Color btnOutNrm = sf::Color(60, 140, 220, 80);
    sf::Color btnOutSel = sf::Color(140, 220, 255, 255);

    // Text inside buttons
    sf::Color txtNormal = sf::Color(160, 210, 255, 220);
    sf::Color txtSel = sf::Color(255, 255, 255, 255);

    // Background
    sf::Texture bgTex;
    bool        bgLoaded = false;
    optional<sf::Sprite> bgSprite;

    // Layout constants — centred, same width as PauseScreen buttons
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
    MainMenu() : selectedOption(0) {}

    bool loadFont(const string& path) {
        if (!font.openFromFile(path)) return false;

        // reuse the same frozen castle background as LoginScreen
        bgLoaded = bgTex.loadFromFile("assets\\images\\Login_bg.png");
        if (bgLoaded) {
            bgSprite.emplace(bgTex);
            sf::Vector2u ts = bgTex.getSize();
            bgSprite->setScale({
                800.f / static_cast<float>(ts.x),
                600.f / static_cast<float>(ts.y)
                });
        }

        txtTitle.emplace(font, "SNOW  BROS", 30u);
        txtSub.emplace(font, "", 11u);
        txtHint.emplace(font, "", 9u);
        for (int i = 0; i < OPT_COUNT; i++)
            txtOptions[i].emplace(font, "", 13u);

        return true;
    }

    void setUsername(const string& name) { username = name; }
    void reset() { selectedOption = 0; }

    MainMenuResult handleEvent(sf::Event& event) {
        if (auto* kp = event.getIf<sf::Event::KeyPressed>()) {
            if (kp->code == sf::Keyboard::Key::Up)
                selectedOption = (selectedOption - 1 + OPT_COUNT) % OPT_COUNT;
            else if (kp->code == sf::Keyboard::Key::Down)
                selectedOption = (selectedOption + 1) % OPT_COUNT;
            else if (kp->code == sf::Keyboard::Key::Enter) {
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

        // ── Background ────────────────────────────────────────────────────
        if (bgLoaded) window.draw(*bgSprite);
        else {
            sf::RectangleShape fb({ 800.f, 600.f });
            fb.setFillColor(sf::Color(5, 12, 35, 255));
            window.draw(fb);
        }

        // Very subtle dark overlay — keeps art visible, text readable
        sf::RectangleShape vignette({ 800.f, 600.f });
        vignette.setFillColor(sf::Color(0, 5, 20, 70));
        window.draw(vignette);

        // ── Title ─────────────────────────────────────────────────────────
        TX(txtTitle).setString("SNOW  BROS");
        TX(txtTitle).setCharacterSize(30u);
        TX(txtTitle).setFillColor(titleCol);
        TX(txtTitle).setStyle(sf::Text::Bold);
        TX(txtTitle).setLetterSpacing(5.f);
        centreText(TX(txtTitle), 78.f);
        window.draw(TX(txtTitle));

        // thin ice divider under title
        dividerTop.setSize({ 260.f, 1.5f });
        dividerTop.setPosition({ CX - 130.f, 130.f });
        dividerTop.setFillColor(sf::Color(120, 200, 255, 80));
        window.draw(dividerTop);

        // ── Subtitle (welcome) ────────────────────────────────────────────
        TX(txtSub).setString("Welcome back,  " + username + "!");
        TX(txtSub).setCharacterSize(11u);
        TX(txtSub).setFillColor(subCol);
        TX(txtSub).setStyle(sf::Text::Regular);
        centreText(TX(txtSub), 148.f);
        window.draw(TX(txtSub));

        // ── Buttons ───────────────────────────────────────────────────────
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

        // ── Hint ──────────────────────────────────────────────────────────
        TX(txtHint).setString("Arrows to navigate   |   Enter to select");
        TX(txtHint).setCharacterSize(9u);
        TX(txtHint).setFillColor(hintCol);
        centreText(TX(txtHint), 570.f);
        window.draw(TX(txtHint));
    }
};