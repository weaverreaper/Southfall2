#ifndef __BLOOD_H
#define __BLOOD_H

#include "GeoObject.h"
#include "d3dUtil.h"
#include "Square.h"

class Blood : public GeoObject
{
public:
	Blood(){}

	virtual void update(float dt);
	virtual void draw(Matrix* vp);

	virtual void init(	ID3D10EffectTechnique* t, 
						ID3D10EffectMatrixVariable* f,
						ID3D10EffectMatrixVariable* w,
						Geometry* g, Vertex v1, Vertex v2);

	void setAngles(float ang1, float ang2) {angle1 = ang1; angle2 = ang2;}
	
	void setDevice(ID3D10Device* d) { md3dDevice = d; }
	void setMFX(ID3D10Effect* fx) { mFX = fx; }
	void setDamage(int d){dmg = d;}
private:

	int dmg;
	float angle1, angle2;

	ID3D10Effect* mFX;
	ID3D10Device* md3dDevice;	
	ID3D10ShaderResourceView* mDiffuseMapRV;
	ID3D10ShaderResourceView* mSpecMapRV;

	ID3D10EffectShaderResourceVariable* mfxDiffuseMapVar;
	ID3D10EffectShaderResourceVariable* mfxSpecMapVar;
	ID3D10EffectMatrixVariable* mfxTexMtxVar;
};
#endif