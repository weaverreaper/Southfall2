//=======================================================================================
// Goblin.cpp by Nate Higgins
//=======================================================================================

#include "Goblin.h"

Goblin::Goblin()
{
	;
        
}
 
Goblin::~Goblin()
{
	;
}
void Goblin::update(float dt)
{

	head.update(dt);
	body.update(dt);
}
void Goblin::update(float dt, Vector3 cam)
{
	Vector3 direction = body.getPosition() - cam;
	float rot = atan2f(direction.x,direction.z)+ToRadian(90);
	D3DXVec3Normalize(&direction,&direction);
	Vector3 v = -direction*200;
	v.y = 0;
	head.setVelocity(v);
	body.setVelocity(v);
	
	head.setroty(rot);
	body.setroty(rot);
	head.update(dt);
	body.update(dt);
}

void Goblin::init(ID3D10EffectTechnique* t, ID3D10EffectMatrixVariable* f, ID3D10EffectMatrixVariable* w, Geometry* h, Geometry* b)
{
	head.init(t,f,w,h);
	body.init(t,f,w,b);
}

void Goblin::draw(D3DXMATRIX* vp)
{
	head.draw(vp);
	body.draw(vp);
}
//BREAK------------------------------
//
//-----------------------------------
