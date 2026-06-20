
#pragma once
#include <SFML/Graphics.hpp>

struct Platform {
    sf::FloatRect rect;
    sf::Color     color;
    sf::Texture* tileTexture = nullptr;  

    Platform() {}
    Platform(float x, float y, float w, float h, sf::Color c = sf::Color(80, 140, 200, 255))
        : rect(sf::Vector2f(x, y), sf::Vector2f(w, h)), color(c) {
    }
    void draw(sf::RenderWindow& window, bool showHitbox = false) const {
        if (tileTexture) {
            float tileW = 200.f; 
            float x = rect.position.x;
            float remaining = rect.size.x;

            while (remaining > 0.5f) {
                float drawW = std::min(remaining, tileW);
                sf::Sprite s(*tileTexture);
                s.setTextureRect(sf::IntRect({ 0, 0 }, { (int)drawW, 24 }));
                s.setPosition({ x, rect.position.y });
                window.draw(s);
                x += drawW;
                remaining -= drawW;
            }
        }
    else {
        sf::RectangleShape shape(rect.size);
        shape.setPosition(rect.position);
        shape.setFillColor(color);
        window.draw(shape);
    }

    if (showHitbox) {
        sf::RectangleShape shape(rect.size);
        shape.setPosition(rect.position);
        shape.setFillColor(sf::Color::Transparent);
        shape.setOutlineThickness(1.f);
        shape.setOutlineColor(sf::Color::Blue);
        window.draw(shape);
    }
}
};