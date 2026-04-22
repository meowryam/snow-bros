#pragma once
#include "PlayerData.h"

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