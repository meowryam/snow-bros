#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <algorithm>
using namespace std;

class SplashScreen {
private:
    sf::Font font;
    sf::Texture logoTex;
    bool logoLoaded = false;

    optional<sf::Sprite> logoSprite;
    optional<sf::Text>   titleText;
    optional<sf::Text>   subText;
    optional<sf::Text>   promptText;
    sf::Font fontTitle;    // Cinzel-Bold  — for SNOW BROS
    sf::Font fontSub;      // CormorantGaramond — for subtitle + prompt
    bool extraFontsLoaded = false;
    sf::RectangleShape background;


    sf::Clock clock;
    float elapsed = 0.f;
    bool  done = false;


    struct Snowflake { float x, y, speed, radius, alpha; };
    static const int SNOW_COUNT = 70;
    Snowflake snowflakes[SNOW_COUNT];
    bool snowInitialized = false;
    float shineTimer = 0.f;
    float auroraPulse = 0.f;
    struct Sparkle { float x, y, phase; };
    static const int SPARKLE_COUNT = 7;
    Sparkle sparkles[SPARKLE_COUNT] = {
        {380.f, 180.f, 0.f}, {420.f, 160.f, 1.2f}, {350.f, 220.f, 2.4f},
        {450.f, 200.f, 0.8f}, {390.f, 140.f, 1.8f}, {430.f, 230.f, 3.0f},
        {360.f, 170.f, 2.1f}
    };
    sf::Text& T(optional<sf::Text>& t) { return t.value(); }

public:
    bool isDone() const { return done; }

    bool load(const string& titleFontPath, const string& subFontPath, const string& logoPath) {
        if (!font.openFromFile(titleFontPath)) return false;
        extraFontsLoaded = fontTitle.openFromFile(titleFontPath);
        if (!extraFontsLoaded) cout << "fontTitle not loaded" << endl;
        bool fontSubLoaded = fontSub.openFromFile(subFontPath);
        if (!fontSubLoaded) cout << "fontSub not loaded" << endl;
        logoLoaded = logoTex.loadFromFile(logoPath);
        if (logoLoaded)
            logoSprite.emplace(logoTex);

        titleText.emplace(extraFontsLoaded ? fontTitle : font);
        subText.emplace(fontSub);
        promptText.emplace(fontSub);
        return true;
    }

    void handleEvent(sf::Event& event) {
        if (event.getIf<sf::Event::KeyPressed>() ||
            event.getIf<sf::Event::MouseButtonPressed>())
            done = true;
    }

    void update(float deltaTime) {
        elapsed += deltaTime;
        auroraPulse += deltaTime;
        shineTimer += deltaTime;
        if (!snowInitialized) {
            for (int i = 0; i < SNOW_COUNT; i++) {
                snowflakes[i].x = (float)(rand() % 800);
                snowflakes[i].y = (float)(rand() % 600);
                snowflakes[i].speed = 15.f + (rand() % 25);
                snowflakes[i].radius = 1.f + (rand() % 3);
                snowflakes[i].alpha = 80.f + (rand() % 80);
            }
            snowInitialized = true;
        }
        for (int i = 0; i < SNOW_COUNT; i++) {
            snowflakes[i].y += snowflakes[i].speed * deltaTime;
            if (snowflakes[i].y > 600.f) {
                snowflakes[i].y = 0.f;
                snowflakes[i].x = (float)(rand() % 800);
            }
        }
    }

