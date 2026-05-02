#pragma once
#include "PlayerData.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <cmath>
class ShopSystem {
private:
	PlayerData& playerData;
public:
	ShopSystem(PlayerData& data) : playerData(data) {}
	bool buyExtraLife() {
		if (playerData.getGemCount() >= 50) {
			playerData.setGemCount(playerData.getGemCount() - 50);
			playerData.setLives(playerData.getLives() + 1);
			return true;
		}
		else return false;
	}
	bool buySpeedBoost() {
		if (playerData.getGemCount() >= 20) {
			playerData.setGemCount(playerData.getGemCount() - 20);
			return true;
		}
		else return false;
	}
	bool buySnowBallPower() {
		if (playerData.getGemCount() >= 30) {
			playerData.setGemCount(playerData.getGemCount() - 30);
			return true;
		}
		else return false;
	}
	bool buyDistanceIncrease() {
		if (playerData.getGemCount() >= 25) {
			playerData.setGemCount(playerData.getGemCount() - 25);
			return true;
		}
		else return false;
	}
	bool buyBalloonMode() {
		if (playerData.getGemCount() >= 35) {
			playerData.setGemCount(playerData.getGemCount() - 35);
			return true;
		}
		else return false;
	}
	int totalGems() const {
		return playerData.getGemCount();
	}
    // Add inside ShopSystem class:
    bool buyLuckyScroll() {
        if (playerData.getGemCount() >= 40) {
            playerData.setGemCount(playerData.getGemCount() - 40);
            return true;  // caller decides which power-up was granted
        }
        return false;
    }
};
/*

class ShopScreen {
private:
    ShopSystem& shop;
    sf::Font    font;

    // FIX: In SFML 3, sf::Text has no default constructor — it requires a font.
    // We store them as optional and initialise them inside loadFont() after the
    // font file is successfully opened.
    std::optional<sf::Text> titleText;
    std::optional<sf::Text> gemText;
    std::optional<sf::Text> gemLabel;
    std::optional<sf::Text> itemNames[5];
    std::optional<sf::Text> itemDescs[5];
    std::optional<sf::Text> itemCosts[5];
    std::optional<sf::Text> buyBtnTexts[5];
    std::optional<sf::Text> messageText;

    sf::RectangleShape background;
    sf::RectangleShape panel;
    sf::RectangleShape gemBar;
    sf::RectangleShape itemBoxes[5];
    sf::RectangleShape buyButtons[5];

    sf::Clock messageClock;
    bool showMessage = false;
    bool layoutReady = false;

    // tracks one-time purchases
    bool purchased[5] = { false, false, false, false, false };

    struct ShopItem {
        string name;
        string desc;
        int    cost;
        bool   oneTime;
    };

    ShopItem items[5] = {
        { "Extra Life",     "+1 life immediately",         50, false },
        { "Speed Boost",    "50% faster for 30s",          20, false },
        { "Snowball Power", "1-hit encase, rest of level", 30, true  },
        { "Distance+",      "Full screen throw range",     25, true  },
        { "Balloon Mode",   "Float + immune 30s",          35, false },
    };

    // ── colours ──────────────────────────────────────────────────
    sf::Color bgColor = sf::Color(26, 10, 46, 255);
    sf::Color panelColor = sf::Color(38, 20, 65, 200);
    sf::Color boxAffordable = sf::Color(40, 70, 110, 220);
    sf::Color boxCantAfford = sf::Color(40, 35, 55, 140);
    sf::Color boxPurchased = sf::Color(30, 80, 60, 180);
    sf::Color gold = sf::Color(255, 229, 102, 255);
    sf::Color gemBlue = sf::Color(91, 191, 255, 255);
    sf::Color mutedPurple = sf::Color(153, 132, 200, 255);
    sf::Color lightText = sf::Color(232, 224, 255, 255);
    sf::Color greenOk = sf::Color(76, 175, 125, 255);
    sf::Color redErr = sf::Color(226, 75, 74, 255);

    void setupLayout(sf::RenderWindow& window);

    // Helper: get a safe reference (asserts optional is engaged)
    sf::Text& T(std::optional<sf::Text>& t) { return t.value(); }

public:
    explicit ShopScreen(ShopSystem& s) : shop(s) {}

    // Not copyable or movable (sf::Font and sf::Text are not copyable)
    ShopScreen(const ShopScreen&) = delete;
    ShopScreen& operator=(const ShopScreen&) = delete;

    // Call this after construction; returns false if font file not found.
    bool loadFont(const string& path);

    // Clears one-time purchase flags and resets layout so it redraws cleanly.
    // Call this instead of reassigning the whole object on reset.
    void reset() {
        for (int i = 0; i < 5; ++i)
            purchased[i] = false;
        showMessage = false;
        layoutReady = false;   // forces setupLayout() on next draw()
    }

    void draw(sf::RenderWindow& window);
    void handleClick(sf::Vector2f mousePos);
    void showMsg(const string& text, bool success);
};


*/
// ============================================================
//  ShopScreen
// ============================================================
class ShopScreen {
public:
    explicit ShopScreen(ShopSystem& s) : shop(s) {}
    ShopScreen(const ShopScreen&) = delete;
    ShopScreen& operator=(const ShopScreen&) = delete;

