#include "Surroundings.h"

void Surroundings::init(	ID3D10EffectTechnique* t, 
							ID3D10EffectMatrixVariable* f,
							ID3D10EffectMatrixVariable* w,
							Terrain* terr
							)
{
	mfxDiffuseMapVar = mFX->GetVariableByName("gDiffuseMap")->AsShaderResource();
	mfxSpecMapVar    = mFX->GetVariableByName("gSpecMap")->AsShaderResource();
	mfxTexMtxVar     = mFX->GetVariableByName("gTexMtx")->AsMatrix();

	wall.init(md3dDevice);

	 HR(D3DX10CreateShaderResourceViewFromFile(md3dDevice, 
		L"Textures/water.png", 0, 0, &mDiffuseMapRV1, 0 ));
	 
	  HR(D3DX10CreateShaderResourceViewFromFile(md3dDevice, 
		L"Textures/Rock1.jpg", 0, 0, &mDiffuseMapRV2, 0 ));

	 HR(D3DX10CreateShaderResourceViewFromFile(md3dDevice, 
		L"Textures/Sky2.jpg", 0, 0, &mDiffuseMapRV3, 0 ));

	 HR(D3DX10CreateShaderResourceViewFromFile(md3dDevice, 
		L"Textures/defaultspec.dds", 0, 0, &mSpecMapRV, 0 ));

	 terrain = terr;
	 Matrix m1,m2,m3,m4;
	 //set matrices of geoobjs

	 RotateX(&m1,-PI/2);
	 Scale(&m2,(terrain->x-2)*terrain->scale, 1000, 0);
	 RotateX(&m3, -PI/2);
	 w1.init(t,f,w,&wall,Vertex(),Vertex());
	 w1.setWorldMatrix(m1*m2*m3);

	 RotateZ(&m1,PI/2);
	 Scale(&m2,1, 1000, (terrain->z-2)*terrain->scale*2);
	 Translate(&m3,0,0,-(terrain->z-2)*terrain->scale);
	 w2.init(t,f,w,&wall,Vertex(),Vertex());
	 w2.setWorldMatrix(m1*m2*m3);

	 RotateZ(&m1,-PI/2);
	 Scale(&m2,1, 1000, (terrain->z-2)*terrain->scale*2);
	 Translate(&m3,(terrain->x-2)*terrain->scale,1000,-(terrain->z-2)*terrain->scale);
	 w3.init(t,f,w,&wall,Vertex(),Vertex());
	 w3.setWorldMatrix(m1*m2*m3);

	 RotateX(&m1,PI/2);
	 Scale(&m2,(terrain->x-2)*terrain->scale, 1000, 1);
	 Translate(&m3,0,1000,(terrain->z-2)*terrain->scale);
	 w4.init(t,f,w,&wall,Vertex(),Vertex());
	 w4.setWorldMatrix(m1*m2*m3);

	 Scale(&m2,(terrain->x-2)*terrain->scale, 1, (terrain->z-2)*terrain->scale*2);
	 Translate(&m3,0,1000,-(terrain->z-2)*terrain->scale);
	 w5.init(t,f,w,&wall,Vertex(),Vertex());
	 w5.setWorldMatrix(m2*m3);	 
}

void Surroundings::draw(Matrix* vp)
{
	D3DXMATRIX texMtx;
	D3DXMatrixIdentity(&texMtx);
	mfxTexMtxVar->SetMatrix((float*)&texMtx);
	
	mfxDiffuseMapVar->SetResource(mDiffuseMapRV1);
	mfxSpecMapVar->SetResource(mSpecMapRV);
	//draw water
	w1.draw(vp);

	mfxDiffuseMapVar->SetResource(mDiffuseMapRV2);
	//draw rock
	w2.draw(vp);
	w3.draw(vp);
	w4.draw(vp);

	mfxDiffuseMapVar->SetResource(mDiffuseMapRV3);
	//draw sky
	w5.draw(vp);

}
