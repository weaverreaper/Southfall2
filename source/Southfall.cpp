//=============================================================================
// Color Cube App.cpp by Frank Luna (C) 2008 All Rights Reserved.
//
// Demonstrates coloring.
//
// Controls:
//		'A'/'D'/'W'/'S' - Rotate 
//
//=============================================================================

//Special thanks to this guy for the Pokemon music: http://www.wimp.com/pokemonpiano/

#include "Southfall.h"
#include <ctime>

const static float delta = .000001f;

float HEALTH = 1;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
				   PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif


	Southfall theApp(hInstance);
	
	theApp.initApp();

	return theApp.run();
}

Southfall::Southfall(HINSTANCE hInstance)
: D3DApp(hInstance), mFX(0), mTech(0), mVertexLayout(0),
mfxWVPVar(0), mTheta(0.0f), mPhi(PI*0.25f), lastFrameTime(0), gameState(0)
{
	D3DXMatrixIdentity(&mView);
	D3DXMatrixIdentity(&mProj);
	D3DXMatrixIdentity(&mWVP); 
}

Southfall::~Southfall()
{
	if( md3dDevice )
		md3dDevice->ClearState();

	DamageSprites::releaseStatics();

	ReleaseCOM(mFX);
	ReleaseCOM(mVertexLayout);
	ReleaseCOM(mNoCullRS);
	ReleaseCOM(mTransparentBS);	
	ReleaseCOM(mSpecMapRV);
	ReleaseCOM(mSplashTextureRV);
	ReleaseCOM(mGoblinSkinTextureRV);
	ReleaseCOM(mWaterMapRV);

	for (int i=0; i<LEVELS; i++) ReleaseCOM(mDiffuseMapRV[i]);
}

