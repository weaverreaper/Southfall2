//=============================================================================
// Color Cube App.cpp by Frank Luna (C) 2008 All Rights Reserved.
//
// Demonstrates coloring.
//
// Controls:
//		'A'/'D'/'W'/'S' - Rotate 
//
//=============================================================================

//#include "C:\Users\weaverem1\Dropbox\S14\Games2\PartII\Common\d3dApp.h"
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
#include "Wave.h"


#include "aiLogic.h"

const static float delta = .000001f;

class ColoredCubeApp : public D3DApp
{
public:
	ColoredCubeApp(HINSTANCE hInstance);
	~ColoredCubeApp();

	void initApp();
	void onResize();
	void updateScene(float dt);
	void drawScene(); 
	int enemiesOut;
	int currentWave;
	int enemiesDone;

private:
	void buildFX();
	void buildVertexLayouts();
 
private:
	
	//This tells weather the game is at menu(state 0), playing(state 1), playing and paused(state 3)
	int gameState;

	const static int LENGTH = 20, HEIGHT = 5,WIDTH = 10;
	float lastFrameTime;
	float dtime;

	//Box* base[LENGTH][HEIGHT][WIDTH];
	GeoObject* baseObj[LENGTH][HEIGHT][WIDTH];
	Wave enemies;
//	Enemy* enemies[MAXENEMIES];
	tower towers[LENGTH][WIDTH];
	int reload[LENGTH][WIDTH];
	int score;
	int lives;
	location selected;
	Square selectBox;
	Pyramid enemy;
	Box baseBox;
	Pyramid pyramid;
	Pyramid Rpyramid,Gpyramid,Bpyramid,RGpyramid,RBpyramid,GBpyramid;
	Box mBox;
	Line line;
	Origin origin;
	GeoObject originObj,selectObj;
	TextDX theText;
	GeoObject* attacks[100];
	int attackTime[100];
	int cost(tower t);

	ID3D10Effect* mFX;
	ID3D10EffectTechnique* mTech;
	ID3D10InputLayout* mVertexLayout;
	ID3D10EffectMatrixVariable* mfxWVPVar;

	D3DXMATRIX mView;
	D3DXMATRIX mProj;
	D3DXMATRIX mWVP;

	float mTheta;
	float mPhi;

	AIMind hiveMind;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
				   PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif


	ColoredCubeApp theApp(hInstance);
	
	theApp.initApp();

	return theApp.run();
}

