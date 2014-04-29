//=============================================================================
// Color Cube App.cpp by Frank Luna (C) 2008 All Rights Reserved.
//
// Demonstrates coloring.
//
// Controls:
//		'A'/'D'/'W'/'S' - Rotate 
//
//=============================================================================

#include "Southfall.h"
#include <ctime>

const static float delta = .000001f;

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

	if(theText.initialize(md3dDevice, 18, true, false, "Arial") == false)
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing DirectX font"));

	splash = Square(1);
	splash.init(md3dDevice,5);
	splashObj.init(mTech, mfxWVPVar, mfxWorldVar, &splash, Vertex(), Vertex());
	
	Matrix tm1;
	Identity(&tm1);

	D3DXMatrixRotationYawPitchRoll(&tm1, PI/2, 0, PI/2); 
	splashObj.setWorldMatrix(tm1);
	
	
	fireball.init(md3dDevice, 1);
	fireballObj.setDevice(md3dDevice); fireballObj.setMFX(mFX);
	fireballObj.init(mTech, mfxWVPVar, mfxWorldVar, &camera, &fireball);
	fireballObj.setInActive();
	fireballObj.setLight(&lights.lights[FIREBALL]);	

	torch1Fireball.setDevice(md3dDevice); torch1Fireball.setMFX(mFX);
	torch1Fireball.init(mTech, mfxWVPVar, mfxWorldVar, &camera, &fireball, 100);	//fireball double use?
	torch1Fireball.setLight(&lights.lights[FIREBALL2]);

	torch.init(md3dDevice, 10);
	torchObj1.setDevice(md3dDevice); torchObj1.setMFX(mFX);
	torchObj1.init(mTech, mfxWVPVar, mfxWorldVar, &torch1Fireball, &torch, Vector3(400,150,200));
	
	head.init(md3dDevice, 5);
	body.init(md3dDevice, 5);
	bearmodel.init(md3dDevice, 5);
	mfxDiffuseMapVar = mFX->GetVariableByName("gDiffuseMap")->AsShaderResource();
	mfxSpecMapVar    = mFX->GetVariableByName("gSpecMap")->AsShaderResource();
	mfxTexMtxVar     = mFX->GetVariableByName("gTexMtx")->AsMatrix();

	sword.init(md3dDevice,.55);
	swordObj.setDevice(md3dDevice); swordObj.setMFX(mFX);
	swordObj.init(mTech, mfxWVPVar, mfxWorldVar, &sword, Vertex(), Vertex());
	swordObj.setActive();

	level = 0;
	pigKilled = false;
	bearKilled = false;
	goblinsKilled = false;
	wraithKilled = false;

	camera.init(Vector3(400,20,10), Vector3(400,50,200), &input, &audio, &mView, &mProj, &terrain[level], &lights);
	
	bear.setMFX(mFX);
	bear.init2(mTech,mfxWVPVar, mfxWorldVar, md3dDevice, &bearmodel, &terrain[level]);
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

	camera.setFireball(&fireballObj);	

	HR(D3DX10CreateShaderResourceViewFromFile(md3dDevice, 
		L"Textures\\Ground1.jpg", 0, 0, &mDiffuseMapRV[0], 0 ));

	HR(D3DX10CreateShaderResourceViewFromFile(md3dDevice, 
		L"Textures\\Leaves5.jpg", 0, 0, &mDiffuseMapRV[1], 0 ));

	HR(D3DX10CreateShaderResourceViewFromFile(md3dDevice, 
		L"Textures\\Stone1.jpg", 0, 0, &mDiffuseMapRV[2], 0 ));

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

	/*
	HR(D3DX10CreateShaderResourceViewFromFile(md3dDevice, 
		L"Textures\\goblinskin.jpg", 0, 0, &mGoblinSkinTextureRV, 0 ));
		*/
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

		surr[i].setDevice(md3dDevice); 
		surr[i].setMFX(mFX);
	}
	surr[0].initTextures(NULL, L"Textures/sandBack.png",L"Textures/Rock1.jpg",L"Textures/Rock1.jpg",L"Textures/Rock1.jpg",L"Textures/Sky2.jpg");
	surr[1].initTextures(L"Textures/Rock1.jpg",NULL,L"Textures/Foliage1.jpg",L"Textures/Foliage1.jpg",L"Textures/Rock1.jpg",L"Textures/Sky2.jpg");
	for(int i = 0; i < LEVELS; ++i)
	{
		surr[i].init(mTech, mfxWVPVar, mfxWorldVar, &terrain[i]);
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
	bear.setInActive();
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
}

