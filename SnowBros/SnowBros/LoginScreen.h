#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "FileManager.h"
#include "PlayerData.h"
using namespace std;

// What the login screen wants to do next
enum class LoginResult {
    NONE,        // still waiting
    NEW_GAME,    // username entered, no save found
    CONTINUE,    // username entered, save found
    QUIT
};

class LoginScreen {
private:
    string usernameInput;   // what the player is typing
    bool waitingForInput;   // true = typing username, false = showing menu

    // colours
    sf::Color bgColor = sf::Color(26, 10, 46, 255);
    sf::Color gold = sf::Color(255, 229, 102, 255);
    sf::Color lightText = sf::Color(232, 224, 255, 255);
    sf::Color mutedColor = sf::Color(153, 132, 200, 255);
    sf::Color greenOk = sf::Color(76, 175, 125, 255);

    // UI elements
    sf::RectangleShape background;
    sf::RectangleShape inputBox;

    // optional texts (SFML 3 requires font at construction)
    optional<sf::Text> titleText;
    optional<sf::Text> promptText;
    optional<sf::Text> inputText;
    optional<sf::Text> option1Text;  // New Game
    optional<sf::Text> option2Text;  // Continue
    optional<sf::Text> option3Text;  // Quit
    optional<sf::Text> errorText;

    int selectedOption;  // 0=NewGame, 1=Continue, 2=Quit
    bool saveFound;
    string errorMsg;

    sf::Text& T(optional<sf::Text>& t) { return t.value(); }

   // sf::Font font;      // PressStart2P for title
   // sf::Font fontUI;    // Orbitron for rest

    sf::Font* font = nullptr;
    sf::Font* fontUI = nullptr;

public:
    LoginScreen() : waitingForInput(true), selectedOption(0),
        saveFound(false) {
    }

    //bool loadFont(const string& titlePath, const string& uiPath) {
    //    if (!font.openFromFile(titlePath)) return false;
    //    if (!fontUI.openFromFile(uiPath)) return false;
    //    titleText.emplace(font);       // SNOW BROS title
    //    promptText.emplace(fontUI);
    //    inputText.emplace(fontUI);
    //    option1Text.emplace(fontUI);
    //    option2Text.emplace(fontUI);
    //    option3Text.emplace(fontUI);
    //    errorText.emplace(fontUI);
    //    return true;
    //}

    bool loadFont(sf::Font& titleFont, sf::Font& uiFont) {
        font = &titleFont;
        fontUI = &uiFont;
        titleText.emplace(*font);       // SNOW BROS title
        promptText.emplace(*fontUI);
        inputText.emplace(*fontUI);
        option1Text.emplace(*fontUI);
        option2Text.emplace(*fontUI);
        option3Text.emplace(*fontUI);
        errorText.emplace(*fontUI);
        return true;
    }


    // Call this every frame to handle typing + menu navigation
    // Returns what should happen next
    LoginResult handleEvent(sf::Event& event) {

        // --- Typing phase ---
        if (waitingForInput) {
            if (auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->code == sf::Keyboard::Key::Enter) {
                    if (usernameInput.empty()) {
                        errorMsg = "please enter a username!";
                        return LoginResult::NONE;
                    }
                    errorMsg = "";
                    saveFound = FileManager::saveExists(usernameInput);
                    waitingForInput = false; // move to menu
                }
                else if (keyPressed->code == sf::Keyboard::Key::Backspace) {
                    if (!usernameInput.empty())
                        usernameInput.pop_back();
                }
            }
            if (auto* textEv = event.getIf<sf::Event::TextEntered>()) {
                char c = (char)textEv->unicode;
                // only allow letters, numbers, underscore, max 16 chars
                if (usernameInput.size() < 16 &&
                    (isalnum(c) || c == '_') && c >= 32) {
                    usernameInput += c;
                }
            }
            return LoginResult::NONE;
        }

