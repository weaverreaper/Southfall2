#include "Fireball.h"

void Fireball::init(	ID3D10EffectTechnique* t, 
						ID3D10EffectMatrixVariable* f,
						ID3D10EffectMatrixVariable* w,
						Geometry* g)
{
	 dist = 0;	
	 setRadius(1);

	mfxDiffuseMapVar = mFX->GetVariableByName("gDiffuseMap")->AsShaderResource();
	mfxSpecMapVar    = mFX->GetVariableByName("gSpecMap")->AsShaderResource();
	mfxTexMtxVar     = mFX->GetVariableByName("gTexMtx")->AsMatrix();

	 HR(D3DX10CreateShaderResourceViewFromFile(md3dDevice, 
		L"Textures/flare.dds", 0, 0, &mDiffuseMapRV, 0 ));
	 
	 HR(D3DX10CreateShaderResourceViewFromFile(md3dDevice, 
		L"Textures/defaultspec.dds", 0, 0, &mSpecMapRV, 0 ));

	 GeoObject::init(t,f,w,g);
}

		
void Fireball::update(float dt)
{
	if (!active) return;

	light->pos += velocity * dt;
	dist += FIREBALL_SPEED * dt;
	if (dist > MAX_DIST)
	{
		light->on = 0;
		dist = 0;
		setInActive();
	}
	
	GeoObject::update(dt);
}

void Fireball::draw(Matrix* vp)
{
	mfxDiffuseMapVar->SetResource(mDiffuseMapRV);
	mfxSpecMapVar->SetResource(mSpecMapRV);
	
	D3DXMATRIX texMtx;
	D3DXMatrixIdentity(&texMtx);
	mfxTexMtxVar->SetMatrix((float*)&texMtx);

	GeoObject::draw(vp);

}

void Fireball::shoot(Vector3 pos, Vector3 dir)
{
	setVelocity(dir*FIREBALL_SPEED);
	setPosition(pos + 50*dir);
	setActive();

	light->dir = dir;
	light->pos = position - 10*dir;
	light->on = 1;
}

