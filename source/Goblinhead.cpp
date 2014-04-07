//=======================================================================================
// Goblin.cpp by Nate Higgins
//=======================================================================================

#include "Goblinhead.h"

GoblinHead::GoblinHead()
:md3dDevice(0), mVB(0), mIB(0)
{
	
	vertices[0] = Vertex(D3DXVECTOR3(2.0f, 4.0f, 0.0f),D3DXVECTOR3(1.4f, 4.1f, 0.0f), 0.6f, 0.5f); //nosetip
	vertices[1] = Vertex(D3DXVECTOR3(1.5f, 3.9f, -0.2f),D3DXVECTOR3(1.4f, 4.1f, 0.0f), 0.7f, 0.0f);//sidesofnoses
	vertices[2] = Vertex(D3DXVECTOR3(1.5f, 3.9f, 0.2f),D3DXVECTOR3(1.4f, 4.1f, 0.0f), 0.7f, 1.0f);
	vertices[3] = Vertex(D3DXVECTOR3(1.5f, 4.2f, 0.0f),D3DXVECTOR3(1.4f, 4.1f, 0.0f), 0.4f, 0.0f);//top
	vertices[4] = Vertex(D3DXVECTOR3(1.5f, 3.8f, 0.0f),D3DXVECTOR3(1.4f, 4.1f, 0.0f), 0.8f, 0.4f);//bottom
	vertices[5] = Vertex(D3DXVECTOR3(1.5f, 3.6f, 0.0f),D3DXVECTOR3(1.4f, 4.1f, 0.0f), 1.0f, 0.2f);//chin x3
	vertices[6] = Vertex(D3DXVECTOR3(1.2f, 3.6f, 0.2f),D3DXVECTOR3(1.4f, 4.1f, 0.0f), 1.0f, 0.4f);
	vertices[7] = Vertex(D3DXVECTOR3(1.2f, 3.6f, -0.2f),D3DXVECTOR3(1.4f, 4.1f, 0.0f), 1.0f, 0.6f);
	vertices[8] = Vertex(D3DXVECTOR3(1.2f, 4.4f, 0.3f),D3DXVECTOR3(1.4f, 4.1f, 0.0f), 0.2f, 0.0f); //crown
	vertices[9] = Vertex(D3DXVECTOR3(1.2f, 4.4f, -0.3f),D3DXVECTOR3(1.4f, 4.1f, 0.0f), 0.2f, 0.5f);
	vertices[10] = Vertex(D3DXVECTOR3(1.4f, 4.2f, 0.2f),D3DXVECTOR3(1.4f, 4.1f, 0.0f), 0.4f, 0.5f); 
	vertices[11] = Vertex(D3DXVECTOR3(1.4f, 4.2f, -0.2f),D3DXVECTOR3(1.4f, 4.1f, 0.0f), 0.4f, 1.0f);
	vertices[12] = Vertex(D3DXVECTOR3(1.2f, 4.6f, 0.0f),D3DXVECTOR3(1.4f, 4.1f, 0.0f), 0.0f, 0.0f);
	vertices[13] = Vertex(D3DXVECTOR3(1.0f, 4.4f, 0.0f),D3DXVECTOR3(1.4f, 4.1f, 0.0f), 0.2f, 1.0f);
	vertices[14] = Vertex(D3DXVECTOR3(0.8f, 3.6f, -0.2f),D3DXVECTOR3(1.4f, 4.1f, 0.0f), 1.0f, 0.8f);
	vertices[15] = Vertex(D3DXVECTOR3(0.8f, 3.6f, 0.2f),D3DXVECTOR3(1.4f, 4.1f, 0.0f), 1.0f, 1.0f);//end head
}


 
GoblinHead::~GoblinHead()
{
	ReleaseCOM(mVB);
	ReleaseCOM(mIB);
}

void GoblinHead::init(ID3D10Device* device, float scale)
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
		// nose
		0, 3, 2,
		0, 1, 3,
		0, 4, 1,
		0, 2, 4,

		// chin
		4, 5, 7,
		4, 6, 5,

		4, 7, 1,
		4, 2, 6,
		8, 2, 10,
		9, 11, 1,
		
		// cheeks
		6, 2, 8,
		7, 9, 1,


		// top face
		11, 10, 3,
		3, 10, 2,
		3, 1, 11,
		//15
		// head
		11, 9, 12, 
		10, 12, 8,
		11, 12, 10,
		13, 8, 12,//Good so far
		13, 12, 9,
		13, 6, 8,
		13, 9, 7,
		13, 15, 6,
		14, 13, 7,
		15, 13, 14
		//end of head beginning body 25 triangles
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

void GoblinHead::draw()
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
