#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <optional>
#include "StarLevel.h"
using namespace std;

class StarLevelScreen {
public:
    StarLevelScreen() = default;
    StarLevelScreen(const StarLevelScreen&) = delete;
    StarLevelScreen& operator=(const StarLevelScreen&) = delete;

    bool loadAssets(const string& fontPath,
        const string& bgPath = "assets\\images\\StarLevel_bg.png");

    void reset();
    void handleEvent(const Event& event);
    void draw(RenderWindow& window);

    int  result = -1;
    bool done = false;

    string getChosenPowerUp() const {
        if (result < 0 || result >= NUM_CARDS) return "";
        return starEvent.getChoice(result);
    }

private:
    static constexpr int   NUM_CARDS = 3;
    static constexpr float CARD_W = 170.f;
    static constexpr float CARD_H = 230.f;
    static constexpr float CARD_Y = 200.f;
    static constexpr float CARD_GAP = 30.f;
    static constexpr float TITLE_Y = 40.f;
    static constexpr float SUBTITLE_Y = 78.f;
    static constexpr float CARDS_START_X = (800.f - NUM_CARDS * CARD_W - (NUM_CARDS - 1) * CARD_GAP) / 2.f;
    const Color C_CARD_IDLE{ 10,  30,  70, 200 };
    const Color C_CARD_HOVER{ 20,  60, 130, 230 };
    const Color C_OUT_IDLE{ 60, 140, 220, 180 };
    const Color C_OUT_HOVER{ 140, 210, 255, 255 };
    const Color C_TITLE_CLR{ 200, 235, 255, 240 };
    const Color C_DESC_CLR{ 160, 200, 240, 210 };
    const Color C_STAR_CLR{ 255, 230,  80, 255 };

    StarEvent starEvent;
    int hoveredCard = 0;
    bool layoutReady = false;

    Font font;
    Texture bgTex;
    bool bgLoaded = false;
    optional<Sprite> bgSprite;

    Texture itemsTex;
    bool itemsLoaded = false;

    optional<Text>txtTitle;
    optional<Text>txtSubtitle;
    RectangleShape cardShape[NUM_CARDS];
    optional<Text> txtCardName[NUM_CARDS];
    optional<Text> txtCardDesc[NUM_CARDS];
    optional<Text> txtHotkey[NUM_CARDS];
    optional<Sprite> potionSprite[NUM_CARDS];
    Text& TX(optional<Text>& t) { return t.value(); }
    IntRect rectFor(const string& name) const {
        if (name == "Speed Boost") return { {325,  17}, {140, 161} };
        if (name == "Snowball Power") return { {325, 166}, {137, 166} };
        if (name == "Distance Increase") return { {333, 326}, {126, 163} };
        if (name == "Balloon Mode") return { {349, 485}, {118, 163} };
        return { {325, 17}, {140, 161} };
    }

    string descriptionFor(const string& name) const {
        if (name == "Speed Boost")  return "+50% movement\nspeed for 15s";
        if (name == "Snowball Power") return "1-hit encase.\nLasts whole level";
        if (name == "Distance Increase") return "Full-width throw.\nLasts whole level";
        if (name == "Balloon Mode") return "Float upward\nfor 10 seconds";
        return "Mystery power!";
    }

    sf::FloatRect cardBounds(int i) const {
        float x = CARDS_START_X + i * (CARD_W + CARD_GAP);
        return { {x, CARD_Y}, {CARD_W, CARD_H} };
    }

    void setupLayout();
    void refreshCard(int i);
    void selectCard(int i);
    void handleClick(sf::Vector2f mp);
};



inline bool StarLevelScreen::loadAssets(const string& fontPath, const string& bgPath)
{
    if (!font.openFromFile(fontPath)) return false;

    bgLoaded = bgTex.loadFromFile(bgPath);
    if (bgLoaded) {
        bgSprite.emplace(bgTex);
        Vector2u ts = bgTex.getSize();
        bgSprite->setScale({800.f / static_cast<float>(ts.x), 600.f / static_cast<float>(ts.y) });
    }

    itemsLoaded = itemsTex.loadFromFile("assets\\images\\Items.png");
    txtTitle.emplace(font, "STAR  LEVEL!", 22u);
    txtSubtitle.emplace(font, "Choose your power-up", 13u);

    for (int i = 0; i < NUM_CARDS; i++) {
        txtCardName[i].emplace(font, "", 11u);
        txtCardDesc[i].emplace(font, "", 11u);
        txtHotkey[i].emplace(font, "Press " + to_string(i + 1), 10u);
    }

    return true;
}

inline void StarLevelScreen::reset()
{
    starEvent.generateChoices();
    hoveredCard = 0;
    result = -1;
    done = false;
    layoutReady = false;   // force setupLayout on next draw
}

