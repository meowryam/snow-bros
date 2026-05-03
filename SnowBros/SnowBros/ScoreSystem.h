#pragma once
#include "PlayerData.h"
#include "EventBus.h"
using namespace std;

class ScoreSystem {
private:
    PlayerData& playerData;
    int chainCount; 

    int randomInRange(int min, int max) {
        return rand() % (max - min + 1) + min;
    }
    void addScore(int pts) {
        int newScore = playerData.getHighscore() + pts;
        playerData.setHighscore(newScore);
    }
public:
    ScoreSystem(PlayerData& data) : playerData(data), chainCount(0) {}

    void onBottomKilled() {
        int pts = randomInRange(100, 500);
        pts += chainCount * pts / 10;
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
        addScore(5000); 
        chainCount = 0;
    }
    void onGamakichiKilled() {
        addScore(10000); 
        chainCount = 0;
    }
    void resetChain() { chainCount = 0; }
    void onBonusBundleCollected() {
        addScore(1000);
        playerData.setGemCount(playerData.getGemCount() + 10);
    }
    void addFoodScore(int pts) { addScore(pts); }
    int getScore() const { return playerData.getHighscore(); }

};