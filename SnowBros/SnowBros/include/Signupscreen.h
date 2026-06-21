#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include "PasswordHasher.h"
using namespace std;

enum class SignupResult {
    NONE,
    DONE,
    BACK
};

class SignupScreen {
private:
    sf::Font font;
    bool fontLoaded = false;
    sf::Font fontTitle;
    sf::Font fontBody;
    bool extraFontsLoaded = false;
    string username;
    string passwordInput;
    string confirmInput;

    int  focusedField = 0;
    string errorMsg;
    bool success = false;

    sf::Color bgFallback = sf::Color(5, 12, 35, 255);
    sf::Color titleCol = sf::Color(200, 240, 255, 255);   // ice white-blue
    sf::Color subCol = sf::Color(160, 215, 255, 230);   // softer ice blue
    sf::Color iceWhite = sf::Color(220, 245, 255, 255);   // typed text
    sf::Color iceMid = sf::Color(210, 235, 255, 255);   // labels
    sf::Color iceDim = sf::Color(100, 160, 220, 180);   // hint
    sf::Color greenOk = sf::Color(180, 255, 230, 255);   // success / subtitle
    sf::Color redErr = sf::Color(220, 80, 80, 255);
    sf::Color inputFill = sf::Color(8, 22, 55, 200);
    sf::Color inputOutAct = sf::Color(160, 230, 255, 255);  // focused border
    sf::Color inputOutNrm = sf::Color(60, 120, 200, 140);   // unfocused border

    sf::RectangleShape background;
    sf::RectangleShape passwordBox;
    sf::RectangleShape confirmBox;
    sf::Texture bgTex;
    bool        bgLoaded = false;
    optional<sf::Sprite> bgSprite;
    optional<sf::Text> titleText;
    optional<sf::Text> subtitleText;
    optional<sf::Text> pwLabel;
    optional<sf::Text> pwText;
    optional<sf::Text> cfLabel;
    optional<sf::Text> cfText;
    optional<sf::Text> errorText;
    optional<sf::Text> hintText;

    sf::Text& T(optional<sf::Text>& t) { return t.value(); }

    // Appends "username:SALT$HASH" to users.dat
    bool saveUser(const string& uname, const string& password) {
        string hash = PasswordHasher::hashPassword(password);
        ofstream file("users.dat", ios::app);
        if (!file.is_open()) return false;
        file << uname << ":" << hash << "\n";
        return true;
    }

public:
    SignupScreen() {}

    bool loadFont(const string& path, const string& titlePath, const string& bodyPath)
    {
        if (!font.openFromFile(path)) return false;
        extraFontsLoaded = fontTitle.openFromFile(titlePath);
        if (!extraFontsLoaded) cout << "fontTitle not loaded" << endl;
        bool fontBodyLoaded = fontBody.openFromFile(bodyPath);
        if (!fontBodyLoaded) cout << "fontBody not loaded" << endl;
        bgLoaded = bgTex.loadFromFile("assets\\images\\Login_bg.png");
        if (bgLoaded) {
            bgSprite.emplace(bgTex);
            sf::Vector2u ts = bgTex.getSize();
            bgSprite->setScale({ 800.f / static_cast<float>(ts.x), 600.f / static_cast<float>(ts.y) });
        }
        fontLoaded = true;
        titleText.emplace(extraFontsLoaded ? fontTitle : font);
        subtitleText.emplace(fontBody);
        pwLabel.emplace(fontBody);
        pwText.emplace(fontBody);
        cfLabel.emplace(fontBody);
        cfText.emplace(fontBody);
        errorText.emplace(fontBody);
        hintText.emplace(fontBody);
        return true;
    }

    void setUsername(const string& uname)
    {
        username = uname;
        passwordInput.clear();
        confirmInput.clear();
        errorMsg.clear();
        focusedField = 0;
        success = false;
    }

    SignupResult handleEvent(sf::Event& event) {
        if (auto* mp = event.getIf<sf::Event::MouseButtonPressed>())
        {
            float cy = (float)mp->position.y;
            if (cy >= 232.f && cy <= 276.f) focusedField = 0;
            else if (cy >= 317.f && cy <= 361.f) focusedField = 1;
        }

        if (auto* kp = event.getIf<sf::Event::KeyPressed>())
        {
            if (kp->code == sf::Keyboard::Key::Tab ||
                kp->code == sf::Keyboard::Key::Down ||
                kp->code == sf::Keyboard::Key::Up) {
                focusedField = (focusedField + 1) % 2;
            }
            else if (kp->code == sf::Keyboard::Key::Backspace)
            {
                string& field = (focusedField == 0) ? passwordInput : confirmInput;
                if (!field.empty()) field.pop_back();
            }
            else if (kp->code == sf::Keyboard::Key::Escape)
            {
                return SignupResult::BACK;
            }
            else if (kp->code == sf::Keyboard::Key::Enter)
            {
                // Validate
                if (passwordInput.empty() || confirmInput.empty())
                {
                    errorMsg = "Please fill in both fields.";
                    return SignupResult::NONE;
                }
                if (passwordInput.size() < 4)
                {
                    errorMsg = "Password must be at least 4 characters.";
                    return SignupResult::NONE;
                }
                if (passwordInput != confirmInput)
                {
                    errorMsg = "Passwords do not match!";
                    confirmInput.clear();
                    return SignupResult::NONE;
                }
                // Save hashed password
                if (!saveUser(username, passwordInput))
                {
                    errorMsg = "Error saving account. Try again.";
                    return SignupResult::NONE;
                }
                success = true;
                errorMsg = "";
                return SignupResult::DONE;
            }
        }

        if (auto* te = event.getIf<sf::Event::TextEntered>())
        {
            uint32_t c = te->unicode;
            if (c >= 32 && c < 127) {
                string& field = (focusedField == 0) ? passwordInput : confirmInput;
                if (field.size() < 30)
                    field += static_cast<char>(c);
            }
        }

        return SignupResult::NONE;
    }

