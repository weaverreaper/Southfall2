#ifndef __ENTITY_H
#define __ENTITY_H

#include "GeoObject.h"
#include "Terrain.h"
#include "input.h"

class Entity : public GeoObject
{
public:

	Entity(){GeoObject::GeoObject();}
	~Entity() {GeoObject::~GeoObject(); }

	virtual void draw(){}
	virtual void update(float dt);
	
	void init(Geometry* geo, Terrain* terrain) {terr = terrain; GeoObject::init(geo); }

protected:
	void checkTerrain();

	Terrain* terr;
};


#endif