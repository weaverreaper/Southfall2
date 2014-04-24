#include "DamageSprites.h"
#include <string>

ID3D10Device* DamageSprites::md3dDevice;

ID3D10ShaderResourceView* DamageSprites::mDamageMapArrayRV;
ID3D10Effect* DamageSprites::mFX;
ID3D10EffectTechnique* DamageSprites::mTech;

ID3D10EffectMatrixVariable* DamageSprites::mfxViewProjVar;
ID3D10EffectVariable* DamageSprites::mfxEyePosVar;
ID3D10EffectVariable* DamageSprites::mfxCenterVar;
ID3D10EffectVariable* DamageSprites::mfxLightVar;
ID3D10EffectVariable* DamageSprites::mfxTimeVar;
ID3D10EffectShaderResourceVariable* DamageSprites::mfxDamageMapArrayVar;

DamageSprites::DamageSprites()
{
	ZeroMemory(this, sizeof(DamageSprites));
}

void DamageSprites::releaseStatics()
{
	ReleaseCOM(mDamageMapArrayRV);
	ReleaseCOM(mFX);
}

DamageSprites::~DamageSprites()
{
	ReleaseCOM(mVB);
	//ReleaseCOM(mFX);
	//ReleaseCOM(mDamageMapArrayRV);
	ReleaseCOM(mVertexLayout);
}

//#include <WinBase.h>

void DamageSprites::init(ID3D10Device* device, int damageW)
{
	//change
	//md3dDevice = device;

	damage = damageW;
	mNumDamages = 0;
	
	int div = 125;
	while(div > 0)
	{
		mNumDamages += damageW/div;
		damageW %= div;
		div /= 5;
	}

	t = 0;
	pos = Vector3(0,0,0);
	dir = Vector3(0,0,0);

	//buildShaderResourceView();
	buildVB();
	//buildFX();
	buildVertexLayout();
}
void DamageSprites::update(const D3DXVECTOR3& eyePosW, const D3DXVECTOR3& posW, float dt)
{
	eyePos = eyePosW;
	pos = posW;
	t += 5*dt;//alter to alter particle expansion rate
	
	
}
bool DamageSprites::done()
{
	const int DURATION = 2;
	return t > 5*DURATION;
}
void DamageSprites::draw(const D3DXMATRIX& viewProj)
{
	mfxTimeVar->SetRawValue((void*)&t,0,sizeof(float));
	mfxEyePosVar->SetRawValue((void*)&eyePos, 0, sizeof(D3DXVECTOR3));
	mfxCenterVar->SetRawValue((void*)&pos, 0, sizeof(D3DXVECTOR3));
	mfxViewProjVar->SetMatrix((float*)&viewProj);
	mfxDamageMapArrayVar->SetResource(mDamageMapArrayRV);

	md3dDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_POINTLIST);
	md3dDevice->IASetInputLayout(mVertexLayout);

	D3D10_TECHNIQUE_DESC techDesc;
    mTech->GetDesc( &techDesc );

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

	
    for(UINT i = 0; i < techDesc.Passes; ++i)
    {
        ID3D10EffectPass* pass = mTech->GetPassByIndex(i);
		pass->Apply(0);

		UINT stride = sizeof(DamageVertex);
		UINT offset = 0;
		float blendFactor[] = {0.0f, 0.0f, 0.0f, 0.0f};			
		md3dDevice->OMSetBlendState(mTransparentBS, blendFactor, 0xffffffff);

		md3dDevice->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
		md3dDevice->Draw(mNumDamages, 0);
	}
	
}

void DamageSprites::buildVB()
{
	DamageVertex* v = new DamageVertex[mNumDamages];

	for(UINT i = 0; i < mNumDamages; ++i)
	{
		int div = 125;
		int damageW = damage;
		int numT;
		int cnt = 0;
		int sizecnt = 3;
		while(div > 0)
		{
			for(int t = 0; t < damageW/div; ++t)
			{
				v[cnt].seed = rand();
				v[cnt].value = sizecnt;
				v[cnt].sizeW = D3DXVECTOR2(float(20+sizecnt*12), float(20+sizecnt*12));
				++cnt;
			}
			damageW %= div;
			div /= 5;
			--sizecnt;
		}
	}
    
    D3D10_BUFFER_DESC vbd;
    vbd.Usage = D3D10_USAGE_IMMUTABLE;
    vbd.ByteWidth = sizeof(DamageVertex) * mNumDamages;
    vbd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    vbd.MiscFlags = 0;
    D3D10_SUBRESOURCE_DATA vinitData;
    vinitData.pSysMem = v;
    HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mVB));

	delete[] v;
}

void DamageSprites::buildFX()
{
	int a = GetTickCount();
	DWORD shaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;
/*
#if defined( DEBUG ) || defined( _DEBUG )
    shaderFlags |= D3D10_SHADER_DEBUG;
	shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif
*/  
	ID3D10Blob* compilationErrors = 0;
	HRESULT hr = 0;
	hr = D3DX10CreateEffectFromFile(L"source/damage.fx", 0, 0, 
		"fx_4_0", shaderFlags, 0, md3dDevice, 0, 0, &mFX, &compilationErrors, 0);
	int b = GetTickCount() - a;
	if(FAILED(hr))
	{
		if( compilationErrors )
		{
			MessageBoxA(0, (char*)compilationErrors->GetBufferPointer(), 0, 0);
			ReleaseCOM(compilationErrors);
		}
		DXTrace(__FILE__, (DWORD)__LINE__, hr, L"D3DX10CreateEffectFromFile", true);
	} 

	mTech = mFX->GetTechniqueByName("DamageBillboardTech");
	mfxViewProjVar     = mFX->GetVariableByName("gViewProj")->AsMatrix();
	mfxEyePosVar       = mFX->GetVariableByName("gEyePosW");
	mfxCenterVar       = mFX->GetVariableByName("gCenter");
	mfxTimeVar		   = mFX->GetVariableByName("dt");
	mfxDamageMapArrayVar = mFX->GetVariableByName("gDiffuseMapArray")->AsShaderResource();
}

