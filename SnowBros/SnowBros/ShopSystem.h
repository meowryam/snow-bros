#pragma once
#include "PlayerData.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <cmath>
using namespace sf;

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
   
    bool buyLuckyScroll() {
        if (playerData.getGemCount() >= 40) {
            playerData.setGemCount(playerData.getGemCount() - 40);
            return true; 
        }
        return false;
    }
};


//display


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

    void handleEvent(const Event& event);
    void handleClick(Vector2f mousePos);
    void draw(RenderWindow& window);
    bool done = false;


private:
    Font font;
    static constexpr int   ITEM_COUNT = 6;
    static constexpr int   COLS = 3;
    static constexpr float CARD_W = 118.f;
    static constexpr float CARD_H = 155.f;
    static constexpr float CARD_GAP_X = 18.f;
    static constexpr float CARD_GAP_Y = 16.f;
    static constexpr float GRID_LEFT = 204.f;
    static constexpr float GRID_TOP = 152.f;
    static constexpr float ICON_SIZE = 40.f;
    static constexpr float MSG_SECS = 2.5f;

    struct ItemDef {
        const char* name;
        const char* desc;
        int cost;
        bool oneTime;
        int sx, sy, sw, sh; //items.png
    };


    static constexpr ItemDef ITEMS[ITEM_COUNT] = {{"Extra Life","+1 life now", 50, false, 320, 647, 131, 155 },{ "Speed Boost", "1.5x speed, 30s",20, false, 348, 27, 97, 141 },{"Snowball Pwr","1-hit encase", 30, true,348,185, 97,139 },{ "Distance+", "Full-screen throw",25, true,348, 342, 97, 141 },{ "Balloon Mode", "Float+immune, 30s", 35, false, 348, 500,  97, 141 },{ "Lucky Scroll", "Random power-up!",40, false,17, 974, 256, 345 }, };
    ShopSystem& shop;
    bool layoutReady = false;
    int selectedIndex = 0;
    bool purchased[ITEM_COUNT] = {};
    bool showMessage = false;
    float messageTimer = 0.f;
    bool messageSuccess = false;

    Texture bgTex;
    bool bgLoaded = false;
    optional<Sprite> bgSprite;

    Texture itemsTex;
    bool itemsTexLoaded = false;
    Sprite* icons[ITEM_COUNT] = {};

    RectangleShape cards[ITEM_COUNT];
    RectangleShape buyBtns[ITEM_COUNT];
    RectangleShape gemBarBg;
    RectangleShape backBtn;

    optional<Text> txtTitle;
    optional<Text> txtGemLabel;
    optional<Text> txtGemCount;
    optional<Text> txtItemName[ITEM_COUNT];
    optional<Text> txtItemDesc[ITEM_COUNT];
    optional<Text> txtItemCost[ITEM_COUNT];
    optional<Text> txtBuyBtn[ITEM_COUNT];
    optional<Text> txtMessage;
    optional<Text> txtBack;

 
    const Color CARD_NORMAL{ 6, 18, 40, 185 };     //dark icy blue
    const Color CARD_SEL{ 20, 55, 110, 215 };      //brighter ice glow
    const Color CARD_BOUGHT{ 20, 95, 120, 235 }; //deeper, richer ice
    const  Color CARD_POOR{ 20, 35, 60, 200 }; //visible but muted

    const Color BTN_BUY{ 70, 170, 200, 235 };      //icy cyan
    const Color BTN_OFF{ 40, 60, 90, 180 };
    const Color BTN_OWN{ 60, 180, 170, 255 };
    const Color COL_BTN_OWN_TXT{ 230, 255, 250, 255 };

    const Color COL_NAME_NORMAL{ 190, 230, 255, 255 };   //ice white-blue
    const Color COL_NAME_SEL{ 220, 245, 255, 255 };      //brighter frost
    const Color COL_NAME_POOR{ 160, 180, 200, 220 };
    const Color COL_DESC_POOR{ 130, 150, 170, 200 };
    const Color COL_COST_POOR{ 100, 130, 150, 200 };
    const Color COL_DESC_NORMAL{ 160, 200, 235, 210 };   //soft ice blue


    const Color COL_COST{ 120, 220, 255, 255 };          //glowing cyan

    const Color COL_BTN_TXT{ 240, 250, 255, 255 };       //frosty white
    const Color COL_BTN_OFF_TXT{ 120, 140, 160, 180 };
   
   
    const Color OUTLINE_SEL{ 120, 220, 255, 255 };       //cyan glow
    const Color OUTLINE_NRM{ 40, 90, 140, 160 };
    const Color OUTLINE_POOR{ 25, 50, 80, 100 };
    const Color OUTLINE_OWNED{ 150, 255, 240, 255 }; //crisp icy glow
    const Color COL_NAME_OWNED{ 220, 255, 245, 255 };
    const Color COL_DESC_OWNED{ 180, 235, 225, 220 };
   

    const Color GEM_COL{ 120, 220, 255, 255 };           // matches crystals
    const Color GREEN_OK{ 100, 255, 200, 255 };          // icy success
    const Color RED_ERR{ 220, 80, 80, 255 };             // keep readable

    
    Text& TX(optional<Text>& t) { return t.value(); }
    Vector2f cardTopLeft(int i) const;
    void setupLayout();
    void refreshCard(int i);
    void refreshGemCount();
    void tryBuy(int i);
    void postMessage(const string& msg, bool success);
};






