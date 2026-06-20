#include "Levelblueprint.h"


Levelblueprint::Levelblueprint() {
	levelno = 0;
	bosslevel = false;
	starlevel = false;
	bottomcount = 0;
	foogacount = 0;
	tornadocount = 0;
	enemyspeed_increaser = 1.0f;
	bgtheme = "";
	music = "";
}

Levelblueprint::Levelblueprint(int levelno, bool bosslevel, bool starlevel, int bottomcount, int foogacount, int tornadocount, float enemyspeed_increaser, string bgtheme, string music) {
	this->levelno = levelno;
	this->bosslevel = bosslevel;
	this->starlevel = starlevel;
	this->bottomcount = bottomcount;
	this->foogacount = foogacount;
	this->tornadocount = tornadocount;
	this->enemyspeed_increaser = enemyspeed_increaser;
	this->bgtheme = bgtheme;
	this->music = music;
}

void Levelblueprint::setStarLevel(bool starlevel) {
	this->starlevel = starlevel;
}

int Levelblueprint::getLevelno() const {
	return levelno;
}
bool Levelblueprint::isBosslevel() const {
	return bosslevel;
}
bool Levelblueprint::isStarlevel() const {
	return starlevel;
}
int Levelblueprint::getBottomcount() const {
	return bottomcount;
}
int Levelblueprint::getFoogacount() const {
	return foogacount;
}
int Levelblueprint::getTornadocount() const {
	return tornadocount;
}
float Levelblueprint::getEnemyspeedIncreaser() const {
	return enemyspeed_increaser;
}
string Levelblueprint::getBgtheme() const {
	return bgtheme;
}
string Levelblueprint::getMusic() const {
	return music;
}