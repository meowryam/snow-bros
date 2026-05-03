#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <fstream>
#include "FileManager.h"
#include "PlayerData.h"
#include "PasswordHasher.h"
using namespace std;

enum class LoginResult
{
    NONE,
    NEW_GAME,
    CONTINUE,
    QUIT
};

class LoginScreen
{
private:
    sf::Font font;

    string usernameInput;
    string passwordInput;
    bool waitingForInput;
    bool waitingForPassword;

    sf::Color bgFallback = sf::Color(5, 12, 35, 255);
    sf::Color titleCol = sf::Color(180, 235, 255, 255);
    sf::Color iceWhite = sf::Color(220, 245, 255, 255);
    sf::Color iceMid = sf::Color(210, 235, 255, 255);
    sf::Color iceDim = sf::Color(160, 200, 240, 220);
    sf::Color greenOk = sf::Color(180, 255, 230, 255);
    sf::Color redErr = sf::Color(220, 80, 80, 255);
    sf::Color inputFill = sf::Color(8, 22, 55, 200);
    sf::Color inputOutAct = sf::Color(160, 230, 255, 255);
    sf::Color btnNormal = sf::Color(10, 35, 90, 140);
    sf::Color btnSel = sf::Color(30, 100, 220, 190);
    sf::Color btnOutNrm = sf::Color(70, 150, 230, 90);
    sf::Color btnOutSel = sf::Color(150, 225, 255, 255);

    sf::Texture bgTex;
    bool        bgLoaded = false;
    optional<sf::Sprite> bgSprite;

    sf::RectangleShape inputBox;
    sf::RectangleShape passwordBox;
    sf::RectangleShape optBtns[3];
    sf::RectangleShape divider;

    optional<sf::Text> txtTitle;
    optional<sf::Text> txtSub;
    optional<sf::Text> txtPrompt;
    optional<sf::Text> txtInputField;
    optional<sf::Text> txtPwdLabel;
    optional<sf::Text> txtPwdField;
    optional<sf::Text> txtOptions[3];
    optional<sf::Text> txtError;
    optional<sf::Text> txtHint;

    int    selectedOption = 0;
    bool   saveFound = false;
    string errorMsg;

    static constexpr float CX = 400.f;
    static constexpr float BW = 320.f;
    static constexpr float BH = 44.f;
    static constexpr float BTN_H = 40.f;
    static constexpr float BX = CX - BW * 0.5f;

    sf::Text& TX(optional<sf::Text>& t)
    {
        return t.value();
    }

    string loadHashForUser(const string& username) const
    {
        ifstream file("users.dat");
        if (!file.is_open()) return "";
        string line;
        while (getline(file, line))
        {
            size_t sep = line.find(':');
            if (sep == string::npos) continue;
            if (line.substr(0, sep) == username) return line.substr(sep + 1);
        }
        return "";
    }
    bool hasPassword(const string& username) const 
    {
        return !loadHashForUser(username).empty();
    }

