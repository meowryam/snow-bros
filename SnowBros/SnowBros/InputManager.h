#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

// This class handles ALL keyboard input
// No other part of the game checks the keyboard directly
// Everything goes through here
class InputManager {
public:

    // Returns true if a key is currently being held down
    bool isKeyHeld(sf::Keyboard::Key key) const {
        return sf::Keyboard::isKeyPressed(key);
    }

    // This checks if the X button was clicked to close the window
    // Returns false if window was closed
    bool processEvents(sf::RenderWindow& window) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
                return false;
            }
        }
        return true;
    }
};