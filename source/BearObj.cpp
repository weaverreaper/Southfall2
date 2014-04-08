//=======================================================================================
// Bear.cpp by Nate Higgins
//=======================================================================================

#include "BearObj.h"

BearObj::BearObj()
{
	health = 400;
	radius = 25;
}
 
BearObj::~BearObj()
{
	;
}
void BearObj::draw(D3DXMATRIX* vp)
{
	if(!active) return;

	mfxDiffuseMapVar->SetResource(mDiffuseMapRV);
	mfxSpecMapVar->SetResource(mSpecMapRV);

	D3DXMatrixMultiply(&wvp,&world,vp);
	fxMatrix->SetMatrix((float*)&(wvp));
	fxWorld->SetMatrix((float*)&world);

	D3D10_TECHNIQUE_DESC techDesc;
    tech->GetDesc( &techDesc );
    for(UINT p = 0; p < techDesc.Passes; ++p)
    {
        tech->GetPassByIndex( p )->Apply(0);
		geom->draw();
	}
}

void BearObj::init2(ID3D10EffectTechnique* t, ID3D10EffectMatrixVariable* f,ID3D10EffectMatrixVariable* w,ID3D10Device* device, Geometry* g, Terrain* ter)
{
	//md3dDevice = device;
	md3dDevice = device;

	mfxDiffuseMapVar = mFX->GetVariableByName("gDiffuseMap")->AsShaderResource();
	mfxSpecMapVar    = mFX->GetVariableByName("gSpecMap")->AsShaderResource();
	mfxTexMtxVar     = mFX->GetVariableByName("gTexMtx")->AsMatrix();

	 HR(D3DX10CreateShaderResourceViewFromFile(md3dDevice, 
		L"Textures/fur.jpg", 0, 0, &mDiffuseMapRV, 0 ));
	 
	 HR(D3DX10CreateShaderResourceViewFromFile(md3dDevice, 
		L"Textures/defaultspec.dds", 0, 0, &mSpecMapRV, 0 ));
	geom = g;
	fxMatrix = f;
	fxWorld = w;
	tech = t;
	terr = ter;
	D3DXMatrixIdentity(&world);
	D3DXMatrixIdentity(&wvp);
}
void BearObj::update(float dt, Vector3 cam, Fireball* fo, SwordObj* so)
{
	if (!getActiveState())
		return;
	if(this->collided(fo) && fo->getActiveState())
	{		health -= 1;
		fo->setInActive();
		fo->light->on = 0;
		fo->dist = 0;
	}
	if(this->collided(so) && !so->hit && so->theta > 0)
	{
		health -= 25;
		so->hit = true;
	}
	if(health <= 0)
	{
		setInActive();
	}
}
void BearObj::update(float dt, Vector3 cam)
{
	Vector3 direction = position - cam;
	float rot = atan2f(direction.x,direction.z)+ToRadian(90);
	if(D3DXVec3Length(&direction) < 1000)
	{
	D3DXVec3Normalize(&direction,&direction);
	Vector3 v = -direction*100;
	v.y = 0;
	roty = rot;
	velocity = v;
	position += velocity*dt;
	}
	Matrix temp;
	Identity(&temp);
	Identity(&world);
	position.y = getTerrHeight()+15;
	D3DXMatrixRotationYawPitchRoll(&temp, roty, rotz, rotx);
	world *= temp;
	Scale(&temp,scale,scale,scale);
	world *= temp;
	Translate(&temp, position.x, position.y, position.z);
	world *= temp;
}
//BREAK------------------------------
//
//-----------------------------------
