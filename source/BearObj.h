//=======================================================================================
// Bear.h by Nate Higgins
//=======================================================================================

#ifndef BearObj_H
#define BearObj_H

//#include "C:\Users\weaverem1\Dropbox\S14\Games2\PartII\Common\d3dUtil.h"
#include "d3dUtil.h"
#include "GeoObject.h"
#include "Fireball.h"
#include "SwordObj.h"

class BearObj : public GeoObject
{
public:

	BearObj();
	~BearObj();
	void update(float dt, Vector3 cam);
	void BearObj::update(float dt, Vector3 cam, Fireball* fo, SwordObj* so);
	
private:
	int health;
};


#endif // BOX_H
