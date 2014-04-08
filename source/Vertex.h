#ifndef VERTEX_H
#define VERTEX_H

#include "d3dUtil.h"
#include "constants.h"
struct Vertex
{
	Vertex(Vector3 pos = ZERO, Color col = BLACK):
		pos(pos), normal(Vector3(0,1,0)), texC(0,0){}
	
	Vertex(float x, float y, float z, 
		float nx, float ny, float nz, 
		float u, float v)
		: pos(x,y,z), normal(nx,ny,nz), texC(u,v){}
	Vertex(Vector3 position, Vector3 center, float u, float v)
	{
		pos = position;
		normal = position - center;
		D3DXVec3Normalize(&normal,&normal);
		texC.x = u;
		texC.y = v;
	}
		
	Vector3 pos;
	Vector3 normal;
	D3DXVECTOR2 texC;
};

#endif // VERTEX_H