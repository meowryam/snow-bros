#include "StarLevel.h"
#include <cstdlib>
StarEvent::StarEvent() {
    starLevel = 0;
    choice0 = "";
    choice1 = " ";;
    choice2 = " ";
}
void StarEvent::Level(int totalLevels) {
    int normalLevels[10];
    int count = 0;
    for (int i=0; i < totalLevels; i++) {
        if (i % 5 != 0) {
            normalLevels[count] = i;
			count++; 
        }
    }
    int n = rand() % count;
	starLevel = normalLevels[n];
}

bool StarEvent::isStarLevel(int levelNumber) const {
    return levelNumber == starLevel;
}
void StarEvent::generateChoices() {
    string shuffled[4];
    for (int i = 0; i < 4; i++) {
		shuffled[i] = allPowerUps[i];
    }
    for (int i = 0; i < 4; i++) {
		int j = rand() % (i+1);
        string temp = shuffled[i] ;
        shuffled[i] = shuffled[j];
        shuffled[j] = temp;
    }
	choice0 = shuffled[0];
	choice1 = shuffled[1];
	choice2 = shuffled[2];
}
string StarEvent::getChoice(int i) const {
        if (i == 0) return choice0;
		if (i == 1) return choice1;
		if (i == 2) return choice2;
                return "";
}
string StarEvent::selectChoice(int i) const {
    return getChoice(i);
}
int StarEvent::getStarLevel() const {
    return starLevel;
}