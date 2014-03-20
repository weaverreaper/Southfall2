#ifndef ENEMY_H
#define ENEMY_H
#include "GeoObject.h"

class Enemy: public GeoObject
{
public:
	Enemy():played(false) {}
	Enemy(float health, int red, int blue, int green): health(health), red(red), blue(blue), green(green), played(false){}

	D3DXVECTOR2 targetPos;
	float initialHealth;
	float health;

	bool played;

	float red;
	float blue;
	float green;
	void init(ID3D10EffectTechnique* t, ID3D10EffectMatrixVariable* f, Geometry* g)
	{
		geom = g;
	fxMatrix = f;
	tech = t;
	D3DXMatrixIdentity(&world);
	D3DXMatrixIdentity(&wvp);
	}
	void init(ID3D10EffectTechnique* t, ID3D10EffectMatrixVariable* f, Geometry* g, D3DXCOLOR c)
	{
			geom = g;
	fxMatrix = f;
	tech = t;
	g->setColor(c);
	D3DXMatrixIdentity(&world);
	D3DXMatrixIdentity(&wvp);
	}
	bool ready;
	bool finished;
	
	virtual void update(float dt)
	{
		if(active)
		{
			if(finished)
			{
				//rotx += dt*1;
				float spin(30-velocity.y);
				roty += dt*15*(spin > 0?spin/30.0:0);
				//rotz += dt*1;
				velocity = D3DXVECTOR3(0, velocity.y + 2*dt, 0);
			}else{
				D3DXVECTOR2 pos2d(position.x, position.z);
				D3DXVECTOR2 dist = targetPos - pos2d;
				if(D3DXVec2Length(&dist) < 0.1)
				{
					velocity = D3DXVECTOR3(0,0,0);
					ready = true;
				}else{
					D3DXVec2Normalize(&dist, &dist);
					dist *= 2;
					setVelocity(D3DXVECTOR3(dist.x, 0, dist.y));
				}
			}
			GeoObject::update(dt);
		}
	}



};




#endif