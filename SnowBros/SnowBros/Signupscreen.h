#pragma once
#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <fstream>
#include "PasswordHasher.h"
using namespace std;

// Returned by SignupScreen to Game after processing
enum class SignupResult {
    NONE,       // still on screen
    DONE,       // account created successfully -> start NEW_GAME
    BACK        // user pressed ESC -> go back to LoginScreen
};

// -----------------------------------------------------------------------
// SignupScreen
//   Shown after "New Game" is chosen in LoginScreen.
//   Collects a password + confirm, hashes it, and appends to users.dat.
//   Game should then treat this exactly like LoginResult::NEW_GAME.
// -----------------------------------------------------------------------
class SignupScreen {
private:
    sf::Font font;
    bool fontLoaded = false;

    string username;            // passed in from LoginScreen (already chosen)
    string passwordInput;
    string confirmInput;

    int  focusedField = 0;      // 0 = password, 1 = confirm
    string errorMsg;
    bool success = false;

    // colours matching your LoginScreen palette
    sf::Color bgColor = sf::Color(26, 10, 46, 255);
    sf::Color gold = sf::Color(255, 229, 102, 255);
    sf::Color lightText = sf::Color(232, 224, 255, 255);
    sf::Color mutedColor = sf::Color(153, 132, 200, 255);
    sf::Color greenOk = sf::Color(76, 175, 125, 255);
    sf::Color redErr = sf::Color(226, 75, 74, 255);

    sf::RectangleShape background;
    sf::RectangleShape passwordBox;
    sf::RectangleShape confirmBox;

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

    bool loadFont(const string& path) {
        if (!font.openFromFile(path)) return false;
        fontLoaded = true;
        titleText.emplace(font);
        subtitleText.emplace(font);
        pwLabel.emplace(font);
        pwText.emplace(font);
        cfLabel.emplace(font);
        cfText.emplace(font);
        errorText.emplace(font);
        hintText.emplace(font);
        return true;
    }

    // Call this before switching to SIGNUP state so the screen knows who signed up
    void setUsername(const string& uname) {
        username = uname;
        passwordInput.clear();
        confirmInput.clear();
        errorMsg.clear();
        focusedField = 0;
        success = false;
    }

