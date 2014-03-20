//=======================================================================================
// Box.cpp by Frank Luna (C) 2008 All Rights Reserved.
//=======================================================================================

#include "Origin.h"

//BREAK------------------------------
//
//-----------------------------------
Origin::Origin()
: md3dDevice(0), mVB(0), mIB(0)
{
	vertices[0] = Vertex(D3DXVECTOR3(0, 0, 0), RED);
	vertices[1] = Vertex(D3DXVECTOR3(1, 0, 0), RED);
	vertices[2] = Vertex(D3DXVECTOR3(0, 0, 0), GREEN);
	vertices[3] = Vertex(D3DXVECTOR3(0, 1, 0), GREEN);
	vertices[4] = Vertex(D3DXVECTOR3(0, 0, 0), CYAN);
	vertices[5] = Vertex(D3DXVECTOR3(0, 0, 1), CYAN);
}
 
Origin::~Origin()
{
	ReleaseCOM(mVB);
	ReleaseCOM(mIB);
}

void Origin::init(ID3D10Device* device, float scale)
{
	md3dDevice = device;

	// Scale the line.
	for(DWORD i = 0; i < mNumVertices; ++i)
		vertices[i].pos *= scale;


    D3D10_BUFFER_DESC vbd;
    vbd.Usage = D3D10_USAGE_IMMUTABLE;
    vbd.ByteWidth = sizeof(Vertex) * mNumVertices;
    vbd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    vbd.MiscFlags = 0;
    D3D10_SUBRESOURCE_DATA vinitData;
    vinitData.pSysMem = vertices;
    HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mVB));

}

void Origin::draw()
{
	/*
	for(DWORD i = 0; i < mNumVertices; ++i)
		D3DXVec3TransformCoord(&(vertices2[i].pos), &(vertices[i].pos), mod);

	void* temp;
	mVB->Map(D3D10_MAP_WRITE_DISCARD,0,&temp);
	memcpy(temp,(void*)vertices2,mNumVertices*sizeof(Vertex));
	mVB->Unmap();
	*/
	UINT stride = sizeof(Vertex);
    UINT offset = 0;
    md3dDevice->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
	md3dDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_LINELIST);
	md3dDevice->Draw(mNumVertices, 0);
}