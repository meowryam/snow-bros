#pragma once
#include<string>
using namespace std;

class Levelblueprint {
private:
	int levelno;
	bool bosslevel;
	bool starlevel;
	int bottomcount;
	int foogacount;
	int tornadocount;
	float enemyspeed_increaser;
	string bgtheme;
	string music;
public:
	Levelblueprint();
	Levelblueprint(int levelno, bool bosslevel, bool starlevel, int bottomcount, int foogacount, int tornadocount, float enemyspeed_increaser, string bgtheme, string music);

	void setStarLevel(bool starlevel);

	int getLevelno() const;
	bool isBosslevel() const;
	bool isStarlevel() const;
	int getBottomcount() const;
	int getFoogacount() const;
	int getTornadocount() const;
	float getEnemyspeedIncreaser() const;
	string getBgtheme() const;
	string getMusic() const;


};