//=======================================================================================
// Wraith.cpp by Nate Higgins
//=======================================================================================

#include "WraithObj.h"

const float KNOCKBACK = 50;

WraithObj::WraithObj()
{
	health = 400;
	radius = 25;
}
 
WraithObj::~WraithObj()
{
	;
}
void WraithObj::draw(D3DXMATRIX* vp)
{
	
	if(active)
	{
		mfxDiffuseMapVar->SetResource(mDiffuseMapRV);
		mfxSpecMapVar->SetResource(mSpecMapRV);

		D3DXMatrixMultiply(&wvp,&world,vp);
		fxMatrix->SetMatrix((float*)&(wvp));
		fxWorld->SetMatrix((float*)&world);

		D3D10_TECHNIQUE_DESC techDesc;
		tech->GetDesc( &techDesc );
		for(UINT p = 0; p < techDesc.Passes; ++p)
		{
			tech->GetPassByIndex( p )->Apply(0);
			geom->draw();
		}
	}

	if(firstDraw)
	{
		std::vector<DamageSprites*>::iterator ds = dmgfx.begin();
		while(ds != dmgfx.end())
		{
			(*ds)->draw(*vp);
			++ds;
		}
		firstDraw = false;
	}
}

void WraithObj::init2(ID3D10EffectTechnique* t, ID3D10EffectMatrixVariable* f,ID3D10EffectMatrixVariable* w,ID3D10Device* device, Geometry* g, Terrain* ter)
{
	//md3dDevice = device;
	md3dDevice = device;

	mfxDiffuseMapVar = mFX->GetVariableByName("gDiffuseMap")->AsShaderResource();
	mfxSpecMapVar    = mFX->GetVariableByName("gSpecMap")->AsShaderResource();
	mfxTexMtxVar     = mFX->GetVariableByName("gTexMtx")->AsMatrix();

	 HR(D3DX10CreateShaderResourceViewFromFile(md3dDevice, 
		L"Textures/wraith.jpg", 0, 0, &mDiffuseMapRV, 0 ));
	 
	 HR(D3DX10CreateShaderResourceViewFromFile(md3dDevice, 
		L"Textures/defaultspec.dds", 0, 0, &mSpecMapRV, 0 ));
	geom = g;
	fxMatrix = f;
	fxWorld = w;
	tech = t;
	terr = ter;
	D3DXMatrixIdentity(&world);
	D3DXMatrixIdentity(&wvp);
}
void WraithObj::update(float dt, Vector3 cam, Fireball* fo, SwordObj* so)
{
	firstDraw = true;
	std::vector<DamageSprites*>::iterator ds = dmgfx.begin();
	while(ds != dmgfx.end())
	{
		(*ds)->update(cam, position, dt);
		if((*ds)->done())
		{
			delete (*ds);
			ds = dmgfx.erase(ds);
		}
		else
			++ds;
	}
	if (!getActiveState())
		return;
	if(this->collided(fo) && fo->viable())
	{		
		int dHealth = fo->getDamage();
		health -= dHealth;
		dmgfx.push_back(new DamageSprites());
		dmgfx.back()->init(md3dDevice, dHealth);
		fo->setInActive();
		//fo->light->on = 0;
		//fo->dist = 0;
		audio->playCue(DAMAGE_CUE);
	}
	if(this->collided(so) && so->viable())
	{
		setPosition(getPosition() + direction*KNOCKBACK);
		int dHealth = so->getDamage();
		health -= dHealth;
		dmgfx.push_back(new DamageSprites());
		dmgfx.back()->init(md3dDevice, dHealth);
		so->hit = true;
		audio->playCue(DAMAGE_CUE);
	}
	if(health <= 0)
	{
		setInActive();
		audio->playCue(DEATH_CUE);
	}
	update(dt,cam);
}
void WraithObj::update(float dt, Vector3 cam)
{
	direction = position - cam;
	float rot = atan2f(direction.x,direction.z)+ToRadian(90);
	if(D3DXVec3Length(&direction) < 1000)
	{
	D3DXVec3Normalize(&direction,&direction);
	Vector3 v = -direction*100;
	v.y = 0;
	roty = rot;
	velocity = v;
	position += velocity*dt;
	}
	Matrix temp;
	Identity(&temp);
	Identity(&world);
	position.y = getTerrHeight()+45;
	D3DXMatrixRotationYawPitchRoll(&temp, roty, rotz, rotx);
	world *= temp;
	Scale(&temp,scale,scale,scale);
	world *= temp;
	Translate(&temp, position.x, position.y, position.z);
	world *= temp;
}
//BREAK------------------------------
//
//-----------------------------------
