#pragma once
#include "Entity.h"

class Enemy : public Entity
{
private:
	int health;
	double speed;
	bool trap;
public:
	Enemy(double xx, double yy, int heal, double sp)
	{
		x = xx;
		y = yy;
		health = heal;
		speed = sp;
	}

	void healthreduce(int h)
	{
		health = health - h;
	}

	bool gettrap()
	{
		return trap;
	}

	void settrap(bool b)
	{
		trap = b;
	}

	int gethealth()
	{
		return health;
	}

	virtual void update(double deltaTime) = 0;
	virtual void draw(sf::RenderWindow& window) = 0;

};