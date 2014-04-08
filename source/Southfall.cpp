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

	ReleaseCOM(mFX);
	ReleaseCOM(mVertexLayout);
}

void Southfall::initApp()
{
	D3DApp::initApp();	

	buildFX();
	buildVertexLayouts();

	if(theText.initialize(md3dDevice, 18, true, false, "Arial") == false)
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing DirectX font"));

	splash = Square(1);
	splash.init(md3dDevice,5);
	splashObj.init(mTech, mfxWVPVar, mfxWorldVar, &splash, Vertex(), Vertex());
	
	Matrix tm1;
	Identity(&tm1);

	D3DXMatrixRotationYawPitchRoll(&tm1, PI/2, 0, PI/2); 
	splashObj.setWorldMatrix(tm1);
	
	
	fireball.init(md3dDevice, 5);
	fireballObj.setDevice(md3dDevice); fireballObj.setMFX(mFX);
	fireballObj.init(mTech, mfxWVPVar, mfxWorldVar, &fireball);
	fireballObj.setInActive();
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

	camera.init(Vector3(400,40,10), Vector3(400,200,200), &input, &audio, &mView, &terrain[level], &lights);

	bear.init(mTech,mfxWVPVar, mfxWorldVar, &bearmodel, &terrain[level]);

	goblin.setMFX(mFX);
	goblin.init(mTech,mfxWVPVar, mfxWorldVar, md3dDevice, &head, &body, &terrain[level]);

	fireballObj.setLight(&lights.lights[FIREBALL]);

	camera.setFireball(&fireballObj);	

	HR(D3DX10CreateShaderResourceViewFromFile(md3dDevice, 
		L"Textures\\Ground1.jpg", 0, 0, &mDiffuseMapRV[0], 0 ));

	HR(D3DX10CreateShaderResourceViewFromFile(md3dDevice, 
		L"Textures\\Leaves5.jpg", 0, 0, &mDiffuseMapRV[1], 0 ));

	HR(D3DX10CreateShaderResourceViewFromFile(md3dDevice, 
		L"Textures\\defaultspec.dds", 0, 0, &mSpecMapRV, 0 ));
	
	HR(D3DX10CreateShaderResourceViewFromFile(md3dDevice, 
		L"Textures\\Title.png", 0, 0, &mSplashTextureRV, 0 ));

	/*
	HR(D3DX10CreateShaderResourceViewFromFile(md3dDevice, 
		L"Textures\\goblinskin.jpg", 0, 0, &mGoblinSkinTextureRV, 0 ));
		*/
	origin.init(md3dDevice, 10);
	terrain[0].initFile("Worlds/Beach.txt");
	terrain[1].initFile("Worlds/Forest.txt");
	for(int i = 0; i < LEVELS; ++i)
	{
		terrain[i].init(md3dDevice);
		terrainObj[i].init(mTech, mfxWVPVar, mfxWorldVar, &terrain[i], Vertex(), Vertex());
		terrainObj[i].setPosition(Vector3(0,0,0));

		surr[i].setDevice(md3dDevice); 
		surr[i].setMFX(mFX);
	}
	surr[0].initTextures(NULL, L"Textures/water.png",L"Textures/Rock1.jpg",L"Textures/Rock1.jpg",L"Textures/Rock1.jpg",L"Textures/Sky2.jpg");
	surr[1].initTextures(L"Textures/Rock1.jpg",NULL,L"Textures/Foliage1.jpg",L"Textures/Foliage1.jpg",L"Textures/Rock1.jpg",L"Textures/Sky2.jpg");
	for(int i = 0; i < LEVELS; ++i)
	{
		surr[i].init(mTech, mfxWVPVar, mfxWorldVar, &terrain[i]);
	}
	originObj.init(mTech, mfxWVPVar, mfxWorldVar, &origin, Vertex(), Vertex());
	goblin.setPosition(D3DXVECTOR3(300,120,300));
	goblin.setScale(5.0f);
	bear.setPosition(D3DXVECTOR3(600,120,300));
	bear.setScale(5.0f);
	score = 0;
	gameState = SPLASH1;
	audio.playCue(BAR_BACKGROUND_CUE);
	input.clearAll();
}

void Southfall::onResize()
{
	D3DApp::onResize();

	float aspect = (float)mClientWidth/mClientHeight;
	D3DXMatrixPerspectiveFovLH(&mProj, 0.25f*PI, aspect, 1.0f, 1000.0f);
}

