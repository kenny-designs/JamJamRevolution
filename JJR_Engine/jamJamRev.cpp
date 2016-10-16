#include "jamJamRev.h"

int currentLevel	= LOAD_CONTROLS;
int spawnCounter	= 0;
int intScore		= 0;
int jamScore		= 0;
int niceScore		= 0;
int coolScore		= 0;
int badScore		= 0;
int missedScore		= 0;
bool showScoreBoard = false; // set to true when you want to see the scoreboard on main menu
bool transNow		= false; // game is in transition, don't spawn prompts	

time_t lastSpawnTime = NULL;
std::string lastKey = "None"; // for debugging
// vector to track our arrows
std::vector<SPRITE_PROMPTS> allPrompts;
// vector to track our score reports
std::vector<SPRITE_SCORED> allScored;
// vectors to track for debugging
std::vector<std::string> debugNames;
std::vector<std::string> debugValues;

LPDIRECT3DTEXTURE9 upArrow		= NULL;
LPDIRECT3DTEXTURE9 downArrow	= NULL;
LPDIRECT3DTEXTURE9 leftArrow	= NULL;
LPDIRECT3DTEXTURE9 rightArrow	= NULL;
LPDIRECT3DTEXTURE9 aButton		= NULL;
LPDIRECT3DTEXTURE9 bButton		= NULL;

D3DSURFACE_DESC upArrowDesc;
D3DSURFACE_DESC downArrowDesc;
D3DSURFACE_DESC leftArrowDesc;
D3DSURFACE_DESC rightArrowDesc;
D3DSURFACE_DESC aButtonDesc;
D3DSURFACE_DESC bButtonDesc;

LPDIRECT3DTEXTURE9 upHit	= NULL;
LPDIRECT3DTEXTURE9 downHit	= NULL;
LPDIRECT3DTEXTURE9 leftHit	= NULL;
LPDIRECT3DTEXTURE9 rightHit	= NULL;
LPDIRECT3DTEXTURE9 aHit		= NULL;
LPDIRECT3DTEXTURE9 bHit		= NULL;

D3DSURFACE_DESC upHitDesc;
D3DSURFACE_DESC downHitDesc;
D3DSURFACE_DESC leftHitDesc;
D3DSURFACE_DESC rightHitDesc;
D3DSURFACE_DESC aHitDesc;
D3DSURFACE_DESC bHitDesc;

LPDIRECT3DTEXTURE9 scored_jammin	= NULL;
LPDIRECT3DTEXTURE9 scored_nice		= NULL;
LPDIRECT3DTEXTURE9 scored_cool		= NULL;
LPDIRECT3DTEXTURE9 scored_bad		= NULL;

D3DSURFACE_DESC jamminDesc;
D3DSURFACE_DESC niceDesc;
D3DSURFACE_DESC coolDesc;
D3DSURFACE_DESC badDesc;

LPDIRECT3DTEXTURE9 scoreBoard		= NULL;
D3DSURFACE_DESC scoreboardDesc;

LPDIRECT3DTEXTURE9 transOne			= NULL;
LPDIRECT3DTEXTURE9 transTwo			= NULL;
LPDIRECT3DTEXTURE9 transFinal		= NULL;

D3DSURFACE_DESC transOneDesc;
D3DSURFACE_DESC transTwoDesc;
D3DSURFACE_DESC transFinalDesc;

CSound *snd_songOne		= NULL;
CSound *snd_songTwo		= NULL;
CSound *snd_songThree	= NULL;
CSound *snd_menuMusic	= NULL;
CSound *snd_creditMusic = NULL;

void SPRITE_PROMPTS::drawPrompt()
{
	LPDIRECT3DTEXTURE9 mainPrompt = NULL;
	LPDIRECT3DTEXTURE9 hitPrompt = NULL;

	if (promptType == 'u')
	{
		mainPrompt	= upArrow;
		hitPrompt	= upHit;
	}
	else if (promptType == 'd')
	{
		mainPrompt	= downArrow;
		hitPrompt	= downHit;
	}
	else if (promptType == 'l')
	{
		mainPrompt	= leftArrow;
		hitPrompt	= leftHit;
	}
	else if (promptType == 'r')
	{
		mainPrompt	= rightArrow;
		hitPrompt	= rightHit;
	}
	else if (promptType == 'a')
	{
		mainPrompt	= aButton;
		hitPrompt	= aHit;
	}
	else if (promptType == 'b')
	{
		mainPrompt	= bButton;
		hitPrompt	= bHit;
	}

	Sprite_Transform_Draw(
		hitPrompt,
		hitX, hitY,
		hitWidth, hitHeight,
		0, 1, 0.0f, 1.0f, 1.0f);
	Sprite_Transform_Draw(
		mainPrompt,
		x, y,
		width, height,
		0, 1, 0.0f, 1.0f, 1.0f);
}

void SPRITE_PROMPTS::findDir()
{
	// first find distance between current pos and the hits pos
	arrowDir.x = hitX - x;
	arrowDir.y = hitY - y;

	// normalize the direction
	D3DXVec2Normalize(&arrowDir, &arrowDir);
}

void SPRITE_PROMPTS::moveToHit()
{
	x += arrowDir.x * speedMult;
	y += arrowDir.y * speedMult;
}

