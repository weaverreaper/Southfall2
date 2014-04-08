#ifndef SWORD_H
#define SWORD_H

#include "d3dUtil.h"
#include "Geometry.h"

class Sword : public Geometry
{
public:

	Sword();
	~Sword();

	void init(ID3D10Device* device, float scale = 1);
	void draw();
	Vertex getPoint(){return Vertex();}
	void setVert1(Vertex v){}
	void setVert2(Vertex v){}
	void setColor(D3DXCOLOR c) {return;}
private:

	const static DWORD mNumVertices = 12;
	const static DWORD mNumFaces = 16;	

	ID3D10Device* md3dDevice;
	ID3D10Buffer* mVB;
	ID3D10Buffer* mIB;
	
};

#endif // SWORD_H