    void draw(sf::RenderWindow& window) {
        float W = (float)window.getSize().x;
        float H = (float)window.getSize().y;

        if (bgLoaded) window.draw(*bgSprite);
        else {
            sf::RectangleShape fb({ 800.f, 600.f }); fb.setFillColor(bgFallback); window.draw(fb);
        }
        sf::RectangleShape vignette({ 800.f, 600.f });
        vignette.setFillColor(sf::Color(0, 5, 20, 110));
        window.draw(vignette);

        // glow behind card
        sf::CircleShape cardGlow(250.f);
        cardGlow.setOrigin({ 250.f, 250.f });
        cardGlow.setPosition({ W / 2.f, 310.f });
        cardGlow.setFillColor(sf::Color(150, 220, 255, 18));
        window.draw(cardGlow);

        // glass card
        sf::RectangleShape card({ 380.f, 340.f });
        card.setPosition({ W / 2.f - 190.f, 140.f });
        card.setFillColor(sf::Color(10, 25, 70, 80));
        card.setOutlineThickness(1.5f);
        card.setOutlineColor(sf::Color(180, 230, 255, 140));
        window.draw(card);


        T(titleText).setString("SNOW BROS");
        T(titleText).setCharacterSize(36u);
        T(titleText).setStyle(sf::Text::Bold);
        T(titleText).setLetterSpacing(5.f);
        T(titleText).setOutlineThickness(2.f);
        T(titleText).setOutlineColor(sf::Color(50, 130, 200, 200));
        sf::FloatRect tb = T(titleText).getLocalBounds();
        T(titleText).setOrigin({ tb.size.x / 2.f, 0.f });
        // shadow
        T(titleText).setFillColor(sf::Color(0, 0, 0, 100));
        T(titleText).setPosition({ W / 2.f + 2.f, 62.f });
        window.draw(T(titleText));
        // main
        T(titleText).setFillColor(titleCol);
        T(titleText).setPosition({ W / 2.f, 60.f });
        window.draw(T(titleText));

        T(subtitleText).setString("Create Password for: " + username);
        T(subtitleText).setCharacterSize(18);
        T(subtitleText).setFillColor(sf::Color(190, 230, 255, 255));
        sf::FloatRect sb = T(subtitleText).getLocalBounds();
        T(subtitleText).setOrigin({ sb.size.x / 2.f, 0.f });
        T(subtitleText).setPosition({ W / 2.f, 155.f });
        window.draw(T(subtitleText));

        T(pwLabel).setString("Password:");
        T(pwLabel).setCharacterSize(16);
        T(pwLabel).setFillColor(iceMid);
        T(pwLabel).setPosition({ W / 2.f - 160.f, 210.f });
        window.draw(T(pwLabel));

        passwordBox.setSize({ 320.f, 44.f });
        passwordBox.setPosition({ W / 2.f - 160.f, 232.f });
        passwordBox.setFillColor(sf::Color(20, 40, 90, 140));
        passwordBox.setOutlineThickness(2.f);
        passwordBox.setOutlineColor(focusedField == 0 ? sf::Color(180, 230, 255, 220) : sf::Color(80, 140, 220, 140));
        window.draw(passwordBox);

        string pwMasked(passwordInput.size(), '*');
        if (focusedField == 0) pwMasked += "|";
        T(pwText).setString(pwMasked);
        T(pwText).setCharacterSize(20);
        T(pwText).setFillColor(iceWhite);
        T(pwText).setPosition({ W / 2.f - 148.f, 242.f });
        window.draw(T(pwText));

        T(cfLabel).setString("Confirm Password:");
        T(cfLabel).setCharacterSize(16);
        T(cfLabel).setFillColor(iceMid);
        T(cfLabel).setPosition({ W / 2.f - 160.f, 295.f });
        window.draw(T(cfLabel));

        confirmBox.setSize({ 320.f, 44.f });
        confirmBox.setPosition({ W / 2.f - 160.f, 317.f });
        confirmBox.setFillColor(sf::Color(20, 40, 90, 140));
        confirmBox.setOutlineThickness(2.f);
        confirmBox.setOutlineColor(focusedField == 1 ? sf::Color(180, 230, 255, 220) : sf::Color(80, 140, 220, 140));
        window.draw(confirmBox);

        string cfMasked(confirmInput.size(), '*');
        if (focusedField == 1) cfMasked += "|";
        T(cfText).setString(cfMasked);
        T(cfText).setCharacterSize(20);
        T(cfText).setFillColor(iceWhite);
        T(cfText).setPosition({ W / 2.f - 148.f, 327.f });
        window.draw(T(cfText));

        if (!errorMsg.empty()) {
            T(errorText).setString(errorMsg);
            T(errorText).setCharacterSize(13);
            T(errorText).setFillColor(redErr);
            sf::FloatRect eb = T(errorText).getLocalBounds();
            T(errorText).setOrigin({ eb.size.x / 2.f, 0.f });
            T(errorText).setPosition({ W / 2.f, 374.f });
            window.draw(T(errorText));
        }

        T(hintText).setString("Tab to switch field   |   Enter to confirm   |   ESC to go back");
        T(hintText).setCharacterSize(13);
        T(hintText).setFillColor(sf::Color(200, 230, 255, 220));
        sf::FloatRect hb = T(hintText).getLocalBounds();
        T(hintText).setOrigin({ hb.size.x / 2.f, 0.f });
        T(hintText).setPosition({ W / 2.f, H - 40.f });
        window.draw(T(hintText));
    }
    string getUsername() const { return username; }
};