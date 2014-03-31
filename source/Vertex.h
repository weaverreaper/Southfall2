#ifndef VERTEX_H
#define VERTEX_H

#include "d3dUtil.h"
#include "constants.h"

struct Vertex
{
	Vertex(Vector3 pos = ZERO, Color col = BLACK):
		pos(pos), normal(Vector3(0,1,0)), diffuse(col), spec(Color(1,1,1,64)){}
	
	Vertex(float x, float y, float z, 
		float nx, float ny, float nz, 
		float u, float v)
		: pos(x,y,z), normal(nx,ny,nz), texC(u,v){}
	
	//to allow for simple default values
	Vertex(float num) 
	{
		pos = Vector3(num, num, num); 
		normal = Vector3(num, num, num); 
		diffuse = Color(num, num, num, num); 
		spec = Color(num, num, num, num); 
	}
	
	Vector3 pos;
	Vector3 normal;
	D3DXVECTOR2 texC;
	Color   diffuse;
	Color   spec; // (r, g, b, specPower);
};

#endif // VERTEX_H