bool SPRITE_PROMPTS::confirmHit(char input)
{
	if (input == promptType)
	{
		// first find the center point of the main and hit prompts
		D3DXVECTOR2 mainCenter(x + width / 2, y + height / 2);
		D3DXVECTOR2 hitCenter(hitX + hitWidth / 2, hitY + hitHeight / 2);

		// Now we need to find the radius of both prompts
		int mainRad, hitRad;
		// mainRad radius
		if (width >= height)
			mainRad = width / 2;
		else
			mainRad = height / 2;
		// hitRad radius
		if (hitWidth >= hitHeight)
			hitRad = width / 2;
		else
			hitRad = height / 2;

		// return true if colliding (circle collision detection)
		int xDif, yDif, radii;
		xDif = hitCenter.x - mainCenter.x;
		yDif = hitCenter.y - mainCenter.y;
		radii = mainRad + hitRad;
		// square results
		xDif *= xDif;
		yDif *= yDif;
		radii *= radii;
		if (xDif + yDif <= (radii * 0.05f))			// Jammin'
		{
			jamScore++;
			intScore += 4;
			SPRITE_SCORED temp;
			temp.x = x + SCREENW * 0.075f;
			temp.y = y;
			temp.width = jamminDesc.Width;
			temp.height = jamminDesc.Height;
			temp.speedMult = 1.0f * scaleRes;
			temp.scoredType = 'j';
			allScored.push_back(temp);
			return true;
		}
		else if (xDif + yDif <= (radii * 0.2f))		// Nice!
		{
			niceScore++;
			intScore += 2;
			SPRITE_SCORED temp;
			temp.x = x + SCREENW * 0.075f;
			temp.y = y;
			temp.width = niceDesc.Width;
			temp.height = niceDesc.Height;
			temp.speedMult = 1.0f * scaleRes;
			temp.scoredType = 'n';
			allScored.push_back(temp);
			return true;
		}
		else if (xDif + yDif <= radii * 0.35f)		// Cool
		{
			coolScore++;
			SPRITE_SCORED temp;
			temp.x = x + SCREENW * 0.075f;
			temp.y = y;
			temp.width = coolDesc.Width;
			temp.height = coolDesc.Height;
			temp.speedMult = 1.0f * scaleRes;
			temp.scoredType = 'c';
			allScored.push_back(temp);
			return true;
		}
		else if (xDif + yDif <= radii * 2.5f)		// Bad...
		{
			badScore++;
			intScore -= 2;
			SPRITE_SCORED temp;
			temp.x = x + SCREENW * 0.075f;
			temp.y = y;
			temp.width = badDesc.Width;
			temp.height = badDesc.Height;
			temp.speedMult = 1.0f * scaleRes;
			temp.scoredType = 'b';
			allScored.push_back(temp);
			return true;
		}
	}

	return false;
}

bool SPRITE_PROMPTS::outBounds()
{
	if (y < 0 - height)
	{
		missedScore++;
		intScore -= 8;
		//vibrate controller
		Vibrate(0, 25000, 10);
		return true;
	}
	return false;
}

void SPRITE_SCORED::drawScored()
{
	LPDIRECT3DTEXTURE9 mainScored = NULL;

	if (scoredType == 'j')
		mainScored = scored_jammin;
	else if (scoredType == 'n')
		mainScored = scored_nice;
	else if (scoredType == 'c')
		mainScored = scored_cool;
	else if (scoredType == 'b')
		mainScored = scored_bad;

	Sprite_Transform_Draw(
		mainScored,
		x, y,
		width, height,
		0, 1, 0.0f, 1.0f, 1.0f);
}

void SPRITE_SCORED::moveScored()
{
	y -= speedMult;
}

bool SPRITE_SCORED::outBounds()
{
	if (y < 0 - height)
		return true;
	return false;
}

void SPRITE_TRANSITION::drawTrans()
{
	LPDIRECT3DTEXTURE9 mainTrans = NULL;

	if (transType == 'o')
		mainTrans = transOne;
	else if (transType == 't')
		mainTrans = transTwo;
	else if (transType == 'f')
		mainTrans = transFinal;

	Sprite_Transform_Draw(
		mainTrans,
		x, y,
		width, height,
		0, 1, 0.0f, 1.0f, 1.0f);
}

void SPRITE_TRANSITION::moveTrans()
{
	y -= speedMult;
}

void SPRITE_TRANSITION::outBounds()
{
	if (y < 0 - height)
		transNow = false;
}

