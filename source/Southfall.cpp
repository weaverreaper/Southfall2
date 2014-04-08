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
	//splashObj.setPosition(Vector3(10,10,10));
	Matrix tm1,tm2,tm3;
	Identity(&tm1);


	D3DXMatrixRotationYawPitchRoll(&tm1, PI/2, 0, PI/2); 
	splashObj.setWorldMatrix(tm1);
	//splashObj.update(1);
	
	fireball.init(md3dDevice, 5);
	fireballObj.setDevice(md3dDevice); fireballObj.setMFX(mFX);
	fireballObj.init(mTech, mfxWVPVar, mfxWorldVar, &fireball, Vertex(), Vertex());
	fireballObj.setInActive();

	mfxDiffuseMapVar = mFX->GetVariableByName("gDiffuseMap")->AsShaderResource();
	mfxSpecMapVar    = mFX->GetVariableByName("gSpecMap")->AsShaderResource();
	mfxTexMtxVar     = mFX->GetVariableByName("gTexMtx")->AsMatrix();

	sword.init(md3dDevice,.55);
	swordObj.setDevice(md3dDevice); swordObj.setMFX(mFX);
	swordObj.init(mTech, mfxWVPVar, mfxWorldVar, &sword, Vertex(), Vertex());
	swordObj.setActive();

	level = 0;

	camera.init(Vector3(400,100,10), Vector3(400,200,200), &input, &audio, &mView, &terrain[level], &lights);
	//action.init() <- haha <- lol

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
	
	score = 0;
	gameState = SPLASH1;
	audio.playCue(BAR_BACKGROUND_CUE);
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

	if(input.wasKeyPressed(VK_ESCAPE))
		PostQuitMessage(0);

	switch (gameState)
	{
	case SPLASH1:
		if(input.anyKeyPressed())
		{
			gameState = CUT1;	
			audio.stopCue(BAR_BACKGROUND_CUE);
			startCut1 = mTimer.getGameTime();
			alpha = 0;	
			for (int i=POINT1; i<=POINT4; i++) lights.lights[i].on=0;
		}
		else
		{
			Vector3 up(0,1,0);
			Vector3 position(2.5,2.5,-7);
			Vector3 target(2.5,2.5,0);
			D3DXMatrixLookAtLH(&mView, &position, &target, &up);
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

		}
		break;
	case CUT1:
		if (mTimer.getGameTime() - startCut1 > 4)
		{
			gameState = CUT2;
			camera.update(dt);
			startCut2 = mTimer.getGameTime();
			alpha = 0;
			break;
		}
		alpha += 80*dt;
		if (alpha > 255) alpha = 255;
		if (alpha < 0) alpha = 0;
		break;
	case CUT2:
		if (mTimer.getGameTime() - startCut2 > 4)
		{
			gameState = LEVEL1;
			camera.update(dt);
			break;
		}
		alpha += 80*dt;
		if (alpha > 255) alpha = 255;
		if (alpha < 0) alpha = 0;
		break;
	case LEVEL1:
	case LEVEL2:
		camera.update(dt);
		fireballObj.update(dt);
		swordObj.setPosition(camera.getPos() - .4*Vector3(0,HEAD_HEIGHT,0));
		swordObj.setAngle(camera.getTheta());
		swordObj.update(dt);
		if(input.getMouseRButton())
			swordObj.swing();
		if(camera.getPos().z >= (terrain[level].z-3)*terrain[level].scale)//level done
		{
			++gameState;
			++level;
			if(level >= LEVELS)
				level = LEVELS-1;
			camera.init(Vector3(400,100,10), Vector3(400,200,200), &input, &audio, &mView, &terrain[level], &lights);
	
		}
		break;
	case END:		

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
	RECT R = {5, 5, 0, 0};
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
		theText.print("You arrive in a darkened land.",GAME_WIDTH/2 - 100,GAME_HEIGHT/2);		
		break;
	case CUT2:
		theText.setFontColor(SETCOLOR_ARGB(alpha, 255,255,255));
		theText.print("Your finger will light the way...",GAME_WIDTH/2 + 100,GAME_HEIGHT/2+50);		
		break;
	case LEVEL1:
	case LEVEL2:
		terrainObj[level].draw(&mWVP);
		surr[level].draw(&mWVP);
		fireballObj.draw(&mWVP);	
		swordObj.draw(&mWVP);

		q << "Score: " << score;
		theText.print(q.str(),0, 0);	
		break;
	case END:
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
