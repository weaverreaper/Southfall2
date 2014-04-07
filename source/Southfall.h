#ifndef __SOUTHFALL_H
#define __SOUTHFALL_H

#include "d3dApp.h"
#include "Box.h"
#include "Line.h"
#include "Origin.h"
#include "Pyramid.h"
#include "Geometry.h"
#include "textDX.h"
#include "audio.h"
#include "input.h"
#include "Square.h"
#include "enemy.h"
#include "Goblin.h"
#include <sstream>
#include "Camera.h"
#include "Terrain.h"
#include "Lighting.h"
#include "fireshaderclass.h"
#include "modelclass.h"
#include "Surroundings.h"
#include "Goblinbody.h"
#include "Goblinhead.h"

	const int LEVELS = 2;

//Global light and shader variables
	ID3D10EffectMatrixVariable* mfxWorldVar;
	ID3D10EffectVariable* mfxEyePosVar;
	ID3D10EffectVariable* mfxLightVar;
	ID3D10ShaderResourceView* mDiffuseMapRV[LEVELS];
	ID3D10ShaderResourceView* mSpecMapRV;
	ID3D10ShaderResourceView* mSplashTextureRV;
	ID3D10ShaderResourceView* mGoblinSkinTextureRV;
	ID3D10EffectShaderResourceVariable* mfxDiffuseMapVar;
	ID3D10EffectShaderResourceVariable* mfxSpecMapVar;
	ID3D10EffectMatrixVariable* mfxTexMtxVar;
	
	enum 
	{
		SPLASH1,
		CUT1,
		CUT2,
		LEVEL1,
		LEVEL2,
		END
	};

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
	void setShaderVals(); //called in draw, for modularity
	void renderFire();
	
	int gameState;
	float lastFrameTime;

	Square splash;
	GeoObject splashObj;

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

	LightingManager lights;

	float mTheta;
	float mPhi;

	float startCut1;
	float startCut2;
	int alpha;

	int level;
	Terrain terrain[LEVELS];
	GeoObject terrainObj[LEVELS];
	
	Surroundings surr[LEVELS];

	GoblinHead head;
	GoblinBody body;
	Goblin goblin;
	Box fireball;
	Fireball fireballObj;
};

#endif