GAME_LEVELS::~GAME_LEVELS()
{
	if (upArrow)
	{
		upArrow->Release();
		upArrow = NULL;
	}
	if (downArrow)
	{
		downArrow->Release();
		downArrow = NULL;
	}
	if (leftArrow)
	{
		leftArrow->Release();
		leftArrow = NULL;
	}
	if (rightArrow)
	{
		rightArrow->Release();
		rightArrow = NULL;
	}
	if (aButton)
	{
		aButton->Release();
		aButton = NULL;
	}
	if (bButton)
	{
		bButton->Release();
		bButton = NULL;
	}
	if (upHit)
	{
		upHit->Release();
		upHit = NULL;
	}
	if (downHit)
	{
		downHit->Release();
		downHit = NULL;
	}
	if (leftHit)
	{
		leftHit->Release();
		leftHit = NULL;
	}
	if (rightHit)
	{
		rightHit->Release();
		rightHit = NULL;
	}
	if (aHit)
	{
		aHit->Release();
		aHit = NULL;
	}
	if (bHit)
	{
		bHit->Release();
		bHit = NULL;
	}
	if (scored_jammin)
	{
		scored_jammin->Release();
		scored_jammin = NULL;
	}
	if (scored_nice)
	{
		scored_nice->Release();
		scored_nice = NULL;
	}
	if (scored_cool)
	{
		scored_cool->Release();
		scored_cool = NULL;
	}
	if (scored_bad)
	{
		scored_bad->Release();
		scored_bad = NULL;
	}
	if (scoreBoard)
	{
		scoreBoard->Release();
		scoreBoard = NULL;
	}
	if (transOne)
	{
		transOne->Release();
		transOne = NULL;
	}
	if (transTwo)
	{
		transTwo->Release();
		transTwo = NULL;
	}
	if (transFinal)
	{
		transFinal->Release();
		transFinal = NULL;
	}
	if (font)
	{
		font->Release();
		font = NULL;
	}
	if (snd_menuMusic)
	{
		delete snd_menuMusic;
		snd_menuMusic = NULL;
	}
	if (snd_songOne)
	{
		delete snd_songOne;
		snd_songOne = NULL;
	}
	if (snd_songTwo)
	{
		delete snd_songTwo;
		snd_songTwo = NULL;
	}
	if (snd_songThree)
	{
		delete snd_songThree;
		snd_songThree = NULL;
	}
	if (snd_creditMusic)
	{
		delete snd_creditMusic;
		snd_creditMusic = NULL;
	}
	if (backbuffer)
	{
		backbuffer->Release();
		backbuffer = NULL;
	}
	if (background)
	{
		background->Release();
		background = NULL;
	}

	// swap out vectors to get them empty
	std::vector<SPRITE_PROMPTS>().swap(allPrompts);
	std::vector<SPRITE_SCORED>().swap(allScored);
	std::vector<std::string>().swap(debugNames);
	std::vector<std::string>().swap(debugValues);
}

GAME_CONTROLS::GAME_CONTROLS()
{
	if (!Create_Background(assets + "JJR_Controls.bmp", 160 * scaleRes, 144 * scaleRes))
		MessageBox(NULL, "JJR_Controls.bmp not found!", APPTITLE.c_str(), 0);

	// load prompts, using for pointing to menu
	if (!loadPrompts())
		MessageBox(NULL, "Missing a Prompt!", APPTITLE.c_str(), 0);
	if (!loadSounds())
		MessageBox(NULL, "Missing a sound file!", APPTITLE.c_str(), 0);

	//create fonts 
	font = MakeFont("Arial Bold", 8 * scaleRes);
}

void GAME_CONTROLS::gameRun(HWND window)
{
	if (!d3ddev) return;
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(0, 0, 100), 1.0f, 0);

	//get current ticks
	currenttime = timeGetTime();

	//calculate core frame rate
	corecount += 1.0;
	if (currenttime > coretime + 1000)
	{
		corefps = corecount;
		corecount = 0.0;
		coretime = currenttime;
	}

	//run update at ~60 hz
	if (currenttime > refresh + 16)
	{
		refresh = currenttime;

		DirectInput_Update();

		// enter moves you to menu
		if (Key_Down_Once(DIK_RETURN) || Button_Down_Once(XINPUT_GAMEPAD_A) ||
			Key_Down_Once(DIK_J) || Key_Down_Once(DIK_Z))
			currentLevel = LOAD_MENU;

		controlTime++;
		if (controlTime >= 400)
			currentLevel = LOAD_MENU;

		//calculate screen frame rate
		screencount += 1.0;
		if (currenttime > screentime + 1000)
		{
			screenfps = screencount;
			screencount = 0.0;
			screentime = currenttime;
		}
	}

	// draw background
	Draw_Background();

	//begin rendering
	if (d3ddev->BeginScene())
	{
		spriteobj->Begin(D3DXSPRITE_ALPHABLEND);

		spriteobj->End();
		d3ddev->EndScene();
		d3ddev->Present(NULL, NULL, NULL, NULL);
	}
}

GAME_MENU::GAME_MENU()
{
	if (!Create_Background(assets + "JJR_Menu.bmp", 160 * scaleRes, 144 * scaleRes))
		MessageBox(NULL, "JJR_Menu.bmp not found!", APPTITLE.c_str(), 0);

	// load prompts, using for pointing to menu
	if (!loadPrompts())
		MessageBox(NULL, "Missing a Prompt!", APPTITLE.c_str(), 0);
	if(!loadSounds())
		MessageBox(NULL, "Missing a sound file!", APPTITLE.c_str(), 0);

	//create fonts 
	font = MakeFont("Arial Bold", 8 * scaleRes);

	PlaySound(snd_menuMusic);
}

