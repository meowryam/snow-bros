#pragma once
#include "Entity.h"

class Enemy : public Entity
{
private:
	int health;
	double speed;
	bool trap;
public:
	Enemy(double xx, double yy, int heal, double sp) //Whenever a class inherits from another and the parent has no default (zero-argument) constructor, the child must call the parent's constructor explicitly in its initializer list.( thats why used it) The compiler won't invent one for you — that's where "no default constructor exists" comes from.
		: Entity(xx, yy), health(heal), speed(sp), trap(false) 
	{
		
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