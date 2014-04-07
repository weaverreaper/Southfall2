//=======================================================================================
// Box.h by Frank Luna (C) 2008 All Rights Reserved.
//=======================================================================================

#ifndef GOBLIN_H
#define GOBLIN_H

//#include "C:\Users\weaverem1\Dropbox\S14\Games2\PartII\Common\d3dUtil.h"
#include "d3dUtil.h"
#include "GeoObject.h"

class Goblin
{
public:

	Goblin();
	~Goblin();
	void init(ID3D10EffectTechnique* t, ID3D10EffectMatrixVariable* f, ID3D10EffectMatrixVariable* w, Geometry* h, Geometry* b);
	void draw(D3DXMATRIX* vp);
	void setPosition(D3DXVECTOR3 pos){head.setPosition(pos); body.setPosition(pos);}
	void setScale(float scale){head.setScale(scale);body.setScale(scale);}
	void update(float dt){head.update(dt);body.update(dt);}
	void setMFX(ID3D10Effect* fx) { head.setMFX(fx); }

private:

	GeoObject head, body;
	
};


#endif // BOX_H