inline void StarLevelScreen::setupLayout()
{
    TX(txtTitle).setFillColor(C_STAR_CLR);
    TX(txtTitle).setStyle(Text::Bold);
    {
        auto lb = TX(txtTitle).getLocalBounds();
        TX(txtTitle).setPosition({400.f - lb.size.x * 0.5f - lb.position.x, TITLE_Y });
    }

    TX(txtSubtitle).setFillColor(C_TITLE_CLR);
    {
        auto lb = TX(txtSubtitle).getLocalBounds();
        TX(txtSubtitle).setPosition({400.f - lb.size.x * 0.5f - lb.position.x, SUBTITLE_Y });
    }

    for (int i = 0; i < NUM_CARDS; i++) {
        sf::FloatRect b = cardBounds(i);
        float cx = b.position.x;
        float cy = b.position.y;

        cardShape[i].setSize({ CARD_W, CARD_H });
        cardShape[i].setPosition({ cx, cy });
        cardShape[i].setOutlineThickness(2.5f);

        string nm = starEvent.getChoice(i);

        potionSprite[i].reset();
        if (itemsLoaded) {
            potionSprite[i].emplace(itemsTex);
            sf::IntRect r = rectFor(nm);
            potionSprite[i]->setTextureRect(r);
            float sw = 64.f / static_cast<float>(r.size.x);
            float sh = 72.f / static_cast<float>(r.size.y);
            potionSprite[i]->setScale({ sw, sh });
            potionSprite[i]->setPosition({ cx + (CARD_W - 64.f) * 0.5f,  cy + 18.f  });
        }

        TX(txtCardName[i]).setString(nm);
        TX(txtCardName[i]).setStyle(sf::Text::Bold);
        {
            auto lb = TX(txtCardName[i]).getLocalBounds();
            TX(txtCardName[i]).setPosition({ cx + (CARD_W - lb.size.x) * 0.5f - lb.position.x, cy + 108.f });
        }

        TX(txtCardDesc[i]).setString(descriptionFor(nm));
        TX(txtCardDesc[i]).setFillColor(C_DESC_CLR);
        TX(txtCardDesc[i]).setLineSpacing(1.5f);
        {
            auto lb = TX(txtCardDesc[i]).getLocalBounds();
            TX(txtCardDesc[i]).setPosition({ cx + (CARD_W - lb.size.x) * 0.5f - lb.position.x,  cy + 148.f});
        }

        TX(txtHotkey[i]).setFillColor(sf::Color(120, 180, 240, 180));
        {
            auto lb = TX(txtHotkey[i]).getLocalBounds();
            TX(txtHotkey[i]).setPosition({cx + (CARD_W - lb.size.x) * 0.5f - lb.position.x,cy + CARD_H - 24.f });
        }

        refreshCard(i);
    }

    layoutReady = true;
}

inline void StarLevelScreen::refreshCard(int i)
{
    bool hovered = (i == hoveredCard);
    cardShape[i].setFillColor(hovered ? C_CARD_HOVER : C_CARD_IDLE);
    cardShape[i].setOutlineColor(hovered ? C_OUT_HOVER : C_OUT_IDLE);
    TX(txtCardName[i]).setFillColor(
        hovered ? sf::Color(255, 255, 255, 255) : C_TITLE_CLR);
}

inline void StarLevelScreen::selectCard(int i)
{
    result = i;
    done = true;
}

inline void StarLevelScreen::handleClick(sf::Vector2f mp)
{
    for (int i = 0; i < NUM_CARDS; i++) {
        if (cardBounds(i).contains(mp)) {
            selectCard(i);
            return;
        }
    }
}

inline void StarLevelScreen::handleEvent(const Event& event)
{
    if (!layoutReady) return;

    if (const auto* kp = event.getIf<Event::KeyPressed>()) {
        switch (kp->code) {
        case Keyboard::Key::Num1: selectCard(0); break;
        case Keyboard::Key::Num2: selectCard(1); break;
        case Keyboard::Key::Num3: selectCard(2); break;
        case Keyboard::Key::Left:
            hoveredCard = (hoveredCard - 1 + NUM_CARDS) % NUM_CARDS;
            for (int j = 0; j < NUM_CARDS; j++) refreshCard(j);
            break;
        case Keyboard::Key::Right:
            hoveredCard = (hoveredCard + 1) % NUM_CARDS;
            for (int j = 0; j < NUM_CARDS; j++) refreshCard(j);
            break;
        case Keyboard::Key::Enter:
        case Keyboard::Key::Space:
            selectCard(hoveredCard);
            break;
        default: break;
        }
        return;
    }

    if (const auto* mm = event.getIf<sf::Event::MouseMoved>()) {
        sf::Vector2f mp(static_cast<float>(mm->position.x),
            static_cast<float>(mm->position.y));
        for (int i = 0; i < NUM_CARDS; i++) {
            if (cardBounds(i).contains(mp)) {
                hoveredCard = i;
                for (int j = 0; j < NUM_CARDS; j++) refreshCard(j);
                break;
            }
        }
    }

    if (const auto* mb = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mb->button == sf::Mouse::Button::Left)
            handleClick({ static_cast<float>(mb->position.x),static_cast<float>(mb->position.y) });
    }
}

inline void StarLevelScreen::draw(sf::RenderWindow& window)
{
    if (!layoutReady) setupLayout();

    if (bgLoaded) window.draw(*bgSprite);
    else {
        RectangleShape fb({ 800.f, 600.f });
        fb.setFillColor(sf::Color(5, 12, 40, 255));
        window.draw(fb);
    }

    RectangleShape topBar({ 800.f, 110.f });
    topBar.setFillColor(sf::Color(0, 5, 20, 160));
    window.draw(topBar);

    window.draw(TX(txtTitle));
    window.draw(TX(txtSubtitle));

    for (int i = 0; i < NUM_CARDS; i++) {
        window.draw(cardShape[i]);
        if (potionSprite[i].has_value())
            window.draw(*potionSprite[i]);
        window.draw(TX(txtCardName[i]));
        window.draw(TX(txtCardDesc[i]));
        window.draw(TX(txtHotkey[i]));
    }
}