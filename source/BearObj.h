//=======================================================================================
// Bear.h by Nate Higgins
//=======================================================================================

#ifndef BearObj_H
#define BearObj_H

//#include "C:\Users\weaverem1\Dropbox\S14\Games2\PartII\Common\d3dUtil.h"
#include "d3dUtil.h"
#include "GeoObject.h"
#include "Fireball.h"
#include "SwordObj.h"

class BearObj : public GeoObject
{
public:

	BearObj();
	~BearObj();
	void init2(ID3D10EffectTechnique* t, ID3D10EffectMatrixVariable* f,ID3D10EffectMatrixVariable* w,ID3D10Device* device, Geometry* g, Terrain* ter);
	void update(float dt, Vector3 cam);
	void draw(D3DXMATRIX* vp);
	int health;
void BearObj::update(float dt, Vector3 cam, Fireball* fo, SwordObj* so);
private:
	ID3D10Device* md3dDevice;	
	ID3D10ShaderResourceView* mDiffuseMapRV;
	ID3D10ShaderResourceView* mSpecMapRV;

	ID3D10EffectShaderResourceVariable* mfxDiffuseMapVar;
	ID3D10EffectShaderResourceVariable* mfxSpecMapVar;
	ID3D10EffectMatrixVariable* mfxTexMtxVar;
};


#endif // BOX_H