ColoredCubeApp::ColoredCubeApp(HINSTANCE hInstance)
: D3DApp(hInstance), mFX(0), mTech(0), mVertexLayout(0),
  mfxWVPVar(0), mTheta(0.0f), mPhi(PI*0.25f), lastFrameTime(0), gameState(0), hiveMind(18, 8, 0, 4, 17, 4)
{
	enemiesOut = 0;
	enemiesDone = 0;
	currentWave = 1;
	D3DXMatrixIdentity(&mView);
	D3DXMatrixIdentity(&mProj);
	D3DXMatrixIdentity(&mWVP); 
	baseBox = Box(D3DXCOLOR(.1,.1,.7,1),D3DXCOLOR(.25,.1,.7,1),D3DXCOLOR(.1,.25,.7,1),D3DXCOLOR(.25,.25,.7,1));
	enemy = Pyramid(D3DXCOLOR(.5,.1,.1,1),D3DXCOLOR(.5,.25,.1,1),D3DXCOLOR(.5,.1,.25,1),D3DXCOLOR(.5,.25,.25,1),D3DXCOLOR(.5,.25,.25,1));
	Rpyramid = Pyramid(D3DXCOLOR(.5,0,0,1),D3DXCOLOR(.5,0,0,1),D3DXCOLOR(.5,0,0,1),D3DXCOLOR(.5,0,0,1),D3DXCOLOR(.25,0,0,1));
	Gpyramid = Pyramid(D3DXCOLOR(0,.5,0,1),D3DXCOLOR(0,.5,0,1),D3DXCOLOR(0,.5,0,1),D3DXCOLOR(0,.5,0,1),D3DXCOLOR(0,.25,0,1));
	Bpyramid = Pyramid(D3DXCOLOR(0,0,.5,1),D3DXCOLOR(0,0,.5,1),D3DXCOLOR(0,0,.5,1),D3DXCOLOR(0,0,.5,1),D3DXCOLOR(0,0,.25,1));
	RGpyramid = Pyramid(D3DXCOLOR(.5,0,0,1),D3DXCOLOR(0,.5,0,1),D3DXCOLOR(.5,0,0,1),D3DXCOLOR(0,.5,0,1),D3DXCOLOR(.25,.25,0,1));
	RBpyramid = Pyramid(D3DXCOLOR(.5,0,0,1),D3DXCOLOR(0,0,.5,1),D3DXCOLOR(.5,0,0,1),D3DXCOLOR(0,0,.5,1),D3DXCOLOR(.25,0,.25,1));
	GBpyramid = Pyramid(D3DXCOLOR(0,.5,0,1),D3DXCOLOR(0,0,.5,1),D3DXCOLOR(0,.5,0,1),D3DXCOLOR(0,0,.5,1),D3DXCOLOR(0,.25,.25,1));
	mBox = Box(D3DXCOLOR(.3,.3,.3,1),D3DXCOLOR(.5,.3,.3,1),D3DXCOLOR(.3,.3,.5,1),D3DXCOLOR(.5,.3,.5,1));
}

ColoredCubeApp::~ColoredCubeApp()
{
	for(int i = 0; i < LENGTH; ++i)
		for(int j = 0; j < HEIGHT; ++j)
			for(int k = 0; k < WIDTH; ++k)
				delete baseObj[i][j][k];

	/*for(int i = 0; i < MAXENEMIES; ++i)
		delete enemies[i];*/

	if( md3dDevice )
		md3dDevice->ClearState();

	ReleaseCOM(mFX);
	ReleaseCOM(mVertexLayout);
}

