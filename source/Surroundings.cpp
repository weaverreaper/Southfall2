#include "Surroundings.h"

void Surroundings::initTextures(wchar_t* t0, wchar_t* t1, wchar_t* t2, wchar_t* t3, wchar_t* t4, wchar_t* t5)
{
	for(int i = 0; i < 6; ++i)
		valid[i] = true;
	if(t0 != NULL){
		HR(D3DX10CreateShaderResourceViewFromFile(md3dDevice, t0, 0, 0, &mDiffuseMapRV[0], 0 ));
	}else
		valid[0] = false;
	if(t1 != NULL){
		HR(D3DX10CreateShaderResourceViewFromFile(md3dDevice, t1, 0, 0, &mDiffuseMapRV[1], 0 ));
	}else
		valid[1] = false;
	if(t2 != NULL){
		HR(D3DX10CreateShaderResourceViewFromFile(md3dDevice, t2, 0, 0, &mDiffuseMapRV[2], 0 ));
	}else
		valid[2] = false;
	if(t3 != NULL){
		HR(D3DX10CreateShaderResourceViewFromFile(md3dDevice, t3, 0, 0, &mDiffuseMapRV[3], 0 ));
	}else
		valid[3] = false;
	if(t4 != NULL){
		HR(D3DX10CreateShaderResourceViewFromFile(md3dDevice, t4, 0, 0, &mDiffuseMapRV[4], 0 ));
	}else
		valid[4] = false;
	if(t5 != NULL){
		HR(D3DX10CreateShaderResourceViewFromFile(md3dDevice, t5, 0, 0, &mDiffuseMapRV[5], 0 ));
	}else
		valid[5] = false;

}
	

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
		L"Textures/defaultspec.dds", 0, 0, &mSpecMapRV, 0 ));

	 terrain = terr;
	 Matrix m1,m2,m3,m4;
	 //set matrices of geoobjs

	 RotateX(&m1,-PI/2);
	 Scale(&m2,(terrain->x-2)*terrain->scale, 1000, 0);
	 //RotateX(&m3, PI/2);
	 w0.init(t,f,w,&wall,Vertex(),Vertex());
	 w0.setWorldMatrix(m1*m2);

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

	mfxSpecMapVar->SetResource(mSpecMapRV);
	if(valid[0]){
		mfxDiffuseMapVar->SetResource(mDiffuseMapRV[0]);
		w0.draw(vp);
	}if(valid[1]){
		mfxDiffuseMapVar->SetResource(mDiffuseMapRV[1]);
		w1.draw(vp);
	}if(valid[2]){
		mfxDiffuseMapVar->SetResource(mDiffuseMapRV[2]);
		w2.draw(vp);
	}if(valid[3]){
		mfxDiffuseMapVar->SetResource(mDiffuseMapRV[3]);
		w3.draw(vp);
	}if(valid[4]){
		mfxDiffuseMapVar->SetResource(mDiffuseMapRV[4]);
		w4.draw(vp);
	}if(valid[5]){
		mfxDiffuseMapVar->SetResource(mDiffuseMapRV[5]);
		w5.draw(vp);
	}
}
