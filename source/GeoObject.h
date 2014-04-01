
#ifndef GEOOBJECT_H
#define GEOOBJECT_H

#include "d3dUtil.h"
#include "Geometry.h"
#include "constants.h"

class GeoObject
{
public:

	GeoObject();
	~GeoObject();

	virtual void init(	ID3D10EffectTechnique* t, 
						ID3D10EffectMatrixVariable* f,
						ID3D10EffectMatrixVariable* w,
						Geometry* g, Vertex v1, Vertex v2);

	virtual void draw(D3DXMATRIX* vp);
	
	virtual void update(float dt);

	//Setters
	void setPosition (Vector3 pos)			{position = pos;}
	void setVelocity (Vector3 vel)			{velocity = vel;}
	void setSpeed(float s)					{speed = s;}
	void setRadius(float r)					{radius = r; radiusSquared = (scale*r)*(scale*r);}
	void setScale(float s)					{scale = s; radiusSquared = (s*radius)*(s*radius);}
	void setActive()						{active = true;}
	void setInActive()						{active = false;}
	void setVert1(Vertex v)					{geom->setVert1(v);}
	void setVert2(Vertex v)					{geom->setVert2(v);}
	void setMTech(ID3D10EffectTechnique* m)	{ tech = m;}

	//Getters
	Vertex getPoint()						{return geom->getPoint();}
	Vector3 getVelocity()					{return velocity;}	
	Vector3 getPosition()					{return position;}	
	Matrix getWorldMatrix()					{return world;}	
	float getSpeed()						{return speed;}	
	float getRadiusSquare()					{return radiusSquared;}
	float getRadius()						{return radius;}
	float getScale()						{return scale;}
	bool getActiveState()					{return active;}

	bool collided(GeoObject *gameObject);		

protected:

	Geometry* geom;
	D3DXMATRIX world, wvp;

	//ID3D10Device* md3dDevice;
	ID3D10EffectTechnique* tech;
	ID3D10EffectMatrixVariable* fxMatrix;
	ID3D10EffectMatrixVariable* fxWorld;

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
