#include "LevelsManager.h"
#include <iostream>
void LevelsManager::markstarlevels() {

	int normallevels[8] = { 0,1,2,3,5,6,7,8 };

	int starlevel1 = normallevels[rand() % 8];
	int starlevel2;
	do {
		starlevel2 = normallevels[rand() % 8];
	} while (starlevel2 == starlevel1);


	levels[starlevel1].setStarLevel(true);
	levels[starlevel2].setStarLevel(true);
}

LevelsManager::LevelsManager() {
	
	current = 0;

	// levelno, bosslevel, starlevel, bottomcount, foogacount, tornadocount, enemyspeed_increaser, bgtheme, music
	levels[0] = Levelblueprint(1, false, false, 1, 0, 0, 1.0f, "Snowy Village", "Standard");
	levels[1] = Levelblueprint(2, false, false, 2, 1, 0, 1.1f, "Ice Caves", "Standard");
	levels[2] = Levelblueprint(3, false, false,1, 1, 1, 1.2f, "Winter Forest", "Standard");
	levels[3] = Levelblueprint(4, false, false,2, 1, 1, 1.3f, "Frozen Lake", "Standard");
	levels[4] = Levelblueprint(5, true, false, 0, 0, 0, 1.4f,"Dark unground layer with dramatic lightning", "Bosslevel");
	levels[5] = Levelblueprint(6, false, false, 2, 2, 0, 1.5f, "Volcanic Wasteland", "Standard");
	levels[6] = Levelblueprint(7, false, false, 1, 2, 1,1.0f, "Haunted Castle", "Standard");
	levels[7] = Levelblueprint(8, false, false, 3, 1, 1,1.1f, "Alien Planet", "Standard");
	levels[8] = Levelblueprint(9, false, false, 2, 1, 1,1.2f, "Storm Clouds", "Standard");
	levels[9] = Levelblueprint(10, true, false, 0, 0, 0, 1.3f,"Final Arena", "Bosslevel");
	markstarlevels();
}
Levelblueprint& LevelsManager::getCurrentLevel() {
	return levels[current];
}

void LevelsManager::NextLevel() {
	if (current < totallevels) {
		current++;
	}
}

void LevelsManager::SpecificLevel(int levelno) {
	if (levelno >= 1 && levelno <= totallevels) {
		current = levelno - 1;
	}
}

bool LevelsManager::isGameDone() const {
	return current >= totallevels;
}

int LevelsManager::gettotalelevls() const{
	return totallevels;
}
int LevelsManager::getcurrentlevelno() const {
	return levels[current].getLevelno();
}
