#ifndef VERTEX_H
#define VERTEX_H

#include "d3dUtil.h"
#include "constants.h"

struct Vertex
{
	Vertex(Vector3 pos = ZERO, Color col = BLACK):
		pos(pos), normal(Vector3(0,1,0)), diffuse(col), spec(Vector3(0,0,0)){}
	
	Vector3 pos;
	Vector3 normal;
	Color   diffuse;
	Color   spec; // (r, g, b, specPower);
};
 
#endif // VERTEX_H