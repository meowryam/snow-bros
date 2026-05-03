#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

class InputManager
{
public:

    bool isKeyHeld(sf::Keyboard::Key key) const
    {
        return sf::Keyboard::isKeyPressed(key);
    }

    bool processEvents(sf::RenderWindow& window)
    {
        while (auto event = window.pollEvent()) 
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
                return false;
            }
        }
        return true;
    }
};