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
private:
    sf::Font font;
    optional<sf::Text> titleText;
    optional<sf::Text> optionTexts[4];
    optional<sf::Text> hintText;

    sf::RectangleShape overlay;
    sf::RectangleShape panel;

    int selectedOption;

    // bright theme
    sf::Color overlayColor = sf::Color(0, 0, 0, 160);
    sf::Color panelColor = sf::Color(20, 30, 80, 230);
    sf::Color titleColor = sf::Color(100, 220, 255, 255);
    sf::Color selectedCol = sf::Color(255, 220, 50, 255);
    sf::Color normalCol = sf::Color(180, 180, 255, 255);
    sf::Color hintCol = sf::Color(120, 120, 180, 255);

    sf::Text& T(optional<sf::Text>& t) { return t.value(); }

public:
    PauseScreen() : selectedOption(0) {}

    bool loadFont(const string& path) {
        if (!font.openFromFile(path)) return false;
        titleText.emplace(font);    // "PAUSED" in Orbitron
        hintText.emplace(font);
        for (int i = 0; i < 4; i++)
            optionTexts[i].emplace(font);
        return true;
    }



    void reset() { selectedOption = 0; }

    PauseResult handleEvent(sf::Event& event) {
        if (auto* kp = event.getIf<sf::Event::KeyPressed>()) {
            if (kp->code == sf::Keyboard::Key::Up)
                selectedOption = (selectedOption - 1 + 4) % 4;
            else if (kp->code == sf::Keyboard::Key::Down)
                selectedOption = (selectedOption + 1) % 4;
            else if (kp->code == sf::Keyboard::Key::Escape)
                return PauseResult::RESUME;
            else if (kp->code == sf::Keyboard::Key::Enter) {
                if (selectedOption == 0) return PauseResult::RESUME;
                if (selectedOption == 1) return PauseResult::OPEN_SHOP;
                if (selectedOption == 2) return PauseResult::OPEN_KEYREMAP;
                if (selectedOption == 3) return PauseResult::QUIT_TO_MENU;
            }
        }
        return PauseResult::NONE;
    }

    void draw(sf::RenderWindow& window) {
        float W = (float)window.getSize().x;
        float H = (float)window.getSize().y;

        // dim the game behind
        overlay.setSize({ W, H });
        overlay.setFillColor(overlayColor);
        window.draw(overlay);

        // panel
        const float panelW = 360.f, panelH = 320.f;
        panel.setSize({ panelW, panelH });
        panel.setPosition({ W / 2.f - panelW / 2.f, H / 2.f - panelH / 2.f });
        panel.setFillColor(panelColor);
        panel.setOutlineThickness(2.f);
        panel.setOutlineColor(titleColor);
        window.draw(panel);

        // title
        T(titleText).setString("PAUSED");
        T(titleText).setCharacterSize(36);
        T(titleText).setFillColor(titleColor);
        T(titleText).setLetterSpacing(4.f);
        sf::FloatRect tb = T(titleText).getLocalBounds();
        T(titleText).setOrigin({ tb.size.x / 2.f, 0.f });
        T(titleText).setPosition({ W / 2.f, H / 2.f - panelH / 2.f + 24.f });
        window.draw(T(titleText));

        // options
        string opts[4] = { "Resume", "Shop", "Key Bindings", "Quit to Menu" };
        for (int i = 0; i < 4; i++) {
            T(optionTexts[i]).setString((selectedOption == i ? "> " : "  ") + opts[i]);
            T(optionTexts[i]).setCharacterSize(22);
            T(optionTexts[i]).setFillColor(selectedOption == i ? selectedCol : normalCol);
            sf::FloatRect ob = T(optionTexts[i]).getLocalBounds();
            T(optionTexts[i]).setOrigin({ ob.size.x / 2.f, 0.f });
            T(optionTexts[i]).setPosition({ W / 2.f, H / 2.f - 60.f + i * 50.f });
            window.draw(T(optionTexts[i]));
        }

        // hint
        T(hintText).setString("arrows to navigate  |  Enter to select  |  Esc to resume");
        T(hintText).setCharacterSize(11);
        T(hintText).setFillColor(hintCol);
        sf::FloatRect hb = T(hintText).getLocalBounds();
        T(hintText).setOrigin({ hb.size.x / 2.f, 0.f });
        T(hintText).setPosition({ W / 2.f, H / 2.f + panelH / 2.f - 28.f });
        window.draw(T(hintText));
    }
};