void Southfall::initApp()
{
	D3DApp::initApp();	

	buildFX();
	buildVertexLayouts();

	DamageSprites::buildShaderResourceView(md3dDevice);
	DamageSprites::buildFX();

	birm = false;

	if(theText.initialize(md3dDevice, 18, true, false, "Arial") == false)
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing DirectX font"));

	endLevel = false;

	splash = Square(1);
	splash.init(md3dDevice,5);
	splashObj.init(mTech, mfxWVPVar, mfxWorldVar, &splash, Vertex(), Vertex());
	
	Matrix tm1;
	Identity(&tm1);

	D3DXMatrixRotationYawPitchRoll(&tm1, PI/2, 0, PI/2); 
	splashObj.setWorldMatrix(tm1);
	
	fireball.init(md3dDevice, 1);
	fireball2.init(md3dDevice, 1);
	fireball3.init(md3dDevice, 1);
	fireball4.init(md3dDevice, 1);
	fireballObj.setDevice(md3dDevice); fireballObj.setMFX(mFX);
	fireballObj.init(mTech, mfxWVPVar, mfxWorldVar, &camera, &fireball);
	fireballObj.setInActive();
	fireballObj.setLight(&lights.lights[FIREBALL]);	

	torch1Fireball.setDevice(md3dDevice); torch1Fireball.setMFX(mFX);
	torch1Fireball.init(mTech, mfxWVPVar, mfxWorldVar, &camera, &fireball2, 100);
	torch1Fireball.setLight(&lights.lights[FIREBALL2]);
	torch1Fireball.setInActive();

	torch.init(md3dDevice, 10);
	torch2.init(md3dDevice, 10);
	torchObj1.setDevice(md3dDevice); torchObj1.setMFX(mFX);
	torchObj1.init(mTech, mfxWVPVar, mfxWorldVar, &torch1Fireball, &fireballObj, &torch, &audio, Vector3(1400,175,2500));

	torch2Fireball.setDevice(md3dDevice); torch2Fireball.setMFX(mFX);
	torch2Fireball.init(mTech, mfxWVPVar, mfxWorldVar, &camera, &fireball3, 100);	
	torch2Fireball.setLight(&lights.lights[FIREBALL3]);
	wraithfireball.setDevice(md3dDevice); wraithfireball.setMFX(mFX);
	wraithfireball.init(mTech, mfxWVPVar, mfxWorldVar, &camera, &fireball4, 100);	
	wraithfireball.setLight(&lights.lights[FIREBALL3]);	
	wraithfireball.maxdist = 3*MAX_DIST;

	torchObj2.setDevice(md3dDevice); torchObj2.setMFX(mFX);
	torchObj2.init(mTech, mfxWVPVar, mfxWorldVar, &torch2Fireball, &fireballObj, &torch2, &audio, Vector3(1550,175,2500));
	
	head.init(md3dDevice, 5);
	body.init(md3dDevice, 5);
	bearmodel.init(md3dDevice, 5);
	pigmodel.init(md3dDevice, 5);
	wraithmodel.init(md3dDevice, 5);
	mfxDiffuseMapVar = mFX->GetVariableByName("gDiffuseMap")->AsShaderResource();
	mfxSpecMapVar    = mFX->GetVariableByName("gSpecMap")->AsShaderResource();
	mfxTexMtxVar     = mFX->GetVariableByName("gTexMtx")->AsMatrix();

	sword.init(md3dDevice,.55);
	swordObj.setDevice(md3dDevice); swordObj.setMFX(mFX);
	swordObj.init(mTech, mfxWVPVar, mfxWorldVar, &sword, Vertex(), Vertex());
	swordObj.setActive();

	bloodSquare = Square(1, true);
	bloodSquare.init(md3dDevice, 1);
	blood.setDevice(md3dDevice); blood.setMFX(mFX);
	blood.init(mTech, mfxWVPVar, mfxWorldVar, &bloodSquare, Vertex(), Vertex());
	blood.setActive();
	
	level = 0;
	pigKilled = false;
	bearKilled = false;
	goblinsKilled = false;
	wraithKilled = false;

	camera.init(Vector3(400,20,10), Vector3(400,50,200), &input, &audio, &mView, &mProj, &terrain[level], &lights);
	camera.lockToTerrain(false);

	wraith.setMFX(mFX);
	wraith.init2(mTech,mfxWVPVar, mfxWorldVar, md3dDevice, &wraithmodel, &terrain[level]);
	bear.setMFX(mFX);
	bear.init2(mTech,mfxWVPVar, mfxWorldVar, md3dDevice, &bearmodel, &terrain[level]);
	pig.setMFX(mFX);
	pig.init2(mTech,mfxWVPVar, mfxWorldVar, md3dDevice, &pigmodel, &terrain[level]);
	goblin1.setMFX(mFX);
	goblin1.init(mTech,mfxWVPVar, mfxWorldVar, md3dDevice, &head, &body, &terrain[level]);
	goblin2.setMFX(mFX);
	goblin2.init(mTech,mfxWVPVar, mfxWorldVar, md3dDevice, &head, &body, &terrain[level]);
	goblin3.setMFX(mFX);
	goblin3.init(mTech,mfxWVPVar, mfxWorldVar, md3dDevice, &head, &body, &terrain[level]);	

	goblin1.setAudio(&audio);
	goblin2.setAudio(&audio);
	goblin3.setAudio(&audio);
	bear.setAudio(&audio);
	pig.setAudio(&audio);
	wraith.setAudio(&audio);

	camera.setFireball(&fireballObj);	

	HR(D3DX10CreateShaderResourceViewFromFile(md3dDevice, 
		L"Textures\\Ground1.jpg", 0, 0, &mDiffuseMapRV[0], 0 ));

	HR(D3DX10CreateShaderResourceViewFromFile(md3dDevice, 
		L"Textures\\Leaves5.jpg", 0, 0, &mDiffuseMapRV[1], 0 ));

	HR(D3DX10CreateShaderResourceViewFromFile(md3dDevice, 
		L"Textures\\Cloud3.jpg", 0, 0, &mDiffuseMapRV[2], 0 ));

	HR(D3DX10CreateShaderResourceViewFromFile(md3dDevice, 
		L"Textures\\Cave1.jpg", 0, 0, &mDiffuseMapRV[3], 0 ));

	HR(D3DX10CreateShaderResourceViewFromFile(md3dDevice, 
		L"Textures\\defaultspec.dds", 0, 0, &mSpecMapRV, 0 ));
	
	HR(D3DX10CreateShaderResourceViewFromFile(md3dDevice, 
		L"Textures\\Title.png", 0, 0, &mSplashTextureRV, 0 ));

	HR(D3DX10CreateShaderResourceViewFromFile(md3dDevice, 
		L"Textures\\water.png", 0, 0, &mWaterMapRV, 0 ));
	HR(D3DX10CreateShaderResourceViewFromFile(md3dDevice, 
		L"Textures\\water2a.dds", 0, 0, &mWaterSpecMapRV, 0 ));
	origin.init(md3dDevice, 10);
	terrain[0].initFile("Worlds/Beach.txt");
	terrain[1].initFile("Worlds/Forest.txt");
	terrain[2].initFile("Worlds/Castle.txt");
	terrain[3].initFile("Worlds/Cave.txt");
	for(int i = 0; i < LEVELS; ++i)
	{
		terrain[i].init(md3dDevice);
		terrainObj[i].init(mTech, mfxWVPVar, mfxWorldVar, &terrain[i], Vertex(), Vertex());
		terrainObj[i].setPosition(Vector3(0,0,0));

	}
	
	originObj.init(mTech, mfxWVPVar, mfxWorldVar, &origin, Vertex(), Vertex());
	goblin1.setPosition(D3DXVECTOR3(450,120,800));
	goblin1.setScale(5.0f);
	goblin2.setPosition(D3DXVECTOR3(300,120,1400));
	goblin2.setScale(5.0f);
	goblin3.setPosition(D3DXVECTOR3(600,120,1400));
	goblin3.setScale(5.0f);
	bear.setPosition(D3DXVECTOR3(450,120,1200));
	bear.setScale(5.0f);
	pig.setPosition(D3DXVECTOR3(1500,120,2200));
	pig.setScale(3.0f);
	wraith.setPosition(D3DXVECTOR3(1800,120,1800));
	wraith.setScale(7.0f);
	bear.setInActive();
	pig.setActive();
	wraith.setInActive();
	goblin1.body.setInActive();
	goblin1.head.setInActive();
	goblin2.body.setInActive();
	goblin2.head.setInActive();
	goblin3.body.setInActive();
	goblin3.head.setInActive();
	score = 0;

//// Wave stuff

	mWaterTexOffset = D3DXVECTOR2(0.0f, 0.0f);

	D3D10_RASTERIZER_DESC rsDesc;
	ZeroMemory(&rsDesc, sizeof(D3D10_RASTERIZER_DESC));
	rsDesc.FillMode = D3D10_FILL_SOLID;
	rsDesc.CullMode = D3D10_CULL_NONE;

	HR(md3dDevice->CreateRasterizerState(&rsDesc, &mNoCullRS));

	D3D10_BLEND_DESC blendDesc = {0};
	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.BlendEnable[0] = true;
	blendDesc.SrcBlend       = D3D10_BLEND_SRC_ALPHA;
	blendDesc.DestBlend      = D3D10_BLEND_INV_SRC_ALPHA;
	blendDesc.BlendOp        = D3D10_BLEND_OP_ADD;
	blendDesc.SrcBlendAlpha  = D3D10_BLEND_ONE;
	blendDesc.DestBlendAlpha = D3D10_BLEND_ZERO;
	blendDesc.BlendOpAlpha   = D3D10_BLEND_OP_ADD;
	blendDesc.RenderTargetWriteMask[0] = D3D10_COLOR_WRITE_ENABLE_ALL;

	HR(md3dDevice->CreateBlendState(&blendDesc, &mTransparentBS));

	int m = 300;
	mWaves.init(md3dDevice, m, m, 30.f, 0.03f, 180.25f, 0.05f, Vector3(400,25,-50));
	srand(time(0));

	// Generate some waves at start up.
	for(int k = 0; k < 30; ++k)
	{ 
		DWORD i = 5 + rand() % 250;
		DWORD j = 5 + rand() % 250;

		float r = RandF(9.5f, 12.25f);


		mWaves.disturb(i, j, r);
	}

	for(int j = 10; j < m-10; j++)
		{ 
		float r = RandF(2.5f, (m/2 - abs(int(j-m/2)))/4.f)/4;	
		mWaves.disturb(125+RandF(-1,1), j, r);
		}

/////

//// Sky stuff
	tm.init(md3dDevice);

	mEnvMapRV = tm.createCubeTex(L"Textures\\CubeMaps\\Level1Sky.dds");
	sky.init(md3dDevice, mEnvMapRV, 15000.0f);
	sky.setCamera(&camera);

////

	gameState = SPLASH1;
	audio.playCue(BAR_BACKGROUND_CUE);
	input.clearAll();
	camera.setWaves(&mWaves);

	Vector3 centers[1];

	centers[0] = Vector3(480,650,3100);
	ladder.init(md3dDevice, centers, 1, L"Textures\\ladder.png", 96, 800);
	ladder.setActive(false);
	
	centers[0] = Vector3(1500, 325 , 3000);
	portal.init(md3dDevice, centers, 1, L"Textures\\ForestPortal.png", 300, 400);
	portal.setActive(false);

	tempO.init(mTech, mfxWVPVar, mfxWorldVar, &origin, Vertex(), Vertex());
	tempO.setRadius(30);

	camera.clearShake();
}

