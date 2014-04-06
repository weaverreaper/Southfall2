//=======================================================================================
// Square.h
//=======================================================================================

#ifndef Square_H
#define Square_H

#include "d3dUtil.h"
#include "Geometry.h"

class Square : public Geometry
{
public:
	Square(int imgNum = 2);
	~Square();
	void init(ID3D10Device* device, float scale = 1);
	void draw();
	Vertex getPoint(){return vertices[0];}
	void setVert1(Vertex v){vertices[0]=v;}
	void setVert2(Vertex v){vertices[1]=v;}
	void setColor(D3DXCOLOR c) {return;}
	
private:
	static const DWORD mNumVertices = 4;
	static const DWORD mNumFaces = 2;
	Vertex vertices[mNumVertices];

	ID3D10Device* md3dDevice;
	ID3D10Buffer* mVB;
	ID3D10Buffer* mIB;
};

#endif