void Southfall::onResize()
{
	D3DApp::onResize();

	float aspect = (float)mClientWidth/mClientHeight;
	D3DXMatrixPerspectiveFovLH(&mProj, 0.25f*PI, aspect, 1.0f, 4000.0f);
}

void Southfall::updateScene(float dt)
{
	D3DApp::updateScene(dt);	
	Matrix tm1;
	Box tembB;
	GeoObject tempO;

	if(input.wasKeyPressed(VK_ESCAPE))
		PostQuitMessage(0);
	
	switch (gameState)
	{
	case SPLASH1:
		if(input.anyKeyPressed())
		{
			//gameState = CUT1;			
			gameState = LEVEL1;
			audio.stopCue(BAR_BACKGROUND_CUE);
			startCut1 = mTimer.getGameTime();
			alpha = 0;	
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
			Vector3 position(2.5,2.5,-7);
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
	case CUT1:
		if (mTimer.getGameTime() - startCut1 > 3)
		{
			gameState = CUT2;
			camera.update(dt);
			startCut2 = mTimer.getGameTime();
			alpha = 0;
			Sleep(400);
			break;
		}
		alpha += 80*dt;
		if (alpha > 255) alpha = 255;
		if (alpha < 0) alpha = 0;

		//Speed along wave progress
		for (int i=0; i<3; i++)
		{
			mWaterTexOffset.y += 0.1f*dt;
			mWaterTexOffset.x = 0.25f*sinf(4.0f*mWaterTexOffset.y);

			mWaves.update(dt);
		}
		break;
	case CUT2:
		if (mTimer.getGameTime() - startCut2 > 3)
		{
			gameState = LEVEL1;
			camera.update(dt);
			audio.playCue(BEACH_CUE);
			break;
		}
		alpha += 80*dt;
		if (alpha > 255) alpha = 255;
		if (alpha < 0) alpha = 0;

		//Speed along wave progress
		for (int i=0; i<3; i++)
		{
			mWaterTexOffset.y += 0.1f*dt;
			mWaterTexOffset.x = 0.25f*sinf(4.0f*mWaterTexOffset.y);

			mWaves.update(dt);
		}

		break;

	case CUT3:
		//Intro "cut scene," camera bobbing and moving toward the island like on a boat
		//Can we get a boat model?

		break;

	case LEVEL1:

		tempO.init(mTech, mfxWVPVar, mfxWorldVar, &origin, Vertex(), Vertex());
		tempO.setPosition(camera.getPos());
		tempO.setRadius(10);
		if(goblin1.head.getActiveState() && (tempO.collided(&goblin1.head) || tempO.collided(&goblin1.body)))
		{
			exit(0);
		}
		if(goblin1.head.getActiveState())
			goblin1.update(dt,camera.getPos(), &fireballObj, &swordObj);
		if(goblin1.health <= 0)
		{
			goblin1.health = 1;
			lights.lights[POINT1].on = 1;
			audio.playCue(ZELDA_CUE);
			pigKilled = true;
		}

		camera.update(dt);
		fireballObj.update(dt);
		swordObj.setPosition(camera.getPos() - .4*Vector3(0,HEAD_HEIGHT,0));
		swordObj.setAngle(camera.getTheta());
		swordObj.update(dt);
		if(input.getMouseLButton()){camera.addShake(.25*(swordObj.power-1.0f));if (swordObj.swing()) audio.playCue(SWING_CUE); }
		else {swordObj.rising = false;}
		
		
		//if(camera.getPos().z >= (terrain[level].z-3)*terrain[level].scale)//level done

//Waves stuff
		// Animate water texture as a function of time.
		mWaterTexOffset.y += 0.1f*dt;
		mWaterTexOffset.x = 0.25f*sinf(4.0f*mWaterTexOffset.y);

		mWaves.update(dt);

		torchObj1.update(dt);
//End level
		if(pigKilled && camera.getPos().z >= (terrain[level].z-3)*terrain[level].scale)//level done.
		{
			
			++gameState;
			++level;
			audio.stopCue(BEACH_CUE);
			audio.playCue(FOREST_CUE);
			if(level >= LEVELS)
				level = LEVELS-1;
			camera.init(Vector3(400,100,10), Vector3(400,200,200), &input, &audio, &mView, &mProj, &terrain[level], &lights);
			bear.init2(mTech,mfxWVPVar, mfxWorldVar, md3dDevice, &bearmodel, &terrain[level]);
			goblin1.init(mTech,mfxWVPVar, mfxWorldVar, md3dDevice, &head, &body, &terrain[level]);
			goblin2.init(mTech,mfxWVPVar, mfxWorldVar, md3dDevice, &head, &body, &terrain[level]);
			goblin3.init(mTech,mfxWVPVar, mfxWorldVar, md3dDevice, &head, &body, &terrain[level]);
			goblin1.setPosition(D3DXVECTOR3(450,120,1000));
			bear.setPosition(D3DXVECTOR3(450,120,1200));
			goblin1.health = 200;
			goblin1.head.setActive();
			goblin1.body.setActive();
			goblin2.head.setActive();
			goblin2.body.setActive();
			goblin3.head.setActive();
			goblin3.body.setActive();
			bear.setActive();
			lights.lights[POINT1].pos		= Vector3(380, 400, (terrain[level].z-3)*terrain[level].scale);

			lights.lights[AMBIENT_DIFFUSE].ambient	 = Color(.4,.4,.4,1);
			lights.lights[AMBIENT_DIFFUSE].diffuse	 = Color(.6f, .75f, .6f, 1.f);
			lights.lights[AMBIENT_DIFFUSE].dir		 = Vector3(0,-1,0);	

			lights.lights[POINT1].on = 0;
			pigKilled = false;

			mEnvMapRV = tm.createCubeTex(L"Textures\\CubeMaps\\Level2Forest.dds");
			sky.init(md3dDevice, mEnvMapRV, 15000.0f);
		}



//Cheat
		if (input.isKeyDown('O'))
		{
			lights.lights[POINT1].on = 1;
			audio.playCue(ZELDA_CUE);
			pigKilled = true;
		}

		break;
	case CUT4:
		++gameState;
		break;
	case LEVEL2:
		
		tempO.init(mTech, mfxWVPVar, mfxWorldVar, &origin, Vertex(), Vertex());
		tempO.setPosition(camera.getPos());
		tempO.setRadius(10);
		if(goblin1.head.getActiveState() && (tempO.collided(&goblin1.head) || tempO.collided(&goblin1.body)))
		{
			exit(0);
		}
		if(goblin2.head.getActiveState() && (tempO.collided(&goblin2.head) || tempO.collided(&goblin2.body)))
		{
			exit(0);
		}
		if(goblin3.head.getActiveState() && (tempO.collided(&goblin3.head) || tempO.collided(&goblin3.body)))
		{
			exit(0);
		}
		if(bear.getActiveState() && (tempO.collided(&bear)))
		{
			exit(0);
		}
		if(!goblin1.done())
			goblin1.update(dt,camera.getPos(), &fireballObj, &swordObj);
		if(!goblin2.done())
			goblin2.update(dt,camera.getPos(), &fireballObj, &swordObj);
		if(!goblin3.done())
			goblin3.update(dt,camera.getPos(), &fireballObj, &swordObj);
		if(!bear.done())
			bear.update(dt,camera.getPos(), &fireballObj, &swordObj);

		camera.update(dt);
		fireballObj.update(dt);
		swordObj.setPosition(camera.getPos() - .4*Vector3(0,HEAD_HEIGHT,0));
		swordObj.setAngle(camera.getTheta());
		swordObj.update(dt);
		if(input.getMouseLButton()){camera.addShake(.25*(swordObj.power-1.0f));if (swordObj.swing()) audio.playCue(SWING_CUE); }
		else {swordObj.rising = false;}

		if (input.isKeyDown('O') || bear.health <= 0)
		{
			bear.health = 1;
			lights.lights[POINT1].on = 1;
			audio.playCue(ZELDA_CUE);
			bearKilled = true;			
			alpha = 0;
		}
		if(bearKilled && camera.getPos().z >= (terrain[level].z-3)*terrain[level].scale)
		{
			++gameState;
			++level;
			camera.init(Vector3(400,100,10), Vector3(400,200,200), &input, &audio, &mView, &mProj, &terrain[level], &lights);
				
			//startEndCut = mTimer.getGameTime();
			audio.stopCue(FOREST_CUE);
		}

		break;
	case CUT5:
		++gameState;
		break;
	case LEVEL3:
		tempO.init(mTech, mfxWVPVar, mfxWorldVar, &origin, Vertex(), Vertex());
		tempO.setPosition(camera.getPos());
		tempO.setRadius(10);
		
		camera.update(dt);
		fireballObj.update(dt);
		swordObj.setPosition(camera.getPos() - .4*Vector3(0,HEAD_HEIGHT,0));
		swordObj.setAngle(camera.getTheta());
		swordObj.update(dt);
		if(input.getMouseLButton()){camera.addShake(.25*(swordObj.power-1.0f));if (swordObj.swing()) audio.playCue(SWING_CUE); }
		else {swordObj.rising = false;}
		if (input.isKeyDown('O'))// || bear.health <= 0)
		{
			bear.health = 1;
			lights.lights[POINT1].on = 1;
			audio.playCue(ZELDA_CUE);
			goblinsKilled = true;			
			alpha = 0;
		}
		if(goblinsKilled && camera.getPos().z >= (terrain[level].z-3)*terrain[level].scale)
		{
			++gameState;
			++level;
				
			camera.init(Vector3(400,100,10), Vector3(400,200,200), &input, &audio, &mView, &mProj, &terrain[level], &lights);
			
			//startEndCut = mTimer.getGameTime();
			//audio.stopCue(FOREST_CUE);
		}

		break;
	case CUT6:
		++gameState;
		break;
	case LEVEL4:

		tempO.init(mTech, mfxWVPVar, mfxWorldVar, &origin, Vertex(), Vertex());
		tempO.setPosition(camera.getPos());
		tempO.setRadius(10);
		
		camera.update(dt);
		fireballObj.update(dt);
		swordObj.setPosition(camera.getPos() - .4*Vector3(0,HEAD_HEIGHT,0));
		swordObj.setAngle(camera.getTheta());
		swordObj.update(dt);

		if(input.getMouseLButton()){camera.addShake(.25*(swordObj.power-1.0f));if (swordObj.swing()) audio.playCue(SWING_CUE); }
		else {swordObj.rising = false;}

		if (input.isKeyDown('O'))//|| bear.health <= 0)
		{
			bear.health = 1;
			lights.lights[POINT1].on = 1;
			audio.playCue(ZELDA_CUE);
			wraithKilled = true;			
			alpha = 0;
		}
		if(wraithKilled && camera.getPos().z >= (terrain[level].z-3)*terrain[level].scale)
		{
				++gameState;
				
				startEndCut = mTimer.getGameTime();
				//audio.stopCue(FOREST_CUE);
		}

		break;
	case CUT7:
		++gameState;
		break;
	case END:		
		if (mTimer.getGameTime() - startEndCut > 3) PostQuitMessage(0);
		
		alpha += 80*dt;
		if (alpha > 255) alpha = 255;
		if (alpha < 0) alpha = 0;

		break;
	}

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
	mfxLightVar->SetRawValue(&lights.lights, 0, sizeof(Light)*LIGHT_COUNT);

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
		break;
	case CUT1:
		theText.setFontColor(SETCOLOR_ARGB(alpha, 255,255,255));
		theText.print("The Wraith has escaped to his coastal home.",GAME_WIDTH/2 - 400,GAME_HEIGHT/2-200);		
		break;
	case CUT2:
		theText.setFontColor(SETCOLOR_ARGB(alpha, 255,255,255));
		theText.print("You must rid the land of evil!",GAME_WIDTH/2 + 200,GAME_HEIGHT/2+100);		
		break;
	case LEVEL1:
		terrainObj[level].draw(&mWVP);
		//surr[level].draw(&mWVP);
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
		setShaderVals();
		torchObj1.draw(&mWVP);
		
		setShaderVals();
		fireballObj.draw(&mWVP);
		
		q << "Bacon: " << score;
		theText.print(q.str(),0, 0);
		break;
	case CUT3:
		break;
	case LEVEL2:
		terrainObj[level].draw(&mWVP);
		//surr[level].draw(&mWVP);
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
		fireballObj.draw(&mWVP);
		q << "Bacon: " << score;
		theText.print(q.str(),0, 0);
		break;
	case CUT4:
		break;
	case LEVEL3:
		terrainObj[level].draw(&mWVP);
		//surr[level].draw(&mWVP);
		sky.draw();
		setShaderVals();
		
		swordObj.draw(&mWVP);
		setShaderVals();
		fireballObj.draw(&mWVP);
		q << "Bacon: " << score;
		theText.print(q.str(),0, 0);
		break;
	case CUT5:
		break;
	case LEVEL4:
		terrainObj[level].draw(&mWVP);
		//surr[level].draw(&mWVP);
		sky.draw();
		setShaderVals();
		
		swordObj.draw(&mWVP);
		setShaderVals();
		fireballObj.draw(&mWVP);
		q << "Bacon: " << score;
		theText.print(q.str(),0, 0);
		break;
	case CUT6:
		break;
	case END:
		theText.setFontColor(SETCOLOR_ARGB(alpha, 255,255,255));
		theText.print("To be continued...",GAME_WIDTH/2 - 50,GAME_HEIGHT/2);		
		break;
	}

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
