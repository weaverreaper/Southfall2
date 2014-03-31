//=======================================================================================
// Terrain.h
//=======================================================================================

#ifndef TERRAIN_H
#define TERRAIN_H

#include "d3dUtil.h"
#include "Geometry.h"
#include <fstream>

class Terrain : public Geometry
{
public:

	Terrain();
	//Terrain(D3DXCOLOR col);
	~Terrain();

	void init(ID3D10Device* device, float fakeScale = 1);
	void draw();
	Vertex getPoint(){return vertices[0];}
	void setVert1(Vertex v){vertices[0]=v;}
	void setVert2(Vertex v){vertices[0]=v;}
	void setColor(D3DXCOLOR c) {return;}
	
	int x, z;
	static const int scale = 20;
	int** grid;//[x][z];

private:
	DWORD mNumVertices;// = (x-1)*(z-1);
	DWORD mNumFaces;// = 4*(x-2)*(z-2);
	Vertex* vertices;//[mNumVertices];
	
	std::ifstream ifs;

	ID3D10Device* md3dDevice;
	ID3D10Buffer* mVB;
	ID3D10Buffer* mIB;
	
};


#endif // BOX_H
