//=======================================================================================
// Terrain.cpp
//=======================================================================================
#include <D3DX10math.h>
#include "Terrain.h"

Terrain::Terrain()
:md3dDevice(0), mVB(0), mIB(0)
{
}
 
Terrain::~Terrain()
{
	for(int i = 0; i < x; ++i)
		delete[] grid[i];
	delete[] grid;
	delete[] vertices;

	ReleaseCOM(mVB);
	ReleaseCOM(mIB);
}
void Terrain::initFile(char* file)
{
	ifs.open(file);
	ifs >> x >> z;
	mNumVertices = (x-1)*(z-1);
	//mNumFaces = 4*(x-2)*(z-2);
	mNumFaces = 2*(x-2)*(z-2);

	grid = new float*[x];
	for(int i = 0; i < x; ++i)
		grid[i] = new float[z];
	
	vertices = new Vertex[mNumVertices];

	for(int i = 0; i < x; ++i)
		for(int j = 0; j < z; ++j)
			ifs >> grid[i][j];
	ifs.close();

	for(int i = 0; i < x; ++i)
	{
		for(int j = 0; j < z; ++j)
		{
			if(i < x-1 && j < z-1)
			{
				Vector3 norm(0,1,0), norm2(0,1,0);
				D3DXVec3Cross(&norm,
					&(Vector3(i+1,grid[i+1][j],j) - Vector3(i,grid[i][j],j)),
					&(Vector3(i,grid[i][j+1],j+1) - Vector3(i,grid[i][j],j)));
				norm = -norm;
				vertices[i*(z-1)+j] = Vertex(i,grid[i][j],j,norm.x,norm.y,norm.z,i%2,j%2);
				//vertices[i*(x-1)+j] = Vertex(D3DXVECTOR3(i,grid[i][j],j),D3DXCOLOR(4*float(i%2)/x,.4,4*float(j%2)/z,1));
			}
		}
	}

}

void Terrain::init(ID3D10Device* device, float sscale)
{

	md3dDevice = device;
	scale = sscale;
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

	DWORD *indices = new DWORD[mNumFaces*3];//3 points per triangle
	
	int loc = 0;
	for(int i = 0; i < x-2; ++i)
	{
		for(int j = 0; j < z-2; ++j)
		{
			//int loc = (i*(z-2)+j)*3*4;
			int a = i*(z-1)+j;
			int b = i*(z-1)+j+1;
			int d = i*(z-1)+j+(z-1);
			int c = i*(z-1)+j+(z-1)+1;

			indices[loc++] = a;
			indices[loc++] = b;
			indices[loc++] = c;
			/*
			indices[loc++] = a;
			indices[loc++] = b;
			indices[loc++] = d;
			*/
			indices[loc++] = a;
			indices[loc++] = c;
			indices[loc++] = d;
			/*
			indices[loc++] = b;
			indices[loc++] = c;
			indices[loc++] = d;
			*/
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
	delete[] indices;
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
