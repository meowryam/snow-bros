#pragma once
#include "PlayerData.h"
#include <SFML/Graphics.hpp>
#include <string>

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
};


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
