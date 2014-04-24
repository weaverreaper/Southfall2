//=======================================================================================
// DamageSprites.h by Frank Luna (C) 2008 All Rights Reserved.
//=======================================================================================

#ifndef DAMAGE_SPRITES_H
#define DAMAGE_SPRITES_H

#include "constants.h"
#include "d3dUtil.h"
#include "Lighting.h"

struct DamageVertex
{
	float seed;
	float value;
	D3DXVECTOR2 sizeW;
};

class DamageSprites
{
public:

	DamageSprites();
	~DamageSprites();

	void init(ID3D10Device* device, int damage);
	void draw(const D3DXMATRIX& viewProj);
	void update(const D3DXVECTOR3& eyePosW, const D3DXVECTOR3& posW, float dt);
	bool done();
	static void buildShaderResourceView(ID3D10Device* device);
	static void buildFX();
	static void releaseStatics();

private:
	void buildVB();
	void buildVertexLayout();
	

private:

	UINT damage;
	UINT mNumDamages;
	float t;
	Vector3 pos, dir, eyePos;
	DamageVertex* dmgs;

	static ID3D10Device* md3dDevice;
	ID3D10Buffer* mVB;

	static ID3D10Effect* mFX;
	static ID3D10EffectTechnique* mTech;
	static ID3D10ShaderResourceView* mDamageMapArrayRV;

	static ID3D10EffectMatrixVariable* mfxViewProjVar;
	static ID3D10EffectVariable* mfxEyePosVar;
	static ID3D10EffectVariable* mfxCenterVar;
	static ID3D10EffectVariable* mfxLightVar;
	static ID3D10EffectVariable* mfxTimeVar;
	static ID3D10EffectShaderResourceVariable* mfxDamageMapArrayVar;

	ID3D10InputLayout* mVertexLayout;
};

#endif // Damage_SPRITES_H