    void draw(sf::RenderWindow& window) {
        float W = (float)window.getSize().x;
        float H = (float)window.getSize().y;

        // background
        background.setSize({ W, H });
        // DELETE: background.setFillColor(bgColor);
// REPLACE WITH:
        sf::RectangleShape bg({ W, H });
        bg.setFillColor(sf::Color(5, 12, 50, 255));
        window.draw(bg);



        // DELETE topBar and bottomBar draw calls entirely
// REPLACE WITH:
        sf::RectangleShape aurora({ W, H * 0.35f });
        aurora.setPosition({ 0.f, H * 0.3f });
        uint8_t auroraA = (uint8_t)(6.f + std::abs(std::sin(auroraPulse * 0.4f)) * 10.f);
        aurora.setFillColor(sf::Color(60, 160, 255, auroraA));
        window.draw(aurora);

        // DELETE: uint8_t alpha = (uint8_t)std::min(255.f, elapsed * 255.f);
        // REPLACE WITH:
        uint8_t alpha = (uint8_t)std::min(255.f, std::max(0.f, (elapsed - 0.f) * 255.f));
        uint8_t titleAlpha = (uint8_t)std::min(255.f, std::max(0.f, (elapsed - 1.f) * 255.f));
        uint8_t subAlpha = (uint8_t)std::min(255.f, std::max(0.f, (elapsed - 1.8f) * 255.f));

        // fade-in alpha
      // BEFORE the logo draw, add halo:
        sf::CircleShape moon(180.f);
        moon.setOrigin({ 180.f, 180.f });
        moon.setPosition({ W / 2.f, H / 2.f - 160.f });
        moon.setFillColor(sf::Color(120, 170, 255, 18));
        window.draw(moon);


        // logo
        if (logoLoaded) {
            sf::Vector2u ts = logoTex.getSize();
            float scale = std::min(340.f / (float)ts.x, 340.f / (float)ts.y);
            logoSprite->setScale({ scale, scale });
            logoSprite->setColor(sf::Color(255, 255, 255, alpha));
            logoSprite->setPosition({
                W / 2.f - (ts.x * scale) / 2.f,
                H / 2.f - 310.f
                });
            window.draw(*logoSprite);
        }

        // title
      // DELETE the single title draw block
// REPLACE WITH:
        T(titleText).setString("SNOW BROS");
        T(titleText).setCharacterSize(65);
        T(titleText).setLetterSpacing(3.f);
        T(titleText).setOutlineThickness(2.f);
        T(titleText).setOutlineColor(sf::Color(50, 130, 200, titleAlpha));
        sf::FloatRect tb = T(titleText).getLocalBounds();
        T(titleText).setOrigin({ tb.size.x / 2.f, 0.f });
        T(titleText).setPosition({ W / 2.f, H / 2.f + 35.f });
        // glow layer 1
        T(titleText).setFillColor(sf::Color(180, 235, 255, (uint8_t)(titleAlpha * 0.08f)));
        T(titleText).setPosition({ W / 2.f + 2.f, H / 2.f + 10.f });
        window.draw(T(titleText));
        T(titleText).setFillColor(sf::Color(180, 235, 255, (uint8_t)(titleAlpha * 0.18f)));
        T(titleText).setPosition({ W / 2.f + 1.f, H / 2.f + 9.f });
        window.draw(T(titleText));
        T(titleText).setFillColor(sf::Color(220, 245, 255, titleAlpha));
        T(titleText).setPosition({ W / 2.f, H / 2.f + 8.f });
        window.draw(T(titleText));

        // subtitle
        T(subText).setString("Nick & Tom's Frozen Adventure");


        T(subText).setCharacterSize(22);
        T(subText).setFillColor(sf::Color(190, 230, 255, subAlpha));
        T(subText).setPosition({ W / 2.f, H / 2.f + 110.f });


        sf::FloatRect sb = T(subText).getLocalBounds();
        T(subText).setOrigin({ sb.size.x / 2.f, 0.f });

        window.draw(T(subText));

        // blinking prompt after 1.5s
       // DELETE the old blink block
// REPLACE WITH:
        if (elapsed > 2.5f) {
            uint8_t promptAlpha = (uint8_t)(180.f + std::sin(elapsed * 3.f) * 75.f);
            T(promptText).setString("* Press any key to start *");
            T(promptText).setPosition({ W / 2.f, H / 2.f + 147.f });

            T(promptText).setCharacterSize(20);
            T(promptText).setFillColor(sf::Color(240, 250, 255, promptAlpha));
            sf::FloatRect pb = T(promptText).getLocalBounds();
            T(promptText).setOrigin({ pb.size.x / 2.f, 0.f });

            window.draw(T(promptText));
        }

        // snowflakes
        for (int i = 0; i < SNOW_COUNT; i++) {
            sf::CircleShape flake(snowflakes[i].radius);
            flake.setPosition({ snowflakes[i].x, snowflakes[i].y });
            flake.setFillColor(sf::Color(220, 240, 255, (uint8_t)snowflakes[i].alpha));
            window.draw(flake);
        }
        for (int i = 0; i < SPARKLE_COUNT; i++) {
            float sparkleAlpha = (std::sin(elapsed * 2.f + sparkles[i].phase) + 1.f) * 0.5f;
            sf::CircleShape sp(2.f);
            sp.setOrigin({ 2.f, 2.f });
            sp.setPosition({ sparkles[i].x, sparkles[i].y });
            sp.setFillColor(sf::Color(200, 180, 255, (uint8_t)(sparkleAlpha * 180.f)));
            window.draw(sp);
        }
        // crystal shine across title every 4 seconds
        float shineCycle = fmod(shineTimer, 4.f);
        if (shineCycle < 0.4f) {
            float shineX = (shineCycle / 0.4f) * W;
            sf::RectangleShape shine({ 18.f, 60.f });
            shine.setOrigin({ 9.f, 30.f });
            shine.setPosition({ shineX, H / 2.f + 70.f });
            shine.setRotation(sf::degrees(20.f));
            shine.setFillColor(sf::Color(255, 255, 255, 55));
            window.draw(shine);
        }

        float mistOffset = fmod(elapsed * 12.f, W);
        sf::RectangleShape mist1({ W + 100.f, H * 0.18f });
        mist1.setPosition({ -mistOffset, H * 0.80f });
        mist1.setFillColor(sf::Color(180, 220, 255, 8));
        window.draw(mist1);
        sf::RectangleShape mist2({ W + 100.f, H * 0.12f });
        mist2.setPosition({ mistOffset - W, H * 0.86f });
        mist2.setFillColor(sf::Color(200, 235, 255, 6));
        window.draw(mist2);
    }
};