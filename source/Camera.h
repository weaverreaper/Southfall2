#ifndef _CAMERA_H
#define _CAMERA_H

#include "d3dUtil.h"
#include "Terrain.h"
#include "input.h"

class Camera
{
public:
	Camera():	mTheta(PI/2), mPhi(PI/2), sensitivity(.5), 
				target(Vector3(0.f,0.f,0.f)), 
				position(Vector3(-10.f, 0.f, 0.f)),
				up(Vector3(0.0f, 1.0f, 0.0f))
			{
				Vector3 temp(target - position);
				lookRadius = D3DXVec3Length(&temp);
			}

	~Camera(){};

	void init(Input* i, D3DXMATRIX* view, Terrain* t){input = i; mView = view; terr = t;}

	void update(float dt);
	
	D3DXVECTOR3 getTarget(){return target;}
	D3DXVECTOR3 getPos(){return position;}

private:
	Input* input;
	float mPhi, mTheta;
	float sensitivity;
	float lookRadius;
	
	Vector3 position;
	Vector3 target;
	Vector3 up;

	Terrain* terr;
	D3DXMATRIX* mView;
};

#endif