#ifndef Constants_H
#define Constants_H

//Defines to make life easier
#define LP_DXFONT   LPD3DXFONT
#define Vector3		D3DXVECTOR3
#define Matrix		D3DXMATRIX
#define Identity	D3DXMatrixIdentity
#define Translate	D3DXMatrixTranslation
#define RotateX		D3DXMatrixRotationX
#define RotateY		D3DXMatrixRotationY
#define RotateZ		D3DXMatrixRotationZ
#define Scale		D3DXMatrixScaling
#define ToRadian	D3DXToRadian
#define ToDegree	D3DXToDegree
#define Color		D3DXCOLOR
#define COLOR_ARGB	DWORD
#define ZERO		Vector3(0,0,0)
#define SETCOLOR_ARGB(a,r,g,b) \
    ((COLOR_ARGB)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))

// audio files required by audio.cpp
// WAVE_BANK must be location of .xwb file.
const char WAVE_BANK[]  = "audio\\Win\\WavesExample1.xwb";
// SOUND_BANK must be location of .xsb file.
const char SOUND_BANK[] = "audio\\Win\\SoundsExample1.xsb";

// audio cues
const char FIREBALL_CUE[] = "Fireball";
const char TORCH_CUE[] = "torch_lit";
const char BACKGROUND_CUE[] = "Background";
const char BAR_BACKGROUND_CUE[] = "Bar2_Background";
const char BEACH_CUE[] = "Beach";
const char FOREST_CUE[] = "Forest";
const char ZELDA_CUE[] = "Zelda_Success";
const char DAMAGE_CUE[] = "Damage";
const char DEATH_CUE[] = "goblin death";
const char SWING_CUE[] = "Hooh";
const char HEARTBEAT_CUE[] = "Heartbeat";
const char BOSS_CUE[] = "Pokemon_battle";
const char VICTORY_CUE[] = "Pokemon_victory";

//const double PI = 3.14159265;
const int GAME_WIDTH =  1280;               // width of game in pixels
const int GAME_HEIGHT = 800;               // height of game in pixels
template <typename T>
inline void safeRelease(T& ptr)
{
    if (ptr)
    { 
        ptr->Release(); 
        ptr = NULL;
    }
}

#endif