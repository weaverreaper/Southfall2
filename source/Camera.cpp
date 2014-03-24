#include "Camera.h"
#include <math.h>

void Camera::update(float dt)
{
	if(input->anyKeyPressed())
	{
		Vector3 dir = target - position;
		dir.y = 0;
		D3DXVec3Normalize(&dir, &dir);		
		
		if (input->isKeyDown('W')){ position += dir; target += dir; }	
		if (input->isKeyDown('S')){ position -= dir; target -= dir; }
		if (input->isKeyDown('A'))
		{
			Matrix rot;
			RotateY(&rot, ToRadian(-90));
			D3DXVECTOR4 out;
			D3DXVec3Transform(&out, &dir, &rot);
			position += Vector3(out);
			target += Vector3(out);
			
		}
		if (input->isKeyDown('D'))
		{
			Matrix rot;
			RotateY(&rot, ToRadian(90));
			D3DXVECTOR4 out;
			D3DXVec3Transform(&out, &dir, &rot);
			position += Vector3(out);
			target += Vector3(out);

		}
	}

	//Restrict angles	
	if( mPhi > 2*PI) mPhi -= 2*PI; 
	if (mTheta < 0)  mTheta += 2*PI;	

	mPhi += input->getMouseRawY()*dt*sensitivity;
	mTheta -= input->getMouseRawX()*dt*sensitivity;	

 	float x =  lookRadius * sinf(mPhi)*sinf(mTheta);
	float y =  lookRadius * cosf(mPhi);
	float z = -lookRadius * sinf(mPhi)*cosf(mTheta);

	target = Vector3(x,y,z);
}