#ifndef _CAMERA_H
#define _CAMERA_H

#include "d3dUtil.h"
#include "Terrain.h"
#include "input.h"
#include <math.h>

class Camera
{
public:
	Camera(){};

	~Camera(){};

	void init(Vector3 pos, Vector3 tar, Input* i, D3DXMATRIX* view, Terrain* t, float sens = .5);

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