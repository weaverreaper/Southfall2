#include "Lighting.h"

void LightingManager::init(ID3D10EffectVariable* l, Color amb, Color dif, Color spec, Vector3 d)
{
	 lightVar = l;
	 ambientDiffuse.ambient = amb;
	 ambientDiffuse.diffuse = dif;
	 ambientDiffuse.specular = spec;
	 ambientDiffuse.dir = d;

}

void LightingManager::update(float dt)
{
}

void LightingManager::draw(float dt)
{
}