#pragma once
#pragma once
#include <SFML/Graphics.hpp>
using namespace std;

enum class MainMenuResult {
    NONE,
    START_GAME,
    START_2PLAYER,
    OPEN_SHOP,
    OPEN_KEYREMAP,
    LOGOUT,
    QUIT
};

class MainMenu {
private:
    sf::Font font;
    optional<sf::Text> titleText;
    optional<sf::Text> subtitleText;
    optional<sf::Text> optionTexts[6];
    optional<sf::Text> hintText;

    sf::RectangleShape background;
    sf::RectangleShape leftBar;   // decorative side bars
    sf::RectangleShape rightBar;

    int selectedOption;
    string username;

    sf::Color bgColor = sf::Color(10, 20, 60, 255);
    sf::Color titleColor = sf::Color(80, 220, 255, 255);
    sf::Color subColor = sf::Color(150, 255, 180, 255);
    sf::Color selectedCol = sf::Color(255, 220, 50, 255);
    sf::Color normalCol = sf::Color(160, 180, 255, 255);
    sf::Color barColor = sf::Color(80, 220, 255, 60);
    sf::Color hintCol = sf::Color(100, 120, 180, 255);

    sf::Text& T(optional<sf::Text>& t) { return t.value(); }

public:
    MainMenu() : selectedOption(0) {}

    bool loadFont(const string& path) {
        if (!font.openFromFile(path)) return false;
        titleText.emplace(font);
        subtitleText.emplace(font);
        hintText.emplace(font);
        for (int i = 0; i < 6; i++)
            optionTexts[i].emplace(font);
        return true;
    }

    void setUsername(const string& name) { username = name; }
    void reset() { selectedOption = 0; }

    MainMenuResult handleEvent(sf::Event& event) {
        if (auto* kp = event.getIf<sf::Event::KeyPressed>()) {
            if (kp->code == sf::Keyboard::Key::Up)
                selectedOption = (selectedOption - 1 + 6) % 6;  // was 5
            else if (kp->code == sf::Keyboard::Key::Down)
                selectedOption = (selectedOption + 1) % 6;       // was 5
            else if (kp->code == sf::Keyboard::Key::Enter) {
                if (selectedOption == 0) return MainMenuResult::START_GAME;
                if (selectedOption == 1) return MainMenuResult::START_2PLAYER; // NEW
                if (selectedOption == 2) return MainMenuResult::OPEN_SHOP;
                if (selectedOption == 3) return MainMenuResult::OPEN_KEYREMAP;
                if (selectedOption == 4) return MainMenuResult::LOGOUT;
                if (selectedOption == 5) return MainMenuResult::QUIT;
            }
        }
        return MainMenuResult::NONE;
    }

    void draw(sf::RenderWindow& window) {
        float W = (float)window.getSize().x;
        float H = (float)window.getSize().y;

        // background
        background.setSize({ W, H });
        background.setFillColor(bgColor);
        window.draw(background);

        // decorative side bars
        leftBar.setSize({ 6.f, H });
        leftBar.setPosition({ 0.f, 0.f });
        leftBar.setFillColor(barColor);
        window.draw(leftBar);

        rightBar.setSize({ 6.f, H });
        rightBar.setPosition({ W - 6.f, 0.f });
        rightBar.setFillColor(barColor);
        window.draw(rightBar);

        // title
        T(titleText).setString("SNOW BROS");
        T(titleText).setCharacterSize(56);
        T(titleText).setFillColor(titleColor);
        T(titleText).setLetterSpacing(2.f);
        sf::FloatRect tb = T(titleText).getLocalBounds();
        T(titleText).setOrigin({ tb.size.x / 2.f, 0.f });
        T(titleText).setPosition({ W / 2.f, 70.f });
        window.draw(T(titleText));

        // subtitle with username
        T(subtitleText).setString("Welcome Back " + username + "!");
        T(subtitleText).setCharacterSize(16);
        T(subtitleText).setFillColor(subColor);
        sf::FloatRect sub = T(subtitleText).getLocalBounds();
        T(subtitleText).setOrigin({ sub.size.x / 2.f, 0.f });
        T(subtitleText).setPosition({ W / 2.f, 170.f });
        window.draw(T(subtitleText));

        // menu options
        string opts[6] = { "1 Player", "2 Player", "Shop", "Key Bindings", "Logout", "Quit" };
        for (int i = 0; i < 6; i++) {
            T(optionTexts[i]).setString((selectedOption == i ? ">  " : "   ") + opts[i]);
            T(optionTexts[i]).setCharacterSize(26);
            T(optionTexts[i]).setFillColor(selectedOption == i ? selectedCol : normalCol);
            sf::FloatRect ob = T(optionTexts[i]).getLocalBounds();
            T(optionTexts[i]).setOrigin({ ob.size.x / 2.f, 0.f });
            T(optionTexts[i]).setPosition({ W / 2.f - 50.f, 220.f + i * 55.f });
            window.draw(T(optionTexts[i]));
        }

        // hint
        T(hintText).setString("Up/Down to navigate  |  Enter to select");
        T(hintText).setCharacterSize(12);
        T(hintText).setFillColor(hintCol);
        sf::FloatRect hb = T(hintText).getLocalBounds();
        T(hintText).setOrigin({ hb.size.x / 2.f, 0.f });
        T(hintText).setPosition({ W / 2.f, H - 30.f });
        window.draw(T(hintText));
    }
};