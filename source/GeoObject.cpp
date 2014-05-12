
#include "GeoObject.h"
#include "Vertex.h"


float findHeight2(float z3, float z1, float z2, float a, float b){
float t = z1*(1-a/(1-b))+z2*a/(1-b);
return t*(1-b)+z3*b;
}
GeoObject::GeoObject()
{
	scale = 1;
	radius = 0;
	radiusSquared = 0;
	speed = 0;
	rotx = roty = rotz = 0;
	active = true;
	Identity(&world);
	Identity(&wvp);
	velocity = Vector3(0,0,0);
	position = Vector3(0,0,0);
	geom = NULL;
	tech = NULL;
	fxMatrix = NULL;

}
 
GeoObject::~GeoObject()
{
	
}

void GeoObject::init(ID3D10EffectTechnique* t, ID3D10EffectMatrixVariable* f,ID3D10EffectMatrixVariable* w, Geometry* g)
{
	//md3dDevice = device;
	geom = g;
	fxMatrix = f;
	fxWorld = w;
	tech = t;
	D3DXMatrixIdentity(&world);
	D3DXMatrixIdentity(&wvp);
}
void GeoObject::init(ID3D10EffectTechnique* t, ID3D10EffectMatrixVariable* f,ID3D10EffectMatrixVariable* w, Geometry* g, Terrain* ter)
{
	//md3dDevice = device;
	geom = g;
	fxMatrix = f;
	fxWorld = w;
	tech = t;
	terr = ter;
	offset = Vector3(0,0,0);
	D3DXMatrixIdentity(&world);
	D3DXMatrixIdentity(&wvp);
}

void GeoObject::init(ID3D10EffectTechnique* t,
					ID3D10EffectMatrixVariable* f, 
					ID3D10EffectMatrixVariable* w, 
					Geometry* g, Vertex v1, Vertex v2)
{
	geom = g;
	fxMatrix = f;
	fxWorld = w;
	tech = t;
	geom->setVert1(v1);
	geom->setVert2(v2);
	offset = Vector3(0,0,0);
	D3DXMatrixIdentity(&world);
	D3DXMatrixIdentity(&wvp);
}

void GeoObject::draw(D3DXMATRIX* vp)
{
	if(!active) return;

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

void GeoObject::update(float dt)
{
	position += velocity*dt;
	Matrix temp;
	Identity(&temp);
	Identity(&world);
	D3DXMatrixRotationYawPitchRoll(&temp, roty, rotz, rotx);
	world *= temp;
	Scale(&temp,scale,scale,scale);
	world *= temp;
	Translate(&temp, position.x, position.y, position.z);
	world *= temp;	
}
void GeoObject::update2(float dt)
{
	position += velocity*dt;
	Matrix temp;
	Identity(&temp);
	Identity(&world);
	position.y = getTerrHeight()+15;
	Translate(&temp, offset.x, offset.y, offset.z);
	world *= temp;
	D3DXMatrixRotationYawPitchRoll(&temp, roty, rotz, rotx);
	world *= temp;
	Scale(&temp,scale,scale,scale);
	world *= temp;
	Translate(&temp, position.x, position.y, position.z);
	world *= temp;	
}


float GeoObject::getTerrHeight()
{
	float y = 0;
	int tx = int(position.x/terr->scale);
	int tz = int(position.z/terr->scale);
	float rx = position.x/terr->scale - tx;
	float rz = position.z/terr->scale - tz;
	if(rx + rz <= 1 && rx > rz)
		y = max(findHeight2(terr->grid[tx][tz+1],terr->grid[tx][tz],terr->grid[tx+1][tz],rx,rz), 
			findHeight2(terr->grid[tx][tz],terr->grid[tx+1][tz],terr->grid[tx+1][tz+1],rz,1-rx));
	else if(rx + rz > 1 && rx > rz)
		y = max(findHeight2(terr->grid[tx][tz],terr->grid[tx+1][tz],terr->grid[tx+1][tz+1],rz,1-rx), 
			findHeight2(terr->grid[tx+1][tz],terr->grid[tx+1][tz+1],terr->grid[tx][tz+1],1-rx,1-rz));
	else if(rx + rz > 1 && rx <= rz)
		y = max(findHeight2(terr->grid[tx+1][tz],terr->grid[tx+1][tz+1],terr->grid[tx][tz+1],1-rx,1-rz), 
			findHeight2(terr->grid[tx+1][tz+1],terr->grid[tx][tz+1],terr->grid[tx][tz],1-rz,rx));
	else if(rx + rz <= 1 && rx <= rz)
		y = max(findHeight2(terr->grid[tx+1][tz+1],terr->grid[tx][tz+1],terr->grid[tx][tz],1-rz,rx), 
			findHeight2(terr->grid[tx][tz+1],terr->grid[tx][tz],terr->grid[tx+1][tz],rx,rz));
	return y*terr->scale;

}

bool GeoObject::collided(GeoObject *gameObject)
{
	if (!active || !gameObject->getActiveState()) return false;
	Vector3 diff = position - gameObject->getPosition();
	float length = D3DXVec3LengthSq(&diff);
	float radii = radiusSquared + gameObject->getRadiusSquare();
	if (length <= radii)
		return true;
	return false;
}