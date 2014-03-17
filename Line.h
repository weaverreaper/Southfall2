//=======================================================================================
// Line.h
//=======================================================================================

#ifndef LINE_H
#define LINE_H

//#include "C:\Users\weaverem1\Dropbox\S14\Games2\PartII\Common\d3dUtil.h"
#include "d3dUtil.h"
#include "Geometry.h"

class Line : public Geometry
{
public:
	Line();
	~Line();
	void init(ID3D10Device* device, float scale = 1);
	void draw();
	void setVert1(Vertex v){vertices[0]=v;
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
	HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mVB));}

	void setVert2(Vertex v){vertices[1]=v;
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
	HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mVB));}
	Vertex getPoint(){return vertices[0];}
	void setColor(D3DXCOLOR c) {return;}
private:
	static const DWORD mNumVertices = 2;
	static const DWORD mNumFaces = 0;
	Vertex vertices[mNumVertices];

	ID3D10Device* md3dDevice;
	ID3D10Buffer* mVB;
	ID3D10Buffer* mIB;
};

#endif // LINE_H
