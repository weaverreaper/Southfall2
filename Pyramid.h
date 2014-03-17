
#ifndef PYRAMID_H
#define PYRAMID_H

//#include "C:\Users\weaverem1\Dropbox\S14\Games2\PartII\Common\d3dUtil.h"
#include "d3dUtil.h"
#include "Geometry.h"

class Pyramid : public Geometry
{
public:

	Pyramid();
	Pyramid(D3DXCOLOR col1,D3DXCOLOR col2,D3DXCOLOR col3,D3DXCOLOR col4,D3DXCOLOR col5);
	~Pyramid();

	void init(ID3D10Device* device, float scale = 1);
	void draw();
	Vertex getPoint(){return vertices[0];}
	void setVert1(Vertex v){vertices[0]=v;}
	void setVert2(Vertex v){vertices[0]=v;}
	void setColor(D3DXCOLOR c);
private:
	static const DWORD mNumVertices = 5;
	static const DWORD mNumFaces = 6;
	Vertex vertices[mNumVertices];

	ID3D10Device* md3dDevice;
	ID3D10Buffer* mVB;
	ID3D10Buffer* mIB;
};


#endif 