void GAME_MENU::gameRun(HWND window)
{
	// if menu music isn't playing then play it again
	if (!snd_menuMusic->IsSoundPlaying())
	{
		snd_menuMusic->Reset();
		PlaySound(snd_menuMusic);
	}

	if (!d3ddev) return;
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(0, 0, 100), 1.0f, 0);

	//get current ticks
	currenttime = timeGetTime();

	//calculate core frame rate
	corecount += 1.0;
	if (currenttime > coretime + 1000)
	{
		corefps = corecount;
		corecount = 0.0;
		coretime = currenttime;
	}

	//run update at ~60 hz
	if (currenttime > refresh + 16)
	{
		refresh = currenttime;

		DirectInput_Update();

		// take input
		if (showScoreBoard && (Key_Down_Once(DIK_RETURN) || Button_Down_Once(XINPUT_GAMEPAD_A) ||
			Key_Down_Once(DIK_J) || Key_Down_Once(DIK_Z)))
		{
			showScoreBoard = false;
		}

		else if(showScoreBoard == false)
		{
			if (Key_Down(DIK_A) || Key_Down(DIK_LEFT) ||
				Button_Down(XINPUT_GAMEPAD_DPAD_LEFT))
			{
				levelSelect = 0;
			}
			else if (Key_Down(DIK_D) || Key_Down(DIK_RIGHT) ||
				Button_Down(XINPUT_GAMEPAD_DPAD_RIGHT))
			{
				levelSelect = 1;
			}
			else if (Key_Down_Once(DIK_RETURN) || Button_Down_Once(XINPUT_GAMEPAD_A) ||
				Key_Down_Once(DIK_J) || Key_Down_Once(DIK_Z))
			{
				switch (levelSelect)
				{
				case 0:
					currentLevel = LOAD_SONG_ONE; // load first level
					break;
				case 1:
					gameover = true; // end game
					break;
				}
			}
		}
		noKey();
		noButton();

		//calculate screen frame rate
		screencount += 1.0;
		if (currenttime > screentime + 1000)
		{
			screenfps = screencount;
			screencount = 0.0;
			screentime = currenttime;
		}
	}

	// draw background
	Draw_Background();

	//begin rendering
	if (d3ddev->BeginScene())
	{
		spriteobj->Begin(D3DXSPRITE_ALPHABLEND);

		if (levelSelect == 0)
		{
			Sprite_Transform_Draw(
				leftArrow,
				SCREENW / 2.85f, SCREENH - leftArrowDesc.Height - SCREENH * 0.05f,
				leftArrowDesc.Width, leftArrowDesc.Height,
				0, 1, 0.0f, 1.0f, 1.0f);
		}
		else if (levelSelect == 1)
		{
			Sprite_Transform_Draw(
				rightArrow,
				SCREENW / 2.85f, SCREENH - rightArrowDesc.Height - SCREENH * 0.05f,
				rightArrowDesc.Width, rightArrowDesc.Height,
				0, 1, 0.0f, 1.0f, 1.0f);
		}
		if (showScoreBoard)
		{
			drawScoreBoard();
			Draw_Score();
		}

		spriteobj->End();
		d3ddev->EndScene();
		d3ddev->Present(NULL, NULL, NULL, NULL);
	}
}

void GAME_MENU::Draw_Score()
{
	// color of font
	D3DCOLOR color = D3DCOLOR_ARGB(255, 50, 67, 41);

	//draw debug messages
	FontPrint(font, 58	* scaleRes, 47 * scaleRes, ToString(intScore),	color);
	FontPrint(font, 60	* scaleRes, 56 * scaleRes, ToString(jamScore),	color);
	FontPrint(font, 54	* scaleRes, 65 * scaleRes, ToString(niceScore), color);
	FontPrint(font, 112 * scaleRes, 47 * scaleRes, ToString(coolScore),	color);
	FontPrint(font, 114 * scaleRes, 56 * scaleRes, ToString(badScore),	color);
	FontPrint(font, 120 * scaleRes, 65 * scaleRes, ToString(missedScore), color);
}

GAME_MENU::~GAME_MENU()
{
	intScore = 0;
	jamScore = 0;
	niceScore = 0;
	coolScore = 0;
	badScore = 0;
	missedScore = 0;
}

GAME_CREDIT::GAME_CREDIT()
{
	if (!Create_Background(assets + "JJR_credits.bmp", 160 * scaleRes, 144 * scaleRes))
		MessageBox(NULL, "JJR_credits.bmp not found!", APPTITLE.c_str(), 0);

	// load prompts, using for pointing to menu
	if (!loadPrompts())
		MessageBox(NULL, "Missing a Prompt!", APPTITLE.c_str(), 0);
	if (!loadSounds())
		MessageBox(NULL, "Missing a sound file!", APPTITLE.c_str(), 0);

	//create fonts 
	font = MakeFont("Arial Bold", 8 * scaleRes);

	PlaySound(snd_creditMusic);
}

void GAME_CREDIT::gameRun(HWND window)
{
	// if menu music isn't playing then play it again
	if (!snd_creditMusic->IsSoundPlaying())
	{
		snd_creditMusic->Reset();
		PlaySound(snd_creditMusic);
	}

	if (!d3ddev) return;
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(0, 0, 100), 1.0f, 0);

	//get current ticks
	currenttime = timeGetTime();

	//calculate core frame rate
	corecount += 1.0;
	if (currenttime > coretime + 1000)
	{
		corefps = corecount;
		corecount = 0.0;
		coretime = currenttime;
	}

	//run update at ~60 hz
	if (currenttime > refresh + 16)
	{
		refresh = currenttime;
		
		DirectInput_Update();

		if (Key_Down(DIK_RETURN) || Button_Down(XINPUT_GAMEPAD_A)
			|| Key_Down(DIK_J) || Key_Down(DIK_Z))
		{
			showScoreBoard = true;
			currentLevel++;
		}

		//calculate screen frame rate
		screencount += 1.0;
		if (currenttime > screentime + 1000)
		{
			screenfps = screencount;
			screencount = 0.0;
			screentime = currenttime;
		}
	}

	// draw background
	Draw_Background();

	//begin rendering
	if (d3ddev->BeginScene())
	{
		spriteobj->Begin(D3DXSPRITE_ALPHABLEND);

		spriteobj->End();
		d3ddev->EndScene();
		d3ddev->Present(NULL, NULL, NULL, NULL);
	}
}

