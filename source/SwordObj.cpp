#include "SwordObj.h"

void SwordObj::init(	ID3D10EffectTechnique* t, 
						ID3D10EffectMatrixVariable* f,
						ID3D10EffectMatrixVariable* w,
						Geometry* g, Vertex v1, Vertex v2)
{
	up = -1;
	power = 1;
	rising = false;
	cuts = 1;

	theta = -1;	
	reset = false;
	hit = false;
	setRadius(150);

	mfxDiffuseMapVar = mFX->GetVariableByName("gDiffuseMap")->AsShaderResource();
	mfxSpecMapVar    = mFX->GetVariableByName("gSpecMap")->AsShaderResource();
	mfxTexMtxVar     = mFX->GetVariableByName("gTexMtx")->AsMatrix();

	 HR(D3DX10CreateShaderResourceViewFromFile(md3dDevice, 
		L"Textures\\Metal1.jpg", 0, 0, &mDiffuseMapRV, 0 ));
	 
	 HR(D3DX10CreateShaderResourceViewFromFile(md3dDevice, 
		L"Textures/defaultspec.dds", 0, 0, &mSpecMapRV, 0 ));

	 GeoObject::init(t,f,w,g,v1,v2);
}

		
void SwordObj::update(float dt)
{
	if (!active) return;

	if(rising)
		power = min(MAX_POWER, power + dt);
	else if(power > 1 && theta == -1)
		release();
	
	if (theta != -1)
	{
		vel += -up*SWING_ACCEL*power*dt;
		theta += vel * dt;
	}
	
	if(vel < 0)
		reset = true;

	if(hit)
	{
		if(!reset)
		{
			vel = 0;
			up = 1;
			reset = true;
		}
	}
	
	else if (up == -1 && theta > MAX_THETA)
	{
		up = 1;
		//theta = MAX_THETA;
	}
	else if(up == 1 && theta < MAX_THETA && theta > 0)
	{
		up = -1;
	}
	

	if (reset && vel > 0 || theta < 0 && theta != -1) //((theta < .1 && reset) || theta == -1)
	{
		power = 1;
		hit = false;
		reset = false;
		vel = 0;
		theta = -1;
	}

	if(theta == -1)
	{
		rotx = 0+(power-1)/14;
		rotz = PI/7;
		roty = -angle+PI+PI/7;
	}
	else
	{
		rotx = theta*3/2;
		rotz = PI/7+theta*2;
		roty = -angle+PI+PI/7;
	}
	position += (power-1)*Vector3(0,5,0);
	
	GeoObject::update(dt);
}

void SwordObj::draw(Matrix* vp)
{
	mfxDiffuseMapVar->SetResource(mDiffuseMapRV);
	mfxSpecMapVar->SetResource(mSpecMapRV);
	
	D3DXMATRIX texMtx;
	D3DXMatrixIdentity(&texMtx);
	mfxTexMtxVar->SetMatrix((float*)&texMtx);

	GeoObject::draw(vp);

}
void SwordObj::release()
{
	reset = false;
	theta = 0;
	up = -1;
	
	vel = 0;
	hit = false;
}
bool SwordObj::swing()
{
	if(theta != -1)// && theta != MAX_THETA)
		return false;

	bool ret = true;
	if(rising)
		ret = false;
	rising = true;
	
	return ret;
}

int SwordObj::getDamage()
{
	int dmg =  SWORD_BASE_DAMAGE + rand()%SWORD_DAMAGE_VARIATION;
	if(theta > MAX_THETA)
		dmg += power*(theta-MAX_THETA)/MAX_THETA*SWORD_SWING_SCALING;
	else
		dmg += power*theta/MAX_THETA*SWORD_SWING_SCALING;

	return dmg;
}
bool SwordObj::viable()
{
	return !hit && !reset && vel > 0 && (theta > MAX_THETA/4 && theta < 3*MAX_THETA/4) || (theta > 5*MAX_THETA/4 && theta < 7*MAX_THETA/4);
}