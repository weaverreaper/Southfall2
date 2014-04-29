#ifndef _CAMERA_H
#define _CAMERA_H

#include "d3dUtil.h"
#include "constants.h"
#include "input.h"
#include "audio.h"
#include "Lighting.h"
#include "Fireball.h"
#include "Waves.h"

const float HEAD_HEIGHT = 70;

class Camera
{
public:
	Camera(){};

	~Camera(){};

	void init(Vector3 pos, Vector3 tar, Input* i, Audio* a, Matrix* view, Matrix* proj, Terrain* t, LightingManager* l, float sens = .5);

	void update(float dt);
	
	D3DXVECTOR3 getTarget(){return target;}
	D3DXVECTOR3 getPos(){return position;}
	float getTheta(){return mTheta;}
	Matrix getView(){return *mView;}
	Matrix getProj(){return *mProj;}
	void setFireball(Fireball* f){fireball = f;}
	void setWaves(Waves* w){waves = w;}
	void setVelocity(Vector3 v){velocity = v;}
	void shootFireBall();
	void releaseFireBall();
	void addShake(float intensity);
private:
	float getTerrHeight();

	float shakeRight, shakeUp;
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

	Matrix *mView, *mProj;

	Terrain* terr;	
	Fireball* fireball;
	LightingManager* lights;
	Waves* waves;

	float stepTime;

};

#endif