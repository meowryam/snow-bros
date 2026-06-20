#pragma once
#include <string>
#include <cstdlib>
using namespace std;
class StarEvent {
private:
    int starLevel;
    string choice0;
    string choice1;
    string choice2;
	string allPowerUps[4] = { "Speed Boost", "Snowball Power", "Distance Increase", "Balloon Mode" };
public:
    StarEvent();
    void Level(int totalLevels);
    bool isStarLevel(int levelNumber) const;
    void generateChoices();
    string getChoice(int i) const;
    string selectChoice(int i) const;
    int getStarLevel() const;
};