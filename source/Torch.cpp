#include "Torch.h"

void Torch::init(	ID3D10EffectTechnique* t, 
						ID3D10EffectMatrixVariable* f,
						ID3D10EffectMatrixVariable* w,
						Fireball* _light, Fireball* gameFB,
						Geometry* g, Audio* a,
						Vector3 pos)
{	
	setRadius(20);
	mfxDiffuseMapVar = mFX->GetVariableByName("gDiffuseMap")->AsShaderResource();
	mfxSpecMapVar    = mFX->GetVariableByName("gSpecMap")->AsShaderResource();
	mfxTexMtxVar     = mFX->GetVariableByName("gTexMtx")->AsMatrix();

	audio = a;

	 HR(D3DX10CreateShaderResourceViewFromFile(md3dDevice, 
		L"Textures/wood.jpg", 0, 0, &mDiffuseMapRV, 0 ));
	 
	 HR(D3DX10CreateShaderResourceViewFromFile(md3dDevice, 
		L"Textures/defaultspec.dds", 0, 0, &mSpecMapRV, 0 ));

	 GeoObject::init(t,f,w,g);
	 setPosition(pos);

	 fb = gameFB;

	 fire = _light;
	 fire->setVelocity(ZERO);
	 fire->setStaticPosition(pos + Vector3(0,20.01,0)); 
}

void Torch::draw(Matrix* vp)
{
	if (!active) return;

	mfxDiffuseMapVar->SetResource(mDiffuseMapRV);
	mfxSpecMapVar->SetResource(mSpecMapRV);
	
	D3DXMATRIX texMtx;
	D3DXMatrixIdentity(&texMtx);
	mfxTexMtxVar->SetMatrix((float*)&texMtx);
	
	GeoObject::draw(vp);
	fire->draw(vp);
}

void Torch::update(float dt)
{
	GeoObject::update(dt);
	
	fire->dist = 0;	
	fire->update(dt);
	
	if(!fire->getActiveState() && collided(fb))
	{			
		fb->setInActive();
		fb->light->on = 0;
		
		fire->setActive();		
		audio->playCue(TORCH_CUE);
	}	
}