void GAME_SONG_ONE::initLevel()
{
	// set the level's pace
	levelSpeed = 2.5f;

	//create fonts 
	font = MakeFont("Arial Bold", 6 * scaleRes);
		
	// create background
	if (!Create_Background(assets + "JJR_Background.bmp", 160 * scaleRes, 144 * scaleRes))
		MessageBox(NULL, "JJR_Background.bmp not found!", APPTITLE.c_str(), 0);

	// load all prompts and sounds
	if (!loadPrompts()) 
		MessageBox(NULL, "Missing a Prompt!", APPTITLE.c_str(), 0);
	if (!loadSounds()) 
		MessageBox(NULL, "Missing a Sound!", APPTITLE.c_str(), 0);

	// create the level's transition
	levelTransition.width = transOneDesc.Width;
	levelTransition.height = transOneDesc.Height;
	levelTransition.x = SCREENW * 0.15f;
	levelTransition.y = SCREENH + levelTransition.height;
	levelTransition.speedMult = 1.0f * scaleRes;
	levelTransition.transType = 'o';

	// make it so the transition start
	transNow = true;

	levelMusic = snd_songOne;
	PlaySound(levelMusic);
}

void GAME_SONG_ONE::Draw_HUD()
{
	// color of font
	D3DCOLOR color = D3DCOLOR_ARGB(255, 50, 67, 41);

	//draw debug messages
	FontPrint(font, 30 * scaleRes, 0, "Score:" + ToString(intScore), color);
}

void GAME_SONG_ONE::gameRun(HWND window)
{
	// for the debugging dialog box
	clearDebug();
	debugDisplay("Last Key Press", lastKey);
	debugDisplay("Prompts On Screen", allPrompts.size());
	debugDisplay("Prompts Spawned", spawnCounter);
	debugDisplay("Score", intScore);

	// go to next level when song ends
	if (levelMusic->IsSoundPlaying())
	{
		if(!transNow)
			promptPeriodicSpawn(levelSpeed); // add new prompts every so often
	}
	else if (allPrompts.empty())
		currentLevel++;

	if (!d3ddev) return;
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(0, 0, 100), 1.0f, 0);

	//get current ticks
	currenttime = timeGetTime();

	//calculate core frame rate
	corecount += 1.0;
	if (currenttime > coretime + 1000)
	{
		corefps = corecount;
		corecount = 0.0;
		coretime = currenttime;
	}

	//run update at ~60 hz
	if (currenttime > refresh + 16)
	{
		refresh = currenttime;

		DirectInput_Update();

		// collect user input
		char userInput = userInputProtocol();

		// all logic goes here
		// follow protocol for dealing with prompts
		allPromptUpdate(userInput);
		allScoredUpdate();
		// protocol for transitions
		if (transNow)
		{
			levelTransition.moveTrans();
			levelTransition.outBounds();
		}
		// vibrate controller
		updateVibration();

		//calculate screen frame rate
		screencount += 1.0;
		if (currenttime > screentime + 1000)
		{
			screenfps = screencount;
			screencount = 0.0;
			screentime = currenttime;
			debugDisplay("FPS", screenfps);
		}

		if (KEY_DOWN(VK_ESCAPE) || Button_Down(XINPUT_GAMEPAD_BACK))
		{
			clearVibration();
			showScoreBoard = true;
			currentLevel = LOAD_MENU;
		}
	}

	// draw background
	Draw_Background();

	//begin rendering
	if (d3ddev->BeginScene())
	{
		spriteobj->Begin(D3DXSPRITE_ALPHABLEND);

		// draw all our prompts from the vector
		for (int i = 0; i < allPrompts.size(); i++)
			allPrompts[i].drawPrompt();
		// draw all our score reports
		for (int i = 0; i < allScored.size(); i++)
			allScored[i].drawScored();

		if (transNow)
			levelTransition.drawTrans();

		Draw_HUD();

		spriteobj->End();
		d3ddev->EndScene();
		d3ddev->Present(NULL, NULL, NULL, NULL);
	}
}

void GAME_SONG_TWO::initLevel()
{
	// set level's speed
	levelSpeed = 4.5f;

	//create fonts 
	font = MakeFont("Arial Bold", 6 * scaleRes);

	// create background
	if (!Create_Background(assets + "JJR_Background.bmp", 160 * scaleRes, 144 * scaleRes))
		MessageBox(NULL, "JJR_Background.bmp not found!", APPTITLE.c_str(), 0);

	// load all prompts and sounds
	if (!loadPrompts())
		MessageBox(NULL, "Missing a Prompt!", APPTITLE.c_str(), 0);
	if (!loadSounds())
		MessageBox(NULL, "Missing a Sound!", APPTITLE.c_str(), 0);

	// create the level's transition
	levelTransition.width = transTwoDesc.Width;
	levelTransition.height = transTwoDesc.Height;
	levelTransition.x = SCREENW * 0.1f;
	levelTransition.y = SCREENH + levelTransition.height;
	levelTransition.speedMult = 1.0f * scaleRes;
	levelTransition.transType = 't';

	// make it so the transition start
	transNow = true;

	levelMusic = snd_songTwo;
	PlaySound(levelMusic);
}

