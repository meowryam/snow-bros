#pragma once
#include "Entity.h"

class PowerUp : public Entity
{
private:
	double duration;
	bool active;
public:
	PowerUp(float x, float y, float duration)
		: Entity(x, y), duration(duration), active(false) {
	}

	virtual void update(float deltaTime) = 0;
	virtual void draw(sf::RenderWindow& window) = 0;
	virtual void apply() = 0;

};
