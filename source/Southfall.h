#ifndef __SOUTHFALL_H
#define __SOUTHFALL_H

#include "d3dApp.h"
#include "Box.h"
#include "Line.h"
#include "Origin.h"
#include "Pyramid.h"
#include "Geometry.h"
#include "GeoObject.h"
#include "textDX.h"
#include "audio.h"
#include "input.h"
#include "Square.h"
#include "enemy.h"
#include <sstream>
#include "Camera.h"
#include "Terrain.h"
#include "Light.h"

class Southfall : public D3DApp
{
public:
	Southfall(HINSTANCE hInstance);
	~Southfall();

	void initApp();
	void onResize();
	void updateScene(float dt);
	void drawScene(); 

private:
	void buildFX();
	void buildVertexLayouts();
	void initLights();
	
	//This tells whether the game is at menu(state 0), playing(state 1), playing and paused(state 3)
	int gameState;
	float lastFrameTime;
	float dtime;

	int score;
	Origin origin;
	GeoObject originObj;
	TextDX theText;
	
	ID3D10Effect* mFX;
	ID3D10EffectTechnique* mTech;
	ID3D10InputLayout* mVertexLayout;
	ID3D10EffectMatrixVariable* mfxWVPVar;	

	D3DXMATRIX mView;
	D3DXMATRIX mProj;
	D3DXMATRIX mWVP;

	Camera camera;

	Light light;

	float mTheta;
	float mPhi;

	Terrain terrain;
	GeoObject terrainObj,t1,t2,t3;

};

//Added for lighting
//Global seemed easiest -- thoughts?
	ID3D10EffectMatrixVariable* mfxWorldVar;
	ID3D10EffectVariable* mfxEyePosVar;
	ID3D10EffectVariable* mfxLightVar;

#endif