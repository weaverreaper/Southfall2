//=======================================================================================
// Billboard.h modified from code by Frank Luna (C) 2008 All Rights Reserved.
//=======================================================================================

#ifndef TREE_SPRITES_H
#define TREE_SPRITES_H

#include "d3dUtil.h"
#include "Lighting.h"

struct TreeVertex
{
	D3DXVECTOR3 centerW;
	D3DXVECTOR2 sizeW;
};

class Billboard
{
public:

	Billboard();
	~Billboard();

	void init(ID3D10Device* device, const D3DXVECTOR3 centers[], UINT numTrees, std::wstring fn, float _width, float _height);
	void draw(const Light& L, const D3DXVECTOR3& eyePosW, const D3DXMATRIX& viewProj);

	void setActive(bool a){active = a;}

private:
	void buildVB(const D3DXVECTOR3 centers[]);
	void buildFX();
	void buildVertexLayout();
	void buildShaderResourceView();

private:

	UINT mNumTrees;
	std::wstring filename;

	float width, height;
	bool active;

	ID3D10Device* md3dDevice;
	ID3D10Buffer* mVB;

	ID3D10Effect* mFX;
	ID3D10EffectTechnique* mTech;
	ID3D10ShaderResourceView* mTreeMapArrayRV;

	ID3D10EffectMatrixVariable* mfxViewProjVar;
	ID3D10EffectVariable* mfxEyePosVar;
	ID3D10EffectVariable* mfxLightVar;
	ID3D10EffectShaderResourceVariable* mfxTreeMapArrayVar;

	ID3D10InputLayout* mVertexLayout;
};

#endif // TREE_SPRITES_H
