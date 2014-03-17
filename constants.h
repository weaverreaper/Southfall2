#ifndef Constants_H
#define Constants_H

//Defines to make life easier
#define LP_DXFONT   LPD3DXFONT
#define Vector3 D3DXVECTOR3
#define Matrix D3DXMATRIX
#define Identity D3DXMatrixIdentity
#define Translate D3DXMatrixTranslation
#define RotateX D3DXMatrixRotationX
#define RotateY D3DXMatrixRotationY
#define RotateZ D3DXMatrixRotationZ
#define Scale D3DXMatrixScaling
#define ToRadian D3DXToRadian
#define ToDegree D3DXToDegree
#define COLOR_ARGB DWORD
#define SETCOLOR_ARGB(a,r,g,b) \
    ((COLOR_ARGB)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))

// audio files required by audio.cpp
// WAVE_BANK must be location of .xwb file.
const char WAVE_BANK[]  = "audio\\Win\\WavesExample1.xwb";
// SOUND_BANK must be location of .xsb file.
const char SOUND_BANK[] = "audio\\Win\\SoundsExample1.xsb";

// audio cues
const char BEEP1[] = "beep1";
const char BEEP2[] = "beep2";
const char BEEP3[] = "beep3";
const char BEEP4[] = "beep4";
const char HIT[]   = "hit";
const char LAZER[] = "lazer";
const char DISCO[] = "disco";
const char WILHELM[] = "wilhelm";

const static int MAXENEMIES = 20;


//const double PI = 3.14159265;
const int GAME_WIDTH =  640;               // width of game in pixels
const int GAME_HEIGHT = 480;               // height of game in pixels
template <typename T>
inline void safeRelease(T& ptr)
{
    if (ptr)
    { 
        ptr->Release(); 
        ptr = NULL;
    }
}
enum location {EMPTY, BASE, TOWER};
struct tower
{
	int r,g,b;
	tower():r(0),g(0),b(0){}
	tower(int r,int g,int b):r(r),g(g),b(b){}

};

#endif