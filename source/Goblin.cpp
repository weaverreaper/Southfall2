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
