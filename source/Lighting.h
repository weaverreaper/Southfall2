//=======================================================================================
// Light.h by Frank Luna (C) 2008 All Rights Reserved.
//=======================================================================================

#ifndef LIGHT_H
#define LIGHT_H

#include "d3dUtil.h"
#include "constants.h"

//Defines indices and count of light array
enum
{
	AMBIENT_DIFFUSE=0,
	POINT1,

	//Accurate if last
	LIGHT_COUNT
};

struct Light
{
	Light()
	{
		ZeroMemory(this, sizeof(Light));
	}

	D3DXVECTOR3 pos; float pad1;
	D3DXVECTOR3 dir; float pad2;
	D3DXCOLOR ambient;
	D3DXCOLOR diffuse;
	D3DXCOLOR specular;
	D3DXVECTOR3 att;
	float range;	
};

class LightingManager
{
public:
	LightingManager(){}
	~LightingManager(){}

	void init();
	
	Light lights[LIGHT_COUNT];
};

#endif // LIGHT_H