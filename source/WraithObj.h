//=======================================================================================
// Wraith.h by Nate Higgins
//=======================================================================================

#ifndef WraithObj_H
#define WraithObj_H

//#include "C:\Users\weaverem1\Dropbox\S14\Games2\PartII\Common\d3dUtil.h"
#include "d3dUtil.h"
#include "GeoObject.h"
#include "Fireball.h"
#include "SwordObj.h"
#include "audio.h"
#include <vector>
#include "DamageSprites.h"

class WraithObj : public GeoObject
{
public:

	WraithObj();
	~WraithObj();
	void init2(ID3D10EffectTechnique* t, ID3D10EffectMatrixVariable* f,ID3D10EffectMatrixVariable* w,ID3D10Device* device, Geometry* g, Terrain* ter);
	void update(float dt, Vector3 cam);
	void draw(D3DXMATRIX* vp);
	int health;
	void WraithObj::update(float dt, Vector3 cam, Fireball* fo, SwordObj* so);
	bool done(){return dmgfx.empty() && health < 0;}
	void setAudio(Audio* a) {audio = a;}
	void setDiffuseMap(ID3D10ShaderResourceView* dm){mDiffuseMapRV = dm;}
	
private:

	std::vector<DamageSprites*> dmgfx;
	bool firstDraw;
	Vector3 direction, offset;

	ID3D10Device* md3dDevice;	
	ID3D10ShaderResourceView* mDiffuseMapRV;
	ID3D10ShaderResourceView* mSpecMapRV;

	ID3D10EffectShaderResourceVariable* mfxDiffuseMapVar;
	ID3D10EffectShaderResourceVariable* mfxSpecMapVar;
	ID3D10EffectMatrixVariable* mfxTexMtxVar;

	Audio* audio;
};


#endif // BOX_H
