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
	//right arm
	vertices[16] = Vertex(D3DXVECTOR3(1.4f, 3.4f, 1.0f), D3DXVECTOR3(1.0f, 2.0f, 1.0f), 0.2f, 0.0f);
	vertices[17] = Vertex(D3DXVECTOR3(0.6f, 3.4f, 1.0f), D3DXVECTOR3(1.0f, 2.0f, 1.0f), 0.2f, 0.3f);
	vertices[18] = Vertex(D3DXVECTOR3(1.0f, 3.2f, 0.8f), D3DXVECTOR3(1.0f, 2.0f, 1.0f), 0.2f, 0.0f);
	vertices[19] = Vertex(D3DXVECTOR3(0.8f, 1.6f, 1.1f), D3DXVECTOR3(1.0f, 2.0f, 1.0f), 0.2f, 0.3f);//22
	vertices[20] = Vertex(D3DXVECTOR3(1.2f, 2.0f, 0.9f), D3DXVECTOR3(1.0f, 2.0f, 1.0f), 0.2f, 0.0f);//24
	vertices[21] = Vertex(D3DXVECTOR3(1.0f, 1.8f, 0.7f), D3DXVECTOR3(1.0f, 2.0f, 1.0f), 0.2f, 0.3f);//23
	vertices[22] = Vertex(D3DXVECTOR3(2.0f, 1.7f, 1.1f), D3DXVECTOR3(2.0f, 1.9f, 1.0f), 0.2f, 0.3f);
	vertices[23] = Vertex(D3DXVECTOR3(2.0f, 1.7f, 0.7f), D3DXVECTOR3(2.0f, 1.9f, 1.0f), 0.2f, 0.0f);
	vertices[24] = Vertex(D3DXVECTOR3(2.1f, 1.9f, 0.9f), D3DXVECTOR3(2.0f, 1.9f, 1.0f), 0.2f, 0.3f);
	//left arm
	vertices[25] = Vertex(D3DXVECTOR3(1.4f, 3.4f, -1.0f), D3DXVECTOR3(1.0f, 2.0f, 1.0f), 0.2f, 0.0f);
	vertices[26] = Vertex(D3DXVECTOR3(0.6f, 3.4f, -1.0f), D3DXVECTOR3(1.0f, 2.0f, 1.0f), 0.2f, 0.3f);
	vertices[27] = Vertex(D3DXVECTOR3(1.0f, 3.2f, -0.8f), D3DXVECTOR3(1.0f, 2.0f, 1.0f), 0.2f, 0.0f);
	vertices[28] = Vertex(D3DXVECTOR3(0.8f, 1.6f, -1.1f), D3DXVECTOR3(1.0f, 2.0f, 1.0f), 0.2f, 0.3f);
	vertices[29] = Vertex(D3DXVECTOR3(1.2f, 2.0f, -0.9f), D3DXVECTOR3(1.0f, 2.0f, 1.0f), 0.2f, 0.0f);
	vertices[30] = Vertex(D3DXVECTOR3(1.0f, 1.8f, -0.7f), D3DXVECTOR3(1.0f, 2.0f, 1.0f), 0.2f, 0.3f);
	vertices[31] = Vertex(D3DXVECTOR3(2.0f, 1.7f, -1.1f), D3DXVECTOR3(2.0f, 1.9f, 1.0f), 0.2f, 0.3f);
	vertices[32] = Vertex(D3DXVECTOR3(2.0f, 1.7f, -0.7f), D3DXVECTOR3(2.0f, 1.9f, 1.0f), 0.2f, 0.0f);
	vertices[33] = Vertex(D3DXVECTOR3(2.1f, 1.9f, -0.9f), D3DXVECTOR3(2.0f, 1.9f, 1.0f), 0.2f, 0.3f);
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
		5, 6, 7,
		
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
		15, 13, 14,
		//end of head beginning arms 26 triangles
		//right arm
		//upper
		16, 17, 19,
		19, 20, 16,
		19, 18, 21,
		17, 18, 19,
		21, 18, 20,
		18, 16, 20,
		//lower
		19, 22, 20,
		22, 24, 20,
		20, 24, 21,
		24, 23, 21,
		21, 23, 19,
		23, 22, 19,
		22, 23, 24,
		//39
		//left
		//upper
		25, 26, 28,
		28, 29, 25,
		28, 27, 30,
		26, 27, 28,
		30, 27, 29,
		27, 25, 29,
		//lower
		28, 31, 29,
		31, 33, 29,
		29, 33, 30,
		33, 32, 30,
		30, 32, 28,
		32, 31, 28,
		31, 32, 33
		//52
		
	};

	redoBuffers(indices);

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
void GoblinHead::redoBuffers(DWORD ind[156])
{
	Vertex vertexBuffer2[156];
	DWORD indexBuffer2[156];

	for(int i = 0; i < 156/3; ++i)
	{
		for(int j = 0; j < 3; ++j)
		{
			vertexBuffer2[i*3+j] = vertices[ind[i*3+j]];
			if(j == 0)
			{
				vertexBuffer2[i*3+j].texC.x = 0;
				vertexBuffer2[i*3+j].texC.y = 0;
			}
			else if(j == 1)
			{
				vertexBuffer2[i*3+j].texC.x = 0;
				vertexBuffer2[i*3+j].texC.y = 1;
			}
			else if(j == 2)
			{
				vertexBuffer2[i*3+j].texC.x = 1;
				vertexBuffer2[i*3+j].texC.y = 0;
			}
			indexBuffer2[i*3+j] = i*3+j;
		}
	}

	D3D10_BUFFER_DESC vbd;
    vbd.Usage = D3D10_USAGE_IMMUTABLE;
    vbd.ByteWidth = sizeof(Vertex) * 156;
    vbd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    vbd.MiscFlags = 0;
    D3D10_SUBRESOURCE_DATA vinitData;
    vinitData.pSysMem = vertexBuffer2;
    HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mVB));

	D3D10_BUFFER_DESC ibd;
    ibd.Usage = D3D10_USAGE_IMMUTABLE;
    ibd.ByteWidth = sizeof(DWORD) * mNumFaces*3;
    ibd.BindFlags = D3D10_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;
    ibd.MiscFlags = 0;
    D3D10_SUBRESOURCE_DATA iinitData;
    iinitData.pSysMem = indexBuffer2;
    HR(md3dDevice->CreateBuffer(&ibd, &iinitData, &mIB));
}