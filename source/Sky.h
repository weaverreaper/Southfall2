#ifndef SKYDOME_H
#define SKYDOME_H

#include "d3dUtil.h"
#include "Camera.h"

class Sky
{
public:
	Sky();
	~Sky();

	void init(ID3D10Device* device, ID3D10ShaderResourceView* cubemap, float radius);

	void draw();
	void setCamera(Camera* cam){camera = cam;}

private:
	Sky(const Sky& rhs);
	Sky& operator=(const Sky& rhs);

private:
	ID3D10Device* md3dDevice;
	ID3D10Buffer* mVB;
	ID3D10Buffer* mIB;
	ID3D10Effect* SkyFX;
	ID3D10InputLayout* Pos;

	ID3D10ShaderResourceView* mCubeMap;

	UINT mNumIndices;
	Camera* camera;
 
	ID3D10EffectTechnique* mTech;
	ID3D10EffectMatrixVariable* mfxWVPVar;
	ID3D10EffectShaderResourceVariable* mfxCubeMapVar;
};



#endif // SKY_H