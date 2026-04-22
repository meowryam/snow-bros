#include <SFML/Graphics.hpp>
#include <cmath>

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Heart");

    const int numPoints = 100;
    sf::VertexArray heart(sf::PrimitiveType::TriangleFan, numPoints + 2);

    // Center point
    heart[0].position = sf::Vector2f(400, 300);
    heart[0].color = sf::Color::Red;

    for (int i = 0; i <= numPoints; i++) {
        float t = i * 2 * 3.14159f / numPoints;

        // Heart parametric equation
        float x = 16 * pow(sin(t), 3);
        float y = -(13 * cos(t) - 5 * cos(2*t) - 2 * cos(3*t) - cos(4*t));

        // Scale and center
        heart[i + 1].position = sf::Vector2f(400 + x * 10, 300 + y * 10);
        heart[i + 1].color = sf::Color::Red;
    }

    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }
        window.clear(sf::Color::Black);
        window.draw(heart);
        window.display();
    }

    return 0;
}