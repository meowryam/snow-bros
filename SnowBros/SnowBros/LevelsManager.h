#pragma once
#include "Levelblueprint.h"
#include <cstdlib>

class LevelsManager {
private:
	static const int totallevels = 10;
	Levelblueprint levels[totallevels];
	int current;
	void markstarlevels();
public:
	LevelsManager();

	Levelblueprint& getCurrentLevel();
	void NextLevel();	
	void SpecificLevel(int levelno);
	bool isGameDone() const;

	int gettotalelevls() const;
	int getcurrentlevelno() const;
};
