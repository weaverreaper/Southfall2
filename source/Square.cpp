//=======================================================================================
// Box.cpp by Frank Luna (C) 2008 All Rights Reserved.
//=======================================================================================

#include "Square.h"

//BREAK------------------------------
//
//-----------------------------------
Square::Square(int imgNum)
: md3dDevice(0), mVB(0), mIB(0)
{
	vertices[0] = Vertex(0, 0, 0, 0, -1, 0, 0, 0);
	vertices[1] = Vertex(1, 0, 0, 0, -1, 0, imgNum, 0);
	vertices[2] = Vertex(1, 0, 1, 0, -1, 0, imgNum, imgNum);
	vertices[3] = Vertex(0, 0, 1, 0, -1, 0, 0, imgNum);
}
 
Square::~Square()
{
	ReleaseCOM(mVB);
	ReleaseCOM(mIB);
}

void Square::init(ID3D10Device* device, float scale)
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
	
	DWORD in[mNumFaces*3];

	in[0]=0;in[1]=1;in[2]=2;
	in[3]=0;in[4]=2;in[5]=3;
	
	D3D10_BUFFER_DESC ibd;
    ibd.Usage = D3D10_USAGE_IMMUTABLE;
    ibd.ByteWidth = sizeof(DWORD)*mNumFaces*3;
    ibd.BindFlags = D3D10_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;
    ibd.MiscFlags = 0;
    D3D10_SUBRESOURCE_DATA iinitData;
    iinitData.pSysMem = in;
    HR(md3dDevice->CreateBuffer(&ibd, &iinitData, &mIB));

}

void Square::draw()
{
	UINT stride = sizeof(Vertex);
    UINT offset = 0;
    md3dDevice->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
	md3dDevice->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);
	md3dDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	md3dDevice->DrawIndexed(mNumFaces*3, 0, 0);
}