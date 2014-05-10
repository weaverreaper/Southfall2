#include "Blood.h"

void Blood::init(	ID3D10EffectTechnique* t, 
						ID3D10EffectMatrixVariable* f,
						ID3D10EffectMatrixVariable* w,
						Geometry* g, Vertex v1, Vertex v2)
{
	dmg = 0;

	mfxDiffuseMapVar = mFX->GetVariableByName("gDiffuseMap")->AsShaderResource();
	mfxSpecMapVar    = mFX->GetVariableByName("gSpecMap")->AsShaderResource();
	mfxTexMtxVar     = mFX->GetVariableByName("gTexMtx")->AsMatrix();
	damageVar		 = mFX->GetVariableByName("blood")->AsScalar();
	
	 HR(D3DX10CreateShaderResourceViewFromFile(md3dDevice, 
		L"Textures\\Red0.png", 0, 0, &mDiffuseMapRV, 0 ));
	 
	 HR(D3DX10CreateShaderResourceViewFromFile(md3dDevice, 
		L"Textures/defaultspec.dds", 0, 0, &mSpecMapRV, 0 ));

	 GeoObject::init(t,f,w,g,v1,v2);
}

		
void Blood::update(float dt)
{
	if (!active) return;

	//rotz = PI/2 - 
	rotz = PI-(angle2);
	roty = -angle1;

	GeoObject::update(dt);
}

void Blood::draw(Matrix* vp)
{
	if(!active) return;

	D3DXMatrixMultiply(&wvp,&world,vp);
	fxMatrix->SetMatrix((float*)&(wvp));
	fxWorld->SetMatrix((float*)&world);

	mfxDiffuseMapVar->SetResource(mDiffuseMapRV);
	mfxSpecMapVar->SetResource(mSpecMapRV);
	
	D3DXMATRIX texMtx;
	D3DXMatrixIdentity(&texMtx);
	mfxTexMtxVar->SetMatrix((float*)&texMtx);


	D3D10_TECHNIQUE_DESC techDesc;
    tech->GetDesc( &techDesc );

	ID3D10BlendState* mTransparentBS;

	D3D10_BLEND_DESC blendDesc = {0};
	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.BlendEnable[0] = true;
	blendDesc.SrcBlend       = D3D10_BLEND_SRC_ALPHA;
	blendDesc.DestBlend      = D3D10_BLEND_INV_SRC_ALPHA;
	blendDesc.BlendOp        = D3D10_BLEND_OP_ADD;
	blendDesc.SrcBlendAlpha  = D3D10_BLEND_ONE;
	blendDesc.DestBlendAlpha = D3D10_BLEND_ZERO;
	blendDesc.BlendOpAlpha   = D3D10_BLEND_OP_ADD;
	blendDesc.RenderTargetWriteMask[0] = D3D10_COLOR_WRITE_ENABLE_ALL;

	HR(md3dDevice->CreateBlendState(&blendDesc, &mTransparentBS));

	damageVar->SetRawValue(&dmg, 0, sizeof(float));

    for(UINT i = 0; i < techDesc.Passes; ++i)
    {
        ID3D10EffectPass* pass = tech->GetPassByIndex(i);		
		pass->Apply(0);

		float blendFactor[] = {0.0f, 0.0f, 0.0f, 0.0f};	
		md3dDevice->OMSetBlendState(mTransparentBS, blendFactor, 0xffffffff);
			
		geom->draw();
	}

	float temp = -1;
	damageVar->SetRawValue(&temp, 0, sizeof(float));

	//GeoObject::draw(vp);

}
