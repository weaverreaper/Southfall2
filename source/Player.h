#ifndef __PLAYER_H
#define __PLAYER_H

#include "Entity.h"
#include "input.h"

class Player : public Entity
{
public:

	virtual void draw(){}
	virtual void update(float dt);

	void init(Geometry* geo, Terrain* terrain)
	{terr = terrain; Entity::init(geo, terr); }

	void shootFireball(){}

private:
	Input* input;
	Vector3* target;
};

#endif