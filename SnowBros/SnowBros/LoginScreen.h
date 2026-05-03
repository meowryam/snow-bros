//#pragma once
//#include <SFML/Graphics.hpp>
//#include <string>
//#include "FileManager.h"
//#include "PlayerData.h"
//using namespace std;
//
//// What the login screen wants to do next
//enum class LoginResult {
//    NONE,        // still waiting
//    NEW_GAME,    // username entered, no save found
//    CONTINUE,    // username entered, save found
//    QUIT
//};
//
//class LoginScreen {
//private:
//    sf::Font font;      // PressStart2P for title
//
//    string usernameInput;   // what the player is typing
//    bool waitingForInput;   // true = typing username, false = showing menu
//
//    // colours
//    sf::Color bgColor = sf::Color(26, 10, 46, 255);
//    sf::Color gold = sf::Color(255, 229, 102, 255);
//    sf::Color lightText = sf::Color(232, 224, 255, 255);
//    sf::Color mutedColor = sf::Color(153, 132, 200, 255);
//    sf::Color greenOk = sf::Color(76, 175, 125, 255);
//
//    // UI elements
//    sf::RectangleShape background;
//    sf::RectangleShape inputBox;
//
//    // optional texts (SFML 3 requires font at construction)
//    optional<sf::Text> titleText;
//    optional<sf::Text> promptText;
//    optional<sf::Text> inputText;
//    optional<sf::Text> option1Text;  // New Game
//    optional<sf::Text> option2Text;  // Continue
//    optional<sf::Text> option3Text;  // Quit
//    optional<sf::Text> errorText;
//
//    int selectedOption;  // 0=NewGame, 1=Continue, 2=Quit
//    bool saveFound;
//    string errorMsg;
//
//    sf::Text& T(optional<sf::Text>& t) { return t.value(); }
//
//   
//    //sf::Font fontUI;    // Orbitron for rest
//
//public:
//    LoginScreen() : waitingForInput(true), selectedOption(0),
//        saveFound(false) {
//    }
//
//    //bool loadFont(const string& titlePath, const string& uiPath) {
//    //    if (!font.openFromFile(titlePath)) return false;
//    //    if (!fontUI.openFromFile(uiPath)) return false;
//    //    titleText.emplace(font);       // SNOW BROS title
//    //    promptText.emplace(fontUI);
//    //    inputText.emplace(fontUI);
//    //    option1Text.emplace(fontUI);
//    //    option2Text.emplace(fontUI);
//    //    option3Text.emplace(fontUI);
//    //    errorText.emplace(fontUI);
//    //    return true;
//    //}
//
//
//    bool loadFont(const string& path) {
//        if (!font.openFromFile(path)) return false;
//
//        titleText.emplace(font);
//        promptText.emplace(font);
//        inputText.emplace(font);
//        option1Text.emplace(font);
//        option2Text.emplace(font);
//        option3Text.emplace(font);
//        errorText.emplace(font);
//        return true;
//    }
//
//    // Call this every frame to handle typing + menu navigation
//    // Returns what should happen next
//    LoginResult handleEvent(sf::Event& event) {
//
//        // --- Typing phase ---
//        if (waitingForInput) {
//            if (auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
//                if (keyPressed->code == sf::Keyboard::Key::Enter) {
//                    if (usernameInput.empty()) {
//                        errorMsg = "please enter a username!";
//                        return LoginResult::NONE;
//                    }
//                    errorMsg = "";
//                    saveFound = FileManager::saveExists(usernameInput);
//                    waitingForInput = false; // move to menu
//                }
//                else if (keyPressed->code == sf::Keyboard::Key::Backspace) {
//                    if (!usernameInput.empty())
//                        usernameInput.pop_back();
//                }
//            }
//            if (auto* textEv = event.getIf<sf::Event::TextEntered>()) {
//                char c = (char)textEv->unicode;
//                // only allow letters, numbers, underscore, max 16 chars
//                if (usernameInput.size() < 16 &&
//                    (isalnum(c) || c == '_') && c >= 32) {
//                    usernameInput += c;
//                }
//            }
//            return LoginResult::NONE;
//        }
//
//        // --- Menu phase ---
//        if (auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
//            int maxOptions = saveFound ? 3 : 2; // Continue only shown if save exists
//
//            if (keyPressed->code == sf::Keyboard::Key::Up)
//                selectedOption = (selectedOption - 1 + maxOptions) % maxOptions;
//            else if (keyPressed->code == sf::Keyboard::Key::Down)
//                selectedOption = (selectedOption + 1) % maxOptions;
//            else if (keyPressed->code == sf::Keyboard::Key::Enter) {
//                if (saveFound) {
//                    if (selectedOption == 0) return LoginResult::NEW_GAME;
//                    if (selectedOption == 1) return LoginResult::CONTINUE;
//                    if (selectedOption == 2) return LoginResult::QUIT;
//                }
//                else {
//                    if (selectedOption == 0) return LoginResult::NEW_GAME;
//                    if (selectedOption == 1) return LoginResult::QUIT;
//                }
//            }
//            // allow going back to retype username
//            else if (keyPressed->code == sf::Keyboard::Key::Escape) {
//                waitingForInput = true;
//                usernameInput = "";
//                selectedOption = 0;
//            }
//        }
//        return LoginResult::NONE;
//    }
//
//    string getUsername() const { return usernameInput; }
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
//        // title
//        T(titleText).setString("SNOW BROS");
//        T(titleText).setCharacterSize(48);
//        T(titleText).setFillColor(gold);
//        T(titleText).setLetterSpacing(3.f);
//        sf::FloatRect tb = T(titleText).getLocalBounds();
//        T(titleText).setOrigin({ tb.size.x / 2.f, 0.f });
//        T(titleText).setPosition({ W / 2.f, 80.f });
//        window.draw(T(titleText));
//
//        if (waitingForInput) {
//            // prompt
//            T(promptText).setString("Enter Your Username:");
//            T(promptText).setCharacterSize(18);
//            T(promptText).setFillColor(mutedColor);
//            sf::FloatRect pb = T(promptText).getLocalBounds();
//            T(promptText).setOrigin({ pb.size.x / 2.f, 0.f });
//            T(promptText).setPosition({ W / 2.f, 220.f });
//            window.draw(T(promptText));
//
//            // input box
//            inputBox.setSize({ 320.f, 48.f });
//            inputBox.setPosition({ W / 2.f - 160.f, 260.f });
//            inputBox.setFillColor(sf::Color(255, 255, 255, 15));
//            inputBox.setOutlineThickness(1.f);
//            inputBox.setOutlineColor(gold);
//            window.draw(inputBox);
//
//            // typed text
//            T(inputText).setString(usernameInput + "|");
//            T(inputText).setCharacterSize(20);
//            T(inputText).setFillColor(lightText);
//            T(inputText).setPosition({ W / 2.f - 148.f, 270.f });
//            window.draw(T(inputText));
//
//            // error
//            if (!errorMsg.empty()) {
//                T(errorText).setString(errorMsg);
//                T(errorText).setCharacterSize(13);
//                T(errorText).setFillColor(sf::Color(226, 75, 74, 255));
//                sf::FloatRect eb = T(errorText).getLocalBounds();
//                T(errorText).setOrigin({ eb.size.x / 2.f, 0.f });
//                T(errorText).setPosition({ W / 2.f, 320.f });
//                window.draw(T(errorText));
//            }
//
//            T(promptText).setString("Press Enter to Continue");
//            T(promptText).setCharacterSize(13);
//            T(promptText).setFillColor(mutedColor);
//            sf::FloatRect pb2 = T(promptText).getLocalBounds();
//            T(promptText).setOrigin({ pb2.size.x / 2.f, 0.f });
//            T(promptText).setPosition({ W / 2.f, 360.f });
//            window.draw(T(promptText));
//
//        }
//        else {
//            // show username
//            T(promptText).setString("Welcome " + usernameInput + "!");
//            T(promptText).setCharacterSize(20);
//            T(promptText).setFillColor(greenOk);
//            sf::FloatRect pb = T(promptText).getLocalBounds();
//            T(promptText).setOrigin({ pb.size.x / 2.f, 0.f });
//            T(promptText).setPosition({ W / 2.f - 15.f, 200.f });
//            window.draw(T(promptText));
//
//            // menu options
//            string opts[3];
//            int optCount;
//            if (saveFound) {
//                opts[0] = "New Game";
//                opts[1] = "Continue";
//                opts[2] = "Quit";
//                optCount = 3;
//            }
//            else {
//                opts[0] = "New Game";
//                opts[1] = "Quit";
//                optCount = 2;
//            }
//
//            optional<sf::Text>* textPtrs[3] = { &option1Text, &option2Text, &option3Text };
//            for (int i = 0; i < optCount; i++) {
//                T(*textPtrs[i]).setString((selectedOption == i ? "> " : "  ") + opts[i]);
//                T(*textPtrs[i]).setCharacterSize(24);
//                T(*textPtrs[i]).setFillColor(selectedOption == i ? gold : mutedColor);
//                sf::FloatRect ob = T(*textPtrs[i]).getLocalBounds();
//                T(*textPtrs[i]).setOrigin({ ob.size.x / 2.f, 0.f });
//                T(*textPtrs[i]).setPosition({ W / 2.f - 30.f, 270.f + i * 50.f });
//                window.draw(T(*textPtrs[i]));
//            }
//
//            T(errorText).setString("ESC to Retype Username   |   Arrows to Navigate   |   Enter to Select");
//            T(errorText).setCharacterSize(11);
//            T(errorText).setFillColor(mutedColor);
//            sf::FloatRect eb = T(errorText).getLocalBounds();
//            T(errorText).setOrigin({ eb.size.x / 2.f, 0.f });
//            T(errorText).setPosition({ W / 2.f, H - 40.f });
//            window.draw(T(errorText));
//        }
//    }
//};



