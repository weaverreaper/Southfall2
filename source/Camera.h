#ifndef _CAMERA_H
#define _CAMERA_H

#include "d3dUtil.h"
#include "constants.h"
#include "input.h"
#include "audio.h"
#include "Lighting.h"
#include "Fireball.h"

class Camera
{
public:
	Camera(){};

	~Camera(){};

	void init(Vector3 pos, Vector3 tar, Input* i, Audio* a, D3DXMATRIX* view, Terrain* t, LightingManager* l, float sens = .5);

	void update(float dt);
	
	D3DXVECTOR3 getTarget(){return target;}
	D3DXVECTOR3 getPos(){return position;}
	void setFireball(Fireball* f){fireball = f;}
	void setVelocity(Vector3 v){velocity = v;}
	void shootFireBall();

private:
	float getTerrHeight();

	Input* input;
	Audio* audio;
	float mPhi, mTheta;
	float sensitivity;
	float lookRadius;
	bool onGround;
	
	
	Vector3 velocity;
	Vector3 position;
	Vector3 target;
	Vector3 up;

	Terrain* terr;
	D3DXMATRIX* mView;
	Fireball* fireball;
	LightingManager* lights;

	float stepTime;

};

#endif