#include "Camera.h"

void Camera::init(Vector3 pos, Vector3 tar, Input* i, D3DXMATRIX* view, float sens)
{
	position = pos;
	target = tar;
	up = Vector3(0.0f, 1.0f, 0.0f);
	sensitivity = sens;
	input = i; 
	mView = view;
			
	Vector3 temp(target - position);
	lookRadius = D3DXVec3Length(&temp);

	//initialize angles based on target position (convert Cartesian to Spherical)
	mPhi = acos(temp.y / lookRadius);
	mTheta = asin(temp.x/(lookRadius*sin(mPhi)));			
}

void Camera::update(float dt)
{
	if(input->anyKeyPressed())
	{
		Vector3 dir = target - position;
		dir.y = 0;
		D3DXVec3Normalize(&dir, &dir);		
		
		if (input->isKeyDown('W')){ position += 50*dt*dir; target += 10*dt*dir; }	
		if (input->isKeyDown('S')){ position -= 50*dt*dir; target -= 10*dt*dir; }
		if (input->isKeyDown('A'))
		{
			Matrix rot;
			RotateY(&rot, ToRadian(-90));
			D3DXVECTOR4 out;
			D3DXVec3Transform(&out, &dir, &rot);
			position += 50*dt*Vector3(out);
			target += Vector3(out);			
		}
		if (input->isKeyDown('D'))
		{
			Matrix rot;
			RotateY(&rot, ToRadian(90));
			D3DXVECTOR4 out;
			D3DXVec3Transform(&out, &dir, &rot);
			position += 50*dt*Vector3(out);
			target += Vector3(out);
		}
	}	

	mPhi += input->getMouseRawY()*dt*sensitivity;
	mTheta -= input->getMouseRawX()*dt*sensitivity;	

	//Restrict angles	
	if(mPhi > PI-.001) mPhi = PI-.001; 
	if(mPhi < 0.001) mPhi = .001;

	if (mTheta < 0)  mTheta += 2*PI;	
	else if (mTheta > 2*PI)  mTheta -= 2*PI;	
	
 	float x =  lookRadius * sinf(mPhi)*sinf(mTheta) + position.x;
	float y =  lookRadius * cosf(mPhi) + position.y;
	float z = -lookRadius * sinf(mPhi)*cosf(mTheta) + position.z;

	target = Vector3(x,y,z);

	D3DXMatrixLookAtLH(mView, &position, &target, &up);
}