#include "Camera.h"
#include <math.h>

void Camera::update(float dt)
{
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