void Southfall::onResize()
{
	D3DApp::onResize();

	float aspect = (float)mClientWidth/mClientHeight;
	D3DXMatrixPerspectiveFovLH(&mProj, 0.25f*PI, aspect, .1f, 4000.0f);//near and far clipping planes
}

void Southfall::updateScene(float dt)
{

	D3DApp::updateScene(dt);
	fireballObj.update(dt);

	if(gameState == CUT3)
		camera.setVelocity(Vector3(0, 50*sin(PI/2*mTimer.getGameTime()),125));

	camera.update(dt);

	blood.setPosition(camera.getPosShake() + (camera.getTarget()-camera.getPosShake())/camera.getRadius()*.101);
	blood.setAngles(camera.getTheta(), camera.getPhi());
	blood.update(dt);

	swordObj.setPosition(camera.getPos() - .4*Vector3(0,HEAD_HEIGHT,0));
	swordObj.setAngle(camera.getTheta());
	swordObj.update(dt);

	Matrix tm1;
	Box tembB;


	tempO.setPosition(camera.getPos());

	if(input.wasKeyPressed(VK_ESCAPE)) PostQuitMessage(0);
	if(input.wasKeyPressed('P')){ camera.incSensitivity(); input.clearKeyPress('P'); }
	if(input.wasKeyPressed('O')){ camera.decSensitivity(); input.clearKeyPress('O'); }
	
	switch (gameState)
	{
#pragma region SPLASH
	case SPLASH1:
		if (input.wasKeyPressed('B')) initBirmingham();
		if((input.wasKeyPressed(VK_SPACE)))
		{
			input.clearKeyPress(VK_SPACE);
			gameState = CUT1;


			gameState = LEVEL4;
			wraith.setActive();
			level = 3;
			camera.init(Vector3(400,100,10), Vector3(250,100,200), &input, &audio, &mView, &mProj, &terrain[level], &lights);
			wraith.init2(mTech,mfxWVPVar, mfxWorldVar, md3dDevice, &wraithmodel, &terrain[level]);

			audio.stopCue(BAR_BACKGROUND_CUE);
			startCut = mTimer.getGameTime();
			alpha = 20;	
			for (int i=POINT1; i<=POINT4; i++) lights.lights[i].on=0;
			
			lights.lights[AMBIENT_DIFFUSE].ambient	 = Color(0.1064453125, 0.1123046875, 0.1337890625,1);
			lights.lights[AMBIENT_DIFFUSE].diffuse	 = Color(0.9921, 0.9058, 0.5450, 1.f);
			lights.lights[AMBIENT_DIFFUSE].dir		 = Vector3(0,-.75,.75);	
			
			lights.lights[POINT1].pos		= Vector3(380, 600, (terrain[level].z-3)*terrain[level].scale);
			lights.lights[POINT1].diffuse	= Color(.05f,.05f,.05f,.5f);
			lights.lights[POINT1].att		= Vector3(0,.0008f,0);
			lights.lights[POINT1].range		= 65.f;

		}
		else
		{
			camera.setVelocity(Vector3(0,0,0));

			float time = mTimer.getGameTime();
			
			lights.lights[1].pos.x =  -2.97 * cosf(time)
				+ .23 * sinf(time)*sinf(time);
			
			lights.lights[1].pos.y = -1.09*sinf(time)
				+ .01 * cosf(time)*cosf(time);

			lights.lights[2].pos.x =  3.48 * cosf(time)
				- .23 * sinf(time)*sinf(time);
			
			lights.lights[2].pos.y = 6.09*sinf(time)
				+ .01 * cosf(time)*cosf(time);	

			lights.lights[3].pos.x =  2.045 + 4.09*sinf(time);
			
			lights.lights[3].pos.y = 4+ .6* cosf(time);

			lights.lights[4].pos.x =  sinf(time)
				- .23 * sinf(time)*sinf(time);
			
			lights.lights[4].pos.y = 2*cosf(time);
			
			Vector3 up(0,1,0);
			Vector3 position(2.5,2.5,-15);
			Vector3 target(2.5,2.5,0);

			position.x += 2.5*cosf(time);
			position.y -= 2.5*sinf(time);

			D3DXMatrixLookAtLH(&mView, &position, &target, &up);

		}
		
		//Speed along wave progress	
		mWaterTexOffset.y += 0.1f*dt;
		mWaterTexOffset.x = 0.25f*sinf(4.0f*mWaterTexOffset.y);

		mWaves.update(dt);
		
		break;
#pragma endregion SPLASH

#pragma region CUT1
	case CUT1:
		if (mTimer.getGameTime() - startCut > 8 || (input.wasKeyPressed(VK_SPACE)))
		{
			input.clearKeyPress(VK_SPACE);
			gameState = CUT2;
			camera.update(dt);
			startCut = mTimer.getGameTime();
			alpha = 20;
			Sleep(400);
			break;
		}
		alpha += 25*dt;
		if (alpha > 255) alpha = 255;
		if (alpha < 0) alpha = 0;

		break;
#pragma endregion CUT1

#pragma region CUT2
	case CUT2:
		if (mTimer.getGameTime() - startCut > 4 || (input.wasKeyPressed(VK_SPACE)))
		{
			input.clearKeyPress(VK_SPACE);
			gameState = CUT3;
			blood.addDamage(.35);
			//camera.init(Vector3(400,100,-1000), Vector3(400,100,200), &input, &audio, &mView, &mProj, &terrain[level], &lights);
			camera.lockToTerrain(false);
			camera.setPos(Vector3(terrain[level].x*terrain[level].scale/2,150,-2000));
			//camera.update(dt);
			audio.playCue(BEACH_CUE);
			alpha = 20;
			break;
		}
		alpha += 35*dt;
		if (alpha > 255) alpha = 255;
		if (alpha < 0) alpha = 0;

		break;
#pragma endregion CUT2

#pragma region CUT3
	case CUT3:
		//Intro "cut scene," camera bobbing and moving toward the island like on a boat
		if (camera.getPos().z >= 0 || (input.wasKeyPressed(VK_SPACE)))
		{
			input.clearKeyPress(VK_SPACE);
			camera.lockToTerrain(true);
			
			gameState = LEVEL1;
			break;
		}
		
		alpha += 25*dt;
		if (alpha > 255) alpha = 255;
		if (alpha < 0) alpha = 0;

		mWaterTexOffset.y += 0.1f*dt;
		mWaterTexOffset.x = 0.25f*sinf(4.0f*mWaterTexOffset.y);

		mWaves.update(dt);

		break;
#pragma endregion CUT3

#pragma region LEVEL1
	case LEVEL1:

		if(input.getMouseLButton())
		{
			camera.addShake(.25*(swordObj.power-1.0f)); 
			if (swordObj.swing()) audio.playCue(SWING_CUE); 
		}
		else swordObj.rising = false;
	
		pig.update(dt,camera.getPos(), &fireballObj, &swordObj);

		torchObj1.update(dt);
		torchObj2.update(dt);	

		mWaterTexOffset.y += 0.1f*dt;
		mWaterTexOffset.x = 0.25f*sinf(4.0f*mWaterTexOffset.y);
		mWaves.update(dt);
		if(pig.health <= 0){ blood.setDamage(0); score = 99; }
		if (!endLevel && torchObj1.isLit() && pig.health <= 0)
		{
			lights.lights[POINT1].on = 1;
			audio.playCue(ZELDA_CUE);
			endLevel = true;
			torchObj2.setInActive();
			portal.setActive(true);
		}

#pragma region END_LEVEL1
		if(endLevel && camera.getPos().z >= (terrain[level].z-3)*terrain[level].scale)//level done.
		{			
			++gameState;
			++level;
			audio.stopCue(BEACH_CUE);
			audio.playCue(FOREST_CUE);
			if(level >= LEVELS) level = LEVELS-1;

			camera.init(Vector3(400,100,10), Vector3(400,100,200), &input, &audio, &mView, &mProj, &terrain[level], &lights);

			bear.init2(mTech,mfxWVPVar, mfxWorldVar, md3dDevice, &bearmodel, &terrain[level]);
			goblin1.init(mTech,mfxWVPVar, mfxWorldVar, md3dDevice, &head, &body, &terrain[level]);
			goblin2.init(mTech,mfxWVPVar, mfxWorldVar, md3dDevice, &head, &body, &terrain[level]);
			goblin3.init(mTech,mfxWVPVar, mfxWorldVar, md3dDevice, &head, &body, &terrain[level]);
			goblin1.setPosition(D3DXVECTOR3(450,120,1000));
			goblin2.setPosition(D3DXVECTOR3(300,120,1400));
			goblin3.setPosition(D3DXVECTOR3(600,120,1400));
			bear.setPosition(D3DXVECTOR3(450,120,1200));
			goblin1.health = 200;
			goblin2.health = 200;
			goblin3.health = 200;
			goblin1.head.setActive();
			goblin1.body.setActive();
			goblin2.head.setActive();
			goblin2.body.setActive();
			goblin3.head.setActive();
			goblin3.body.setActive();
			bear.setActive();

			lights.lights[AMBIENT_DIFFUSE].ambient	 = Color(.4,.4,.4,1);
			lights.lights[AMBIENT_DIFFUSE].diffuse	 = Color(.6f, .75f, .6f, 1.f);
			lights.lights[AMBIENT_DIFFUSE].dir		 = Vector3(0,-1,0);	

			lights.lights[POINT1].on = 0;			

			mEnvMapRV = tm.createCubeTex(L"Textures\\CubeMaps\\Level2Forest.dds");
			sky.init(md3dDevice, mEnvMapRV, 15000.0f);

			startCut = mTimer.getGameTime();
			input.clearKeyPress(VK_SPACE);
			alpha = 20;
		}
#pragma endregion END_LEVEL1


		break;
#pragma endregion LEVEL1

#pragma region CUT4
	case CUT4:
		if (mTimer.getGameTime() - startCut > 8 || (input.wasKeyPressed(VK_SPACE)))
		{
			input.clearKeyPress(VK_SPACE);
			gameState++;
			camera.update(dt);
			alpha = 20;		
			break;
		}
		alpha += 25*dt;
		if (alpha > 255) alpha = 255;
		if (alpha < 0) alpha = 0;
		break;
#pragma endregion CUT4

#pragma region LEVEL2
	case LEVEL2:	

		if(input.getMouseLButton())
		{
			camera.addShake(.25*(swordObj.power-1.0f)); 
			if (swordObj.swing()) audio.playCue(SWING_CUE); 
		}
		else swordObj.rising = false;
		
		if(goblin1.head.getActiveState() && goblin1.collided(&tempO))
		{
			blood.addDamage(.05);
			Vector3 diff = goblin1.body.getVelocity();
			D3DXVec3Normalize(&diff, &diff);
			camera.setPos(camera.getPos() + 5*diff);
			audio.playCue(DAMAGE_CUE);
		}
		if(goblin2.head.getActiveState() && goblin2.collided(&tempO))
		{
			blood.addDamage(.05);
			Vector3 diff = goblin2.body.getVelocity();
			D3DXVec3Normalize(&diff, &diff);
			camera.setPos(camera.getPos() + 5*diff);
			audio.playCue(DAMAGE_CUE);
		}
		if(goblin3.head.getActiveState() && goblin3.collided(&tempO))
		{			
			blood.addDamage(.05);
			Vector3 diff = goblin3.body.getVelocity();
			D3DXVec3Normalize(&diff, &diff);
			camera.setPos(camera.getPos() + 5*diff);
			audio.playCue(DAMAGE_CUE);
		}
		if(bear.getActiveState() && bear.collided(&tempO))
		{
			blood.addDamage(.1);
			Vector3 diff = bear.getVelocity();
			D3DXVec3Normalize(&diff, &diff);
			camera.setPos(camera.getPos() + 5*diff);
			audio.playCue(DAMAGE_CUE);
		}
		
		goblin1.update(dt,camera.getPos(), &fireballObj, &swordObj);		
		goblin2.update(dt,camera.getPos(), &fireballObj, &swordObj);	
		goblin3.update(dt,camera.getPos(), &fireballObj, &swordObj);		
		bear.update(dt,camera.getPos(), &fireballObj, &swordObj);

		if (!bearKilled && (input.isKeyDown('B') || bear.health <= 0))
		{
			bear.health = 1;
			lights.lights[POINT1].on = 1;
			audio.playCue(ZELDA_CUE);
			bearKilled = true;				
			ladder.setActive(true);
		}

#pragma region END_LEVEL2
		if(bearKilled && camera.getPos().z >= (terrain[level].z-3)*terrain[level].scale)
		{
			++gameState;
			++level;
			camera.init(Vector3(1000,100,10), Vector3(1000,100,200), &input, &audio, &mView, &mProj, &terrain[level], &lights);
				
			audio.stopCue(FOREST_CUE);

			mEnvMapRV = tm.createCubeTex(L"Textures\\CubeMaps\\StormyDays.dds");
			sky.init(md3dDevice, mEnvMapRV, 15000.0f);
			blood.setDamage(0);

			lights.lights[AMBIENT_DIFFUSE].ambient	 = Color(.9,.9,.9,1);
			lights.lights[AMBIENT_DIFFUSE].diffuse	 = Color(.8f, .8f, .8f, 1.f);
			lights.lights[AMBIENT_DIFFUSE].dir		 = Vector3(-1,-.25,0);				

			startCut = mTimer.getGameTime();
			input.clearKeyPress(VK_SPACE);
			alpha = 20;
		}
#pragma endregion END_LEVEL2

		break;
#pragma endregion LEVEL2

#pragma region CUT5
	case CUT5:
		if (mTimer.getGameTime() - startCut > 8 || (input.wasKeyPressed(VK_SPACE)))
		{
			input.clearKeyPress(VK_SPACE);
			gameState = LEVEL3;
			camera.update(dt);
			alpha = 20;		
			audio.playCue(HEARTBEAT_CUE);
			break;
		}
		alpha += 25*dt;
		if (alpha > 255) alpha = 255;
		if (alpha < 0) alpha = 0;		
		break;
#pragma endregion CUT5

#pragma region LEVEL3
	case LEVEL3:	
		
		if(input.getMouseLButton())
		{
			camera.addShake(.25*(swordObj.power-1.0f)); 
			if (swordObj.swing()) audio.playCue(SWING_CUE); 
		}
		else swordObj.rising = false;

		if(camera.getPos().z >= (terrain[level].z-3)*terrain[level].scale)
		{
			++gameState;
			++level;
				
			camera.init(Vector3(400,100,10), Vector3(250,100,200), &input, &audio, &mView, &mProj, &terrain[level], &lights);
			
			mEnvMapRV = tm.createCubeTex(L"Textures\\CubeMaps\\Miramar.dds");
			sky.init(md3dDevice, mEnvMapRV, 15000.0f);

			wraith.init2(mTech,mfxWVPVar, mfxWorldVar, md3dDevice, &wraithmodel, &terrain[level]);
			wraith.setPosition(D3DXVECTOR3(1800,120,1800));
			wraith.setActive();

			lights.lights[AMBIENT_DIFFUSE].ambient	 = Color(.3,.3,.3,1);
			lights.lights[AMBIENT_DIFFUSE].diffuse	 = Color(.7f, .71f, .7f, 1.f);
			lights.lights[AMBIENT_DIFFUSE].dir		 = Vector3(1,-.45,0);	
			wraith.init2(mTech,mfxWVPVar, mfxWorldVar, md3dDevice, &wraithmodel, &terrain[level]);


			audio.stopCue(HEARTBEAT_CUE);
			audio.playCue(BOSS_CUE);

		}

		break;
#pragma endregion LEVEL3

#pragma region CUT6
	case CUT6:
		++gameState;
		break;
#pragma endregion CUT6

#pragma region LEVEL4
	case LEVEL4:				

		if(input.getMouseLButton())
		{
			camera.addShake(.25*(swordObj.power-1.0f)); 
			if (swordObj.swing()) audio.playCue(SWING_CUE); 
		}
		else swordObj.rising = false;

		wraith.update(dt, camera.getPos(), &fireballObj, &swordObj);	
		wraithfireball.update(dt);


		if(wraith.getActiveState() && (tempO.collided(&wraith)))
		{
			blood.addDamage(.25);	
			Vector3 diff = wraith.getVelocity();
			D3DXVec3Normalize(&diff, &diff);
			camera.setPos(camera.getPos() + 60*diff);
			audio.playCue(DAMAGE_CUE);
		}
		if(wraithfireball.getActiveState() && (tempO.collided(&wraithfireball)))
		{
			blood.addDamage(0.1);
			wraithfireball.setInActive();
		}
		if(wraith.getfiretime() <= 0)
		{
			wraithfireball.setActive();
			wraithfireball.shoot2(wraith.getPosition(),camera.getPos()-wraith.getPosition(), camera.getPos());
			wraith.setfiretime(5.0f);
		}

		if (!wraith.getActiveState())
		{			
			alpha = 0;
			++gameState;				
			startCut = mTimer.getGameTime();
			audio.stopCue(BOSS_CUE);
			audio.playCue(VICTORY_CUE);	
		}

		break;
#pragma endregion LEVEL4

#pragma region CUT7
	case CUT7:
		gameState++;
		break;

#pragma endregion CUT7

#pragma region END
	case END:		
		if (mTimer.getGameTime() - startCut > 10) PostQuitMessage(0);
		
		alpha += 80*dt;
		if (alpha > 255) alpha = 255;
		if (alpha < 0) alpha = 0;

		break;
#pragma endregion END

#pragma region LOSE
	case LOSE:		
		if (mTimer.getGameTime() - startCut > 5) PostQuitMessage(0);
		
		alpha += 80*dt;
		if (alpha > 255) alpha = 255;
		if (alpha < 0) alpha = 0;

		break;	
#pragma endregion LOSE
	}

	if (gameState != LOSE && blood.getDamage() > HEALTH)
		{
			//if(level == 3)
			audio.stopCue(BOSS_CUE);
			audio.stopCue(FOREST_CUE);

			Sleep(2000);

			blood.setDamage(0);
			--level;
			camera.setPos(Vector3(0,0,(terrain[level].z-3)*terrain[level].scale));
			endLevel = true;
			gameState -= 2;
			alpha = 0;
			updateScene(0);
			/*
			gameState = LOSE; 
			startCut = mTimer.getGameTime();
			*/
		}

	if (birm) blood.setDamage(0);
}

