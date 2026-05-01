#pragma once
#include <SFML/Graphics.hpp>
#include "PlayerData.h"
using namespace std;

// In-game overlay — shows lives, gems, level, score
class HUD
{
private:
    sf::Font font;
    optional<sf::Text> livesText;
    optional<sf::Text> p1LivesText;
    optional<sf::Text> p2LivesText;
    optional<sf::Text> gemsText;
    optional<sf::Text> levelText;
    optional<sf::Text> scoreText;

    sf::RectangleShape topBar;

    sf::Color barColor = sf::Color(15, 15, 40, 200);
    sf::Color redLife = sf::Color(255, 80, 80, 255);
    sf::Color gemBlue = sf::Color(80, 200, 255, 255);
    sf::Color yellow = sf::Color(255, 220, 50, 255);
    sf::Color white = sf::Color(255, 255, 255, 255);

    sf::Text& T(optional<sf::Text>& t) 
    { 
        return t.value();
    }

public:
    bool loadFont(const string& path)
    {
        if (!font.openFromFile(path))
            return false;
        livesText.emplace(font);
        p1LivesText.emplace(font);
        p2LivesText.emplace(font);
        gemsText.emplace(font);
        levelText.emplace(font);
        scoreText.emplace(font);
        return true;
    }

    

    void draw(sf::RenderWindow& window, const PlayerData& data, const PlayerData* data2 = nullptr)
    {
        float W = (float)window.getSize().x;

        // top bar background
        topBar.setSize({ W, 48.f });
        topBar.setPosition({ 0.f, 0.f });
        topBar.setFillColor(barColor);
        window.draw(topBar);

        if (data2 == nullptr) {
            // ?? Single player layout (your existing code) ??
            T(livesText).setString("LIVES: " + to_string(data.getLives()));
            T(livesText).setCharacterSize(16);
            T(livesText).setFillColor(white);
            T(livesText).setPosition({ 20.f, 12.f });
            window.draw(T(livesText));

            T(gemsText).setString("GEMS: " + to_string(data.getGemCount()));
            T(gemsText).setCharacterSize(16);
            T(gemsText).setFillColor(white);
            T(gemsText).setPosition({ 200.f, 12.f });
            window.draw(T(gemsText));

            T(levelText).setString("LEVEL " + to_string(data.getCurrentLevel()));
            T(levelText).setCharacterSize(16);
            T(levelText).setFillColor(white);
            sf::FloatRect lb = T(levelText).getLocalBounds();
            T(levelText).setOrigin({ lb.size.x / 2.f, 0.f });
            T(levelText).setPosition({ W / 2.f + 50.f, 12.f });
            window.draw(T(levelText));

            T(scoreText).setString("SCORE: " + to_string(data.getHighscore()));
            T(scoreText).setCharacterSize(16);
            T(scoreText).setFillColor(white);
            sf::FloatRect sb = T(scoreText).getLocalBounds();
            T(scoreText).setOrigin({ sb.size.x, 0.f });
            T(scoreText).setPosition({ W - 20.f, 12.f });
            window.draw(T(scoreText));
        }
        //else {
        //    // ?? Multiplayer layout ??
        //    // Player 1 on the LEFT side
        //    T(livesText).setString("P1 LV: " + to_string(data.getLives()));
        //    T(livesText).setCharacterSize(11);
        //    T(livesText).setFillColor(sf::Color(100, 220, 255, 255)); // blue tint for P1
        //    T(livesText).setPosition({ 10.f, 4.f });
        //    window.draw(T(livesText));

        //    T(gemsText).setString("P1 GM: " + to_string(data.getGemCount()));
        //    T(gemsText).setCharacterSize(11);
        //    T(gemsText).setFillColor(sf::Color(100, 220, 255, 255));
        //    T(gemsText).setPosition({ 10.f, 24.f });
        //    window.draw(T(gemsText));

        //    // Level in the CENTER
        //    T(levelText).setString("LEVEL " + to_string(data.getCurrentLevel()));
        //    T(levelText).setCharacterSize(13);
        //    T(levelText).setFillColor(white);
        //    sf::FloatRect lb = T(levelText).getLocalBounds();
        //    T(levelText).setOrigin({ lb.size.x / 2.f, 0.f });
        //    T(levelText).setPosition({ W / 2.f, 14.f });
        //    window.draw(T(levelText));

        //    // Player 2 on the RIGHT side
        //    T(scoreText).setString("P2 LV: " + to_string(data2->getLives()));
        //    T(scoreText).setCharacterSize(11);
        //    T(scoreText).setFillColor(sf::Color(255, 180, 80, 255)); // orange tint for P2
        //    sf::FloatRect sb = T(scoreText).getLocalBounds();
        //    T(scoreText).setOrigin({ sb.size.x, 0.f });
        //    T(scoreText).setPosition({ W - 10.f, 4.f });
        //    window.draw(T(scoreText));

        //    T(livesText).setString("P2 GM: " + to_string(data2->getGemCount()));
        //    T(livesText).setCharacterSize(11);
        //    T(livesText).setFillColor(sf::Color(255, 180, 80, 255));
        //    sf::FloatRect lb2 = T(livesText).getLocalBounds();
        //    T(livesText).setOrigin({ lb2.size.x, 0.f });
        //    T(livesText).setPosition({ W - 10.f, 24.f });
        //    window.draw(T(livesText));
        //}

        else {
            // P1 LEFT — use dedicated objects
            T(p1LivesText).setString("P1 LV: " + to_string(data.getLives()));
            T(p1LivesText).setCharacterSize(11);
            T(p1LivesText).setFillColor(sf::Color(100, 220, 255, 255));
            T(p1LivesText).setPosition({ 10.f, 4.f });
            window.draw(T(p1LivesText));

            T(gemsText).setString("P1 GM: " + to_string(data.getGemCount()));
            T(gemsText).setCharacterSize(11);
            T(gemsText).setFillColor(sf::Color(100, 220, 255, 255));
            T(gemsText).setPosition({ 10.f, 24.f });
            window.draw(T(gemsText));

            // CENTER
            T(levelText).setString("LEVEL " + to_string(data.getCurrentLevel()));
            T(levelText).setCharacterSize(13);
            T(levelText).setFillColor(white);
            sf::FloatRect lb = T(levelText).getLocalBounds();
            T(levelText).setOrigin({ lb.size.x / 2.f, 0.f });
            T(levelText).setPosition({ W / 2.f, 14.f });
            window.draw(T(levelText));

            // P2 RIGHT — use dedicated objects
            T(p2LivesText).setString("P2 LV: " + to_string(data2->getLives()));
            T(p2LivesText).setCharacterSize(11);
            T(p2LivesText).setFillColor(sf::Color(255, 180, 80, 255));
            sf::FloatRect sb = T(p2LivesText).getLocalBounds();
            T(p2LivesText).setOrigin({ sb.size.x, 0.f });
            T(p2LivesText).setPosition({ W - 10.f, 4.f });
            window.draw(T(p2LivesText));

            T(scoreText).setString("P2 GM: " + to_string(data2->getGemCount()));
            T(scoreText).setCharacterSize(11);
            T(scoreText).setFillColor(sf::Color(255, 180, 80, 255));
            sf::FloatRect sb2 = T(scoreText).getLocalBounds();
            T(scoreText).setOrigin({ sb2.size.x, 0.f });
            T(scoreText).setPosition({ W - 10.f, 24.f });
            window.draw(T(scoreText));
        }
    }
};