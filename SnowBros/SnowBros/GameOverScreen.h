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
private:
    sf::Font font;
    optional<sf::Text> titleText;
    optional<sf::Text> statsText;
    optional<sf::Text> optionTexts[2];
    optional<sf::Text> hintText;

    sf::RectangleShape background;
    sf::RectangleShape panel;

    int selectedOption;

    sf::Color bgColor = sf::Color(40, 0, 0, 255);
    sf::Color panelColor = sf::Color(80, 10, 10, 230);
    sf::Color redTitle = sf::Color(255, 60, 60, 255);
    sf::Color selectedCol = sf::Color(255, 220, 50, 255);
    sf::Color normalCol = sf::Color(220, 160, 160, 255);
    sf::Color statsCol = sf::Color(200, 200, 255, 255);
    sf::Color hintCol = sf::Color(150, 100, 100, 255);

    sf::Text& T(optional<sf::Text>& t) { return t.value(); }

public:
    GameOverScreen() : selectedOption(0) {}

    bool loadFont(const string& path) {
        if (!font.openFromFile(path)) return false;
        titleText.emplace(font);
        statsText.emplace(font);
        hintText.emplace(font);
        for (int i = 0; i < 2; i++)
            optionTexts[i].emplace(font);
        return true;
    }

    void reset() { selectedOption = 0; }

    GameOverResult handleEvent(sf::Event& event) {
        if (auto* kp = event.getIf<sf::Event::KeyPressed>()) {
            if (kp->code == sf::Keyboard::Key::Up ||
                kp->code == sf::Keyboard::Key::Down)
                selectedOption = 1 - selectedOption; // toggle between 0 and 1
            else if (kp->code == sf::Keyboard::Key::Enter) {
                if (selectedOption == 0) return GameOverResult::RETRY;
                if (selectedOption == 1) return GameOverResult::QUIT_TO_MENU;
            }
        }
        return GameOverResult::NONE;
    }

    void draw(sf::RenderWindow& window, const PlayerData& data) {
        float W = (float)window.getSize().x;
        float H = (float)window.getSize().y;

        background.setSize({ W, H });
        background.setFillColor(bgColor);
        window.draw(background);

        const float panelW = 400.f, panelH = 340.f;
        panel.setSize({ panelW, panelH });
        panel.setPosition({ W / 2.f - panelW / 2.f, H / 2.f - panelH / 2.f });
        panel.setFillColor(panelColor);
        panel.setOutlineThickness(2.f);
        panel.setOutlineColor(redTitle);
        window.draw(panel);

        // title
        T(titleText).setString("GAME OVER");
        T(titleText).setCharacterSize(42);
        T(titleText).setFillColor(redTitle);
        T(titleText).setLetterSpacing(3.f);
        sf::FloatRect tb = T(titleText).getLocalBounds();
        T(titleText).setOrigin({ tb.size.x / 2.f, 0.f });
        T(titleText).setPosition({ W / 2.f, H / 2.f - panelH / 2.f + 20.f });
        window.draw(T(titleText));

        // stats
        string stats = "Level: " + to_string(data.getCurrentLevel()) +
            "   Gems: " + to_string(data.getGemCount()) +
            "   Best: " + to_string(data.getHighscore());
        T(statsText).setString(stats);
        T(statsText).setCharacterSize(16);
        T(statsText).setFillColor(statsCol);
        sf::FloatRect sb = T(statsText).getLocalBounds();
        T(statsText).setOrigin({ sb.size.x / 2.f, 0.f });
        T(statsText).setPosition({ W / 2.f, H / 2.f - 50.f });
        window.draw(T(statsText));

        // options
        string opts[2] = { "Retry", "Quit to Menu" };
        for (int i = 0; i < 2; i++) {
            T(optionTexts[i]).setString((selectedOption == i ? "> " : "  ") + opts[i]);
            T(optionTexts[i]).setCharacterSize(24);
            T(optionTexts[i]).setFillColor(selectedOption == i ? selectedCol : normalCol);
            sf::FloatRect ob = T(optionTexts[i]).getLocalBounds();
            T(optionTexts[i]).setOrigin({ ob.size.x / 2.f, 0.f });
            T(optionTexts[i]).setPosition({ W / 2.f, H / 2.f + 20.f + i * 55.f });
            window.draw(T(optionTexts[i]));
        }

        // hint
        T(hintText).setString("arrows to navigate  |  Enter to select");
        T(hintText).setCharacterSize(11);
        T(hintText).setFillColor(hintCol);
        sf::FloatRect hb = T(hintText).getLocalBounds();
        T(hintText).setOrigin({ hb.size.x / 2.f, 0.f });
        T(hintText).setPosition({ W / 2.f, H / 2.f + panelH / 2.f - 24.f });
        window.draw(T(hintText));
    }
};