#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <fstream>
#include "FileManager.h"
#include "PlayerData.h"
#include "PasswordHasher.h"
using namespace std;

// What the login screen wants to do next
enum class LoginResult {
    NONE,        // still waiting
    NEW_GAME,    // new account -> go to SignupScreen
    CONTINUE,    // existing user, password verified -> load save
    QUIT
};

class LoginScreen {
private:
    sf::Font font;

    string usernameInput;
    string passwordInput;       // NEW: password field for existing users
    bool waitingForInput;       // Phase 1: typing username
    bool waitingForPassword;    // Phase 2: typing password (login flow only)

    sf::Color bgColor = sf::Color(26, 10, 46, 255);
    sf::Color gold = sf::Color(255, 229, 102, 255);
    sf::Color lightText = sf::Color(232, 224, 255, 255);
    sf::Color mutedColor = sf::Color(153, 132, 200, 255);
    sf::Color greenOk = sf::Color(76, 175, 125, 255);
    sf::Color redErr = sf::Color(226, 75, 74, 255);

    sf::RectangleShape background;
    sf::RectangleShape inputBox;
    sf::RectangleShape passwordBox;   // NEW

    optional<sf::Text> titleText;
    optional<sf::Text> promptText;
    optional<sf::Text> inputText;
    optional<sf::Text> passwordText;  // NEW: masked password display
    optional<sf::Text> option1Text;
    optional<sf::Text> option2Text;
    optional<sf::Text> option3Text;
    optional<sf::Text> errorText;

