#include "Lighting.h"

void LightingManager::init()
{
	lights[0].ambient = Color(.5,.5,.5,1);
	lights[0].diffuse = Color(1.f, 1.f, 1.f, 1.f);
	lights[0].specular = Color(0,0,0,0);
	lights[0].dir = Vector3(0,-1,0);
	
	lights[1].pos = Vector3(0.f,10.f,0.f);
	lights[1].ambient = Color(.5f,.5f,.5f,1.f);	
	lights[1].diffuse = Color(1.f,0.f,0.f,1.f);	
	lights[1].specular = Color(1.f,0.f,0.f,1.f);
	lights[1].att = Vector3(0.f,1.f,0.f);
	lights[1].range = 100.f;
}
