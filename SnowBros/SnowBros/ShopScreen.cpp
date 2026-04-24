#include "ShopSystem.h"

// ── loadFont ─────────────────────────────────────────────────────────────────
// FIX: sf::Text requires a font at construction in SFML 3.
// We create every sf::Text here, after the font is loaded.

bool ShopScreen::loadFont(const string& path) {
    if (!font.openFromFile(path))
        return false;

    // Construct all Text objects now that font is ready
    titleText.emplace(font);
    gemText.emplace(font);
    gemLabel.emplace(font);
    messageText.emplace(font);

    for (int i = 0; i < 5; ++i) {
        itemNames[i].emplace(font);
        itemDescs[i].emplace(font);
        itemCosts[i].emplace(font);
        buyBtnTexts[i].emplace(font);
    }
    return true;
}

// ── setupLayout ──────────────────────────────────────────────────────────────

void ShopScreen::setupLayout(sf::RenderWindow& window) {
    float W = static_cast<float>(window.getSize().x);
    float H = static_cast<float>(window.getSize().y);

    // background
    background.setSize({ W, H });
    background.setFillColor(bgColor);

    // centre panel
    const float panelW = 520.f, panelH = 520.f;
    float panelX = (W - panelW) / 2.f;
    float panelY = (H - panelH) / 2.f;
    panel.setSize({ panelW, panelH });
    panel.setPosition({ panelX, panelY });
    panel.setFillColor(panelColor);
    panel.setOutlineThickness(1.f);
    panel.setOutlineColor(sf::Color(255, 229, 102, 60));

    // title
    T(titleText).setString("* SHOP *");
    T(titleText).setCharacterSize(32);
    T(titleText).setFillColor(gold);
    T(titleText).setLetterSpacing(3.f);
    {
        sf::FloatRect tb = T(titleText).getLocalBounds();
        T(titleText).setOrigin({ tb.size.x / 2.f, 0.f });
    }
    T(titleText).setPosition({ W / 2.f, panelY + 18.f });

    // gem bar
    float barY = panelY + 68.f;
    gemBar.setSize({ panelW - 40.f, 42.f });
    gemBar.setPosition({ panelX + 20.f, barY });
    gemBar.setFillColor(sf::Color(255, 255, 255, 15));
    gemBar.setOutlineThickness(1.f);
    gemBar.setOutlineColor(sf::Color(255, 229, 102, 50));

    T(gemLabel).setString("YOUR GEMS");
    T(gemLabel).setCharacterSize(11);
    T(gemLabel).setFillColor(mutedPurple);
    T(gemLabel).setLetterSpacing(2.f);
    T(gemLabel).setPosition({ panelX + 36.f, barY + 13.f });

    T(gemText).setCharacterSize(20);
    T(gemText).setFillColor(gemBlue);

    // item boxes (2×2 + 1 centred)
    const float boxW = 228.f, boxH = 115.f, gapX = 16.f, gapY = 12.f;
    float startX = panelX + 18.f;
    float startY = barY + 56.f;

    float posX[5] = {
        startX,              startX + boxW + gapX,
        startX,              startX + boxW + gapX,
        panelX + (panelW - boxW) / 2.f
    };
    float posY[5] = {
        startY,              startY,
        startY + boxH + gapY, startY + boxH + gapY,
        startY + (boxH + gapY) * 2.f
    };

    for (int i = 0; i < 5; ++i) {
        itemBoxes[i].setSize({ boxW, boxH });
        itemBoxes[i].setPosition({ posX[i], posY[i] });
        itemBoxes[i].setOutlineThickness(1.f);

        T(itemNames[i]).setString(items[i].name);
        T(itemNames[i]).setCharacterSize(13);
        T(itemNames[i]).setFillColor(lightText);
        T(itemNames[i]).setPosition({ posX[i] + 12.f, posY[i] + 10.f });

        T(itemDescs[i]).setString(items[i].desc);
        T(itemDescs[i]).setCharacterSize(10);
        T(itemDescs[i]).setFillColor(mutedPurple);
        T(itemDescs[i]).setPosition({ posX[i] + 12.f, posY[i] + 32.f });

        T(itemCosts[i]).setString(to_string(items[i].cost) + " gems");
        T(itemCosts[i]).setCharacterSize(12);
        T(itemCosts[i]).setFillColor(gemBlue);
        T(itemCosts[i]).setPosition({ posX[i] + 12.f, posY[i] + 56.f });

        const float btnW = 64.f, btnH = 24.f;
        buyButtons[i].setSize({ btnW, btnH });
        buyButtons[i].setPosition({
            posX[i] + boxW - btnW - 10.f,
            posY[i] + boxH - btnH - 10.f
            });
        buyButtons[i].setFillColor(sf::Color(255, 229, 102, 38));
        buyButtons[i].setOutlineThickness(1.f);
        buyButtons[i].setOutlineColor(sf::Color(255, 229, 102, 100));

        T(buyBtnTexts[i]).setCharacterSize(10);
        T(buyBtnTexts[i]).setLetterSpacing(1.5f);
        // Initial centering — will be updated every frame in draw()
    }

    // message text
    T(messageText).setCharacterSize(11);
    T(messageText).setLetterSpacing(1.5f);
    T(messageText).setPosition({ W / 2.f, panelY + panelH - 28.f });

    layoutReady = true;
}

