#ifndef __FIREBALL_H
#define __FIREBALL_H

#include "GeoObject.h"
#include "Lighting.h"

const float MAX_DIST =  900.f;
const float FIREBALL_SPEED = 1000.f;

class Fireball : public GeoObject
{
public:
	Fireball(){}
	virtual void update(float dt);
	virtual void draw(Matrix* vp);

	virtual void init(	ID3D10EffectTechnique* t, 
						ID3D10EffectMatrixVariable* f,
						ID3D10EffectMatrixVariable* w,
						Geometry* g);

	void setLight(Light* l){ light = l; }
	void setDevice(ID3D10Device* d) { md3dDevice = d; }
	void setMFX(ID3D10Effect* fx) { mFX = fx; }

	void shoot(Vector3 pos, Vector3 dir);

private:
	Light* light;
	float dist;

	ID3D10Effect* mFX;
	ID3D10Device* md3dDevice;	
	ID3D10ShaderResourceView* mDiffuseMapRV;
	ID3D10ShaderResourceView* mSpecMapRV;

	ID3D10EffectShaderResourceVariable* mfxDiffuseMapVar;
	ID3D10EffectShaderResourceVariable* mfxSpecMapVar;
	ID3D10EffectMatrixVariable* mfxTexMtxVar;
};
#endif
