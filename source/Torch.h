#include "GeoObject.h"
#include "Fireball.h"
#include  "audio.h"

class Torch : public GeoObject
{
public:
	Torch(){}

	virtual void update(float dt);
	virtual void draw(Matrix* vp);

	virtual void init(	ID3D10EffectTechnique* t, 
						ID3D10EffectMatrixVariable* f,
						ID3D10EffectMatrixVariable* w,
						Fireball* _light, Fireball* gameFB,
						Geometry* g, Audio* a,
						Vector3 pos);

	void setDevice(ID3D10Device* d) { md3dDevice = d; }
	void setMFX(ID3D10Effect* fx) { mFX = fx; }

	bool isLit() {return fire->getActiveState();}

private:

	Fireball* fire, *fb;
	Audio* audio;

	ID3D10Effect* mFX;
	ID3D10Device* md3dDevice;	
	ID3D10ShaderResourceView* mDiffuseMapRV;
	ID3D10ShaderResourceView* mSpecMapRV;

	ID3D10EffectShaderResourceVariable* mfxDiffuseMapVar;
	ID3D10EffectShaderResourceVariable* mfxSpecMapVar;
	ID3D10EffectMatrixVariable* mfxTexMtxVar;
};