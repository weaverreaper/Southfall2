#include "Sky.h"
#include "TextureMgr.h"
#include "Camera.h"

ID3D10Effect* CreateFX(ID3D10Device* device, std::wstring filename)
{
	DWORD shaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
    shaderFlags |= D3D10_SHADER_DEBUG;
	shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif
  
	ID3D10Blob* compilationErrors = 0;
	HRESULT hr = 0;
	ID3D10Effect* fx = 0;
	hr = D3DX10CreateEffectFromFile(filename.c_str(), 0, 0, 
		"fx_4_0", shaderFlags, 0, device, 0, 0, &fx, &compilationErrors, 0);
	if(FAILED(hr))
	{
		if( compilationErrors )
		{
			MessageBoxA(0, (char*)compilationErrors->GetBufferPointer(), 0, 0);
			ReleaseCOM(compilationErrors);
		}
		DXTrace(__FILE__, (DWORD)__LINE__, hr, filename.c_str(), true);
	}

	return fx;
}

struct SkyVertex
{
	D3DXVECTOR3 pos;
};
 
Sky::Sky()
: md3dDevice(0), mVB(0), mIB(0), mCubeMap(0)
{
	mNumIndices = 0;
}

Sky::~Sky()
{
	ReleaseCOM(mVB);
	ReleaseCOM(mIB);
}

void Sky::init(ID3D10Device* device, ID3D10ShaderResourceView* cubemap, float radius)
{
	md3dDevice = device;
	mCubeMap   = cubemap;

	SkyFX = CreateFX(device, L"source\\sky.fx");

	mTech         = SkyFX->GetTechniqueByName("SkyTech");
	mfxWVPVar     = SkyFX->GetVariableByName("gWVP")->AsMatrix();
	mfxCubeMapVar = SkyFX->GetVariableByName("gCubeMap")->AsShaderResource();


	std::vector<D3DXVECTOR3> vertices;
	std::vector<DWORD> indices;

	BuildGeoSphere(2, radius, vertices, indices);

	std::vector<SkyVertex> skyVerts(vertices.size());
	for(size_t i = 0; i < vertices.size(); ++i)
	{
		// Scale on y-axis to turn into an ellipsoid to make a flatter Sky surface
		skyVerts[i].pos = 0.5f*vertices[i];
	}

	D3D10_BUFFER_DESC vbd;
    vbd.Usage = D3D10_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(SkyVertex) * (UINT)skyVerts.size();
    vbd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    vbd.MiscFlags = 0;
    D3D10_SUBRESOURCE_DATA vinitData;
    vinitData.pSysMem = &skyVerts[0];
    HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mVB));

	mNumIndices = (UINT)indices.size();

	D3D10_BUFFER_DESC ibd;
    ibd.Usage = D3D10_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(DWORD) * mNumIndices;
    ibd.BindFlags = D3D10_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;
    ibd.MiscFlags = 0;
    D3D10_SUBRESOURCE_DATA iinitData;
    iinitData.pSysMem = &indices[0];
    HR(md3dDevice->CreateBuffer(&ibd, &iinitData, &mIB));
}

void Sky::draw()
{
	D3DXVECTOR3 eyePos = camera->getPos();

	// center Sky about eye in world space
	D3DXMATRIX W;
	D3DXMatrixTranslation(&W, eyePos.x, eyePos.y, eyePos.z);

	D3DXMATRIX V = camera->getView();
	D3DXMATRIX P = camera->getProj();

	D3DXMATRIX WVP = W*V*P;

	HR(mfxWVPVar->SetMatrix((float*)WVP));
	HR(mfxCubeMapVar->SetResource(mCubeMap));

	D3D10_PASS_DESC PassDesc;	

	SkyFX->GetTechniqueByName("SkyTech")->GetPassByIndex(0)->GetDesc(&PassDesc);

	//
	// Position vertex
	//
 	D3D10_INPUT_ELEMENT_DESC posVertexDesc[] =
	{
		{"POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D10_INPUT_PER_VERTEX_DATA, 0}
	};

    HR(md3dDevice->CreateInputLayout(posVertexDesc, 1, PassDesc.pIAInputSignature,
		PassDesc.IAInputSignatureSize, &Pos));

	UINT stride = sizeof(SkyVertex);
    UINT offset = 0;
    md3dDevice->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
	md3dDevice->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);
	md3dDevice->IASetInputLayout(Pos);
	md3dDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	D3D10_TECHNIQUE_DESC techDesc;
    mTech->GetDesc( &techDesc );

    for(UINT p = 0; p < techDesc.Passes; ++p)
    {
        ID3D10EffectPass* pass = mTech->GetPassByIndex(p);

		pass->Apply(0);
		md3dDevice->DrawIndexed(mNumIndices, 0, 0);
	}
}
 