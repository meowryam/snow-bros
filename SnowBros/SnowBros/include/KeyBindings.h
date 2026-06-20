#pragma once
#include <SFML/Window.hpp>
#include <fstream>
#include <string>
using namespace std;

struct KeyBindings 
{
    sf::Keyboard::Key moveLeft;
    sf::Keyboard::Key moveRight;
    sf::Keyboard::Key jump;
    sf::Keyboard::Key throwSnowball;
    sf::Keyboard::Key pause;

    KeyBindings()
    {
        moveLeft = sf::Keyboard::Key::A;
        moveRight = sf::Keyboard::Key::D;
        jump = sf::Keyboard::Key::W;
        throwSnowball = sf::Keyboard::Key::Space;
        pause = sf::Keyboard::Key::Escape;
    }

    void saveToFile(const string& username)
    {
        ofstream file("saves/" + username + "_keys.txt");
        if (!file.is_open()) 
            return;
        file << "left=" << (int)moveLeft << "\n";
        file << "right=" << (int)moveRight << "\n";
        file << "jump=" << (int)jump << "\n";
        file << "throw=" << (int)throwSnowball << "\n";
        file << "pause=" << (int)pause << "\n";
    }

    void loadFromFile(const string& username)
    {
        ifstream file("saves/" + username + "_keys.txt");
        if (!file.is_open()) return; 
        string line;

        while (getline(file, line)) 
        {
            istringstream ss(line);
            string key, value;

            if (getline(ss, key, '=') && getline(ss, value))
            {
                int code = stoi(value);
                if (key == "left") 
                    moveLeft = (sf::Keyboard::Key)code;

                else if (key == "right")
                    moveRight = (sf::Keyboard::Key)code;

                else if (key == "jump") 
                    jump = (sf::Keyboard::Key)code;

                else if (key == "throw")
                    throwSnowball = (sf::Keyboard::Key)code;

                else if (key == "pause")
                    pause = (sf::Keyboard::Key)code;
            }
        }
    }
};