    ~ShopScreen() {
        for (int i = 0; i < ITEM_COUNT; i++) {
            delete icons[i];
            icons[i] = nullptr;
        }
    }

    bool loadFont(const string& path);

    void reset() {
        for (int i = 0; i < ITEM_COUNT; i++) purchased[i] = false;
        showMessage = false;
        layoutReady = false;
        selectedIndex = 0;
        done = false;
    }

    void handleEvent(const sf::Event& event);
    void handleClick(sf::Vector2f mousePos);
    void draw(sf::RenderWindow& window);

    bool done = false;

private:
    // ── layout ─────────────────────────────────────────────────
    static constexpr int   ITEM_COUNT = 6;
    static constexpr int   COLS = 3;

    // Smaller cards — give breathing room inside the arch
    // Arch usable zone: x 182–616 (434px wide), y 90–540 (450px tall)
    // 3×118 + 2×18 = 390 → 22px padding each side from shelf
    // 2×155 + 1×16 = 326 → vertically centred in 450px zone
    static constexpr float CARD_W = 118.f;
    static constexpr float CARD_H = 155.f;
    static constexpr float CARD_GAP_X = 18.f;
    static constexpr float CARD_GAP_Y = 16.f;
    static constexpr float GRID_LEFT = 204.f;
    static constexpr float GRID_TOP = 152.f;
    static constexpr float ICON_SIZE = 40.f;
    static constexpr float MSG_SECS = 2.5f;

    // ── item table ─────────────────────────────────────────────
    struct ItemDef {
        const char* name;
        const char* desc;
        int         cost;
        bool        oneTime;
        int sx, sy, sw, sh;   // rect in Items.png
    };

    // 6 items — slot 5 repeats Extra Life as placeholder
    // (swap it out for a real 6th item later)
    static constexpr ItemDef ITEMS[ITEM_COUNT] = {
        // ShopSystem.h — ITEMS table, change the desc strings:
{ "Extra Life",   "+1 life now",          50, false, 320, 647, 131, 155 },
{ "Speed Boost",  "1.5x speed, 30s",      20, false, 348,  27,  97, 141 },
{ "Snowball Pwr", "1-hit encase",         30, true,  348, 185,  97, 139 },
{ "Distance+",    "Full-screen throw",    25, true,  348, 342,  97, 141 },
{ "Balloon Mode", "Float+immune, 30s",    35, false, 348, 500,  97, 141 },
{ "Lucky Scroll", "Random power-up!",     40, false,  17, 974, 256, 345 },
    };

    // ── state ──────────────────────────────────────────────────
    ShopSystem& shop;
    sf::Font    font;
    bool        layoutReady = false;
    int         selectedIndex = 0;
    bool        purchased[ITEM_COUNT] = {};
    bool        showMessage = false;
    float       messageTimer = 0.f;
    bool        messageSuccess = false;

    sf::Texture          bgTex;
    bool                 bgLoaded = false;
    optional<sf::Sprite> bgSprite;

    sf::Texture  itemsTex;
    bool         itemsTexLoaded = false;
    sf::Sprite* icons[ITEM_COUNT] = {};

    sf::RectangleShape cards[ITEM_COUNT];
    sf::RectangleShape buyBtns[ITEM_COUNT];
    sf::RectangleShape gemBarBg;
    sf::RectangleShape backBtn;