void ColoredCubeApp::initApp()
{
	D3DApp::initApp();
	
	///////////Font /////////////////
	if(theText.initialize(md3dDevice, 18, true, false, "Arial") == false)
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing DirectX font"));

	/////////////////////////


	buildFX();
	buildVertexLayouts();
	
	enemy.init(md3dDevice,.5);
	pyramid.init(md3dDevice);
	mBox.init(md3dDevice, 1);
	origin.init(md3dDevice, 10);
	baseBox.init(md3dDevice);
	selectBox.init(md3dDevice,2.1);
	line.init(md3dDevice, 10);
	Rpyramid.init(md3dDevice);
	Gpyramid.init(md3dDevice);
	Bpyramid.init(md3dDevice);
	RGpyramid.init(md3dDevice);
	RBpyramid.init(md3dDevice);
	GBpyramid.init(md3dDevice);
	
	D3DXMATRIX temp;

	for(int i =0; i< 100; i++)
	{
		attacks[i] = NULL;
	}
	for(int i =0; i< 100; i++)
	{
		attackTime[i] = 0;
	}

	for(int i = 0; i < 15*MAXENEMIES; ++i)
	{
//		enemies.Add(200, 0, 0, 1, 1);
		int r(rand()%6);
		enemies.Add(100+(400.0*(pow((rand()%(i+1))/150.0, 2.0))),  (r%2), (r > 2), (r <= 2), 1);
//		enemies[i] = new Enemy();
//		enemies[i]->init(mTech,mfxWVPVar,&enemy, D3DXCOLOR(enemies[i]->red, enemies[i]->green, enemies[i]->blue, 1));
		/*			Rpyramid.init(md3dDevice);
					Gpyramid.init(md3dDevice);
					Bpyramid.init(md3dDevice);
					RGpyramid.init(md3dDevice);
					RBpyramid.init(md3dDevice);
					GBpyramid.init(md3dDevice);*/
		enemies[i]->init(mTech,mfxWVPVar,(enemies[i]->red?(enemies[i]->green?(enemies[i]->blue?&pyramid:&RGpyramid):(enemies[i]->blue?&RBpyramid:&Rpyramid)):(enemies[i]->green?(enemies[i]->blue?&GBpyramid:&Gpyramid):(enemies[i]->blue?&Bpyramid:&enemy))));
		enemies[i]->scale = 0.5;		
		enemies[i]->velocity = Vector3(2,0,0);
		enemies[i]->position = Vector3(0-2*i,2,WIDTH+1);
		enemies[i]->targetPos = hiveMind.getStart();
		/*enemies[i]->health = 100;
		enemies[i]->red = 1;
		enemies[i]->blue = 0;
		enemies[i]->green = 0;*/
		enemies[i]->ready = false;
		enemies[i]->finished = false;
	}

	for(int i = 0; i < LENGTH; ++i)
	{
		for(int j = 0; j < HEIGHT; ++j)
		{
			for(int k = 0; k < WIDTH; ++k)
			{
				towers[i][k] = tower();
				if(j == 0 || (j == 1 && (k == 0 || k == WIDTH - 1 || ((k != WIDTH / 2) && (i == 0 || i == LENGTH - 1)))))
				{
					D3DXMatrixTranslation(&temp,2*i+1,2*j+1,2*k+1);
					baseObj[i][j][k] = new GeoObject();
					baseObj[i][j][k]->init(mTech,mfxWVPVar,&baseBox);
					baseObj[i][j][k]->position = Vector3(2*i+1,2*j+1,2*k+1);
					baseObj[i][j][k]->world = temp;
				}
				else
				{
					baseObj[i][j][k] = NULL;
				}
			}
		}
	}
	//go1.init(mTech, mfxWVPVar, &mBox);
	originObj.init(mTech, mfxWVPVar, &origin);
	//go3.init(mTech, mfxWVPVar, &pyramid);
	
	selectObj.init(mTech, mfxWVPVar, &selectBox);
	selectObj.position = Vector3(LENGTH-.1,2.1,WIDTH-.1);
	score = 2000;
	lives = 10;

	for(int i = 0; i < enemies.size(); ++i)
	{
		enemies[i]->update(0);
	}
	//D3DXMatrixTranslation(&temp,21,3,11);
	//go1.world = temp;
	//D3DXMatrixTranslation(&temp,21,4,11);
	//go3.world = temp;
}

void ColoredCubeApp::onResize()
{
	D3DApp::onResize();

	float aspect = (float)mClientWidth/mClientHeight;
	D3DXMatrixPerspectiveFovLH(&mProj, 0.25f*PI, aspect, 1.0f, 1000.0f);
}

