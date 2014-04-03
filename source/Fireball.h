#ifndef __FIREBALL_H
#define __FIREBALL_H

#include "GeoObject.h"
#include "Lighting.h"

const float MAX_DIST =  1000.f;
const float FIREBALL_SPEED = 1000.f;

class Fireball : public GeoObject
{
public:
	Fireball() {dist = 0;}
	virtual void update(float dt);
	void setLight(Light* l){ light = l; }
	void shoot(Vector3 pos, Vector3 dir);

private:
	Light* light;
	float dist;

};
#endif;