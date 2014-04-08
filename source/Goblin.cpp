//=======================================================================================
// Goblin.cpp by Nate Higgins
//=======================================================================================

#include "Goblin.h"

Goblin::Goblin()
{
	;
        
}
 
Goblin::~Goblin()
{
	;
}
void Goblin::update(float dt)
{

	head.update(dt);
	body.update(dt);
}
void Goblin::update(float dt, Vector3 cam)
{
	Vector3 direction = body.getPosition() - cam;
	float rot = atan2f(direction.x,direction.z)+ToRadian(90);
	if(D3DXVec3Length(&direction) < 1000)
	{
	D3DXVec3Normalize(&direction,&direction);
	Vector3 v = -direction*100;
	v.y = 0;
	head.setVelocity(v);
	body.setVelocity(v);
	}
	else
	{
	head.setVelocity(Vector3(0,0,0));
	body.setVelocity(Vector3(0,0,0));
	}
	
	head.setroty(rot);
	body.setroty(rot);
	head.update2(dt);
	body.update2(dt);
}

void Goblin::init(ID3D10EffectTechnique* t, ID3D10EffectMatrixVariable* f, ID3D10EffectMatrixVariable* w, ID3D10Device* device, Geometry* h, Geometry* b, Terrain* ter)
{
	md3dDevice = device;

	mfxDiffuseMapVar = mFX->GetVariableByName("gDiffuseMap")->AsShaderResource();
	mfxSpecMapVar    = mFX->GetVariableByName("gSpecMap")->AsShaderResource();
	mfxTexMtxVar     = mFX->GetVariableByName("gTexMtx")->AsMatrix();

	 HR(D3DX10CreateShaderResourceViewFromFile(md3dDevice, 
		L"Textures/hide.jpg", 0, 0, &mDiffuseMapRV1, 0 ));

	 HR(D3DX10CreateShaderResourceViewFromFile(md3dDevice, 
		L"Textures/goblinskin.jpg", 0, 0, &mDiffuseMapRV2, 0 ));
	 
	 HR(D3DX10CreateShaderResourceViewFromFile(md3dDevice, 
		L"Textures/defaultspec.dds", 0, 0, &mSpecMapRV, 0 ));

	head.init(t,f,w,h,ter);
	body.init(t,f,w,b,ter);
}

void Goblin::draw(D3DXMATRIX* vp)
{
	mfxDiffuseMapVar->SetResource(mDiffuseMapRV2);
	mfxSpecMapVar->SetResource(mSpecMapRV);
	head.draw(vp);

	mfxDiffuseMapVar->SetResource(mDiffuseMapRV1);
	mfxSpecMapVar->SetResource(mSpecMapRV);
	body.draw(vp);
}
//BREAK------------------------------
//
//-----------------------------------