void ColoredCubeApp::updateScene(float dt)
{
	//dt *= 5;
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
	switch (gameState)
	{
	case 0:
		if(input.wasKeyPressed(' '))
		{
			audio.playCue(DISCO);
			gameState = 2;
		}

		break;
	case 1:
		if(enemiesDone >= MAXENEMIES*currentWave)
		{
			if(currentWave == 15)
			{
				if(lives <= 0)
				{
					gameState = 4;
				}else{
					gameState = 3;
				}
				break;
			}
			gameState = 2;
			currentWave++;
		}
		//Updates Enemy pos
		for(int i = 0; i < enemies.size(); ++i)
		{
			if(enemies[i])
			{
				int eX = int((enemies[i]->position.x-.75)/2)-1;
				int eZ = int((enemies[i]->position.z)/2)-1;
				//enemies[i]->targetPos = D3DXVECTOR2(selectObj.position.x+1, selectObj.position.z+1);
				if(!enemies[i]->finished && (hiveMind.getEnd() == D3DXVECTOR2(eX, eZ) && enemies[i]->ready))
				{
					audio.playCue(WILHELM);
					lives--;
					if(lives <= 0)
					{
						gameState = 4;
						break;
					}
					enemies[i]->finished = true;
					enemiesDone++;
				}

				if(hiveMind.validLocation(eX, eZ))
				{
					if(!enemies[i]->played)
					{
						enemiesOut++;
						enemies[i]->played = true;
					}
					if(enemies[i]->ready)
					//if(eX == static_cast<int>(enemies[i]->targetPos.x) && eZ == static_cast<int>(enemies[i]->targetPos.y))
					{
						enemies[i]->targetPos = hiveMind.bestPath(eX, eZ);
						enemies[i]->targetPos = D3DXVECTOR2(enemies[i]->targetPos.x*2 + 3, enemies[i]->targetPos.y*2 + 3);
						enemies[i]->ready = false;
						//D3DXVECTOR2 tempVel = enemies[i]->targetPos - D3DXVECTOR2(eX, eZ);
						//enemies[i]->velocity = 2*D3DXVECTOR3(tempVel.x, 0, tempVel.y);
					}
				}else{
					enemies[i]->targetPos = hiveMind.getStart();
					enemies[i]->targetPos = D3DXVECTOR2(enemies[i]->targetPos.x*2 + 3, enemies[i]->targetPos.y*2 + 3);
					enemies[i]->ready = false;
				}
				if(enemiesOut <= MAXENEMIES*currentWave || i < MAXENEMIES*currentWave)
				{
					enemies[i]->update(dt);
				}
			}

		}

	case 2:
		if(gameState == 2 && input.wasKeyPressed(' '))
		{
			done = true;
			gameState = 1;
		}
		// Update angles based on input to orbit camera around box.
		for(int i = 1; i < LENGTH-1; ++i)
		{
			int j = 2;
				for(int k = 1; k < WIDTH-1; ++k)
				{
					if(reload[i][k]>0)
						reload[i][k] = reload[i][k] - dt;
				}

		}
		for(int i =0; i< 100; i++)
		{
			if(attackTime[i] > 0)
				attackTime[i] = attackTime[i] - dt;
		}


		if(GetAsyncKeyState('A') & 0x8000)
		{
			mTheta -= 2.0f*dt;
		}
		if(GetAsyncKeyState('D') & 0x8000)
		{
			mTheta += 2.0f*dt;
		}
		if(GetAsyncKeyState('W') & 0x8000)	
		{
			mPhi -= 2.0f*dt;
		}
		if(GetAsyncKeyState('S') & 0x8000)	
		{
			mPhi += 2.0f*dt;
		}

		// Restrict the angle mPhi.
		if( mPhi < 0.1f )	mPhi = 0.1f;
		if( mPhi > PI-0.1f)	mPhi = PI-0.1f;

		//if( mPhi < 0 )	{mPhi += 2*PI; mTheta += PI;}
		//if( mPhi > PI)	{mPhi -= 2*PI; mTheta += PI;}

		// Convert Spherical to Cartesian coordinates: mPhi measured from +y
		// and mTheta measured counterclockwise from -z.
		xf =  45.0f*sinf(mPhi)*sinf(mTheta) + 20;
		yf =  45.0f*cosf(mPhi);
		zf = -45.0f*sinf(mPhi)*cosf(mTheta) + 10;

		// Build the view matrix.
		pos = D3DXVECTOR3(xf, yf, zf);
		target = D3DXVECTOR3(20.0f, 0.0f, 10.0f);
		up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		D3DXMatrixLookAtLH(&mView, &pos, &target, &up);

		if(input.wasKeyPressed(VK_LEFT) && selectObj.position.x > 3)
		{
			selectObj.position.x -= 2;
		}
		if(input.wasKeyPressed(VK_RIGHT) && selectObj.position.x < LENGTH*2 - 5)
		{
			selectObj.position.x += 2;
		}
		if(input.wasKeyPressed(VK_UP) && selectObj.position.z < WIDTH*2 - 5)
		{
			selectObj.position.z += 2;
		}
		if(input.wasKeyPressed(VK_DOWN) && selectObj.position.z > 3)	
		{
			selectObj.position.z -= 2;
		}
		
		selectObj.update(dt);

		x = int(selectObj.position.x/2)+1;
		y = 1;
		z = int(selectObj.position.z/2)+1;
		if(!baseObj[x][y][z])
		{
			selected = location::EMPTY;
		}
		else
		{
			if(baseObj[x][y+1][z])
				selected = location::TOWER;
			else
				selected = location::BASE;
		}
		if(input.wasKeyPressed('Z') && gameState == 2 && !baseObj[x][y][z] && score >= 50)	
		{
			if(hiveMind.canAddBlock(x-1, z-1))
			{
				hiveMind.addBlock(x-1, z-1);
				score = score - 50;
				D3DXMatrixTranslation(&temp,2*x+1,2*y+1,2*z+1);
				baseObj[x][y][z] = new GeoObject();
				baseObj[x][y][z]->init(mTech,mfxWVPVar,&mBox);
				baseObj[x][y][z]->world = temp;
				baseObj[x][y][z]->position = Vector3(2*x+1,2*y+1,2*z+1);
				
			}
		}
		else if(input.wasKeyPressed('1') && baseObj[x][y][z] && score >= cost(towers[x][z]))
		{
			int c = cost(towers[x][z]);
			if(baseObj[x][y+1][z] && towers[x][z].r <= 2)
				delete baseObj[x][y+1][z];
			else if(!baseObj[x][y+1][z])
				towers[x][z] = tower(0,0,0);
			
			if(towers[x][z].r <= 2)
			{
				score -= c;
				++towers[x][z].r;
				D3DXMatrixTranslation(&temp,2*x+1,2*y+2,2*z+1);
				baseObj[x][y+1][z] = new GeoObject();
				if(towers[x][z].g && towers[x][z].b)
				baseObj[x][y+1][z]->init(mTech,mfxWVPVar,&pyramid);
				else if(towers[x][z].g)
					baseObj[x][y+1][z]->init(mTech,mfxWVPVar,&RGpyramid);
				else if(towers[x][z].b)
					baseObj[x][y+1][z]->init(mTech,mfxWVPVar,&RBpyramid);
				else
					baseObj[x][y+1][z]->init(mTech,mfxWVPVar,&Rpyramid);
				baseObj[x][y+1][z]->world = temp;
				baseObj[x][y+1][z]->setRadius(6*(1+.1*(towers[x][z].r+towers[x][z].g+towers[x][z].b-1)));
				baseObj[x][y+1][z]->position = Vector3(2*x+1,2*y+2,2*z+1);
			}
		}
		else if(input.wasKeyPressed('2') && baseObj[x][y][z] && score >= cost(towers[x][z]))
		{
			int c = cost(towers[x][z]);
			if(baseObj[x][y+1][z] && towers[x][z].g <= 2)
				delete baseObj[x][y+1][z];
			else if(!baseObj[x][y+1][z])
				towers[x][z] = tower(0,0,0);
			
			if(towers[x][z].g <= 2)
			{
				score -= c;
				++towers[x][z].g;
				D3DXMatrixTranslation(&temp,2*x+1,2*y+2,2*z+1);
				baseObj[x][y+1][z] = new GeoObject();
				if(towers[x][z].r && towers[x][z].b)
					baseObj[x][y+1][z]->init(mTech,mfxWVPVar,&pyramid);
				else if(towers[x][z].r)
					baseObj[x][y+1][z]->init(mTech,mfxWVPVar,&RGpyramid);
				else if(towers[x][z].b)
					baseObj[x][y+1][z]->init(mTech,mfxWVPVar,&GBpyramid);
				else
					baseObj[x][y+1][z]->init(mTech,mfxWVPVar,&Gpyramid);
				baseObj[x][y+1][z]->world = temp;
				baseObj[x][y+1][z]->setRadius(6*(1+.1*(towers[x][z].r+towers[x][z].g+towers[x][z].b-1)));
				baseObj[x][y+1][z]->position = Vector3(2*x+1,2*y+2,2*z+1);
			}
		}
		else if(input.wasKeyPressed('3') && baseObj[x][y][z] && score >= cost(towers[x][z]))
		{
			int c = cost(towers[x][z]);
			if(baseObj[x][y+1][z] && towers[x][z].b <= 2)
				delete baseObj[x][y+1][z];
			else if(!baseObj[x][y+1][z])
				towers[x][z] = tower(0,0,0);
			
			if(towers[x][z].b <= 2)
			{
				score -= c;
				++towers[x][z].b;
				D3DXMatrixTranslation(&temp,2*x+1,2*y+2,2*z+1);
				baseObj[x][y+1][z] = new GeoObject();
				if(towers[x][z].r && towers[x][z].g)
					baseObj[x][y+1][z]->init(mTech,mfxWVPVar,&pyramid);
				else if(towers[x][z].r)
					baseObj[x][y+1][z]->init(mTech,mfxWVPVar,&RBpyramid);
				else if(towers[x][z].g)
					baseObj[x][y+1][z]->init(mTech,mfxWVPVar,&GBpyramid);
				else
					baseObj[x][y+1][z]->init(mTech,mfxWVPVar,&Bpyramid);
				baseObj[x][y+1][z]->world = temp;
				baseObj[x][y+1][z]->setRadius(6*(1+.1*(towers[x][z].r+towers[x][z].g+towers[x][z].b-1)));
				baseObj[x][y+1][z]->position = Vector3(2*x+1,2*y+2,2*z+1);
			}
		}
		else if(input.wasKeyPressed('X') && baseObj[x][y][z])
		{
			audio.playCue(BEEP1);
			hiveMind.removeBlock(x-1, z-1);
			for(int i = y; i < 5; ++i) 
			{
				delete baseObj[x][i][z];
				baseObj[x][i][z] = NULL;
			}
		}
		for(int i = 1; i < LENGTH-1; ++i)
		{
			int j = 2;
				for(int k = 1; k < WIDTH-1; ++k)
				{
					if(baseObj[i][j][k] != NULL && reload[i][k] <= 0)
					{
						for(int m = 0; m < enemies.size(); m++)
						{
							if(enemies[m]->getActiveState() && !enemies[m]->finished)
								done = false;
							if(enemies[m]->getActiveState() && enemies[m]->played)
							if(baseObj[i][j][k]->collided(enemies[m]))
							{
								int index = 0;
								if(towers[i][k].r+towers[i][k].g+towers[i][k].b>8)
									reload[i][k]=20;
								else if(towers[i][k].r+towers[i][k].g+towers[i][k].b>6)
									reload[i][k]=40;
								else if(towers[i][k].r+towers[i][k].g+towers[i][k].b>4)
									reload[i][k]=60;
								else if(towers[i][k].r+towers[i][k].g+towers[i][k].b>2)
									reload[i][k]=80;
								else
									reload[i][k]=100;
								for(int n = 0; n<100; n++)
								{
									if(attackTime[n]< 0)
									{
										index = n;
									}
								}
								if(enemies[m]->health > 0)
								{
									int damage = 0;
									int difference = 0;
									difference = towers[i][k].r - enemies[m]->red;
									if(difference > 0)
									{
										damage += 30*difference;
									}
									difference = towers[i][k].g - enemies[m]->green;
									if(difference > 0)
									{
										damage += 30*difference;
									}
									difference = towers[i][k].b - enemies[m]->blue;
									if(difference > 0)
									{
										damage += 30*difference;
									}
									enemies[m]->health -= damage;
									if(enemies[m]->health <= 0)
									{
										enemies[m]->setInActive();
										enemiesDone++;
										score += 10+5*(enemies[m]->red+enemies[m]->green+enemies[m]->blue);
									}
								}
								attackTime[index] = 15;
								D3DXMatrixTranslation(&temp,1,1,1);
								attacks[index] = new GeoObject();
								attacks[index]->init(mTech,mfxWVPVar,&line,
									Vertex(D3DXVECTOR3(i*2,j*2+.5,k*2), D3DXCOLOR(towers[i][k].r/4.0, towers[i][k].g/4.0, towers[i][k].b/4.0, 1.0f)),
								Vertex(D3DXVECTOR3(enemies[m]->getPosition().x-.5,enemies[m]->getPosition().y-.75,enemies[m]->getPosition().z-.5), D3DXCOLOR(towers[i][k].r/3.0, towers[i][k].g/3.0, towers[i][k].b/3.0, 1.0f)));
								attacks[index]->world = temp;
								audio.playCue(LAZER);
								break;
							}
						}
						}
					}
				}

		

		if(done)
			gameState = 2;
		break;
	
	default:
		
		break;
	}
	input.clearAll();
}