    optional<sf::Text> txtTitle;
    optional<sf::Text> txtGemLabel;
    optional<sf::Text> txtGemCount;
    optional<sf::Text> txtItemName[ITEM_COUNT];
    optional<sf::Text> txtItemDesc[ITEM_COUNT];
    optional<sf::Text> txtItemCost[ITEM_COUNT];
    optional<sf::Text> txtBuyBtn[ITEM_COUNT];
    optional<sf::Text> txtMessage;
    optional<sf::Text> txtBack;

    // ── palette (Frozen / Ice Theme) ───────────────────────────

  // Cards — deep frozen glass look
    const sf::Color CARD_NORMAL{ 6, 18, 40, 185 };     // dark icy blue
    const sf::Color CARD_SEL{ 20, 55, 110, 215 };      // brighter ice glow
    const sf::Color CARD_BOUGHT{ 20, 95, 120, 235 }; // deeper, richer ice
    const sf::Color CARD_POOR{ 20, 35, 60, 200 }; // visible but muted
    // Buttons
    const sf::Color BTN_BUY{ 70, 170, 200, 235 };      // icy cyan
    const sf::Color BTN_OFF{ 40, 60, 90, 180 };
    const sf::Color BTN_OWN{ 60, 180, 170, 255 };
    const sf::Color COL_BTN_OWN_TXT{ 230, 255, 250, 255 };

    // Text colours — shift from gold → frost tones
    const sf::Color COL_NAME_NORMAL{ 190, 230, 255, 255 };   // ice white-blue
    const sf::Color COL_NAME_SEL{ 220, 245, 255, 255 };      // brighter frost
    const sf::Color COL_NAME_POOR{ 160, 180, 200, 220 };
    const sf::Color COL_DESC_POOR{ 130, 150, 170, 200 };
    const sf::Color COL_COST_POOR{ 100, 130, 150, 200 };

    const sf::Color COL_DESC_NORMAL{ 160, 200, 235, 210 };   // soft ice blue


    const sf::Color COL_COST{ 120, 220, 255, 255 };          // glowing cyan


    const sf::Color COL_BTN_TXT{ 240, 250, 255, 255 };       // frosty white
    const sf::Color COL_BTN_OFF_TXT{ 120, 140, 160, 180 };
   
    // Outlines — icy glow instead of purple
    const sf::Color OUTLINE_SEL{ 120, 220, 255, 255 };       // cyan glow
    const sf::Color OUTLINE_NRM{ 40, 90, 140, 160 };
    const sf::Color OUTLINE_POOR{ 25, 50, 80, 100 };
    const sf::Color OUTLINE_OWNED{ 150, 255, 240, 255 }; // crisp icy glow
    const sf::Color COL_NAME_OWNED{ 220, 255, 245, 255 };
    const sf::Color COL_DESC_OWNED{ 180, 235, 225, 220 };
   

    // UI accents
    const sf::Color GEM_COL{ 120, 220, 255, 255 };           // matches crystals
    const sf::Color GREEN_OK{ 100, 255, 200, 255 };          // icy success
    const sf::Color RED_ERR{ 220, 80, 80, 255 };             // keep readable

    // ── helpers ────────────────────────────────────────────────
    sf::Text& TX(optional<sf::Text>& t) { return t.value(); }
    sf::Vector2f cardTopLeft(int i) const;
    void setupLayout();
    void refreshCard(int i);
    void refreshGemCount();
    void tryBuy(int i);
    void postMessage(const string& msg, bool success);
};

// ──────────────────────────────────────────────────────────────
//  Implementation
// ──────────────────────────────────────────────────────────────

inline sf::Vector2f ShopScreen::cardTopLeft(int i) const {
    int col = i % COLS;
    int row = i / COLS;
    return {
        GRID_LEFT + col * (CARD_W + CARD_GAP_X),
        GRID_TOP + row * (CARD_H + CARD_GAP_Y)
    };
}

