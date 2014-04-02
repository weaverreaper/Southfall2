#include "Camera.h"

const float HEAD_HEIGHT = 70;
const float speed = 300;
const float yAcc = -500;
const float termYVel = -1000;

float findHeight(float z3, float z1, float z2, float a, float b);

void Camera::init(Vector3 pos, Vector3 tar, Input* i, D3DXMATRIX* view, Terrain* t, float sens)
{
	position = pos;
	target = tar;
	up = Vector3(0.0f, 1.0f, 0.0f);
	sensitivity = sens;
	input = i; 
	mView = view;
	terr = t;
	onGround = true;
	velocity = Vector3(0,0,0);

	Vector3 temp(target - position);
	lookRadius = D3DXVec3Length(&temp);

	//initialize angles based on target position (convert Cartesian to Spherical)
	mPhi = acos(temp.y / lookRadius);
	mTheta = asin(temp.x/(lookRadius*sin(mPhi)));			
}

void Camera::update(float dt)
{
	if(input->anyKeyPressed() && onGround)
	{
		Vector3 dir = target - position;
		dir.y = 0;

		Vector3 dpos = Vector3(0,0,0);
		if (input->isKeyDown('W'))
			dpos += dir;
		else if (input->isKeyDown('S'))
			dpos -= dir;
		
		if (input->isKeyDown(VK_SPACE))
		{
			velocity.y = 300;
		}
		if (input->isKeyDown('A'))
		{
			Matrix rot;
			RotateY(&rot, ToRadian(-90));
			D3DXVECTOR4 out;
			D3DXVec3Transform(&out, &dir, &rot);
			dpos += Vector3(out);	
		}
		else if (input->isKeyDown('D'))
		{
			Matrix rot;
			RotateY(&rot, ToRadian(90));
			D3DXVECTOR4 out;
			D3DXVec3Transform(&out, &dir, &rot);
			dpos += Vector3(out);
		}
		
		D3DXVec3Normalize(&dpos, &dpos);
		velocity.x = 0;
		velocity.z = 0;
		velocity += speed*dpos;
	}	

	position += velocity*dt;
	velocity.y += yAcc*dt;
	if(velocity.y < termYVel)
		velocity.y = termYVel;

	mPhi += input->getMouseRawY()*dt*sensitivity;
	mTheta -= input->getMouseRawX()*dt*sensitivity;	

	//Restrict angles	
	if(mPhi > PI-.001) mPhi = PI-.001; 
	if(mPhi < 0.001) mPhi = .001;

	if (mTheta < 0)  mTheta += 2*PI;	
	else if (mTheta > 2*PI)  mTheta -= 2*PI;	
	
	
	if(position.x > 0 && position.x < (terr->x-1)*terr->scale && position.z > 0 && position.z < (terr->z-1)*terr->scale) 
	{
		onGround = false;
		float terrY = getTerrHeight();
		if(position.y < terrY*terr->scale + HEAD_HEIGHT)
		{
			velocity = Vector3(0,0,0);
			onGround = true;
			position.y = terrY*terr->scale + HEAD_HEIGHT;
		}
	}
	else
		onGround = true;

 	float x =  lookRadius * sinf(mPhi)*sinf(mTheta) + position.x;
	float y =  lookRadius * cosf(mPhi) + position.y;
	float z = -lookRadius * sinf(mPhi)*cosf(mTheta) + position.z;

	target = Vector3(x,y,z);

	D3DXMatrixLookAtLH(mView, &position, &target, &up);
}

float findHeight(float z3, float z1, float z2, float a, float b)
{
	float t = z1*(1-a/(1-b))+z2*a/(1-b);
	return t*(1-b)+z3*b;
}

float Camera::getTerrHeight()
{
	float y = 0;
	int tx = int(position.x/terr->scale);
	int tz = int(position.z/terr->scale);
	float rx = position.x/terr->scale - tx;
	float rz = position.z/terr->scale - tz;
	if(rx + rz <= 1 && rx > rz)
		y = max(findHeight(terr->grid[tx][tz+1],terr->grid[tx][tz],terr->grid[tx+1][tz],rx,rz), 
			findHeight(terr->grid[tx][tz],terr->grid[tx+1][tz],terr->grid[tx+1][tz+1],rz,1-rx));
	else if(rx + rz > 1 && rx > rz)
		y = max(findHeight(terr->grid[tx][tz],terr->grid[tx+1][tz],terr->grid[tx+1][tz+1],rz,1-rx), 
			findHeight(terr->grid[tx+1][tz],terr->grid[tx+1][tz+1],terr->grid[tx][tz+1],1-rx,1-rz));
	else if(rx + rz > 1 && rx <= rz)
		y = max(findHeight(terr->grid[tx+1][tz],terr->grid[tx+1][tz+1],terr->grid[tx][tz+1],1-rx,1-rz), 
			findHeight(terr->grid[tx+1][tz+1],terr->grid[tx][tz+1],terr->grid[tx][tz],1-rz,rx));
	else if(rx + rz <= 1 && rx <= rz)
		y = max(findHeight(terr->grid[tx+1][tz+1],terr->grid[tx][tz+1],terr->grid[tx][tz],1-rz,rx), 
			findHeight(terr->grid[tx][tz+1],terr->grid[tx][tz],terr->grid[tx+1][tz],rx,rz));
	return y;

}