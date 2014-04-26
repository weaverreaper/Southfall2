//=======================================================================================
// Goblin.h by Nate Higgins
//=======================================================================================

#ifndef GOBLIN_H
#define GOBLIN_H

//#include "C:\Users\weaverem1\Dropbox\S14\Games2\PartII\Common\d3dUtil.h"
#include "d3dUtil.h"
#include "GeoObject.h"
#include "Fireball.h"
#include "SwordObj.h"
#include <vector>
#include "DamageSprites.h"

class Goblin
{
public:

	Goblin();
	~Goblin();
	void init(ID3D10EffectTechnique* t, ID3D10EffectMatrixVariable* f, ID3D10EffectMatrixVariable* w, ID3D10Device* device, Geometry* h, Geometry* b, Terrain* ter);
	void draw(D3DXMATRIX* vp);
	void setPosition(D3DXVECTOR3 pos){head.setPosition(pos); body.setPosition(pos);}
	void setScale(float scale){head.setScale(scale);body.setScale(scale);}
	void update(float dt);
	void update(float dt, Vector3 cam, Fireball* fo, SwordObj* so);
	void setMFX(ID3D10Effect* fx) {mFX = fx; head.setMFX(fx); body.setMFX(fx);}
	bool done(){return dmgfx.empty() && health < 0;}

	int health;
	GeoObject head, body;
	
	
private:
	
	Vector3 direction;
	std::vector<DamageSprites*> dmgfx;

	ID3D10Effect* mFX;
	ID3D10Device* md3dDevice;	
	ID3D10ShaderResourceView* mDiffuseMapRV1, *mDiffuseMapRV2;
	ID3D10ShaderResourceView* mSpecMapRV;

	ID3D10EffectShaderResourceVariable* mfxDiffuseMapVar;
	ID3D10EffectShaderResourceVariable* mfxSpecMapVar;
	ID3D10EffectMatrixVariable* mfxTexMtxVar;
};


#endif // BOX_H