void DamageSprites::buildVertexLayout()
{
	// Create the vertex input layout.
	D3D10_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{"PSIZE", 0, DXGI_FORMAT_R32_FLOAT, 0, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"BLENDWEIGHT", 0, DXGI_FORMAT_R32_FLOAT, 0, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"SIZE",     0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_VERTEX_DATA, 0},
	};

	// Create the input layout



    D3D10_PASS_DESC PassDesc;
    mTech->GetPassByIndex(0)->GetDesc(&PassDesc);
    HR(md3dDevice->CreateInputLayout(vertexDesc, 3, PassDesc.pIAInputSignature,
		PassDesc.IAInputSignatureSize, &mVertexLayout));
}

void DamageSprites::buildShaderResourceView(ID3D10Device* device)
{
	//
	// Load the texture elements individually from file.  These textures
	// won't be used by the GPU (0 bind flags), they are just used to 
	// load the image data from file.  We use the STAGING usage so the
	// CPU can read the resource.
	//
	md3dDevice = device;

	std::wstring filenames[4] = 
	{
		L"Textures/Dmg1.png",
		L"Textures/Dmg5.png",
		L"Textures/Dmg25.png",
		L"Textures/Dmg125.png"
	};

	ID3D10Texture2D* srcTex[4];
	for(UINT i = 0; i < 4; ++i)
	{
		D3DX10_IMAGE_LOAD_INFO loadInfo;

        loadInfo.Width  = D3DX10_FROM_FILE;
        loadInfo.Height = D3DX10_FROM_FILE;
        loadInfo.Depth  = D3DX10_FROM_FILE;
        loadInfo.FirstMipLevel = 0;
        loadInfo.MipLevels = D3DX10_FROM_FILE;
        loadInfo.Usage = D3D10_USAGE_STAGING;
        loadInfo.BindFlags = 0;
        loadInfo.CpuAccessFlags = D3D10_CPU_ACCESS_WRITE | D3D10_CPU_ACCESS_READ;
        loadInfo.MiscFlags = 0;
        loadInfo.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        loadInfo.Filter = D3DX10_FILTER_NONE;
        loadInfo.MipFilter = D3DX10_FILTER_NONE;
		loadInfo.pSrcInfo  = 0;

        HR(D3DX10CreateTextureFromFile(md3dDevice, filenames[i].c_str(), 
			&loadInfo, 0, (ID3D10Resource**)&srcTex[i], 0));
	}

	//
	// Create the texture array.  Each element in the texture 
	// array has the same format/dimensions.
	//

	D3D10_TEXTURE2D_DESC texElementDesc;
	srcTex[0]->GetDesc(&texElementDesc);

	D3D10_TEXTURE2D_DESC texArrayDesc;
	texArrayDesc.Width              = texElementDesc.Width;
	texArrayDesc.Height             = texElementDesc.Height;
	texArrayDesc.MipLevels          = texElementDesc.MipLevels;
	texArrayDesc.ArraySize          = 4;
	texArrayDesc.Format             = DXGI_FORMAT_R8G8B8A8_UNORM;
	texArrayDesc.SampleDesc.Count   = 1;
	texArrayDesc.SampleDesc.Quality = 0;
	texArrayDesc.Usage              = D3D10_USAGE_DEFAULT;
	texArrayDesc.BindFlags          = D3D10_BIND_SHADER_RESOURCE;
	texArrayDesc.CPUAccessFlags     = 0;
	texArrayDesc.MiscFlags          = 0;

	ID3D10Texture2D* texArray = 0;
	HR(md3dDevice->CreateTexture2D( &texArrayDesc, 0, &texArray));

	//
	// Copy individual texture elements into texture array.
	//

	// for each texture element...
	for(UINT i = 0; i < 4; ++i)
	{
		// for each mipmap level...
		for(UINT j = 0; j < texElementDesc.MipLevels; ++j)
		{
			D3D10_MAPPED_TEXTURE2D mappedTex2D;
			srcTex[i]->Map(j, D3D10_MAP_READ, 0, &mappedTex2D);
                    
            md3dDevice->UpdateSubresource(texArray, 
				D3D10CalcSubresource(j, i, texElementDesc.MipLevels),
                0, mappedTex2D.pData, mappedTex2D.RowPitch, 0);

            srcTex[i]->Unmap(j);
		}
	}	

	//
	// Create a resource view to the texture array.
	//
	
	D3D10_SHADER_RESOURCE_VIEW_DESC viewDesc;
	viewDesc.Format = texArrayDesc.Format;
	viewDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2DARRAY;
	viewDesc.Texture2DArray.MostDetailedMip = 0;
	viewDesc.Texture2DArray.MipLevels = texArrayDesc.MipLevels;
	viewDesc.Texture2DArray.FirstArraySlice = 0;
	viewDesc.Texture2DArray.ArraySize = 4;

	HR(md3dDevice->CreateShaderResourceView(texArray, &viewDesc, &mDamageMapArrayRV));

	//
	// Cleanup--we only need the resource view.
	//

	ReleaseCOM(texArray);

	for(UINT i = 0; i < 4; ++i)
		ReleaseCOM(srcTex[i]);
}