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

void Goblin::init(ID3D10EffectTechnique* t, ID3D10EffectMatrixVariable* f, ID3D10EffectMatrixVariable* w, Geometry* h, Geometry* b)
{
	head.init(t,f,w,h);
	body.init(t,f,w,h);
}

void Goblin::draw(D3DXMATRIX* vp)
{
	head.draw(vp);
	body.draw(vp);
}
//BREAK------------------------------
//
//-----------------------------------
