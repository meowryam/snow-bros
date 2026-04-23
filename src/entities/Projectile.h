#pragma once
#include "Entity.h"

class Projectile : public Entity
{
private:
	int damage;
public:
	Projectile(double xx, double yy, int dam)
	{
		x = xx;
		y = yy;
		damage = dam;
	}

	int getdamage()
	{
		return damage;
	}

	virtual void update(double deltaTime) = 0;
	virtual void draw(sf::RenderWindow& window) = 0;

};