void Southfall::setShaderVals()
{
	// Restore default states, input layout and primitive topology 
	// because mFont->DrawText changes them.  Note that we can 
	// restore the default states by passing null.
	md3dDevice->OMSetDepthStencilState(0, 0);
	float blendFactors[] = {0.0f, 0.0f, 0.0f, 0.0f};
	md3dDevice->OMSetBlendState(0, blendFactors, 0xffffffff);
	md3dDevice->IASetInputLayout(mVertexLayout);
	
	mfxEyePosVar->SetRawValue(&camera.getPos(), 0, sizeof(D3DXVECTOR3));	
	mfxLightVar->SetRawValue(lights.lightP, 0, sizeof(Light)*LIGHT_COUNT);

	mfxDiffuseMapVar->SetResource(mDiffuseMapRV[level]);
	mfxSpecMapVar->SetResource(mSpecMapRV);

	// Don't transform texture coordinates, so just use identity transformation.
	D3DXMATRIX texMtx;
	D3DXMatrixIdentity(&texMtx);
	mfxTexMtxVar->SetMatrix((float*)&texMtx);
}

void Southfall::drawScene()
{
	D3DApp::drawScene();
	setShaderVals();
	mWVP = mView*mProj;
	std::stringstream q;
	Vector3 temp;
	D3DXMATRIX T, S;
	D3DXMATRIX waterTexMtx;
	
	switch (gameState)
	{
	case SPLASH1:
		temp = Vector3(0,0,0);
		mfxEyePosVar->SetRawValue(&temp, 0, sizeof(D3DXVECTOR3));
		mfxDiffuseMapVar->SetResource(mSplashTextureRV);
		splashObj.draw(&mWVP);
		theText.setFontColor(SETCOLOR_ARGB(255, 255,255,255));

		theText.print("Press Space to move forward",GAME_WIDTH/2 - 100,GAME_HEIGHT - 50);

		theText.print("Movement - WASD",GAME_WIDTH - 400,GAME_HEIGHT/2-50);	
		theText.print("Camera - Mouse",GAME_WIDTH - 400,GAME_HEIGHT/2-25);	
		theText.print("Adjust sensitivity - O and P",GAME_WIDTH - 400,GAME_HEIGHT/2);
		theText.print("Swing Sword - Left Click (hold to charge)",GAME_WIDTH - 400,GAME_HEIGHT/2+25);
		theText.print("Shoot Fireball - Right Click (hold to charge)",GAME_WIDTH - 400,GAME_HEIGHT/2+50);

		theText.print("Enter Birmingham Mode - B",200,GAME_HEIGHT/2);	

		break;
	case CUT1:
		theText.setFontColor(SETCOLOR_ARGB((int)alpha, 255,255,255));
		theText.print("\"Attack him where he is unprepared, appear where you are not expected.\" \n -- Sun Tzu, The Art of War",GAME_WIDTH/2 - 400,GAME_HEIGHT/2-200);		
		break;
	case CUT2:
		theText.setFontColor(SETCOLOR_ARGB((int)alpha, 255,255,255));
		theText.print("One wraith remains. \n   You approach his secret lair...",GAME_WIDTH/2 + 200,GAME_HEIGHT/2+100);		
		break;
	case CUT3: 
		terrainObj[level].draw(&mWVP);
		setShaderVals();
		sky.draw();
		setShaderVals();
		
		D3DXMatrixScaling(&S, 5.0f, 5.0f, 5.0f);
	
		// Scale and translate the texture.
		
		D3DXMatrixTranslation(&T, mWaterTexOffset.x, mWaterTexOffset.y, 0.0f);
		waterTexMtx = S*T;
		
		md3dDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		md3dDevice->IASetInputLayout(mVertexLayout);

		D3D10_TECHNIQUE_DESC techDesc0;
		mTech->GetDesc( &techDesc0 );

		for(UINT i = 0; i < techDesc0.Passes; ++i)
		{
			ID3D10EffectPass* pass = mTech->GetPassByIndex(i);

			Matrix mWavesWorld; Identity(&mWavesWorld);
			mWVP = mWavesWorld*mView*mProj;
			mfxWVPVar->SetMatrix((float*)&mWVP);
			mfxWorldVar->SetMatrix((float*)&mWavesWorld);
			mfxTexMtxVar->SetMatrix((float*)&waterTexMtx);
			mfxDiffuseMapVar->SetResource(mWaterMapRV);
			mfxSpecMapVar->SetResource(mSpecMapRV);
			pass->Apply(0);
			float blendFactor[] = {0.0f, 0.0f, 0.0f, 0.0f};			
			md3dDevice->OMSetBlendState(mTransparentBS, blendFactor, 0xffffffff);
			mWaves.draw();
		}

		lights.setNoLight();
		setShaderVals();
		blood.draw(&mWVP);
		lights.resetLight();
		setShaderVals();

		theText.setFontColor(SETCOLOR_ARGB((int)alpha, 255,255,255));
		theText.print("After weeks of sailing, \n   you espy the coast. \n You must find something to eat \nbefore starvation claims you....",GAME_WIDTH/2 + 200,GAME_HEIGHT/2-100);		
		break;

#pragma region LEVEL1
	case LEVEL1:
		terrainObj[level].draw(&mWVP);

		setShaderVals();
		sky.draw();
		setShaderVals();
		swordObj.draw(&mWVP);
		

	//Waves stuff (will NOT be put into modules ever)

		// Scale texture coordinates by 5 units to map [0,1]-->[0,5]
		// so that the texture repeats five times in each direction.	
		D3DXMatrixScaling(&S, 5.0f, 5.0f, 5.0f);
	
		// Scale and translate the texture.
		
		D3DXMatrixTranslation(&T, mWaterTexOffset.x, mWaterTexOffset.y, 0.0f);
		waterTexMtx = S*T;
		
		md3dDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		md3dDevice->IASetInputLayout(mVertexLayout);

		D3D10_TECHNIQUE_DESC techDesc;
		mTech->GetDesc( &techDesc );

		for(UINT i = 0; i < techDesc.Passes; ++i)
		{
			ID3D10EffectPass* pass = mTech->GetPassByIndex(i);

			Matrix mWavesWorld; Identity(&mWavesWorld);
			mWVP = mWavesWorld*mView*mProj;
			mfxWVPVar->SetMatrix((float*)&mWVP);
			mfxWorldVar->SetMatrix((float*)&mWavesWorld);
			mfxTexMtxVar->SetMatrix((float*)&waterTexMtx);
			mfxDiffuseMapVar->SetResource(mWaterMapRV);
			mfxSpecMapVar->SetResource(mSpecMapRV);
			pass->Apply(0);
			float blendFactor[] = {0.0f, 0.0f, 0.0f, 0.0f};			
			md3dDevice->OMSetBlendState(mTransparentBS, blendFactor, 0xffffffff);
			mWaves.draw();
		}

		if(pig.getActiveState())
		pig.draw(&mWVP);

		setShaderVals();
		portal.draw(lights.lights[0], camera.getPos(), mView*mProj);
		//md3dDevice->RSSetState(0);

		setShaderVals();
		torchObj1.draw(&mWVP);
		setShaderVals();
		torchObj2.draw(&mWVP);		
		setShaderVals();
		fireballObj.draw(&mWVP);
		setShaderVals();	

		lights.setNoLight();
		setShaderVals();
		blood.draw(&mWVP);
		lights.resetLight();
		setShaderVals();

		theText.setFontColor(SETCOLOR_ARGB(255, 255,255,255));
		q << "Bacon: " << score;
		theText.print(q.str(),0, 0);

		break;
#pragma endregion LEVEL1

	case CUT4:
		theText.setFontColor(SETCOLOR_ARGB((int)alpha, 255,255,255));
		theText.print("You enter the portal, \n and find yourself transported to a forest. \n            Everything seems calm...",GAME_WIDTH/2 - 200,GAME_HEIGHT/2);	
		break;

#pragma region LEVEL2
	case LEVEL2:
		terrainObj[level].draw(&mWVP);
		
		sky.draw();
		setShaderVals();
		goblin1.draw(&mWVP);
		setShaderVals();
		goblin2.draw(&mWVP);
		setShaderVals();
		goblin3.draw(&mWVP);
		setShaderVals();
		bear.draw(&mWVP);
		setShaderVals();
		swordObj.draw(&mWVP);		

		setShaderVals();
		ladder.draw(lights.lights[0], camera.getPos(), mView*mProj);
	
		setShaderVals();
		fireballObj.draw(&mWVP);
		
		lights.setNoLight();
		setShaderVals();
		blood.draw(&mWVP);
		lights.resetLight();
		setShaderVals();

		theText.setFontColor(SETCOLOR_ARGB(255, 255,255,255));
		q << "Bacon: " << score;
		theText.print(q.str(),0, 0);
		break;
#pragma endregion LEVEL2

	case CUT5:
		theText.setFontColor(SETCOLOR_ARGB((int)alpha, 255,255,255));
		theText.print("The ladder leads up up into the clouds. \n \"How strange.\" you wonder...",GAME_WIDTH/2 - 50,GAME_HEIGHT/2);	
		break;

#pragma region LEVEL3
	case LEVEL3:
		terrainObj[level].draw(&mWVP);
		
		sky.draw();
		setShaderVals();
		
		swordObj.draw(&mWVP);
		setShaderVals();
		fireballObj.draw(&mWVP);
		
		lights.setNoLight();
		setShaderVals();
		blood.draw(&mWVP);
		lights.resetLight();
		setShaderVals();

		theText.setFontColor(SETCOLOR_ARGB(255, 255,255,255));
		q << "Bacon: " << score;
		theText.print(q.str(),0, 0);
		break;
	case CUT6:
		break;
#pragma endregion LEVEL3

#pragma region LEVEL4
	case LEVEL4:
		terrainObj[level].draw(&mWVP);
		
		sky.draw();
		setShaderVals();
		wraith.draw(&mWVP);
		setShaderVals();
		swordObj.draw(&mWVP);
		setShaderVals();
		fireballObj.draw(&mWVP);
		setShaderVals();
		wraithfireball.draw(&mWVP);
		
		lights.setNoLight();
		setShaderVals();
		blood.draw(&mWVP);
		lights.resetLight();
		setShaderVals();
		
		theText.setFontColor(SETCOLOR_ARGB(255, 255,255,255));
		q << "Bacon: " << score;
		theText.print(q.str(),0, 0);
		break;
#pragma endregion LEVEL4

#pragma region END
	case END:
		theText.setFontColor(SETCOLOR_ARGB((int)alpha, 255,255,255));
		theText.print("The End",GAME_WIDTH/2 - 50,GAME_HEIGHT/2);	
		break;
#pragma endregion END

#pragma region LOSE
	case LOSE:
		theText.setFontColor(SETCOLOR_ARGB((int)alpha, 255,255,255));
		theText.print("...You Lose...",GAME_WIDTH/2 - 50,GAME_HEIGHT/2);	
		break;
	
#pragma endregion LOSE
	}
	/*stringstream ss;
	string bs;
	ss << blood.getDamage();
	ss >> bs;
	theText.print(bs.c_str(),GAME_WIDTH/2,GAME_HEIGHT/2);	*/
	mSwapChain->Present(0, 0);
}

