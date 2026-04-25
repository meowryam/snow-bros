#pragma once
#include <SFML/Graphics.hpp>

// A single solid platform — player and enemies land on these
struct Platform {
    sf::FloatRect rect;  // position and size
    sf::Color     color;

    Platform() {}
    Platform(float x, float y, float w, float h, sf::Color c = sf::Color(80, 140, 200, 255))
        : rect(sf::Vector2f(x, y), sf::Vector2f(w, h)), color(c) {
    }

    void draw(sf::RenderWindow& window, bool showHitbox = false) const {
        sf::RectangleShape shape(rect.size);
        shape.setPosition(rect.position);
        shape.setFillColor(color);
        window.draw(shape);

        if (showHitbox) {
            shape.setFillColor(sf::Color::Transparent);
            shape.setOutlineThickness(1.f);
            shape.setOutlineColor(sf::Color::Blue);
            window.draw(shape);
        }
    }
};