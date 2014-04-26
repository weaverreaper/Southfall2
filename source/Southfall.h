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
#include "modelclass.h"
#include "Surroundings.h"
#include "Sword.h"
#include "SwordObj.h"
#include "Goblinbody.h"
#include "Goblinhead.h"
#include "Bear.h"
#include "BearObj.h"
#include "Waves.h"
#include "Sky.h"
#include "TextureMgr.h"
#include "DamageSprites.h"


	const int LEVELS = 4;

//Global light and shader variables
	ID3D10EffectMatrixVariable* mfxWorldVar;
	ID3D10EffectVariable* mfxEyePosVar;
	ID3D10EffectVariable* mfxLightVar;

	ID3D10ShaderResourceView* mDiffuseMapRV[LEVELS];
	ID3D10ShaderResourceView* mSpecMapRV;
	ID3D10ShaderResourceView* mSplashTextureRV;
	ID3D10ShaderResourceView* mGoblinSkinTextureRV;
	ID3D10ShaderResourceView* mWaterMapRV;
	ID3D10ShaderResourceView* mWaterSpecMapRV;
	ID3D10ShaderResourceView* mEnvMapRV;

	ID3D10EffectShaderResourceVariable* mfxDiffuseMapVar;
	ID3D10EffectShaderResourceVariable* mfxSpecMapVar;
	ID3D10EffectMatrixVariable* mfxTexMtxVar;

	ID3D10RasterizerState* mNoCullRS;
	ID3D10BlendState* mTransparentBS;
	
	enum 
	{
		SPLASH1,
		CUT1,
		CUT2,
		LEVEL1,
		CUT3,
		LEVEL2,
		CUT4,
		LEVEL3,
		CUT5,
		LEVEL4,
		CUT6,
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

	Waves mWaves;
	D3DXVECTOR2 mWaterTexOffset;

	float mTheta;
	float mPhi;

	float startCut1;
	float startCut2;
	float startEndCut;
	int alpha;

	bool pigKilled, bearKilled, goblinsKilled, wraithKilled;

	int level;
	Terrain terrain[LEVELS];
	GeoObject terrainObj[LEVELS];
	
	Surroundings surr[LEVELS];
	Sky sky;
	TextureMgr tm;

	Sword sword;
	SwordObj swordObj;
	GoblinHead head;
	GoblinBody body;
	Goblin goblin1, goblin2, goblin3;
	Box fireball;
	Fireball fireballObj;
	Bear bearmodel;
	BearObj bear;
};

#endif