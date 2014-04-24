//=======================================================================================
// FireballSprites.h by Frank Luna (C) 2008 All Rights Reserved.
//=======================================================================================

#ifndef FIREBALL_SPRITES_H
#define FIREBALL_SPRITES_H

#include "constants.h"
#include "d3dUtil.h"
#include "Lighting.h"

struct FireballVertex
{
	float offset;
	float seed;
	float depth;
	D3DXVECTOR2 sizeW;
};

class FireballSprites
{
public:

	FireballSprites();
	~FireballSprites();

	void init(ID3D10Device* device, UINT numFireballs);
	void draw(const D3DXVECTOR3& eyePosW, const D3DXMATRIX& viewProj);
	void update(float dt);
	void setPath(Vector3 posV, Vector3 dirV);

private:
	void buildVB();
	void buildFX();
	void buildVertexLayout();
	void buildShaderResourceView();

private:

	UINT mNumFireballs;
	float t;
	Vector3 pos, dir;
	

	ID3D10Device* md3dDevice;
	ID3D10Buffer* mVB;

	ID3D10Effect* mFX;
	ID3D10EffectTechnique* mTech;
	ID3D10ShaderResourceView* mFireballMapArrayRV;

	ID3D10EffectMatrixVariable* mfxViewProjVar;
	ID3D10EffectVariable* mfxEyePosVar;
	ID3D10EffectVariable* mfxCenterVar;
	ID3D10EffectVariable* mfxLightVar;
	ID3D10EffectVariable* mfxTimeVar;
	ID3D10EffectShaderResourceVariable* mfxFireballMapArrayVar;

	ID3D10InputLayout* mVertexLayout;
};

#endif // Fireball_SPRITES_H
