#ifndef _CAMERA_H
#define _CAMERA_H

#include "d3dUtil.h"
#include "input.h"

class Camera
{
public:
	Camera(): mTheta(0.0f), mPhi(3.14*0.25f), sensitivity(1), lookRadius(53.8516f), 
		target(Vector3(20.0f, 0.0f, 10.0f)), position(Vector3(0.f, 10.f, -40.f)){}
	~Camera(){};

	void init(Input* i){input = i;}

	void update(float dt);
	D3DXVECTOR3 getTarget(){return target;}
	D3DXVECTOR3 getPos(){return position;}

private:
	Input* input;
	float mPhi, mTheta;
	float sensitivity;
	float lookRadius;

	D3DXVECTOR3 position;
	D3DXVECTOR3 target;
};

#endif