//=======================================================================================
// Box.h by Frank Luna (C) 2008 All Rights Reserved.
//=======================================================================================

#ifndef BOX_H
#define BOX_H

#include "d3dUtil.h"
#include "Geometry.h"

class Box : public Geometry
{
public:

	Box();
	Box(D3DXCOLOR col1,D3DXCOLOR col2,D3DXCOLOR col3,D3DXCOLOR col4);
	~Box();

	void init(ID3D10Device* device, float scale = 1);
	void draw();
	Vertex getPoint(){return Vertex();}
	void setVert1(Vertex v){}
	void setVert2(Vertex v){}
	void setColor(D3DXCOLOR c) {return;}
private:

	DWORD mNumVertices;
	DWORD mNumFaces;	

	ID3D10Device* md3dDevice;
	ID3D10Buffer* mVB;
	ID3D10Buffer* mIB;
	
};


#endif // BOX_H
