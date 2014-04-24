#ifndef __SWORDOBJ_H
#define __SWORDOBJ_H

#include "GeoObject.h"
#include "Lighting.h"
#include "d3dUtil.h"

const int SWORD_BASE_DAMAGE = 32;
const int SWORD_DAMAGE_VARIATION = 18;
const float MAX_THETA =  PI/4;
const float SWING_SPEED = PI/2;

class SwordObj : public GeoObject
{
public:
	SwordObj(){}

	virtual void update(float dt);
	virtual void draw(Matrix* vp);

	virtual void init(	ID3D10EffectTechnique* t, 
						ID3D10EffectMatrixVariable* f,
						ID3D10EffectMatrixVariable* w,
						Geometry* g, Vertex v1, Vertex v2);

	void setDevice(ID3D10Device* d) { md3dDevice = d; }
	void setMFX(ID3D10Effect* fx) { mFX = fx; }
	void setAngle(float ang) {angle = ang;}
	void swing();
	bool hit;
	float theta;
	int getDamage(){return SWORD_BASE_DAMAGE + rand()%SWORD_DAMAGE_VARIATION;}

private:
	float angle;

	ID3D10Effect* mFX;
	ID3D10Device* md3dDevice;	
	ID3D10ShaderResourceView* mDiffuseMapRV;
	ID3D10ShaderResourceView* mSpecMapRV;

	ID3D10EffectShaderResourceVariable* mfxDiffuseMapVar;
	ID3D10EffectShaderResourceVariable* mfxSpecMapVar;
	ID3D10EffectMatrixVariable* mfxTexMtxVar;
};
#endif