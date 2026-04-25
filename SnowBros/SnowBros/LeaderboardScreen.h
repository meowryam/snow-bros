#pragma once
#include <SFML/Graphics.hpp>
#include "Leaderboard.h"
using namespace std;

class LeaderboardScreen {
private:
    sf::Font font;
    optional<sf::Text> titleText;
    optional<sf::Text> rowTexts[10];
    optional<sf::Text> headerText;
    optional<sf::Text> hintText;
    optional<sf::Text> emptyText;

    sf::RectangleShape background;
    sf::RectangleShape panel;
    sf::RectangleShape rowHighlight;

    Leaderboard& leaderboard;

    sf::Color bgColor = sf::Color(10, 20, 60, 255);
    sf::Color panelColor = sf::Color(15, 30, 80, 230);
    sf::Color titleColor = sf::Color(80, 220, 255, 255);
    sf::Color goldColor = sf::Color(255, 215, 0, 255);
    sf::Color silver = sf::Color(192, 192, 192, 255);
    sf::Color bronze = sf::Color(205, 127, 50, 255);
    sf::Color normalCol = sf::Color(180, 200, 255, 255);
    sf::Color hintCol = sf::Color(100, 120, 180, 255);

    sf::Text& T(optional<sf::Text>& t) { return t.value(); }

public:
    bool done;

    LeaderboardScreen(Leaderboard& lb) : leaderboard(lb), done(false) {}

    bool loadFont(const string& path) {
        if (!font.openFromFile(path)) return false;
        titleText.emplace(font);
        headerText.emplace(font);
        hintText.emplace(font);
        emptyText.emplace(font);
        for (int i = 0; i < 10; i++)
            rowTexts[i].emplace(font);
        return true;
    }

    void handleEvent(sf::Event& event) {
        if (auto* kp = event.getIf<sf::Event::KeyPressed>())
            if (kp->code == sf::Keyboard::Key::Escape ||
                kp->code == sf::Keyboard::Key::Enter)
                done = true;
    }

    void draw(sf::RenderWindow& window) {
        float W = (float)window.getSize().x;
        float H = (float)window.getSize().y;

        background.setSize({ W, H });
        background.setFillColor(bgColor);
        window.draw(background);

        const float panelW = 620.f, panelH = 480.f;
        panel.setSize({ panelW, panelH });
        panel.setPosition({ W / 2.f - panelW / 2.f, H / 2.f - panelH / 2.f });
        panel.setFillColor(panelColor);
        panel.setOutlineThickness(2.f);
        panel.setOutlineColor(titleColor);
        window.draw(panel);

        // title
        T(titleText).setString("LEADERBOARD");
        T(titleText).setCharacterSize(32);
        T(titleText).setFillColor(titleColor);
        T(titleText).setLetterSpacing(4.f);
        sf::FloatRect tb = T(titleText).getLocalBounds();
        T(titleText).setOrigin({ tb.size.x / 2.f, 0.f });
        T(titleText).setPosition({ W / 2.f, H / 2.f - panelH / 2.f + 18.f });
        window.draw(T(titleText));

        // header row
        T(headerText).setString("#    NAME                SCORE      LEVEL    DATE");
        T(headerText).setCharacterSize(13);
        T(headerText).setFillColor(sf::Color(150, 180, 255, 180));
        T(headerText).setPosition({ W / 2.f - panelW / 2.f + 24.f, H / 2.f - panelH / 2.f + 72.f });
        window.draw(T(headerText));

        if (leaderboard.getCount() == 0) {
            T(emptyText).setString("no scores yet — be the first!");
            T(emptyText).setCharacterSize(18);
            T(emptyText).setFillColor(normalCol);
            sf::FloatRect eb = T(emptyText).getLocalBounds();
            T(emptyText).setOrigin({ eb.size.x / 2.f, 0.f });
            T(emptyText).setPosition({ W / 2.f, H / 2.f });
            window.draw(T(emptyText));
        }
        else {
            for (int i = 0; i < leaderboard.getCount(); i++) {
                const LeaderboardEntry& e = leaderboard.getEntry(i);
                float rowY = H / 2.f - panelH / 2.f + 100.f + i * 36.f;

                // highlight top 3
                if (i < 3) {
                    rowHighlight.setSize({ panelW - 20.f, 30.f });
                    rowHighlight.setPosition({ W / 2.f - panelW / 2.f + 10.f, rowY - 4.f });
                    rowHighlight.setFillColor(sf::Color(255, 255, 255, i == 0 ? 18 : 10));
                    window.draw(rowHighlight);
                }

                sf::Color col = (i == 0) ? goldColor : (i == 1) ? silver :
                    (i == 2) ? bronze : normalCol;

                // pad strings manually (no setw without iomanip)
                string rank = to_string(i + 1) + ".";
                string name = e.username.substr(0, 16);
                string score = to_string(e.score);
                string level = to_string(e.levelReached);

                // spaces for alignment
                while (rank.size() < 5)  rank += " ";
                while (name.size() < 20) name += " ";
                while (score.size() < 10) score += " ";
                while (level.size() < 8)  level += " ";

                T(rowTexts[i]).setString(rank + name + score + level + e.date);
                T(rowTexts[i]).setCharacterSize(15);
                T(rowTexts[i]).setFillColor(col);
                T(rowTexts[i]).setPosition({ W / 2.f - panelW / 2.f + 24.f, rowY });
                window.draw(T(rowTexts[i]));
            }
        }

        // hint
        T(hintText).setString("Enter or Esc to go back");
        T(hintText).setCharacterSize(12);
        T(hintText).setFillColor(hintCol);
        sf::FloatRect hb = T(hintText).getLocalBounds();
        T(hintText).setOrigin({ hb.size.x / 2.f, 0.f });
        T(hintText).setPosition({ W / 2.f, H / 2.f + panelH / 2.f - 24.f });
        window.draw(T(hintText));
    }
};