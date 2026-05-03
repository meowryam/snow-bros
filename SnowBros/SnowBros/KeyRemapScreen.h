#pragma once
//#include <SFML/Graphics.hpp>
//#include "KeyBindings.h"
//using namespace std;
//
//// Screen where player can remap their keys
//// Press Enter on a binding, then press the new key
//class KeyRemapScreen {
//private:
//    sf::Font font;
//    KeyBindings& bindings;
//
//    optional<sf::Text> titleText;
//    optional<sf::Text> actionTexts[5];
//    optional<sf::Text> keyTexts[5];
//    optional<sf::Text> hintText;
//
//    sf::RectangleShape background;
//    sf::RectangleShape highlight;
//
//    int selectedRow;      // which binding is selected
//    bool waitingForKey;   // true = next keypress becomes the new binding
//
//    sf::Color bgColor = sf::Color(26, 10, 46, 255);
//    sf::Color gold = sf::Color(255, 229, 102, 255);
//    sf::Color lightText = sf::Color(232, 224, 255, 255);
//    sf::Color mutedColor = sf::Color(153, 132, 200, 255);
//
//    sf::Text& T(optional<sf::Text>& t) { return t.value(); }
//
//    // Convert key code to readable string
//    string keyName(sf::Keyboard::Key k) {
//        switch (k) {
//        case sf::Keyboard::Key::A:      return "A";
//        case sf::Keyboard::Key::D:      return "D";
//        case sf::Keyboard::Key::W:      return "W";
//        case sf::Keyboard::Key::S:      return "S";
//        case sf::Keyboard::Key::Space:  return "Space";
//        case sf::Keyboard::Key::Escape: return "Escape";
//        case sf::Keyboard::Key::Enter:  return "Enter";
//        case sf::Keyboard::Key::Left:   return "Left Arrow";
//        case sf::Keyboard::Key::Right:  return "Right Arrow";
//        case sf::Keyboard::Key::Up:     return "Up Arrow";
//        case sf::Keyboard::Key::Down:   return "Down Arrow";
//        default:                        return "Key(" + to_string((int)k) + ")";
//        }
//    }
//
//    // Apply the newly pressed key to whichever row is selected
//    void applyKey(sf::Keyboard::Key newKey) {
//        switch (selectedRow) {
//        case 0: bindings.moveLeft = newKey; break;
//        case 1: bindings.moveRight = newKey; break;
//        case 2: bindings.jump = newKey; break;
//        case 3: bindings.throwSnowball = newKey; break;
//        case 4: bindings.pause = newKey; break;
//        }
//    }
//
//public:
//    bool done; // set to true when player presses Escape to exit
//
//    KeyRemapScreen(KeyBindings& b) : bindings(b), selectedRow(0),
//        waitingForKey(false), done(false) {
//    }
//
//    bool loadFont(const string& path) {
//        if (!font.openFromFile(path)) return false;
//        titleText.emplace(font);
//        hintText.emplace(font);
//        for (int i = 0; i < 5; i++) {
//            actionTexts[i].emplace(font);
//            keyTexts[i].emplace(font);
//        }
//        return true;
//    }
//
//    void handleEvent(sf::Event& event) {
//        if (auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
//
//            if (waitingForKey) {
//                // any key pressed becomes the new binding
//                applyKey(keyPressed->code);
//                waitingForKey = false;
//                return;
//            }
//
//            if (keyPressed->code == sf::Keyboard::Key::Up)
//                selectedRow = (selectedRow - 1 + 5) % 5;
//            else if (keyPressed->code == sf::Keyboard::Key::Down)
//                selectedRow = (selectedRow + 1) % 5;
//            else if (keyPressed->code == sf::Keyboard::Key::Enter)
//                waitingForKey = true;  // next keypress = new binding
//            else if (keyPressed->code == sf::Keyboard::Key::Escape)
//                done = true;  // exit remap screen
//        }
//    }
//
//    void draw(sf::RenderWindow& window) {
//        float W = (float)window.getSize().x;
//        float H = (float)window.getSize().y;
//
//        background.setSize({ W, H });
//        background.setFillColor(bgColor);
//        window.draw(background);
//
//        T(titleText).setString("KEY BINDINGS");
//        T(titleText).setCharacterSize(32);
//        T(titleText).setFillColor(gold);
//        T(titleText).setLetterSpacing(3.f);
//        sf::FloatRect tb = T(titleText).getLocalBounds();
//        T(titleText).setOrigin({ tb.size.x / 2.f, 0.f });
//        T(titleText).setPosition({ W / 2.f, 60.f });
//        window.draw(T(titleText));
//
//        string actionNames[5] = { "Move Left", "Move Right", "Jump",
//                                  "Throw Snowball", "Pause" };
//        sf::Keyboard::Key* keyPtrs[5] = {
//            &bindings.moveLeft, &bindings.moveRight, &bindings.jump,
//            &bindings.throwSnowball, &bindings.pause
//        };
//
//        for (int i = 0; i < 5; i++) {
//            float rowY = 160.f + i * 60.f;
//            bool isSelected = (selectedRow == i);
//
//            // highlight selected row
//            if (isSelected) {
//                highlight.setSize({ 500.f, 44.f });
//                highlight.setPosition({ W / 2.f - 250.f, rowY - 6.f });
//                highlight.setFillColor(sf::Color(255, 229, 102, 25));
//                window.draw(highlight);
//            }
//
//            T(actionTexts[i]).setString(actionNames[i]);
//            T(actionTexts[i]).setCharacterSize(20);
//            T(actionTexts[i]).setFillColor(isSelected ? gold : lightText);
//            T(actionTexts[i]).setPosition({ W / 2.f - 220.f, rowY });
//            window.draw(T(actionTexts[i]));
//
//            string keyStr = (waitingForKey && isSelected) ?
//                "[ Press any key... ]" : keyName(*keyPtrs[i]);
//            T(keyTexts[i]).setString(keyStr);
//            T(keyTexts[i]).setCharacterSize(20);
//            T(keyTexts[i]).setFillColor(isSelected ?
//                sf::Color(91, 191, 255, 255) : mutedColor);
//            T(keyTexts[i]).setPosition({ W / 2.f + 60.f, rowY });
//            window.draw(T(keyTexts[i]));
//        }
//
//        string hint = waitingForKey ?
//            "Press any key to Bind..." :
//            "Up/Down to select   |   Enter to rebind   |   Esc to save & exit";
//        T(hintText).setString(hint);
//        T(hintText).setCharacterSize(12);
//        T(hintText).setFillColor(mutedColor);
//        sf::FloatRect hb = T(hintText).getLocalBounds();
//        T(hintText).setOrigin({ hb.size.x / 2.f, 0.f });
//        T(hintText).setPosition({ W / 2.f, H - 40.f });
//        window.draw(T(hintText));
//    }
//};



