//=======================================================================================
// Terrain.cpp
//=======================================================================================

#include "Terrain.h"

Terrain::Terrain()
:md3dDevice(0), mVB(0), mIB(0)
{
	ifs.open("Worlds/World1.txt");
	for(int i = 0; i < x; ++i)
	{
		for(int j = 0; j < z; ++j)
		{
			ifs >> grid[i][j];
			if(i < x-1 && j < z-1)
				vertices[i*(x-1)+j] = Vertex(D3DXVECTOR3(i,grid[i][j],j),D3DXCOLOR(4*float(i%2)/x,.4,4*float(j%2)/z,1));
		}
	}
	ifs.close();        
}

Terrain::Terrain(D3DXCOLOR col)
:md3dDevice(0), mVB(0), mIB(0)
{
	
	ifs.open("World1.txt");
	for(int i = 0; i < x; ++i)
	{
		for(int j = 0; j < z; ++j)
		{
			ifs >> grid[i][j];
			if(i < x-1 && j < z-1)
				vertices[i*(x-1)+j] = Vertex(D3DXVECTOR3(i,grid[i][j],j),D3DXCOLOR(min(1,4*float(i%2)/x+col.r),col.g,min(1,4*float(j%2)/z+col.b),1));
		}
	}
	ifs.close();
}
 
Terrain::~Terrain()
{
	ReleaseCOM(mVB);
	ReleaseCOM(mIB);
}

void Terrain::init(ID3D10Device* device, float scale)
{
	md3dDevice = device;
 
	// Scale
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

	DWORD indices[(x-2)*(z-2)*3*4];//4 triangles per square
	
	for(int i = 0; i < x-2; ++i)
	{
		for(int j = 0; j < z-2; ++j)
		{
			int loc = (i*(x-2)+j)*3*4;
			int a = i*(x-1)+j;
			int b = i*(x-1)+j+1;
			int d = i*(x-1)+j+(x-1);
			int c = i*(x-1)+j+(x-1)+1;

			indices[loc++] = a;
			indices[loc++] = b;
			indices[loc++] = c;

			indices[loc++] = a;
			indices[loc++] = b;
			indices[loc++] = d;

			indices[loc++] = a;
			indices[loc++] = c;
			indices[loc++] = d;

			indices[loc++] = b;
			indices[loc++] = c;
			indices[loc++] = d;

		}
	}

	D3D10_BUFFER_DESC ibd;
    ibd.Usage = D3D10_USAGE_IMMUTABLE;
    ibd.ByteWidth = sizeof(DWORD)*mNumFaces*3;
    ibd.BindFlags = D3D10_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;
    ibd.MiscFlags = 0;
    D3D10_SUBRESOURCE_DATA iinitData;
    iinitData.pSysMem = indices;
    HR(md3dDevice->CreateBuffer(&ibd, &iinitData, &mIB));
}

void Terrain::draw()
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
