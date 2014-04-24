#include "SwordObj.h"

void SwordObj::init(	ID3D10EffectTechnique* t, 
						ID3D10EffectMatrixVariable* f,
						ID3D10EffectMatrixVariable* w,
						Geometry* g, Vertex v1, Vertex v2)
{
	theta = -1;	
	hit = false;
	setRadius(150);

	mfxDiffuseMapVar = mFX->GetVariableByName("gDiffuseMap")->AsShaderResource();
	mfxSpecMapVar    = mFX->GetVariableByName("gSpecMap")->AsShaderResource();
	mfxTexMtxVar     = mFX->GetVariableByName("gTexMtx")->AsMatrix();

	 HR(D3DX10CreateShaderResourceViewFromFile(md3dDevice, 
		L"Textures\\Metal1.jpg", 0, 0, &mDiffuseMapRV, 0 ));
	 
	 HR(D3DX10CreateShaderResourceViewFromFile(md3dDevice, 
		L"Textures/defaultspec.dds", 0, 0, &mSpecMapRV, 0 ));

	 GeoObject::init(t,f,w,g,v1,v2);
}

		
void SwordObj::update(float dt)
{
	if (!active) return;

	if (theta >= 0)
		theta += SWING_SPEED * dt;

	if (theta > MAX_THETA || theta < 0)
	{
		theta = -1;
		rotx = 0;
		rotz = PI/7;
		roty = -angle+PI+PI/7;
	}
	else
	{
		rotx = theta*3/2;
		rotz = PI/7+theta*2;
		roty = -angle+PI+PI/7;
	}

	GeoObject::update(dt);
}

void SwordObj::draw(Matrix* vp)
{
	mfxDiffuseMapVar->SetResource(mDiffuseMapRV);
	mfxSpecMapVar->SetResource(mSpecMapRV);
	
	D3DXMATRIX texMtx;
	D3DXMatrixIdentity(&texMtx);
	mfxTexMtxVar->SetMatrix((float*)&texMtx);

	GeoObject::draw(vp);

}

void SwordObj::swing()
{
	if(theta >= 0)
		return;
	theta = 0;
	hit = false;
}

