#include "Fireball.h"
#include "Camera.h"

void Fireball::init(	ID3D10EffectTechnique* t, 
						ID3D10EffectMatrixVariable* f,
						ID3D10EffectMatrixVariable* w,
						Camera* c,
						Geometry* g,
						int count)
{
	ready = true;
	rising = false;
	power = 1;
	scale = 1;
	 dist = 0;	
	 setRadius(1);
	 velocity = Vector3(0,0,0);

	 cam = c;	
	
	fireballSprites.init(md3dDevice, count);	

	mfxDiffuseMapVar = mFX->GetVariableByName("gDiffuseMap")->AsShaderResource();
	mfxSpecMapVar    = mFX->GetVariableByName("gSpecMap")->AsShaderResource();
	mfxTexMtxVar     = mFX->GetVariableByName("gTexMtx")->AsMatrix();

	 HR(D3DX10CreateShaderResourceViewFromFile(md3dDevice, 
		L"Textures/flare.dds", 0, 0, &mDiffuseMapRV, 0 ));
	 
	 HR(D3DX10CreateShaderResourceViewFromFile(md3dDevice, 
		L"Textures/defaultspec.dds", 0, 0, &mSpecMapRV, 0 ));

	 GeoObject::init(t,f,w,g);	 
}

		
void Fireball::update(float dt)
{
	setRadius(power);

	if(rising)
	{
		power = min(MAX_FIRE_POWER,power+dt);
		light->att = Vector3(0,.03,0)/power;
		light->range = power*1000.0f;
		
	}
		//power = 1;
		
	dist += dt * D3DXVec3Length(&velocity);

	if (dist > MAX_DIST)
	{
		light->on = 0;
		dist = 0;
		setInActive();
		ready = true;
	}
	
	if (!active) return;
	light->pos += velocity * dt;
	
	fireballSprites.update(dt);
	
	GeoObject::update(dt);
}

void Fireball::draw(Matrix* vp)
{
	if (!active) return;

	mfxDiffuseMapVar->SetResource(mDiffuseMapRV);
	mfxSpecMapVar->SetResource(mSpecMapRV);
	
	D3DXMATRIX texMtx;
	D3DXMatrixIdentity(&texMtx);
	mfxTexMtxVar->SetMatrix((float*)&texMtx);
	
	//GeoObject::draw(vp);
	
	fireballSprites.draw(cam->getPos(), *vp);
	
}


void Fireball::release(Vector3 pos, Vector3 dir)
{
	ready = false;
	setPosition(pos + 200*dir);
	setVelocity(dir*FIREBALL_SPEED);
	fireballSprites.setPath(position, velocity);
}

void Fireball::shoot2(Vector3 pos, Vector3 dir)
{
	setPosition(pos + 200*dir);
	fireballSprites.setPath(position, Vector3(0,0,0), false);
	fireballSprites.scale = power;
	setVelocity(dir);
	power = MAX_FIRE_POWER;
	light->dir = dir;
	light->pos = position - 10*dir;
	light->on = 1;
	light->att = Vector3(0,.03,0)/power;
	light->range = power*1000.0f;
	rising = false;
}


bool Fireball::shoot(Vector3 pos, Vector3 dir)
{
	dist = 0;
	bool ret = rising;
	
	setPosition(pos + 200*dir);
	fireballSprites.setPath(position, Vector3(0,0,0), false);
	fireballSprites.scale = power;
	setVelocity(Vector3(0,0,0));
		
	setActive();
		
	light->dir = dir;
	light->pos = position - 10*dir;
	light->on = 1;

	if(rising)
		cam->addShake(.6*(power-1.0f));
	else
	{
		power = 1;
	}
	//setVelocity(dir*FIREBALL_SPEED);

	
	//}
	//else
	//{
		//setVelocity(dir*FIREBALL_SPEED);
		//fireballSprites.setPath(position, velocity);
	//}
	rising = true;
	
	return !ret;
}

int Fireball::getDamage()
{
	return (power)*(FIREBALL_BASE_DAMAGE + rand()%FIREBALL_DAMAGE_VARIATION);
}