void Southfall::buildFX()
{
	DWORD shaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;
/*
#if defined( DEBUG ) || defined( _DEBUG )
    shaderFlags |= D3D10_SHADER_DEBUG;
	shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif
*/
 
	ID3D10Blob* compilationErrors = 0;
	HRESULT hr = 0;
	hr = D3DX10CreateEffectFromFile(L"source\\color.fx", 0, 0, 
		"fx_4_0", shaderFlags, 0, md3dDevice, 0, 0, &mFX, &compilationErrors, 0);
	if(FAILED(hr))
	{
		if( compilationErrors )
		{
			MessageBoxA(0, (char*)compilationErrors->GetBufferPointer(), 0, 0);
			ReleaseCOM(compilationErrors);
		}
		DXTrace(__FILE__, (DWORD)__LINE__, hr, L"D3DX10CreateEffectFromFile", true);
	} 

	mTech = mFX->GetTechniqueByName("ColorTech");
	
	mfxWVPVar = mFX->GetVariableByName("gWVP")->AsMatrix();
	mfxWorldVar  = mFX->GetVariableByName("gWorld")->AsMatrix();
	mfxEyePosVar = mFX->GetVariableByName("gEyePosW");
	mfxLightVar  = mFX->GetVariableByName("gLights");
	mfxDiffuseMapVar = mFX->GetVariableByName("gDiffuseMap")->AsShaderResource();
	mfxSpecMapVar    = mFX->GetVariableByName("gSpecMap")->AsShaderResource();
	mfxTexMtxVar     = mFX->GetVariableByName("gTexMtx")->AsMatrix();
}