inline bool ShopScreen::loadFont(const string& path) {
    if (!font.openFromFile(path)) return false;

    bgLoaded = bgTex.loadFromFile("assets\\images\\Shop_bg.png");
    if (bgLoaded) {
        bgSprite.emplace(bgTex);
        sf::Vector2u ts = bgTex.getSize();
        bgSprite->setScale({
            800.f / static_cast<float>(ts.x),
            600.f / static_cast<float>(ts.y)
            });
    }

    itemsTexLoaded = itemsTex.loadFromFile("assets\\images\\Items.png");
    for (int i = 0; i < ITEM_COUNT; i++) {
        if (itemsTexLoaded) {
            icons[i] = new sf::Sprite(itemsTex);
            icons[i]->setTextureRect(
                sf::IntRect({ ITEMS[i].sx, ITEMS[i].sy }, { ITEMS[i].sw, ITEMS[i].sh }));
            icons[i]->setScale({
                ICON_SIZE / static_cast<float>(ITEMS[i].sw),
                ICON_SIZE / static_cast<float>(ITEMS[i].sh)
                });
        }
    }

    // Font sizes tuned to the smaller cards
    txtTitle.emplace(font, "SNOW  SHOP", 15u);
    txtGemLabel.emplace(font, "GEMS:", 8u);
    txtGemCount.emplace(font, "0", 11u);
    txtMessage.emplace(font, "", 8u);
    txtBack.emplace(font, "BACK  [ESC]", 8u);

    for (int i = 0; i < ITEM_COUNT; i++) {
        txtItemName[i].emplace(font, ITEMS[i].name, 7u);
        txtItemDesc[i].emplace(font, ITEMS[i].desc, 6u);
        txtItemCost[i].emplace(font, "", 6u);
        txtBuyBtn[i].emplace(font, "BUY", 7u);
    }
    return true;
}

inline void ShopScreen::setupLayout() {

    // Title — on the blank wooden sign (~x=401, y=32)
    TX(txtTitle).setFillColor(sf::Color(200, 235, 255, 255));
    TX(txtTitle).setStyle(sf::Text::Bold);
    {
        auto lb = TX(txtTitle).getLocalBounds();
        TX(txtTitle).setPosition({ 401.f - lb.size.x * 0.5f, 87.f });
    }

    // Gem counter — top right corner, compact
    gemBarBg.setSize({ 148.f, 40.f });
    gemBarBg.setPosition({ 638.f, 16.f });
    gemBarBg.setFillColor(sf::Color(6, 20, 40, 190));
    gemBarBg.setOutlineColor(GEM_COL);
    gemBarBg.setOutlineThickness(1.5f);
    TX(txtGemLabel).setFillColor(GEM_COL);
    TX(txtGemLabel).setPosition({ 648.f, 22.f });

    // Back button — bottom centre on wood floor strip
    backBtn.setSize({ 140.f, 32.f });
    backBtn.setPosition({ 330.f, 558.f });
    
   
    backBtn.setFillColor(sf::Color(10, 30, 60, 215));
    backBtn.setOutlineColor(sf::Color(120, 220, 255, 210));
    backBtn.setOutlineThickness(1.5f);
    TX(txtBack).setFillColor(sf::Color(220, 245, 255, 255));

  
    {
        auto lb = TX(txtBack).getLocalBounds();
        TX(txtBack).setPosition({
            330.f + (140.f - lb.size.x) * 0.5f,
            558.f + (32.f - lb.size.y) * 0.5f - 2.f
            });
    }

    // Message
    TX(txtMessage).setPosition({ 400.f, 526.f });

    // Cards
    for (int i = 0; i < ITEM_COUNT; i++) {
        sf::Vector2f cp = cardTopLeft(i);

        cards[i].setSize({ CARD_W, CARD_H });
        cards[i].setPosition(cp);
        cards[i].setOutlineThickness(1.5f);

        // Icon — top centre
        if (icons[i])
            icons[i]->setPosition({
                cp.x + (CARD_W - ICON_SIZE) * 0.5f,
                cp.y + 7.f
                });

        // Text layout inside card
        float tx = cp.x + 6.f;
        float nameY = cp.y + ICON_SIZE + 12.f;

        TX(txtItemName[i]).setPosition({ tx, nameY });
        TX(txtItemDesc[i]).setPosition({ tx, nameY + 14.f });

        TX(txtItemCost[i]).setString(to_string(ITEMS[i].cost) + " gems");
        TX(txtItemCost[i]).setPosition({ tx, nameY + 27.f });

        // Buy button — pinned to card bottom
        float btnY = cp.y + CARD_H - 30.f;
        buyBtns[i].setSize({ CARD_W - 12.f, 24.f });
        buyBtns[i].setPosition({ cp.x + 6.f, btnY });
        buyBtns[i].setOutlineThickness(1.f);

        {
            auto lb = TX(txtBuyBtn[i]).getLocalBounds();
            TX(txtBuyBtn[i]).setPosition({
                cp.x + 6.f + (CARD_W - 12.f - lb.size.x) * 0.5f,
                btnY + (24.f - lb.size.y) * 0.5f - 2.f
                });
        }

        refreshCard(i);
    }

    layoutReady = true;
}