inline sf::Vector2f ShopScreen::cardTopLeft(int i) const {
    int col = i % COLS;
    int row = i / COLS;
    return {GRID_LEFT + col * (CARD_W + CARD_GAP_X),GRID_TOP + row * (CARD_H + CARD_GAP_Y) };
}


inline bool ShopScreen::loadFont(const string& path) {

    if (!font.openFromFile(path)) return false;

    bgLoaded = bgTex.loadFromFile("assets\\images\\Shop_bg.png");
    if (bgLoaded) {
        bgSprite.emplace(bgTex);
        Vector2u ts = bgTex.getSize();
        bgSprite->setScale({800.f / static_cast<float>(ts.x), 600.f / static_cast<float>(ts.y)});
    }


    itemsTexLoaded = itemsTex.loadFromFile("assets\\images\\Items.png");
    for (int i = 0; i < ITEM_COUNT; i++) {
        if (itemsTexLoaded) {
            icons[i] = new Sprite(itemsTex);
            icons[i]->setTextureRect(
            IntRect({ ITEMS[i].sx, ITEMS[i].sy }, { ITEMS[i].sw, ITEMS[i].sh }));
            icons[i]->setScale({ ICON_SIZE / static_cast<float>(ITEMS[i].sw), ICON_SIZE / static_cast<float>(ITEMS[i].sh) });
        }
    }

   
    txtTitle.emplace(font, "SNOW  SHOP", 20u);
    txtGemLabel.emplace(font, "GEMS:", 8u);
    txtGemCount.emplace(font, "0", 11u);
    txtMessage.emplace(font, "", 8u);
    txtBack.emplace(font, "BACK  [ESC]", 8u);

    for (int i = 0; i < ITEM_COUNT; i++) {
        txtItemName[i].emplace(font, ITEMS[i].name, 11u);
        txtItemDesc[i].emplace(font, ITEMS[i].desc, 9u);
        txtItemCost[i].emplace(font, "", 9u);
        txtBuyBtn[i].emplace(font, "BUY", 11u);
    }
    return true;
}

inline void ShopScreen::setupLayout() {

    
    TX(txtTitle).setFillColor(Color(200, 235, 255, 255));
    TX(txtTitle).setStyle(Text::Bold);
    {
        auto lb = TX(txtTitle).getLocalBounds();
        TX(txtTitle).setPosition({ 401.f - lb.size.x * 0.5f, 80.f });
    }

    gemBarBg.setSize({ 148.f, 30.f });
    gemBarBg.setPosition({ 638.f, 16.f });
    gemBarBg.setFillColor(Color(6, 20, 40, 190));
    gemBarBg.setOutlineColor(GEM_COL);
    gemBarBg.setOutlineThickness(1.5f);
    TX(txtGemLabel).setFillColor(GEM_COL);
    TX(txtGemLabel).setPosition({ 648.f, 22.f });

    backBtn.setSize({ 140.f, 32.f });
    backBtn.setPosition({ 330.f, 558.f });
    backBtn.setFillColor(Color(10, 30, 60, 215));
    backBtn.setOutlineColor(Color(120, 220, 255, 210));
    backBtn.setOutlineThickness(1.5f);
    TX(txtBack).setFillColor(Color(220, 245, 255, 255));
    {
        auto lb = TX(txtBack).getLocalBounds();
        TX(txtBack).setPosition({330.f + (140.f - lb.size.x) * 0.5f,558.f + (32.f - lb.size.y) * 0.5f - 2.f });
    }

    
    TX(txtMessage).setPosition({ 400.f, 526.f });
    for (int i = 0; i < ITEM_COUNT; i++) {
        Vector2f cp = cardTopLeft(i);
        cards[i].setSize({ CARD_W, CARD_H });
        cards[i].setPosition(cp);
        cards[i].setOutlineThickness(1.5f);

        if (icons[i])
            icons[i]->setPosition({cp.x + (CARD_W - ICON_SIZE) * 0.5f,cp.y + 7.f});

//txt layout inside card
        float tx = cp.x + 6.f;
        float btnY = cp.y + CARD_H - 30.f;
        float nameY = btnY - 52.f;

        TX(txtItemName[i]).setPosition({ tx, nameY });
        TX(txtItemDesc[i]).setPosition({ tx, nameY + 18.f });
        TX(txtItemCost[i]).setString(to_string(ITEMS[i].cost) + " gems");
        TX(txtItemCost[i]).setPosition({ tx, nameY + 34.f });

        
        buyBtns[i].setSize({ CARD_W - 12.f, 24.f });
        buyBtns[i].setPosition({ cp.x + 6.f, btnY });
        buyBtns[i].setOutlineThickness(1.f);

        {
            auto lb = TX(txtBuyBtn[i]).getLocalBounds();
            TX(txtBuyBtn[i]).setPosition({ cp.x + 6.f + (CARD_W - 12.f - lb.size.x) * 0.5f, btnY + (24.f - lb.size.y) * 0.5f - 2.f});
        }

        refreshCard(i);
    }

    layoutReady = true;
}

