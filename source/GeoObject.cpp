
#include "GeoObject.h"
#include "Vertex.h"

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

bool GeoObject::collided(GeoObject *gameObject)
{
	Vector3 diff = position - gameObject->getPosition();
	float length = D3DXVec3LengthSq(&diff);
	float radii = radiusSquared + gameObject->getRadiusSquare();
	if (length <= radii)
		return true;
	return false;
}