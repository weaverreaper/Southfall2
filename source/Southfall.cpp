//=============================================================================
// Color Cube App.cpp by Frank Luna (C) 2008 All Rights Reserved.
//
// Demonstrates coloring.
//
// Controls:
//		'A'/'D'/'W'/'S' - Rotate 
//
//=============================================================================

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

#include "aiLogic.h"

const static float delta = .000001f;

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
 
private:
	
	//This tells weather the game is at menu(state 0), playing(state 1), playing and paused(state 3)
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

	float mTheta;
	float mPhi;

	Terrain terrain;
	GeoObject terrainObj,t1,t2,t3;

};

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
	
	///////////Font /////////////////
	if(theText.initialize(md3dDevice, 18, true, false, "Arial") == false)
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing DirectX font"));

	/////////////////////////

	camera.init(&input);

	buildFX();
	buildVertexLayouts();
	
	origin.init(md3dDevice, 10);
	terrain.init(md3dDevice,3);
	
	D3DXMATRIX temp;

	terrainObj.init(mTech, mfxWVPVar, &terrain);
	Translate(&temp,0,-23,-20);
	terrainObj.world = temp;

	t1.init(mTech, mfxWVPVar, &terrain);
	Translate(&temp,0,-23,-50);
	t1.world = temp;

	t2.init(mTech, mfxWVPVar, &terrain);
	Translate(&temp,-30,-23,-50);
	t2.world = temp;

	t3.init(mTech, mfxWVPVar, &terrain);
	Translate(&temp,0-30,-23,-20);
	t3.world = temp;
	
	originObj.init(mTech, mfxWVPVar, &origin);
	
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

	if(input.wasKeyPressed(VK_ESCAPE))
		exit(0);

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
	
	default:
		
		break;
	}
	//input.clearAll();

	// Build the view matrix.
	pos = camera.getPos();
	target = camera.getTarget();
	up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&mView, &pos, &target, &up);

	camera.set(&mView);
}

void Southfall::drawScene()
{
	D3DApp::drawScene();

	// Restore default states, input layout and primitive topology 
	// because mFont->DrawText changes them.  Note that we can 
	// restore the default states by passing null.
	md3dDevice->OMSetDepthStencilState(0, 0);
	float blendFactors[] = {0.0f, 0.0f, 0.0f, 0.0f};
	md3dDevice->OMSetBlendState(0, blendFactors, 0xffffffff);
	md3dDevice->IASetInputLayout(mVertexLayout);
       
	mWVP = mView*mProj;
	originObj.draw(&mWVP);
	terrainObj.draw(&mWVP);
	t1.draw(&mWVP);
	t2.draw(&mWVP);
	t3.draw(&mWVP);

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
}

void Southfall::buildVertexLayouts()
{
	// Create the vertex input layout.
	D3D10_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0}
	};

	// Create the input layout
    D3D10_PASS_DESC PassDesc;
    mTech->GetPassByIndex(0)->GetDesc(&PassDesc);
    HR(md3dDevice->CreateInputLayout(vertexDesc, 2, PassDesc.pIAInputSignature,
		PassDesc.IAInputSignatureSize, &mVertexLayout));
}