void GAME_SONG_THREE::initLevel()
{
	// set level's speed
	levelSpeed = 6.5f;

	//create fonts 
	font = MakeFont("Arial Bold", 6 * scaleRes);

	// create background
	if (!Create_Background(assets + "JJR_Background.bmp", 160 * scaleRes, 144 * scaleRes))
		MessageBox(NULL, "JJR_Background.bmp not found!", APPTITLE.c_str(), 0);

	// load all prompts and sounds
	if (!loadPrompts())
		MessageBox(NULL, "Missing a Prompt!", APPTITLE.c_str(), 0);
	if (!loadSounds())
		MessageBox(NULL, "Missing a Sound!", APPTITLE.c_str(), 0);

	// create the level's transition
	levelTransition.width = transFinalDesc.Width;
	levelTransition.height = transFinalDesc.Height;
	levelTransition.x = SCREENW * 0.125f;
	levelTransition.y = SCREENH + levelTransition.height;
	levelTransition.speedMult = 1.0f * scaleRes;
	levelTransition.transType = 'f';

	// make it so the transition start
	transNow = true;

	levelMusic = snd_songThree;
	PlaySound(levelMusic);
}

bool loadPrompts()
{
	// load in all up arrow prompts
	upArrow = LoadTexture(assets + "arrowUp.png");
	if (!upArrow)
	{
		MessageBox(NULL, "arrowUp.png not found!", APPTITLE.c_str(), 0);
		return false;
	}
	else
		upArrow->GetLevelDesc(0, &upArrowDesc);
	upHit = LoadTexture(assets + "upHit.png");
	if (!upHit)
	{
		MessageBox(NULL, "upHit.png not found!", APPTITLE.c_str(), 0);
		return false;
	}
	else
		upHit->GetLevelDesc(0, &upHitDesc);

	// load in all down arrow prompts
	downArrow = LoadTexture(assets + "arrowDown.png");
	if (!downArrow)
	{
		MessageBox(NULL, "arrowDown.png not found!", APPTITLE.c_str(), 0);
		return false;
	}
	else
		downArrow->GetLevelDesc(0, &downArrowDesc);
	downHit = LoadTexture(assets + "downHit.png");
	if (!downHit)
	{
		MessageBox(NULL, "downHit.png not found!", APPTITLE.c_str(), 0);
		return false;
	}
	else
		downHit->GetLevelDesc(0, &downHitDesc);

	// load in all left arrow prompts
	leftArrow = LoadTexture(assets + "arrowLeft.png");
	if (!leftArrow)
	{
		MessageBox(NULL, "arrowLeft.png not found!", APPTITLE.c_str(), 0);
		return false;
	}
	else
		leftArrow->GetLevelDesc(0, &leftArrowDesc);
	leftHit = LoadTexture(assets + "leftHit.png");
	if (!leftHit)
	{
		MessageBox(NULL, "leftHit.png not found!", APPTITLE.c_str(), 0);
		return false;
	}
	else
		leftHit->GetLevelDesc(0, &leftHitDesc);

	// load in all right arrow prompts
	rightArrow = LoadTexture(assets + "arrowRight.png");
	if (!rightArrow)
	{
		MessageBox(NULL, "arrowRight.png not found!", APPTITLE.c_str(), 0);
		return false;
	}
	else
		rightArrow->GetLevelDesc(0, &rightArrowDesc);
	rightHit = LoadTexture(assets + "rightHit.png");
	if (!rightHit)
	{
		MessageBox(NULL, "rightHit.png not found!", APPTITLE.c_str(), 0);
		return false;
	}
	else
		rightHit->GetLevelDesc(0, &rightHitDesc);

	// load in all a button prompts
	aButton = LoadTexture(assets + "aButton.png");
	if (!aButton)
	{
		MessageBox(NULL, "aButton.png not found!", APPTITLE.c_str(), 0);
		return false;
	}
	else
		aButton->GetLevelDesc(0, &aButtonDesc);
	aHit = LoadTexture(assets + "aHit.png");
	if (!aHit)
	{
		MessageBox(NULL, "aHit.png not found!", APPTITLE.c_str(), 0);
		return false;
	}
	else
		aHit->GetLevelDesc(0, &aHitDesc);

	// load in all b button prompts
	bButton = LoadTexture(assets + "bButton.png");
	if (!bButton)
	{
		MessageBox(NULL, "bButton.png not found!", APPTITLE.c_str(), 0);
		return false;
	}
	else
		bButton->GetLevelDesc(0, &bButtonDesc);
	bHit = LoadTexture(assets + "bHit.png");
	if (!bHit)
	{
		MessageBox(NULL, "bHit.png not found!", APPTITLE.c_str(), 0);
		return false;
	}
	else
		bHit->GetLevelDesc(0, &bHitDesc);

	// loading in all score reports
	// jammin score report
	scored_jammin = LoadTexture(assets + "jammin.png");
	if (!scored_jammin)
	{
		MessageBox(NULL, "jammin.png not found!", APPTITLE.c_str(), 0);
		return false;
	}
	else
		scored_jammin->GetLevelDesc(0, &jamminDesc);

	// nice score report
	scored_nice = LoadTexture(assets + "nice.png");
	if (!scored_nice)
	{
		MessageBox(NULL, "nice.png not found!", APPTITLE.c_str(), 0);
		return false;
	}
	else
		scored_nice->GetLevelDesc(0, &niceDesc);

	// cool score report
	scored_cool = LoadTexture(assets + "cool.png");
	if (!scored_cool)
	{
		MessageBox(NULL, "cool.png not found!", APPTITLE.c_str(), 0);
		return false;
	}
	else
		scored_cool->GetLevelDesc(0, &coolDesc);

	// bad score report
	scored_bad = LoadTexture(assets + "bad.png");
	if (!scored_bad)
	{
		MessageBox(NULL, "bad.png not found!", APPTITLE.c_str(), 0);
		return false;
	}
	else
		scored_bad->GetLevelDesc(0, &badDesc);

	// load in scoreboard
	scoreBoard = LoadTexture(assets + "scoreBoard.png");
	if (!scoreBoard)
	{
		MessageBox(NULL, "scoreBoard.png not found!", APPTITLE.c_str(), 0);
		return false;
	}
	else
		scoreBoard->GetLevelDesc(0, &scoreboardDesc);

	// load transitions
	// first transition
	transOne = LoadTexture(assets + "transition_1.png");
	if (!transOne)
	{
		MessageBox(NULL, "transition_1.png not found!", APPTITLE.c_str(), 0);
		return false;
	}
	else
		transOne->GetLevelDesc(0, &transOneDesc);

	// second transition
	transTwo = LoadTexture(assets + "transition_2.png");
	if (!transTwo)
	{
		MessageBox(NULL, "transition_2.png not found!", APPTITLE.c_str(), 0);
		return false;
	}
	else
		transTwo->GetLevelDesc(0, &transTwoDesc);

	// final transition
	transFinal = LoadTexture(assets + "transition_final.png");
	if (!transFinal)
	{
		MessageBox(NULL, "transition_final.png is found!", APPTITLE.c_str(), 0);
		return false;
	}
	else
		transFinal->GetLevelDesc(0, &transFinalDesc);

	return true;
}

