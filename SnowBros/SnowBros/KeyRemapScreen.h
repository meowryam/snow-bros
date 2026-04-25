#pragma once
#include <SFML/Graphics.hpp>
#include "KeyBindings.h"
using namespace std;

// Screen where player can remap their keys
// Press Enter on a binding, then press the new key
class KeyRemapScreen {
private:
    sf::Font font;
    KeyBindings& bindings;

    optional<sf::Text> titleText;
    optional<sf::Text> actionTexts[5];
    optional<sf::Text> keyTexts[5];
    optional<sf::Text> hintText;

    sf::RectangleShape background;
    sf::RectangleShape highlight;

    int selectedRow;      // which binding is selected
    bool waitingForKey;   // true = next keypress becomes the new binding

    sf::Color bgColor = sf::Color(26, 10, 46, 255);
    sf::Color gold = sf::Color(255, 229, 102, 255);
    sf::Color lightText = sf::Color(232, 224, 255, 255);
    sf::Color mutedColor = sf::Color(153, 132, 200, 255);

    sf::Text& T(optional<sf::Text>& t) { return t.value(); }

    // Convert key code to readable string
    string keyName(sf::Keyboard::Key k) {
        switch (k) {
        case sf::Keyboard::Key::A:      return "A";
        case sf::Keyboard::Key::D:      return "D";
        case sf::Keyboard::Key::W:      return "W";
        case sf::Keyboard::Key::S:      return "S";
        case sf::Keyboard::Key::Space:  return "Space";
        case sf::Keyboard::Key::Escape: return "Escape";
        case sf::Keyboard::Key::Enter:  return "Enter";
        case sf::Keyboard::Key::Left:   return "Left Arrow";
        case sf::Keyboard::Key::Right:  return "Right Arrow";
        case sf::Keyboard::Key::Up:     return "Up Arrow";
        case sf::Keyboard::Key::Down:   return "Down Arrow";
        default:                        return "Key(" + to_string((int)k) + ")";
        }
    }

    // Apply the newly pressed key to whichever row is selected
    void applyKey(sf::Keyboard::Key newKey) {
        switch (selectedRow) {
        case 0: bindings.moveLeft = newKey; break;
        case 1: bindings.moveRight = newKey; break;
        case 2: bindings.jump = newKey; break;
        case 3: bindings.throwSnowball = newKey; break;
        case 4: bindings.pause = newKey; break;
        }
    }

public:
    bool done; // set to true when player presses Escape to exit

    KeyRemapScreen(KeyBindings& b) : bindings(b), selectedRow(0),
        waitingForKey(false), done(false) {
    }

    bool loadFont(const string& path) {
        if (!font.openFromFile(path)) return false;
        titleText.emplace(font);
        hintText.emplace(font);
        for (int i = 0; i < 5; i++) {
            actionTexts[i].emplace(font);
            keyTexts[i].emplace(font);
        }
        return true;
    }

    void handleEvent(sf::Event& event) {
        if (auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {

            if (waitingForKey) {
                // any key pressed becomes the new binding
                applyKey(keyPressed->code);
                waitingForKey = false;
                return;
            }

            if (keyPressed->code == sf::Keyboard::Key::Up)
                selectedRow = (selectedRow - 1 + 5) % 5;
            else if (keyPressed->code == sf::Keyboard::Key::Down)
                selectedRow = (selectedRow + 1) % 5;
            else if (keyPressed->code == sf::Keyboard::Key::Enter)
                waitingForKey = true;  // next keypress = new binding
            else if (keyPressed->code == sf::Keyboard::Key::Escape)
                done = true;  // exit remap screen
        }
    }

    void draw(sf::RenderWindow& window) {
        float W = (float)window.getSize().x;
        float H = (float)window.getSize().y;

        background.setSize({ W, H });
        background.setFillColor(bgColor);
        window.draw(background);

        T(titleText).setString("KEY BINDINGS");
        T(titleText).setCharacterSize(32);
        T(titleText).setFillColor(gold);
        T(titleText).setLetterSpacing(3.f);
        sf::FloatRect tb = T(titleText).getLocalBounds();
        T(titleText).setOrigin({ tb.size.x / 2.f, 0.f });
        T(titleText).setPosition({ W / 2.f, 60.f });
        window.draw(T(titleText));

        string actionNames[5] = { "Move Left", "Move Right", "Jump",
                                  "Throw Snowball", "Pause" };
        sf::Keyboard::Key* keyPtrs[5] = {
            &bindings.moveLeft, &bindings.moveRight, &bindings.jump,
            &bindings.throwSnowball, &bindings.pause
        };

        for (int i = 0; i < 5; i++) {
            float rowY = 160.f + i * 60.f;
            bool isSelected = (selectedRow == i);

            // highlight selected row
            if (isSelected) {
                highlight.setSize({ 500.f, 44.f });
                highlight.setPosition({ W / 2.f - 250.f, rowY - 6.f });
                highlight.setFillColor(sf::Color(255, 229, 102, 25));
                window.draw(highlight);
            }

            T(actionTexts[i]).setString(actionNames[i]);
            T(actionTexts[i]).setCharacterSize(20);
            T(actionTexts[i]).setFillColor(isSelected ? gold : lightText);
            T(actionTexts[i]).setPosition({ W / 2.f - 220.f, rowY });
            window.draw(T(actionTexts[i]));

            string keyStr = (waitingForKey && isSelected) ?
                "[ press any key... ]" : keyName(*keyPtrs[i]);
            T(keyTexts[i]).setString(keyStr);
            T(keyTexts[i]).setCharacterSize(20);
            T(keyTexts[i]).setFillColor(isSelected ?
                sf::Color(91, 191, 255, 255) : mutedColor);
            T(keyTexts[i]).setPosition({ W / 2.f + 60.f, rowY });
            window.draw(T(keyTexts[i]));
        }

        string hint = waitingForKey ?
            "press any key to bind..." :
            "Up/Down to select   |   Enter to rebind   |   Esc to save & exit";
        T(hintText).setString(hint);
        T(hintText).setCharacterSize(12);
        T(hintText).setFillColor(mutedColor);
        sf::FloatRect hb = T(hintText).getLocalBounds();
        T(hintText).setOrigin({ hb.size.x / 2.f, 0.f });
        T(hintText).setPosition({ W / 2.f, H - 40.f });
        window.draw(T(hintText));
    }
};