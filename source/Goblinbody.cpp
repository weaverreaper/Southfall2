//=======================================================================================
// Goblin.cpp by Nate Higgins
//=======================================================================================

#include "Goblinbody.h"

GoblinBody::GoblinBody()
:md3dDevice(0), mVB(0), mIB(0)
{
	
	//start shoulders
	vertices[1] = Vertex(D3DXVECTOR3(1.2f, 3.6f, 0.2f), D3DXVECTOR3(1.0f, 3.5f, 0.0f), 0.0f, 0.0f);
	vertices[2] = Vertex(D3DXVECTOR3(0.8f, 3.6f, 0.2f), D3DXVECTOR3(1.0f, 3.5f, 0.0f), 0.0f, 0.3f);
	vertices[3] = Vertex(D3DXVECTOR3(1.4f, 3.4f, 1.0f), D3DXVECTOR3(1.0f, 3.5f, 0.0f), 0.2f, 0.0f);
	vertices[4] = Vertex(D3DXVECTOR3(0.6f, 3.4f, 1.0f), D3DXVECTOR3(1.0f, 3.5f, 0.0f), 0.2f, 0.3f);
	vertices[5] = Vertex(D3DXVECTOR3(1.2f, 3.6f, -0.2f),D3DXVECTOR3(1.0f, 3.5f, 0.0f), 0.0f, 1.0f);
	vertices[6] = Vertex(D3DXVECTOR3(0.8f, 3.6f, -0.2f), D3DXVECTOR3(1.0f, 3.5f, 0.0f), 0.0f, 0.6f);
	vertices[7] = Vertex(D3DXVECTOR3(1.4f, 3.4f, -1.0f), D3DXVECTOR3(1.0f, 3.5f, 0.0f), 0.2f, 1.0f);
	vertices[8] = Vertex(D3DXVECTOR3(0.8f, 3.4f, -1.0f), D3DXVECTOR3(1.0f, 3.5f, 0.0f), 0.2f, 0.6f);
	//end shoulders
	//begin waist
	vertices[9] = Vertex(D3DXVECTOR3(1.2f, 2.0f, 0.6f), D3DXVECTOR3(0.8f, 2.0f, 0.0f), 0.4f, 0.0f);
	vertices[10] = Vertex(D3DXVECTOR3(1.2f, 2.0f, -0.6f), D3DXVECTOR3(0.8f, 2.0f, 0.0f), 0.4f, 0.4f);
	vertices[11] = Vertex(D3DXVECTOR3(1.2f, 2.0f, 0.0f), D3DXVECTOR3(0.8f, 2.0f, 0.0f), 0.4f, 0.2f);
	vertices[12] = Vertex(D3DXVECTOR3(0.6f, 2.0f, 0.6f), D3DXVECTOR3(0.8f, 2.0f, 0.0f), 0.4f, 1.0f);
	vertices[13] = Vertex(D3DXVECTOR3(0.6f, 2.0f, -0.6f), D3DXVECTOR3(0.8f, 2.0f, 0.0f), 0.4f, 0.6f);
	vertices[14] = Vertex(D3DXVECTOR3(0.6f, 2.0f, 0.0f), D3DXVECTOR3(0.8f, 2.0f, 0.0f), 0.4f, 0.8f);
	//begin legs
    vertices[15] = Vertex(D3DXVECTOR3(1.2f, 1.6f, 0.0f), D3DXVECTOR3(0.8f, 1.0f, 0.0f), 0.8f, 0.5f);
	vertices[16] = Vertex(D3DXVECTOR3(0.6f, 1.6f, 0.0f), D3DXVECTOR3(0.8f, 1.0f, 0.0f), 0.8f, 0.5f);
	vertices[17] = Vertex(D3DXVECTOR3(1.4f, 0.0f, 0.4f), D3DXVECTOR3(0.8f, 1.0f, 0.0f), 0.6f, 0.5f);
	vertices[18] = Vertex(D3DXVECTOR3(1.4f, 0.0f, -0.4f), D3DXVECTOR3(0.8f, 1.0f, 0.0f), 0.6f, 0.5f);
	vertices[19] = Vertex(D3DXVECTOR3(0.8f, 0.0f, 0.4f), D3DXVECTOR3(0.8f, 1.0f, 0.0f), 0.6f, 0.5f);
	vertices[20] = Vertex(D3DXVECTOR3(0.8f, 0.0f, -0.4f), D3DXVECTOR3(0.8f, 1.0f, 0.0f), 0.6f, 0.5f);
	vertices[21] = Vertex(D3DXVECTOR3(1.4f, 0.0f, 0.6f), D3DXVECTOR3(0.8f, 1.0f, 0.0f), 0.6f, 0.5f);
	vertices[22] = Vertex(D3DXVECTOR3(1.4f, 0.0f, -0.6f), D3DXVECTOR3(0.8f, 1.0f, 0.0f), 0.6f, 0.5f);
	vertices[23] = Vertex(D3DXVECTOR3(0.8f, 0.0f, 0.6f), D3DXVECTOR3(0.8f, 1.0f, 0.0f), 0.6f, 0.5f);
	vertices[24] = Vertex(D3DXVECTOR3(0.8f, 0.0f, -0.6f), D3DXVECTOR3(0.8f, 1.0f, 0.0f), 0.6f, 0.5f);
}

//Left side is positive
 
GoblinBody::~GoblinBody()
{
	ReleaseCOM(mVB);
	ReleaseCOM(mIB);
}

void GoblinBody::init(ID3D10Device* device, float scale)
{
	md3dDevice = device;
 
	// Scale the box.
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
	//md3dDevice->

	// Create the index buffer

	DWORD indices[] = {
		//end of head beginning body 25 triangles
		1, 4, 3,
		2, 4, 1,
		5, 7, 8,
		8, 6, 5,
		//end of shoulders
		1, 3, 9,
		7, 5, 10,
		1, 9, 11,
		10, 5, 11,
		5, 1, 11,
		//end front
		4, 2, 12,
		6, 8, 13,
		12, 2, 14,
		6, 13, 14,
		2, 6, 14,
		//end back
		//sides
		12, 9, 4,
		4, 9, 3,
		10, 13, 8,
		10, 8, 7,
		12, 13, 16,// begin legs
		9, 15, 10,
		9, 17, 15,
		19, 12 ,16,
		18, 10, 15,
		20, 16 ,13,//24
		15, 16, 20,
		15, 20, 18,
		15, 19, 16,
		15, 17, 19,
		17, 9, 21,
		10, 18, 22,
		12, 19, 23,
		13, 24, 20,
		9,12, 21,
		23, 21, 12,
		24, 13, 22,
		13, 10, 22, //36
	};

	D3D10_BUFFER_DESC ibd;
    ibd.Usage = D3D10_USAGE_IMMUTABLE;
    ibd.ByteWidth = sizeof(DWORD) * mNumFaces*3;
    ibd.BindFlags = D3D10_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;
    ibd.MiscFlags = 0;
    D3D10_SUBRESOURCE_DATA iinitData;
    iinitData.pSysMem = indices;
    HR(md3dDevice->CreateBuffer(&ibd, &iinitData, &mIB));
}

void GoblinBody::draw()
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
	md3dDevice->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);
	md3dDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	md3dDevice->DrawIndexed(mNumFaces*3, 0, 0);
}
//BREAK------------------------------
//
//-----------------------------------