//#include <SFML/Graphics.hpp>
//using namespace std;
//
//enum class MainMenuResult {
//    NONE,
//    START_GAME,
//    START_2PLAYER,
//    OPEN_SHOP,
//    OPEN_LEADERBOARD,
//    OPEN_KEYREMAP,
//    LOGOUT,
//    QUIT
//};
//
//class MainMenu {
//private:
//
//    sf::Font font;        // title font (PressStart2P)
//
//    optional<sf::Text> titleText;
//    optional<sf::Text> subtitleText;
//    optional<sf::Text> optionTexts[7];
//    optional<sf::Text> hintText;
//
//    sf::RectangleShape background;
//    sf::RectangleShape leftBar;   // decorative side bars
//    sf::RectangleShape rightBar;
//
//    int selectedOption;
//    string username;
//
//    sf::Color bgColor = sf::Color(10, 20, 60, 255);
//    sf::Color titleColor = sf::Color(80, 220, 255, 255);
//    sf::Color subColor = sf::Color(150, 255, 180, 255);
//    sf::Color selectedCol = sf::Color(255, 220, 50, 255);
//    sf::Color normalCol = sf::Color(160, 180, 255, 255);
//    sf::Color barColor = sf::Color(80, 220, 255, 60);
//    sf::Color hintCol = sf::Color(100, 120, 180, 255);
//
//    sf::Text& T(optional<sf::Text>& t) { return t.value(); }
//
//
//    //  sf::Font fontUI;      // ui font (Orbitron)
//
//public:
//    MainMenu() : selectedOption(0) {}
//
//    //bool loadFont(const string& titlePath, const string& uiPath) {
//    //    if (!font.openFromFile(titlePath)) return false;
//    //    if (!fontUI.openFromFile(uiPath)) return false;
//    //    titleText.emplace(font);       // title uses PressStart2P
//    //    subtitleText.emplace(fontUI);  // rest use Orbitron
//    //    hintText.emplace(fontUI);
//    //    for (int i = 0; i < 6; i++)
//    //        optionTexts[i].emplace(fontUI);
//    //    return true;
//    //}
//
//
//    bool loadFont(const string& path) {
//        if (!font.openFromFile(path)) return false;
//        titleText.emplace(font);
//        subtitleText.emplace(font);
//        hintText.emplace(font);
//        for (int i = 0; i < 7; i++)
//            optionTexts[i].emplace(font);
//        return true;
//    }
//
//
//    void setUsername(const string& name) { username = name; }
//    void reset() { selectedOption = 0; }
//
//    MainMenuResult handleEvent(sf::Event& event) {
//        if (auto* kp = event.getIf<sf::Event::KeyPressed>()) {
//            if (kp->code == sf::Keyboard::Key::Up)
//                selectedOption = (selectedOption - 1 + 7) % 7;
//            else if (kp->code == sf::Keyboard::Key::Down)
//                selectedOption = (selectedOption + 1) % 7;
//            else if (kp->code == sf::Keyboard::Key::Enter) {
//                if (selectedOption == 0) return MainMenuResult::START_GAME;
//                if (selectedOption == 1) return MainMenuResult::START_2PLAYER;
//                if (selectedOption == 2) return MainMenuResult::OPEN_SHOP;
//                if (selectedOption == 3) return MainMenuResult::OPEN_LEADERBOARD;
//                if (selectedOption == 4) return MainMenuResult::OPEN_KEYREMAP;
//                if (selectedOption == 5) return MainMenuResult::LOGOUT;
//                if (selectedOption == 6) return MainMenuResult::QUIT;
//            }
//        }
//        return MainMenuResult::NONE;
//    }
//
//    void draw(sf::RenderWindow& window) {
//        float W = (float)window.getSize().x;
//        float H = (float)window.getSize().y;
//
//        // background
//        background.setSize({ W, H });
//        background.setFillColor(bgColor);
//        window.draw(background);
//
//        // decorative side bars
//        leftBar.setSize({ 6.f, H });
//        leftBar.setPosition({ 0.f, 0.f });
//        leftBar.setFillColor(barColor);
//        window.draw(leftBar);
//
//        rightBar.setSize({ 6.f, H });
//        rightBar.setPosition({ W - 6.f, 0.f });
//        rightBar.setFillColor(barColor);
//        window.draw(rightBar);
//
//        // title
//        T(titleText).setString("SNOW BROS");
//        T(titleText).setCharacterSize(56);
//        T(titleText).setFillColor(titleColor);
//        T(titleText).setLetterSpacing(2.f);
//        sf::FloatRect tb = T(titleText).getLocalBounds();
//        T(titleText).setOrigin({ tb.size.x / 2.f, 0.f });
//        T(titleText).setPosition({ W / 2.f, 70.f });
//        window.draw(T(titleText));
//
//        // subtitle with username
//        T(subtitleText).setString("Welcome Back " + username + "!");
//        T(subtitleText).setCharacterSize(16);
//        T(subtitleText).setFillColor(subColor);
//        sf::FloatRect sub = T(subtitleText).getLocalBounds();
//        T(subtitleText).setOrigin({ sub.size.x / 2.f, 0.f });
//        T(subtitleText).setPosition({ W / 2.f, 170.f });
//        window.draw(T(subtitleText));
//
//        // menu options
//        string opts[7] = { "1 Player", "2 Player", "Shop", "Leaderboard", "Key Bindings", "Logout", "Quit" };
//        for (int i = 0; i < 7; i++) {
//            T(optionTexts[i]).setString((selectedOption == i ? ">  " : "   ") + opts[i]);
//            T(optionTexts[i]).setCharacterSize(26);
//            T(optionTexts[i]).setFillColor(selectedOption == i ? selectedCol : normalCol);
//            sf::FloatRect ob = T(optionTexts[i]).getLocalBounds();
//            T(optionTexts[i]).setOrigin({ ob.size.x / 2.f, 0.f });
//            T(optionTexts[i]).setPosition({ W / 2.f - 50.f, 220.f + i * 55.f });
//            window.draw(T(optionTexts[i]));
//        }
//
//        // hint
//        T(hintText).setString("Up/Down to navigate  |  Enter to select");
//        T(hintText).setCharacterSize(12);
//        T(hintText).setFillColor(hintCol);
//        sf::FloatRect hb = T(hintText).getLocalBounds();
//        T(hintText).setOrigin({ hb.size.x / 2.f, 0.f });
//        T(hintText).setPosition({ W / 2.f, H - 30.f });
//        window.draw(T(hintText));
//    }
//};









