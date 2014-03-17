
#include "Pyramid.h"

Pyramid::Pyramid()
: md3dDevice(0), mVB(0), mIB(0)
{
	vertices[0] = Vertex(D3DXVECTOR3(+1, 0, -1), D3DXCOLOR(0,.5,0,.5));
	vertices[1] = Vertex(D3DXVECTOR3(+1, 0, +1), D3DXCOLOR(.5,0,0,.5));
	vertices[2] = Vertex(D3DXVECTOR3(-1, 0, +1), D3DXCOLOR(0,.5,0,.5));
	vertices[3] = Vertex(D3DXVECTOR3(-1, 0, -1), D3DXCOLOR(0,0,.5,.5));
	vertices[4] = Vertex(D3DXVECTOR3(+0, 2, +0), D3DXCOLOR(.25,.25,.25,1));
}
 
Pyramid::Pyramid(D3DXCOLOR col1,D3DXCOLOR col2,D3DXCOLOR col3,D3DXCOLOR col4,D3DXCOLOR col5)
: md3dDevice(0), mVB(0), mIB(0)
{
	vertices[0] = Vertex(D3DXVECTOR3(+1, 0, -1), col1);
	vertices[1] = Vertex(D3DXVECTOR3(+1, 0, +1), col2);
	vertices[2] = Vertex(D3DXVECTOR3(-1, 0, +1), col3);
	vertices[3] = Vertex(D3DXVECTOR3(-1, 0, -1), col4);
	vertices[4] = Vertex(D3DXVECTOR3(+0, 2, +0), col5);
}

Pyramid::~Pyramid()
{
	ReleaseCOM(mVB);
	ReleaseCOM(mIB);
}

void Pyramid::init(ID3D10Device* device, float scale)
{
	md3dDevice = device;

	// Scale the Pyramid.
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

	// Create the index buffer

	DWORD indices[] = {
		0,4,1,
		1,4,2,
		2,4,3,
		3,4,0,
		0,1,2,
		0,2,3
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

void Pyramid::setColor(D3DXCOLOR c)
{
	vertices[0] = Vertex(D3DXVECTOR3(+1, 0, -1), c);
	vertices[1] = Vertex(D3DXVECTOR3(+1, 0, +1), c);
	vertices[2] = Vertex(D3DXVECTOR3(-1, 0, +1), c);
	vertices[3] = Vertex(D3DXVECTOR3(-1, 0, -1), c);
	vertices[4] = Vertex(D3DXVECTOR3(+0, 2, +0), c);
	for(DWORD i = 0; i < mNumVertices; ++i)
		vertices[i].pos *= 1;
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

void Pyramid::draw()
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
	md3dDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	md3dDevice->DrawIndexed(mNumFaces*3, 0, 0);
}
//BREAK------------------------------
//
//-----------------------------------