inline void ShopScreen::refreshGemCount() {
    TX(txtGemCount).setString(to_string(shop.totalGems()));
    TX(txtGemCount).setFillColor(sf::Color::White);
    auto lb = TX(txtGemCount).getLocalBounds();
    TX(txtGemCount).setPosition({ 778.f - lb.size.x, 22.f });
}

inline void ShopScreen::refreshCard(int i) {
    bool canAfford = (shop.totalGems() >= ITEMS[i].cost);
    bool owned = purchased[i] && ITEMS[i].oneTime;
    bool sel = (i == selectedIndex);

    if (owned) {
        cards[i].setFillColor(CARD_BOUGHT);
        cards[i].setOutlineColor(OUTLINE_OWNED);
        buyBtns[i].setFillColor(BTN_OWN);
        buyBtns[i].setOutlineColor(sf::Color::Transparent);
        TX(txtBuyBtn[i]).setString("OWNED");
        TX(txtBuyBtn[i]).setFillColor(COL_BTN_OWN_TXT);
        TX(txtItemName[i]).setFillColor(COL_NAME_OWNED);
        TX(txtItemDesc[i]).setFillColor(COL_DESC_OWNED);
        TX(txtItemCost[i]).setFillColor(sf::Color(60, 130, 80, 180));
        cards[i].setOutlineThickness(sel ? 2.8f : 2.2f);
    }
    else if (!canAfford) {
        cards[i].setFillColor(CARD_POOR);
        cards[i].setOutlineColor(sel ? OUTLINE_SEL : OUTLINE_POOR);
        buyBtns[i].setFillColor(BTN_OFF);
        buyBtns[i].setOutlineColor(sf::Color::Transparent);
        TX(txtBuyBtn[i]).setString("BUY");
        TX(txtBuyBtn[i]).setFillColor(COL_BTN_OFF_TXT);
        TX(txtItemName[i]).setFillColor(COL_NAME_POOR);
        TX(txtItemDesc[i]).setFillColor(COL_DESC_POOR);
        TX(txtItemCost[i]).setFillColor(COL_COST_POOR);
    }
    else {
        cards[i].setFillColor(sel ? CARD_SEL : CARD_NORMAL);
        cards[i].setOutlineColor(sel ? OUTLINE_SEL : OUTLINE_NRM);
        buyBtns[i].setFillColor(BTN_BUY);
        buyBtns[i].setOutlineColor(sf::Color(75, 175, 95, 185));
        TX(txtBuyBtn[i]).setString("BUY");
        TX(txtBuyBtn[i]).setFillColor(COL_BTN_TXT);
        TX(txtItemName[i]).setFillColor(sel ? COL_NAME_SEL : COL_NAME_NORMAL);
        TX(txtItemDesc[i]).setFillColor(COL_DESC_NORMAL);
        TX(txtItemCost[i]).setFillColor(COL_COST);
    }

    // Selection border always overrides thickness
    if (sel) {
        cards[i].setOutlineColor(OUTLINE_SEL);
        cards[i].setOutlineThickness(2.5f);
    }
    else {
        cards[i].setOutlineThickness(1.5f);
    }
}

inline void ShopScreen::tryBuy(int i) {
    if (purchased[i] && ITEMS[i].oneTime) {
        postMessage("Already owned!", false);
        return;
    }

    bool ok = false;
    switch (i) {
    case 0: ok = shop.buyExtraLife();        break;
    case 1: ok = shop.buySpeedBoost();       break;
    case 2: ok = shop.buySnowBallPower();    break;
    case 3: ok = shop.buyDistanceIncrease(); break;
    case 4: ok = shop.buyBalloonMode();      break;
    case 5: ok = shop.buyLuckyScroll(); break;
    }

    if (ok) {
        if (ITEMS[i].oneTime) purchased[i] = true;
        postMessage(string(ITEMS[i].name) + " purchased!", true);
    }
    else {
        postMessage("Not enough gems!", false);
    }

    for (int j = 0; j < ITEM_COUNT; j++) refreshCard(j);
    refreshGemCount();
}

