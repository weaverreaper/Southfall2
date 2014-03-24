#include "Camera.h"
#include <math.h>

void Camera::update(float dt)
{
	//Restrict angles	
	if( mPhi > PI) mPhi = PI; 
	if (mTheta < 0)  mTheta = 2*PI;	
	if(mPhi < 0) mPhi = 0;

	mPhi += input->getMouseRawY()*dt*sensitivity;
	mTheta -= input->getMouseRawX()*dt*sensitivity;	
	//SetCursorPos(GAME_WIDTH/2,GAME_HEIGHT/2);
	//ShowCursor(false);

	float x =  lookRadius * sinf(mPhi)*sinf(mTheta);
	float y =  lookRadius * cosf(mPhi);
	float z = -lookRadius * sinf(mPhi)*cosf(mTheta);

	target = Vector3(x,y,z);

}