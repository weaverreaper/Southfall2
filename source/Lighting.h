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
	POINT2,
	POINT3,
	POINT4,
	FIREBALL,

	//Accurate if last
	LIGHT_COUNT
};

struct Light
{
	Light(){ ZeroMemory(this, sizeof(Light)); on = 1;}

	D3DXVECTOR3 pos; float pad1;
	//int on;
	D3DXVECTOR3 dir; float pad2;
	D3DXCOLOR ambient;
	D3DXCOLOR diffuse;
	D3DXCOLOR specular;
	D3DXVECTOR3 att;
	float range;
	int on;
	float pad[3];
};

class LightingManager
{
public:
	LightingManager()
	{
		lights[AMBIENT_DIFFUSE].ambient	 = Color(.5,.5,.5,1);
		lights[AMBIENT_DIFFUSE].diffuse	 = Color(.7f, .9f, .7f, 1.f);
		lights[AMBIENT_DIFFUSE].dir		 = Vector3(0,-1,0);	
		
		lights[POINT1].diffuse	= Color(1.f,.2f,.2f,1.f);		
		lights[POINT1].pos		= Vector3(0.f,20.f,-20.f);	
		lights[POINT1].att		= Vector3(.0f,.03f,0.f);
		lights[POINT1].range	= 20.01f;

		lights[POINT2].diffuse	= Color(1.f,0.4039f,0.f,1.f);		
		lights[POINT2].pos		= Vector3(0.f,20.f,-20.f);	
		lights[POINT2].att		= Vector3(.0f,.03f,0.f);
		lights[POINT2].range	= 20.01f;
		
		lights[POINT3].diffuse	= Color(1.f,1.f,1.f,1.f);		
		lights[POINT3].pos		= Vector3(0.f,20.f,-20.f);	
		lights[POINT3].att		= Vector3(.0f,.03f,0.f);
		lights[POINT3].range	= 20.01f;
		
		lights[POINT4].diffuse	= Color(1.f,0,.4f,1.f);		
		lights[POINT4].pos		= Vector3(0.f,20.f,-20.f);	
		lights[POINT4].att		= Vector3(.0f,.03f,0.f);
		lights[POINT4].range	= 20.01f;		


		lights[FIREBALL].diffuse	= Color(1.f,0.8039f,0.3922f,1.f);		
		lights[FIREBALL].att		= Vector3(0,.03,0);
		lights[FIREBALL].range		= 1000.f;
		lights[FIREBALL].on			= 0;		

	}	

	Light lights[LIGHT_COUNT];
};

#endif // LIGHT_H