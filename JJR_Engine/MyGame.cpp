// JJR Engine
// MyGame.cpp

#include "jamJamRev.h"
#include "resource.h"

const std::string APPTITLE = "Jam Jam Revolution";
const int SCREENW = 800; // 800 for upscale, 160 original
const int SCREENH = 720; // 720 for upscale, 144 original
const int scaleRes = SCREENW / 160; // how much we upscale by, if any
const bool FULLSCREEN = FALSE;
bool debugOn = true; // set true to use debugger window
std::string assets = "Assets/";

GAME_CONTROLS *gameControls;
GAME_MENU *gameMenu;
GAME_CREDIT *gameCredit;
GAME_SONG_ONE *gameSongOne;
GAME_SONG_TWO *gameSongTwo;
GAME_SONG_THREE *gameSongThree;

bool Game_Init(HWND window)
{
	Direct3D_Init(window, SCREENW, SCREENH, FULLSCREEN);
	DirectInput_Init(window);
	DirectSound_Init(window);

	return true;
}

void Game_End()
{
	if (gameControls)
	{
		delete gameControls;
		gameControls = NULL;
	}
	if (gameMenu)
	{
		delete gameMenu;
		gameMenu = NULL;
	}
	if (gameCredit)
	{
		delete gameCredit;
		gameCredit = NULL;
	}
	if (gameSongOne)
	{
		delete gameSongOne;
		gameSongOne = NULL;
	}
	if (gameSongTwo)
	{
		delete gameSongTwo;
		gameSongTwo = NULL;
	}
	if (gameSongThree)
	{
		delete gameSongThree;
		gameSongThree = NULL;
	}

	DirectSound_Shutdown();
	DirectInput_Shutdown();
	Direct3D_Shutdown();
}

void Game_Run(HWND window)
{
	int prevLevel = currentLevel;
	switch (currentLevel)
	{
	case LOAD_CONTROLS:
	{
		if (!gameControls)
			gameControls = new GAME_CONTROLS();
		gameControls->gameRun(window);
		break;
	}
	case LOAD_MENU:
	{
		if(!gameMenu)
			gameMenu = new GAME_MENU();
		gameMenu->gameRun(window);
		break;
	}

	case LOAD_SONG_ONE:
	{
		if (!gameSongOne)
		{
			gameSongOne = new GAME_SONG_ONE();
			gameSongOne->initLevel();
		}
		gameSongOne->gameRun(window);
		break;
	}
	case LOAD_SONG_TWO:
	{
		if (!gameSongTwo)
		{
			gameSongTwo = new GAME_SONG_TWO();
			gameSongTwo->initLevel();
		}
		gameSongTwo->gameRun(window);
		break;
	}
	case LOAD_SONG_THREE:
	{
		if (!gameSongThree)
		{
			gameSongThree = new GAME_SONG_THREE();
			gameSongThree->initLevel();
		}
		gameSongThree->gameRun(window);
		break;
	}
	case LOAD_CREDITS:
	{
		if (!gameCredit)
		{
			gameCredit = new GAME_CREDIT();
		}
		gameCredit->gameRun(window);
		break;
	}
	default:
		currentLevel = LOAD_MENU;
		break;
	}
	if (prevLevel != currentLevel)
	{
		if (gameControls)
		{
			delete gameControls;
			gameControls = NULL;
		}
		if (gameMenu)
		{
			delete gameMenu;
			gameMenu = NULL;
		}
		if (gameCredit)
		{
			delete gameCredit;
			gameCredit = NULL;
		}
		if (gameSongOne)
		{
			delete gameSongOne;
			gameSongOne = NULL;
		}
		if (gameSongTwo)
		{
			delete gameSongTwo;
			gameSongTwo = NULL;
		}
		if (gameSongThree)
		{
			delete gameSongThree;
			gameSongThree = NULL;
		}
	}
}