bool loadSounds()
{
	snd_menuMusic	= LoadSound(assets + "MenuMusic.wav");
	snd_songOne		= LoadSound(assets + "songOne.wav");
	snd_songTwo		= LoadSound(assets + "songTwo.wav");
	snd_songThree	= LoadSound(assets + "songThree.wav");
	snd_creditMusic = LoadSound(assets + "CreditMusic.wav");

	return true;
}

SPRITE_PROMPTS promptSpawner(float speed, const int scaling, char promptChar)
{
	SPRITE_PROMPTS prompt; // the prompt we're gonna return
	char pc[] = { 'u', 'd','l', 'r', 'a', 'b' };

	// spawn a random prompt if nothing specified
	if (promptChar == ' ')
	{
		int randomNum;
		srand(time(NULL)); // initialize random seed
		randomNum = rand() % (sizeof(pc) / sizeof(char));
		prompt.promptType = pc[randomNum];
	}
	else
		prompt.promptType = promptChar;

	switch (prompt.promptType)
	{
	// we have an up arrow
	case 'u':
	{
		prompt.width		= upArrowDesc.Width;
		prompt.height		= upArrowDesc.Height;
		prompt.hitWidth		= upHitDesc.Width;
		prompt.hitHeight	= upHitDesc.Height;
		prompt.x			= 87.0f * scaling;
		prompt.y			= SCREENH + prompt.height;
		prompt.hitX			= prompt.x;
		prompt.hitY			= 17.0f * scaling;
		break;
	}
	case 'd':
	{
		prompt.width		= downArrowDesc.Width;
		prompt.height		= downArrowDesc.Height;
		prompt.hitWidth		= downHitDesc.Width;
		prompt.hitHeight	= downHitDesc.Height;
		prompt.x			= 57.0f * scaling;
		prompt.y			= SCREENH + prompt.height;
		prompt.hitX			= prompt.x;
		prompt.hitY			= 17.0f * scaling;
		break;
	}
	case 'l':
	{
		prompt.width		= leftArrowDesc.Width;
		prompt.height		= leftArrowDesc.Height;
		prompt.hitWidth		= leftHitDesc.Width;
		prompt.hitHeight	= leftHitDesc.Height;
		prompt.x			= 16.0f * scaling;
		prompt.y			= SCREENH + prompt.height;
		prompt.hitX			= prompt.x;
		prompt.hitY			= 45.0f * scaling;
		break;
	}
	case 'r':
	{
		prompt.width		= rightArrowDesc.Width;
		prompt.height		= rightArrowDesc.Height;
		prompt.hitWidth		= rightHitDesc.Width;
		prompt.hitHeight	= rightHitDesc.Height;
		prompt.x			= 110.0f * scaling;
		prompt.y			= SCREENH + prompt.height;
		prompt.hitX			= prompt.x;
		prompt.hitY			= 45.0f * scaling;
		break;
	}
	case 'a':
	{
		prompt.width		= aButtonDesc.Width;
		prompt.height		= aButtonDesc.Height;
		prompt.hitWidth		= aHitDesc.Width;
		prompt.hitHeight	= aHitDesc.Height;
		prompt.x = 28.0f * scaling;
		prompt.y = SCREENH + prompt.height;
		prompt.hitX = prompt.x;
		prompt.hitY = 72.0f * scaling;
		break;
	}
	case 'b':
	{
		prompt.width		= bButtonDesc.Width;
		prompt.height		= bButtonDesc.Height;
		prompt.hitWidth		= bHitDesc.Width;
		prompt.hitHeight	= bHitDesc.Height;
		prompt.x = 108.0f * scaling;
		prompt.y = SCREENH + prompt.height;
		prompt.hitX = prompt.x;
		prompt.hitY = 72.0f * scaling;
		break;
	}
	}

	// figure out speed and direction
	prompt.speedMult = speed * scaling;
	prompt.findDir();

	return prompt;
}

