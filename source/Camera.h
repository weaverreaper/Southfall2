#ifndef _CAMERA_H
#define _CAMERA_H

#include "d3dUtil.h"
#include "input.h"

class Camera
{
public:
	Camera():	mTheta(2*PI), mPhi(PI/2), sensitivity(.5), 
				target(Vector3(0.f,0.f,0.f)), 
				position(Vector3(0.f, 0.f, -10.f)),
				up(Vector3(0.0f, 1.0f, 0.0f))
			{
				Vector3 temp(target - position);
				lookRadius = D3DXVec3Length(&temp);
			}

	~Camera(){};

	void init(Input* i){input = i;}

	void update(float dt);
	void set(Matrix* mView){D3DXMatrixLookAtLH(mView, &position, &target, &up);}
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
};

#endif