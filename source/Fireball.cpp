#include "Fireball.h"

void Fireball::update(float dt)
{
	if (!active) return;

	light->pos += velocity * dt;
	dist += FIREBALL_SPEED * dt;
	if (dist > MAX_DIST)
	{
		light->on = 0;
		dist = 0;
		setInActive();
	}
	
	GeoObject::update(dt);
}

void Fireball::shoot(Vector3 pos, Vector3 dir)
{
	setVelocity(dir*FIREBALL_SPEED);
	setPosition(pos + 50*dir);
	setActive();

	light->dir = dir;
	light->pos = position;
	light->on = 1;
}