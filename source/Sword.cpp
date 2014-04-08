//=======================================================================================
// Sword.cpp by Frank Luna (C) 2008 All Rights Reserved.
//=======================================================================================

#include "Sword.h"
#include "Vertex.h"

Sword::Sword(): md3dDevice(0), mVB(0), mIB(0)
{
}
 
Sword::~Sword()
{
	ReleaseCOM(mVB);
	ReleaseCOM(mIB);
}

void Sword::init(ID3D10Device* device, float scale)
{
	md3dDevice = device;

	// Create vertex buffer
    Vertex v[mNumVertices];

	// Fill in the front face vertex data.
 	v[0] = Vertex(+1.0f, 0.0f, +0.0f, +1.0f, 0.0f, +0.0f, 0.0f, 0.0f);
	v[1] = Vertex(+0.0f, 0.0f, -3.0f, +0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[2] = Vertex(-1.0f, 0.0f, +0.0f, -1.0f, 0.0f, +0.0f, 1.0f, 0.0f);
	v[3] = Vertex(+0.0f, 0.0f, +3.0f, +0.0f, 0.0f, +1.0f, 0.0f, 0.0f);

	// Fill in the back face vertex data.
	v[4] = Vertex(+1.0f, 80.0f, +0.0f, +1.0f, 0.0f, +0.0f, 0.0f, 0.0f);
	v[5] = Vertex(+0.0f, 80.0f, -3.0f, +0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[6] = Vertex(-1.0f, 80.0f, +0.0f, -1.0f, 0.0f, +0.0f, 1.0f, 0.0f);
	v[7] = Vertex(+0.0f, 80.0f, +3.0f, +0.0f, 0.0f, +1.0f, 0.0f, 0.0f);

	// Fill in the top face vertex data.
	 v[8] = Vertex(+0.0f, 90.0f, +0.0f, +1.0f, 0.0f, +0.0f, 0.0f, 0.0f);
	 v[9] = Vertex(+0.0f, 90.0f, +0.0f, +0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[10] = Vertex(+0.0f, 90.0f, +0.0f, -1.0f, 0.0f, +0.0f, 1.0f, 0.0f);
	v[11] = Vertex(+0.0f, 90.0f, +0.0f, +0.0f, 0.0f, +1.0f, 0.0f, 0.0f);

	// Scale the sword.
	for(DWORD i = 0; i < mNumVertices; ++i)
		v[i].pos *= scale;

    D3D10_BUFFER_DESC vbd;
    vbd.Usage = D3D10_USAGE_IMMUTABLE;
    vbd.ByteWidth = sizeof(Vertex) * mNumVertices;
    vbd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    vbd.MiscFlags = 0;
    D3D10_SUBRESOURCE_DATA vinitData;
    vinitData.pSysMem = v;
    HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mVB));


	// Create the index buffer

	DWORD i[mNumFaces*3];

	// Fill in the first face index data
	i[0] = 0; i[1] = 1; i[2] = 4;
	i[3] = 1; i[4] = 5; i[5] = 4;

	// Fill in the second face index data
	i[6] = 1; i[7]  = 2; i[8]  = 5;
	i[9] = 2; i[10] = 6; i[11] = 5;

	// Fill in the third face index data
	i[12] = 2; i[13] = 3; i[14] = 6;
	i[15] = 3; i[16] = 7; i[17] = 6;

	// Fill in the fourth face index data
	i[18] = 3; i[19] = 0; i[20] = 7;
	i[21] = 0; i[22] = 4; i[23] = 7;

	int d = 24;//offset

	// Fill in the first face index data
	i[0+d] = 4; i[1+d] = 5; i[2+d] = 8;
	i[3+d] = 5; i[4+d] = 9; i[5+d] = 8;

	// Fill in the second face index data
	i[6+d] = 5; i[7+d]  = 6; i[8+d]  = 9;
	i[9+d] = 6; i[10+d] = 10; i[11+d] = 9;

	// Fill in the third face index data
	i[12+d] = 6; i[13+d] = 7; i[14+d] = 10;
	i[15+d] = 7; i[16+d] = 11; i[17+d] = 10;

	// Fill in the fourth face index data
	i[18+d] = 7; i[19+d] = 4; i[20+d] = 11;
	i[21+d] = 4; i[22+d] = 8; i[23+d] = 11;


	D3D10_BUFFER_DESC ibd;
    ibd.Usage = D3D10_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(DWORD) * mNumFaces*3;
    ibd.BindFlags = D3D10_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;
    ibd.MiscFlags = 0;
    D3D10_SUBRESOURCE_DATA iinitData;
    iinitData.pSysMem = i;
    HR(md3dDevice->CreateBuffer(&ibd, &iinitData, &mIB));
}

void Sword::draw()
{
	UINT stride = sizeof(Vertex);
    UINT offset = 0;
    md3dDevice->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
	md3dDevice->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);
	md3dDevice->DrawIndexed(mNumFaces*3, 0, 0);
}