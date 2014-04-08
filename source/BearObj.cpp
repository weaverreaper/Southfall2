//=======================================================================================
// Bear.cpp by Nate Higgins
//=======================================================================================

#include "BearObj.h"

BearObj::BearObj()
{
	health = 400;
	radius = 25;
}
 
BearObj::~BearObj()
{
	;
}
void BearObj::update(float dt, Vector3 cam, Fireball* fo, SwordObj* so)
{
	if (!getActiveState())
		return;
	if(this->collided(fo) && fo->getActiveState())
	{		health -= 10;
		fo->setInActive();
		//fo->light->on = 0;
		//fo->dist = 0;
	}
	if(this->collided(so) && !so->hit && so->theta > 0)
	{
		health -= 25;
		so->hit = true;
	}
	if(health <= 0)
	{
		setInActive();
	}
}
void BearObj::update(float dt, Vector3 cam)
{
	Vector3 direction = position - cam;
	float rot = atan2f(direction.x,direction.z)+ToRadian(90);
	D3DXVec3Normalize(&direction,&direction);
	Vector3 v = -direction*100;
	//v.y = 0;
	//roty = rot;
	//velocity = v;
	position += velocity*dt;
	Matrix temp;
	Identity(&temp);
	Identity(&world);
	position.y = getTerrHeight()+15;
	D3DXMatrixRotationYawPitchRoll(&temp, roty, rotz, rotx);
	world *= temp;
	Scale(&temp,scale,scale,scale);
	world *= temp;
	Translate(&temp, position.x, position.y, position.z);
	world *= temp;	
}
//BREAK------------------------------
//
//-----------------------------------