    int    selectedOption;
    bool   saveFound;
    string errorMsg;

    // Returns stored "SALT$HASH" for username, or "" if not found
    string loadHashForUser(const string& username) const {
        ifstream file("users.dat");
        if (!file.is_open()) return "";
        string line;
        while (getline(file, line)) {
            size_t sep = line.find(':');
            if (sep == string::npos) continue;
            if (line.substr(0, sep) == username)
                return line.substr(sep + 1);
        }
        return "";
    }

    bool hasPassword(const string& username) const {
        return !loadHashForUser(username).empty();
    }

    sf::Text& T(optional<sf::Text>& t) { return t.value(); }

public:
    LoginScreen()
        : waitingForInput(true)
        , waitingForPassword(false)
        , selectedOption(0)
        , saveFound(false)
    {
    }

    bool loadFont(const string& path) {
        if (!font.openFromFile(path)) return false;
        titleText.emplace(font);
        promptText.emplace(font);
        inputText.emplace(font);
        passwordText.emplace(font);
        option1Text.emplace(font);
        option2Text.emplace(font);
        option3Text.emplace(font);
        errorText.emplace(font);
        return true;
    }

    LoginResult handleEvent(sf::Event& event) {

        // ============================================================
        // Phase 1: Username entry  (your original logic, unchanged)
        // ============================================================
        if (waitingForInput) {
            if (auto* kp = event.getIf<sf::Event::KeyPressed>()) {
                if (kp->code == sf::Keyboard::Key::Enter) {
                    if (usernameInput.empty()) {
                        errorMsg = "Please enter a username!";
                        return LoginResult::NONE;
                    }
                    errorMsg = "";
                    saveFound = FileManager::saveExists(usernameInput);

                    if (saveFound && hasPassword(usernameInput)) {
                        // Existing account with a password -> ask for it
                        waitingForInput = false;
                        waitingForPassword = true;
                    }
                    else {
                        // No save (new user) or legacy save without password -> show menu
                        waitingForInput = false;
                    }
                }
                else if (kp->code == sf::Keyboard::Key::Backspace) {
                    if (!usernameInput.empty()) usernameInput.pop_back();
                }
            }
            if (auto* te = event.getIf<sf::Event::TextEntered>()) {
                char c = (char)te->unicode;
                if (usernameInput.size() < 16 && (isalnum(c) || c == '_') && c >= 32)
                    usernameInput += c;
            }
            return LoginResult::NONE;
        }

        // ============================================================
        // Phase 2 (NEW): Password verification for existing accounts
        // ============================================================
        if (waitingForPassword) {
            if (auto* kp = event.getIf<sf::Event::KeyPressed>()) {
                if (kp->code == sf::Keyboard::Key::Enter) {
                    if (passwordInput.empty()) {
                        errorMsg = "Please enter your password!";
                        return LoginResult::NONE;
                    }
                    string stored = loadHashForUser(usernameInput);
                    if (PasswordHasher::verifyPassword(passwordInput, stored)) {
                        errorMsg = "";
                        waitingForPassword = false;
                        return LoginResult::CONTINUE;
                    }
                    else {
                        errorMsg = "Incorrect password. Try again.";
                        passwordInput.clear();
                        return LoginResult::NONE;
                    }
                }
                else if (kp->code == sf::Keyboard::Key::Backspace) {
                    if (!passwordInput.empty()) passwordInput.pop_back();
                }
                else if (kp->code == sf::Keyboard::Key::Escape) {
                    waitingForPassword = false;
                    waitingForInput = true;
                    passwordInput.clear();
                    usernameInput.clear();
                    errorMsg.clear();
                }
            }
            if (auto* te = event.getIf<sf::Event::TextEntered>()) {
                uint32_t c = te->unicode;
                if (passwordInput.size() < 30 && c >= 32 && c < 127)
                    passwordInput += static_cast<char>(c);
            }
            return LoginResult::NONE;
        }

        // ============================================================
        // Phase 3: Menu  (your original logic, unchanged)
        // ============================================================
        if (auto* kp = event.getIf<sf::Event::KeyPressed>()) {
            int maxOptions = saveFound ? 3 : 2;

            if (kp->code == sf::Keyboard::Key::Up)
                selectedOption = (selectedOption - 1 + maxOptions) % maxOptions;
            else if (kp->code == sf::Keyboard::Key::Down)
                selectedOption = (selectedOption + 1) % maxOptions;
            else if (kp->code == sf::Keyboard::Key::Enter) {
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
            else if (kp->code == sf::Keyboard::Key::Escape) {
                waitingForInput = true;
                usernameInput.clear();
                passwordInput.clear();
                selectedOption = 0;
            }
        }
        return LoginResult::NONE;
    }

    string getUsername() const { return usernameInput; }

    void draw(sf::RenderWindow& window) {
        float W = (float)window.getSize().x;
        float H = (float)window.getSize().y;

        background.setSize({ W, H });
        background.setFillColor(bgColor);
        window.draw(background);

        // Title (your original)
        T(titleText).setString("SNOW BROS");
        T(titleText).setCharacterSize(48);
        T(titleText).setFillColor(gold);
        T(titleText).setLetterSpacing(3.f);
        sf::FloatRect tb = T(titleText).getLocalBounds();
        T(titleText).setOrigin({ tb.size.x / 2.f, 0.f });
        T(titleText).setPosition({ W / 2.f, 80.f });
        window.draw(T(titleText));

        // ---- Phase 1: Username (your original draw) ----
        if (waitingForInput) {
            T(promptText).setString("Enter Your Username:");
            T(promptText).setCharacterSize(18);
            T(promptText).setFillColor(mutedColor);
            sf::FloatRect pb = T(promptText).getLocalBounds();
            T(promptText).setOrigin({ pb.size.x / 2.f, 0.f });
            T(promptText).setPosition({ W / 2.f, 220.f });
            window.draw(T(promptText));

            inputBox.setSize({ 320.f, 48.f });
            inputBox.setPosition({ W / 2.f - 160.f, 260.f });
            inputBox.setFillColor(sf::Color(255, 255, 255, 15));
            inputBox.setOutlineThickness(1.f);
            inputBox.setOutlineColor(gold);
            window.draw(inputBox);

            T(inputText).setString(usernameInput + "|");
            T(inputText).setCharacterSize(20);
            T(inputText).setFillColor(lightText);
            T(inputText).setPosition({ W / 2.f - 148.f, 270.f });
            window.draw(T(inputText));

            if (!errorMsg.empty()) {
                T(errorText).setString(errorMsg);
                T(errorText).setCharacterSize(13);
                T(errorText).setFillColor(redErr);
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

        // ---- Phase 2 (NEW): Password entry ----
        else if (waitingForPassword) {
            T(promptText).setString("Welcome back, " + usernameInput + "!");
            T(promptText).setCharacterSize(18);
            T(promptText).setFillColor(greenOk);
            sf::FloatRect pb = T(promptText).getLocalBounds();
            T(promptText).setOrigin({ pb.size.x / 2.f, 0.f });
            T(promptText).setPosition({ W / 2.f, 200.f });
            window.draw(T(promptText));

            T(inputText).setString("Enter Password:");
            T(inputText).setCharacterSize(16);
            T(inputText).setFillColor(mutedColor);
            sf::FloatRect il = T(inputText).getLocalBounds();
            T(inputText).setOrigin({ il.size.x / 2.f, 0.f });
            T(inputText).setPosition({ W / 2.f, 252.f });
            window.draw(T(inputText));

            passwordBox.setSize({ 320.f, 48.f });
            passwordBox.setPosition({ W / 2.f - 160.f, 278.f });
            passwordBox.setFillColor(sf::Color(255, 255, 255, 15));
            passwordBox.setOutlineThickness(1.f);
            passwordBox.setOutlineColor(gold);
            window.draw(passwordBox);

            string masked(passwordInput.size(), '*');
            T(passwordText).setString(masked + "|");
            T(passwordText).setCharacterSize(20);
            T(passwordText).setFillColor(lightText);
            T(passwordText).setPosition({ W / 2.f - 148.f, 288.f });
            window.draw(T(passwordText));

            if (!errorMsg.empty()) {
                T(errorText).setString(errorMsg);
                T(errorText).setCharacterSize(13);
                T(errorText).setFillColor(redErr);
                sf::FloatRect eb = T(errorText).getLocalBounds();
                T(errorText).setOrigin({ eb.size.x / 2.f, 0.f });
                T(errorText).setPosition({ W / 2.f, 338.f });
                window.draw(T(errorText));
            }

            T(errorText).setString("Enter to Login   |   ESC to go back");
            T(errorText).setCharacterSize(11);
            T(errorText).setFillColor(mutedColor);
            sf::FloatRect eb2 = T(errorText).getLocalBounds();
            T(errorText).setOrigin({ eb2.size.x / 2.f, 0.f });
            T(errorText).setPosition({ W / 2.f, H - 40.f });
            window.draw(T(errorText));
        }

        // ---- Phase 3: Menu (your original draw) ----
        else {
            T(promptText).setString("Welcome " + usernameInput + "!");
            T(promptText).setCharacterSize(20);
            T(promptText).setFillColor(greenOk);
            sf::FloatRect pb = T(promptText).getLocalBounds();
            T(promptText).setOrigin({ pb.size.x / 2.f, 0.f });
            T(promptText).setPosition({ W / 2.f - 15.f, 200.f });
            window.draw(T(promptText));

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