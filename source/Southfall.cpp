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
	
	fireball.init(md3dDevice, 5);
	fireballObj.setDevice(md3dDevice); fireballObj.setMFX(mFX);
	fireballObj.init(mTech, mfxWVPVar, mfxWorldVar, &fireball, Vertex(), Vertex());
	fireballObj.setInActive();
	
	camera.init(Vector3(10,100,10), Vector3(200,0,0), &input, &mView, &terrain, &lights);
	//action.init() <- haha <- lol

	fireballObj.setLight(&lights.lights[FIREBALL]);

	camera.setFireball(&fireballObj);	

	HR(D3DX10CreateShaderResourceViewFromFile(md3dDevice, 
		L"Textures\\Ground1.jpg", 0, 0, &mDiffuseMapRV, 0 ));

	HR(D3DX10CreateShaderResourceViewFromFile(md3dDevice, 
		L"Textures\\defaultspec.dds", 0, 0, &mSpecMapRV, 0 ));

	
	origin.init(md3dDevice, 10);
	terrain.init(md3dDevice);

	D3DXMATRIX temp;

	terrainObj.init(mTech, mfxWVPVar, mfxWorldVar, &terrain, Vertex(), Vertex());
	terrainObj.setPosition(Vector3(0,0,0));
	
	originObj.init(mTech, mfxWVPVar, mfxWorldVar, &origin, Vertex(), Vertex());
	
	score = 0;	
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
	dtime = dt;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 target;
	D3DXVECTOR3 up;
	float xf, yf, zf;
	int x,y,z;
	Matrix temp;
	int w1,w2;
	bool done = false;
	camera.update(dt);
	fireballObj.update(dt);

	if(input.wasKeyPressed(VK_ESCAPE))
		PostQuitMessage(0);

	switch (gameState)
	{
	case 0:
		if(input.wasKeyPressed(' '))
		{
			//audio.playCue(DISCO);
			gameState = 2;
		}
		break;
	case 1:
		break;
	case 2:		
		if(done)
			gameState = 2;
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

	mfxDiffuseMapVar->SetResource(mDiffuseMapRV);
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
	originObj.draw(&mWVP);
	terrainObj.draw(&mWVP);
	fireballObj.draw(&mWVP);


	// We specify DT_NOCLIP, so we do not care about width/height of the rect.
	RECT R = {5, 5, 0, 0};
	mFont->DrawText(0, mFrameStats.c_str(), -1, &R, DT_NOCLIP, BLACK);	

	std::stringstream q;
	q << "Score: " << score;
	theText.print(q.str(),0, 0);	
       
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
