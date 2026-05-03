#pragma once
#include "PlayerData.h"
#include "EventBus.h"
using namespace std;

// Handles all scoring logic per spec section 9
class ScoreSystem {
private:
    PlayerData& playerData;
    int chainCount;  // enemies killed in one roll chain

    int randomInRange(int min, int max) {
        return rand() % (max - min + 1) + min;
    }

public:
    ScoreSystem(PlayerData& data) : playerData(data), chainCount(0) {}

    // Call these when enemies die — spec 9.1
    void onBottomKilled() {
        int pts = randomInRange(100, 500);
        pts += chainCount * pts / 10; // +10% per chain
        addScore(pts);
        chainCount++;
    }

    void onFoogaKilled() {
        int pts = randomInRange(200, 800);
        pts += chainCount * pts / 10;
        addScore(pts);
        chainCount++;
    }

    void onTornadoKilled() {
        int pts = randomInRange(300, 1200);
        pts += chainCount * pts / 10;
        addScore(pts);
        chainCount++;
    }

    void onMogeraKilled() {
        addScore(5000); // fixed — spec 9.1
        chainCount = 0;
    }

    void onGamakichiKilled() {
        addScore(10000); // fixed — spec 9.1
        chainCount = 0;
    }

    // Reset chain when player throws a new snowball
    void resetChain() { chainCount = 0; }

    // Bonus level cash bundle collected — spec 9.2 / 9.3
    void onBonusBundleCollected() {
        addScore(1000);
        playerData.setGemCount(playerData.getGemCount() + 10);
    }

    // Food item collected — awards the item's own point value
    // Called from GameLevel when getType() == CollectableType::FOOD
    void addFoodScore(int pts) {
        addScore(pts);
    }

    int getScore() const { return playerData.getHighscore(); }

private:
    void addScore(int pts) {
        int newScore = playerData.getHighscore() + pts;
        playerData.setHighscore(newScore);
    }
};