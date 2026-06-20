#pragma once
#include <SFML/Graphics.hpp>
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

    sf::RectangleShape background;
    sf::RectangleShape topBar;
    sf::RectangleShape bottomBar;

    sf::Clock clock;
    float elapsed = 0.f;
    bool  done = false;

    sf::Color bgColor = sf::Color(8, 16, 48, 255);
    sf::Color titleColor = sf::Color(80, 220, 255, 255);
    sf::Color subColor = sf::Color(150, 255, 180, 255);
    sf::Color promptColor = sf::Color(200, 200, 255, 255);
    sf::Color barColor = sf::Color(80, 220, 255, 80);

    sf::Text& T(optional<sf::Text>& t) { return t.value(); }

public:
    bool isDone() const { return done; }

    bool load(const string& fontPath, const string& logoPath) {
        if (!font.openFromFile(fontPath)) return false;

        logoLoaded = logoTex.loadFromFile(logoPath);
        if (logoLoaded)
            logoSprite.emplace(logoTex);

        titleText.emplace(font);
        subText.emplace(font);
        promptText.emplace(font);
        return true;
    }

    void handleEvent(sf::Event& event) {
        if (event.getIf<sf::Event::KeyPressed>() ||
            event.getIf<sf::Event::MouseButtonPressed>())
            done = true;
    }

    void update(float deltaTime) {
        elapsed += deltaTime;
        if (elapsed >= 5.f)
            done = true;
    }

    void draw(sf::RenderWindow& window) {
        float W = (float)window.getSize().x;
        float H = (float)window.getSize().y;

        // background
        background.setSize({ W, H });
        background.setFillColor(bgColor);
        window.draw(background);

        // decorative bars
        topBar.setSize({ W, 5.f });
        topBar.setPosition({ 0.f, 0.f });
        topBar.setFillColor(barColor);
        window.draw(topBar);

        bottomBar.setSize({ W, 5.f });
        bottomBar.setPosition({ 0.f, H - 5.f });
        bottomBar.setFillColor(barColor);
        window.draw(bottomBar);

        // fade-in alpha
        uint8_t alpha = (uint8_t)std::min(255.f, elapsed * 255.f);

        // logo
        if (logoLoaded) {
            sf::Vector2u ts = logoTex.getSize();
            float scale = std::min(220.f / (float)ts.x, 220.f / (float)ts.y);
            logoSprite->setScale({ scale, scale });
            logoSprite->setColor(sf::Color(255, 255, 255, alpha));
            logoSprite->setPosition({
                W / 2.f - (ts.x * scale) / 2.f,
                H / 2.f - 200.f
                });
            window.draw(*logoSprite);
        }

        // title
        T(titleText).setString("SNOW BROS");
        T(titleText).setCharacterSize(52);
        T(titleText).setFillColor(sf::Color(titleColor.r, titleColor.g, titleColor.b, alpha));
        T(titleText).setLetterSpacing(3.f);
        sf::FloatRect tb = T(titleText).getLocalBounds();
        T(titleText).setOrigin({ tb.size.x / 2.f, 0.f });
        T(titleText).setPosition({ W / 2.f, H / 2.f + 40.f });
        window.draw(T(titleText));

        // subtitle
        T(subText).setString("Nick & Tom's Frozen Adventure");
        T(subText).setCharacterSize(14);
        T(subText).setFillColor(sf::Color(subColor.r, subColor.g, subColor.b, alpha));
        sf::FloatRect sb = T(subText).getLocalBounds();
        T(subText).setOrigin({ sb.size.x / 2.f, 0.f });
        T(subText).setPosition({ W / 2.f, H / 2.f + 110.f });
        window.draw(T(subText));

        // blinking prompt after 1.5s
        if (elapsed > 1.5f) {
            float blink = fmod(elapsed, 1.0f);
            uint8_t promptAlpha = blink < 0.5f ? (uint8_t)255 : (uint8_t)0;
            T(promptText).setString("Press any key to start");
            T(promptText).setCharacterSize(13);
            T(promptText).setFillColor(sf::Color(promptColor.r, promptColor.g, promptColor.b, promptAlpha));
            sf::FloatRect pb = T(promptText).getLocalBounds();
            T(promptText).setOrigin({ pb.size.x / 2.f, 0.f });
            T(promptText).setPosition({ W / 2.f, H / 2.f + 170.f });
            window.draw(T(promptText));
        }
    }
};