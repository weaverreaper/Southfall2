

#ifndef VERTEX_H
#define VERTEX_H

//#include "C:\Users\weaverem1\Dropbox\S14\Games2\PartII\Common\d3dUtil.h"
#include "d3dUtil.h"

struct Vertex
{
	Vertex(D3DXVECTOR3 pos = D3DXVECTOR3(0,0,0),	D3DXCOLOR color = BLACK):pos(pos),color(color){}
	
	D3DXVECTOR3 pos;
	D3DXCOLOR   color;
};
 
#endif // VERTEX_H