    SignupResult handleEvent(sf::Event& event) {
        if (auto* mp = event.getIf<sf::Event::MouseButtonPressed>()) {
            float cy = (float)mp->position.y;
            if (cy >= 232.f && cy <= 276.f) focusedField = 0;
            else if (cy >= 317.f && cy <= 361.f) focusedField = 1;
        }

        if (auto* kp = event.getIf<sf::Event::KeyPressed>()) {
            if (kp->code == sf::Keyboard::Key::Tab ||
                kp->code == sf::Keyboard::Key::Down ||
                kp->code == sf::Keyboard::Key::Up) {
                focusedField = (focusedField + 1) % 2;
            }
            else if (kp->code == sf::Keyboard::Key::Backspace) {
                string& field = (focusedField == 0) ? passwordInput : confirmInput;
                if (!field.empty()) field.pop_back();
            }
            else if (kp->code == sf::Keyboard::Key::Escape) {
                return SignupResult::BACK;
            }
            else if (kp->code == sf::Keyboard::Key::Enter) {
                // Validate
                if (passwordInput.empty() || confirmInput.empty()) {
                    errorMsg = "Please fill in both fields.";
                    return SignupResult::NONE;
                }
                if (passwordInput.size() < 4) {
                    errorMsg = "Password must be at least 4 characters.";
                    return SignupResult::NONE;
                }
                if (passwordInput != confirmInput) {
                    errorMsg = "Passwords do not match!";
                    confirmInput.clear();
                    return SignupResult::NONE;
                }
                // Save hashed password
                if (!saveUser(username, passwordInput)) {
                    errorMsg = "Error saving account. Try again.";
                    return SignupResult::NONE;
                }
                success = true;
                errorMsg = "";
                return SignupResult::DONE;
            }
        }

        if (auto* te = event.getIf<sf::Event::TextEntered>()) {
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

        // Background
        background.setSize({ W, H });
        background.setFillColor(bgColor);
        window.draw(background);

        // Title
        T(titleText).setString("SNOW BROS");
        T(titleText).setCharacterSize(48);
        T(titleText).setFillColor(gold);
        T(titleText).setLetterSpacing(3.f);
        sf::FloatRect tb = T(titleText).getLocalBounds();
        T(titleText).setOrigin({ tb.size.x / 2.f, 0.f });
        T(titleText).setPosition({ W / 2.f, 60.f });
        window.draw(T(titleText));

        // Subtitle
        T(subtitleText).setString("Create Password for: " + username);
        T(subtitleText).setCharacterSize(16);
        T(subtitleText).setFillColor(greenOk);
        sf::FloatRect sb = T(subtitleText).getLocalBounds();
        T(subtitleText).setOrigin({ sb.size.x / 2.f, 0.f });
        T(subtitleText).setPosition({ W / 2.f, 155.f });
        window.draw(T(subtitleText));

        // ---- Password field ----
        T(pwLabel).setString("Password:");
        T(pwLabel).setCharacterSize(14);
        T(pwLabel).setFillColor(focusedField == 0 ? gold : mutedColor);
        T(pwLabel).setPosition({ W / 2.f - 160.f, 210.f });
        window.draw(T(pwLabel));

        passwordBox.setSize({ 320.f, 44.f });
        passwordBox.setPosition({ W / 2.f - 160.f, 232.f });
        passwordBox.setFillColor(sf::Color(255, 255, 255, 15));
        passwordBox.setOutlineThickness(focusedField == 0 ? 2.f : 1.f);
        passwordBox.setOutlineColor(focusedField == 0 ? gold : sf::Color(80, 60, 120));
        window.draw(passwordBox);

        string pwMasked(passwordInput.size(), '*');
        if (focusedField == 0) pwMasked += "|";
        T(pwText).setString(pwMasked);
        T(pwText).setCharacterSize(20);
        T(pwText).setFillColor(lightText);
        T(pwText).setPosition({ W / 2.f - 148.f, 242.f });
        window.draw(T(pwText));

        // ---- Confirm field ----
        T(cfLabel).setString("Confirm Password:");
        T(cfLabel).setCharacterSize(14);
        T(cfLabel).setFillColor(focusedField == 1 ? gold : mutedColor);
        T(cfLabel).setPosition({ W / 2.f - 160.f, 295.f });
        window.draw(T(cfLabel));

        confirmBox.setSize({ 320.f, 44.f });
        confirmBox.setPosition({ W / 2.f - 160.f, 317.f });
        confirmBox.setFillColor(sf::Color(255, 255, 255, 15));
        confirmBox.setOutlineThickness(focusedField == 1 ? 2.f : 1.f);
        confirmBox.setOutlineColor(focusedField == 1 ? gold : sf::Color(80, 60, 120));
        window.draw(confirmBox);

        string cfMasked(confirmInput.size(), '*');
        if (focusedField == 1) cfMasked += "|";
        T(cfText).setString(cfMasked);
        T(cfText).setCharacterSize(20);
        T(cfText).setFillColor(lightText);
        T(cfText).setPosition({ W / 2.f - 148.f, 327.f });
        window.draw(T(cfText));

        // ---- Error / success message ----
        if (!errorMsg.empty()) {
            T(errorText).setString(errorMsg);
            T(errorText).setCharacterSize(13);
            T(errorText).setFillColor(redErr);
            sf::FloatRect eb = T(errorText).getLocalBounds();
            T(errorText).setOrigin({ eb.size.x / 2.f, 0.f });
            T(errorText).setPosition({ W / 2.f, 374.f });
            window.draw(T(errorText));
        }

        // ---- Bottom hint ----
        T(hintText).setString("Tab to switch field   |   Enter to confirm   |   ESC to go back");
        T(hintText).setCharacterSize(11);
        T(hintText).setFillColor(mutedColor);
        sf::FloatRect hb = T(hintText).getLocalBounds();
        T(hintText).setOrigin({ hb.size.x / 2.f, 0.f });
        T(hintText).setPosition({ W / 2.f, H - 40.f });
        window.draw(T(hintText));
    }
    string getUsername() const { return username; }
};