void ColoredCubeApp::drawScene()
{
       D3DApp::drawScene();

       // Restore default states, input layout and primitive topology 
       // because mFont->DrawText changes them.  Note that we can 
       // restore the default states by passing null.
       md3dDevice->OMSetDepthStencilState(0, 0);
       float blendFactors[] = {0.0f, 0.0f, 0.0f, 0.0f};
       md3dDevice->OMSetBlendState(0, blendFactors, 0xffffffff);
    md3dDevice->IASetInputLayout(mVertexLayout);
       //abstracted
    //md3dDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_LINELIST);
       
       mWVP = mView*mProj;
       //go1.draw(&mWVP);
       //originObj.draw(&mWVP);
       //go3.draw(&mWVP);
       if(gameState == 1 || gameState == 2)
       {
       for(int i = 0; i < LENGTH; ++i)
       {
              for(int j = 0; j < HEIGHT; ++j)
              {
                     for(int k = 0; k < WIDTH; ++k)
                     {
                           if(baseObj[i][j][k])
                                  baseObj[i][j][k]->draw(&mWVP);
                     }
              }
       }
       for(int n = 0; n<100; n++)
       {
              if(attackTime[n]> 0)
              {
                     attacks[n]->draw(&mWVP);
              }
       }
       for(int i = 0; i < enemies.size(); ++i)
              if(enemies[i])
                     enemies[i]->draw(&mWVP);

       selectObj.draw(&mWVP);
       }
       
       //FOLLOWING CODE IS ABSTRACTED NOW
       //mfxWVPVar->SetMatrix((float*)&mWVP);
       /*
    D3D10_TECHNIQUE_DESC techDesc;
    mTech->GetDesc( &techDesc );
    for(UINT p = 0; p < techDesc.Passes; ++p)
    {
        mTech->GetPassByIndex( p )->Apply(0);
        
              go1.draw(&mWVP);
              go2.draw(&mWVP);
              go3.draw(&mWVP);
              
    }
       */

       // We specify DT_NOCLIP, so we do not care about width/height of the rect.
       RECT R = {5, 5, 0, 0};
       mFont->DrawText(0, mFrameStats.c_str(), -1, &R, DT_NOCLIP, BLACK);
       if(gameState == 0)
       {
              std::stringstream p, q, r, s, t1, t2, u, v, w;
              p << "Welcome to your first day as the Color Custodian.";
              theText.print(p.str(),240, 200);
              q << "You must clean up the wild Disco Fragments.";
              theText.print(q.str(),260, 220);
              r << "Your tools will obliterate the Fragments.";
              theText.print(r.str(),280, 240);
              s << "Enemies of the same color are resistant to that color";
              theText.print(s.str(),235, 260);
              t1 << "Z will place a block, but CANNOT be placed mid round ";
              theText.print(t1.str(),233, 280);
              t2 << "X will delete a block, and arrow keys navigate.";
              theText.print(t2.str(),260, 300);
              u << "1 will buy or upgrade red, 2 is green, and 3 blue.";
              theText.print(u.str(),255, 320);
              v << "Use WASD to direct the camera.";
              theText.print(v.str(),300, 340);
              w << "Press Space to Begin";
              theText.print(w.str(),340, 360);
       }
       else if(gameState == 2 || gameState == 1)
       {
              std::stringstream s, q, r, p, t;
			  t << "Wave number " << currentWave;
              theText.print(t.str(),300, 40);
              if(selected == location::EMPTY)
              {
                     s << "Pay $50 to place base";
                     theText.print(s.str(),400, 0);
              }
              if(selected == location::BASE)
              {
                     s << "Pay $100 to place tower";
                     theText.print(s.str(),400, 0);
              }
              if(selected == location::TOWER)
              {
                     int x = int(selectObj.position.x/2)+1;
                     int z = int(selectObj.position.z/2)+1;
                     if((towers[x][z].r+towers[x][z].g+towers[x][z].b)<9)
                           s << "Pay $"<<cost(towers[x][z])<<" to upgrade tower";
                     else
                           s << "Tower maxed!";
                     theText.print(s.str(),400, 0);
                     p << "Red: ";
                     if(towers[x][z].r < 3)
                     {
                           p<<"  "<< towers[x][z].r;
                     }
                     else
                           p<< "MAX";
                     p <<" Green: ";
                     if(towers[x][z].g < 3)
                     {
                           p<<"  "<< towers[x][z].g;
                     }
                     else
                           p<< "MAX";
                     p <<" Blue: ";
                     if(towers[x][z].b < 3)
                     {
                           p<<"  "<< towers[x][z].b;
                     }
                     else
                           p<< "MAX";
                     theText.print(p.str(),400, 20);
              }
              r << "Lives: " << lives;
              theText.print(r.str(),0, 20);
              q << "$ " << score;
              theText.print(q.str(),0, 0);
       }
       else if (gameState == 3)
       {
              std::stringstream p, q, s;
              p << "Congratulations! You have won";
              theText.print(p.str(),270, 200);
              q << "With " << lives << " lives remaining!";
              theText.print(q.str(),290, 220);
			  s << "With $" << score << " remaining!";
              theText.print(s.str(),300, 240);
       }
       else if (gameState == 4)
       {
              std::stringstream p, q, s, r;
              p << "Congratulations! You have lost";
              theText.print(p.str(),270, 200);
              q << "Better luck next time!";
              theText.print(q.str(),290, 220);
			  s << "With $" << score << " remaining!";
              theText.print(s.str(),290, 240);
			  r << "On wave " << currentWave;
              theText.print(r.str(),330, 260);
       }
       
       mSwapChain->Present(0, 0);
}

void ColoredCubeApp::buildFX()
{
	DWORD shaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
    shaderFlags |= D3D10_SHADER_DEBUG;
	shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif
 
	ID3D10Blob* compilationErrors = 0;
	HRESULT hr = 0;
	hr = D3DX10CreateEffectFromFile(L"color.fx", 0, 0, 
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

void ColoredCubeApp::buildVertexLayouts()
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

int ColoredCubeApp::cost(tower t)
{
	int sum = t.r+t.b+t.g;
	switch(sum)
	{
	case 0:
		return 100;
	case 1:
		return 150;
	case 2:
		return 200;
	case 3:
		return 250;
	case 4:
		return 300;
	case 5:
		return 350;
	case 6:
		return 400;
	case 7:
		return 450;
	case 8:
		return 500;
	}

}
