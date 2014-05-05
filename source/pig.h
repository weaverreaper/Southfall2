//=======================================================================================
// Goblin.h by Nate Higgins
//=======================================================================================

#ifndef pig_H
#define pig_H

//#include "C:\Users\weaverem1\Dropbox\S14\Games2\PartII\Common\d3dUtil.h"
#include "d3dUtil.h"
#include "Geometry.h"

class pig : public Geometry
{
public:

	pig();
	~pig();

	void init(ID3D10Device* device, float scale = 1);
	void draw();
	Vertex getPoint(){return vertices[0];}
	void setVert1(Vertex v){vertices[0]=v;}
	void setVert2(Vertex v){vertices[0]=v;}
	void setColor(D3DXCOLOR c) {return;}
private:

	void redoBuffers(DWORD ind[300]);

	static const DWORD mNumVertices = 60;
	static const DWORD mNumFaces = 100;
	Vertex vertices[mNumVertices];

	ID3D10Device* md3dDevice;
	ID3D10Buffer* mVB;
	ID3D10Buffer* mIB;
	
};


#endif // BOX_H
