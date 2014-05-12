#ifndef __FIREBALL_H
#define __FIREBALL_H

#include "GeoObject.h"
#include "Lighting.h"
#include "FireballSprites.h"
#include "d3dUtil.h"

class Camera;

const int FIREBALL_BASE_DAMAGE = 28;
const int FIREBALL_DAMAGE_VARIATION = 12;
const float MAX_FIRE_POWER = 3;
const float MAX_DIST =  700.f;
const float FIREBALL_SPEED = 300.f;

class Fireball : public GeoObject
{
public:
	Fireball(){}
	virtual void update(float dt);
	virtual void draw(Matrix* vp);

	virtual void init(	ID3D10EffectTechnique* t, 
						ID3D10EffectMatrixVariable* f,
						ID3D10EffectMatrixVariable* w,
						Camera* c,
						Geometry* g,
						int count = 200);

	void setLight(Light* l){ light = l; }
	void setDevice(ID3D10Device* d) { md3dDevice = d; }
	void setMFX(ID3D10Effect* fx) { mFX = fx; }
	void setStaticPosition(Vector3 pos) { fireballSprites.setPath(pos, ZERO); }	
	int getDamage();
	bool viable(){return getActiveState() && !rising;}

	bool shoot(Vector3 pos, Vector3 dir);
	void shoot2(Vector3 pos, Vector3 dir, Vector3 cam);
	void Fireball::release(Vector3 pos, Vector3 dir);
	Light* light;
	float dist;
	bool rising;
	bool ready;
	float maxdist;

private:
	
	float power;
	FireballSprites fireballSprites;
	Camera* cam;

	ID3D10Effect* mFX;
	ID3D10Device* md3dDevice;	
	ID3D10ShaderResourceView* mDiffuseMapRV;
	ID3D10ShaderResourceView* mSpecMapRV;

	ID3D10EffectShaderResourceVariable* mfxDiffuseMapVar;
	ID3D10EffectShaderResourceVariable* mfxSpecMapVar;
	ID3D10EffectMatrixVariable* mfxTexMtxVar;
};
#endif