#pragma once
#include <SFML/Graphics.hpp>
#include "KeyBindings.h"
using namespace std;

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
        default: return "Key(" + to_string((int)k) + ")";
        }
    }

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

    bool done;

    KeyRemapScreen(KeyBindings& b)
        : bindings(b), selectedRow(0), waitingForKey(false), done(false) {
    }

    bool loadFont(const string& path) {
        if (!font.openFromFile(path)) return false;
        bgLoaded = bgTex.loadFromFile("assets\\images\\Login_bg.png");
        if (bgLoaded) {
            bgSprite.emplace(bgTex);
            sf::Vector2u ts = bgTex.getSize();
            bgSprite->setScale({ 800.f / static_cast<float>(ts.x), 600.f / static_cast<float>(ts.y) });
        }
        titleText.emplace(font);
        hintText.emplace(font);
        for (int i = 0; i < 5; i++) {
            actionTexts[i].emplace(font);
            keyTexts[i].emplace(font);
        }
        return true;
    }

    void handleEvent(sf::Event& event) {
        if (auto* kp = event.getIf<sf::Event::KeyPressed>()) {
            if (waitingForKey) {
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

    void draw(sf::RenderWindow& window) {
        float W = (float)window.getSize().x;
        float H = (float)window.getSize().y;
        if (bgLoaded) window.draw(*bgSprite);
        else {
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

        for (int i = 0; i < 5; i++) {
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