inline void ShopScreen::refreshGemCount() {
    TX(txtGemCount).setString(to_string(shop.totalGems()));
    TX(txtGemCount).setFillColor(Color::White);
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
        buyBtns[i].setOutlineColor(Color::Transparent);
        TX(txtBuyBtn[i]).setString("OWNED");
        TX(txtBuyBtn[i]).setFillColor(COL_BTN_OWN_TXT);
        TX(txtItemName[i]).setFillColor(COL_NAME_OWNED);
        TX(txtItemDesc[i]).setFillColor(COL_DESC_OWNED);
        TX(txtItemCost[i]).setFillColor(Color(60, 130, 80, 180));
        cards[i].setOutlineThickness(sel ? 2.8f : 2.2f);
    }
    else if (!canAfford) {
        cards[i].setFillColor(CARD_POOR);
        cards[i].setOutlineColor(sel ? OUTLINE_SEL : OUTLINE_POOR);
        buyBtns[i].setFillColor(BTN_OFF);
        buyBtns[i].setOutlineColor(Color::Transparent);
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
        buyBtns[i].setOutlineColor(Color(75, 175, 95, 185));
        TX(txtBuyBtn[i]).setString("BUY");
        TX(txtBuyBtn[i]).setFillColor(COL_BTN_TXT);
        TX(txtItemName[i]).setFillColor(sel ? COL_NAME_SEL : COL_NAME_NORMAL);
        TX(txtItemDesc[i]).setFillColor(COL_DESC_NORMAL);
        TX(txtItemCost[i]).setFillColor(COL_COST);
    }

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
    case 0: ok = shop.buyExtraLife(); break;
    case 1: ok = shop.buySpeedBoost(); break;
    case 2: ok = shop.buySnowBallPower(); break;
    case 3: ok = shop.buyDistanceIncrease(); break;
    case 4: ok = shop.buyBalloonMode(); break;
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

inline void ShopScreen::handleEvent(const Event& event) {
    if (!layoutReady) return;

    if (const auto* kp = event.getIf<Event::KeyPressed>()) {
        switch (kp->code) {
        case Keyboard::Key::Escape:
            done = true;
            break;
        case Keyboard::Key::Left:
            selectedIndex = (selectedIndex - 1 + ITEM_COUNT) % ITEM_COUNT;
            for (int j = 0; j < ITEM_COUNT; j++) refreshCard(j);
            break;
        case Keyboard::Key::Right:
            selectedIndex = (selectedIndex + 1) % ITEM_COUNT;
            for (int j = 0; j < ITEM_COUNT; j++) refreshCard(j);
            break;
        case  Keyboard::Key::Up:
            selectedIndex = (selectedIndex - COLS + ITEM_COUNT) % ITEM_COUNT;
            for (int j = 0; j < ITEM_COUNT; j++) refreshCard(j);
            break;
        case Keyboard::Key::Down:
            selectedIndex = (selectedIndex + COLS) % ITEM_COUNT;
            for (int j = 0; j < ITEM_COUNT; j++) refreshCard(j);
            break;
        case Keyboard::Key::Enter:
        case Keyboard::Key::Space:
            tryBuy(selectedIndex);
            break;
        default: break;
        }
        return;
    }

    if (const auto* mb = event.getIf<Event::MouseButtonPressed>()) {
        if (mb->button == Mouse::Button::Left)
            handleClick({ static_cast<float>(mb->position.x),
                         static_cast<float>(mb->position.y) });
    }
}

inline void ShopScreen::handleClick(Vector2f mp) {
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



inline void ShopScreen::draw(RenderWindow& window) {
    if (!layoutReady) setupLayout();

    if (showMessage) {
        messageTimer -= 1.f / 30.f;
        if (messageTimer <= 0.f) showMessage = false;
    }

    refreshGemCount();
    if (bgLoaded)
        window.draw(*bgSprite);
    else {
        RectangleShape fb({ 800.f, 600.f });
        fb.setFillColor(Color(5, 18, 35, 255));
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