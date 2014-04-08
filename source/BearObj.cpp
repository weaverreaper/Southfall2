//=======================================================================================
// Bear.cpp by Nate Higgins
//=======================================================================================

#include "BearObj.h"

BearObj::BearObj()
{
	;
        
}
 
BearObj::~BearObj()
{
	;
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
