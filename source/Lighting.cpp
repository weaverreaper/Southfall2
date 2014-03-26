#include "Lighting.h"

void LightingManager::init(ID3D10EffectVariable* l)
{
	 lightVar = l;

	 ambientDiffuse.ambient = DARKBROWN;
	 ambientDiffuse.diffuse = DARKBROWN;
	 ambientDiffuse.dir = Vector3(-1.f, -1.f, -1.f);	
	 ambientDiffuse.specular = Color(1.f, 1.f, 1.f, 8.f);
}

void LightingManager::update(float dt)
{
}

void LightingManager::draw(float dt)
{
}