inline void ShopScreen::postMessage(const string& msg, bool success) {
    messageSuccess = success;
    showMessage = true;
    messageTimer = MSG_SECS;
    TX(txtMessage).setString(msg);
    TX(txtMessage).setFillColor(success ? GREEN_OK : RED_ERR);
    auto lb = TX(txtMessage).getLocalBounds();
    TX(txtMessage).setPosition({ 400.f - lb.size.x * 0.5f, 526.f });
}

inline void ShopScreen::handleEvent(const sf::Event& event) {
    if (!layoutReady) return;

    if (const auto* kp = event.getIf<sf::Event::KeyPressed>()) {
        switch (kp->code) {
        case sf::Keyboard::Key::Escape:
            done = true;
            break;
        case sf::Keyboard::Key::Left:
            selectedIndex = (selectedIndex - 1 + ITEM_COUNT) % ITEM_COUNT;
            for (int j = 0; j < ITEM_COUNT; j++) refreshCard(j);
            break;
        case sf::Keyboard::Key::Right:
            selectedIndex = (selectedIndex + 1) % ITEM_COUNT;
            for (int j = 0; j < ITEM_COUNT; j++) refreshCard(j);
            break;
        case sf::Keyboard::Key::Up:
            selectedIndex = (selectedIndex - COLS + ITEM_COUNT) % ITEM_COUNT;
            for (int j = 0; j < ITEM_COUNT; j++) refreshCard(j);
            break;
        case sf::Keyboard::Key::Down:
            selectedIndex = (selectedIndex + COLS) % ITEM_COUNT;
            for (int j = 0; j < ITEM_COUNT; j++) refreshCard(j);
            break;
        case sf::Keyboard::Key::Enter:
        case sf::Keyboard::Key::Space:
            tryBuy(selectedIndex);
            break;
        default: break;
        }
        return;
    }

    if (const auto* mb = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mb->button == sf::Mouse::Button::Left)
            handleClick({ static_cast<float>(mb->position.x),
                         static_cast<float>(mb->position.y) });
    }
}

inline void ShopScreen::handleClick(sf::Vector2f mp) {
    if (!layoutReady) return;

    if (backBtn.getGlobalBounds().contains(mp)) {
        done = true;
        return;
    }

    for (int i = 0; i < ITEM_COUNT; i++) {
        if (cards[i].getGlobalBounds().contains(mp)) {
            selectedIndex = i;
            for (int j = 0; j < ITEM_COUNT; j++) refreshCard(j);
            if (buyBtns[i].getGlobalBounds().contains(mp)) tryBuy(i);
            return;
        }
    }
}

inline void ShopScreen::draw(sf::RenderWindow& window) {
    if (!layoutReady) setupLayout();

    if (showMessage) {
        messageTimer -= 1.f / 30.f;
        if (messageTimer <= 0.f) showMessage = false;
    }

    refreshGemCount();

    // Background
    if (bgLoaded)
        window.draw(*bgSprite);
    else {
        sf::RectangleShape fb({ 800.f, 600.f });
        fb.setFillColor(sf::Color(5, 18, 35, 255));
        window.draw(fb);
    }

    window.draw(TX(txtTitle));

    window.draw(gemBarBg);
    window.draw(TX(txtGemLabel));
    window.draw(TX(txtGemCount));

    for (int i = 0; i < ITEM_COUNT; i++) {
        window.draw(cards[i]);

        if (icons[i]) window.draw(*icons[i]);
        window.draw(TX(txtItemName[i]));
        window.draw(TX(txtItemDesc[i]));
        window.draw(TX(txtItemCost[i]));
        window.draw(buyBtns[i]);
        window.draw(TX(txtBuyBtn[i]));
    }

    window.draw(backBtn);
    window.draw(TX(txtBack));

    if (showMessage) window.draw(TX(txtMessage));
}