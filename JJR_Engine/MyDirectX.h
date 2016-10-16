//  Beginning Game Programming
//  MyDirectX.h

#pragma once

//header files
#define WIN32_EXTRA_LEAN
#define DIRECTINPUT_VERSION 0x0800
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>
#include <xinput.h>
#include <ctime>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <io.h>
#include <algorithm>
#include <vector>
#include <stdlib.h> // srand, rand

//libraries
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"user32.lib")
#pragma comment(lib,"gdi32.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"xinput.lib")

// debugger window
extern HWND debugWin;

//program values
extern const std::string APPTITLE;
extern const int SCREENW;
extern const int SCREENH;
extern const int scaleRes;
extern bool gameover;
extern const bool FULLSCREEN;
extern bool debugOn;
extern int prevKey;
extern int prevBut;
extern std::vector<std::string> debugNames;
extern std::vector<std::string> debugValues;
extern std::string assets; 

//macro to detect key presses
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)

//Direct3D objects
extern LPDIRECT3D9 d3d;
extern LPDIRECT3DDEVICE9 d3ddev;
extern LPD3DXSPRITE spriteobj;
extern LPDIRECT3DSURFACE9 backbuffer;
extern LPDIRECT3DSURFACE9 background;

//sprite structure
struct SPRITE
{
	float x, y;
	int frame, columns;
	int width, height;
	float scaling, rotation;
	int startframe, endframe;
	int starttime, delay;
	int direction;
	float velx, vely;
	D3DCOLOR color;

	SPRITE()
	{
		frame = 0;
		columns = 1;
		width = height = 0;
		scaling = 1.0f;
		rotation = 0.0f;
		startframe = endframe = 0;
		direction = 1;
		starttime = delay = 0;
		velx = vely = 0.0f;
		color = D3DCOLOR_XRGB(255, 255, 255);
	}
};

//allow quick string conversion anywhere in the program
template <class T>
std::string static ToString(const T & t, int places = 2)
{
	std::ostringstream oss;
	oss.precision(places);
	oss.setf(std::ios_base::fixed);
	oss << t;
	return oss.str();
}

// Takes a string and converts it to a wide string
std::wstring s2ws(const std::string& s);

//Direct3D functions
bool Direct3D_Init(HWND hwnd, int width, int height, bool fullscreen);
void Direct3D_Shutdown();
LPDIRECT3DSURFACE9 LoadSurface(std::string filename);
void DrawSurface(LPDIRECT3DSURFACE9 dest, float x, float y, LPDIRECT3DSURFACE9 source);
LPDIRECT3DTEXTURE9 LoadTexture(std::string filename, D3DCOLOR transcolor = D3DCOLOR_XRGB(0, 0, 0));
void Sprite_Draw_Frame(LPDIRECT3DTEXTURE9 texture, int destx, int desty,
	int framenum, int framew, int frameh, int columns);
void Sprite_Animate(int &frame, int startframe, int endframe, int direction, int &starttime, int delay);

void Sprite_Transform_Draw(LPDIRECT3DTEXTURE9 image, int x, int y, int width, int height,
	int frame = 0, int columns = 1, float rotation = 0.0f,
	float scaleW = 1.0f, float scaleH = 1.0f, D3DCOLOR color = D3DCOLOR_XRGB(255, 255, 255));

void Sprite_Transform_Draw(LPDIRECT3DTEXTURE9 image, int x, int y, int width, int height,
	int frame = 0, int columns = 1, float rotation = 0.0f, float scaling = 1.0f,
	D3DCOLOR color = D3DCOLOR_XRGB(255, 255, 255));

//bounding box collision detection
int Collision(SPRITE sprite1, SPRITE sprite2);

//distance based collision detection
bool CollisionD(SPRITE sprite1, SPRITE sprite2);

//DirectInput objects, devices, and states
extern LPDIRECTINPUT8 dinput;
extern LPDIRECTINPUTDEVICE8 dimouse;
extern LPDIRECTINPUTDEVICE8 dikeyboard;
extern DIMOUSESTATE mouse_state;
extern XINPUT_GAMEPAD controllers[4];

//DirectInput functions
bool DirectInput_Init(HWND);
void DirectInput_Update();
void DirectInput_Shutdown();
bool Key_Down(int);
bool Key_Down_Once(int);	// only allows once key press at a time
bool Button_Down(int);		// used for Xbox 360 controllers
bool Button_Down_Once(int);
bool noKey();				// returns true if nothing is pressed
bool noButton();			// returns true if nothing is pressed
int Mouse_Button(int);
int Mouse_X();
int Mouse_Y();
void XInput_Vibrate(int contNum = 0, int amount = 65535);
bool XInput_Controller_Found();
void Vibrate(int contnum, int amount, int length); // vibrates controller
void updateVibration(); // updates vibration so it doesn't run forever
void clearVibration();	// for when we want the bad vibes to go

//game functions
bool Game_Init(HWND window);
void Game_Run(HWND window);
void Game_End();


//font functions
LPD3DXFONT MakeFont(std::string name, int size);
void FontPrint(LPD3DXFONT font, int x, int y, std::string text, D3DCOLOR color = D3DCOLOR_XRGB(255, 255, 255));

// template function to populate debugNames and debugValues for the debugger window
template<class T>
inline void debugDisplay(std::string name, T val)
{
	debugNames.push_back((name + '\n')); 
	debugValues.push_back(ToString(val) + '\n');
}
// empty out the debug vectors
void clearDebug();

// Extra functions to help with game creation
bool Create_Background(std::string bgName, int bgWidth, int bgHeight);
void Draw_Background();

//DirectSound code added in chapter 11
#include "DirectSound.h"

#pragma comment(lib,"dsound.lib")
#pragma comment(lib,"dxerr.lib")

//primary DirectSound object
extern CSoundManager *dsound;

//function prototypes
bool DirectSound_Init(HWND hwnd);
void DirectSound_Shutdown();
CSound *LoadSound(std::string filename);
void PlaySound(CSound *sound);
void LoopSound(CSound *sound);
void StopSound(CSound *sound);


//define the MODEL struct
struct MODEL
{
	LPD3DXMESH mesh;
	D3DMATERIAL9* materials;
	LPDIRECT3DTEXTURE9* textures;
	DWORD material_count;
	D3DXVECTOR3 translate;
	D3DXVECTOR3 rotate;
	D3DXVECTOR3 scale;

	MODEL()
	{
		material_count = 0;
		mesh = NULL;
		materials = NULL;
		textures = NULL;
		translate = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		rotate = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	}
};

//3D mesh function prototypes
void DrawModel(MODEL *model);
void DeleteModel(MODEL *model);
MODEL *LoadModel(std::string filename);
bool FindFile(std::string *filename);
bool DoesFileExist(const std::string &filename);
void SplitPath(const std::string& inputPath, std::string* pathOnly, std::string* filenameOnly);
void SetCamera(float posx, float posy, float posz, float lookx = 0.0f, float looky = 0.0f, float lookz = 0.0f);