void promptPeriodicSpawn(float speed)
{
	// add new prompt every second or so
	time_t now;
	if (lastSpawnTime == NULL)
		time(&lastSpawnTime);
	now = time(&now);
	if (difftime(now, lastSpawnTime) > 0.1)
	{
		spawnCounter++;
		time(&lastSpawnTime); // reset lastSpawnTime
		allPrompts.push_back(promptSpawner(speed, scaleRes)); // spawn new prompt

		// occasionally spawn an a or b button with an arrow
		srand(time(NULL)); // initialize random seed
		if ((rand() % 4 + 1) == 4)
		{
			if ((rand() % 2 + 1) == 1 && allPrompts.back().promptType != 'a')
				allPrompts.push_back(promptSpawner(speed, scaleRes, 'a')); // spawn new prompt
			else if(allPrompts.back().promptType != 'b')
				allPrompts.push_back(promptSpawner(speed, scaleRes, 'b')); // spawn new prompt
		}
	}
}

void allPromptUpdate(char input)
{
	// p is for when the user presses both a and b
	char inputTwo = ' ';
	if (input == 'p')
	{
		input = 'a';
		inputTwo = 'b';
	}
	for (int i = 0; i < allPrompts.size(); i++)
	{
		// get rid of prompts if out of bounds or hit
		if (allPrompts[i].confirmHit(input) || 
			allPrompts[i].confirmHit(inputTwo) ||
			allPrompts[i].outBounds())
		{
			allPrompts.erase(allPrompts.begin() + i);
			if(inputTwo != ' ')
				i = -1;
		}
		// else update prompt position
		else
			allPrompts[i].moveToHit();
	}
}

void allScoredUpdate()
{
	for (int i = 0; i < allScored.size(); i++)
	{
		if (allScored[i].outBounds())
			allScored.erase(allScored.begin() + i);
		else
			allScored[i].moveScored();
	}
}

char userInputProtocol()
{
	char userInput = ' ';
	// gather userInput
	if (Key_Down_Once(DIK_W) || Key_Down_Once(DIK_UP) ||
		Button_Down_Once(XINPUT_GAMEPAD_DPAD_UP))
	{
		lastKey = "Up";
		userInput = 'u';
	}
	else if (Key_Down_Once(DIK_S) || Key_Down_Once(DIK_DOWN) ||
		Button_Down_Once(XINPUT_GAMEPAD_DPAD_DOWN))
	{
		lastKey = "Down";
		userInput = 'd';
	}
	else if (Key_Down_Once(DIK_A) || Key_Down_Once(DIK_LEFT) ||
		Button_Down_Once(XINPUT_GAMEPAD_DPAD_LEFT))
	{
		lastKey = "Left";
		userInput = 'l';
	}
	else if (Key_Down_Once(DIK_D) || Key_Down_Once(DIK_RIGHT) ||
		Button_Down_Once(XINPUT_GAMEPAD_DPAD_RIGHT))
	{
		lastKey = "Right";
		userInput = 'r';
	}
	else if (Key_Down(DIK_J) && Key_Down_Once(DIK_K) ||
		(Key_Down(DIK_Z) && Key_Down_Once(DIK_X)) ||
		(Button_Down(XINPUT_GAMEPAD_A) && Button_Down_Once(XINPUT_GAMEPAD_B)))
	{
		lastKey = "A + B";
		userInput = 'p'; // user pressed both a and b
	}
	else if (Key_Down_Once(DIK_J) || Key_Down_Once(DIK_Z) ||
		Button_Down_Once(XINPUT_GAMEPAD_A))
	{
		lastKey = "A";
		userInput = 'a';
	}
	else if (Key_Down_Once(DIK_K) || Key_Down_Once(DIK_X) ||
		Button_Down_Once(XINPUT_GAMEPAD_B))
	{
		lastKey = "B";
		userInput = 'b';
	}
	else
	{
		// makes it so we can't spam buttons with key_down_once
		noKey();
		noButton();
	}

	return userInput;
}

void drawScoreBoard()
{
	Sprite_Transform_Draw(
		scoreBoard,
		19 * scaleRes, 27 * scaleRes,
		scoreboardDesc.Width, scoreboardDesc.Height,
		0, 1, 0.0f, 1.0f, 1.0f);
}