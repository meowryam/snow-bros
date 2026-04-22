#pragma once
#include <string>
#include <iostream>
#include "PlayerData.h"
#include <cstdlib>

class GemSystem {
private:
	PlayerData& playerData; 
	int randomGemdrop(int min, int max) {
		return rand() % (max - min + 1) + min;
	}
public:
	GemSystem(PlayerData& playerData) : playerData(playerData) {}
	void enemykilled() {
		playerData.setGemCount(playerData.getGemCount() + randomGemdrop(1, 5));
	}
	void Mogerakilled() {
		playerData.setGemCount(playerData.getGemCount() + 200);
	}
	void gamakichikilled() {
		playerData.setGemCount(playerData.getGemCount() + 500);
	}
	void addGems(int count) //bonus level
	{
		playerData.setGemCount(playerData.getGemCount() + count);
	}
	
	int totalGems() const {
		return playerData.getGemCount();
	}
	
};