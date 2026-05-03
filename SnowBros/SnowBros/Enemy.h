#pragma once
#include "Entity.h"

class Enemy : public Entity
{
protected:
	int health;
	double speed; 
	bool trap; 
public:
	Enemy(double xx, double yy, int heal, double sp) // Whenever a class inherits from another and the parent has no default (zero-argument) constructor, the child must call the parent's constructor explicitly in its initializer list.( thats why used it) The compiler won't invent one for you — that's where "no default constructor exists" comes from.
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
	
// Pure virtual functions just means: "I'm declaring this function exists, but I'm not writing what it does, whoever inherits from me MUST write it."
// Because every enemy has to be drawn and updated, but how they're drawn/updated depends on the specific enemy. 
};