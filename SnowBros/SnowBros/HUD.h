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
    optional<sf::Text> gemsText;
    optional<sf::Text> levelText;
    optional<sf::Text> scoreText;

    sf::RectangleShape topBar;

    // bright colorful theme
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
        gemsText.emplace(font);
        levelText.emplace(font);
        scoreText.emplace(font);
        return true;
    }

    void draw(sf::RenderWindow& window, const PlayerData& data) 
    {
        float W = (float)window.getSize().x;

        // top bar background
        topBar.setSize({ W, 48.f });
        topBar.setPosition({ 0.f, 0.f });
        topBar.setFillColor(barColor);
        window.draw(topBar);

        // lives
        T(livesText).setString("LIVES: " + to_string(data.getLives()));
        T(livesText).setCharacterSize(16);
        T(livesText).setFillColor(white);
        T(livesText).setPosition({ 20.f, 12.f });
        window.draw(T(livesText));

        // gems
        T(gemsText).setString("GEMS: " + to_string(data.getGemCount()));
        T(gemsText).setCharacterSize(16);
        T(gemsText).setFillColor(white);
        T(gemsText).setPosition({ 200.f, 12.f });
        window.draw(T(gemsText));

        // level — centred
        T(levelText).setString("LEVEL " + to_string(data.getCurrentLevel()));
        T(levelText).setCharacterSize(16);
        T(levelText).setFillColor(white);
        sf::FloatRect lb = T(levelText).getLocalBounds();
        T(levelText).setOrigin({ lb.size.x / 2.f, 0.f });
        T(levelText).setPosition({ W / 2.f + 50.f, 12.f });
        window.draw(T(levelText));

        // score — right side
        T(scoreText).setString("SCORE: " + to_string(data.getHighscore()));
        T(scoreText).setCharacterSize(16);
        T(scoreText).setFillColor(white);
        sf::FloatRect sb = T(scoreText).getLocalBounds();
        T(scoreText).setOrigin({ sb.size.x, 0.f });
        T(scoreText).setPosition({ W - 20.f, 12.f });
        window.draw(T(scoreText));
    }
};