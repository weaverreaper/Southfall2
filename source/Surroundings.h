#ifndef __SURROUNDINGS_H
#define __SURROUNDINGS_H

#include "constants.h"
#include "GeoObject.h"
#include "Square.h"
#include "Terrain.h"

class Surroundings
{
public:
	Surroundings(){}
	void update(float dt);
	void draw(Matrix* vp);
	void initTextures(wchar_t* t0, wchar_t* t1, wchar_t* t2, wchar_t* t3, wchar_t* t4, wchar_t* t5);
	void init(	ID3D10EffectTechnique* t, 
						ID3D10EffectMatrixVariable* f,
						ID3D10EffectMatrixVariable* w,
						Terrain* terr
						);

	void setDevice(ID3D10Device* d) { md3dDevice = d; }
	void setMFX(ID3D10Effect* fx) { mFX = fx; }

private:
	Square wall;
	bool valid[6];
	GeoObject w0,w1,w2,w3,w4,w5;
	Terrain* terrain;
	ID3D10Effect* mFX;
	ID3D10Device* md3dDevice;	
	ID3D10ShaderResourceView* mDiffuseMapRV[6];
	ID3D10ShaderResourceView* mSpecMapRV;

	ID3D10EffectShaderResourceVariable* mfxDiffuseMapVar;
	ID3D10EffectShaderResourceVariable* mfxSpecMapVar;
	ID3D10EffectMatrixVariable* mfxTexMtxVar;
};
#endif;