// ── showMsg ───────────────────────────────────────────────────────────────────

void ShopScreen::showMsg(const string& text, bool success) {
    T(messageText).setString(text);
    T(messageText).setFillColor(success ? greenOk : redErr);
    sf::FloatRect mb = T(messageText).getLocalBounds();
    T(messageText).setOrigin({ mb.size.x / 2.f, 0.f });
    showMessage = true;
    messageClock.restart();
}

// ── draw ──────────────────────────────────────────────────────────────────────

void ShopScreen::draw(sf::RenderWindow& window) {
    if (!layoutReady)
        setupLayout(window);

    // update gem display
    T(gemText).setString(to_string(shop.totalGems()));
    {
        sf::FloatRect gb = T(gemText).getLocalBounds();
        float barRight = gemBar.getPosition().x + gemBar.getSize().x - 16.f;
        T(gemText).setPosition({ barRight - gb.size.x, gemBar.getPosition().y + 10.f });
    }

    // update item boxes
    for (int i = 0; i < 5; ++i) {
        bool bought = purchased[i] && items[i].oneTime;
        bool canAfford = shop.totalGems() >= items[i].cost;

        if (bought) {
            itemBoxes[i].setFillColor(boxPurchased);
            itemBoxes[i].setOutlineColor(sf::Color(76, 175, 125, 120));
            T(buyBtnTexts[i]).setString("OWNED");
            T(buyBtnTexts[i]).setFillColor(greenOk);
            buyButtons[i].setFillColor(sf::Color(76, 175, 125, 40));
            buyButtons[i].setOutlineColor(sf::Color(76, 175, 125, 120));
        }
        else if (!canAfford) {
            itemBoxes[i].setFillColor(boxCantAfford);
            itemBoxes[i].setOutlineColor(sf::Color(255, 255, 255, 25));
            T(buyBtnTexts[i]).setString("BUY");
            T(buyBtnTexts[i]).setFillColor(sf::Color(255, 229, 102, 80));
            buyButtons[i].setFillColor(sf::Color(255, 229, 102, 15));
            buyButtons[i].setOutlineColor(sf::Color(255, 229, 102, 40));
        }
        else {
            itemBoxes[i].setFillColor(boxAffordable);
            itemBoxes[i].setOutlineColor(sf::Color(91, 191, 255, 80));
            T(buyBtnTexts[i]).setString("BUY");
            T(buyBtnTexts[i]).setFillColor(gold);
            buyButtons[i].setFillColor(sf::Color(255, 229, 102, 38));
            buyButtons[i].setOutlineColor(sf::Color(255, 229, 102, 100));
        }

        // recentre button label
        sf::FloatRect  bb = T(buyBtnTexts[i]).getLocalBounds();
        sf::Vector2f   btnPos = buyButtons[i].getPosition();
        sf::Vector2f   btnSize = buyButtons[i].getSize();
        T(buyBtnTexts[i]).setPosition({
            btnPos.x + (btnSize.x - bb.size.x) / 2.f,
            btnPos.y + (btnSize.y - bb.size.y) / 2.f - 2.f
            });
    }

    if (showMessage && messageClock.getElapsedTime().asSeconds() > 2.f)
        showMessage = false;

    // render
    window.draw(background);
    window.draw(panel);
    window.draw(T(titleText));
    window.draw(gemBar);
    window.draw(T(gemLabel));
    window.draw(T(gemText));

    for (int i = 0; i < 5; ++i) {
        window.draw(itemBoxes[i]);
        window.draw(T(itemNames[i]));
        window.draw(T(itemDescs[i]));
        window.draw(T(itemCosts[i]));
        window.draw(buyButtons[i]);
        window.draw(T(buyBtnTexts[i]));
    }

    if (showMessage)
        window.draw(T(messageText));
}

// ── handleClick ───────────────────────────────────────────────────────────────

void ShopScreen::handleClick(sf::Vector2f mousePos) {
    for (int i = 0; i < 5; ++i) {
        if (!buyButtons[i].getGlobalBounds().contains(mousePos))
            continue;

        bool bought = purchased[i] && items[i].oneTime;
        bool canAfford = shop.totalGems() >= items[i].cost;

        if (bought) { showMsg("already purchased!", false); return; }
        if (!canAfford) { showMsg("not enough gems!", false); return; }

        bool success = false;
        switch (i) {
        case 0: success = shop.buyExtraLife();        break;
        case 1: success = shop.buySpeedBoost();       break;
        case 2: success = shop.buySnowBallPower();    break;
        case 3: success = shop.buyDistanceIncrease(); break;
        case 4: success = shop.buyBalloonMode();      break;
        }

        if (success) {
            if (items[i].oneTime) purchased[i] = true;
            showMsg("purchased " + items[i].name + "!", true);
        }
        else {
            showMsg("purchase failed!", false);
        }
        return;
    }
}