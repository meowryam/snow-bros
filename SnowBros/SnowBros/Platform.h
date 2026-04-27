/*#pragma once
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
*/

#pragma once
#include <SFML/Graphics.hpp>

struct Platform {
    sf::FloatRect rect;
    sf::Color     color;
    sf::Texture* tileTexture = nullptr;  // ADD THIS

    Platform() {}
    Platform(float x, float y, float w, float h, sf::Color c = sf::Color(80, 140, 200, 255))
        : rect(sf::Vector2f(x, y), sf::Vector2f(w, h)), color(c) {
    }

    void draw(sf::RenderWindow& window, bool showHitbox = false) const {
        if (tileTexture) {
            float tileW = static_cast<float>(tileTexture->getSize().x);
            float tileH = static_cast<float>(tileTexture->getSize().y);
            float scaleY = rect.size.y / tileH;
            float scaledTileW = tileW * scaleY; // tile width after scaling to match platform height
            float x = rect.position.x;
            float remaining = rect.size.x;

            while (remaining > 0) {
                float drawW = std::min(remaining, scaledTileW);
                sf::Sprite s(*tileTexture);
                s.setTextureRect(sf::IntRect({ 0, 0 }, { (int)(drawW / scaleY), (int)tileH }));
                s.setPosition({ x, rect.position.y });
                s.setScale({ scaleY, scaleY });
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