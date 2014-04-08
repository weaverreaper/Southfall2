//=======================================================================================
// Goblin.h by Nate Higgins
//=======================================================================================

#ifndef GOBLINBODY_H
#define GOBLINBODY_H

//#include "C:\Users\weaverem1\Dropbox\S14\Games2\PartII\Common\d3dUtil.h"
#include "d3dUtil.h"
#include "Geometry.h"

class GoblinBody : public Geometry
{
public:

	GoblinBody();
	~GoblinBody();

	void init(ID3D10Device* device, float scale = 1);
	void draw();
	Vertex getPoint(){return vertices[0];}
	void setVert1(Vertex v){vertices[0]=v;}
	void setVert2(Vertex v){vertices[0]=v;}
	void setColor(D3DXCOLOR c) {return;}
private:

	static const DWORD mNumVertices = 31;
	static const DWORD mNumFaces = 44;
	Vertex vertices[mNumVertices];

	void redoBuffers(DWORD indices[44*3]);

	ID3D10Device* md3dDevice;
	ID3D10Buffer* mVB;
	ID3D10Buffer* mIB;
	
};


#endif // BOX_H
