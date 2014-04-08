
#ifndef GEOMETRY_H
#define GEOMETRY_H

//#include "C:\Users\weaverem1\Dropbox\S14\Games2\PartII\Common\d3dUtil.h"
#include "d3dUtil.h"
#include "Vertex.h"
#include <math.h>
class Geometry
{
public:

	//Geometry();
	//~Geometry();

	virtual void init(ID3D10Device* device, float scale = 1) = 0;
	virtual void draw() = 0;
	virtual void setVert1(Vertex v) = 0;
	virtual void setVert2(Vertex v) = 0;
	virtual Vertex getPoint() = 0;
	virtual void setColor(D3DXCOLOR c) = 0;

//private:
	/*
	DWORD mNumVertices;
	DWORD mNumFaces;

	ID3D10Device* md3dDevice;
	ID3D10Buffer* mVB;
	ID3D10Buffer* mIB;
	*/
};


#endif 
