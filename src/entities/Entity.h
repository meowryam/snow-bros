#pragma once
#include <SFML/Graphics.hpp>


class Entity
{
private:

	double x, y;
	double xspeed, yspeed;
	bool alive;

public:

	Entity(double xx, double yy)
	{
		x = xx;
		y = yy;
		xspeed = 0.0;
		yspeed = 0.0;
		alive = true;
	}

	double getx()
	{
		return x;
	}

	double gety()
	{
		return y;
	}

	double getxspeed()
	{
		return xspeed;
	}


	double getyspeed()
	{
		return yspeed;
	}

	bool getalive()
	{
		return alive;
	}

	void setalive(bool live)
	{
		alive = live;
	}

	void setxspeed(double xsp)
	{
		xspeed = xsp;
	}

	void setyspeed(double ysp)
	{
		yspeed = ysp;
	}

	virtual void update(double deltaTime) = 0;
	virtual void draw(sf::RenderWindow& window) = 0;

};