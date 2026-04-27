#pragma once
#include <string>
#include <iostream>
using namespace std;

class PlayerData {
private:
	    string username;
		int currentlevel;
		int lives;
		int gemCount;
		int highscore;
public:
	PlayerData() {
		username = "Maryam";
		currentlevel = 1;
		lives = 2;
		gemCount = 500;
		highscore = 0;
	}
	PlayerData(string username, int currentlevel, int lives, int gemCount, int highscore) {
		this->username = username;
		this->currentlevel = currentlevel;
		this->lives = lives;
		this->gemCount = gemCount;
		this->highscore = highscore;
	}
	PlayerData(const PlayerData& other) {
		username = other.username;
		currentlevel = other.currentlevel;
		lives = other.lives;
		gemCount = other.gemCount;
		highscore = other.highscore;
	}
	void setUsername(string username) {
		this->username = username;
	}
	void setCurrentLevel(int currentlevel) {
		this->currentlevel = currentlevel;
	}
	void setLives(int lives) {
		this->lives = lives;
	}
	void setGemCount(int gemCount) {
		this->gemCount = gemCount;
	}
	void setHighscore(int highscore) {
		this->highscore = highscore;
	}
	string getUsername() const {
		return username;
	}
	int getCurrentLevel() const {
		return currentlevel;
	}
	int getLives() const {
		return lives;
	}
	int getGemCount() const {
		return gemCount;
	}
	int getHighscore() const {
		return highscore;
	}
};