        // --- Menu phase ---
        if (auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
            int maxOptions = saveFound ? 3 : 2; // Continue only shown if save exists

            if (keyPressed->code == sf::Keyboard::Key::Up)
                selectedOption = (selectedOption - 1 + maxOptions) % maxOptions;
            else if (keyPressed->code == sf::Keyboard::Key::Down)
                selectedOption = (selectedOption + 1) % maxOptions;
            else if (keyPressed->code == sf::Keyboard::Key::Enter) {
                if (saveFound) {
                    if (selectedOption == 0) return LoginResult::NEW_GAME;
                    if (selectedOption == 1) return LoginResult::CONTINUE;
                    if (selectedOption == 2) return LoginResult::QUIT;
                }
                else {
                    if (selectedOption == 0) return LoginResult::NEW_GAME;
                    if (selectedOption == 1) return LoginResult::QUIT;
                }
            }
            // allow going back to retype username
            else if (keyPressed->code == sf::Keyboard::Key::Escape) {
                waitingForInput = true;
                usernameInput = "";
                selectedOption = 0;
            }
        }
        return LoginResult::NONE;
    }

    string getUsername() const { return usernameInput; }

    void draw(sf::RenderWindow& window) {
        float W = (float)window.getSize().x;
        float H = (float)window.getSize().y;

        // background
        background.setSize({ W, H });
        background.setFillColor(bgColor);
        window.draw(background);

        // title
        T(titleText).setString("SNOW BROS");
        T(titleText).setCharacterSize(48);
        T(titleText).setFillColor(gold);
        T(titleText).setLetterSpacing(3.f);
        sf::FloatRect tb = T(titleText).getLocalBounds();
        T(titleText).setOrigin({ tb.size.x / 2.f, 0.f });
        T(titleText).setPosition({ W / 2.f, 80.f });
        window.draw(T(titleText));

        if (waitingForInput) {
            // prompt
            T(promptText).setString("Enter Your Username:");
            T(promptText).setCharacterSize(18);
            T(promptText).setFillColor(mutedColor);
            sf::FloatRect pb = T(promptText).getLocalBounds();
            T(promptText).setOrigin({ pb.size.x / 2.f, 0.f });
            T(promptText).setPosition({ W / 2.f, 220.f });
            window.draw(T(promptText));

            // input box
            inputBox.setSize({ 320.f, 48.f });
            inputBox.setPosition({ W / 2.f - 160.f, 260.f });
            inputBox.setFillColor(sf::Color(255, 255, 255, 15));
            inputBox.setOutlineThickness(1.f);
            inputBox.setOutlineColor(gold);
            window.draw(inputBox);

            // typed text
            T(inputText).setString(usernameInput + "|");
            T(inputText).setCharacterSize(20);
            T(inputText).setFillColor(lightText);
            T(inputText).setPosition({ W / 2.f - 148.f, 270.f });
            window.draw(T(inputText));

            // error
            if (!errorMsg.empty()) {
                T(errorText).setString(errorMsg);
                T(errorText).setCharacterSize(13);
                T(errorText).setFillColor(sf::Color(226, 75, 74, 255));
                sf::FloatRect eb = T(errorText).getLocalBounds();
                T(errorText).setOrigin({ eb.size.x / 2.f, 0.f });
                T(errorText).setPosition({ W / 2.f, 320.f });
                window.draw(T(errorText));
            }

            T(promptText).setString("Press Enter to Continue");
            T(promptText).setCharacterSize(13);
            T(promptText).setFillColor(mutedColor);
            sf::FloatRect pb2 = T(promptText).getLocalBounds();
            T(promptText).setOrigin({ pb2.size.x / 2.f, 0.f });
            T(promptText).setPosition({ W / 2.f, 360.f });
            window.draw(T(promptText));

        }
        else {
            // show username
            T(promptText).setString("Welcome " + usernameInput + "!");
            T(promptText).setCharacterSize(20);
            T(promptText).setFillColor(greenOk);
            sf::FloatRect pb = T(promptText).getLocalBounds();
            T(promptText).setOrigin({ pb.size.x / 2.f, 0.f });
            T(promptText).setPosition({ W / 2.f - 15.f, 200.f });
            window.draw(T(promptText));

            // menu options
            string opts[3];
            int optCount;
            if (saveFound) {
                opts[0] = "New Game";
                opts[1] = "Continue";
                opts[2] = "Quit";
                optCount = 3;
            }
            else {
                opts[0] = "New Game";
                opts[1] = "Quit";
                optCount = 2;
            }

            optional<sf::Text>* textPtrs[3] = { &option1Text, &option2Text, &option3Text };
            for (int i = 0; i < optCount; i++) {
                T(*textPtrs[i]).setString((selectedOption == i ? "> " : "  ") + opts[i]);
                T(*textPtrs[i]).setCharacterSize(24);
                T(*textPtrs[i]).setFillColor(selectedOption == i ? gold : mutedColor);
                sf::FloatRect ob = T(*textPtrs[i]).getLocalBounds();
                T(*textPtrs[i]).setOrigin({ ob.size.x / 2.f, 0.f });
                T(*textPtrs[i]).setPosition({ W / 2.f - 30.f, 270.f + i * 50.f });
                window.draw(T(*textPtrs[i]));
            }

            T(errorText).setString("ESC to Retype Username   |   Arrows to Navigate   |   Enter to Select");
            T(errorText).setCharacterSize(11);
            T(errorText).setFillColor(mutedColor);
            sf::FloatRect eb = T(errorText).getLocalBounds();
            T(errorText).setOrigin({ eb.size.x / 2.f, 0.f });
            T(errorText).setPosition({ W / 2.f, H - 40.f });
            window.draw(T(errorText));
        }
    }
};