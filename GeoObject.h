
#ifndef GEOOBJECT_H
#define GEOOBJECT_H

//#include "C:\Users\weaverem1\Dropbox\S14\Games2\PartII\Common\d3dUtil.h"
#include "d3dUtil.h"
#include "Geometry.h"
#include "constants.h"

class GeoObject
{
public:

	GeoObject();
	~GeoObject();

	virtual void init(ID3D10EffectTechnique* t, ID3D10EffectMatrixVariable* f, Geometry* g);
	virtual void init(ID3D10EffectTechnique* t, ID3D10EffectMatrixVariable* f, Geometry* g, Vertex v1, Vertex v2);
	virtual void draw(D3DXMATRIX* vp);
	
	virtual void update(float dt);

	void setPosition (Vector3 pos) {position = pos;}
	Vector3 getPosition() {return position;}
	void setVelocity (Vector3 vel) {velocity = vel;}
	Vector3 getVelocity() {return velocity;}
	void setSpeed(float s) {speed = s;}
	float getSpeed() {return speed;}
	void setRadius(float r) {radius = r; radiusSquared = (scale*r)*(scale*r);}
	float getRadiusSquare() {return radiusSquared;}
	float getRadius() {return radius;}
	Matrix getWorldMatrix() {return world;}
	void setScale(float s) {scale = s; radiusSquared = (s*radius)*(s*radius);}
	float getScale() {return scale;}
	void setActive() {active = true;}
	void setInActive() {active = false;}
	bool getActiveState() {return active;}
	void setMTech(ID3D10EffectTechnique* m){ tech = m;}
	bool collided(GeoObject *gameObject);
	void setVert1(Vertex v){geom->setVert1(v);}
	void setVert2(Vertex v){geom->setVert2(v);}
	Vertex getPoint(){return geom->getPoint();}

	Geometry* geom;
	D3DXMATRIX world, wvp;

	//ID3D10Device* md3dDevice;
	ID3D10EffectTechnique* tech;
	ID3D10EffectMatrixVariable* fxMatrix;

	Vector3 position;
	Vector3 velocity;
	float speed;
	float radius;
	float radiusSquared;
	bool active;
	float scale;
	float rotx,roty,rotz;

};


#endif 