void Southfall::buildVertexLayouts()
{
	// Create the vertex input layout.
	D3D10_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 24, D3D10_INPUT_PER_VERTEX_DATA, 0}
	};

	// Create the input layout
    D3D10_PASS_DESC PassDesc;
    mTech->GetPassByIndex(0)->GetDesc(&PassDesc);
    HR(md3dDevice->CreateInputLayout(vertexDesc, 3, PassDesc.pIAInputSignature,
		PassDesc.IAInputSignatureSize, &mVertexLayout));
}

void Southfall::initBirmingham()
{
	birm = true;

	HR(D3DX10CreateShaderResourceViewFromFile(md3dDevice, 
		L"Textures\\Birmingham.png", 0, 0, &mDiffuseMapRV[0], 0 ));

	HR(D3DX10CreateShaderResourceViewFromFile(md3dDevice, 
		L"Textures\\Birmingham.png", 0, 0, &mDiffuseMapRV[1], 0 ));

	HR(D3DX10CreateShaderResourceViewFromFile(md3dDevice, 
		L"Textures\\Birmingham.png", 0, 0, &mDiffuseMapRV[2], 0 ));

	HR(D3DX10CreateShaderResourceViewFromFile(md3dDevice, 
		L"Textures\\Birmingham.png", 0, 0, &mDiffuseMapRV[3], 0 ));

	HR(D3DX10CreateShaderResourceViewFromFile(md3dDevice, 
		L"Textures\\Birmingham.png", 0, 0, &mWaterMapRV, 0 ));
	
	ID3D10ShaderResourceView* temp;
	
	HR(D3DX10CreateShaderResourceViewFromFile(md3dDevice, 
		L"Textures\\Birmingham.png", 0, 0, &temp, 0 ));

	wraith.setDiffuseMap(temp);

	audio.playCue(BIRMINGHAM_CUE);
	
	//Advance level;
	input.clearKeyPress('B');
	gameState = CUT1;			
	audio.stopCue(BAR_BACKGROUND_CUE);
	startCut = mTimer.getGameTime();
	alpha = 20;	
	for (int i=POINT1; i<=POINT4; i++) lights.lights[i].on=0;
			
	lights.lights[AMBIENT_DIFFUSE].ambient	 = Color(0.1064453125, 0.1123046875, 0.1337890625,1);
	lights.lights[AMBIENT_DIFFUSE].diffuse	 = Color(0.9921, 0.9058, 0.5450, 1.f);
	lights.lights[AMBIENT_DIFFUSE].dir		 = Vector3(0,-.75,.75);	
			
	lights.lights[POINT1].pos		= Vector3(380, 600, (terrain[level].z-3)*terrain[level].scale);
	lights.lights[POINT1].diffuse	= Color(.05f,.05f,.05f,.5f);
	lights.lights[POINT1].att		= Vector3(0,.0008f,0);
	lights.lights[POINT1].range		= 65.f;

}