void Southfall::updateScene(float dt)
{
	D3DApp::updateScene(dt);	
	Matrix tm1;
	Box tembB;
	GeoObject tempO;

	if(input.wasKeyPressed(VK_ESCAPE))
		exit(0);//PostQuitMessage(0);
	
	switch (gameState)
	{
	case SPLASH1:
		if(input.anyKeyPressed())
		{
			gameState = LEVEL1;	
			audio.stopCue(BAR_BACKGROUND_CUE);
			startCut1 = mTimer.getGameTime();
			alpha = 0;	
			for (int i=POINT1; i<=POINT4; i++) lights.lights[i].on=0;
			lights.lights[AMBIENT_DIFFUSE].dir		 = Vector3(0,-1,1);

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
		break;

	case LEVEL1:

		tempO.init(mTech, mfxWVPVar, mfxWorldVar, &origin, Vertex(), Vertex());
		tempO.setPosition(camera.getPos());
		tempO.setRadius(10);
		if(goblin.head.getActiveState() && (tempO.collided(&goblin.head) || tempO.collided(&goblin.body)))
		{
			exit(0);
		}

		goblin.update(dt,camera.getPos(), &fireballObj, &swordObj);
		bear.update(dt,camera.getPos());

		camera.update(dt);
		fireballObj.update(dt);
		swordObj.setPosition(camera.getPos() - .4*Vector3(0,HEAD_HEIGHT,0));
		swordObj.setAngle(camera.getTheta());
		swordObj.update(dt);
		if(input.getMouseRButton())
			swordObj.swing();
		if(camera.getPos().z >= (terrain[level].z-3)*terrain[level].scale)//level done

		if(pigKilled && camera.getPos().z >= (terrain[level].z-3)*terrain[level].scale)//level done.
		{
			
			++gameState;
			++level;
			audio.stopCue(BEACH_CUE);
			audio.playCue(FOREST_CUE);
			if(level >= LEVELS)
				level = LEVELS-1;
			camera.init(Vector3(400,100,10), Vector3(400,200,200), &input, &audio, &mView, &terrain[level], &lights);

			bear.init(mTech,mfxWVPVar, mfxWorldVar, &bearmodel, &terrain[level]);

			goblin.init(mTech,mfxWVPVar, mfxWorldVar, md3dDevice, &head, &body, &terrain[level]);

			lights.lights[POINT1].pos		= Vector3(380, 400, (terrain[level].z-3)*terrain[level].scale);

			lights.lights[AMBIENT_DIFFUSE].ambient	 = Color(.4,.4,.4,1);
			lights.lights[AMBIENT_DIFFUSE].diffuse	 = Color(.6f, .75f, .6f, 1.f);
			lights.lights[AMBIENT_DIFFUSE].dir		 = Vector3(0,-1,0);	

			lights.lights[POINT1].on = 0;
		}
		if (input.isKeyDown('O'))
		{
			lights.lights[POINT1].on = 1;
			audio.playCue(ZELDA_CUE);
			pigKilled = true;
		}

		break;

	case LEVEL2:
		
		tempO.init(mTech, mfxWVPVar, mfxWorldVar, &origin, Vertex(), Vertex());
		tempO.setPosition(camera.getPos());
		tempO.setRadius(10);
		if(goblin.head.getActiveState() && (tempO.collided(&goblin.head) || tempO.collided(&goblin.body)))
		{
			exit(0);
		}
		
		goblin.update(dt,camera.getPos(), &fireballObj, &swordObj);
		bear.update(dt,camera.getPos());

		camera.update(dt);
		fireballObj.update(dt);
		swordObj.setPosition(camera.getPos() - .4*Vector3(0,HEAD_HEIGHT,0));
		swordObj.setAngle(camera.getTheta());
		swordObj.update(dt);
		if(input.getMouseRButton())
			swordObj.swing();
		if (input.isKeyDown('O'))
		{
			lights.lights[POINT1].on = 1;
			audio.playCue(ZELDA_CUE);
			bearKilled = true;			
			alpha = 0;
		}

		if(bearKilled && camera.getPos().z >= (terrain[level].z-3)*terrain[level].scale)
		{
				gameState = END;
				startEndCut = mTimer.getGameTime();
				audio.stopCue(FOREST_CUE);
		}

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
		theText.print("You arrive in a darkened land.",GAME_WIDTH/2 - 400,GAME_HEIGHT/2-200);		
		break;
	case CUT2:
		theText.setFontColor(SETCOLOR_ARGB(alpha, 255,255,255));
		theText.print("Your finger will light the way...",GAME_WIDTH/2 + 200,GAME_HEIGHT/2+100);		
		break;
	case LEVEL1:

	case LEVEL2:
		terrainObj[level].draw(&mWVP);
		surr[level].draw(&mWVP);
		fireballObj.draw(&mWVP);
		goblin.draw(&mWVP);
		bear.draw(&mWVP);
		swordObj.draw(&mWVP);

		q << "Bacon: " << score;
		theText.print(q.str(),0, 0);	
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
#if defined( DEBUG ) || defined( _DEBUG )
    shaderFlags |= D3D10_SHADER_DEBUG;
	shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif
 
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
