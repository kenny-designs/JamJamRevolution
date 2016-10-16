// Contains all functions, classes, etc..
// needed for Jam Jam Revolution
#pragma once

// definitions
#define LOAD_CONTROLS 0
#define LOAD_MENU 1
#define LOAD_SONG_ONE 2
#define LOAD_SONG_TWO 3
#define LOAD_SONG_THREE 4
#define LOAD_CREDITS 5
#include "MyDirectX.h"

extern int currentLevel;
extern int spawnCounter;
extern int intScore;

// bold arrows 
extern LPDIRECT3DTEXTURE9 upArrow;
extern LPDIRECT3DTEXTURE9 downArrow;
extern LPDIRECT3DTEXTURE9 leftArrow;
extern LPDIRECT3DTEXTURE9 rightArrow;
extern LPDIRECT3DTEXTURE9 aButton;
extern LPDIRECT3DTEXTURE9 bButton;
// d3dsurface_desc for bold arrows
extern D3DSURFACE_DESC upArrowDesc;
extern D3DSURFACE_DESC downArrowDesc;
extern D3DSURFACE_DESC leftArrowDesc;
extern D3DSURFACE_DESC rightArrowDesc;
extern D3DSURFACE_DESC aButtonDesc;
extern D3DSURFACE_DESC bButtonDesc;

// arrows player needs to hit
extern LPDIRECT3DTEXTURE9 upHit;
extern LPDIRECT3DTEXTURE9 downHit;
extern LPDIRECT3DTEXTURE9 leftHit;
extern LPDIRECT3DTEXTURE9 rightHit;
extern LPDIRECT3DTEXTURE9 aHit;
extern LPDIRECT3DTEXTURE9 bHit;
// d3dsurface_desc for hit arrows
extern D3DSURFACE_DESC upHitDesc;
extern D3DSURFACE_DESC downHitDesc;
extern D3DSURFACE_DESC leftHitDesc;
extern D3DSURFACE_DESC rightHitDesc;
extern D3DSURFACE_DESC aHitDesc;
extern D3DSURFACE_DESC bHitDesc;

class SPRITE_PROMPTS : public SPRITE 
{
public:
	float hitX, hitY;				// coords for the hit arrow
	float speedMult;				// how quickly the arrow moves
	D3DXVECTOR2 arrowDir;			// direction arrow is moving in
	int hitWidth, hitHeight;		// width and height of the hit arrow
	char promptType;				// the type of prompt to draw
	int radius, hitRadius;			// radius of main and hit arrows for collision testing
	void drawPrompt();				// draws the prompt and it's corresponding hit prompt
	void findDir();					// gives the direction the main arrow moves in
	void moveToHit();				// moves the main arrow towards its hit
	bool confirmHit(char input);	// test if the user hit the arrow
	bool outBounds();				// if prompt is out of bounds, return false to be destroyed
};

// a class for our score reports jammin, nice, cool, and bad
class SPRITE_SCORED : public SPRITE
{
public:
	char scoredType;	// j = jammin, n = nice, c = cool, and b = bad
	float speedMult;	// rate score report moves
	void drawScored();	// draws score report to the screen
	void moveScored();	// moves the score report up
	bool outBounds();	// score report destroyed once it goes outside of bounds
};

// class for our transition sprites
class SPRITE_TRANSITION : public SPRITE
{
public:
	char transType;		// o = songOne, t = songTwo, f = songFinal
	float speedMult;	// rate transition moves
	void drawTrans();	// draw transition onto screen
	void moveTrans();	// moves the trans
	void outBounds();	// destroy score and set transNow to false
};


// class that holds all the data needed for every level
class GAME_LEVELS
{
public:
	// font variables
	LPD3DXFONT font = NULL;
	// start transition
	SPRITE_TRANSITION levelTransition;

	//timing variables
	DWORD refresh = 0;
	DWORD screentime = 0;
	double screenfps = 0.0;
	double screencount = 0.0;
	DWORD coretime = 0;
	double corefps = 0.0;
	double corecount = 0.0;
	DWORD currenttime;

	~GAME_LEVELS(); // release memory for arrows, sounds, etc...
};

// controls screen. only shows once
class GAME_CONTROLS : public GAME_LEVELS
{
public:
	int controlTime = 0; // once over 400 we go to main menu

	GAME_CONTROLS();
	void gameRun(HWND window);
};

// main menu level
class GAME_MENU : public GAME_LEVELS
{
public:
	int levelSelect = 0;
	GAME_MENU();
	void Draw_Score(); // draws final score over scoreboard
	void gameRun(HWND window);
	~GAME_MENU(); // reset score and everything
};

// credit level
class GAME_CREDIT : public GAME_LEVELS
{
public:
	GAME_CREDIT();
	void gameRun(HWND window);
};

// song one level
class GAME_SONG_ONE : public GAME_LEVELS
{
public:
	CSound *levelMusic = NULL;
	float levelSpeed;

	void initLevel();
	void Draw_HUD();
	void gameRun(HWND window); // the games loop for this particular level
};

// song two level
class GAME_SONG_TWO : public GAME_SONG_ONE
{
public:
	void initLevel();
};

// song three level
class GAME_SONG_THREE : public GAME_SONG_ONE
{
public:
	void initLevel();
};

// vector to track our arrows
extern std::vector<SPRITE_PROMPTS> allPrompts;

// load all prompt textures
bool loadPrompts();
// load all sounds
bool loadSounds();

// spawns prompt with random attributes or given input
SPRITE_PROMPTS promptSpawner(float speed, const int scaling, char promptChar = ' ');
// spawns a new prompt every so often
void promptPeriodicSpawn(float speed);
// check on the status of all prompts and deal with them accordingly
void allPromptUpdate(char input);
// check on the status of all score reports and deal with them accordingly
void allScoredUpdate();
// checks if the user pressed anything and returns a char
char userInputProtocol();
// draw the scoreboard at the end of the game
void drawScoreBoard();