    void centreText(sf::Text& t, float y)
    {
        auto lb = t.getLocalBounds();
        t.setPosition({ CX - lb.size.x * 0.5f, y });
    }

public:
    LoginScreen() : waitingForInput(true), waitingForPassword(false), selectedOption(0), saveFound(false) {}

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
        txtTitle.emplace(font, "SNOW  BROS", 32u);
        txtSub.emplace(font, "", 11u);
        txtPrompt.emplace(font, "", 12u);
        txtInputField.emplace(font, "", 15u);
        txtPwdLabel.emplace(font, "", 12u);
        txtPwdField.emplace(font, "", 15u);
        txtError.emplace(font, "", 10u);
        txtHint.emplace(font, "", 9u);
        for (int i = 0; i < 3; i++) txtOptions[i].emplace(font, "", 14u);
        return true;
    }

    LoginResult handleEvent(sf::Event& event)
    {
        if (waitingForInput)
        {
            if (auto* kp = event.getIf<sf::Event::KeyPressed>()) 
            {
                if (kp->code == sf::Keyboard::Key::Enter)
                {
                    if (usernameInput.empty())
                    {
                        errorMsg = "Please enter a username!"; 
                        return LoginResult::NONE;
                    }
                    errorMsg = "";
                    saveFound = FileManager::saveExists(usernameInput);
                    if (saveFound && hasPassword(usernameInput)) 
                    {
                        waitingForInput = false; 
                        waitingForPassword = true; 
                    }

                    else 
                    {
                        waitingForInput = false; 
                    }
                }
                else if (kp->code == sf::Keyboard::Key::Backspace) {
                    if (!usernameInput.empty()) usernameInput.pop_back();
                }
            }

            if (auto* te = event.getIf<sf::Event::TextEntered>()) 
            {
                char c = (char)te->unicode;
                if (usernameInput.size() < 16 && (isalnum(c) || c == '_') && c >= 32) 
                    usernameInput += c;
            }
            return LoginResult::NONE;
        }

        if (waitingForPassword) 
        {
            if (auto* kp = event.getIf<sf::Event::KeyPressed>())
            {
                if (kp->code == sf::Keyboard::Key::Enter) 
                {
                    if (passwordInput.empty()) 
                    { 
                        errorMsg = "Please enter your password!";
                    return LoginResult::NONE;
                    }
                    string stored = loadHashForUser(usernameInput);
                    if (PasswordHasher::verifyPassword(passwordInput, stored)) 
                    {
                        errorMsg = ""; waitingForPassword = false;
                        return LoginResult::CONTINUE;
                    }
                    else 
                    {
                        errorMsg = "Incorrect password. Try again."; 
                        passwordInput.clear();
                        return LoginResult::NONE;
                    }
                }
                else if (kp->code == sf::Keyboard::Key::Backspace) 
                {
                    if (!passwordInput.empty()) passwordInput.pop_back();
                }
                else if (kp->code == sf::Keyboard::Key::Escape)
                {
                    waitingForPassword = false; waitingForInput = true;
                    passwordInput.clear(); usernameInput.clear(); errorMsg.clear();
                }
            }
            if (auto* te = event.getIf<sf::Event::TextEntered>()) 
            {
                uint32_t c = te->unicode;
                if (passwordInput.size() < 30 && c >= 32 && c < 127)
                    passwordInput += static_cast<char>(c);
            }
            return LoginResult::NONE;
        }

        if (auto* kp = event.getIf<sf::Event::KeyPressed>())
        {
            int maxOpt = saveFound ? 3 : 2;
            if (kp->code == sf::Keyboard::Key::Up)   
                selectedOption = (selectedOption - 1 + maxOpt) % maxOpt;
            else if (kp->code == sf::Keyboard::Key::Down)
                selectedOption = (selectedOption + 1) % maxOpt;
            else if (kp->code == sf::Keyboard::Key::Enter)
            {
                if (saveFound) {
                    if (selectedOption == 0)
                        return LoginResult::NEW_GAME;

                    if (selectedOption == 1) 
                        return LoginResult::CONTINUE;

                    if (selectedOption == 2)
                        return LoginResult::QUIT;
                }
                else
                {
                    if (selectedOption == 0)
                        return LoginResult::NEW_GAME;

                    if (selectedOption == 1) 
                        return LoginResult::QUIT;
                }
            }
            else if (kp->code == sf::Keyboard::Key::Escape)
            {
                waitingForInput = true;
                usernameInput.clear();
                passwordInput.clear();
                selectedOption = 0;
            }
        }
        return LoginResult::NONE;
    }

    string getUsername() const 
    {
        return usernameInput;
    }

    void draw(sf::RenderWindow& win) 
    {
        if (bgLoaded) win.draw(*bgSprite);
        else
        {
            sf::RectangleShape fb({ 800.f, 600.f }); fb.setFillColor(bgFallback); win.draw(fb);
        }

        // Very subtle overlay so text is readable without killing the art
        sf::RectangleShape vignette({ 800.f, 600.f });
        vignette.setFillColor(sf::Color(0, 5, 20, 60));
        win.draw(vignette);

        // Title
        TX(txtTitle).setString("SNOW  BROS");
        TX(txtTitle).setCharacterSize(32u);
        TX(txtTitle).setFillColor(titleCol);
        TX(txtTitle).setStyle(sf::Text::Bold);
        TX(txtTitle).setLetterSpacing(5.f);
        centreText(TX(txtTitle), 88.f);
        win.draw(TX(txtTitle));

        // thin divider
        divider.setSize({ 260.f, 1.5f });
        divider.setPosition({ CX - 130.f, 143.f });
        divider.setFillColor(sf::Color(120, 200, 255, 80));
        win.draw(divider);

        // ── Phase 1 : username ────────────────────────────────────────
        if (waitingForInput)
        {
            TX(txtPrompt).setString("Enter Username:");
            TX(txtPrompt).setCharacterSize(12u);
            TX(txtPrompt).setFillColor(iceMid);
            TX(txtPrompt).setStyle(sf::Text::Regular);
            centreText(TX(txtPrompt), 185.f);
            win.draw(TX(txtPrompt));

            inputBox.setSize({ BW, BH });
            inputBox.setPosition({ BX, 210.f });
            inputBox.setFillColor(inputFill);
            inputBox.setOutlineThickness(1.8f);
            inputBox.setOutlineColor(inputOutAct);
            win.draw(inputBox);

            TX(txtInputField).setString(usernameInput + "|");
            TX(txtInputField).setCharacterSize(15u);
            TX(txtInputField).setFillColor(iceWhite);
            TX(txtInputField).setPosition({ BX + 12.f, 218.f });
            win.draw(TX(txtInputField));

            if (!errorMsg.empty()) 
            {
                TX(txtError).setString(errorMsg);
                TX(txtError).setCharacterSize(10u);
                TX(txtError).setFillColor(redErr);
                centreText(TX(txtError), 265.f);
                win.draw(TX(txtError));
            }

            TX(txtHint).setString("Press Enter to Continue");
            TX(txtHint).setCharacterSize(9u);
            TX(txtHint).setFillColor(iceDim);
            centreText(TX(txtHint), 570.f);
            win.draw(TX(txtHint));
        }

        // ── Phase 2 : password ────────────────────────────────────────
        else if (waitingForPassword) 
        {
            TX(txtSub).setString("Welcome back,  " + usernameInput + "!");
            TX(txtSub).setCharacterSize(13u);
            TX(txtSub).setFillColor(greenOk);
            TX(txtSub).setStyle(sf::Text::Regular);
            centreText(TX(txtSub), 170.f);
            win.draw(TX(txtSub));

            TX(txtPwdLabel).setString("Enter Password:");
            TX(txtPwdLabel).setCharacterSize(12u);
            TX(txtPwdLabel).setFillColor(iceMid);
            centreText(TX(txtPwdLabel), 208.f);
            win.draw(TX(txtPwdLabel));

            passwordBox.setSize({ BW, BH });
            passwordBox.setPosition({ BX, 232.f });
            passwordBox.setFillColor(inputFill);
            passwordBox.setOutlineThickness(1.8f);
            passwordBox.setOutlineColor(inputOutAct);
            win.draw(passwordBox);

            string masked(passwordInput.size(), '*');
            TX(txtPwdField).setString(masked + "|");
            TX(txtPwdField).setCharacterSize(15u);
            TX(txtPwdField).setFillColor(iceWhite);
            TX(txtPwdField).setPosition({ BX + 12.f, 240.f });
            win.draw(TX(txtPwdField));

            if (!errorMsg.empty())
            {
                TX(txtError).setString(errorMsg);
                TX(txtError).setCharacterSize(10u);
                TX(txtError).setFillColor(redErr);
                centreText(TX(txtError), 287.f);
                win.draw(TX(txtError));
            }

            TX(txtHint).setString("Enter to Login   |   ESC to go back");
            TX(txtHint).setCharacterSize(9u);
            TX(txtHint).setFillColor(iceDim);
            centreText(TX(txtHint), 570.f);
            win.draw(TX(txtHint));
        }

        // ── Phase 3 : menu ────────────────────────────────────────────
        else
        {
            TX(txtSub).setString("Welcome,  " + usernameInput + "!");
            TX(txtSub).setCharacterSize(13u);
            TX(txtSub).setFillColor(greenOk);
            TX(txtSub).setStyle(sf::Text::Regular);
            centreText(TX(txtSub), 168.f);
            win.draw(TX(txtSub));

            string opts[3];
            int optCount;
            if (saveFound) { opts[0] = "New Game"; opts[1] = "Continue"; opts[2] = "Quit"; optCount = 3; }
            else { opts[0] = "New Game"; opts[1] = "Quit"; optCount = 2; }

            float btnStartY = 212.f;
            float btnStep = BTN_H + 14.f;

            for (int i = 0; i < optCount; i++) 
            {
                bool sel = (i == selectedOption);
                float by = btnStartY + i * btnStep;

                optBtns[i].setSize({ BW, BTN_H });
                optBtns[i].setPosition({ BX, by });
                optBtns[i].setFillColor(sel ? btnSel : btnNormal);
                optBtns[i].setOutlineThickness(1.5f);
                optBtns[i].setOutlineColor(sel ? btnOutSel : btnOutNrm);
                win.draw(optBtns[i]);

                TX(txtOptions[i]).setString(opts[i]);
                TX(txtOptions[i]).setCharacterSize(14u);
                TX(txtOptions[i]).setFillColor(sel ? iceWhite : iceMid);
                TX(txtOptions[i]).setStyle(sel ? sf::Text::Bold : sf::Text::Regular);
                auto lb = TX(txtOptions[i]).getLocalBounds();
                TX(txtOptions[i]).setPosition({
                    BX + (BW - lb.size.x) * 0.5f,
                    by + (BTN_H - lb.size.y) * 0.5f - 2.f
                    });
                win.draw(TX(txtOptions[i]));
            }

            TX(txtHint).setString("ESC to retype   |   Arrows + Enter to select");
            TX(txtHint).setCharacterSize(9u);
            TX(txtHint).setFillColor(iceDim);
            centreText(TX(txtHint), 570.f);
            win.draw(TX(txtHint));
        }
    }
};