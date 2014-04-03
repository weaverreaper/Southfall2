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
	FIREBALL1,
	FIREBALL2,
	FIREBALL3,
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

const float MAX_DIST =  1000.f;
const float FIREBALL_SPEED = 1000.f;

class LightingManager
{
public:
	LightingManager()
	{
		dist[0] = dist[1] = dist[2] = 0;

		lights[AMBIENT_DIFFUSE].ambient	 = Color(.5,.5,.5,1);
		lights[AMBIENT_DIFFUSE].diffuse	 = Color(.6f, .75f, .6f, 1.f);
		lights[AMBIENT_DIFFUSE].dir		 = Vector3(0,1,0);	
		
		lights[POINT1].diffuse	= Color(.6f,.2f,.2f,1.f);		
		lights[POINT1].pos		= Vector3(0.f,100.f,0.f);	
		lights[POINT1].att		= Vector3(.25f,0.f,0.f);
		lights[POINT1].range	= 106.f;

		lights[POINT2].diffuse	= Color(1.f,0.f,0.f,1.f);		
		lights[POINT2].pos		= Vector3(0.f,40.f,40.f);	
		lights[POINT2].att		= Vector3(0.f,.25f,0.f);
		lights[POINT2].range	= 1000.f;

		lights[FIREBALL1].diffuse	= Color(1.f,0.8039f,0.3922f,1.f);		
		lights[FIREBALL1].att		= Vector3(0,.1,0);
		lights[FIREBALL1].range		= 1000.f;
		lights[FIREBALL1].on		= 0;		

		lights[FIREBALL2].diffuse	= Color(1.f,0.8039f,0.3922f,1.f);		
		lights[FIREBALL2].att		= Vector3(0,.1,0);
		lights[FIREBALL2].range		= 1000.f;
		lights[FIREBALL2].on		= 0;

		lights[FIREBALL3].diffuse	= Color(1.f,0.8039f,0.3922f,1.f);		
		lights[FIREBALL3].att		= Vector3(0,.1,0);
		lights[FIREBALL3].range		= 1000.f;
		lights[FIREBALL3].on		= 0;
	}

	void update(float dt) 
	{ 
		for (int i=FIREBALL1; i<= FIREBALL3; i++)
		{
			if (lights[i].on){
				lights[i].pos += FIREBALL_SPEED * dt * lights[i].dir;
				dist[i-FIREBALL1] += FIREBALL_SPEED * dt;
				if (dist[i-FIREBALL1] > MAX_DIST)
				{
					lights[i].on = 0;
					dist[i-FIREBALL1] = 0;
				}
			}
		}
	}
	

	float dist[3];	
	Light lights[LIGHT_COUNT];
};

#endif // LIGHT_H