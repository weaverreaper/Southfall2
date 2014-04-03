#include "Player.h"

void Player::update(float dt)
{
	if(input->anyKeyPressed())
	{
		Vector3 dir = *target - position;
		dir.y = 0;
		D3DXVec3Normalize(&dir, &dir);		
		
		if (input->isKeyDown('W')){ position += 50*dt*dir; *target += 10*dt*dir; }	
		if (input->isKeyDown('S')){ position -= 50*dt*dir; *target -= 10*dt*dir; }
		if (input->isKeyDown('A'))
		{
			Matrix rot;
			RotateY(&rot, ToRadian(-90));
			D3DXVECTOR4 out;
			D3DXVec3Transform(&out, &dir, &rot);
			position += 50*dt*Vector3(out);
			*target += Vector3(out);			
		}
		if (input->isKeyDown('D'))
		{
			Matrix rot;
			RotateY(&rot, ToRadian(90));
			D3DXVECTOR4 out;
			D3DXVec3Transform(&out, &dir, &rot);
			position += 50*dt*Vector3(out);
			*target += Vector3(out);
		}
	}

	Entity::update(dt);
}