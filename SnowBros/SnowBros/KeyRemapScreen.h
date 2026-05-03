#pragma once
#pragma once
#include <SFML/Graphics.hpp>
#include "KeyBindings.h"
using namespace std;

class KeyRemapScreen 
{
private:
    sf::Font font;
    KeyBindings& bindings;

    optional<sf::Text> titleText;
    optional<sf::Text> actionTexts[5];
    optional<sf::Text> keyTexts[5];
    optional<sf::Text> hintText;

    sf::RectangleShape background;
    sf::RectangleShape highlight;
    sf::Texture bgTex;
    bool        bgLoaded = false;
    optional<sf::Sprite> bgSprite;
    int selectedRow;
    bool waitingForKey;
    
    sf::Color titleCol = sf::Color(200, 240, 255, 255);  // ice white-blue
    sf::Color iceWhite = sf::Color(220, 245, 255, 255);  // action name text
    sf::Color iceMid = sf::Color(160, 210, 255, 220);  // key value normal
    sf::Color iceDim = sf::Color(100, 160, 220, 180);  // hint
    sf::Color keySelCol = sf::Color(150, 225, 255, 255);  // key value when selected

    // Row highlight / button colours
    sf::Color rowSelFill = sf::Color(30, 100, 220, 160);
    sf::Color rowSelOut = sf::Color(140, 220, 255, 255);
    sf::Color rowNrmOut = sf::Color(60, 140, 220, 80);

    sf::Text& T(optional<sf::Text>& t) { return t.value(); }

    string keyName(sf::Keyboard::Key k)
    {
        switch (k)
        {
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
        default: return "Key(" + to_string((int)k) + ")";
        }
    }

    void applyKey(sf::Keyboard::Key newKey)
    {
        switch (selectedRow)
        {
        case 0: bindings.moveLeft = newKey; break;
        case 1: bindings.moveRight = newKey; break;
        case 2: bindings.jump = newKey; break;
        case 3: bindings.throwSnowball = newKey; break;
        case 4: bindings.pause = newKey; break;
        }
    }

public:

    bool done;

    KeyRemapScreen(KeyBindings& b)
        : bindings(b), selectedRow(0), waitingForKey(false), done(false) 
    {
    }

    bool loadFont(const string& path) 
    {
        if (!font.openFromFile(path))
            return false;
        bgLoaded = bgTex.loadFromFile("assets\\images\\Login_bg.png");
        if (bgLoaded)
        {
            bgSprite.emplace(bgTex);
            sf::Vector2u ts = bgTex.getSize();
            bgSprite->setScale({ 800.f / static_cast<float>(ts.x), 600.f / static_cast<float>(ts.y) });
        }
        titleText.emplace(font);
        hintText.emplace(font);
        for (int i = 0; i < 5; i++) 
        {
            actionTexts[i].emplace(font);
            keyTexts[i].emplace(font);
        }
        return true;
    }

    void handleEvent(sf::Event& event) 
    {
        if (auto* kp = event.getIf<sf::Event::KeyPressed>())
        {
            if (waitingForKey) 
            {
                applyKey(kp->code);
                waitingForKey = false;
                return;
            }
            if (kp->code == sf::Keyboard::Key::Up)
                selectedRow = (selectedRow - 1 + 5) % 5;

            else if (kp->code == sf::Keyboard::Key::Down)
                selectedRow = (selectedRow + 1) % 5;

            else if (kp->code == sf::Keyboard::Key::Enter)
                waitingForKey = true;

            else if (kp->code == sf::Keyboard::Key::Escape)
                done = true;
        }
    }

    void draw(sf::RenderWindow& window)
    {
        float W = (float)window.getSize().x;
        float H = (float)window.getSize().y;
        if (bgLoaded) window.draw(*bgSprite);
        else 
        {
            sf::RectangleShape fb({ 800.f, 600.f });
            fb.setFillColor(sf::Color(5, 12, 35, 255));
            window.draw(fb);
        }
        sf::RectangleShape vignette({ 800.f, 600.f });
        vignette.setFillColor(sf::Color(0, 5, 20, 70));
        window.draw(vignette);

        string actionNames[5] = { "Move Left","Move Right","Jump","Throw Snowball","Pause" };
        sf::Keyboard::Key* keyPtrs[5] = {
            &bindings.moveLeft, &bindings.moveRight, &bindings.jump,
            &bindings.throwSnowball, &bindings.pause
        };

        for (int i = 0; i < 5; i++)
        {
            float rowY = 160.f + i * 60.f;
            bool sel = (selectedRow == i);
            highlight.setSize({ 500.f, 44.f });
            highlight.setPosition({ W / 2.f - 250.f, rowY - 6.f });
            highlight.setFillColor(sel ? rowSelFill : sf::Color(10, 30, 80, 80));
            highlight.setOutlineThickness(1.2f);
            highlight.setOutlineColor(sel ? rowSelOut : rowNrmOut);
            window.draw(highlight);
            T(actionTexts[i]).setString(actionNames[i]);
            T(actionTexts[i]).setCharacterSize(20);
            T(actionTexts[i]).setFillColor(sel ? sf::Color(255, 255, 255, 255) : iceWhite);
            T(actionTexts[i]).setPosition({ W / 2.f - 220.f, rowY });
            window.draw(T(actionTexts[i]));

            string keyStr = (waitingForKey && sel) ? "[ Press any key... ]" : keyName(*keyPtrs[i]);
            T(keyTexts[i]).setString(keyStr);
            T(keyTexts[i]).setCharacterSize(20);
            T(keyTexts[i]).setFillColor(sel ? keySelCol : iceMid);
            T(keyTexts[i]).setPosition({ W / 2.f + 60.f, rowY });
            window.draw(T(keyTexts[i]));
        }

        string hint = waitingForKey ? "Press any key to Bind..." :
            "Up/Down to select   |   Enter to rebind   |   Esc to save & exit";
        T(hintText).setString(hint);
        T(hintText).setCharacterSize(12);
        T(hintText).setFillColor(iceDim);
        sf::FloatRect hb = T(hintText).getLocalBounds();
        T(hintText).setOrigin({ hb.size.x / 2.f, 0.f });
        T(hintText).setPosition({ W / 2.f, H - 40.f });
        window.draw(T(hintText));
    }
};