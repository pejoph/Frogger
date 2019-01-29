//\===========================================================================================================================================/\\
//\	 Filename: main.cpp																														  /\\
//\																																			  /\\
//\	 Author  : Peter Phillips																												  /\\
//\																																			  /\\
//\	 Date    : First entry - 7 / 11 / 2017																									  /\\
//\			   Last entry - 11 / 12 / 2017																									  /\\
//\																																			  /\\
//\	 Brief   : A clone of the game 'Frogger' made by Konami in 1981.																		  /\\
//\			   NOTE: As this is a Konami game, there is an Easter egg accessed through entering the Konami code into the menu screen!		  /\\
//\===========================================================================================================================================/\\

#define _USE_MATH_DEFINES

// Includes to allow use of the UG Framework and some tools from other libraries.
#include "UGFW.h"
#include "UG_Defines.h"
#include <vector>
#include <ctime>
#include <cmath>
#include <ctype.h>
#include <iostream>
#include <algorithm>

#pragma region Defining and initialising enums
// All the different game states, each with its own unique screen.
enum GameStates
{
	SPLASH,
	MENU,
	PLAY,
	HIGHSCORES,
	HOWTOPLAY,
	PLAY2,		//this is the game in an earlier stage of development (before I added sprites) and can be accessed by entering the Konami code (UP>UP>DOWN>DOWN>LEFT>RIGHT>LEFT>RIGHT>B>A) into the menu
	GAMEOVER
};

// The different positions the player can be in, each has a unique sprite.
enum PlayerSprite 
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
	UPJUMP,
	DOWNJUMP,
	LEFTJUMP,
	RIGHTJUMP
};

// Everything that can be selected in the menu (this is changed when something in the menu is highlighted).
enum MenuSelect
{
	HTP,
	PLAYGAME,
	HISCORES,
};

// All the characters that can be put into a player's name*.
enum HSLetter 
{
	A,
	B,
	C,
	D,
	E,
	F,
	G,
	H,
	I,
	J,
	K,
	L,
	M,
	N,
	O,
	P,
	Q,
	R,
	S,
	T,
	U,
	V,
	W,
	X,
	Y,
	Z,
	SPACE // *'Space' is the default setting and appears blank, allowing names with less than 5 characters. 'Space' cannot actually be selected.
};

// Initalising all the enums used.
GameStates eCurrentState = SPLASH;
PlayerSprite eCurrentSprite = UP;
MenuSelect eCurrentSelect = PLAYGAME;
HSLetter eCurrentLetter1 = SPACE;
HSLetter eCurrentLetter2 = SPACE;
HSLetter eCurrentLetter3 = SPACE;
HSLetter eCurrentLetter4 = SPACE;
HSLetter eCurrentLetter5 = SPACE;
#pragma endregion

#pragma region Create struct
// Create a struct with x and y co-ordinates as well as a Sprite ID.
struct Sprite
{
	float xPos;
	float yPos;
	int iSpriteID;
};
#pragma endregion

#pragma region Setting global values
// Set screen dimensions.
const int iScreenWidth = 896;
const int iScreenHeight = 1024;

// Set sprite dimensions.
const int iSpriteWidth = 64;
const int iSpriteHeight = 64;

// Determine number of sprite tiles in each direction.
const int iSpriteRows = iScreenHeight / iSpriteHeight;
const int iSpriteColumns = iScreenWidth / iSpriteWidth;

// Declare sprites.
Sprite carPositions[3 + 3 + 3 + 1 + 2];
Sprite logturtlePositions[5 + 3 + 3 + 4 + 4];
Sprite carPositions2[3 + 3 + 3 + 1 + 4];
Sprite logturtlePositions2[15 + 9 + 18 + 8 + 16];
Sprite playerPosition; 
Sprite sBlueBackground;
Sprite sBlueBackgroundPlay;
Sprite sTitle;
Sprite sLetter;
Sprite sSplashFrog;
Sprite sPhantomFrog;
Sprite sMenuFrog;
Sprite sMenuCar1;
Sprite sMenuCar2;
Sprite sMenuCar3;
Sprite sMenuCar4;
Sprite sMenuCar5;
Sprite sHTPFrog;
Sprite sHTPCar;
Sprite sHTPLog;
Sprite sHTPLilyPad1;
Sprite sHTPLilyPad2;
Sprite sLilyPad1_1;
Sprite sLilyPad2_1;
Sprite sLilyPad3_1;
Sprite sLilyPad4_1;
Sprite sLilyPad5_1;
Sprite sLilyPad1_2;
Sprite sLilyPad2_2;
Sprite sLilyPad3_2;
Sprite sLilyPad4_2;
Sprite sLilyPad5_2;
Sprite sHTPRiver1;
Sprite sHTPRiver2;
Sprite sHTPFrogOnLilyPad;
Sprite sFrogOnLilyPad;
Sprite sDrown1;
Sprite sDrown2;
Sprite sDrown3;
Sprite sHitByCar1;
Sprite sHitByCar2;
Sprite sHitByCar3;
Sprite sHitByCar4;
Sprite sHTPBush;
Sprite sGrass1;
Sprite sGrass2;
Sprite sBush;
Sprite sLife;
Sprite sCar1;
Sprite sCar2;
Sprite sCar3;
Sprite sCar4;
Sprite sCar5;
Sprite sLog3;
Sprite sLog4;
Sprite sLog6;
Sprite sTurtle2_1;
Sprite sTurtle2_2;
Sprite sTurtle2_3;
Sprite sTurtle3_1;
Sprite sTurtle3_2;
Sprite sTurtle3_3;
Sprite sFrogUp;
Sprite sFrogDown;
Sprite sFrogLeft;
Sprite sFrogRight;
Sprite sFrogUpJump;
Sprite sFrogDownJump;
Sprite sFrogLeftJump;
Sprite sFrogRightJump;
Sprite sLetterUnderline;
Sprite sFrogOutOfTime;

// Create colours.
UG::SColour grey = UG::SColour(222, 222, 247, 255);
UG::SColour yellow = UG::SColour(255, 255, 0, 255);
UG::SColour red = UG::SColour(255, 0, 0, 255);
UG::SColour cyan = UG::SColour(0, 222, 247, 255);
UG::SColour magenta = UG::SColour(255, 71, 247, 255);

// Declare font.
int iFont;

// Set Initial conditions
/// Use this to spread sprites out randomly later on.
int randomShift;
/// State that frog is alive at game start.
bool bIsFrogDead = false;
/// Set initial game timer.
float fGameTimer = 30.0f;
/// Set initial display timer.
float fLevelDisplayTimer = 0.0f;
/// Set initial score timer.
float fScoreTimer = 1.0f;
/// Set starting # of lives and initialise current lives.
int iStartLives = 4;
int iCurrentLives = iStartLives;
int iStartLives2 = 28;
int iCurrentLives2 = iStartLives2;
/// Set starting score.
int iPlayerScore = 0;
int iScoreCompare = 0;
int iScoreGain = 0;
std::vector<int> vScoreGain;
/// State that the frog has not moved across the map.
bool bHasReachedRow1 = false;
bool bHasReachedRow2 = false;
bool bHasReachedRow3 = false;
bool bHasReachedRow4 = false;
bool bHasReachedRow5 = false;
bool bHasReachedRow6 = false;
bool bHasReachedRow7 = false;
bool bHasReachedRow8 = false;
bool bHasReachedRow9 = false;
bool bHasReachedRow10 = false;
bool bHasReachedRow11 = false;
/// State that the frog has not reached any lily pads.
bool bHasReachedLilyPad1 = false;
bool bHasReachedLilyPad2 = false;
bool bHasReachedLilyPad3 = false;
bool bHasReachedLilyPad4 = false;
bool bHasReachedLilyPad5 = false;
/// Set starting level.
int iLevel = 1;
/// Set the time display timer.
float fTimeDisplayTimer = 2.0f;
int iTimeTaken;
/// Set the movement reset timers.
float fMovementTimer = 0.25f;
float fMoveUpTimer = 0.25f;
float fMoveDownTimer = 0.25f;
float fMoveLeftTimer = 0.25f;
float fMoveRightTimer = 0.25f;
/// Set the death animation timer and conditions.
float fDeathTimer = 1.0f;
bool bHitByCar = false;
bool bDrowned = false;
bool bTimeRunOut = false;
/// Setting splash timer.
float fSplashTimer = 0.0f;
/// Set menu timer.
float fMenuTimer = 0.0f;
/// Set colour values.
UG::SColour HTPcolour = grey;
UG::SColour PGcolour = yellow;
UG::SColour HScolour = grey;
/// For Konami code
bool bKonami1 = false;
bool bKonami2 = false;
bool bKonami3 = false;
bool bKonami4 = false;
bool bKonami5 = false;
bool bKonami6 = false;
bool bKonami7 = false;
bool bKonami8 = false;
bool bKonami9 = false;
bool bKonami10 = false;
/// For bonus life.
bool bClaimed = false;
/// Set Highscores Timer.
float fHighscoresTimer = 0.0f;
float fHighscoresTimer2 = 0.0f;
/// Set How to play conditions.
bool bMovementInstruction = true;
bool bCarInstruction = false;
bool bLogInstruction = false;
bool bLilyPadInstruction = false;
bool bComplete = false;
float fHTPTimer = 0;
float fTargetYPosition = 3.5 * iSpriteHeight;
bool bDrawTarget = true;
/// Set Game Over conditions.
int iCounter = 0;
float fHSTimer = 0.0f;
int iCurrentSelect = 1;
/// Set Highscores conditions.
int iaHighScores[10] = { 10, 25, 50, 100, 250, 500, 1000, 2500, 5000, 10000 };
char caHighScoreNames[10][6] = { "JJJ", "III", "HHH", "GGG", "FFF", "EEE", "DDD", "CCC", "BBB", "AAA" };
char caPlayerName[5] = "";
#pragma endregion

#pragma region Declaring methods
// Methods are all defined at the end of this file.
void DrawCharacter(char character, float xStart = 100, float yStart = 100, char r = (char)255, char g = (char)255, char b = (char)255, char a = (char)255);
void DrawNumber(int number, float xStart = 100, float yStart = 100, char r = (char)255, char g = (char)255, char b = (char)255, char a = (char)255);
void DrawLargeNumber(int number, float xStart = 100, float yStart = 100, char r = (char)255, char g = (char)255, char b = (char)255, char a = (char)255);
void DrawLargeNumberHS(int number, float xStart = 100, float yStart = 100, char r = (char)255, char g = (char)255, char b = (char)255, char a = (char)255);
void ScoreGain();
#pragma endregion


int main(int argv, char* argc[])
{
#pragma region Initiate framework
	if (!UG::Create(iScreenWidth, iScreenHeight, false, "Sample", 100, 100)) return 1;
#pragma endregion

#pragma region Set background colour
	// Makes background black.
	UG::SetBackgroundColor(UG::SColour(0x00, 0x00, 0x00, 0x00));
#pragma endregion

#pragma region Seed random number
	// Seed random number with current time (for later use).
	srand((unsigned char)time(NULL));
#pragma endregion
	
#pragma region Initialise Splash
	// Blue portion of the background.
	sBlueBackground.iSpriteID = UG::CreateSprite("./images/BlueBackground.png", iSpriteWidth * 14, iSpriteHeight * 8, true);
	UG::SetSpritePosition(sBlueBackground.iSpriteID, iScreenWidth / 2, iScreenHeight * 3 / 4);
	// "Frogger" title.
	sTitle.iSpriteID = UG::CreateSprite("./images/FroggerTitle.png", float(iSpriteWidth) * 10, float(iSpriteHeight), true);
	UG::SetSpritePosition(sTitle.iSpriteID, iScreenWidth / 2, iScreenHeight - 3 * iSpriteHeight);
	// Copyright symbol.
	sLetter.iSpriteID = UG::CreateSprite("./images/copyrightGrey.png", iSpriteWidth / 2, iSpriteHeight / 2, true);
	UG::SetSpritePosition(sLetter.iSpriteID, iScreenWidth / 2 + 34, 2.75 * iSpriteHeight);
	// Frog - have the frog	start in the centre of the screen.
	sSplashFrog.xPos = iScreenWidth / 2;
	sSplashFrog.yPos = iScreenHeight / 2;
	sPhantomFrog.xPos = iScreenWidth / 2;
	sPhantomFrog.yPos = iScreenHeight / 2;
#pragma endregion

#pragma region Initialise Menu
	// Cars.
	randomShift = rand() % (iScreenWidth + 2 * iSpriteWidth); 
	sMenuCar1.xPos = float(randomShift); //spreads the car somewhere across the screen + 2 sprite widths
	sMenuCar1.yPos = iSpriteHeight;
	sMenuCar1.iSpriteID = UG::CreateSprite("./images/Car1.png", float(iSpriteWidth), float(iSpriteHeight), true);
	UG::SetSpritePosition(sMenuCar1.iSpriteID, sMenuCar1.xPos, sMenuCar1.yPos);
	randomShift = rand() % (iScreenWidth + 2 * iSpriteWidth);
	sMenuCar2.xPos = float(randomShift);
	sMenuCar2.yPos = 3 * iSpriteHeight;
	sMenuCar2.iSpriteID = UG::CreateSprite("./images/Car2.png", float(iSpriteWidth), float(iSpriteHeight), true);
	UG::SetSpritePosition(sMenuCar2.iSpriteID, sMenuCar2.xPos, sMenuCar2.yPos);
	randomShift = rand() % (iScreenWidth + 2 * iSpriteWidth);
	sMenuCar3.xPos = float(randomShift);
	sMenuCar3.yPos = 5 * iSpriteHeight;
	sMenuCar3.iSpriteID = UG::CreateSprite("./images/Car3.png", float(iSpriteWidth), float(iSpriteHeight), true);
	UG::SetSpritePosition(sMenuCar3.iSpriteID, sMenuCar3.xPos, sMenuCar3.yPos);
	randomShift = rand() % (iScreenWidth + 2 * iSpriteWidth);
	sMenuCar4.xPos = float(randomShift);
	sMenuCar4.yPos = 7 * iSpriteHeight;
	sMenuCar4.iSpriteID = UG::CreateSprite("./images/Car4.png", float(iSpriteWidth), float(iSpriteHeight), true);
	UG::SetSpritePosition(sMenuCar4.iSpriteID, sMenuCar4.xPos, sMenuCar4.yPos);
	// Frog - this is the icon that appears next to the current menu selection.
	sMenuFrog.xPos = 235;
	sMenuFrog.yPos = iScreenHeight - 6.27f * iSpriteHeight;
	sMenuFrog.iSpriteID = UG::CreateSprite("./images/FrogLife.png", iSpriteWidth / 2, iSpriteHeight / 2, true);
	UG::SetSpritePosition(sMenuFrog.iSpriteID, sMenuFrog.xPos, sMenuFrog.yPos);
#pragma endregion

#pragma region Initialise How to play
	// River.
	sHTPRiver1.iSpriteID = UG::CreateSprite("./images/BlueBackground.png", float(iSpriteWidth) * 14, float(iSpriteHeight), true);
	UG::SetSpritePosition(sHTPRiver1.iSpriteID, iScreenWidth / 2, 10.5 * iSpriteHeight);
	sHTPRiver2.iSpriteID = UG::CreateSprite("./images/BlueBackground.png", float(iSpriteWidth) * 14, float(iSpriteHeight), true);
	UG::SetSpritePosition(sHTPRiver2.iSpriteID, iScreenWidth / 2, 14.5 * iSpriteHeight);
	// Car.
	sHTPCar.xPos = iScreenWidth + iSpriteWidth;
	sHTPCar.yPos = 5.5 * iSpriteHeight;
	sHTPCar.iSpriteID = UG::CreateSprite("./images/Car1.png", float(iSpriteWidth), float(iSpriteHeight), true);
	UG::SetSpritePosition(sHTPCar.iSpriteID, sHTPCar.xPos, sHTPCar.yPos);
	// Log.
	sHTPLog.xPos = iScreenWidth + 2 * iSpriteWidth;
	sHTPLog.yPos = 10.5 * iSpriteHeight;
	sHTPLog.iSpriteID = UG::CreateSprite("./images/Log3.png", float(iSpriteWidth) * 3, float(iSpriteHeight), true);
	UG::SetSpritePosition(sHTPLog.iSpriteID, sHTPLog.xPos, sHTPLog.yPos);
	// Bush / lily pad.
	sHTPBush.iSpriteID = UG::CreateSprite("./images/Bush.png", float(iSpriteWidth) * 14, float(iSpriteHeight) * 2, true);
	UG::SetSpritePosition(sHTPBush.iSpriteID, iScreenWidth / 2, 15 * iSpriteHeight);
	sHTPLilyPad1.xPos = 7 * iSpriteWidth;
	sHTPLilyPad1.yPos = 14.5 * iSpriteHeight;
	sHTPLilyPad1.iSpriteID = UG::CreateSprite("./images/LilyPad1.png", float(iSpriteWidth), float(iSpriteHeight), true);
	UG::SetSpritePosition(sHTPLilyPad1.iSpriteID, sHTPLilyPad1.xPos, sHTPLilyPad1.yPos);
	sHTPLilyPad2.iSpriteID = UG::CreateSprite("./images/LilyPad2.png", float(iSpriteWidth), float(iSpriteHeight), true);
	UG::SetSpritePosition(sHTPLilyPad2.iSpriteID, sHTPLilyPad1.xPos, sHTPLilyPad1.yPos);
	sHTPFrogOnLilyPad.iSpriteID = UG::CreateSprite("./images/FrogOnLilyPad.png", float(iSpriteWidth), float(iSpriteHeight), true);
	UG::SetSpritePosition(sHTPFrogOnLilyPad.iSpriteID, 7 * iSpriteWidth, 14.5 * iSpriteHeight);
	// Death.
	sDrown1.iSpriteID = UG::CreateSprite("./images/Drown1.png", float(iSpriteWidth), float(iSpriteHeight), true);
	UG::SetSpritePosition(sDrown1.iSpriteID, sDrown1.xPos, sDrown1.yPos);
	sDrown2.iSpriteID = UG::CreateSprite("./images/Drown2.png", float(iSpriteWidth), float(iSpriteHeight), true);
	UG::SetSpritePosition(sDrown2.iSpriteID, sDrown1.xPos, sDrown1.yPos);
	sDrown3.iSpriteID = UG::CreateSprite("./images/Drown3.png", float(iSpriteWidth), float(iSpriteHeight), true);
	UG::SetSpritePosition(sDrown3.iSpriteID, sDrown1.xPos, sDrown1.yPos);
	sHitByCar1.iSpriteID = UG::CreateSprite("./images/HitByCar1.png", float(iSpriteWidth), float(iSpriteHeight), true);
	UG::SetSpritePosition(sHitByCar1.iSpriteID, sHitByCar1.xPos, sHitByCar1.yPos);
	sHitByCar2.iSpriteID = UG::CreateSprite("./images/HitByCar2.png", float(iSpriteWidth), float(iSpriteHeight), true);
	UG::SetSpritePosition(sHitByCar2.iSpriteID, sHitByCar1.xPos, sHitByCar1.yPos);
	sHitByCar3.iSpriteID = UG::CreateSprite("./images/HitByCar3.png", float(iSpriteWidth), float(iSpriteHeight), true);
	UG::SetSpritePosition(sHitByCar3.iSpriteID, sHitByCar1.xPos, sHitByCar1.yPos);
	sHitByCar4.iSpriteID = UG::CreateSprite("./images/HitByCar4.png", float(iSpriteWidth), float(iSpriteHeight), true);
	UG::SetSpritePosition(sHitByCar4.iSpriteID, sHitByCar1.xPos, sHitByCar1.yPos);
	// Frog.
	sHTPFrog.xPos = iSpriteWidth * 7;
	sHTPFrog.yPos = iSpriteHeight * 1.5;
#pragma endregion

#pragma region Initialise Play Game
	// Set starting player position.
	playerPosition.xPos = iSpriteWidth * 7;
	playerPosition.yPos = iSpriteHeight * 1.5;
	// Background.
	sBlueBackgroundPlay.iSpriteID = UG::CreateSprite("./images/BlueBackground.png", float(iSpriteWidth) * 14, float(iSpriteHeight) * 6.25f, true);
	UG::SetSpritePosition(sBlueBackgroundPlay.iSpriteID, iScreenWidth / 2, 11 * iSpriteHeight);
	sGrass1.iSpriteID = UG::CreateSprite("./images/Grass.png", float(iSpriteWidth) * 14, float(iSpriteHeight), true);
	UG::SetSpritePosition(sGrass1.iSpriteID, iScreenWidth / 2, 1.5 * iSpriteHeight);
	sGrass2.iSpriteID = UG::CreateSprite("./images/Grass.png", float(iSpriteWidth) * 14, float(iSpriteHeight), true);
	UG::SetSpritePosition(sGrass2.iSpriteID, iScreenWidth / 2, 7.5 * iSpriteHeight);
	sBush.iSpriteID = UG::CreateSprite("./images/Bush.png", float(iSpriteWidth) * 14, float(iSpriteHeight) * 2, true);
	UG::SetSpritePosition(sBush.iSpriteID, iScreenWidth / 2, iScreenHeight - 2 * iSpriteHeight);
	// Lily pads.
	sLilyPad1_1.xPos = (float)iSpriteWidth;
	sLilyPad2_1.xPos = 4 * (float)iSpriteWidth;
	sLilyPad3_1.xPos = 7 * (float)iSpriteWidth;
	sLilyPad4_1.xPos = 10 * (float)iSpriteWidth;
	sLilyPad5_1.xPos = 13 * (float)iSpriteWidth;
	sLilyPad1_1.yPos = (float)iScreenHeight - 2.5f * iSpriteHeight;
	sLilyPad1_1.iSpriteID = UG::CreateSprite("./images/LilyPad1.png", float(iSpriteWidth), float(iSpriteHeight), true);
	UG::SetSpritePosition(sLilyPad1_1.iSpriteID, sLilyPad1_1.xPos, sLilyPad1_1.yPos);
	sLilyPad2_1.iSpriteID = UG::CreateSprite("./images/LilyPad1.png", float(iSpriteWidth), float(iSpriteHeight), true);
	UG::SetSpritePosition(sLilyPad2_1.iSpriteID, sLilyPad2_1.xPos, sLilyPad1_1.yPos);
	sLilyPad3_1.iSpriteID = UG::CreateSprite("./images/LilyPad1.png", float(iSpriteWidth), float(iSpriteHeight), true);
	UG::SetSpritePosition(sLilyPad3_1.iSpriteID, sLilyPad3_1.xPos, sLilyPad1_1.yPos);
	sLilyPad4_1.iSpriteID = UG::CreateSprite("./images/LilyPad1.png", float(iSpriteWidth), float(iSpriteHeight), true);
	UG::SetSpritePosition(sLilyPad4_1.iSpriteID, sLilyPad4_1.xPos, sLilyPad1_1.yPos);
	sLilyPad5_1.iSpriteID = UG::CreateSprite("./images/LilyPad1.png", float(iSpriteWidth), float(iSpriteHeight), true);
	UG::SetSpritePosition(sLilyPad5_1.iSpriteID, sLilyPad5_1.xPos, sLilyPad1_1.yPos);
	sLilyPad1_2.iSpriteID = UG::CreateSprite("./images/LilyPad2.png", float(iSpriteWidth), float(iSpriteHeight), true);
	UG::SetSpritePosition(sLilyPad1_2.iSpriteID, sLilyPad1_1.xPos, sLilyPad1_1.yPos);
	sLilyPad2_2.iSpriteID = UG::CreateSprite("./images/LilyPad2.png", float(iSpriteWidth), float(iSpriteHeight), true);
	UG::SetSpritePosition(sLilyPad2_2.iSpriteID, sLilyPad2_1.xPos, sLilyPad1_1.yPos);
	sLilyPad3_2.iSpriteID = UG::CreateSprite("./images/LilyPad2.png", float(iSpriteWidth), float(iSpriteHeight), true);
	UG::SetSpritePosition(sLilyPad3_2.iSpriteID, sLilyPad3_1.xPos, sLilyPad1_1.yPos);
	sLilyPad4_2.iSpriteID = UG::CreateSprite("./images/LilyPad2.png", float(iSpriteWidth), float(iSpriteHeight), true);
	UG::SetSpritePosition(sLilyPad4_2.iSpriteID, sLilyPad4_1.xPos, sLilyPad1_1.yPos);
	sLilyPad5_2.iSpriteID = UG::CreateSprite("./images/LilyPad2.png", float(iSpriteWidth), float(iSpriteHeight), true);
	UG::SetSpritePosition(sLilyPad5_2.iSpriteID, sLilyPad5_1.xPos, sLilyPad1_1.yPos);
	sFrogOnLilyPad.iSpriteID = UG::CreateSprite("./images/FrogOnLilyPad.png", float(iSpriteWidth), float(iSpriteHeight), true);	
	// Lives.
	sLife.iSpriteID = UG::CreateSprite("./images/FrogLife.png", iSpriteWidth / 2, iSpriteHeight / 2, true);
	// Looping through array of cars to set positions.
	/// First 3 rows.
	for (int row = 0; row < 3; row++)
	{
		// Generate random # between 0 and 1024 (iScreenWidth + 2 * iSpriteWidth in pixels).
		randomShift = rand() % (iScreenWidth + 2 * iSpriteWidth);
		for (int car = 0; car < 3; car++)
		{
			carPositions[row * 3 + car].xPos = car * 4 * float(iSpriteWidth) + randomShift;
			carPositions[row * 3 + car].yPos = (row + 2) * (float)iSpriteHeight + iSpriteHeight / 2; //moves to a new row every 3 cars
		}
	}
	/// Fourth row.
	// Generate new random #.
	randomShift = rand() % (iScreenWidth + 2 * iSpriteWidth);
	carPositions[9].xPos = (float)randomShift;
	carPositions[9].yPos = 5.5 * iSpriteHeight;
	/// Fifth row.
	// Generate new random #.
	randomShift = rand() % (iScreenWidth + 2 * iSpriteWidth);
	for (int car = 0; car < 2; car++)
	{
		carPositions[10 + car].xPos = car * 4 * (float)iSpriteWidth + randomShift;
		carPositions[10 + car].yPos = 6.5 * iSpriteHeight;
	}	
	// Create Car Sprites.
	sCar1.iSpriteID = UG::CreateSprite("./images/Car1.png", float(iSpriteWidth), float(iSpriteHeight), true);
	sCar2.iSpriteID = UG::CreateSprite("./images/Car2.png", float(iSpriteWidth), float(iSpriteHeight), true);
	sCar3.iSpriteID = UG::CreateSprite("./images/Car3.png", float(iSpriteWidth), float(iSpriteHeight), true);
	sCar4.iSpriteID = UG::CreateSprite("./images/Car4.png", float(iSpriteWidth), float(iSpriteHeight), true);
	sCar5.iSpriteID = UG::CreateSprite("./images/Car5.png", float(iSpriteWidth) * 2, float(iSpriteHeight), true);
	// Looping through array of log/turtles to set positions.
	/// First row.	
	// Generate new random #.
	randomShift = rand() % (iScreenWidth + 8 * iSpriteWidth);
	for (int logturtle = 0; logturtle < 5; logturtle++)
	{
		logturtlePositions[logturtle].xPos = logturtle * 4 * (float)iSpriteWidth + randomShift;
		logturtlePositions[logturtle].yPos = 8.5 * iSpriteHeight;
	}
	/// Second row.	
	// Generate new random #.
	randomShift = rand() % (iScreenWidth + 8 * iSpriteWidth);
	for (int logturtle = 5; logturtle < 8; logturtle++)
	{
		int normLogturtle = logturtle - 5; //normalise the logturtle values so they start from 0
		logturtlePositions[logturtle].xPos = normLogturtle * 6 * iSpriteWidth - (float)(trunc(normLogturtle / 2)) * iSpriteWidth + randomShift; //trunc part reduces the triple gap after the first log to a double gap
		logturtlePositions[logturtle].yPos = 9.5 * iSpriteHeight;
	}
	/// Third row.	
	// Generate new random #.
	randomShift = rand() % (iScreenWidth + 14 * iSpriteWidth);
	for (int logturtle = 8; logturtle < 11; logturtle++)
	{
		int normLogturtle = logturtle - 8;
		logturtlePositions[logturtle].xPos = normLogturtle * 8 * (float)iSpriteWidth + randomShift;
		logturtlePositions[logturtle].yPos = 10.5 * iSpriteHeight;
	}
	/// Fourth row.
	// Generate new random #.
	randomShift = rand() % (iScreenWidth + 7 * iSpriteWidth);
	for (int logturtle = 11; logturtle < 15; logturtle++)
	{
		int normLogturtle = logturtle - 11;
		logturtlePositions[logturtle].xPos = normLogturtle * 4 * (float)iSpriteWidth + randomShift;
		logturtlePositions[logturtle].yPos = 11.5 * iSpriteHeight;
	}
	/// Fifth row.
	// Generate new random #.
	randomShift = rand() % (iScreenWidth + 11 * iSpriteWidth);
	for (int logturtle = 15; logturtle < 19; logturtle++)
	{
		int normLogturtle = logturtle - 15;
		logturtlePositions[logturtle].xPos = normLogturtle * 5 * iSpriteWidth + (float)(trunc(normLogturtle / 2) + trunc(normLogturtle / 3)) * iSpriteWidth + randomShift; //trunc part increases the single gap after the second and third logs to a double gap.
		logturtlePositions[logturtle].yPos = 12.5 * iSpriteHeight;		
	}
	// Create log/turtle sprites.
	sLog3.iSpriteID = UG::CreateSprite("./images/Log3.png", float(iSpriteWidth) * 3, float(iSpriteHeight), true);
	sLog4.iSpriteID = UG::CreateSprite("./images/Log4.png", float(iSpriteWidth) * 4, float(iSpriteHeight), true);
	sLog6.iSpriteID = UG::CreateSprite("./images/Log6.png", float(iSpriteWidth) * 6, float(iSpriteHeight), true);
	sTurtle2_1.iSpriteID = UG::CreateSprite("./images/Turtle2_1.png", float(iSpriteWidth) * 2, float(iSpriteHeight), true);
	sTurtle2_2.iSpriteID = UG::CreateSprite("./images/Turtle2_2.png", float(iSpriteWidth) * 2, float(iSpriteHeight), true);
	sTurtle2_3.iSpriteID = UG::CreateSprite("./images/Turtle2_3.png", float(iSpriteWidth) * 2, float(iSpriteHeight), true);
	sTurtle3_1.iSpriteID = UG::CreateSprite("./images/Turtle3_1.png", float(iSpriteWidth) * 3, float(iSpriteHeight), true);
	sTurtle3_2.iSpriteID = UG::CreateSprite("./images/Turtle3_2.png", float(iSpriteWidth) * 3, float(iSpriteHeight), true);
	sTurtle3_3.iSpriteID = UG::CreateSprite("./images/Turtle3_3.png", float(iSpriteWidth) * 3, float(iSpriteHeight), true);
	// Death.
	sFrogOutOfTime.iSpriteID = UG::CreateSprite("./images/FrogOutOfTime.png", float(iSpriteWidth), float(iSpriteHeight), true);
#pragma endregion

#pragma region Initialise Play2

#pragma region Set background positions
	// Create array to store sprite positions.
	Sprite backgroundPositions[iSpriteRows * iSpriteColumns];
	// Loop through array to set positions.
	for (int row = 0; row < iSpriteRows; row++)
	{
		for (int col = 0; col < iSpriteColumns; col++)
		{
			backgroundPositions[row * iSpriteColumns + col].xPos = float(iSpriteWidth) * col;
			backgroundPositions[row * iSpriteColumns + col].yPos = float(iSpriteHeight) * row;
		}
	}
#pragma endregion

#pragma region Set car positions
	// Loop through array to set positions.
	/// First 3 rows.
	for (int row = 0; row < 3; row++) 
	{		
		// Generate random # between 0 and 1024 (iScreenWidth + 2 * iSpriteWidth in pixels).
		randomShift = rand() % (iScreenWidth + 2 * iSpriteWidth);
		for (int car = 0; car < 3; car++)
		{
			carPositions2[row * 3 + car].xPos = float(iSpriteWidth) * car * 16 / 4 + float(iSpriteWidth) / 2 + randomShift;
			carPositions2[row * 3 + car].yPos = (row + 2) * iSpriteHeight + float(iSpriteHeight) / 2;
		}
	}
	/// Fourth row.
	// Generate new random #.
	randomShift = rand() % (iScreenWidth + 2 * iSpriteWidth);
	carPositions2[9].xPos = float(iSpriteWidth) / 2 + randomShift;
	carPositions2[9].yPos = 5 * iSpriteHeight + iSpriteHeight / 2;
	/// Fifth row.
	// Generate new random #.
	randomShift = rand() % (iScreenWidth + 2 * iSpriteWidth);
	for (int car = 0; car < 4; car++)
	{
		carPositions2[10 + car].xPos = iSpriteWidth * car * 8 / 4 + iSpriteWidth / 2 - car % 2 * iSpriteWidth + float(randomShift);
		carPositions2[10 + car].yPos = 6 * iSpriteHeight + iSpriteHeight / 2;
	}	
#pragma endregion

#pragma region Set logturtle positions
	// Loop through array to set positions.
	/// First row.	
	// Generate new random #.
	randomShift = rand() % (iScreenWidth + 8 * iSpriteWidth);
	for (int logturtle = 0; logturtle < 15; logturtle++)
	{
		logturtlePositions2[logturtle].xPos = iSpriteWidth * logturtle + iSpriteWidth / 2 + float(trunc(logturtle / 3)) * iSpriteWidth + float(randomShift); //trunc part adds a gap after every third turtle
		logturtlePositions2[logturtle].yPos = 8 * iSpriteHeight + iSpriteHeight / 2;
	}
	/// Second row.	
	// Generate new random #.
	randomShift = rand() % (iScreenWidth + 8 * iSpriteWidth);
	for (int logturtle = 15; logturtle < 24; logturtle++)
	{
		int normLogturtle = logturtle - 15; //normalise the logturtle values so they start from 0
		logturtlePositions2[logturtle].xPos = iSpriteWidth * normLogturtle + iSpriteWidth / 2 + (3 * float(trunc(normLogturtle / 3)) - float(trunc(normLogturtle / 6))) * iSpriteWidth + randomShift; //trunc part adds a triple gap after every 3rd log and removes a gap after the 6th
		logturtlePositions2[logturtle].yPos = 9 * iSpriteHeight + iSpriteHeight / 2;
	}
	/// Third row.	
	// Generate new random #.
	randomShift = rand() % (iScreenWidth + 14 * iSpriteWidth);
	for (int logturtle = 24; logturtle < 42; logturtle++)
	{
		int normLogturtle = logturtle - 24;
		logturtlePositions2[logturtle].xPos = iSpriteWidth * normLogturtle + iSpriteWidth / 2 + 2 * float(trunc(normLogturtle / 6)) * iSpriteWidth + randomShift;
		logturtlePositions2[logturtle].yPos = 10 * iSpriteHeight + iSpriteHeight / 2;
	}
	/// Fourth row.
	// Generate new random #.
	randomShift = rand() % (iScreenWidth + 7 * iSpriteWidth);
	for (int logturtle = 42; logturtle < 50; logturtle++)
	{
		int normLogturtle = logturtle - 42;
		logturtlePositions2[logturtle].xPos = iSpriteWidth * normLogturtle + iSpriteWidth / 2 + 2 * float(trunc(normLogturtle / 2)) * iSpriteWidth + randomShift;
		logturtlePositions2[logturtle].yPos = 11 * iSpriteHeight + iSpriteHeight / 2;
	}
	/// Fifth row.
	// Generate new random #.
	randomShift = rand() % (iScreenWidth + 11 * iSpriteWidth);
	for (int logturtle = 50; logturtle < 66; logturtle++)
	{
		int normLogturtle = logturtle - 50;
		logturtlePositions2[logturtle].xPos = iSpriteWidth * normLogturtle + iSpriteWidth / 2 + float((trunc(normLogturtle / 4) + trunc(normLogturtle / 8) + trunc(normLogturtle / 12))) * iSpriteWidth + randomShift;
		logturtlePositions2[logturtle].yPos = 12 * iSpriteHeight + iSpriteHeight / 2;
	}

#pragma endregion

#pragma endregion

#pragma region Initialise Game Over
	sLetterUnderline.iSpriteID = UG::CreateSprite("./images/HyphenYellow.png", float(iSpriteWidth) / 2, float(iSpriteHeight) / 2, true);
#pragma endregion

#pragma region UI
	// Font.
	iFont = UG::AddFont("./fonts/emulogic.ttf", 42);
	UG::SetFont(iFont);
#pragma endregion

#pragma region Initialise frog sprites
	// Frog sprites.
	sFrogUp.iSpriteID = UG::CreateSprite("./images/FrogUp.png", float(iSpriteWidth), float(iSpriteHeight), true);
	sFrogDown.iSpriteID = UG::CreateSprite("./images/FrogDown.png", float(iSpriteWidth), float(iSpriteHeight), true);
	sFrogLeft.iSpriteID = UG::CreateSprite("./images/FrogLeft.png", float(iSpriteWidth), float(iSpriteHeight), true);
	sFrogRight.iSpriteID = UG::CreateSprite("./images/FrogRight.png", float(iSpriteWidth), float(iSpriteHeight), true);
	sFrogUpJump.iSpriteID = UG::CreateSprite("./images/FrogUpJump.png", float(iSpriteWidth), float(iSpriteHeight), true);
	sFrogDownJump.iSpriteID = UG::CreateSprite("./images/FrogDownJump.png", float(iSpriteWidth), float(iSpriteHeight), true);
	sFrogLeftJump.iSpriteID = UG::CreateSprite("./images/FrogLeftJump.png", float(iSpriteWidth), float(iSpriteHeight), true);
	sFrogRightJump.iSpriteID = UG::CreateSprite("./images/FrogRightJump.png", float(iSpriteWidth), float(iSpriteHeight), true);
#pragma endregion

	// This is the start of the main game loop.
	do 
	{		
		switch (eCurrentState)
		{
#pragma region SPLASH
			// The splash screen is interactive! The frog can be moved around.
		case SPLASH:	
#pragma region Draw background
			// Blue portion of the background.
			UG::DrawSprite(sBlueBackground.iSpriteID); 
			
			// "Frogger" title.
			UG::SetSpriteScale(sTitle.iSpriteID, (const float)((iSpriteWidth) * 9.5 + float(iSpriteWidth) * sin(M_PI * fSplashTimer / 1.5)), (const float)iSpriteHeight);
			UG::DrawSprite(sTitle.iSpriteID);

			// Copyright symbol.
			UG::DrawSprite(sLetter.iSpriteID);

			// Main text.
			UG::DrawString("Re-created by", 234, int(iScreenHeight - 5.5 * iSpriteHeight), 0, red);
			UG::DrawString("Peter Phillips", 215, int(iScreenHeight - 6.5 * iSpriteHeight), 0, red);
			UG::DrawString("Sprites captured by", 133, 6 * iSpriteHeight, 0, cyan);
			UG::DrawString("Gary Kertopermono", 164, 5 * iSpriteHeight, 0, cyan);
			UG::DrawString("Konami     1981", 200, int(2.5 * iSpriteHeight), 0, grey);
#pragma endregion
			
#pragma region Player controls 
			// Changes player location upon user input
			if (fMovementTimer >= .25f) //this is the time taken for the movement animation to play out
			{
				if (UG::WasKeyPressed(UG::KEY_UP) || UG::WasKeyPressed(UG::KEY_W))
				{
					sSplashFrog.yPos += iSpriteHeight; //player position moves straight away but then the sprite is drawn on the phantom frog which follows the player position
					fMovementTimer = 0;
					fMoveUpTimer = 0;
					eCurrentSprite = UP;
				}
				if (UG::WasKeyPressed(UG::KEY_DOWN) || UG::WasKeyPressed(UG::KEY_S))
				{
					sSplashFrog.yPos -= iSpriteHeight;
					fMovementTimer = 0;
					fMoveDownTimer = 0;
					eCurrentSprite = DOWN;
				}
				if (UG::WasKeyPressed(UG::KEY_LEFT) || UG::WasKeyPressed(UG::KEY_A))
				{
					sSplashFrog.xPos -= iSpriteWidth;
					fMovementTimer = 0;
					fMoveLeftTimer = 0;
					eCurrentSprite = LEFT;
				}
				if (UG::WasKeyPressed(UG::KEY_RIGHT) || UG::WasKeyPressed(UG::KEY_D))
				{
					sSplashFrog.xPos += iSpriteWidth;
					fMovementTimer = 0;
					fMoveRightTimer = 0;
					eCurrentSprite = RIGHT;
				}
			}

			// Changes the sprite drawn and draws it on the phantom frog.
			switch(eCurrentSprite)
			{
			case UP:
				if (fMovementTimer < .25) 
				{
					UG::SetSpritePosition(sFrogUpJump.iSpriteID, sPhantomFrog.xPos, sPhantomFrog.yPos);
					UG::DrawSprite(sFrogUpJump.iSpriteID);
				}
				else 
				{
					UG::SetSpritePosition(sFrogUp.iSpriteID, sPhantomFrog.xPos, sPhantomFrog.yPos);
					UG::DrawSprite(sFrogUp.iSpriteID);
				}
				break;
			case DOWN:
				if (fMovementTimer < .25)
				{
					UG::SetSpritePosition(sFrogDownJump.iSpriteID, sPhantomFrog.xPos, sPhantomFrog.yPos);
					UG::DrawSprite(sFrogDownJump.iSpriteID);
				}
				else
				{
					UG::SetSpritePosition(sFrogDown.iSpriteID, sPhantomFrog.xPos, sPhantomFrog.yPos);
					UG::DrawSprite(sFrogDown.iSpriteID);
				}
				break;
			case LEFT:
				if (fMovementTimer < .25)
				{
					UG::SetSpritePosition(sFrogLeftJump.iSpriteID, sPhantomFrog.xPos, sPhantomFrog.yPos);
					UG::DrawSprite(sFrogLeftJump.iSpriteID);
				}
				else
				{
					UG::SetSpritePosition(sFrogLeft.iSpriteID, sPhantomFrog.xPos, sPhantomFrog.yPos);
					UG::DrawSprite(sFrogLeft.iSpriteID);
				}
				break;
			case RIGHT:
				if (fMovementTimer < .25)
				{
					UG::SetSpritePosition(sFrogRightJump.iSpriteID, sPhantomFrog.xPos, sPhantomFrog.yPos);
					UG::DrawSprite(sFrogRightJump.iSpriteID);
				}
				else
				{
					UG::SetSpritePosition(sFrogRight.iSpriteID, sPhantomFrog.xPos, sPhantomFrog.yPos);
					UG::DrawSprite(sFrogRight.iSpriteID);
				}
				break;
			};			

			// Realigns phantom frog position with player position.
			sPhantomFrog = sSplashFrog;

			// Controls the movement timers and updates the phantom frog position.
			if (fMovementTimer < .25)
			{
				fMovementTimer += UG::GetDeltaTime();					
			}
			if (fMoveUpTimer < .25)
			{
				sPhantomFrog.yPos = sSplashFrog.yPos - iSpriteHeight * 4 * (.25f - fMovementTimer);
				
				fMoveUpTimer += UG::GetDeltaTime();
			}
			if (fMoveDownTimer < .25)
			{
				sPhantomFrog.yPos = sSplashFrog.yPos + iSpriteHeight * 4 * (.25f - fMovementTimer);
				
				fMoveDownTimer += UG::GetDeltaTime();
			}
			if (fMoveLeftTimer < .25)
			{
				sPhantomFrog.xPos = sSplashFrog.xPos + iSpriteHeight * 4 * (.25f - fMovementTimer);
				
				fMoveLeftTimer += UG::GetDeltaTime();
			}
			if (fMoveRightTimer < .25)
			{
				sPhantomFrog.xPos = sSplashFrog.xPos - iSpriteHeight * 4 * (.25f - fMovementTimer);
				
				fMoveRightTimer += UG::GetDeltaTime();
			}
#pragma endregion
						
#pragma region Update splash timer
			fSplashTimer += UG::GetDeltaTime();	
#pragma endregion

#pragma region Exit splash screen
			// Fade - Draws a black box across the whole screen which increases in opactity from 0 to 255 between 4.5 and 5 seconds after entering the splash screen.
			if (fSplashTimer >= 5.0f) //prevents the alpha value from wrapping around if the timer exceeds 5
			{
				UG::DrawLine(0, iScreenHeight / 2, float(iScreenWidth), iScreenHeight / 2, float(iScreenHeight), 1, UG::SColour(0, 0, 0, 255));
			}
			else if (fSplashTimer >= 4.5f)
			{
				UG::DrawLine(0, iScreenHeight / 2, float(iScreenWidth), iScreenHeight / 2, float(iScreenHeight), 1, UG::SColour(0,0,0,(unsigned char)((fSplashTimer - 4.5)*2*255)));
			}

			// Exits splash screen after 5 seconds or if ESC is pressed.
			if (fSplashTimer >= 5.0f || UG::WasKeyPressed(UG::KEY_ESCAPE))
			{
				eCurrentState = MENU;
				eCurrentSprite = UP;
			}
#pragma endregion

			break;
#pragma endregion

#pragma region MENU
		case MENU:
#pragma region Draw background
			// Blue portion of the background.
			UG::DrawSprite(sBlueBackground.iSpriteID);

			// "Frogger" title.
			UG::SetSpriteScale(sTitle.iSpriteID, (const float)(iSpriteWidth * 9.5 + iSpriteWidth * sin(M_PI * fMenuTimer / 1.5)), (const float)iSpriteHeight);
			UG::DrawSprite(sTitle.iSpriteID);
#pragma endregion
			
#pragma region Konami code
			// Monitors which buttons in the sequence have been pressed.
			if (UG::WasKeyPressed(UG::KEY_UP)) 
			{
				bKonami1 = true;
			}			
			if (bKonami1 && UG::WasKeyPressed(UG::KEY_UP)) 
			{
				bKonami2 = true;
			}
			if (bKonami2 && UG::WasKeyPressed(UG::KEY_DOWN))
			{
				bKonami3 = true;
			}
			if (bKonami3 && UG::WasKeyPressed(UG::KEY_DOWN))
			{
				bKonami4 = true;
			}
			if (bKonami4 && UG::WasKeyPressed(UG::KEY_LEFT))
			{
				bKonami5 = true;
			}
			if (bKonami5 && UG::WasKeyPressed(UG::KEY_RIGHT))
			{
				bKonami6 = true;
			}
			if (bKonami6 && UG::WasKeyPressed(UG::KEY_LEFT))
			{
				bKonami7 = true;
			}
			if (bKonami7 && UG::WasKeyPressed(UG::KEY_RIGHT))
			{
				bKonami8 = true;
			}
			if (bKonami8 && UG::WasKeyPressed(UG::KEY_B))
			{
				bKonami9 = true;
			}
			if (bKonami9 && UG::WasKeyPressed(UG::KEY_A))
			{
				bKonami10 = true;
			}
			// When the sequence is complete, red text will appear on the screen and a secret game mode is unlocked.
			if (bKonami10)
			{
				UG::DrawString("Classic mode: activated", 64, int(3.8 * iSpriteHeight), 0, red);
			}
#pragma endregion

#pragma region Menu selections
			// Menu selections.
			switch (iCurrentSelect)
			{
			case 0:
				HTPcolour = yellow; //highlights current selection in yellow whilst the other options are in grey
				PGcolour = grey;
				HScolour = grey;
				sMenuFrog.yPos = iScreenHeight - 5.27f * iSpriteHeight;

				if (UG::WasKeyPressed(UG::KEY_SPACE) || UG::WasKeyPressed(UG::KEY_ENTER)) //pressing space or enter will select the highlighted option
				{
					eCurrentState = HOWTOPLAY;
					// Resets Konami sequence.
					bKonami1 = false;
					bKonami2 = false;
					bKonami3 = false;
					bKonami4 = false;
					bKonami5 = false;
					bKonami6 = false;
					bKonami7 = false;
					bKonami8 = false;
					bKonami9 = false;
					bKonami10 = false;
				}

				break;
			case 1:
				HTPcolour = grey;
				PGcolour = yellow;
				HScolour = grey;
				sMenuFrog.yPos = iScreenHeight - 6.27f * iSpriteHeight;				
				
				if (UG::WasKeyPressed(UG::KEY_SPACE) || UG::WasKeyPressed(UG::KEY_ENTER))
				{
					// If the Konami sequence has been entered in its entirety, selecting 'Play game' will load up an earlier version of the game before sprites were added. Scores in this version are not saved and you can't actually reach the 'Game over' screen.
					if (bKonami10)
					{
						eCurrentState = PLAY2;
						bKonami1 = false;
						bKonami2 = false;
						bKonami3 = false;
						bKonami4 = false;
						bKonami5 = false;
						bKonami6 = false;
						bKonami7 = false;
						bKonami8 = false;
						bKonami9 = false;
						bKonami10 = false;
					}
					else 
					{
						eCurrentState = PLAY;
						bKonami1 = false;
						bKonami2 = false;
						bKonami3 = false;
						bKonami4 = false;
						bKonami5 = false;
						bKonami6 = false;
						bKonami7 = false;
						bKonami8 = false;
						bKonami9 = false;
						bKonami10 = false;
					}
				}
				
				break;
			case 2:
				HTPcolour = grey;
				PGcolour = grey;
				HScolour = yellow;
				sMenuFrog.yPos = iScreenHeight - 7.27f * iSpriteHeight;

				if (UG::WasKeyPressed(UG::KEY_SPACE) || UG::WasKeyPressed(UG::KEY_ENTER))
				{
					eCurrentState = HIGHSCORES;
					bKonami1 = false;
					bKonami2 = false;
					bKonami3 = false;
					bKonami4 = false;
					bKonami5 = false;
					bKonami6 = false;
					bKonami7 = false;
					bKonami8 = false;
					bKonami9 = false;
					bKonami10 = false;
				}

				break;
			}
#pragma endregion

#pragma region Draw the selection names
			UG::DrawString("How To Play", 267, int(iScreenHeight - 5.5 * iSpriteHeight), 0, HTPcolour);
			UG::DrawString("Play Game", 267, int(iScreenHeight - 6.5 * iSpriteHeight), 0, PGcolour);
			UG::DrawString("Highscores", 267, int(iScreenHeight - 7.5 * iSpriteHeight), 0, HScolour);
#pragma endregion

#pragma region Cars
			// Update car positions.
			sMenuCar1.xPos -= 1;
			sMenuCar2.xPos += 1;
			sMenuCar3.xPos -= 2;
			sMenuCar4.xPos += 4;

			UG::SetSpritePosition(sMenuCar1.iSpriteID, sMenuCar1.xPos, sMenuCar1.yPos);
			UG::SetSpritePosition(sMenuCar2.iSpriteID, sMenuCar2.xPos, sMenuCar2.yPos);
			UG::SetSpritePosition(sMenuCar3.iSpriteID, sMenuCar3.xPos, sMenuCar3.yPos);
			UG::SetSpritePosition(sMenuCar4.iSpriteID, sMenuCar4.xPos, sMenuCar4.yPos);

			// Wrap cars around screen.
			if (sMenuCar1.xPos > iScreenWidth + iSpriteWidth || sMenuCar1.xPos < 1 * -iSpriteWidth)
			{
				sMenuCar1.xPos -= sMenuCar1.xPos / abs(sMenuCar1.xPos) * (iScreenWidth + 2 * iSpriteWidth);
			}
			if (sMenuCar2.xPos > iScreenWidth + iSpriteWidth || sMenuCar2.xPos < 1 * -iSpriteWidth)
			{
				sMenuCar2.xPos -= sMenuCar2.xPos / abs(sMenuCar2.xPos) * (iScreenWidth + 2 * iSpriteWidth);
			}
			if (sMenuCar3.xPos > iScreenWidth + iSpriteWidth || sMenuCar3.xPos < 1 * -iSpriteWidth)
			{
				sMenuCar3.xPos -= sMenuCar3.xPos / abs(sMenuCar3.xPos) * (iScreenWidth + 2 * iSpriteWidth);
			}
			if (sMenuCar4.xPos > iScreenWidth + iSpriteWidth || sMenuCar4.xPos < 1 * -iSpriteWidth)
			{
				sMenuCar4.xPos -= sMenuCar4.xPos / abs(sMenuCar4.xPos) * (iScreenWidth + 2 * iSpriteWidth);
			}

			// Draw the car sprites.
			UG::DrawSprite(sMenuCar1.iSpriteID);
			UG::DrawSprite(sMenuCar2.iSpriteID);
			UG::DrawSprite(sMenuCar3.iSpriteID);
			UG::DrawSprite(sMenuCar4.iSpriteID);
#pragma endregion			

#pragma region Frog
			// Draws the frog that appears next to the current menu selection.
			UG::SetSpritePosition(sMenuFrog.iSpriteID, sMenuFrog.xPos, sMenuFrog.yPos);
			UG::DrawSprite(sMenuFrog.iSpriteID);
#pragma endregion

#pragma region User controls
			// Changes the menu selection upon user input and wraps around the top and bottom selection.
			if (UG::WasKeyPressed(UG::KEY_UP) || UG::WasKeyPressed(UG::KEY_W))
			{
				if (iCurrentSelect == 0)
				{
					iCurrentSelect = 2;
				}
				else 
				{
					iCurrentSelect = (iCurrentSelect - 1) % 3;
				}

			}
			if (UG::WasKeyPressed(UG::KEY_DOWN) || UG::WasKeyPressed(UG::KEY_S))
			{
				iCurrentSelect = (iCurrentSelect + 1) % 3;
			}
#pragma endregion

#pragma region Update menu timer
			fMenuTimer += UG::GetDeltaTime();
#pragma endregion

#pragma region fade animation
			// Fade - Draws a black box across the whole screen which decreases in opactity from 255 to 0 for the first 0.5 seconds after exiting the splash screen. 
			if (fMenuTimer <= 0.5f)
			{
				UG::DrawLine(0, iScreenHeight / 2, float(iScreenWidth), iScreenHeight / 2, float(iScreenHeight), 1, UG::SColour(0, 0, 0, (unsigned char)((.5 - fMenuTimer) * 2 * 255)));
			}
#pragma endregion

#pragma region Exit game
			// Quit our application when escape is pressed.
			if (UG::WasKeyPressed(UG::KEY_ESCAPE))
				UG::Close();
			break;
#pragma endregion
#pragma endregion

#pragma region PLAY
		case PLAY:
#pragma region Draw background		
			UG::DrawSprite(sBlueBackgroundPlay.iSpriteID);
			UG::DrawSprite(sGrass1.iSpriteID);
			UG::DrawSprite(sGrass2.iSpriteID);
			UG::DrawSprite(sBush.iSpriteID);	
#pragma endregion

#pragma region Draw lily pads
			// Changes the lily pad sprite every half a second to give the appearance of the surroudning water rippling.
			if (fGameTimer - (int)fGameTimer < .5f)
			{
				UG::DrawSprite(sLilyPad1_1.iSpriteID);
				UG::DrawSprite(sLilyPad2_1.iSpriteID);
				UG::DrawSprite(sLilyPad3_1.iSpriteID);
				UG::DrawSprite(sLilyPad4_1.iSpriteID);
				UG::DrawSprite(sLilyPad5_1.iSpriteID);
			}
			else 
			{
				UG::DrawSprite(sLilyPad1_2.iSpriteID);
				UG::DrawSprite(sLilyPad2_2.iSpriteID);
				UG::DrawSprite(sLilyPad3_2.iSpriteID);
				UG::DrawSprite(sLilyPad4_2.iSpriteID);
				UG::DrawSprite(sLilyPad5_2.iSpriteID);
			}
#pragma endregion

#pragma region Call ScoreGain()
		// Calls the fn ScoreGain for 1 second to display the difference in score.
		if (fScoreTimer < 1) 
		{
			ScoreGain();
		}
#pragma endregion

#pragma region Draw timer and label
		// Draws timer.		
		if (fGameTimer > 0)
		{
			if (fDeathTimer >= 1)
			{
				fGameTimer -= UG::GetDeltaTime();
			}
			float xStart = iScreenWidth / 2 + (1 - fGameTimer / 30) * (5 * iSpriteWidth);
			float yStart = iSpriteHeight / 2;
			float xFinish = iScreenWidth - 2 * iSpriteWidth;
			float yFinish = iSpriteHeight / 2;
			float thickness = 24.0f;
			unsigned int layer = 0;
			UG::SColour colour = UG::SColour((char)((1 - fGameTimer / 30) * 255), (char)(255 * (fGameTimer / 30)), 0, 255); //colour changes from green to red as time left decreases

			UG::DrawLine(xStart, yStart, xFinish, yFinish, thickness, layer, colour);
		}

		// Draws timer label.
		DrawCharacter('t', iScreenWidth - 1.75 * iSpriteWidth + 6, iSpriteHeight / 2, (char)255, (char)255, 0);
		DrawCharacter('i', iScreenWidth - 1.25 * iSpriteWidth + 2, iSpriteHeight / 2, (char)255, (char)255, 0);
		DrawCharacter('m', iScreenWidth - .75 * iSpriteWidth - 2, iSpriteHeight / 2, (char)255, (char)255, 0);
		DrawCharacter('e', iScreenWidth - .25 * iSpriteWidth - 6, iSpriteHeight / 2, (char)255, (char)255, 0);
#pragma endregion
		
#pragma region Draw time taken and label
		// Draws time taken and label after each successful run.
		if (fTimeDisplayTimer < 2)
		{
			DrawCharacter('t', iScreenWidth / 2 - .75 * iSpriteWidth, iScreenHeight - iSpriteHeight / 2, (char)(255 - trunc(fTimeDisplayTimer / 1.5) * (fTimeDisplayTimer - 1.5) * 255 * 2), (char)(255 - trunc(fTimeDisplayTimer / 1.5) * (fTimeDisplayTimer - 1.5) * 255 * 2), (char)(255 - trunc(fTimeDisplayTimer / 1.5) * (fTimeDisplayTimer - 1.5) * 255 * 2), (char)255); //colours begin to fade to black after 1.5 seconds
			DrawCharacter('i', iScreenWidth / 2 - .25 * iSpriteWidth, iScreenHeight - iSpriteHeight / 2, (char)(255 - trunc(fTimeDisplayTimer / 1.5) * (fTimeDisplayTimer - 1.5) * 255 * 2), (char)(255 - trunc(fTimeDisplayTimer / 1.5) * (fTimeDisplayTimer - 1.5) * 255 * 2), (char)(255 - trunc(fTimeDisplayTimer / 1.5) * (fTimeDisplayTimer - 1.5) * 255 * 2), (char)255);
			DrawCharacter('m', iScreenWidth / 2 + .25 * iSpriteWidth, iScreenHeight - iSpriteHeight / 2, (char)(255 - trunc(fTimeDisplayTimer / 1.5) * (fTimeDisplayTimer - 1.5) * 255 * 2), (char)(255 - trunc(fTimeDisplayTimer / 1.5) * (fTimeDisplayTimer - 1.5) * 255 * 2), (char)(255 - trunc(fTimeDisplayTimer / 1.5) * (fTimeDisplayTimer - 1.5) * 255 * 2), (char)255);
			DrawCharacter('e', iScreenWidth / 2 + .75 * iSpriteWidth, iScreenHeight - iSpriteHeight / 2, (char)(255 - trunc(fTimeDisplayTimer / 1.5) * (fTimeDisplayTimer - 1.5) * 255 * 2), (char)(255 - trunc(fTimeDisplayTimer / 1.5) * (fTimeDisplayTimer - 1.5) * 255 * 2), (char)(255 - trunc(fTimeDisplayTimer / 1.5) * (fTimeDisplayTimer - 1.5) * 255 * 2), (char)255);
			DrawNumber((int)trunc((30 - iTimeTaken) / 10) % 10, iScreenWidth / 2 - .75 * iSpriteWidth, iScreenHeight - iSpriteHeight, (char)(255 - trunc(fTimeDisplayTimer / 1.5) * (fTimeDisplayTimer - 1.5) * 255 * 2), (char)(255 - trunc(fTimeDisplayTimer / 1.5) * (fTimeDisplayTimer - 1.5) * 255 * 2), (char)(255 - trunc(fTimeDisplayTimer / 1.5) * (fTimeDisplayTimer - 1.5) * 255 * 2), (char)255);
			DrawNumber((int)(30 - iTimeTaken) % 10,				iScreenWidth / 2 - .25 * iSpriteWidth, iScreenHeight - iSpriteHeight, (char)(255 - trunc(fTimeDisplayTimer / 1.5) * (fTimeDisplayTimer - 1.5) * 255 * 2), (char)(255 - trunc(fTimeDisplayTimer / 1.5) * (fTimeDisplayTimer - 1.5) * 255 * 2), (char)(255 - trunc(fTimeDisplayTimer / 1.5) * (fTimeDisplayTimer - 1.5) * 255 * 2), (char)255);
			DrawCharacter('s', iScreenWidth / 2 + .75 * iSpriteWidth, iScreenHeight - iSpriteHeight, (char)(255 - trunc(fTimeDisplayTimer / 1.5) * (fTimeDisplayTimer - 1.5) * 255 * 2), (char)(255 - trunc(fTimeDisplayTimer / 1.5) * (fTimeDisplayTimer - 1.5) * 255 * 2), (char)(255 - trunc(fTimeDisplayTimer / 1.5) * (fTimeDisplayTimer - 1.5) * 255 * 2), (char)255);

			// Increments display timer so the display disappears.
			fTimeDisplayTimer += UG::GetDeltaTime();
		}
#pragma endregion
				
#pragma region Draw Lives
		// Draws lives.
		for (int i = 1; i <= iCurrentLives; i++) 
		{
			UG::SetSpritePosition(sLife.iSpriteID, (i - .5f) * 1.5f * iSpriteWidth / 2, iSpriteHeight / 2);
			UG::DrawSprite(sLife.iSpriteID);
		}
#pragma endregion

#pragma region Draw score and label
		// Draws score.
		DrawNumber((int)trunc(iPlayerScore / 10000) % 10, 2.5 * iSpriteWidth, iScreenHeight - 2 * iSpriteHeight / 2, (char)220, (char)20, (char)20); //each line draws a different digit of the player score
		DrawNumber((int)trunc(iPlayerScore / 1000) % 10,	3 * iSpriteWidth, iScreenHeight - 2 * iSpriteHeight / 2, (char)220, (char)20, (char)20);
		DrawNumber((int)trunc(iPlayerScore / 100) % 10,	  3.5 * iSpriteWidth, iScreenHeight - 2 * iSpriteHeight / 2, (char)220, (char)20, (char)20);
		DrawNumber((int)trunc(iPlayerScore / 10) % 10,      4 * iSpriteWidth, iScreenHeight - 2 * iSpriteHeight / 2, (char)220, (char)20, (char)20);
		DrawNumber(iPlayerScore % 10,					  4.5 * iSpriteWidth, iScreenHeight - 2 * iSpriteHeight / 2, (char)220, (char)20, (char)20);
		// Draws score label.
		DrawCharacter('s', 2.5 * iSpriteWidth, iScreenHeight - iSpriteHeight / 2, (char)220, (char)220, (char)220);
		DrawCharacter('c',   3 * iSpriteWidth, iScreenHeight - iSpriteHeight / 2, (char)220, (char)220, (char)220);
		DrawCharacter('o', 3.5 * iSpriteWidth, iScreenHeight - iSpriteHeight / 2, (char)220, (char)220, (char)220);
		DrawCharacter('r',   4 * iSpriteWidth, iScreenHeight - iSpriteHeight / 2, (char)220, (char)220, (char)220);
		DrawCharacter('e', 4.5 * iSpriteWidth, iScreenHeight - iSpriteHeight / 2, (char)220, (char)220, (char)220);
#pragma endregion

#pragma region Define user controls
		// Changes player location upon user input
		if (fMovementTimer >= .25f && fDeathTimer >= 1.0f)
		{
			if ((UG::WasKeyPressed(UG::KEY_UP) || UG::WasKeyPressed(UG::KEY_W)) && playerPosition.yPos < iScreenHeight - 3 * iSpriteHeight) //stops player from moving out of the field of play
			{
				playerPosition.yPos += iSpriteHeight;
				fMovementTimer = 0;
				fMoveUpTimer = 0;
				eCurrentSprite = UP;
			}
			if ((UG::WasKeyPressed(UG::KEY_DOWN) || UG::WasKeyPressed(UG::KEY_S)) && playerPosition.yPos > 2 * iSpriteHeight)
			{
				playerPosition.yPos -= iSpriteHeight;
				fMovementTimer = 0;
				fMoveDownTimer = 0;
				eCurrentSprite = DOWN;
			}
			if ((UG::WasKeyPressed(UG::KEY_LEFT) || UG::WasKeyPressed(UG::KEY_A)) && playerPosition.xPos > iSpriteWidth)
			{
				playerPosition.xPos -= iSpriteWidth;
				fMovementTimer = 0;
				fMoveLeftTimer = 0;
				eCurrentSprite = LEFT;
			}
			if ((UG::WasKeyPressed(UG::KEY_RIGHT) || UG::WasKeyPressed(UG::KEY_D)) && playerPosition.xPos < iScreenWidth - iSpriteWidth)
			{
				playerPosition.xPos += iSpriteWidth;
				fMovementTimer = 0;
				fMoveRightTimer = 0;
				eCurrentSprite = RIGHT;
			}
		}

		// Update the phantom frog position.
		sPhantomFrog = playerPosition;

		// Controls the movement timers and updates phantom frog position for smooth animation.
		if (fMovementTimer < .25 && fDeathTimer >= 1.0f)
		{
			if (fMoveUpTimer < .25)
			{
				sPhantomFrog.yPos = playerPosition.yPos - iSpriteHeight * 4 * (.25f - fMovementTimer);

				fMoveUpTimer += UG::GetDeltaTime();
			}
			if (fMoveDownTimer < .25)
			{
				sPhantomFrog.yPos = playerPosition.yPos + iSpriteHeight * 4 * (.25f - fMovementTimer);

				fMoveDownTimer += UG::GetDeltaTime();
			}
			if (fMoveLeftTimer < .25)
			{
				sPhantomFrog.xPos = playerPosition.xPos + iSpriteHeight * 4 * (.25f - fMovementTimer);

				fMoveLeftTimer += UG::GetDeltaTime();
			}
			if (fMoveRightTimer < .25)
			{
				sPhantomFrog.xPos = playerPosition.xPos - iSpriteHeight * 4 * (.25f - fMovementTimer);

				fMoveRightTimer += UG::GetDeltaTime();
			}

			fMovementTimer += UG::GetDeltaTime();
		}
#pragma endregion

#pragma region Give score upon reaching new row
		// Grants score upon reaching a new row.
		if (!bHasReachedRow1 && playerPosition.yPos > 2 * iSpriteHeight) //score only gained if player reaches a row and hasn't reached it before
		{
			iPlayerScore += 10;
			bHasReachedRow1 = true;
		}
		if (!bHasReachedRow2 && playerPosition.yPos > 3 * iSpriteHeight)
		{
			iPlayerScore += 10;
			bHasReachedRow2 = true;
		}
		if (!bHasReachedRow3 && playerPosition.yPos > 4 * iSpriteHeight)
		{
			iPlayerScore += 10;
			bHasReachedRow3 = true;
		}
		if (!bHasReachedRow4 && playerPosition.yPos > 5 * iSpriteHeight)
		{
			iPlayerScore += 10;
			bHasReachedRow4 = true;
		}
		if (!bHasReachedRow5 && playerPosition.yPos > 6 * iSpriteHeight)
		{
			iPlayerScore += 10;
			bHasReachedRow5 = true;
		}
		if (!bHasReachedRow6 && playerPosition.yPos > 7 * iSpriteHeight)
		{
			iPlayerScore += 10;
			bHasReachedRow6 = true;
		}
		if (!bHasReachedRow7 && playerPosition.yPos > 8 * iSpriteHeight)
		{
			iPlayerScore += 10;
			bHasReachedRow7 = true;
		}
		if (!bHasReachedRow8 && playerPosition.yPos > 9 * iSpriteHeight) 
		{
			iPlayerScore += 10;
			bHasReachedRow8 = true;
		}
		if (!bHasReachedRow9 && playerPosition.yPos > 10 * iSpriteHeight)
		{
			iPlayerScore += 10;
			bHasReachedRow9 = true;
		}
		if (!bHasReachedRow10 && playerPosition.yPos > 11 * iSpriteHeight)
		{
			iPlayerScore += 10;
			bHasReachedRow10 = true;
		}
		if (!bHasReachedRow11 && playerPosition.yPos > 12 * iSpriteHeight)
		{
			iPlayerScore += 10;
			bHasReachedRow11 = true;
		}
#pragma endregion

#pragma region Draw cars and update their positions
		// Draws cars.
		/// First row.
		for (int i = 0; i < 3; i++)
		{
			// Moves cars in a direction determined by their row.
			if ((int)(carPositions[i].yPos / iSpriteHeight - .5) % 2 == 0)
			{
				carPositions[i].xPos -= 1 + (float)(iLevel - 1) / 5;
			}
			else
			{
				carPositions[i].xPos += 1 + (float)(iLevel - 1) / 5;
			}

			// Wraps cars around screen in both directions.
			if (carPositions[i].xPos > iScreenWidth + iSpriteWidth || carPositions[i].xPos < -iSpriteWidth)
			{
				carPositions[i].xPos -= carPositions[i].xPos/abs(carPositions[i].xPos)*(iScreenWidth + 2 * iSpriteWidth);
			}

			UG::SetSpritePosition(sCar1.iSpriteID, carPositions[i].xPos, carPositions[i].yPos);
			UG::DrawSprite(sCar1.iSpriteID);
		}
		/// Second Row.
		for (int i = 3; i < 6; i++)
		{
			// Moves cars in a direction determined by their row.
			if ((int)(carPositions[i].yPos / iSpriteHeight - .5) % 2 == 0)
			{
				carPositions[i].xPos -= 1 + (float)(iLevel - 1) / 5;
			}
			else
			{
				carPositions[i].xPos += 1 + (float)(iLevel - 1) / 5;
			}

			// Wraps cars around screen in both directions.
			if (carPositions[i].xPos > iScreenWidth + iSpriteWidth || carPositions[i].xPos < -iSpriteWidth)
			{
				carPositions[i].xPos -= carPositions[i].xPos / abs(carPositions[i].xPos)*(iScreenWidth + 2 * iSpriteWidth);
			}

			UG::SetSpritePosition(sCar2.iSpriteID, carPositions[i].xPos, carPositions[i].yPos);
			UG::DrawSprite(sCar2.iSpriteID);
		}
		/// Third row.
		for (int i = 6; i < 9; i++)
		{
			// Moves cars in a direction determined by their row.
			if ((int)(carPositions[i].yPos / iSpriteHeight - .5) % 2 == 0)
			{
				carPositions[i].xPos -= 2 + (float)(iLevel - 1) / 5;
			}
			else
			{
				carPositions[i].xPos += 2 + (float)(iLevel - 1) / 5;
			}

			// Wraps cars around screen in both directions.
			if (carPositions[i].xPos > iScreenWidth + iSpriteWidth || carPositions[i].xPos < -iSpriteWidth)
			{
				carPositions[i].xPos -= carPositions[i].xPos / abs(carPositions[i].xPos)*(iScreenWidth + 2 * iSpriteWidth);
			}

			UG::SetSpritePosition(sCar3.iSpriteID, carPositions[i].xPos, carPositions[i].yPos);
			UG::DrawSprite(sCar3.iSpriteID);
		}
		/// Fourth row.
		for (int i = 9; i < 10; i++)
		{
			// Moves cars in a direction determined by their row.
			if ((int)(carPositions[i].yPos / iSpriteHeight - .5) % 2 == 0)
			{
				carPositions[i].xPos -= 4 + (float)(iLevel - 1) / 5;
			}
			else
			{
				carPositions[i].xPos += 4 + (float)(iLevel - 1) / 5;
			}

			// Wraps cars around screen in both directions.
			if (carPositions[i].xPos > iScreenWidth + iSpriteWidth || carPositions[i].xPos < -iSpriteWidth)
			{
				carPositions[i].xPos -= carPositions[i].xPos / abs(carPositions[i].xPos)*(iScreenWidth + 2 * iSpriteWidth);
			}

			UG::SetSpritePosition(sCar4.iSpriteID, carPositions[i].xPos, carPositions[i].yPos);
			UG::DrawSprite(sCar4.iSpriteID);
		}
		/// Fifth row.
		for (int i = 10; i < 12; i++)
		{
			// Moves cars in a direction determined by their row.
			if ((int)(carPositions[i].yPos / iSpriteHeight - .5) % 2 == 0)
			{
				carPositions[i].xPos -= 2 + (float)(iLevel - 1) / 5;
			}
			else
			{
				carPositions[i].xPos += 2 + (float)(iLevel - 1) / 5;
			}

			// Wraps cars around screen in both directions.
			if (carPositions[i].xPos > iScreenWidth + iSpriteWidth || carPositions[i].xPos < -iSpriteWidth)
			{
				carPositions[i].xPos -= carPositions[i].xPos / abs(carPositions[i].xPos)*(iScreenWidth + 2 * iSpriteWidth);
			}

			UG::SetSpritePosition(sCar5.iSpriteID, carPositions[i].xPos, carPositions[i].yPos);
			UG::DrawSprite(sCar5.iSpriteID);
		}
#pragma endregion

#pragma region Draw logs/turtles and update their positions
		// Draws logs/turtles.
		/// First row.
		for (int i = 0; i < 5; i++)
		{
			// Set turtle direction.
			logturtlePositions[i].xPos -= 1 + (float)(iLevel - 1) / 5;

			// Wraps logs/turtles around screen in both directions.
			if (logturtlePositions[i].xPos > iScreenWidth + 4 * iSpriteWidth || logturtlePositions[i].xPos < 4 * -iSpriteWidth)
			{
				logturtlePositions[i].xPos -= logturtlePositions[i].xPos / abs(logturtlePositions[i].xPos)*(iScreenWidth + 8 * iSpriteWidth);
			}

			UG::SetSpritePosition(sTurtle3_1.iSpriteID, logturtlePositions[i].xPos, logturtlePositions[i].yPos);
			UG::SetSpritePosition(sTurtle3_2.iSpriteID, logturtlePositions[i].xPos, logturtlePositions[i].yPos);
			UG::SetSpritePosition(sTurtle3_3.iSpriteID, logturtlePositions[i].xPos, logturtlePositions[i].yPos);

			if (fGameTimer - (int)fGameTimer < .33)
			{
				UG::DrawSprite(sTurtle3_2.iSpriteID);
			}
			else if (fGameTimer - (int)fGameTimer < .67)
			{
				UG::DrawSprite(sTurtle3_1.iSpriteID);
			}
			else
			{
				UG::DrawSprite(sTurtle3_3.iSpriteID);
			}
		}
		/// Second row.
		for (int i = 5; i < 8; i++)
		{
			// Set turtle direction.
			logturtlePositions[i].xPos += 1 + (float)(iLevel - 1) / 5;

			// Wraps logs/turtles around screen in both directions.
			if (logturtlePositions[i].xPos > iScreenWidth + 4 * iSpriteWidth || logturtlePositions[i].xPos < 4 * -iSpriteWidth)
			{
				logturtlePositions[i].xPos -= logturtlePositions[i].xPos / abs(logturtlePositions[i].xPos)*(iScreenWidth + 8 * iSpriteWidth);
			}

			UG::SetSpritePosition(sLog3.iSpriteID, logturtlePositions[i].xPos, logturtlePositions[i].yPos);
			UG::DrawSprite(sLog3.iSpriteID);
		}
		/// Third row.
		for (int i = 8; i < 11; i++)
		{
			// Set turtle direction.
			logturtlePositions[i].xPos += 2 + (float)(iLevel - 1) / 5;

			// Wraps logs/turtles around screen in both directions.
			if (logturtlePositions[i].xPos > iScreenWidth + 7 * iSpriteWidth || logturtlePositions[i].xPos < 7 * -iSpriteWidth)
			{
				logturtlePositions[i].xPos -= logturtlePositions[i].xPos / abs(logturtlePositions[i].xPos)*(iScreenWidth + 14 * iSpriteWidth);
			}

			UG::SetSpritePosition(sLog6.iSpriteID, logturtlePositions[i].xPos, logturtlePositions[i].yPos);
			UG::DrawSprite(sLog6.iSpriteID);
		}
		/// Fourth row.
		for (int i = 11; i < 15; i++)
		{
			// Set turtle direction.
			logturtlePositions[i].xPos -= 1 + (float)(iLevel - 1) / 5;

			// Wraps logs/turtles around screen in both directions.
			if (logturtlePositions[i].xPos > iScreenWidth + 3 * iSpriteWidth || logturtlePositions[i].xPos < 4 * -iSpriteWidth)
			{
				logturtlePositions[i].xPos -= logturtlePositions[i].xPos / abs(logturtlePositions[i].xPos)*(iScreenWidth + 7 * iSpriteWidth);
			}

			UG::SetSpritePosition(sTurtle2_1.iSpriteID, logturtlePositions[i].xPos, logturtlePositions[i].yPos);
			UG::SetSpritePosition(sTurtle2_2.iSpriteID, logturtlePositions[i].xPos, logturtlePositions[i].yPos);
			UG::SetSpritePosition(sTurtle2_3.iSpriteID, logturtlePositions[i].xPos, logturtlePositions[i].yPos);

			if (fGameTimer - (int)fGameTimer < .33)
			{
				UG::DrawSprite(sTurtle2_1.iSpriteID);
			}
			else if (fGameTimer - (int)fGameTimer < .67)
			{
				UG::DrawSprite(sTurtle2_3.iSpriteID);
			}
			else
			{
				UG::DrawSprite(sTurtle2_2.iSpriteID);
			}
		}
		/// Fifth row.
		for (int i = 15; i < 19; i++)
		{
			// Set turtle direction.
			logturtlePositions[i].xPos += 1.5f + (float)(iLevel - 1) / 5;

			// Wraps logs/turtles around screen in both directions.
			if (logturtlePositions[i].xPos > iScreenWidth + 5 * iSpriteWidth || logturtlePositions[i].xPos < 6 * -iSpriteWidth)
			{
				logturtlePositions[i].xPos -= logturtlePositions[i].xPos / abs(logturtlePositions[i].xPos)*(iScreenWidth + 11 * iSpriteWidth);
			}

			UG::SetSpritePosition(sLog4.iSpriteID, logturtlePositions[i].xPos, logturtlePositions[i].yPos);
			UG::DrawSprite(sLog4.iSpriteID);
		}
#pragma endregion

#pragma region Move player onto a sprite position upon exiting river also reset player position and row progress upon landing on a lilypad, add score, and make the lilypad 'used'.
#pragma region Below
		// Moves player onto a sprite position upon exiting the river.
		if (playerPosition.yPos == 7.5 * iSpriteHeight)
		{
			int offset = (int)playerPosition.xPos % iSpriteWidth;

			if (offset <= 32) //rounds the correction to the left or right
			{
				playerPosition.xPos -= offset;
			}
			else
			{
				playerPosition.xPos += iSpriteWidth - offset;
			}
		}
#pragma endregion

#pragma region Above
		else if (playerPosition.yPos == 13.5 * iSpriteHeight && fDeathTimer >= 1)
		{
			int offset = (int)playerPosition.xPos % iSpriteWidth;

			if (offset <= 32) //rounds the correction to the left or right
			{
				playerPosition.xPos -= offset;
			}
			else
			{
				playerPosition.xPos += iSpriteWidth - offset;
			}

			// Resets player position and row progress upon landing on a lilypad, adds score, and makes the lilypad 'used'.
			if (playerPosition.xPos >= iSpriteWidth - 1 && playerPosition.xPos <= iSpriteWidth + 1 && !bHasReachedLilyPad1) 
			{
				playerPosition.xPos = iSpriteWidth * 7;
				playerPosition.yPos = iSpriteHeight * 1.5;
				iTimeTaken = (int)fGameTimer;
				bHasReachedLilyPad1 = true;
				bHasReachedRow1 = false;
				bHasReachedRow2 = false;
				bHasReachedRow3 = false;
				bHasReachedRow4 = false;
				bHasReachedRow5 = false;
				bHasReachedRow6 = false;
				bHasReachedRow7 = false;
				bHasReachedRow8 = false;
				bHasReachedRow9 = false;
				bHasReachedRow10 = false;
				bHasReachedRow11 = false;
				iPlayerScore += 50;		
				iPlayerScore += (int)fGameTimer * 5;
				fTimeDisplayTimer = 0;
				fGameTimer = 30.0f;
			}
			else if (playerPosition.xPos >= 4 * iSpriteWidth - 1 && playerPosition.xPos <= 4 * iSpriteWidth + 1 && !bHasReachedLilyPad2)
			{
				playerPosition.xPos = iSpriteWidth * 7;
				playerPosition.yPos = iSpriteHeight * 1.5;
				iTimeTaken = (int)fGameTimer;
				bHasReachedLilyPad2 = true;
				bHasReachedRow1 = false;
				bHasReachedRow2 = false;
				bHasReachedRow3 = false;
				bHasReachedRow4 = false;
				bHasReachedRow5 = false;
				bHasReachedRow6 = false;
				bHasReachedRow7 = false;
				bHasReachedRow8 = false;
				bHasReachedRow9 = false;
				bHasReachedRow10 = false;
				bHasReachedRow11 = false;
				iPlayerScore += 50;
				iPlayerScore += (int)fGameTimer * 5;
				fTimeDisplayTimer = 0;
				fGameTimer = 30.0f;
			}
			else if (playerPosition.xPos >= 7 * iSpriteWidth - 1 && playerPosition.xPos <= 7 * iSpriteWidth + 1 && !bHasReachedLilyPad3)
			{
				playerPosition.xPos = iSpriteWidth * 7;
				playerPosition.yPos = iSpriteHeight * 1.5;
				iTimeTaken = (int)fGameTimer;
				bHasReachedLilyPad3 = true;
				bHasReachedRow1 = false;
				bHasReachedRow2 = false;
				bHasReachedRow3 = false;
				bHasReachedRow4 = false;
				bHasReachedRow5 = false;
				bHasReachedRow6 = false;
				bHasReachedRow7 = false;
				bHasReachedRow8 = false;
				bHasReachedRow9 = false;
				bHasReachedRow10 = false;
				bHasReachedRow11 = false;
				iPlayerScore += 50;
				iPlayerScore += (int)fGameTimer * 5;
				fTimeDisplayTimer = 0;
				fGameTimer = 30.0f;
			}
			else if (playerPosition.xPos >= 10 * iSpriteWidth - 1 && playerPosition.xPos <= 10 * iSpriteWidth + 1 && !bHasReachedLilyPad4)
			{
				playerPosition.xPos = iSpriteWidth * 7;
				playerPosition.yPos = iSpriteHeight * 1.5;
				iTimeTaken = (int)fGameTimer;
				bHasReachedLilyPad4 = true;
				bHasReachedRow1 = false;
				bHasReachedRow2 = false;
				bHasReachedRow3 = false;
				bHasReachedRow4 = false;
				bHasReachedRow5 = false;
				bHasReachedRow6 = false;
				bHasReachedRow7 = false;
				bHasReachedRow8 = false;
				bHasReachedRow9 = false;
				bHasReachedRow10 = false;
				bHasReachedRow11 = false;
				iPlayerScore += 50;
				iPlayerScore += (int)fGameTimer * 5;
				fTimeDisplayTimer = 0;
				fGameTimer = 30.0f;
			}
			else if (playerPosition.xPos >= 13 * iSpriteWidth - 1 && playerPosition.xPos <= 13 * iSpriteWidth + 1 && !bHasReachedLilyPad5)
			{
				playerPosition.xPos = iSpriteWidth * 7;
				playerPosition.yPos = iSpriteHeight * 1.5;
				iTimeTaken = (int)fGameTimer;
				bHasReachedLilyPad5 = true;
				bHasReachedRow1 = false;
				bHasReachedRow2 = false;
				bHasReachedRow3 = false;
				bHasReachedRow4 = false;
				bHasReachedRow5 = false;
				bHasReachedRow6 = false;
				bHasReachedRow7 = false;
				bHasReachedRow8 = false;
				bHasReachedRow9 = false;
				bHasReachedRow10 = false;
				bHasReachedRow11 = false;
				iPlayerScore += 50;
				iPlayerScore += (int)fGameTimer * 5;
				fTimeDisplayTimer = 0;
				fGameTimer = 30.0f;
			}			
		}
#pragma endregion
#pragma endregion

#pragma region Draw frog on lilypad after reaching each one
		if (bHasReachedLilyPad1)
		{
			UG::SetSpritePosition(sFrogOnLilyPad.iSpriteID, sLilyPad1_1.xPos, sLilyPad1_1.yPos);
			UG::DrawSprite(sFrogOnLilyPad.iSpriteID);
		}
		if (bHasReachedLilyPad2)
		{
			UG::SetSpritePosition(sFrogOnLilyPad.iSpriteID, sLilyPad2_1.xPos, sLilyPad1_1.yPos);
			UG::DrawSprite(sFrogOnLilyPad.iSpriteID);
		}
		if (bHasReachedLilyPad3)
		{
			UG::SetSpritePosition(sFrogOnLilyPad.iSpriteID, sLilyPad3_1.xPos, sLilyPad1_1.yPos);
			UG::DrawSprite(sFrogOnLilyPad.iSpriteID);
		}
		if (bHasReachedLilyPad4)
		{
			UG::SetSpritePosition(sFrogOnLilyPad.iSpriteID, sLilyPad4_1.xPos, sLilyPad1_1.yPos);
			UG::DrawSprite(sFrogOnLilyPad.iSpriteID);
		}
		if (bHasReachedLilyPad5)
		{
			UG::SetSpritePosition(sFrogOnLilyPad.iSpriteID, sLilyPad5_1.xPos, sLilyPad1_1.yPos);
			UG::DrawSprite(sFrogOnLilyPad.iSpriteID);
		}
#pragma endregion

#pragma region Sets conditions for going to the next level
		// Sets conditions for returning all 5 frogs home safely.
		if (bHasReachedLilyPad1 && bHasReachedLilyPad2 && bHasReachedLilyPad3 && bHasReachedLilyPad4 && bHasReachedLilyPad5 && fMovementTimer >= .25)
		{
			iPlayerScore += 200;
			bHasReachedLilyPad1 = false;
			bHasReachedLilyPad2 = false;
			bHasReachedLilyPad3 = false;
			bHasReachedLilyPad4 = false;
			bHasReachedLilyPad5 = false;
			iLevel += 1;
			fLevelDisplayTimer = 0;
		}
#pragma endregion

#pragma region Display level
		// Displays level.
		if (fLevelDisplayTimer < 2)
		{
			// Draws the level sign in white and fades after 1.5 seconds
			DrawCharacter('l', iScreenWidth / 2 + 2 * iSpriteWidth	, iScreenHeight - iSpriteHeight, (char)(255 - trunc(fLevelDisplayTimer / 1.5) * (fLevelDisplayTimer - 1.5) * 255 * 2), (char)(255 - trunc(fLevelDisplayTimer / 1.5) * (fLevelDisplayTimer - 1.5) * 255 * 2), (char)(255 - trunc(fLevelDisplayTimer / 1.5) * (fLevelDisplayTimer - 1.5) * 255 * 2), (char)255); //the colour is solid white for 1.5 seconds then gradually fades to black over the next half a second.
			DrawCharacter('e', iScreenWidth / 2 + 2.5 * iSpriteWidth, iScreenHeight - iSpriteHeight, (char)(255 - trunc(fLevelDisplayTimer / 1.5) * (fLevelDisplayTimer - 1.5) * 255 * 2), (char)(255 - trunc(fLevelDisplayTimer / 1.5) * (fLevelDisplayTimer - 1.5) * 255 * 2), (char)(255 - trunc(fLevelDisplayTimer / 1.5) * (fLevelDisplayTimer - 1.5) * 255 * 2), (char)255);
			DrawCharacter('v', iScreenWidth / 2 + 3 * iSpriteWidth	, iScreenHeight - iSpriteHeight, (char)(255 - trunc(fLevelDisplayTimer / 1.5) * (fLevelDisplayTimer - 1.5) * 255 * 2), (char)(255 - trunc(fLevelDisplayTimer / 1.5) * (fLevelDisplayTimer - 1.5) * 255 * 2), (char)(255 - trunc(fLevelDisplayTimer / 1.5) * (fLevelDisplayTimer - 1.5) * 255 * 2), (char)255);
			DrawCharacter('e', iScreenWidth / 2 + 3.5 * iSpriteWidth, iScreenHeight - iSpriteHeight, (char)(255 - trunc(fLevelDisplayTimer / 1.5) * (fLevelDisplayTimer - 1.5) * 255 * 2), (char)(255 - trunc(fLevelDisplayTimer / 1.5) * (fLevelDisplayTimer - 1.5) * 255 * 2), (char)(255 - trunc(fLevelDisplayTimer / 1.5) * (fLevelDisplayTimer - 1.5) * 255 * 2), (char)255);
			DrawCharacter('l', iScreenWidth / 2 + 4 * iSpriteWidth	, iScreenHeight - iSpriteHeight, (char)(255 - trunc(fLevelDisplayTimer / 1.5) * (fLevelDisplayTimer - 1.5) * 255 * 2), (char)(255 - trunc(fLevelDisplayTimer / 1.5) * (fLevelDisplayTimer - 1.5) * 255 * 2), (char)(255 - trunc(fLevelDisplayTimer / 1.5) * (fLevelDisplayTimer - 1.5) * 255 * 2), (char)255);
			DrawNumber(iLevel, iScreenWidth / 2 + 5 * iSpriteWidth	, iScreenHeight - iSpriteHeight, (char)(255 - trunc(fLevelDisplayTimer / 1.5) * (fLevelDisplayTimer - 1.5) * 255 * 2), (char)(255 - trunc(fLevelDisplayTimer / 1.5) * (fLevelDisplayTimer - 1.5) * 255 * 2), (char)(255 - trunc(fLevelDisplayTimer / 1.5) * (fLevelDisplayTimer - 1.5) * 255 * 2), (char)255);
			
			fLevelDisplayTimer += UG::GetDeltaTime();
		}
#pragma endregion

#pragma region Bonus life
		// Grants player an extra life upon reaching 10,000 score.
		if (iPlayerScore >= 10000 && !bClaimed)
		{
			iCurrentLives += 1;
			bClaimed = true;
		}
#pragma endregion

#pragma region Death Timer		
		// Increment the death timer.
		if (fDeathTimer < 1 && fMovementTimer >= .25) 
		{
			fDeathTimer += UG::GetDeltaTime();
		}
#pragma endregion

#pragma region Reset game
		// Resets game when 'R' or 'ESC' is pressed.
		if (UG::WasKeyPressed(UG::KEY_R) || UG::WasKeyPressed(UG::KEY_ESCAPE))
		{
			// Loop through array to set positions.
			/// First 3 rows.
			for (int row = 0; row < 3; row++)
			{
				// Generate random # between 0 and 1024 (iScreenWidth + 2 * iSpriteWidth in pixels).
				randomShift = rand() % (iScreenWidth + 2 * iSpriteWidth);
				for (int car = 0; car < 3; car++)
				{
					carPositions2[row * 3 + car].xPos = iSpriteWidth * car * 16 / 4 + iSpriteWidth / 2 + float(randomShift);
					carPositions2[row * 3 + car].yPos = (row + 2) * iSpriteHeight + float(iSpriteHeight / 2);
				}
			}
			/// Fourth row.
			// Generate new random #.
			randomShift = rand() % (iScreenWidth + 2 * iSpriteWidth);
			carPositions2[9].xPos = iSpriteWidth / 2 + float(randomShift);
			carPositions2[9].yPos = 5 * iSpriteHeight + iSpriteHeight / 2;
			/// Fifth row.
			// Generate new random #.
			randomShift = rand() % (iScreenWidth + 2 * iSpriteWidth);
			for (int car = 0; car < 4; car++)
			{
				carPositions2[10 + car].xPos = iSpriteWidth * car * 8 / 4 + iSpriteWidth / 2 - car % 2 * iSpriteWidth + float(randomShift);
				carPositions2[10 + car].yPos = 6 * iSpriteHeight + iSpriteHeight / 2;
			}
						
			// Loop through array to set positions.
			/// First row.	
			// Generate new random #.
			randomShift = rand() % (iScreenWidth + 8 * iSpriteWidth);
			for (int logturtle = 0; logturtle < 15; logturtle++)
			{
				logturtlePositions2[logturtle].xPos = iSpriteWidth * logturtle + iSpriteWidth / 2 + float(trunc(logturtle / 3)) * iSpriteWidth + randomShift; //trunc part adds a gap after every third turtle
				logturtlePositions2[logturtle].yPos = 8 * iSpriteHeight + iSpriteHeight / 2;
			}
			/// Second row.	
			// Generate new random #.
			randomShift = rand() % (iScreenWidth + 8 * iSpriteWidth);
			for (int logturtle = 15; logturtle < 24; logturtle++)
			{
				int normLogturtle = logturtle - 15; //normalise the logturtle values so they start from 0
				logturtlePositions2[logturtle].xPos = iSpriteWidth * normLogturtle + iSpriteWidth / 2 + float((3 * trunc(normLogturtle / 3) - trunc(normLogturtle / 6))) * iSpriteWidth + randomShift; //trunc part adds a triple gap after every 3rd log and removes a gap after the 6th
				logturtlePositions2[logturtle].yPos = 9 * iSpriteHeight + iSpriteHeight / 2;
			}
			/// Third row.	
			// Generate new random #.
			randomShift = rand() % (iScreenWidth + 14 * iSpriteWidth);
			for (int logturtle = 24; logturtle < 42; logturtle++)
			{
				int normLogturtle = logturtle - 24;
				logturtlePositions2[logturtle].xPos = iSpriteWidth * normLogturtle + iSpriteWidth / 2 + 2 * float(trunc(normLogturtle / 6)) * iSpriteWidth + randomShift;
				logturtlePositions2[logturtle].yPos = 10 * iSpriteHeight + iSpriteHeight / 2;
			}
			/// Fourth row.
			// Generate new random #.
			randomShift = rand() % (iScreenWidth + 7 * iSpriteWidth);
			for (int logturtle = 42; logturtle < 50; logturtle++)
			{
				int normLogturtle = logturtle - 42;
				logturtlePositions2[logturtle].xPos = iSpriteWidth * normLogturtle + iSpriteWidth / 2 + 2 * float(trunc(normLogturtle / 2)) * iSpriteWidth + randomShift;
				logturtlePositions2[logturtle].yPos = 11 * iSpriteHeight + iSpriteHeight / 2;
			}
			/// Fifth row.
			// Generate new random #.
			randomShift = rand() % (iScreenWidth + 11 * iSpriteWidth);
			for (int logturtle = 50; logturtle < 66; logturtle++)
			{
				int normLogturtle = logturtle - 50;
				logturtlePositions2[logturtle].xPos = iSpriteWidth * normLogturtle + iSpriteWidth / 2 + float((trunc(normLogturtle / 4) + trunc(normLogturtle / 8) + trunc(normLogturtle / 12))) * iSpriteWidth + randomShift;
				logturtlePositions2[logturtle].yPos = 12 * iSpriteHeight + iSpriteHeight / 2;
			}
			
			// Set Initial conditions
			/// Set starting player position.
			playerPosition.xPos = iSpriteWidth * 7;
			playerPosition.yPos = iSpriteHeight * 1.5;
			/// State that frog is alive at game start.
			bIsFrogDead = false;
			/// Set initial game timer.
			fGameTimer = 30.0f;
			/// Reset current lives.
			iCurrentLives = iStartLives;
			/// Set starting score.
			iPlayerScore = 0;
			/// State that the frog has not moved across the map.
			bHasReachedRow1 = false;
			bHasReachedRow2 = false;
			bHasReachedRow3 = false;
			bHasReachedRow4 = false;
			bHasReachedRow5 = false;
			bHasReachedRow6 = false;
			bHasReachedRow7 = false;
			bHasReachedRow8 = false;
			bHasReachedRow9 = false;
			bHasReachedRow10 = false;
			bHasReachedRow11 = false;
			/// State that the frog has not reached any lily pads.
			bHasReachedLilyPad1 = false;
			bHasReachedLilyPad2 = false;
			bHasReachedLilyPad3 = false;
			bHasReachedLilyPad4 = false;
			bHasReachedLilyPad5 = false;
			/// Reset the display timer and level and tell the level to display.
			fLevelDisplayTimer = 0;
			iLevel = 1;
			/// Reset the bonus life.
			bClaimed = false;
			/// Reset frog sprite position.
			eCurrentSprite = UP;
		}
#pragma endregion

#pragma region Display score gain
		// Checks the score gained every update and displays the score gain if it's larger than 10 (so score gain doesn't appear every time you jump to a new row).
		if (iPlayerScore > iScoreCompare + 10)
		{
			fScoreTimer = 0;
			iScoreGain = iPlayerScore - iScoreCompare;			
		}
		iScoreCompare = iPlayerScore;
#pragma endregion

#pragma region Draw player or set death conditions
		if (fDeathTimer < 1)
		{
			// Plays the drowning animation.
			if (bDrowned)
			{
				sDrown1.xPos = playerPosition.xPos;
				sDrown1.yPos = playerPosition.yPos;

				if (fDeathTimer < .33f)
				{
					UG::SetSpritePosition(sDrown1.iSpriteID, sDrown1.xPos, sDrown1.yPos);
					UG::DrawSprite(sDrown1.iSpriteID);
				}
				else if (fDeathTimer < .67f)
				{
					UG::SetSpritePosition(sDrown2.iSpriteID, sDrown1.xPos, sDrown1.yPos);
					UG::DrawSprite(sDrown2.iSpriteID);
				}
				else
				{
					UG::SetSpritePosition(sDrown3.iSpriteID, sDrown1.xPos, sDrown1.yPos);
					UG::DrawSprite(sDrown3.iSpriteID);
				}
			}
			// Plays the hit-by-car animation.
			else if (bHitByCar)
			{
				sHitByCar1.xPos = playerPosition.xPos;
				sHitByCar1.yPos = playerPosition.yPos;

				if (fDeathTimer < .25f)
				{
					UG::SetSpritePosition(sHitByCar1.iSpriteID, sHitByCar1.xPos, sHitByCar1.yPos);
					UG::DrawSprite(sHitByCar1.iSpriteID);
				}
				else if (fDeathTimer < .5f)
				{
					UG::SetSpritePosition(sHitByCar2.iSpriteID, sHitByCar1.xPos, sHitByCar1.yPos);
					UG::DrawSprite(sHitByCar2.iSpriteID);
				}
				else if (fDeathTimer < .75f)
				{
					UG::SetSpritePosition(sHitByCar3.iSpriteID, sHitByCar1.xPos, sHitByCar1.yPos);
					UG::DrawSprite(sHitByCar3.iSpriteID);
				}
				else
				{
					UG::SetSpritePosition(sHitByCar4.iSpriteID, sHitByCar1.xPos, sHitByCar1.yPos);
					UG::DrawSprite(sHitByCar4.iSpriteID);
				}
			}
			else if (bTimeRunOut) 
			{
				UG::SetSpritePosition(sFrogOutOfTime.iSpriteID, playerPosition.xPos, playerPosition.yPos);
				UG::DrawSprite(sFrogOutOfTime.iSpriteID);
			}

			fDeathTimer += UG::GetDeltaTime();
		}
		// If frog dies, reset position and timer and lose a life.
		else if (bIsFrogDead)
		{
			playerPosition.xPos = 7 * iSpriteWidth;
			playerPosition.yPos = 1.5 * iSpriteHeight;
			bIsFrogDead = false;
			bDrowned = false;
			bHitByCar = false;
			bTimeRunOut = false;
			iCurrentLives -= 1;
			iTimeTaken = (int)fGameTimer;
			fGameTimer = 30.0f;
		}		
		// Check death conditions then draw frog.
		else 
		{
			// Kills player if timer runs out.
			if (fGameTimer <= 0)
			{
				bIsFrogDead = true;
				fDeathTimer = 0;
				bTimeRunOut = true;
			}
			// Kills frog if frog drifts off-screen.
			if (playerPosition.xPos < -iSpriteWidth / 2 || playerPosition.xPos > iScreenWidth + iSpriteWidth / 2)
			{
				bIsFrogDead = true;
				fDeathTimer = 0;
			}
			// Kills player if lands in a bush.
			if (playerPosition.yPos == 13.5 * iSpriteHeight)
			{
				bIsFrogDead = true;
				fDeathTimer = 0;
				bDrowned = true;
			}
			// Kills player if hit by a car.
			for (int i = 0; i < 10; i++) 
			{
				if (sPhantomFrog.xPos >= carPositions[i].xPos - 48 && sPhantomFrog.xPos <= carPositions[i].xPos + 48 && sPhantomFrog.yPos == carPositions[i].yPos)
				{
					bIsFrogDead = true;
					bHitByCar = true;
					fDeathTimer = 0;
				}
			}
			for (int i = 10; i < 12; i++)
			{
				if (sPhantomFrog.xPos >= carPositions[i].xPos - 70 && sPhantomFrog.xPos <= carPositions[i].xPos + 70 && sPhantomFrog.yPos == carPositions[i].yPos)
				{
					bIsFrogDead = true;
					bHitByCar = true;
					fDeathTimer = 0;
				}
			}			
		
			// Kills player if player lands in the river.
			/// First row.
			if (playerPosition.yPos == 8.5 * iSpriteHeight)
			{
				// Checks if Frog is on a log/turtle.
				if ((sPhantomFrog.xPos >= logturtlePositions[0].xPos - 110 && sPhantomFrog.xPos <= logturtlePositions[0].xPos + 110) ||
					(sPhantomFrog.xPos >= logturtlePositions[1].xPos - 110 && sPhantomFrog.xPos <= logturtlePositions[1].xPos + 110) ||
					(sPhantomFrog.xPos >= logturtlePositions[2].xPos - 110 && sPhantomFrog.xPos <= logturtlePositions[2].xPos + 110) ||
					(sPhantomFrog.xPos >= logturtlePositions[3].xPos - 110 && sPhantomFrog.xPos <= logturtlePositions[3].xPos + 110) ||
					(sPhantomFrog.xPos >= logturtlePositions[4].xPos - 110 && sPhantomFrog.xPos <= logturtlePositions[4].xPos + 110))
				{
					// Moves player by same amount as log/turtle.
					playerPosition.xPos -= 1 + (float)(iLevel - 1) / 5;
				}
				else
				{
					bIsFrogDead = true;
					fDeathTimer = 0;
					bDrowned = true;
				}
			}			
			/// Second row.
			else if (playerPosition.yPos == 9.5 * iSpriteHeight)
			{
				
				if ((sPhantomFrog.xPos >= logturtlePositions[5].xPos - 100 && sPhantomFrog.xPos <= logturtlePositions[5].xPos + 100) ||
					(sPhantomFrog.xPos >= logturtlePositions[6].xPos - 100 && sPhantomFrog.xPos <= logturtlePositions[6].xPos + 100) ||
					(sPhantomFrog.xPos >= logturtlePositions[7].xPos - 100 && sPhantomFrog.xPos <= logturtlePositions[7].xPos + 100))
				{
					// Moves player by same amount as log/turtle.
					playerPosition.xPos += 1 + (float)(iLevel - 1) / 5;
				}
				else
				{
					bIsFrogDead = true;
					fDeathTimer = 0;
					bDrowned = true;
				}				
			}
			/// Third row.
			else if (playerPosition.yPos == 10.5 * iSpriteHeight)
			{
				
				if ((sPhantomFrog.xPos >= logturtlePositions[8].xPos - 196 && sPhantomFrog.xPos <= logturtlePositions[8].xPos + 196) ||
					(sPhantomFrog.xPos >= logturtlePositions[9].xPos - 196 && sPhantomFrog.xPos <= logturtlePositions[9].xPos + 196) ||
					(sPhantomFrog.xPos >= logturtlePositions[10].xPos - 196 && sPhantomFrog.xPos <= logturtlePositions[10].xPos + 196))
				{
					// Moves player by same amount as log/turtle.
					playerPosition.xPos += 2 + (float)(iLevel - 1) / 5;
				}
				else
				{
					bIsFrogDead = true;
					fDeathTimer = 0;
					bDrowned = true;
				}
			
			}
			/// Fourth row.
			else if (playerPosition.yPos == 11.5 * iSpriteHeight)
			{
				
				if ((sPhantomFrog.xPos >= logturtlePositions[11].xPos - 74 && sPhantomFrog.xPos <= logturtlePositions[11].xPos + 74) ||
					(sPhantomFrog.xPos >= logturtlePositions[12].xPos - 74 && sPhantomFrog.xPos <= logturtlePositions[12].xPos + 74) ||
					(sPhantomFrog.xPos >= logturtlePositions[13].xPos - 74 && sPhantomFrog.xPos <= logturtlePositions[13].xPos + 74) ||
					(sPhantomFrog.xPos >= logturtlePositions[14].xPos - 74 && sPhantomFrog.xPos <= logturtlePositions[14].xPos + 74))
				{
					// Moves player by same amount as log/turtle.
					playerPosition.xPos -= 1 + (float)(iLevel - 1) / 5;
				}
				else
				{
					bIsFrogDead = true;
					fDeathTimer = 0;
					bDrowned = true;
				}
				
			}
			/// Fifth row.
			else if (playerPosition.yPos == 12.5 * iSpriteHeight)
			{
				
				if ((sPhantomFrog.xPos >= logturtlePositions[15].xPos - 132 && sPhantomFrog.xPos <= logturtlePositions[15].xPos + 132) ||
					(sPhantomFrog.xPos >= logturtlePositions[16].xPos - 132 && sPhantomFrog.xPos <= logturtlePositions[16].xPos + 132) ||
					(sPhantomFrog.xPos >= logturtlePositions[17].xPos - 132 && sPhantomFrog.xPos <= logturtlePositions[17].xPos + 132) ||
					(sPhantomFrog.xPos >= logturtlePositions[18].xPos - 132 && sPhantomFrog.xPos <= logturtlePositions[18].xPos + 132))
				{
					// Moves player by same amount as log/turtle.
					playerPosition.xPos += 1.5f  + (float)(iLevel - 1) / 5;
				}
				else
				{
					bIsFrogDead = true;
					fDeathTimer = 0;
					bDrowned = true;
				}
				
			}
			// Draws frog sprite in correct direction and state of jumping.
			switch (eCurrentSprite)
			{
			case UP:
				if (fMovementTimer < .25)
				{
					UG::SetSpritePosition(sFrogUpJump.iSpriteID, sPhantomFrog.xPos, sPhantomFrog.yPos);
					UG::DrawSprite(sFrogUpJump.iSpriteID);
				}
				else
				{
					UG::SetSpritePosition(sFrogUp.iSpriteID, sPhantomFrog.xPos, sPhantomFrog.yPos);
					UG::DrawSprite(sFrogUp.iSpriteID);
				}
				break;
			case DOWN:
				if (fMovementTimer < .25)
				{
					UG::SetSpritePosition(sFrogDownJump.iSpriteID, sPhantomFrog.xPos, sPhantomFrog.yPos);
					UG::DrawSprite(sFrogDownJump.iSpriteID);
				}
				else
				{
					UG::SetSpritePosition(sFrogDown.iSpriteID, sPhantomFrog.xPos, sPhantomFrog.yPos);
					UG::DrawSprite(sFrogDown.iSpriteID);
				}
				break;
			case LEFT:
				if (fMovementTimer < .25)
				{
					UG::SetSpritePosition(sFrogLeftJump.iSpriteID, sPhantomFrog.xPos, sPhantomFrog.yPos);
					UG::DrawSprite(sFrogLeftJump.iSpriteID);
				}
				else
				{
					UG::SetSpritePosition(sFrogLeft.iSpriteID, sPhantomFrog.xPos, sPhantomFrog.yPos);
					UG::DrawSprite(sFrogLeft.iSpriteID);
				}
				break;
			case RIGHT:
				if (fMovementTimer < .25)
				{
					UG::SetSpritePosition(sFrogRightJump.iSpriteID, sPhantomFrog.xPos, sPhantomFrog.yPos);
					UG::DrawSprite(sFrogRightJump.iSpriteID);
				}
				else
				{
					UG::SetSpritePosition(sFrogRight.iSpriteID, sPhantomFrog.xPos, sPhantomFrog.yPos);
					UG::DrawSprite(sFrogRight.iSpriteID);
				}
				break;
			};
		}					
#pragma endregion

#pragma region Exit
		if (iCurrentLives <= 0) 
		// Take player to 'Game over' screen when player runs out of lives.
		{
			iCurrentLives = iStartLives;
			eCurrentState = GAMEOVER;
		}
		// Quit out to the menu when escape is pressed.
		if (UG::WasKeyPressed(UG::KEY_ESCAPE))
			eCurrentState = MENU;
		break;
#pragma endregion

#pragma endregion

#pragma region GAMEOVER
		case GAMEOVER:

#pragma region User input
			// Selects the letter pressed to be drawn, adds that letter to the player name and moves the current character selection 
			if (UG::WasKeyPressed(UG::KEY_A) && iCounter <= 4)
			{
				if (iCounter == 0) 
				{
					eCurrentLetter1 = A;
					caPlayerName[0] = 'A';
				}
				else if (iCounter == 1)
				{
					eCurrentLetter2 = A;
					caPlayerName[1] = 'A';
				}
				else if (iCounter == 2)
				{
					eCurrentLetter3 = A;
					caPlayerName[2] = 'A';
				}
				else if (iCounter == 3)
				{
					eCurrentLetter4 = A;
					caPlayerName[3] = 'A';
				}
				else if (iCounter == 4)
				{
					eCurrentLetter5 = A;
					caPlayerName[4] = 'A';
				}

				iCounter += 1;
				fHSTimer = 0.0f; //resetting this timer causes the underscore on the selected character to start as black so it's more noticable which character is selected
			}
			if (UG::WasKeyPressed(UG::KEY_B) && iCounter <= 4)
			{
				if (iCounter == 0)
				{
					eCurrentLetter1 = B;
					caPlayerName[0] = 'B';
				}
				else if (iCounter == 1)
				{
					eCurrentLetter2 = B;
					caPlayerName[1] = 'B';
				}
				else if (iCounter == 2)
				{
					eCurrentLetter3 = B;
					caPlayerName[2] = 'B';
				}
				else if (iCounter == 3)
				{
					eCurrentLetter4 = B;
					caPlayerName[3] = 'B';
				}
				else if (iCounter == 4)
				{
					eCurrentLetter5 = B;
					caPlayerName[4] = 'B';
				}

				iCounter += 1;
				fHSTimer = 0.0f;
			}
			if (UG::WasKeyPressed(UG::KEY_C) && iCounter <= 4)
			{
				if (iCounter == 0)
				{
					eCurrentLetter1 = C;
					caPlayerName[0] = 'C';
				}
				else if (iCounter == 1)
				{
					eCurrentLetter2 = C;
					caPlayerName[1] = 'C';
				}
				else if (iCounter == 2)
				{
					eCurrentLetter3 = C;
					caPlayerName[2] = 'C';
				}
				else if (iCounter == 3)
				{
					eCurrentLetter4 = C;
					caPlayerName[3] = 'C';
				}
				else if (iCounter == 4)
				{
					eCurrentLetter5 = C;
					caPlayerName[4] = 'C';
				}

				iCounter += 1;
				fHSTimer = 0.0f;
			}
			if (UG::WasKeyPressed(UG::KEY_D) && iCounter <= 4)
			{
				if (iCounter == 0)
				{
					eCurrentLetter1 = D;
					caPlayerName[0] = 'D';
				}
				else if (iCounter == 1)
				{
					eCurrentLetter2 = D;
					caPlayerName[1] = 'D';
				}
				else if (iCounter == 2)
				{
					eCurrentLetter3 = D;
					caPlayerName[2] = 'D';
				}
				else if (iCounter == 3)
				{
					eCurrentLetter4 = D;
					caPlayerName[3] = 'D';
				}
				else if (iCounter == 4)
				{
					eCurrentLetter5 = D;
					caPlayerName[4] = 'D';
				}

				iCounter += 1;
				fHSTimer = 0.0f;
			}
			if (UG::WasKeyPressed(UG::KEY_E) && iCounter <= 4)
			{
				if (iCounter == 0)
				{
					eCurrentLetter1 = E;
					caPlayerName[0] = 'E';
				}
				else if (iCounter == 1)
				{
					eCurrentLetter2 = E;
					caPlayerName[1] = 'E';
				}
				else if (iCounter == 2)
				{
					eCurrentLetter3 = E;
					caPlayerName[2] = 'E';
				}
				else if (iCounter == 3)
				{
					eCurrentLetter4 = E;
					caPlayerName[3] = 'E';
				}
				else if (iCounter == 4)
				{
					eCurrentLetter5 = E;
					caPlayerName[4] = 'E';
				}

				iCounter += 1;
				fHSTimer = 0.0f;
			}
			if (UG::WasKeyPressed(UG::KEY_F) && iCounter <= 4)
			{
				if (iCounter == 0)
				{
					eCurrentLetter1 = F;
					caPlayerName[0] = 'F';
				}
				else if (iCounter == 1)
				{
					eCurrentLetter2 = F;
					caPlayerName[1] = 'F';
				}
				else if (iCounter == 2)
				{
					eCurrentLetter3 = F;
					caPlayerName[2] = 'F';
				}
				else if (iCounter == 3)
				{
					eCurrentLetter4 = F;
					caPlayerName[3] = 'F';
				}
				else if (iCounter == 4)
				{
					eCurrentLetter5 = F;
					caPlayerName[4] = 'F';
				}

				iCounter += 1;
				fHSTimer = 0.0f;
			}
			if (UG::WasKeyPressed(UG::KEY_G) && iCounter <= 4)
			{
				if (iCounter == 0)
				{
					eCurrentLetter1 = G;
					caPlayerName[0] = 'G';
				}
				else if (iCounter == 1)
				{
					eCurrentLetter2 = G;
					caPlayerName[1] = 'G';
				}
				else if (iCounter == 2)
				{
					eCurrentLetter3 = G;
					caPlayerName[2] = 'G';
				}
				else if (iCounter == 3)
				{
					eCurrentLetter4 = G;
					caPlayerName[3] = 'G';
				}
				else if (iCounter == 4)
				{
					eCurrentLetter5 = G;
					caPlayerName[4] = 'G';
				}

				iCounter += 1;
				fHSTimer = 0.0f;
			}
			if (UG::WasKeyPressed(UG::KEY_H) && iCounter <= 4)
			{
				if (iCounter == 0)
				{
					eCurrentLetter1 = H;
					caPlayerName[0] = 'H';
				}
				else if (iCounter == 1)
				{
					eCurrentLetter2 = H;
					caPlayerName[1] = 'H';
				}
				else if (iCounter == 2)
				{
					eCurrentLetter3 = H;
					caPlayerName[2] = 'H';
				}
				else if (iCounter == 3)
				{
					eCurrentLetter4 = H;
					caPlayerName[3] = 'H';
				}
				else if (iCounter == 4)
				{
					eCurrentLetter5 = H;
					caPlayerName[4] = 'H';
				}

				iCounter += 1;
				fHSTimer = 0.0f;
			}
			if (UG::WasKeyPressed(UG::KEY_I) && iCounter <= 4)
			{
				if (iCounter == 0)
				{
					eCurrentLetter1 = I;
					caPlayerName[0] = 'I';
				}
				else if (iCounter == 1)
				{
					eCurrentLetter2 = I;
					caPlayerName[1] = 'I';
				}
				else if (iCounter == 2)
				{
					eCurrentLetter3 = I;
					caPlayerName[2] = 'I';
				}
				else if (iCounter == 3)
				{
					eCurrentLetter4 = I;
					caPlayerName[3] = 'I';
				}
				else if (iCounter == 4)
				{
					eCurrentLetter5 = I;
					caPlayerName[4] = 'I';
				}

				iCounter += 1;
				fHSTimer = 0.0f;
			}
			if (UG::WasKeyPressed(UG::KEY_J) && iCounter <= 4)
			{
				if (iCounter == 0)
				{
					eCurrentLetter1 = J;
					caPlayerName[0] = 'J';
				}
				else if (iCounter == 1)
				{
					eCurrentLetter2 = J;
					caPlayerName[1] = 'J';
				}
				else if (iCounter == 2)
				{
					eCurrentLetter3 = J;
					caPlayerName[2] = 'J';
				}
				else if (iCounter == 3)
				{
					eCurrentLetter4 = J;
					caPlayerName[3] = 'J';
				}
				else if (iCounter == 4)
				{
					eCurrentLetter5 = J;
					caPlayerName[4] = 'J';
				}

				iCounter += 1;
				fHSTimer = 0.0f;
			}
			if (UG::WasKeyPressed(UG::KEY_K) && iCounter <= 4)
			{
				if (iCounter == 0)
				{
					eCurrentLetter1 = K;
					caPlayerName[0] = 'K';
				}
				else if (iCounter == 1)
				{
					eCurrentLetter2 = K;
					caPlayerName[1] = 'K';
				}
				else if (iCounter == 2)
				{
					eCurrentLetter3 = K;
					caPlayerName[2] = 'K';
				}
				else if (iCounter == 3)
				{
					eCurrentLetter4 = K;
					caPlayerName[3] = 'K';
				}
				else if (iCounter == 4)
				{
					eCurrentLetter5 = K;
					caPlayerName[4] = 'K';
				}

				iCounter += 1;
				fHSTimer = 0.0f;
			}
			if (UG::WasKeyPressed(UG::KEY_L) && iCounter <= 4)
			{
				if (iCounter == 0)
				{
					eCurrentLetter1 = L;
					caPlayerName[0] = 'L';
				}
				else if (iCounter == 1)
				{
					eCurrentLetter2 = L;
					caPlayerName[1] = 'L';
				}
				else if (iCounter == 2)
				{
					eCurrentLetter3 = L;
					caPlayerName[2] = 'L';
				}
				else if (iCounter == 3)
				{
					eCurrentLetter4 = L;
					caPlayerName[3] = 'L';
				}
				else if (iCounter == 4)
				{
					eCurrentLetter5 = L;
					caPlayerName[4] = 'L';
				}

				iCounter += 1;
				fHSTimer = 0.0f;
			}
			if (UG::WasKeyPressed(UG::KEY_M) && iCounter <= 4)
			{
				if (iCounter == 0)
				{
					eCurrentLetter1 = M;
					caPlayerName[0] = 'M';
				}
				else if (iCounter == 1)
				{
					eCurrentLetter2 = M;
					caPlayerName[1] = 'M';
				}
				else if (iCounter == 2)
				{
					eCurrentLetter3 = M;
					caPlayerName[2] = 'M';
				}
				else if (iCounter == 3)
				{
					eCurrentLetter4 = M;
					caPlayerName[3] = 'M';
				}
				else if (iCounter == 4)
				{
					eCurrentLetter5 = M;
					caPlayerName[4] = 'M';
				}

				iCounter += 1;
				fHSTimer = 0.0f;
			}
			if (UG::WasKeyPressed(UG::KEY_N) && iCounter <= 4)
			{
				if (iCounter == 0)
				{
					eCurrentLetter1 = N;
					caPlayerName[0] = 'N';
				}
				else if (iCounter == 1)
				{
					eCurrentLetter2 = N;
					caPlayerName[1] = 'N';
				}
				else if (iCounter == 2)
				{
					eCurrentLetter3 = N;
					caPlayerName[2] = 'N';
				}
				else if (iCounter == 3)
				{
					eCurrentLetter4 = N;
					caPlayerName[3] = 'N';
				}
				else if (iCounter == 4)
				{
					eCurrentLetter5 = N;
					caPlayerName[4] = 'N';
				}

				iCounter += 1;
				fHSTimer = 0.0f;
			}
			if (UG::WasKeyPressed(UG::KEY_O) && iCounter <= 4)
			{
				if (iCounter == 0)
				{
					eCurrentLetter1 = O;
					caPlayerName[0] = 'O';
				}
				else if (iCounter == 1)
				{
					eCurrentLetter2 = O;
					caPlayerName[1] = 'O';
				}
				else if (iCounter == 2)
				{
					eCurrentLetter3 = O;
					caPlayerName[2] = 'O';
				}
				else if (iCounter == 3)
				{
					eCurrentLetter4 = O;
					caPlayerName[3] = 'O';
				}
				else if (iCounter == 4)
				{
					eCurrentLetter5 = O;
					caPlayerName[4] = 'O';
				}

				iCounter += 1;
				fHSTimer = 0.0f;
			}
			if (UG::WasKeyPressed(UG::KEY_P) && iCounter <= 4)
			{
				if (iCounter == 0)
				{
					eCurrentLetter1 = P;
					caPlayerName[0] = 'P';
				}
				else if (iCounter == 1)
				{
					eCurrentLetter2 = P;
					caPlayerName[1] = 'P';
				}
				else if (iCounter == 2)
				{
					eCurrentLetter3 = P;
					caPlayerName[2] = 'P';
				}
				else if (iCounter == 3)
				{
					eCurrentLetter4 = P;
					caPlayerName[3] = 'P';
				}
				else if (iCounter == 4)
				{
					eCurrentLetter5 = P;
					caPlayerName[4] = 'P';
				}

				iCounter += 1;
				fHSTimer = 0.0f;
			}
			if (UG::WasKeyPressed(UG::KEY_Q) && iCounter <= 4)
			{
				if (iCounter == 0)
				{
					eCurrentLetter1 = Q;
					caPlayerName[0] = 'Q';
				}
				else if (iCounter == 1)
				{
					eCurrentLetter2 = Q;
					caPlayerName[1] = 'Q';
				}
				else if (iCounter == 2)
				{
					eCurrentLetter3 = Q;
					caPlayerName[2] = 'Q';
				}
				else if (iCounter == 3)
				{
					eCurrentLetter4 = Q;
					caPlayerName[3] = 'Q';
				}
				else if (iCounter == 4)
				{
					eCurrentLetter5 = Q;
					caPlayerName[4] = 'Q';
				}

				iCounter += 1;
				fHSTimer = 0.0f;
			}
			if (UG::WasKeyPressed(UG::KEY_R) && iCounter <= 4)
			{
				if (iCounter == 0)
				{
					eCurrentLetter1 = R;
					caPlayerName[0] = 'R';
				}
				else if (iCounter == 1)
				{
					eCurrentLetter2 = R;
					caPlayerName[1] = 'R';
				}
				else if (iCounter == 2)
				{
					eCurrentLetter3 = R;
					caPlayerName[2] = 'R';
				}
				else if (iCounter == 3)
				{
					eCurrentLetter4 = R;
					caPlayerName[3] = 'R';
				}
				else if (iCounter == 4)
				{
					eCurrentLetter5 = R;
					caPlayerName[4] = 'R';
				}

				iCounter += 1;
				fHSTimer = 0.0f;
			}
			if (UG::WasKeyPressed(UG::KEY_S) && iCounter <= 4)
			{
				if (iCounter == 0)
				{
					eCurrentLetter1 = S;
					caPlayerName[0] = 'S';
				}
				else if (iCounter == 1)
				{
					eCurrentLetter2 = S;
					caPlayerName[1] = 'S';
				}
				else if (iCounter == 2)
				{
					eCurrentLetter3 = S;
					caPlayerName[2] = 'S';
				}
				else if (iCounter == 3)
				{
					eCurrentLetter4 = S;
					caPlayerName[3] = 'S';
				}
				else if (iCounter == 4)
				{
					eCurrentLetter5 = S;
					caPlayerName[4] = 'S';
				}

				iCounter += 1;
				fHSTimer = 0.0f;
			}
			if (UG::WasKeyPressed(UG::KEY_T) && iCounter <= 4)
			{
				if (iCounter == 0)
				{
					eCurrentLetter1 = T;
					caPlayerName[0] = 'T';
				}
				else if (iCounter == 1)
				{
					eCurrentLetter2 = T;
					caPlayerName[1] = 'T';
				}
				else if (iCounter == 2)
				{
					eCurrentLetter3 = T;
					caPlayerName[2] = 'T';
				}
				else if (iCounter == 3)
				{
					eCurrentLetter4 = T;
					caPlayerName[3] = 'T';
				}
				else if (iCounter == 4)
				{
					eCurrentLetter5 = T;
					caPlayerName[4] = 'T';
				}

				iCounter += 1;
				fHSTimer = 0.0f;
			}
			if (UG::WasKeyPressed(UG::KEY_U) && iCounter <= 4)
			{
				if (iCounter == 0)
				{
					eCurrentLetter1 = U;
					caPlayerName[0] = 'U';
				}
				else if (iCounter == 1)
				{
					eCurrentLetter2 = U;
					caPlayerName[1] = 'U';
				}
				else if (iCounter == 2)
				{
					eCurrentLetter3 = U;
					caPlayerName[2] = 'U';
				}
				else if (iCounter == 3)
				{
					eCurrentLetter4 = U;
					caPlayerName[3] = 'U';
				}
				else if (iCounter == 4)
				{
					eCurrentLetter5 = U;
					caPlayerName[4] = 'U';
				}

				iCounter += 1;
				fHSTimer = 0.0f;
			}
			if (UG::WasKeyPressed(UG::KEY_V) && iCounter <= 4)
			{
				if (iCounter == 0)
				{
					eCurrentLetter1 = V;
					caPlayerName[0] = 'V';
				}
				else if (iCounter == 1)
				{
					eCurrentLetter2 = V;
					caPlayerName[1] = 'V';
				}
				else if (iCounter == 2)
				{
					eCurrentLetter3 = V;
					caPlayerName[2] = 'V';
				}
				else if (iCounter == 3)
				{
					eCurrentLetter4 = V;
					caPlayerName[3] = 'V';
				}
				else if (iCounter == 4)
				{
					eCurrentLetter5 = V;
					caPlayerName[4] = 'V';
				}

				iCounter += 1;
				fHSTimer = 0.0f;
			}
			if (UG::WasKeyPressed(UG::KEY_W) && iCounter <= 4)
			{
				if (iCounter == 0)
				{
					eCurrentLetter1 = W;
					caPlayerName[0] = 'W';
				}
				else if (iCounter == 1)
				{
					eCurrentLetter2 = W;
					caPlayerName[1] = 'W';
				}
				else if (iCounter == 2)
				{
					eCurrentLetter3 = W;
					caPlayerName[2] = 'W';
				}
				else if (iCounter == 3)
				{
					eCurrentLetter4 = W;
					caPlayerName[3] = 'W';
				}
				else if (iCounter == 4)
				{
					eCurrentLetter5 = W;
					caPlayerName[4] = 'W';
				}

				iCounter += 1;
				fHSTimer = 0.0f;
			}
			if (UG::WasKeyPressed(UG::KEY_X) && iCounter <= 4)
			{
				if (iCounter == 0)
				{
					eCurrentLetter1 = X;
					caPlayerName[0] = 'X';
				}
				else if (iCounter == 1)
				{
					eCurrentLetter2 = X;
					caPlayerName[1] = 'X';
				}
				else if (iCounter == 2)
				{
					eCurrentLetter3 = X;
					caPlayerName[2] = 'X';
				}
				else if (iCounter == 3)
				{
					eCurrentLetter4 = X;
					caPlayerName[3] = 'X';
				}
				else if (iCounter == 4)
				{
					eCurrentLetter5 = X;
					caPlayerName[4] = 'X';
				}

				iCounter += 1;
				fHSTimer = 0.0f;
			}
			if (UG::WasKeyPressed(UG::KEY_Y) && iCounter <= 4)
			{
				if (iCounter == 0)
				{
					eCurrentLetter1 = Y;
					caPlayerName[0] = 'Y';
				}
				else if (iCounter == 1)
				{
					eCurrentLetter2 = Y;
					caPlayerName[1] = 'Y';
				}
				else if (iCounter == 2)
				{
					eCurrentLetter3 = Y;
					caPlayerName[1] = 'Y';
				}
				else if (iCounter == 3)
				{
					eCurrentLetter4 = Y;
					caPlayerName[1] = 'Y';
				}
				else if (iCounter == 4)
				{
					eCurrentLetter5 = Y;
					caPlayerName[1] = 'Y';
				}

				iCounter += 1;
				fHSTimer = 0.0f;
			}
			if (UG::WasKeyPressed(UG::KEY_Z) && iCounter <= 4)
			{
				if (iCounter == 0)
				{
					eCurrentLetter1 = Z;
					caPlayerName[0] = 'Z';
				}
				else if (iCounter == 1)
				{
					eCurrentLetter2 = Z;
					caPlayerName[1] = 'Z';
				}
				else if (iCounter == 2)
				{
					eCurrentLetter3 = Z;
					caPlayerName[2] = 'Z';
				}
				else if (iCounter == 3)
				{
					eCurrentLetter4 = Z;
					caPlayerName[3] = 'Z';
				}
				else if (iCounter == 4)
				{
					eCurrentLetter5 = Z;
					caPlayerName[4] = 'Z';
				}

				iCounter += 1;
				fHSTimer = 0.0f;
			}
			// Deletes last letter entry and moves the selection back.
			if (UG::WasKeyPressed(UG::KEY_BACKSPACE) && iCounter >= 1) 
			{
				if (iCounter == 5) 
				{
					eCurrentLetter5 = SPACE;
					caPlayerName[4] = ' ';
				}
				else if (iCounter == 4)
				{
					eCurrentLetter4 = SPACE;
					caPlayerName[4] = ' ';
				}
				else if (iCounter == 3)
				{
					eCurrentLetter3 = SPACE;
					caPlayerName[4] = ' ';
				}
				else if (iCounter == 2)
				{
					eCurrentLetter2 = SPACE;
					caPlayerName[4] = ' ';
				}
				else if (iCounter == 1)
				{
					eCurrentLetter1 = SPACE;
					caPlayerName[4] = ' ';
				}

				iCounter -= 1;
				fHSTimer = 0.0f;
			}
#pragma endregion

#pragma region First letter
			// Draws whichever letter is selected as the first character.
			switch (eCurrentLetter1)
			{
			case SPACE:
				UG::DrawString("", iScreenWidth / 2 - 2 * iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow); 
				break;
			case A:
				UG::DrawString("A", iScreenWidth / 2 - 2 * iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case B:
				UG::DrawString("B", iScreenWidth / 2 - 2 * iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case C:
				UG::DrawString("C", iScreenWidth / 2 - 2 * iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case D:
				UG::DrawString("D", iScreenWidth / 2 - 2 * iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case E:
				UG::DrawString("E", iScreenWidth / 2 - 2 * iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case F:
				UG::DrawString("F", iScreenWidth / 2 - 2 * iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case G:
				UG::DrawString("G", iScreenWidth / 2 - 2 * iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case H:
				UG::DrawString("H", iScreenWidth / 2 - 2 * iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case I:
				UG::DrawString("I", iScreenWidth / 2 - 2 * iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case J:
				UG::DrawString("J", iScreenWidth / 2 - 2 * iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case K:
				UG::DrawString("K", iScreenWidth / 2 - 2 * iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case L:
				UG::DrawString("L", iScreenWidth / 2 - 2 * iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case M:
				UG::DrawString("M", iScreenWidth / 2 - 2 * iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case N:
				UG::DrawString("N", iScreenWidth / 2 - 2 * iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case O:
				UG::DrawString("O", iScreenWidth / 2 - 2 * iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case P:
				UG::DrawString("P", iScreenWidth / 2 - 2 * iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case Q:
				UG::DrawString("Q", iScreenWidth / 2 - 2 * iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case R:
				UG::DrawString("R", iScreenWidth / 2 - 2 * iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case S:
				UG::DrawString("S", iScreenWidth / 2 - 2 * iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case T:
				UG::DrawString("T", iScreenWidth / 2 - 2 * iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case U:
				UG::DrawString("U", iScreenWidth / 2 - 2 * iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case V:
				UG::DrawString("V", iScreenWidth / 2 - 2 * iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case W:
				UG::DrawString("W", iScreenWidth / 2 - 2 * iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case X:
				UG::DrawString("X", iScreenWidth / 2 - 2 * iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case Y:
				UG::DrawString("Y", iScreenWidth / 2 - 2 * iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case Z:
				UG::DrawString("Z", iScreenWidth / 2 - 2 * iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			}
#pragma endregion

#pragma region Second letter.
			// Draws whichever letter is selected as the second character.
			switch (eCurrentLetter2)
			{
			case SPACE:
				UG::DrawString("", iScreenWidth / 2 - iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case A:
				UG::DrawString("A", iScreenWidth / 2 - iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case B:
				UG::DrawString("B", iScreenWidth / 2 - iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case C:
				UG::DrawString("C", iScreenWidth / 2 - iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case D:
				UG::DrawString("D", iScreenWidth / 2 - iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case E:
				UG::DrawString("E", iScreenWidth / 2 - iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case F:
				UG::DrawString("F", iScreenWidth / 2 - iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case G:
				UG::DrawString("G", iScreenWidth / 2 - iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case H:
				UG::DrawString("H", iScreenWidth / 2 - iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case I:
				UG::DrawString("I", iScreenWidth / 2 - iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case J:
				UG::DrawString("J", iScreenWidth / 2 - iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case K:
				UG::DrawString("K", iScreenWidth / 2 - iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case L:
				UG::DrawString("L", iScreenWidth / 2 - iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case M:
				UG::DrawString("M", iScreenWidth / 2 - iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case N:
				UG::DrawString("N", iScreenWidth / 2 - iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case O:
				UG::DrawString("O", iScreenWidth / 2 - iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case P:
				UG::DrawString("P", iScreenWidth / 2 - iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case Q:
				UG::DrawString("Q", iScreenWidth / 2 - iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case R:
				UG::DrawString("R", iScreenWidth / 2 - iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case S:
				UG::DrawString("S", iScreenWidth / 2 - iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case T:
				UG::DrawString("T", iScreenWidth / 2 - iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case U:
				UG::DrawString("U", iScreenWidth / 2 - iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case V:
				UG::DrawString("V", iScreenWidth / 2 - iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case W:
				UG::DrawString("W", iScreenWidth / 2 - iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case X:
				UG::DrawString("X", iScreenWidth / 2 - iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case Y:
				UG::DrawString("Y", iScreenWidth / 2 - iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case Z:
				UG::DrawString("Z", iScreenWidth / 2 - iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			}
#pragma endregion

#pragma region Third letter
			// Draws whichever letter is selected as the third character.
			switch (eCurrentLetter3)
			{
			case SPACE:
				UG::DrawString("", iScreenWidth / 2 - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case A:
				UG::DrawString("A", iScreenWidth / 2 - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case B:
				UG::DrawString("B", iScreenWidth / 2 - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case C:
				UG::DrawString("C", iScreenWidth / 2 - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case D:
				UG::DrawString("D", iScreenWidth / 2 - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case E:
				UG::DrawString("E", iScreenWidth / 2 - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case F:
				UG::DrawString("F", iScreenWidth / 2 - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case G:
				UG::DrawString("G", iScreenWidth / 2 - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case H:
				UG::DrawString("H", iScreenWidth / 2 - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case I:
				UG::DrawString("I", iScreenWidth / 2 - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case J:
				UG::DrawString("J", iScreenWidth / 2 - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case K:
				UG::DrawString("K", iScreenWidth / 2 - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case L:
				UG::DrawString("L", iScreenWidth / 2 - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case M:
				UG::DrawString("M", iScreenWidth / 2 - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case N:
				UG::DrawString("N", iScreenWidth / 2 - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case O:
				UG::DrawString("O", iScreenWidth / 2 - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case P:
				UG::DrawString("P", iScreenWidth / 2 - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case Q:
				UG::DrawString("Q", iScreenWidth / 2 - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case R:
				UG::DrawString("R", iScreenWidth / 2 - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case S:
				UG::DrawString("S", iScreenWidth / 2 - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case T:
				UG::DrawString("T", iScreenWidth / 2 - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case U:
				UG::DrawString("U", iScreenWidth / 2 - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case V:
				UG::DrawString("V", iScreenWidth / 2 - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case W:
				UG::DrawString("W", iScreenWidth / 2 - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case X:
				UG::DrawString("X", iScreenWidth / 2 - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case Y:
				UG::DrawString("Y", iScreenWidth / 2 - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case Z:
				UG::DrawString("Z", iScreenWidth / 2 - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			}
#pragma endregion

#pragma region Fourth letter
			// Draws whichever letter is selected as the fourth character.
			switch (eCurrentLetter4)
			{
			case SPACE:
				UG::DrawString("", iScreenWidth / 2 + iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case A:
				UG::DrawString("A", iScreenWidth / 2 + iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case B:
				UG::DrawString("B", iScreenWidth / 2 + iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case C:
				UG::DrawString("C", iScreenWidth / 2 + iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case D:
				UG::DrawString("D", iScreenWidth / 2 + iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case E:
				UG::DrawString("E", iScreenWidth / 2 + iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case F:
				UG::DrawString("F", iScreenWidth / 2 + iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case G:
				UG::DrawString("G", iScreenWidth / 2 + iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case H:
				UG::DrawString("H", iScreenWidth / 2 + iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case I:
				UG::DrawString("I", iScreenWidth / 2 + iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case J:
				UG::DrawString("J", iScreenWidth / 2 + iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case K:
				UG::DrawString("K", iScreenWidth / 2 + iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case L:
				UG::DrawString("L", iScreenWidth / 2 + iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case M:
				UG::DrawString("M", iScreenWidth / 2 + iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case N:
				UG::DrawString("N", iScreenWidth / 2 + iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case O:
				UG::DrawString("O", iScreenWidth / 2 + iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case P:
				UG::DrawString("P", iScreenWidth / 2 + iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case Q:
				UG::DrawString("Q", iScreenWidth / 2 + iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case R:
				UG::DrawString("R", iScreenWidth / 2 + iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case S:
				UG::DrawString("S", iScreenWidth / 2 + iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case T:
				UG::DrawString("T", iScreenWidth / 2 + iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case U:
				UG::DrawString("U", iScreenWidth / 2 + iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case V:
				UG::DrawString("V", iScreenWidth / 2 + iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case W:
				UG::DrawString("W", iScreenWidth / 2 + iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case X:
				UG::DrawString("X", iScreenWidth / 2 + iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case Y:
				UG::DrawString("Y", iScreenWidth / 2 + iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case Z:
				UG::DrawString("Z", iScreenWidth / 2 + iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			}
#pragma endregion

#pragma region Fifth letter
			// Draws whichever letter is selected as the fifth character.
			switch (eCurrentLetter5)
			{
			case SPACE:
				UG::DrawString("", iScreenWidth / 2 + 2 * iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case A:
				UG::DrawString("A", iScreenWidth / 2 + 2 * iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case B:
				UG::DrawString("B", iScreenWidth / 2 + 2 * iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case C:
				UG::DrawString("C", iScreenWidth / 2 + 2 * iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case D:
				UG::DrawString("D", iScreenWidth / 2 + 2 * iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case E:
				UG::DrawString("E", iScreenWidth / 2 + 2 * iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case F:
				UG::DrawString("F", iScreenWidth / 2 + 2 * iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case G:
				UG::DrawString("G", iScreenWidth / 2 + 2 * iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case H:
				UG::DrawString("H", iScreenWidth / 2 + 2 * iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case I:
				UG::DrawString("I", iScreenWidth / 2 + 2 * iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case J:
				UG::DrawString("J", iScreenWidth / 2 + 2 * iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case K:
				UG::DrawString("K", iScreenWidth / 2 + 2 * iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case L:
				UG::DrawString("L", iScreenWidth / 2 + 2 * iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case M:
				UG::DrawString("M", iScreenWidth / 2 + 2 * iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case N:
				UG::DrawString("N", iScreenWidth / 2 + 2 * iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case O:
				UG::DrawString("O", iScreenWidth / 2 + 2 * iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case P:
				UG::DrawString("P", iScreenWidth / 2 + 2 * iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case Q:
				UG::DrawString("Q", iScreenWidth / 2 + 2 * iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case R:
				UG::DrawString("R", iScreenWidth / 2 + 2 * iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case S:
				UG::DrawString("S", iScreenWidth / 2 + 2 * iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case T:
				UG::DrawString("T", iScreenWidth / 2 + 2 * iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case U:
				UG::DrawString("U", iScreenWidth / 2 + 2 * iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case V:
				UG::DrawString("V", iScreenWidth / 2 + 2 * iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case W:
				UG::DrawString("W", iScreenWidth / 2 + 2 * iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case X:
				UG::DrawString("X", iScreenWidth / 2 + 2 * iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case Y:
				UG::DrawString("Y", iScreenWidth / 2 + 2 * iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			case Z:
				UG::DrawString("Z", iScreenWidth / 2 + 2 * iSpriteWidth - 15, iScreenHeight / 2 - 2 * iSpriteHeight, 0, yellow);
				break;
			}
#pragma endregion

#pragma region Draw underlines
			// Draws the underlines which indicate which character in the player name the player is selecting.
			/// First underline.
			UG::SetSpritePosition(sLetterUnderline.iSpriteID, iScreenWidth / 2 - 2 * iSpriteWidth, iScreenHeight / 2 - 2.25 * iSpriteHeight);
			// Draws a solid yellow line when this character is not currently selected.
			if (iCounter != 0)
			{
				UG::DrawSprite(sLetterUnderline.iSpriteID);
			}
			// Draws a solid yellow line for half a second every second when this character is currently selected.
			else if (fHSTimer - (int)fHSTimer > .5f)
			{
				UG::DrawSprite(sLetterUnderline.iSpriteID);
			}

			/// Second underline.
			UG::SetSpritePosition(sLetterUnderline.iSpriteID, iScreenWidth / 2 - iSpriteWidth, iScreenHeight / 2 - 2.25 * iSpriteHeight);
			if (iCounter != 1)
			{
				UG::DrawSprite(sLetterUnderline.iSpriteID);
			}
			else if (fHSTimer - (int)fHSTimer > .5f)
			{
				UG::DrawSprite(sLetterUnderline.iSpriteID);
			}

			/// Third underline.
			UG::SetSpritePosition(sLetterUnderline.iSpriteID, iScreenWidth / 2, iScreenHeight / 2 - 2.25 * iSpriteHeight);
			if (iCounter != 2)
			{
				UG::DrawSprite(sLetterUnderline.iSpriteID);
			}
			else if (fHSTimer - (int)fHSTimer > .5f)
			{
				UG::DrawSprite(sLetterUnderline.iSpriteID);
			}

			/// Fourth underline.
			UG::SetSpritePosition(sLetterUnderline.iSpriteID, iScreenWidth / 2 + iSpriteWidth, iScreenHeight / 2 - 2.25 * iSpriteHeight);
			if (iCounter != 3)
			{
				UG::DrawSprite(sLetterUnderline.iSpriteID);
			}
			else if (fHSTimer - (int)fHSTimer > .5f)
			{
				UG::DrawSprite(sLetterUnderline.iSpriteID);
			}
			
			/// Fifth underline.
			UG::SetSpritePosition(sLetterUnderline.iSpriteID, iScreenWidth / 2 + 2 * iSpriteWidth, iScreenHeight / 2 - 2.25 * iSpriteHeight);
			if (iCounter != 4)
			{
				UG::DrawSprite(sLetterUnderline.iSpriteID);
			}
			else if (fHSTimer - (int)fHSTimer > .5f)
			{
				UG::DrawSprite(sLetterUnderline.iSpriteID);
			}
#pragma endregion

#pragma region Timer
			// Increment the High score timer.
			fHSTimer += UG::GetDeltaTime();			
#pragma endregion

#pragma region Draw score and label and display level and main text
			// Draws main text.
			UG::DrawString("GAME OVER", 299, iScreenHeight / 2 + 3 * iSpriteHeight, 0, red);
			UG::DrawString("Enter your name:", 192, iScreenHeight / 2, 0, grey);
			// Draws score.
			DrawNumber((int)trunc(iPlayerScore / 10000) % 10, 2.5 * iSpriteWidth, iScreenHeight - 2 * iSpriteHeight / 2, (char)220, (char)20, (char)20);
			DrawNumber((int)trunc(iPlayerScore / 1000) % 10, 3 * iSpriteWidth, iScreenHeight - 2 * iSpriteHeight / 2, (char)220, (char)20, (char)20);
			DrawNumber((int)trunc(iPlayerScore / 100) % 10, 3.5 * iSpriteWidth, iScreenHeight - 2 * iSpriteHeight / 2, (char)220, (char)20, (char)20);
			DrawNumber((int)trunc(iPlayerScore / 10) % 10, 4 * iSpriteWidth, iScreenHeight - 2 * iSpriteHeight / 2, (char)220, (char)20, (char)20);
			DrawNumber(iPlayerScore % 10, 4.5 * iSpriteWidth, iScreenHeight - 2 * iSpriteHeight / 2, (char)220, (char)20, (char)20);
			// Draws score label.
			DrawCharacter('s', 2.5 * iSpriteWidth, iScreenHeight - iSpriteHeight / 2, (char)220, (char)220, (char)220);
			DrawCharacter('c', 3 * iSpriteWidth, iScreenHeight - iSpriteHeight / 2, (char)220, (char)220, (char)220);
			DrawCharacter('o', 3.5 * iSpriteWidth, iScreenHeight - iSpriteHeight / 2, (char)220, (char)220, (char)220);
			DrawCharacter('r', 4 * iSpriteWidth, iScreenHeight - iSpriteHeight / 2, (char)220, (char)220, (char)220);
			DrawCharacter('e', 4.5 * iSpriteWidth, iScreenHeight - iSpriteHeight / 2, (char)220, (char)220, (char)220);
			// Draws level.
			DrawCharacter('l', iScreenWidth / 2 + 2 * iSpriteWidth, iScreenHeight - iSpriteHeight, (char)(255 - trunc(fLevelDisplayTimer / 1.5) * (fLevelDisplayTimer - 1.5) * 255 * 2), (char)(255 - trunc(fLevelDisplayTimer / 1.5) * (fLevelDisplayTimer - 1.5) * 255 * 2), (char)(255 - trunc(fLevelDisplayTimer / 1.5) * (fLevelDisplayTimer - 1.5) * 255 * 2), (char)255); //the colour is solid white for 1.5 seconds then gradually fades to black over the next half a second.
			DrawCharacter('e', iScreenWidth / 2 + 2.5 * iSpriteWidth, iScreenHeight - iSpriteHeight, (char)(255 - trunc(fLevelDisplayTimer / 1.5) * (fLevelDisplayTimer - 1.5) * 255 * 2), (char)(255 - trunc(fLevelDisplayTimer / 1.5) * (fLevelDisplayTimer - 1.5) * 255 * 2), (char)(255 - trunc(fLevelDisplayTimer / 1.5) * (fLevelDisplayTimer - 1.5) * 255 * 2), (char)255);
			DrawCharacter('v', iScreenWidth / 2 + 3 * iSpriteWidth, iScreenHeight - iSpriteHeight, (char)(255 - trunc(fLevelDisplayTimer / 1.5) * (fLevelDisplayTimer - 1.5) * 255 * 2), (char)(255 - trunc(fLevelDisplayTimer / 1.5) * (fLevelDisplayTimer - 1.5) * 255 * 2), (char)(255 - trunc(fLevelDisplayTimer / 1.5) * (fLevelDisplayTimer - 1.5) * 255 * 2), (char)255);
			DrawCharacter('e', iScreenWidth / 2 + 3.5 * iSpriteWidth, iScreenHeight - iSpriteHeight, (char)(255 - trunc(fLevelDisplayTimer / 1.5) * (fLevelDisplayTimer - 1.5) * 255 * 2), (char)(255 - trunc(fLevelDisplayTimer / 1.5) * (fLevelDisplayTimer - 1.5) * 255 * 2), (char)(255 - trunc(fLevelDisplayTimer / 1.5) * (fLevelDisplayTimer - 1.5) * 255 * 2), (char)255);
			DrawCharacter('l', iScreenWidth / 2 + 4 * iSpriteWidth, iScreenHeight - iSpriteHeight, (char)(255 - trunc(fLevelDisplayTimer / 1.5) * (fLevelDisplayTimer - 1.5) * 255 * 2), (char)(255 - trunc(fLevelDisplayTimer / 1.5) * (fLevelDisplayTimer - 1.5) * 255 * 2), (char)(255 - trunc(fLevelDisplayTimer / 1.5) * (fLevelDisplayTimer - 1.5) * 255 * 2), (char)255);
			DrawNumber(iLevel, iScreenWidth / 2 + 5 * iSpriteWidth, iScreenHeight - iSpriteHeight, (char)(255 - trunc(fLevelDisplayTimer / 1.5) * (fLevelDisplayTimer - 1.5) * 255 * 2), (char)(255 - trunc(fLevelDisplayTimer / 1.5) * (fLevelDisplayTimer - 1.5) * 255 * 2), (char)(255 - trunc(fLevelDisplayTimer / 1.5) * (fLevelDisplayTimer - 1.5) * 255 * 2), (char)255);
#pragma endregion

#pragma region Next screen
			// Quit out to the menu when escape is pressed and reset values.
			if (UG::WasKeyPressed(UG::KEY_ESCAPE)) 
			{
				eCurrentState = MENU;
				// Reset values.
				fHSTimer = 0.0f;
				eCurrentLetter1 = SPACE;
				eCurrentLetter2 = SPACE;
				eCurrentLetter3 = SPACE;
				eCurrentLetter4 = SPACE;
				eCurrentLetter5 = SPACE;
				iPlayerScore = 0;
				bClaimed = false;
				iLevel = 1;
				bHasReachedRow1 = false;
				bHasReachedRow2 = false;
				bHasReachedRow3 = false;
				bHasReachedRow4 = false;
				bHasReachedRow5 = false;
				bHasReachedRow6 = false;
				bHasReachedRow7 = false;
				bHasReachedRow8 = false;
				bHasReachedRow9 = false;
				bHasReachedRow10 = false;
				bHasReachedRow11 = false;
				bHasReachedLilyPad1 = false;
				bHasReachedLilyPad2 = false;
				bHasReachedLilyPad3 = false;
				bHasReachedLilyPad4 = false;
				bHasReachedLilyPad5 = false;
				fLevelDisplayTimer = 0.0f;
				fGameTimer = 30.0f;
				iCounter = 0;
				caPlayerName[0] = ' ';
				caPlayerName[1] = ' ';
				caPlayerName[2] = ' ';
				caPlayerName[3] = ' ';
				caPlayerName[4] = ' ';
			}
			// If enter is pressed, the score, along with the player's name, is inserted into the correct place on the High scores list, the screen changes to display the High scores and values are reset.
			if (UG::WasKeyPressed(UG::KEY_ENTER))
			{
				if (iPlayerScore > iaHighScores[0]) 
				{
					iaHighScores[0] = iPlayerScore;
					caHighScoreNames[0][0] = caPlayerName[0];
					caHighScoreNames[0][1] = caPlayerName[1];
					caHighScoreNames[0][2] = caPlayerName[2];
					caHighScoreNames[0][3] = caPlayerName[3];
					caHighScoreNames[0][4] = caPlayerName[4];

					if (iPlayerScore > iaHighScores[1]) 					
					{
						iaHighScores[0] = iaHighScores[1];
						iaHighScores[1] = iPlayerScore;
						caHighScoreNames[0][0] = caHighScoreNames[1][0];
						caHighScoreNames[0][1] = caHighScoreNames[1][1];
						caHighScoreNames[0][2] = caHighScoreNames[1][2];
						caHighScoreNames[0][3] = caHighScoreNames[1][3];
						caHighScoreNames[0][4] = caHighScoreNames[1][4];
						caHighScoreNames[1][0] = caPlayerName[0];
						caHighScoreNames[1][1] = caPlayerName[1];
						caHighScoreNames[1][2] = caPlayerName[2];
						caHighScoreNames[1][3] = caPlayerName[3];
						caHighScoreNames[1][4] = caPlayerName[4];

						if (iPlayerScore > iaHighScores[2])
						{
							iaHighScores[1] = iaHighScores[2];
							iaHighScores[2] = iPlayerScore;
							caHighScoreNames[1][0] = caHighScoreNames[2][0];
							caHighScoreNames[1][1] = caHighScoreNames[2][1];
							caHighScoreNames[1][2] = caHighScoreNames[2][2];
							caHighScoreNames[1][3] = caHighScoreNames[2][3];
							caHighScoreNames[1][4] = caHighScoreNames[2][4];
							caHighScoreNames[2][0] = caPlayerName[0];
							caHighScoreNames[2][1] = caPlayerName[1];
							caHighScoreNames[2][2] = caPlayerName[2];
							caHighScoreNames[2][3] = caPlayerName[3];
							caHighScoreNames[2][4] = caPlayerName[4];

							if (iPlayerScore > iaHighScores[3])
							{
								iaHighScores[2] = iaHighScores[3];
								iaHighScores[3] = iPlayerScore;
								caHighScoreNames[2][0] = caHighScoreNames[3][0];
								caHighScoreNames[2][1] = caHighScoreNames[3][1];
								caHighScoreNames[2][2] = caHighScoreNames[3][2];
								caHighScoreNames[2][3] = caHighScoreNames[3][3];
								caHighScoreNames[2][4] = caHighScoreNames[3][4];
								caHighScoreNames[3][0] = caPlayerName[0];
								caHighScoreNames[3][1] = caPlayerName[1];
								caHighScoreNames[3][2] = caPlayerName[2];
								caHighScoreNames[3][3] = caPlayerName[3];
								caHighScoreNames[3][4] = caPlayerName[4];

								if (iPlayerScore > iaHighScores[4])
								{
									iaHighScores[3] = iaHighScores[4];
									iaHighScores[4] = iPlayerScore;
									caHighScoreNames[3][0] = caHighScoreNames[4][0];
									caHighScoreNames[3][1] = caHighScoreNames[4][1];
									caHighScoreNames[3][2] = caHighScoreNames[4][2];
									caHighScoreNames[3][3] = caHighScoreNames[4][3];
									caHighScoreNames[3][4] = caHighScoreNames[4][4];
									caHighScoreNames[4][0] = caPlayerName[0];
									caHighScoreNames[4][1] = caPlayerName[1];
									caHighScoreNames[4][2] = caPlayerName[2];
									caHighScoreNames[4][3] = caPlayerName[3];
									caHighScoreNames[4][4] = caPlayerName[4];

									if (iPlayerScore > iaHighScores[5])
									{
										iaHighScores[4] = iaHighScores[5];
										iaHighScores[5] = iPlayerScore;
										caHighScoreNames[4][0] = caHighScoreNames[5][0];
										caHighScoreNames[4][1] = caHighScoreNames[5][1];
										caHighScoreNames[4][2] = caHighScoreNames[5][2];
										caHighScoreNames[4][3] = caHighScoreNames[5][3];
										caHighScoreNames[4][4] = caHighScoreNames[5][4];
										caHighScoreNames[5][0] = caPlayerName[0];
										caHighScoreNames[5][1] = caPlayerName[1];
										caHighScoreNames[5][2] = caPlayerName[2];
										caHighScoreNames[5][3] = caPlayerName[3];
										caHighScoreNames[5][4] = caPlayerName[4];

										if (iPlayerScore > iaHighScores[6])
										{
											iaHighScores[5] = iaHighScores[6];
											iaHighScores[6] = iPlayerScore;
											caHighScoreNames[5][0] = caHighScoreNames[6][0];
											caHighScoreNames[5][1] = caHighScoreNames[6][1];
											caHighScoreNames[5][2] = caHighScoreNames[6][2];
											caHighScoreNames[5][3] = caHighScoreNames[6][3];
											caHighScoreNames[5][4] = caHighScoreNames[6][4];
											caHighScoreNames[6][0] = caPlayerName[0];
											caHighScoreNames[6][1] = caPlayerName[1];
											caHighScoreNames[6][2] = caPlayerName[2];
											caHighScoreNames[6][3] = caPlayerName[3];
											caHighScoreNames[6][4] = caPlayerName[4];

											if (iPlayerScore > iaHighScores[7])
											{
												iaHighScores[6] = iaHighScores[7];
												iaHighScores[7] = iPlayerScore;
												caHighScoreNames[6][0] = caHighScoreNames[7][0];
												caHighScoreNames[6][1] = caHighScoreNames[7][1];
												caHighScoreNames[6][2] = caHighScoreNames[7][2];
												caHighScoreNames[6][3] = caHighScoreNames[7][3];
												caHighScoreNames[6][4] = caHighScoreNames[7][4];
												caHighScoreNames[7][0] = caPlayerName[0];
												caHighScoreNames[7][1] = caPlayerName[1];
												caHighScoreNames[7][2] = caPlayerName[2];
												caHighScoreNames[7][3] = caPlayerName[3];
												caHighScoreNames[7][4] = caPlayerName[4];

												if (iPlayerScore > iaHighScores[8])
												{
													iaHighScores[7] = iaHighScores[8];
													iaHighScores[8] = iPlayerScore;
													caHighScoreNames[7][0] = caHighScoreNames[8][0];
													caHighScoreNames[7][1] = caHighScoreNames[8][1];
													caHighScoreNames[7][2] = caHighScoreNames[8][2];
													caHighScoreNames[7][3] = caHighScoreNames[8][3];
													caHighScoreNames[7][4] = caHighScoreNames[8][4];
													caHighScoreNames[8][0] = caPlayerName[0];
													caHighScoreNames[8][1] = caPlayerName[1];
													caHighScoreNames[8][2] = caPlayerName[2];
													caHighScoreNames[8][3] = caPlayerName[3];
													caHighScoreNames[8][4] = caPlayerName[4];

													if (iPlayerScore > iaHighScores[9])
													{
														iaHighScores[8] = iaHighScores[9];
														iaHighScores[9] = iPlayerScore;
														caHighScoreNames[8][0] = caHighScoreNames[9][0];
														caHighScoreNames[8][1] = caHighScoreNames[9][1];
														caHighScoreNames[8][2] = caHighScoreNames[9][2];
														caHighScoreNames[8][3] = caHighScoreNames[9][3];
														caHighScoreNames[8][4] = caHighScoreNames[9][4];
														caHighScoreNames[9][0] = caPlayerName[0];
														caHighScoreNames[9][1] = caPlayerName[1];
														caHighScoreNames[9][2] = caPlayerName[2];
														caHighScoreNames[9][3] = caPlayerName[3];
														caHighScoreNames[9][4] = caPlayerName[4];
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
				eCurrentState = HIGHSCORES;				
				// Reset values.
				fHSTimer = 0.0f;
				eCurrentLetter1 = SPACE;
				eCurrentLetter2 = SPACE;
				eCurrentLetter3 = SPACE;
				eCurrentLetter4 = SPACE;
				eCurrentLetter5 = SPACE;
				iPlayerScore = 0;
				bClaimed = false;
				iLevel = 1;
				bHasReachedRow1 = false;
				bHasReachedRow2 = false;
				bHasReachedRow3 = false;
				bHasReachedRow4 = false;
				bHasReachedRow5 = false;
				bHasReachedRow6 = false;
				bHasReachedRow7 = false;
				bHasReachedRow8 = false;
				bHasReachedRow9 = false;
				bHasReachedRow10 = false;
				bHasReachedRow11 = false;
				bHasReachedLilyPad1 = false;
				bHasReachedLilyPad2 = false;
				bHasReachedLilyPad3 = false;
				bHasReachedLilyPad4 = false;
				bHasReachedLilyPad5 = false;
				fLevelDisplayTimer = 0.0f;
				fGameTimer = 30.0f;
				iCounter = 0;
				caPlayerName[0] = ' ';
				caPlayerName[1] = ' ';
				caPlayerName[2] = ' ';
				caPlayerName[3] = ' ';
				caPlayerName[4] = ' ';
			}
			break;
#pragma endregion
#pragma endregion

#pragma region PLAY2
			// This is the game in an earlier version of development that can be accessed by entering the Konami code into the menu screen.
		case PLAY2:
#pragma region Draw background
			// Draws background
			/// Draws 1st (bottom) row.
			for (int i = 0; i < iSpriteColumns; i++)
			{
				float x1Start = backgroundPositions[i].xPos + 4;
				float y1Start = backgroundPositions[i].yPos + 4;
				float x1Finish = backgroundPositions[i].xPos + iSpriteWidth - 4;
				float y1Finish = backgroundPositions[i].yPos + iSpriteHeight - 4;

				float x2Start = backgroundPositions[i].xPos + 4;
				float y2Start = backgroundPositions[i].yPos + iSpriteHeight - 4;
				float x2Finish = backgroundPositions[i].xPos + iSpriteWidth - 4;
				float y2Finish = backgroundPositions[i].yPos + 4;

				float thickness = 1;
				unsigned int layer = 0;
				UG::SColour colour = UG::SColour(0, 0, 0, 255);

				UG::DrawLine(x1Start, y1Start, x1Finish, y1Finish, thickness, layer, colour);
				UG::DrawLine(x2Start, y2Start, x2Finish, y2Finish, thickness, layer, colour);
			}

			/// Draws 2nd row.
			for (int i = iSpriteColumns; i < iSpriteColumns * 2; i++)
			{
				float x1Start = backgroundPositions[i].xPos + 4;
				float y1Start = backgroundPositions[i].yPos + 4;
				float x1Finish = backgroundPositions[i].xPos + iSpriteWidth - 4;
				float y1Finish = backgroundPositions[i].yPos + iSpriteHeight - 4;

				float x2Start = backgroundPositions[i].xPos + 4;
				float y2Start = backgroundPositions[i].yPos + iSpriteHeight - 4;
				float x2Finish = backgroundPositions[i].xPos + iSpriteWidth - 4;
				float y2Finish = backgroundPositions[i].yPos + 4;

				float thickness = 1;
				unsigned int layer = 0;
				UG::SColour colour = UG::SColour(148, 0, 211, 255);

				UG::DrawLine(x1Start, y1Start, x1Finish, y1Finish, thickness, layer, colour);
				UG::DrawLine(x2Start, y2Start, x2Finish, y2Finish, thickness, layer, colour);
			}

			/// Draws 3rd row.
			for (int i = iSpriteColumns * 2; i < iSpriteColumns * 3; i++)
			{
				float x1Start = backgroundPositions[i].xPos + 4;
				float y1Start = backgroundPositions[i].yPos + 4;
				float x1Finish = backgroundPositions[i].xPos + iSpriteWidth - 4;
				float y1Finish = backgroundPositions[i].yPos + iSpriteHeight - 4;

				float x2Start = backgroundPositions[i].xPos + 4;
				float y2Start = backgroundPositions[i].yPos + iSpriteHeight - 4;
				float x2Finish = backgroundPositions[i].xPos + iSpriteWidth - 4;
				float y2Finish = backgroundPositions[i].yPos + 4;

				float thickness = 1;
				unsigned int layer = 0;
				UG::SColour colour = UG::SColour(0, 0, 0, 255);

				UG::DrawLine(x1Start, y1Start, x1Finish, y1Finish, thickness, layer, colour);
				UG::DrawLine(x2Start, y2Start, x2Finish, y2Finish, thickness, layer, colour);
			}

			/// Draws 4th row.
			for (int i = iSpriteColumns * 3; i < iSpriteColumns * 4; i++)
			{
				float x1Start = backgroundPositions[i].xPos + 4;
				float y1Start = backgroundPositions[i].yPos + 4;
				float x1Finish = backgroundPositions[i].xPos + iSpriteWidth - 4;
				float y1Finish = backgroundPositions[i].yPos + iSpriteHeight - 4;

				float x2Start = backgroundPositions[i].xPos + 4;
				float y2Start = backgroundPositions[i].yPos + iSpriteHeight - 4;
				float x2Finish = backgroundPositions[i].xPos + iSpriteWidth - 4;
				float y2Finish = backgroundPositions[i].yPos + 4;

				float thickness = 1;
				unsigned int layer = 0;
				UG::SColour colour = UG::SColour(0, 0, 0, 255);

				UG::DrawLine(x1Start, y1Start, x1Finish, y1Finish, thickness, layer, colour);
				UG::DrawLine(x2Start, y2Start, x2Finish, y2Finish, thickness, layer, colour);
			}

			/// Draws 5th row.
			for (int i = iSpriteColumns * 4; i < iSpriteColumns * 5; i++)
			{
				float x1Start = backgroundPositions[i].xPos + 4;
				float y1Start = backgroundPositions[i].yPos + 4;
				float x1Finish = backgroundPositions[i].xPos + iSpriteWidth - 4;
				float y1Finish = backgroundPositions[i].yPos + iSpriteHeight - 4;

				float x2Start = backgroundPositions[i].xPos + 4;
				float y2Start = backgroundPositions[i].yPos + iSpriteHeight - 4;
				float x2Finish = backgroundPositions[i].xPos + iSpriteWidth - 4;
				float y2Finish = backgroundPositions[i].yPos + 4;

				float thickness = 1;
				unsigned int layer = 0;
				UG::SColour colour = UG::SColour(0, 0, 0, 255);

				UG::DrawLine(x1Start, y1Start, x1Finish, y1Finish, thickness, layer, colour);
				UG::DrawLine(x2Start, y2Start, x2Finish, y2Finish, thickness, layer, colour);
			}

			/// Draws 6th row.
			for (int i = iSpriteColumns * 5; i < iSpriteColumns * 6; i++)
			{
				float x1Start = backgroundPositions[i].xPos + 4;
				float y1Start = backgroundPositions[i].yPos + 4;
				float x1Finish = backgroundPositions[i].xPos + iSpriteWidth - 4;
				float y1Finish = backgroundPositions[i].yPos + iSpriteHeight - 4;

				float x2Start = backgroundPositions[i].xPos + 4;
				float y2Start = backgroundPositions[i].yPos + iSpriteHeight - 4;
				float x2Finish = backgroundPositions[i].xPos + iSpriteWidth - 4;
				float y2Finish = backgroundPositions[i].yPos + 4;

				float thickness = 1;
				unsigned int layer = 0;
				UG::SColour colour = UG::SColour(0, 0, 0, 255);

				UG::DrawLine(x1Start, y1Start, x1Finish, y1Finish, thickness, layer, colour);
				UG::DrawLine(x2Start, y2Start, x2Finish, y2Finish, thickness, layer, colour);
			}

			/// Draws 7th row.
			for (int i = iSpriteColumns * 6; i < iSpriteColumns * 7; i++)
			{
				float x1Start = backgroundPositions[i].xPos + 4;
				float y1Start = backgroundPositions[i].yPos + 4;
				float x1Finish = backgroundPositions[i].xPos + iSpriteWidth - 4;
				float y1Finish = backgroundPositions[i].yPos + iSpriteHeight - 4;

				float x2Start = backgroundPositions[i].xPos + 4;
				float y2Start = backgroundPositions[i].yPos + iSpriteHeight - 4;
				float x2Finish = backgroundPositions[i].xPos + iSpriteWidth - 4;
				float y2Finish = backgroundPositions[i].yPos + 4;

				float thickness = 1;
				unsigned int layer = 0;
				UG::SColour colour = UG::SColour(0, 0, 0, 255);

				UG::DrawLine(x1Start, y1Start, x1Finish, y1Finish, thickness, layer, colour);
				UG::DrawLine(x2Start, y2Start, x2Finish, y2Finish, thickness, layer, colour);
			}

			/// Draws 8th row.
			for (int i = iSpriteColumns * 7; i < iSpriteColumns * 8; i++)
			{
				float x1Start = backgroundPositions[i].xPos + 4;
				float y1Start = backgroundPositions[i].yPos + 4;
				float x1Finish = backgroundPositions[i].xPos + iSpriteWidth - 4;
				float y1Finish = backgroundPositions[i].yPos + iSpriteHeight - 4;

				float x2Start = backgroundPositions[i].xPos + 4;
				float y2Start = backgroundPositions[i].yPos + iSpriteHeight - 4;
				float x2Finish = backgroundPositions[i].xPos + iSpriteWidth - 4;
				float y2Finish = backgroundPositions[i].yPos + 4;

				float thickness = 1;
				unsigned int layer = 0;
				UG::SColour colour = UG::SColour(148, 0, 211, 255);

				UG::DrawLine(x1Start, y1Start, x1Finish, y1Finish, thickness, layer, colour);
				UG::DrawLine(x2Start, y2Start, x2Finish, y2Finish, thickness, layer, colour);
			}

			/// Draws 9th row.
			for (int i = iSpriteColumns * 8; i < iSpriteColumns * 9; i++)
			{
				float x1Start = backgroundPositions[i].xPos + 4;
				float y1Start = backgroundPositions[i].yPos + 4;
				float x1Finish = backgroundPositions[i].xPos + iSpriteWidth - 4;
				float y1Finish = backgroundPositions[i].yPos + iSpriteHeight - 4;

				float x2Start = backgroundPositions[i].xPos + 4;
				float y2Start = backgroundPositions[i].yPos + iSpriteHeight - 4;
				float x2Finish = backgroundPositions[i].xPos + iSpriteWidth - 4;
				float y2Finish = backgroundPositions[i].yPos + 4;

				float thickness = 1;
				unsigned int layer = 0;
				UG::SColour colour = UG::SColour(25, 25, 112, 255);

				UG::DrawLine(x1Start, y1Start, x1Finish, y1Finish, thickness, layer, colour);
				UG::DrawLine(x2Start, y2Start, x2Finish, y2Finish, thickness, layer, colour);
			}

			/// Draws 10th row.
			for (int i = iSpriteColumns * 9; i < iSpriteColumns * 10; i++)
			{
				float x1Start = backgroundPositions[i].xPos + 4;
				float y1Start = backgroundPositions[i].yPos + 4;
				float x1Finish = backgroundPositions[i].xPos + iSpriteWidth - 4;
				float y1Finish = backgroundPositions[i].yPos + iSpriteHeight - 4;

				float x2Start = backgroundPositions[i].xPos + 4;
				float y2Start = backgroundPositions[i].yPos + iSpriteHeight - 4;
				float x2Finish = backgroundPositions[i].xPos + iSpriteWidth - 4;
				float y2Finish = backgroundPositions[i].yPos + 4;

				float thickness = 1;
				unsigned int layer = 0;
				UG::SColour colour = UG::SColour(25, 25, 112, 255);

				UG::DrawLine(x1Start, y1Start, x1Finish, y1Finish, thickness, layer, colour);
				UG::DrawLine(x2Start, y2Start, x2Finish, y2Finish, thickness, layer, colour);
			}

			/// Draws 11th row.
			for (int i = iSpriteColumns * 10; i < iSpriteColumns * 11; i++)
			{
				float x1Start = backgroundPositions[i].xPos + 4;
				float y1Start = backgroundPositions[i].yPos + 4;
				float x1Finish = backgroundPositions[i].xPos + iSpriteWidth - 4;
				float y1Finish = backgroundPositions[i].yPos + iSpriteHeight - 4;

				float x2Start = backgroundPositions[i].xPos + 4;
				float y2Start = backgroundPositions[i].yPos + iSpriteHeight - 4;
				float x2Finish = backgroundPositions[i].xPos + iSpriteWidth - 4;
				float y2Finish = backgroundPositions[i].yPos + 4;

				float thickness = 1;
				unsigned int layer = 0;
				UG::SColour colour = UG::SColour(25, 25, 112, 255);

				UG::DrawLine(x1Start, y1Start, x1Finish, y1Finish, thickness, layer, colour);
				UG::DrawLine(x2Start, y2Start, x2Finish, y2Finish, thickness, layer, colour);
			}

			/// Draws 12th row.
			for (int i = iSpriteColumns * 11; i < iSpriteColumns * 12; i++)
			{
				float x1Start = backgroundPositions[i].xPos + 4;
				float y1Start = backgroundPositions[i].yPos + 4;
				float x1Finish = backgroundPositions[i].xPos + iSpriteWidth - 4;
				float y1Finish = backgroundPositions[i].yPos + iSpriteHeight - 4;

				float x2Start = backgroundPositions[i].xPos + 4;
				float y2Start = backgroundPositions[i].yPos + iSpriteHeight - 4;
				float x2Finish = backgroundPositions[i].xPos + iSpriteWidth - 4;
				float y2Finish = backgroundPositions[i].yPos + 4;

				float thickness = 1;
				unsigned int layer = 0;
				UG::SColour colour = UG::SColour(25, 25, 112, 255);

				UG::DrawLine(x1Start, y1Start, x1Finish, y1Finish, thickness, layer, colour);
				UG::DrawLine(x2Start, y2Start, x2Finish, y2Finish, thickness, layer, colour);
			}

			/// Draws 13th row.
			for (int i = iSpriteColumns * 12; i < iSpriteColumns * 13; i++)
			{
				float x1Start = backgroundPositions[i].xPos + 4;
				float y1Start = backgroundPositions[i].yPos + 4;
				float x1Finish = backgroundPositions[i].xPos + iSpriteWidth - 4;
				float y1Finish = backgroundPositions[i].yPos + iSpriteHeight - 4;

				float x2Start = backgroundPositions[i].xPos + 4;
				float y2Start = backgroundPositions[i].yPos + iSpriteHeight - 4;
				float x2Finish = backgroundPositions[i].xPos + iSpriteWidth - 4;
				float y2Finish = backgroundPositions[i].yPos + 4;

				float thickness = 1;
				unsigned int layer = 0;
				UG::SColour colour = UG::SColour(25, 25, 112, 255);

				UG::DrawLine(x1Start, y1Start, x1Finish, y1Finish, thickness, layer, colour);
				UG::DrawLine(x2Start, y2Start, x2Finish, y2Finish, thickness, layer, colour);
			}

			/// Draws 14th row.
			for (int i = iSpriteColumns * 13; i < iSpriteColumns * 14; i++)
			{
				if ((i - iSpriteColumns * 13 - 1) % 3 != 0)
				{
					float x1Start = backgroundPositions[i].xPos + 4;
					float y1Start = backgroundPositions[i].yPos + 4;
					float x1Finish = backgroundPositions[i].xPos + iSpriteWidth / 2;
					float y1Finish = backgroundPositions[i].yPos + iSpriteHeight / 2;

					float x2Start = backgroundPositions[i].xPos + 4;
					float y2Start = backgroundPositions[i].yPos + iSpriteHeight - 4;
					float x2Finish = backgroundPositions[i].xPos + iSpriteWidth / 2;
					float y2Finish = backgroundPositions[i].yPos + iSpriteHeight / 2;

					float thickness = 1;
					unsigned int layer = 0;
					UG::SColour colour = UG::SColour(124, 252, 0, 255);

					UG::DrawLine(x1Start, y1Start, x1Finish, y1Finish, thickness, layer, colour);
					UG::DrawLine(x2Start, y2Start, x2Finish, y2Finish, thickness, layer, colour);
				}
				if ((i - iSpriteColumns * 13) % 3 != 0)
				{
					float x1Start = backgroundPositions[i].xPos + iSpriteWidth / 2;
					float y1Start = backgroundPositions[i].yPos + iSpriteHeight / 2;
					float x1Finish = backgroundPositions[i].xPos + iSpriteWidth - 4;
					float y1Finish = backgroundPositions[i].yPos + iSpriteHeight - 4;

					float x2Start = backgroundPositions[i].xPos + iSpriteWidth / 2;
					float y2Start = backgroundPositions[i].yPos + iSpriteHeight / 2;
					float x2Finish = backgroundPositions[i].xPos + iSpriteWidth - 4;
					float y2Finish = backgroundPositions[i].yPos + 4;

					float thickness = 1;
					unsigned int layer = 0;
					UG::SColour colour = UG::SColour(124, 252, 0, 255);

					UG::DrawLine(x1Start, y1Start, x1Finish, y1Finish, thickness, layer, colour);
					UG::DrawLine(x2Start, y2Start, x2Finish, y2Finish, thickness, layer, colour);
				}
				if ((i - iSpriteColumns * 13 - 1) % 3 == 0)
				{
					UG::DrawLine(backgroundPositions[i].xPos - 16, backgroundPositions[i].yPos + iSpriteHeight / 2 - 16, backgroundPositions[i].xPos + 16, backgroundPositions[i].yPos + iSpriteHeight / 2 - 16, 2.0f, 0, UG::SColour(0, 100, 0, 255));
					UG::DrawLine(backgroundPositions[i].xPos - 16, backgroundPositions[i].yPos + iSpriteHeight / 2 + 16, backgroundPositions[i].xPos + 16, backgroundPositions[i].yPos + iSpriteHeight / 2 + 16, 2.0f, 0, UG::SColour(0, 100, 0, 255));
					UG::DrawLine(backgroundPositions[i].xPos - 16, backgroundPositions[i].yPos + iSpriteHeight / 2 - 16, backgroundPositions[i].xPos - 16, backgroundPositions[i].yPos + iSpriteHeight / 2 + 16, 2.0f, 0, UG::SColour(0, 100, 0, 255));
					UG::DrawLine(backgroundPositions[i].xPos + 16, backgroundPositions[i].yPos + iSpriteHeight / 2 - 16, backgroundPositions[i].xPos + 16, backgroundPositions[i].yPos + iSpriteHeight / 2 + 16, 2.0f, 0, UG::SColour(0, 100, 0, 255));
				}
			}

			/*/// Draws 15th row.
			for (int i = iSpriteColumns * 14; i < iSpriteColumns * 15; i++)
			{
			float x1Start = backgroundPositions[i].xPos + 4;
			float y1Start = backgroundPositions[i].yPos + 4;
			float x1Finish = backgroundPositions[i].xPos + iSpriteWidth - 4;
			float y1Finish = backgroundPositions[i].yPos + iSpriteHeight - 4;

			float x2Start = backgroundPositions[i].xPos + 4;
			float y2Start = backgroundPositions[i].yPos + iSpriteHeight - 4;
			float x2Finish = backgroundPositions[i].xPos + iSpriteWidth - 4;
			float y2Finish = backgroundPositions[i].yPos + 4;

			float thickness = 1;
			unsigned int layer = 0;
			UG::SColour colour = UG::SColour(0, 0, 0, 255);

			UG::DrawLine(x1Start, y1Start, x1Finish, y1Finish, thickness, layer, colour);
			UG::DrawLine(x2Start, y2Start, x2Finish, y2Finish, thickness, layer, colour);
			}

			/// Draws 16th (top) row.
			for (int i = iSpriteColumns * 15; i < iSpriteColumns * 16; i++)
			{
			float x1Start = backgroundPositions[i].xPos + 4;
			float y1Start = backgroundPositions[i].yPos + 4;
			float x1Finish = backgroundPositions[i].xPos + iSpriteWidth - 4;
			float y1Finish = backgroundPositions[i].yPos + iSpriteHeight - 4;

			float x2Start = backgroundPositions[i].xPos + 4;
			float y2Start = backgroundPositions[i].yPos + iSpriteHeight - 4;
			float x2Finish = backgroundPositions[i].xPos + iSpriteWidth - 4;
			float y2Finish = backgroundPositions[i].yPos + 4;

			float thickness = 1;
			unsigned int layer = 0;
			UG::SColour colour = UG::SColour(0, 0, 0, 255);

			UG::DrawLine(x1Start, y1Start, x1Finish, y1Finish, thickness, layer, colour);
			UG::DrawLine(x2Start, y2Start, x2Finish, y2Finish, thickness, layer, colour);
			}*/
#pragma endregion

#pragma region Call ScoreGain()
			// Calls the fn ScoreGain for 1 second to display the difference in score.
			if (fScoreTimer < 1)
			{
				ScoreGain();
			}
#pragma endregion

#pragma region Draw timer and label
			// Draws timer.

			if (fGameTimer >= 0)
			{
				if (fDeathTimer >= 1)
				{
					fGameTimer -= UG::GetDeltaTime();
				}
				float xStart = iScreenWidth / 2 + (1 - fGameTimer / 30) * (5 * iSpriteWidth);
				float yStart = iSpriteHeight / 2;
				float xFinish = iScreenWidth - 2 * iSpriteWidth;
				float yFinish = iSpriteHeight / 2;
				float thickness = 24.0f;
				unsigned int layer = 0;
				UG::SColour colour = UG::SColour((char)((1 - fGameTimer / 30) * 255), (char)(255 * (fGameTimer / 30)), 0, 255);

				UG::DrawLine(xStart, yStart, xFinish, yFinish, thickness, layer, colour);
			}
			else if (fDeathTimer >= 1)
			{
				bIsFrogDead = true;
				fDeathTimer = 0;
			}

			// Draws timer label.
			DrawCharacter('t', iScreenWidth - 1.75 * iSpriteWidth + 6, iSpriteHeight / 2, (char)255, (char)255, 0);
			DrawCharacter('i', iScreenWidth - 1.25 * iSpriteWidth + 2, iSpriteHeight / 2, (char)255, (char)255, 0);
			DrawCharacter('m', iScreenWidth - .75 * iSpriteWidth - 2, iSpriteHeight / 2, (char)255, (char)255, 0);
			DrawCharacter('e', iScreenWidth - .25 * iSpriteWidth - 6, iSpriteHeight / 2, (char)255, (char)255, 0);
#pragma endregion

#pragma region Draw time taken and label
			// Draws time taken and label after each successful run.
			if (fTimeDisplayTimer < 2)
			{
				DrawCharacter('t', iScreenWidth / 2 - .75 * iSpriteWidth, iScreenHeight - iSpriteHeight / 2, (char)(255 - trunc(fTimeDisplayTimer / 1.5) * (fTimeDisplayTimer - 1.5) * 255 * 2), (char)(255 - trunc(fTimeDisplayTimer / 1.5) * (fTimeDisplayTimer - 1.5) * 255 * 2), (char)(255 - trunc(fTimeDisplayTimer / 1.5) * (fTimeDisplayTimer - 1.5) * 255 * 2), (char)255);
				DrawCharacter('i', iScreenWidth / 2 - .25 * iSpriteWidth, iScreenHeight - iSpriteHeight / 2, (char)(255 - trunc(fTimeDisplayTimer / 1.5) * (fTimeDisplayTimer - 1.5) * 255 * 2), (char)(255 - trunc(fTimeDisplayTimer / 1.5) * (fTimeDisplayTimer - 1.5) * 255 * 2), (char)(255 - trunc(fTimeDisplayTimer / 1.5) * (fTimeDisplayTimer - 1.5) * 255 * 2), (char)255);
				DrawCharacter('m', iScreenWidth / 2 + .25 * iSpriteWidth, iScreenHeight - iSpriteHeight / 2, (char)(255 - trunc(fTimeDisplayTimer / 1.5) * (fTimeDisplayTimer - 1.5) * 255 * 2), (char)(255 - trunc(fTimeDisplayTimer / 1.5) * (fTimeDisplayTimer - 1.5) * 255 * 2), (char)(255 - trunc(fTimeDisplayTimer / 1.5) * (fTimeDisplayTimer - 1.5) * 255 * 2), (char)255);
				DrawCharacter('e', iScreenWidth / 2 + .75 * iSpriteWidth, iScreenHeight - iSpriteHeight / 2, (char)(255 - trunc(fTimeDisplayTimer / 1.5) * (fTimeDisplayTimer - 1.5) * 255 * 2), (char)(255 - trunc(fTimeDisplayTimer / 1.5) * (fTimeDisplayTimer - 1.5) * 255 * 2), (char)(255 - trunc(fTimeDisplayTimer / 1.5) * (fTimeDisplayTimer - 1.5) * 255 * 2), (char)255);
				DrawNumber((int)trunc((30 - iTimeTaken) / 10) % 10, iScreenWidth / 2 - .75 * iSpriteWidth, iScreenHeight - iSpriteHeight, (char)(255 - trunc(fTimeDisplayTimer / 1.5) * (fTimeDisplayTimer - 1.5) * 255 * 2), (char)(255 - trunc(fTimeDisplayTimer / 1.5) * (fTimeDisplayTimer - 1.5) * 255 * 2), (char)(255 - trunc(fTimeDisplayTimer / 1.5) * (fTimeDisplayTimer - 1.5) * 255 * 2), (char)255);
				DrawNumber((int)(30 - iTimeTaken) % 10, iScreenWidth / 2 - .25 * iSpriteWidth, iScreenHeight - iSpriteHeight, (char)(255 - trunc(fTimeDisplayTimer / 1.5) * (fTimeDisplayTimer - 1.5) * 255 * 2), (char)(255 - trunc(fTimeDisplayTimer / 1.5) * (fTimeDisplayTimer - 1.5) * 255 * 2), (char)(255 - trunc(fTimeDisplayTimer / 1.5) * (fTimeDisplayTimer - 1.5) * 255 * 2), (char)255);
				DrawCharacter('s', iScreenWidth / 2 + .75 * iSpriteWidth, iScreenHeight - iSpriteHeight, (char)(255 - trunc(fTimeDisplayTimer / 1.5) * (fTimeDisplayTimer - 1.5) * 255 * 2), (char)(255 - trunc(fTimeDisplayTimer / 1.5) * (fTimeDisplayTimer - 1.5) * 255 * 2), (char)(255 - trunc(fTimeDisplayTimer / 1.5) * (fTimeDisplayTimer - 1.5) * 255 * 2), (char)255);

				fTimeDisplayTimer += UG::GetDeltaTime();
			}
#pragma endregion

#pragma region Draw Lives
			// Draws lives.
			for (int i = 1; i <= iCurrentLives2; i++)
			{
				// The trunc parts wrap the lives round to a new row if they exceed 14.
				UG::DrawLine(((2 * i - 1) - 28 * float((trunc((i - 1) / 14)))) * iSpriteWidth / 4 - 8, (3 - 2 * float((trunc((i - 1) / 14)))) * iSpriteHeight / 4 - 8, ((2 * i - 1) - 28 * float((trunc((i - 1) / 14)))) * iSpriteWidth / 4 + 8, (3 - 2 * float((trunc((i - 1) / 14)))) * iSpriteHeight / 4 - 8, 2.0f, 0, UG::SColour((unsigned char)173, (unsigned char)255, (unsigned char)47, (unsigned char)255));
				UG::DrawLine(((2 * i - 1) - 28 * float((trunc((i - 1) / 14)))) * iSpriteWidth / 4 - 8, (3 - 2 * float((trunc((i - 1) / 14)))) * iSpriteHeight / 4 + 8, ((2 * i - 1) - 28 * float((trunc((i - 1) / 14)))) * iSpriteWidth / 4 + 8, (3 - 2 * float((trunc((i - 1) / 14)))) * iSpriteHeight / 4 + 8, 2.0f, 0, UG::SColour((unsigned char)173, (unsigned char)255, (unsigned char)47, (unsigned char)255));
				UG::DrawLine(((2 * i - 1) - 28 * float((trunc((i - 1) / 14)))) * iSpriteWidth / 4 - 8, (3 - 2 * float((trunc((i - 1) / 14)))) * iSpriteHeight / 4 - 8, ((2 * i - 1) - 28 * float((trunc((i - 1) / 14)))) * iSpriteWidth / 4 - 8, (3 - 2 * float((trunc((i - 1) / 14)))) * iSpriteHeight / 4 + 8, 2.0f, 0, UG::SColour((unsigned char)173, (unsigned char)255, (unsigned char)47, (unsigned char)255));
				UG::DrawLine(((2 * i - 1) - 28 * float((trunc((i - 1) / 14)))) * iSpriteWidth / 4 + 8, (3 - 2 * float((trunc((i - 1) / 14)))) * iSpriteHeight / 4 - 8, ((2 * i - 1) - 28 * float((trunc((i - 1) / 14)))) * iSpriteWidth / 4 + 8, (3 - 2 * float((trunc((i - 1) / 14)))) * iSpriteHeight / 4 + 8, 2.0f, 0, UG::SColour((unsigned char)173, (unsigned char)255, (unsigned char)47, (unsigned char)255));
			}
#pragma endregion

#pragma region Draw score and label
			// Draws score.
			DrawNumber((int)trunc(iPlayerScore / 10000) % 10, 2.5 * iSpriteWidth, iScreenHeight - 2 * iSpriteHeight / 2, (char)220, (char)20, (char)20);
			DrawNumber((int)trunc(iPlayerScore / 1000) % 10, 3 * iSpriteWidth, iScreenHeight - 2 * iSpriteHeight / 2, (char)220, (char)20, (char)20);
			DrawNumber((int)trunc(iPlayerScore / 100) % 10, 3.5 * iSpriteWidth, iScreenHeight - 2 * iSpriteHeight / 2, (char)220, (char)20, (char)20);
			DrawNumber((int)trunc(iPlayerScore / 10) % 10, 4 * iSpriteWidth, iScreenHeight - 2 * iSpriteHeight / 2, (char)220, (char)20, (char)20);
			DrawNumber(iPlayerScore % 10, 4.5 * iSpriteWidth, iScreenHeight - 2 * iSpriteHeight / 2, (char)220, (char)20, (char)20);
			// Draws score label.
			DrawCharacter('s', 2.5 * iSpriteWidth, iScreenHeight - iSpriteHeight / 2, (char)220, (char)220, (char)220);
			DrawCharacter('c', 3 * iSpriteWidth, iScreenHeight - iSpriteHeight / 2, (char)220, (char)220, (char)220);
			DrawCharacter('o', 3.5 * iSpriteWidth, iScreenHeight - iSpriteHeight / 2, (char)220, (char)220, (char)220);
			DrawCharacter('r', 4 * iSpriteWidth, iScreenHeight - iSpriteHeight / 2, (char)220, (char)220, (char)220);
			DrawCharacter('e', 4.5 * iSpriteWidth, iScreenHeight - iSpriteHeight / 2, (char)220, (char)220, (char)220);
#pragma endregion

#pragma region Define user controls


			// Changes player location upon user input
			if (fMovementTimer >= .25f && fDeathTimer >= 1.0f)
			{
				if ((UG::WasKeyPressed(UG::KEY_UP) || UG::WasKeyPressed(UG::KEY_W)) && playerPosition.yPos < iScreenHeight - iSpriteHeight)
				{
					playerPosition.yPos += iSpriteHeight;
					fMovementTimer = 0;
					fMoveUpTimer = 0;
				}
				if ((UG::WasKeyPressed(UG::KEY_DOWN) || UG::WasKeyPressed(UG::KEY_S)) && playerPosition.yPos > 2 * iSpriteHeight)
				{
					playerPosition.yPos -= iSpriteHeight;
					fMovementTimer = 0;
					fMoveDownTimer = 0;
				}
				if ((UG::WasKeyPressed(UG::KEY_LEFT) || UG::WasKeyPressed(UG::KEY_A)) && playerPosition.xPos > iSpriteWidth)
				{
					playerPosition.xPos -= iSpriteWidth;
					fMovementTimer = 0;
					fMoveLeftTimer = 0;
				}
				if ((UG::WasKeyPressed(UG::KEY_RIGHT) || UG::WasKeyPressed(UG::KEY_D)) && playerPosition.xPos < iScreenWidth - iSpriteWidth)
				{
					playerPosition.xPos += iSpriteWidth;
					fMovementTimer = 0;
					fMoveRightTimer = 0;
				}
			}
			// Controls the movement timers.
			if (fMovementTimer < .25)
			{
				fMovementTimer += UG::GetDeltaTime();
			}
			if (fMoveUpTimer < .25)
			{
				fMoveUpTimer += UG::GetDeltaTime();
			}
			if (fMoveDownTimer < .25)
			{
				fMoveDownTimer += UG::GetDeltaTime();
			}
			if (fMoveLeftTimer < .25)
			{
				fMoveLeftTimer += UG::GetDeltaTime();
			}
			if (fMoveRightTimer < .25)
			{
				fMoveRightTimer += UG::GetDeltaTime();
			}

#pragma endregion

#pragma region Give score upon reaching new row
			// Grants score upon reaching a new row.
			if (!bHasReachedRow1 && playerPosition.yPos > 2 * iSpriteHeight) //score only gained if player reaches a row and hasn't reached it before
			{
				iPlayerScore += 10;
				bHasReachedRow1 = true;
			}
			if (!bHasReachedRow2 && playerPosition.yPos > 3 * iSpriteHeight)
			{
				iPlayerScore += 10;
				bHasReachedRow2 = true;
			}
			if (!bHasReachedRow3 && playerPosition.yPos > 4 * iSpriteHeight)
			{
				iPlayerScore += 10;
				bHasReachedRow3 = true;
			}
			if (!bHasReachedRow4 && playerPosition.yPos > 5 * iSpriteHeight)
			{
				iPlayerScore += 10;
				bHasReachedRow4 = true;
			}
			if (!bHasReachedRow5 && playerPosition.yPos > 6 * iSpriteHeight)
			{
				iPlayerScore += 10;
				bHasReachedRow5 = true;
			}
			if (!bHasReachedRow6 && playerPosition.yPos > 7 * iSpriteHeight)
			{
				iPlayerScore += 10;
				bHasReachedRow6 = true;
			}
			if (!bHasReachedRow7 && playerPosition.yPos > 8 * iSpriteHeight)
			{
				iPlayerScore += 10;
				bHasReachedRow7 = true;
			}
			if (!bHasReachedRow8 && playerPosition.yPos > 9 * iSpriteHeight)
			{
				iPlayerScore += 10;
				bHasReachedRow8 = true;
			}
			if (!bHasReachedRow9 && playerPosition.yPos > 10 * iSpriteHeight)
			{
				iPlayerScore += 10;
				bHasReachedRow9 = true;
			}
			if (!bHasReachedRow10 && playerPosition.yPos > 11 * iSpriteHeight)
			{
				iPlayerScore += 10;
				bHasReachedRow10 = true;
			}
			if (!bHasReachedRow11 && playerPosition.yPos > 12 * iSpriteHeight)
			{
				iPlayerScore += 10;
				bHasReachedRow11 = true;
			}
#pragma endregion

#pragma region Draw cars and update their positions
			// Draws cars.
			/// First row.
			for (int i = 0; i < 3; i++)
			{
				// Moves cars in a direction determined by their row.
				if ((int)(carPositions2[i].yPos / iSpriteHeight - .5) % 2 == 0)
				{
					carPositions2[i].xPos -= 1;
				}
				else
				{
					carPositions2[i].xPos += 1;
				}

				// Wraps cars around screen in both directions.
				if (carPositions2[i].xPos > iScreenWidth + iSpriteWidth || carPositions2[i].xPos < -iSpriteWidth)
				{
					carPositions2[i].xPos -= carPositions2[i].xPos / abs(carPositions2[i].xPos)*(iScreenWidth + 2 * iSpriteWidth);
				}

				UG::DrawLine(carPositions2[i].xPos - 16, carPositions2[i].yPos - 16, carPositions2[i].xPos + 16, carPositions2[i].yPos - 16, 2.0f, 0, UG::SColour(255, 255, 0, 255));
				UG::DrawLine(carPositions2[i].xPos - 16, carPositions2[i].yPos + 16, carPositions2[i].xPos + 16, carPositions2[i].yPos + 16, 2.0f, 0, UG::SColour(255, 255, 0, 255));
				UG::DrawLine(carPositions2[i].xPos - 16, carPositions2[i].yPos - 16, carPositions2[i].xPos - 16, carPositions2[i].yPos + 16, 2.0f, 0, UG::SColour(255, 255, 0, 255));
				UG::DrawLine(carPositions2[i].xPos + 16, carPositions2[i].yPos - 16, carPositions2[i].xPos + 16, carPositions2[i].yPos + 16, 2.0f, 0, UG::SColour(255, 255, 0, 255));

				// Kills player if hit by a car.
				if (playerPosition.xPos >= carPositions2[i].xPos - 30 && playerPosition.xPos <= carPositions2[i].xPos + 30 && playerPosition.yPos == carPositions2[i].yPos && fDeathTimer >= 1)
				{
					bIsFrogDead = true;
					bHitByCar = true;
					fDeathTimer = 0;
				}
			}
			/// Second Row.
			for (int i = 3; i < 6; i++)
			{
				// Moves cars in a direction determined by their row.
				if ((int)(carPositions2[i].yPos / iSpriteHeight - .5) % 2 == 0)
				{
					carPositions2[i].xPos -= 1;
				}
				else
				{
					carPositions2[i].xPos += 1;
				}

				// Wraps cars around screen in both directions.
				if (carPositions2[i].xPos > iScreenWidth + iSpriteWidth || carPositions2[i].xPos < -iSpriteWidth)
				{
					carPositions2[i].xPos -= carPositions2[i].xPos / abs(carPositions2[i].xPos)*(iScreenWidth + 2 * iSpriteWidth);
				}

				UG::DrawLine(carPositions2[i].xPos - 16, carPositions2[i].yPos - 16, carPositions2[i].xPos + 16, carPositions2[i].yPos - 16, 2.0f, 0, UG::SColour(0, 255, 0, 255));
				UG::DrawLine(carPositions2[i].xPos - 16, carPositions2[i].yPos + 16, carPositions2[i].xPos + 16, carPositions2[i].yPos + 16, 2.0f, 0, UG::SColour(0, 255, 0, 255));
				UG::DrawLine(carPositions2[i].xPos - 16, carPositions2[i].yPos - 16, carPositions2[i].xPos - 16, carPositions2[i].yPos + 16, 2.0f, 0, UG::SColour(0, 255, 0, 255));
				UG::DrawLine(carPositions2[i].xPos + 16, carPositions2[i].yPos - 16, carPositions2[i].xPos + 16, carPositions2[i].yPos + 16, 2.0f, 0, UG::SColour(0, 255, 0, 255));

				if (playerPosition.xPos >= carPositions2[i].xPos - 30 && playerPosition.xPos <= carPositions2[i].xPos + 30 && playerPosition.yPos == carPositions2[i].yPos && fDeathTimer >= 1)
				{
					bIsFrogDead = true;
					bHitByCar = true;
					fDeathTimer = 0;
				}
			}
			/// Third row.
			for (int i = 6; i < 9; i++)
			{
				// Moves cars in a direction determined by their row.
				if ((int)(carPositions2[i].yPos / iSpriteHeight - .5) % 2 == 0)
				{
					carPositions2[i].xPos -= 2;
				}
				else
				{
					carPositions2[i].xPos += 2;
				}

				// Wraps cars around screen in both directions.
				if (carPositions2[i].xPos > iScreenWidth + iSpriteWidth || carPositions2[i].xPos < -iSpriteWidth)
				{
					carPositions2[i].xPos -= carPositions2[i].xPos / abs(carPositions2[i].xPos)*(iScreenWidth + 2 * iSpriteWidth);
				}

				UG::DrawLine(carPositions2[i].xPos - 16, carPositions2[i].yPos - 16, carPositions2[i].xPos + 16, carPositions2[i].yPos - 16, 2.0f, 0, UG::SColour(255, 0, 255, 255));
				UG::DrawLine(carPositions2[i].xPos - 16, carPositions2[i].yPos + 16, carPositions2[i].xPos + 16, carPositions2[i].yPos + 16, 2.0f, 0, UG::SColour(255, 0, 255, 255));
				UG::DrawLine(carPositions2[i].xPos - 16, carPositions2[i].yPos - 16, carPositions2[i].xPos - 16, carPositions2[i].yPos + 16, 2.0f, 0, UG::SColour(255, 0, 255, 255));
				UG::DrawLine(carPositions2[i].xPos + 16, carPositions2[i].yPos - 16, carPositions2[i].xPos + 16, carPositions2[i].yPos + 16, 2.0f, 0, UG::SColour(255, 0, 255, 255));

				if (playerPosition.xPos >= carPositions2[i].xPos - 30 && playerPosition.xPos <= carPositions2[i].xPos + 30 && playerPosition.yPos == carPositions2[i].yPos && fDeathTimer >= 1)
				{
					bIsFrogDead = true;
					bHitByCar = true;
					fDeathTimer = 0;
				}
			}
			/// Fourth row.
			for (int i = 9; i < 10; i++)
			{
				// Moves cars in a direction determined by their row.
				if ((int)(carPositions2[i].yPos / iSpriteHeight - .5) % 2 == 0)
				{
					carPositions2[i].xPos -= 4;
				}
				else
				{
					carPositions2[i].xPos += 4;
				}

				// Wraps cars around screen in both directions.
				if (carPositions2[i].xPos > iScreenWidth + iSpriteWidth || carPositions2[i].xPos < -iSpriteWidth)
				{
					carPositions2[i].xPos -= carPositions2[i].xPos / abs(carPositions2[i].xPos)*(iScreenWidth + 2 * iSpriteWidth);
				}

				UG::DrawLine(carPositions2[i].xPos - 16, carPositions2[i].yPos - 16, carPositions2[i].xPos + 16, carPositions2[i].yPos - 16, 2.0f, 0, UG::SColour(255, 0, 0, 255));
				UG::DrawLine(carPositions2[i].xPos - 16, carPositions2[i].yPos + 16, carPositions2[i].xPos + 16, carPositions2[i].yPos + 16, 2.0f, 0, UG::SColour(255, 0, 0, 255));
				UG::DrawLine(carPositions2[i].xPos - 16, carPositions2[i].yPos - 16, carPositions2[i].xPos - 16, carPositions2[i].yPos + 16, 2.0f, 0, UG::SColour(255, 0, 0, 255));
				UG::DrawLine(carPositions2[i].xPos + 16, carPositions2[i].yPos - 16, carPositions2[i].xPos + 16, carPositions2[i].yPos + 16, 2.0f, 0, UG::SColour(255, 0, 0, 255));

				if (playerPosition.xPos >= carPositions2[i].xPos - 30 && playerPosition.xPos <= carPositions2[i].xPos + 30 && playerPosition.yPos == carPositions2[i].yPos && fDeathTimer >= 1)
				{
					bIsFrogDead = true;
					bHitByCar = true;
					fDeathTimer = 0;
				}
			}
			/// Fifth row.
			for (int i = 10; i < 14; i++)
			{
				// Moves cars in a direction determined by their row.
				if ((int)(carPositions2[i].yPos / iSpriteHeight - .5) % 2 == 0)
				{
					carPositions2[i].xPos -= 2;
				}
				else
				{
					carPositions2[i].xPos += 2;
				}

				// Wraps cars around screen in both directions.
				if (carPositions2[i].xPos > iScreenWidth + iSpriteWidth || carPositions2[i].xPos < -iSpriteWidth)
				{
					carPositions2[i].xPos -= carPositions2[i].xPos / abs(carPositions2[i].xPos)*(iScreenWidth + 2 * iSpriteWidth);
				}

				UG::DrawLine(carPositions2[i].xPos - 16, carPositions2[i].yPos - 16, carPositions2[i].xPos + 16, carPositions2[i].yPos - 16, 2.0f, 0, UG::SColour(255, 255, 255, 255));
				UG::DrawLine(carPositions2[i].xPos - 16, carPositions2[i].yPos + 16, carPositions2[i].xPos + 16, carPositions2[i].yPos + 16, 2.0f, 0, UG::SColour(255, 255, 255, 255));
				UG::DrawLine(carPositions2[i].xPos - 16, carPositions2[i].yPos - 16, carPositions2[i].xPos - 16, carPositions2[i].yPos + 16, 2.0f, 0, UG::SColour(255, 255, 255, 255));
				UG::DrawLine(carPositions2[i].xPos + 16, carPositions2[i].yPos - 16, carPositions2[i].xPos + 16, carPositions2[i].yPos + 16, 2.0f, 0, UG::SColour(255, 255, 255, 255));

				if (playerPosition.xPos >= carPositions2[i].xPos - 30 && playerPosition.xPos <= carPositions2[i].xPos + 30 && playerPosition.yPos == carPositions2[i].yPos && fDeathTimer >= 1)
				{
					bIsFrogDead = true;
					bHitByCar = true;
					fDeathTimer = 0;
				}
			}
#pragma endregion

#pragma region Draw logs/turtles and update their positions
			// Draws logs/turtles.
			/// First row.
			for (int i = 0; i < 15; i++)
			{
				// Set turtle direction.
				logturtlePositions2[i].xPos -= 1;

				// Wraps cars around screen in both directions.
				if (logturtlePositions2[i].xPos > iScreenWidth + 4 * iSpriteWidth || logturtlePositions2[i].xPos < 4 * -iSpriteWidth)
				{
					logturtlePositions2[i].xPos -= logturtlePositions2[i].xPos / abs(logturtlePositions2[i].xPos)*(iScreenWidth + 8 * iSpriteWidth);
				}

				UG::DrawLine(logturtlePositions2[i].xPos - 16, logturtlePositions2[i].yPos - 16, logturtlePositions2[i].xPos + 16, logturtlePositions2[i].yPos - 16, 2.0f, 0, UG::SColour(175, 0, 0, 255));
				UG::DrawLine(logturtlePositions2[i].xPos - 16, logturtlePositions2[i].yPos + 16, logturtlePositions2[i].xPos + 16, logturtlePositions2[i].yPos + 16, 2.0f, 0, UG::SColour(175, 0, 0, 255));
				UG::DrawLine(logturtlePositions2[i].xPos - 16, logturtlePositions2[i].yPos - 16, logturtlePositions2[i].xPos - 16, logturtlePositions2[i].yPos + 16, 2.0f, 0, UG::SColour(175, 0, 0, 255));
				UG::DrawLine(logturtlePositions2[i].xPos + 16, logturtlePositions2[i].yPos - 16, logturtlePositions2[i].xPos + 16, logturtlePositions2[i].yPos + 16, 2.0f, 0, UG::SColour(175, 0, 0, 255));
			}
			/// Second row.
			for (int i = 15; i < 24; i++)
			{
				// Set turtle direction.
				logturtlePositions2[i].xPos += 1;

				// Wraps cars around screen in both directions.
				if (logturtlePositions2[i].xPos > iScreenWidth + 4 * iSpriteWidth || logturtlePositions2[i].xPos < 4 * -iSpriteWidth)
				{
					logturtlePositions2[i].xPos -= logturtlePositions2[i].xPos / abs(logturtlePositions2[i].xPos)*(iScreenWidth + 8 * iSpriteWidth);
				}

				UG::DrawLine(logturtlePositions2[i].xPos - 16, logturtlePositions2[i].yPos - 16, logturtlePositions2[i].xPos + 16, logturtlePositions2[i].yPos - 16, 2.0f, 0, UG::SColour(150, 90, 15, 255));
				UG::DrawLine(logturtlePositions2[i].xPos - 16, logturtlePositions2[i].yPos + 16, logturtlePositions2[i].xPos + 16, logturtlePositions2[i].yPos + 16, 2.0f, 0, UG::SColour(150, 90, 15, 255));
				UG::DrawLine(logturtlePositions2[i].xPos - 16, logturtlePositions2[i].yPos - 16, logturtlePositions2[i].xPos - 16, logturtlePositions2[i].yPos + 16, 2.0f, 0, UG::SColour(150, 90, 15, 255));
				UG::DrawLine(logturtlePositions2[i].xPos + 16, logturtlePositions2[i].yPos - 16, logturtlePositions2[i].xPos + 16, logturtlePositions2[i].yPos + 16, 2.0f, 0, UG::SColour(150, 90, 15, 255));
			}
			/// Third row.
			for (int i = 24; i < 42; i++)
			{
				// Set turtle direction.
				logturtlePositions2[i].xPos += 2;

				// Wraps cars around screen in both directions.
				if (logturtlePositions2[i].xPos > iScreenWidth + 7 * iSpriteWidth || logturtlePositions2[i].xPos < 7 * -iSpriteWidth)
				{
					logturtlePositions2[i].xPos -= logturtlePositions2[i].xPos / abs(logturtlePositions2[i].xPos)*(iScreenWidth + 14 * iSpriteWidth);
				}

				UG::DrawLine(logturtlePositions2[i].xPos - 16, logturtlePositions2[i].yPos - 16, logturtlePositions2[i].xPos + 16, logturtlePositions2[i].yPos - 16, 2.0f, 0, UG::SColour(150, 90, 15, 255));
				UG::DrawLine(logturtlePositions2[i].xPos - 16, logturtlePositions2[i].yPos + 16, logturtlePositions2[i].xPos + 16, logturtlePositions2[i].yPos + 16, 2.0f, 0, UG::SColour(150, 90, 15, 255));
				UG::DrawLine(logturtlePositions2[i].xPos - 16, logturtlePositions2[i].yPos - 16, logturtlePositions2[i].xPos - 16, logturtlePositions2[i].yPos + 16, 2.0f, 0, UG::SColour(150, 90, 15, 255));
				UG::DrawLine(logturtlePositions2[i].xPos + 16, logturtlePositions2[i].yPos - 16, logturtlePositions2[i].xPos + 16, logturtlePositions2[i].yPos + 16, 2.0f, 0, UG::SColour(150, 90, 15, 255));
			}
			/// Fourth row.
			for (int i = 42; i < 50; i++)
			{
				// Set turtle direction.
				logturtlePositions2[i].xPos -= 1;

				// Wraps cars around screen in both directions.
				if (logturtlePositions2[i].xPos > iScreenWidth + 3 * iSpriteWidth || logturtlePositions2[i].xPos < 4 * -iSpriteWidth)
				{
					logturtlePositions2[i].xPos -= logturtlePositions2[i].xPos / abs(logturtlePositions2[i].xPos)*(iScreenWidth + 7 * iSpriteWidth);
				}

				UG::DrawLine(logturtlePositions2[i].xPos - 16, logturtlePositions2[i].yPos - 16, logturtlePositions2[i].xPos + 16, logturtlePositions2[i].yPos - 16, 2.0f, 0, UG::SColour(175, 0, 0, 255));
				UG::DrawLine(logturtlePositions2[i].xPos - 16, logturtlePositions2[i].yPos + 16, logturtlePositions2[i].xPos + 16, logturtlePositions2[i].yPos + 16, 2.0f, 0, UG::SColour(175, 0, 0, 255));
				UG::DrawLine(logturtlePositions2[i].xPos - 16, logturtlePositions2[i].yPos - 16, logturtlePositions2[i].xPos - 16, logturtlePositions2[i].yPos + 16, 2.0f, 0, UG::SColour(175, 0, 0, 255));
				UG::DrawLine(logturtlePositions2[i].xPos + 16, logturtlePositions2[i].yPos - 16, logturtlePositions2[i].xPos + 16, logturtlePositions2[i].yPos + 16, 2.0f, 0, UG::SColour(175, 0, 0, 255));
			}
			/// Fifth row.
			for (int i = 50; i < 66; i++)
			{
				// Set turtle direction.
				logturtlePositions2[i].xPos += 1.5;

				// Wraps cars around screen in both directions.
				if (logturtlePositions2[i].xPos > iScreenWidth + 5 * iSpriteWidth || logturtlePositions2[i].xPos < 6 * -iSpriteWidth)
				{
					logturtlePositions2[i].xPos -= logturtlePositions2[i].xPos / abs(logturtlePositions2[i].xPos)*(iScreenWidth + 11 * iSpriteWidth);
				}

				UG::DrawLine(logturtlePositions2[i].xPos - 16, logturtlePositions2[i].yPos - 16, logturtlePositions2[i].xPos + 16, logturtlePositions2[i].yPos - 16, 2.0f, 0, UG::SColour(150, 90, 15, 255));
				UG::DrawLine(logturtlePositions2[i].xPos - 16, logturtlePositions2[i].yPos + 16, logturtlePositions2[i].xPos + 16, logturtlePositions2[i].yPos + 16, 2.0f, 0, UG::SColour(150, 90, 15, 255));
				UG::DrawLine(logturtlePositions2[i].xPos - 16, logturtlePositions2[i].yPos - 16, logturtlePositions2[i].xPos - 16, logturtlePositions2[i].yPos + 16, 2.0f, 0, UG::SColour(150, 90, 15, 255));
				UG::DrawLine(logturtlePositions2[i].xPos + 16, logturtlePositions2[i].yPos - 16, logturtlePositions2[i].xPos + 16, logturtlePositions2[i].yPos + 16, 2.0f, 0, UG::SColour(150, 90, 15, 255));
			}
#pragma endregion

#pragma region Move player if on a log/turtle
			// Moves player if player is on a log or a turtle.
			/// First row.
			if (fDeathTimer >= 1)
			{
				if (playerPosition.yPos == 8.5 * iSpriteHeight)
				{
					// Checks if Frog is on a log/turtle.
					if ((playerPosition.xPos >= logturtlePositions2[0].xPos - 16 && playerPosition.xPos <= logturtlePositions2[2].xPos + 16) ||
						(playerPosition.xPos >= logturtlePositions2[3].xPos - 16 && playerPosition.xPos <= logturtlePositions2[5].xPos + 16) ||
						(playerPosition.xPos >= logturtlePositions2[6].xPos - 16 && playerPosition.xPos <= logturtlePositions2[8].xPos + 16) ||
						(playerPosition.xPos >= logturtlePositions2[9].xPos - 16 && playerPosition.xPos <= logturtlePositions2[11].xPos + 16) ||
						(playerPosition.xPos >= logturtlePositions2[12].xPos - 16 && playerPosition.xPos <= logturtlePositions2[14].xPos + 16))
					{
						// Moves player by same amount as log/turtle.
						playerPosition.xPos -= 1;
					}
					else
					{
						bIsFrogDead = true;
						fDeathTimer = 0;
						bDrowned = true;
					}
				}
				/// Second row.
				else if (playerPosition.yPos == 9.5 * iSpriteHeight)
				{
					if ((playerPosition.xPos >= logturtlePositions2[15].xPos - 16 && playerPosition.xPos <= logturtlePositions2[17].xPos + 16) ||
						(playerPosition.xPos >= logturtlePositions2[18].xPos - 16 && playerPosition.xPos <= logturtlePositions2[20].xPos + 16) ||
						(playerPosition.xPos >= logturtlePositions2[21].xPos - 16 && playerPosition.xPos <= logturtlePositions2[23].xPos + 16))
					{
						playerPosition.xPos += 1;
					}
					else
					{
						bIsFrogDead = true;
						fDeathTimer = 0;
						bDrowned = true;
					}
				}
				/// Third row.
				else if (playerPosition.yPos == 10.5 * iSpriteHeight)
				{
					if ((playerPosition.xPos >= logturtlePositions2[24].xPos - 16 && playerPosition.xPos <= logturtlePositions2[29].xPos + 16) ||
						(playerPosition.xPos >= logturtlePositions2[30].xPos - 16 && playerPosition.xPos <= logturtlePositions2[35].xPos + 16) ||
						(playerPosition.xPos >= logturtlePositions2[36].xPos - 16 && playerPosition.xPos <= logturtlePositions2[41].xPos + 16))
					{
						playerPosition.xPos += 2;
					}
					else
					{
						bIsFrogDead = true;
						fDeathTimer = 0;
						bDrowned = true;
					}
				}
				/// Fourth row.
				else if (playerPosition.yPos == 11.5 * iSpriteHeight)
				{
					if ((playerPosition.xPos >= logturtlePositions2[42].xPos - 16 && playerPosition.xPos <= logturtlePositions2[43].xPos + 16) ||
						(playerPosition.xPos >= logturtlePositions2[44].xPos - 16 && playerPosition.xPos <= logturtlePositions2[45].xPos + 16) ||
						(playerPosition.xPos >= logturtlePositions2[46].xPos - 16 && playerPosition.xPos <= logturtlePositions2[47].xPos + 16) ||
						(playerPosition.xPos >= logturtlePositions2[48].xPos - 16 && playerPosition.xPos <= logturtlePositions2[49].xPos + 16))
					{
						playerPosition.xPos -= 1;
					}
					else
					{
						bIsFrogDead = true;
						fDeathTimer = 0;
						bDrowned = true;
					}
				}
				/// Fifth row.
				else if (playerPosition.yPos == 12.5 * iSpriteHeight)
				{
					if ((playerPosition.xPos >= logturtlePositions2[50].xPos - 16 && playerPosition.xPos <= logturtlePositions2[53].xPos + 16) ||
						(playerPosition.xPos >= logturtlePositions2[54].xPos - 16 && playerPosition.xPos <= logturtlePositions2[57].xPos + 16) ||
						(playerPosition.xPos >= logturtlePositions2[58].xPos - 16 && playerPosition.xPos <= logturtlePositions2[61].xPos + 16) ||
						(playerPosition.xPos >= logturtlePositions2[62].xPos - 16 && playerPosition.xPos <= logturtlePositions2[65].xPos + 16))
					{
						playerPosition.xPos += 1.5;
					}
					else
					{
						bIsFrogDead = true;
						fDeathTimer = 0;
						bDrowned = true;
					}
				}
			}
#pragma endregion

#pragma region Move player onto a sprite position upon exiting river also reset player position and row progress upon landing on a lilypad, add score, and make the lilypad 'used'.
#pragma region Below
			// Moves player onto a sprite position upon exiting the river.
			if (playerPosition.yPos == 7.5 * iSpriteHeight)
			{
				int offset = (int)playerPosition.xPos % iSpriteWidth;

				if (offset <= 32) //rounds the correction to the left or right
				{
					playerPosition.xPos -= offset;
				}
				else
				{
					playerPosition.xPos += iSpriteWidth - offset;
				}
			}
#pragma endregion

#pragma region Above
			else if (playerPosition.yPos == 13.5 * iSpriteHeight && fDeathTimer >= 1)
			{
				int offset = (int)playerPosition.xPos % iSpriteWidth;

				if (offset <= 32) //rounds the correction to the left or right
				{
					playerPosition.xPos -= offset;
				}
				else
				{
					playerPosition.xPos += iSpriteWidth - offset;
				}

				// Resets player position and row progress upon landing on a lilypad, adds score, and makes the lilypad 'used'.
				if (playerPosition.xPos >= iSpriteWidth - 1 && playerPosition.xPos <= iSpriteWidth + 1 && !bHasReachedLilyPad1)
				{
					playerPosition.xPos = iSpriteWidth * 7;
					playerPosition.yPos = iSpriteHeight * 1.5;
					iTimeTaken = (int)fGameTimer;
					bHasReachedLilyPad1 = true;
					bHasReachedRow1 = false;
					bHasReachedRow2 = false;
					bHasReachedRow3 = false;
					bHasReachedRow4 = false;
					bHasReachedRow5 = false;
					bHasReachedRow6 = false;
					bHasReachedRow7 = false;
					bHasReachedRow8 = false;
					bHasReachedRow9 = false;
					bHasReachedRow10 = false;
					bHasReachedRow11 = false;
					iPlayerScore += 50;
					iPlayerScore += (int)fGameTimer * 5;
					fTimeDisplayTimer = 0;
					fGameTimer = 30.0f;
				}
				else if (playerPosition.xPos >= 4 * iSpriteWidth - 1 && playerPosition.xPos <= 4 * iSpriteWidth + 1 && !bHasReachedLilyPad2)
				{
					playerPosition.xPos = iSpriteWidth * 7;
					playerPosition.yPos = iSpriteHeight * 1.5;
					iTimeTaken = (int)fGameTimer;
					bHasReachedLilyPad2 = true;
					bHasReachedRow1 = false;
					bHasReachedRow2 = false;
					bHasReachedRow3 = false;
					bHasReachedRow4 = false;
					bHasReachedRow5 = false;
					bHasReachedRow6 = false;
					bHasReachedRow7 = false;
					bHasReachedRow8 = false;
					bHasReachedRow9 = false;
					bHasReachedRow10 = false;
					bHasReachedRow11 = false;
					iPlayerScore += 50;
					iPlayerScore += (int)fGameTimer * 5;
					fTimeDisplayTimer = 0;
					fGameTimer = 30.0f;
				}
				else if (playerPosition.xPos >= 7 * iSpriteWidth - 1 && playerPosition.xPos <= 7 * iSpriteWidth + 1 && !bHasReachedLilyPad3)
				{
					playerPosition.xPos = iSpriteWidth * 7;
					playerPosition.yPos = iSpriteHeight * 1.5;
					iTimeTaken = (int)fGameTimer;
					bHasReachedLilyPad3 = true;
					bHasReachedRow1 = false;
					bHasReachedRow2 = false;
					bHasReachedRow3 = false;
					bHasReachedRow4 = false;
					bHasReachedRow5 = false;
					bHasReachedRow6 = false;
					bHasReachedRow7 = false;
					bHasReachedRow8 = false;
					bHasReachedRow9 = false;
					bHasReachedRow10 = false;
					bHasReachedRow11 = false;
					iPlayerScore += 50;
					iPlayerScore += (int)fGameTimer * 5;
					fTimeDisplayTimer = 0;
					fGameTimer = 30.0f;
				}
				else if (playerPosition.xPos >= 10 * iSpriteWidth - 1 && playerPosition.xPos <= 10 * iSpriteWidth + 1 && !bHasReachedLilyPad4)
				{
					playerPosition.xPos = iSpriteWidth * 7;
					playerPosition.yPos = iSpriteHeight * 1.5;
					iTimeTaken = (int)fGameTimer;
					bHasReachedLilyPad4 = true;
					bHasReachedRow1 = false;
					bHasReachedRow2 = false;
					bHasReachedRow3 = false;
					bHasReachedRow4 = false;
					bHasReachedRow5 = false;
					bHasReachedRow6 = false;
					bHasReachedRow7 = false;
					bHasReachedRow8 = false;
					bHasReachedRow9 = false;
					bHasReachedRow10 = false;
					bHasReachedRow11 = false;
					iPlayerScore += 50;
					iPlayerScore += (int)fGameTimer * 5;
					fTimeDisplayTimer = 0;
					fGameTimer = 30.0f;
				}
				else if (playerPosition.xPos >= 13 * iSpriteWidth - 1 && playerPosition.xPos <= 13 * iSpriteWidth + 1 && !bHasReachedLilyPad5)
				{
					playerPosition.xPos = iSpriteWidth * 7;
					playerPosition.yPos = iSpriteHeight * 1.5;
					iTimeTaken = (int)fGameTimer;
					bHasReachedLilyPad5 = true;
					bHasReachedRow1 = false;
					bHasReachedRow2 = false;
					bHasReachedRow3 = false;
					bHasReachedRow4 = false;
					bHasReachedRow5 = false;
					bHasReachedRow6 = false;
					bHasReachedRow7 = false;
					bHasReachedRow8 = false;
					bHasReachedRow9 = false;
					bHasReachedRow10 = false;
					bHasReachedRow11 = false;
					iPlayerScore += 50;
					iPlayerScore += (int)fGameTimer * 5;
					fTimeDisplayTimer = 0;
					fGameTimer = 30.0f;
				}
				else
				{
					bIsFrogDead = true;
					fDeathTimer = 0;
				}
			}
#pragma endregion
#pragma endregion

#pragma region Draw frog on lilypad after reaching each one
			if (bHasReachedLilyPad1)
			{
				UG::DrawLine(iSpriteWidth - 8, iScreenHeight - 2.5 * iSpriteHeight - 8, iSpriteWidth + 8, iScreenHeight - 2.5 * iSpriteHeight - 8, 2.0f, 0, UG::SColour(173, 255, 47, 255));
				UG::DrawLine(iSpriteWidth - 8, iScreenHeight - 2.5 * iSpriteHeight + 8, iSpriteWidth + 8, iScreenHeight - 2.5 * iSpriteHeight + 8, 2.0f, 0, UG::SColour(173, 255, 47, 255));
				UG::DrawLine(iSpriteWidth - 8, iScreenHeight - 2.5 * iSpriteHeight - 8, iSpriteWidth - 8, iScreenHeight - 2.5 * iSpriteHeight + 8, 2.0f, 0, UG::SColour(173, 255, 47, 255));
				UG::DrawLine(iSpriteWidth + 8, iScreenHeight - 2.5 * iSpriteHeight - 8, iSpriteWidth + 8, iScreenHeight - 2.5 * iSpriteHeight + 8, 2.0f, 0, UG::SColour(173, 255, 47, 255));
			}
			if (bHasReachedLilyPad2)
			{
				UG::DrawLine(4 * iSpriteWidth - 8, iScreenHeight - 2.5 * iSpriteHeight - 8, 4 * iSpriteWidth + 8, iScreenHeight - 2.5 * iSpriteHeight - 8, 2.0f, 0, UG::SColour(173, 255, 47, 255));
				UG::DrawLine(4 * iSpriteWidth - 8, iScreenHeight - 2.5 * iSpriteHeight + 8, 4 * iSpriteWidth + 8, iScreenHeight - 2.5 * iSpriteHeight + 8, 2.0f, 0, UG::SColour(173, 255, 47, 255));
				UG::DrawLine(4 * iSpriteWidth - 8, iScreenHeight - 2.5 * iSpriteHeight - 8, 4 * iSpriteWidth - 8, iScreenHeight - 2.5 * iSpriteHeight + 8, 2.0f, 0, UG::SColour(173, 255, 47, 255));
				UG::DrawLine(4 * iSpriteWidth + 8, iScreenHeight - 2.5 * iSpriteHeight - 8, 4 * iSpriteWidth + 8, iScreenHeight - 2.5 * iSpriteHeight + 8, 2.0f, 0, UG::SColour(173, 255, 47, 255));
			}
			if (bHasReachedLilyPad3)
			{
				UG::DrawLine(7 * iSpriteWidth - 8, iScreenHeight - 2.5 * iSpriteHeight - 8, 7 * iSpriteWidth + 8, iScreenHeight - 2.5 * iSpriteHeight - 8, 2.0f, 0, UG::SColour(173, 255, 47, 255));
				UG::DrawLine(7 * iSpriteWidth - 8, iScreenHeight - 2.5 * iSpriteHeight + 8, 7 * iSpriteWidth + 8, iScreenHeight - 2.5 * iSpriteHeight + 8, 2.0f, 0, UG::SColour(173, 255, 47, 255));
				UG::DrawLine(7 * iSpriteWidth - 8, iScreenHeight - 2.5 * iSpriteHeight - 8, 7 * iSpriteWidth - 8, iScreenHeight - 2.5 * iSpriteHeight + 8, 2.0f, 0, UG::SColour(173, 255, 47, 255));
				UG::DrawLine(7 * iSpriteWidth + 8, iScreenHeight - 2.5 * iSpriteHeight - 8, 7 * iSpriteWidth + 8, iScreenHeight - 2.5 * iSpriteHeight + 8, 2.0f, 0, UG::SColour(173, 255, 47, 255));
			}
			if (bHasReachedLilyPad4)
			{
				UG::DrawLine(10 * iSpriteWidth - 8, iScreenHeight - 2.5 * iSpriteHeight - 8, 10 * iSpriteWidth + 8, iScreenHeight - 2.5 * iSpriteHeight - 8, 2.0f, 0, UG::SColour(173, 255, 47, 255));
				UG::DrawLine(10 * iSpriteWidth - 8, iScreenHeight - 2.5 * iSpriteHeight + 8, 10 * iSpriteWidth + 8, iScreenHeight - 2.5 * iSpriteHeight + 8, 2.0f, 0, UG::SColour(173, 255, 47, 255));
				UG::DrawLine(10 * iSpriteWidth - 8, iScreenHeight - 2.5 * iSpriteHeight - 8, 10 * iSpriteWidth - 8, iScreenHeight - 2.5 * iSpriteHeight + 8, 2.0f, 0, UG::SColour(173, 255, 47, 255));
				UG::DrawLine(10 * iSpriteWidth + 8, iScreenHeight - 2.5 * iSpriteHeight - 8, 10 * iSpriteWidth + 8, iScreenHeight - 2.5 * iSpriteHeight + 8, 2.0f, 0, UG::SColour(173, 255, 47, 255));
			}
			if (bHasReachedLilyPad5)
			{
				UG::DrawLine(13 * iSpriteWidth - 8, iScreenHeight - 2.5 * iSpriteHeight - 8, 13 * iSpriteWidth + 8, iScreenHeight - 2.5 * iSpriteHeight - 8, 2.0f, 0, UG::SColour(173, 255, 47, 255));
				UG::DrawLine(13 * iSpriteWidth - 8, iScreenHeight - 2.5 * iSpriteHeight + 8, 13 * iSpriteWidth + 8, iScreenHeight - 2.5 * iSpriteHeight + 8, 2.0f, 0, UG::SColour(173, 255, 47, 255));
				UG::DrawLine(13 * iSpriteWidth - 8, iScreenHeight - 2.5 * iSpriteHeight - 8, 13 * iSpriteWidth - 8, iScreenHeight - 2.5 * iSpriteHeight + 8, 2.0f, 0, UG::SColour(173, 255, 47, 255));
				UG::DrawLine(13 * iSpriteWidth + 8, iScreenHeight - 2.5 * iSpriteHeight - 8, 13 * iSpriteWidth + 8, iScreenHeight - 2.5 * iSpriteHeight + 8, 2.0f, 0, UG::SColour(173, 255, 47, 255));
			}
#pragma endregion

#pragma region Sets conditions for going to the next level
			// Sets conditions for returning all 5 frogs home safely.
			if (bHasReachedLilyPad1 && bHasReachedLilyPad2 && bHasReachedLilyPad3 && bHasReachedLilyPad4 && bHasReachedLilyPad5 && fMovementTimer >= .25)
			{
				iPlayerScore += 200;
				bHasReachedLilyPad1 = false;
				bHasReachedLilyPad2 = false;
				bHasReachedLilyPad3 = false;
				bHasReachedLilyPad4 = false;
				bHasReachedLilyPad5 = false;
				iLevel += 1;
				fLevelDisplayTimer = 0;
			}
#pragma endregion

#pragma region Display level
			// Displays level.
			if (fLevelDisplayTimer < 2)
			{
				// Draws the level sign in white and fades after 1.5 seconds
				DrawCharacter('l', iScreenWidth / 2 + 2 * iSpriteWidth, iScreenHeight - iSpriteHeight, (char)(255 - trunc(fLevelDisplayTimer / 1.5) * (fLevelDisplayTimer - 1.5) * 255 * 2), (char)(255 - trunc(fLevelDisplayTimer / 1.5) * (fLevelDisplayTimer - 1.5) * 255 * 2), (char)(255 - trunc(fLevelDisplayTimer / 1.5) * (fLevelDisplayTimer - 1.5) * 255 * 2), (char)255); //the colour is solid white for 1.5 seconds then gradually fades to black over the next half a second.
				DrawCharacter('e', iScreenWidth / 2 + 2.5 * iSpriteWidth, iScreenHeight - iSpriteHeight, (char)(255 - trunc(fLevelDisplayTimer / 1.5) * (fLevelDisplayTimer - 1.5) * 255 * 2), (char)(255 - trunc(fLevelDisplayTimer / 1.5) * (fLevelDisplayTimer - 1.5) * 255 * 2), (char)(255 - trunc(fLevelDisplayTimer / 1.5) * (fLevelDisplayTimer - 1.5) * 255 * 2), (char)255);
				DrawCharacter('v', iScreenWidth / 2 + 3 * iSpriteWidth, iScreenHeight - iSpriteHeight, (char)(255 - trunc(fLevelDisplayTimer / 1.5) * (fLevelDisplayTimer - 1.5) * 255 * 2), (char)(255 - trunc(fLevelDisplayTimer / 1.5) * (fLevelDisplayTimer - 1.5) * 255 * 2), (char)(255 - trunc(fLevelDisplayTimer / 1.5) * (fLevelDisplayTimer - 1.5) * 255 * 2), (char)255);
				DrawCharacter('e', iScreenWidth / 2 + 3.5 * iSpriteWidth, iScreenHeight - iSpriteHeight, (char)(255 - trunc(fLevelDisplayTimer / 1.5) * (fLevelDisplayTimer - 1.5) * 255 * 2), (char)(255 - trunc(fLevelDisplayTimer / 1.5) * (fLevelDisplayTimer - 1.5) * 255 * 2), (char)(255 - trunc(fLevelDisplayTimer / 1.5) * (fLevelDisplayTimer - 1.5) * 255 * 2), (char)255);
				DrawCharacter('l', iScreenWidth / 2 + 4 * iSpriteWidth, iScreenHeight - iSpriteHeight, (char)(255 - trunc(fLevelDisplayTimer / 1.5) * (fLevelDisplayTimer - 1.5) * 255 * 2), (char)(255 - trunc(fLevelDisplayTimer / 1.5) * (fLevelDisplayTimer - 1.5) * 255 * 2), (char)(255 - trunc(fLevelDisplayTimer / 1.5) * (fLevelDisplayTimer - 1.5) * 255 * 2), (char)255);
				DrawNumber(iLevel, iScreenWidth / 2 + 5 * iSpriteWidth, iScreenHeight - iSpriteHeight, (char)(255 - trunc(fLevelDisplayTimer / 1.5) * (fLevelDisplayTimer - 1.5) * 255 * 2), (char)(255 - trunc(fLevelDisplayTimer / 1.5) * (fLevelDisplayTimer - 1.5) * 255 * 2), (char)(255 - trunc(fLevelDisplayTimer / 1.5) * (fLevelDisplayTimer - 1.5) * 255 * 2), (char)255);

				fLevelDisplayTimer += UG::GetDeltaTime();
			}
#pragma endregion

#pragma region Kill frog if frog drifts off-screen
			// Kills frog if frog drifts off-screen.
			if ((playerPosition.xPos < 0 || playerPosition.xPos > iScreenWidth) && fDeathTimer >= 1)
			{
				bIsFrogDead = true;
				fDeathTimer = 0;
			}
#pragma endregion

#pragma region Death Timer		
			if (fDeathTimer < 1 && fMovementTimer >= .25)
			{
				fDeathTimer += UG::GetDeltaTime();
			}
#pragma endregion

#pragma region Reset game if 'R' is pressed
			// Resets game when 'R' is pressed.
			if (UG::WasKeyPressed(UG::KEY_R))
			{
				// Loop through array to set positions.
				/// First 3 rows.
				for (int row = 0; row < 3; row++)
				{
					// Generate random # between 0 and 1024 (iScreenWidth + 2 * iSpriteWidth in pixels).
					randomShift = rand() % (iScreenWidth + 2 * iSpriteWidth);
					for (int car = 0; car < 3; car++)
					{
						carPositions2[row * 3 + car].xPos = iSpriteWidth * car * 16 / 4 + iSpriteWidth / 2 + float(randomShift);
						carPositions2[row * 3 + car].yPos = (row + 2) * iSpriteHeight + float(iSpriteHeight / 2);
					}
				}
				/// Fourth row.
				// Generate new random #.
				randomShift = rand() % (iScreenWidth + 2 * iSpriteWidth);
				carPositions2[9].xPos = iSpriteWidth / 2 + float(randomShift);
				carPositions2[9].yPos = 5 * iSpriteHeight + iSpriteHeight / 2;
				/// Fifth row.
				// Generate new random #.
				randomShift = rand() % (iScreenWidth + 2 * iSpriteWidth);
				for (int car = 0; car < 4; car++)
				{
					carPositions2[10 + car].xPos = iSpriteWidth * car * 8 / 4 + iSpriteWidth / 2 - car % 2 * iSpriteWidth + float(randomShift);
					carPositions2[10 + car].yPos = 6 * iSpriteHeight + iSpriteHeight / 2;
				}

				// Loop through array to set positions.
				/// First row.	
				// Generate new random #.
				randomShift = rand() % (iScreenWidth + 8 * iSpriteWidth);
				for (int logturtle = 0; logturtle < 15; logturtle++)
				{
					logturtlePositions2[logturtle].xPos = iSpriteWidth * logturtle + iSpriteWidth / 2 + float(trunc(logturtle / 3)) * iSpriteWidth + randomShift; //trunc part adds a gap after every third turtle
					logturtlePositions2[logturtle].yPos = 8 * iSpriteHeight + iSpriteHeight / 2;
				}
				/// Second row.	
				// Generate new random #.
				randomShift = rand() % (iScreenWidth + 8 * iSpriteWidth);
				for (int logturtle = 15; logturtle < 24; logturtle++)
				{
					int normLogturtle = logturtle - 15; //normalise the logturtle values so they start from 0
					logturtlePositions2[logturtle].xPos = iSpriteWidth * normLogturtle + iSpriteWidth / 2 + float((3 * trunc(normLogturtle / 3) - trunc(normLogturtle / 6))) * iSpriteWidth + randomShift; //trunc part adds a triple gap after every 3rd log and removes a gap after the 6th
					logturtlePositions2[logturtle].yPos = 9 * iSpriteHeight + iSpriteHeight / 2;
				}
				/// Third row.	
				// Generate new random #.
				randomShift = rand() % (iScreenWidth + 14 * iSpriteWidth);
				for (int logturtle = 24; logturtle < 42; logturtle++)
				{
					int normLogturtle = logturtle - 24;
					logturtlePositions2[logturtle].xPos = iSpriteWidth * normLogturtle + iSpriteWidth / 2 + 2 * float(trunc(normLogturtle / 6)) * iSpriteWidth + randomShift;
					logturtlePositions2[logturtle].yPos = 10 * iSpriteHeight + iSpriteHeight / 2;
				}
				/// Fourth row.
				// Generate new random #.
				randomShift = rand() % (iScreenWidth + 7 * iSpriteWidth);
				for (int logturtle = 42; logturtle < 50; logturtle++)
				{
					int normLogturtle = logturtle - 42;
					logturtlePositions2[logturtle].xPos = iSpriteWidth * normLogturtle + iSpriteWidth / 2 + 2 * float(trunc(normLogturtle / 2)) * iSpriteWidth + randomShift;
					logturtlePositions2[logturtle].yPos = 11 * iSpriteHeight + iSpriteHeight / 2;
				}
				/// Fifth row.
				// Generate new random #.
				randomShift = rand() % (iScreenWidth + 11 * iSpriteWidth);
				for (int logturtle = 50; logturtle < 66; logturtle++)
				{
					int normLogturtle = logturtle - 50;
					logturtlePositions2[logturtle].xPos = iSpriteWidth * normLogturtle + iSpriteWidth / 2 + float((trunc(normLogturtle / 4) + trunc(normLogturtle / 8) + trunc(normLogturtle / 12))) * iSpriteWidth + randomShift;
					logturtlePositions2[logturtle].yPos = 12 * iSpriteHeight + iSpriteHeight / 2;
				}

				// Set Initial conditions
				/// Set starting player position.
				playerPosition.xPos = iSpriteWidth * 7;
				playerPosition.yPos = iSpriteHeight * 1.5;
				/// State that frog is alive at game start.
				bIsFrogDead = false;
				/// Set initial game timer.
				fGameTimer = 30.0f;
				/// Set starting # of lives and initialise current lives.
				iStartLives = 4;
				iCurrentLives = iStartLives;
				/// Set starting score.
				iPlayerScore = 0;
				/// State that the frog has not moved across the map.
				bHasReachedRow1 = false;
				bHasReachedRow2 = false;
				bHasReachedRow3 = false;
				bHasReachedRow4 = false;
				bHasReachedRow5 = false;
				bHasReachedRow6 = false;
				bHasReachedRow7 = false;
				bHasReachedRow8 = false;
				bHasReachedRow9 = false;
				bHasReachedRow10 = false;
				bHasReachedRow11 = false;
				/// State that the frog has not reached any lily pads.
				bHasReachedLilyPad1 = false;
				bHasReachedLilyPad2 = false;
				bHasReachedLilyPad3 = false;
				bHasReachedLilyPad4 = false;
				bHasReachedLilyPad5 = false;
				/// Reset the display timer and level and tell the level to display.
				fLevelDisplayTimer = 0;
				iLevel = 1;
			}
#pragma endregion

#pragma region Display score gain
			// Checks the score gained every update and displays the score gain if it's larger than 10
			if (iPlayerScore > iScoreCompare + 10)
			{
				fScoreTimer = 0;
				iScoreGain = iPlayerScore - iScoreCompare;
			}
			iScoreCompare = iPlayerScore;
#pragma endregion

#pragma region Draw player or set death conditions
			// If frog dies, reset position and timer and lose a life.
			if (fDeathTimer >= 1)
			{
				if (bIsFrogDead && fMovementTimer >= .25) //  else if frog dead + mvmt tiumer > .25
				{
					playerPosition.xPos = iSpriteWidth * 7;
					playerPosition.yPos = iSpriteHeight * 1.5;
					iCurrentLives2 -= 1;
					iTimeTaken = (int)fGameTimer;
					fGameTimer = 30.0f;
					bIsFrogDead = false;
					bHitByCar = false;
					bDrowned = false;
				}
				// Draws Player.
				UG::DrawLine(playerPosition.xPos - 16 - (float)(1 - trunc((fMoveRightTimer + 10) / 10.25)) * ((1 - fMoveRightTimer * 4) * iSpriteHeight) + (float)(1 - trunc((fMoveLeftTimer + 10) / 10.25)) * ((1 - fMoveLeftTimer * 4) * iSpriteHeight), playerPosition.yPos - 16 - (float)(1 - trunc((fMoveUpTimer + 10) / 10.25)) * ((1 - fMoveUpTimer * 4) * iSpriteHeight) + (float)(1 - trunc((fMoveDownTimer + 10) / 10.25)) * ((1 - fMoveDownTimer * 4) * iSpriteHeight), playerPosition.xPos + 16 - (float)(1 - trunc((fMoveRightTimer + 10) / 10.25)) * ((1 - fMoveRightTimer * 4) * iSpriteHeight) + (float)(1 - trunc((fMoveLeftTimer + 10) / 10.25)) * ((1 - fMoveLeftTimer * 4) * iSpriteHeight), playerPosition.yPos - 16 - (float)(1 - trunc((fMoveUpTimer + 10) / 10.25)) * ((1 - fMoveUpTimer * 4) * iSpriteHeight) + (float)(1 - trunc((fMoveDownTimer + 10) / 10.25)) * ((1 - fMoveDownTimer * 4) * iSpriteHeight), 2.0f, 0, UG::SColour(173, 255, 47, 255)); //the +10 part in the trunc allows for an extra 10 seconds of lag.
				UG::DrawLine(playerPosition.xPos - 16 - (float)(1 - trunc((fMoveRightTimer + 10) / 10.25)) * ((1 - fMoveRightTimer * 4) * iSpriteHeight) + (float)(1 - trunc((fMoveLeftTimer + 10) / 10.25)) * ((1 - fMoveLeftTimer * 4) * iSpriteHeight), playerPosition.yPos + 16 - (float)(1 - trunc((fMoveUpTimer + 10) / 10.25)) * ((1 - fMoveUpTimer * 4) * iSpriteHeight) + (float)(1 - trunc((fMoveDownTimer + 10) / 10.25)) * ((1 - fMoveDownTimer * 4) * iSpriteHeight), playerPosition.xPos + 16 - (float)(1 - trunc((fMoveRightTimer + 10) / 10.25)) * ((1 - fMoveRightTimer * 4) * iSpriteHeight) + (float)(1 - trunc((fMoveLeftTimer + 10) / 10.25)) * ((1 - fMoveLeftTimer * 4) * iSpriteHeight), playerPosition.yPos + 16 - (float)(1 - trunc((fMoveUpTimer + 10) / 10.25)) * ((1 - fMoveUpTimer * 4) * iSpriteHeight) + (float)(1 - trunc((fMoveDownTimer + 10) / 10.25)) * ((1 - fMoveDownTimer * 4) * iSpriteHeight), 2.0f, 0, UG::SColour(173, 255, 47, 255));
				UG::DrawLine(playerPosition.xPos - 16 - (float)(1 - trunc((fMoveRightTimer + 10) / 10.25)) * ((1 - fMoveRightTimer * 4) * iSpriteHeight) + (float)(1 - trunc((fMoveLeftTimer + 10) / 10.25)) * ((1 - fMoveLeftTimer * 4) * iSpriteHeight), playerPosition.yPos - 16 - (float)(1 - trunc((fMoveUpTimer + 10) / 10.25)) * ((1 - fMoveUpTimer * 4) * iSpriteHeight) + (float)(1 - trunc((fMoveDownTimer + 10) / 10.25)) * ((1 - fMoveDownTimer * 4) * iSpriteHeight), playerPosition.xPos - 16 - (float)(1 - trunc((fMoveRightTimer + 10) / 10.25)) * ((1 - fMoveRightTimer * 4) * iSpriteHeight) + (float)(1 - trunc((fMoveLeftTimer + 10) / 10.25)) * ((1 - fMoveLeftTimer * 4) * iSpriteHeight), playerPosition.yPos + 16 - (float)(1 - trunc((fMoveUpTimer + 10) / 10.25)) * ((1 - fMoveUpTimer * 4) * iSpriteHeight) + (float)(1 - trunc((fMoveDownTimer + 10) / 10.25)) * ((1 - fMoveDownTimer * 4) * iSpriteHeight), 2.0f, 0, UG::SColour(173, 255, 47, 255));
				UG::DrawLine(playerPosition.xPos + 16 - (float)(1 - trunc((fMoveRightTimer + 10) / 10.25)) * ((1 - fMoveRightTimer * 4) * iSpriteHeight) + (float)(1 - trunc((fMoveLeftTimer + 10) / 10.25)) * ((1 - fMoveLeftTimer * 4) * iSpriteHeight), playerPosition.yPos - 16 - (float)(1 - trunc((fMoveUpTimer + 10) / 10.25)) * ((1 - fMoveUpTimer * 4) * iSpriteHeight) + (float)(1 - trunc((fMoveDownTimer + 10) / 10.25)) * ((1 - fMoveDownTimer * 4) * iSpriteHeight), playerPosition.xPos + 16 - (float)(1 - trunc((fMoveRightTimer + 10) / 10.25)) * ((1 - fMoveRightTimer * 4) * iSpriteHeight) + (float)(1 - trunc((fMoveLeftTimer + 10) / 10.25)) * ((1 - fMoveLeftTimer * 4) * iSpriteHeight), playerPosition.yPos + 16 - (float)(1 - trunc((fMoveUpTimer + 10) / 10.25)) * ((1 - fMoveUpTimer * 4) * iSpriteHeight) + (float)(1 - trunc((fMoveDownTimer + 10) / 10.25)) * ((1 - fMoveDownTimer * 4) * iSpriteHeight), 2.0f, 0, UG::SColour(173, 255, 47, 255));

				UG::DrawLine(playerPosition.xPos - 8 - (float)(1 - trunc((fMoveRightTimer + 10) / 10.25)) * ((1 - fMoveRightTimer * 4) * iSpriteHeight) + (float)(1 - trunc((fMoveLeftTimer + 10) / 10.25)) * ((1 - fMoveLeftTimer * 4) * iSpriteHeight), playerPosition.yPos - 8 - (float)(1 - trunc((fMoveUpTimer + 10) / 10.25)) * ((1 - fMoveUpTimer * 4) * iSpriteHeight) + (float)(1 - trunc((fMoveDownTimer + 10) / 10.25)) * ((1 - fMoveDownTimer * 4) * iSpriteHeight), playerPosition.xPos + 8 - (float)(1 - trunc((fMoveRightTimer + 10) / 10.25)) * ((1 - fMoveRightTimer * 4) * iSpriteHeight) + (float)(1 - trunc((fMoveLeftTimer + 10) / 10.25)) * ((1 - fMoveLeftTimer * 4) * iSpriteHeight), playerPosition.yPos - 8 - (float)(1 - trunc((fMoveUpTimer + 10) / 10.25)) * ((1 - fMoveUpTimer * 4) * iSpriteHeight) + (float)(1 - trunc((fMoveDownTimer + 10) / 10.25)) * ((1 - fMoveDownTimer * 4) * iSpriteHeight), 2.0f, 0, UG::SColour(173, 255, 47, 255));
				UG::DrawLine(playerPosition.xPos - 8 - (float)(1 - trunc((fMoveRightTimer + 10) / 10.25)) * ((1 - fMoveRightTimer * 4) * iSpriteHeight) + (float)(1 - trunc((fMoveLeftTimer + 10) / 10.25)) * ((1 - fMoveLeftTimer * 4) * iSpriteHeight), playerPosition.yPos + 8 - (float)(1 - trunc((fMoveUpTimer + 10) / 10.25)) * ((1 - fMoveUpTimer * 4) * iSpriteHeight) + (float)(1 - trunc((fMoveDownTimer + 10) / 10.25)) * ((1 - fMoveDownTimer * 4) * iSpriteHeight), playerPosition.xPos + 8 - (float)(1 - trunc((fMoveRightTimer + 10) / 10.25)) * ((1 - fMoveRightTimer * 4) * iSpriteHeight) + (float)(1 - trunc((fMoveLeftTimer + 10) / 10.25)) * ((1 - fMoveLeftTimer * 4) * iSpriteHeight), playerPosition.yPos + 8 - (float)(1 - trunc((fMoveUpTimer + 10) / 10.25)) * ((1 - fMoveUpTimer * 4) * iSpriteHeight) + (float)(1 - trunc((fMoveDownTimer + 10) / 10.25)) * ((1 - fMoveDownTimer * 4) * iSpriteHeight), 2.0f, 0, UG::SColour(173, 255, 47, 255));
				UG::DrawLine(playerPosition.xPos - 8 - (float)(1 - trunc((fMoveRightTimer + 10) / 10.25)) * ((1 - fMoveRightTimer * 4) * iSpriteHeight) + (float)(1 - trunc((fMoveLeftTimer + 10) / 10.25)) * ((1 - fMoveLeftTimer * 4) * iSpriteHeight), playerPosition.yPos - 8 - (float)(1 - trunc((fMoveUpTimer + 10) / 10.25)) * ((1 - fMoveUpTimer * 4) * iSpriteHeight) + (float)(1 - trunc((fMoveDownTimer + 10) / 10.25)) * ((1 - fMoveDownTimer * 4) * iSpriteHeight), playerPosition.xPos - 8 - (float)(1 - trunc((fMoveRightTimer + 10) / 10.25)) * ((1 - fMoveRightTimer * 4) * iSpriteHeight) + (float)(1 - trunc((fMoveLeftTimer + 10) / 10.25)) * ((1 - fMoveLeftTimer * 4) * iSpriteHeight), playerPosition.yPos + 8 - (float)(1 - trunc((fMoveUpTimer + 10) / 10.25)) * ((1 - fMoveUpTimer * 4) * iSpriteHeight) + (float)(1 - trunc((fMoveDownTimer + 10) / 10.25)) * ((1 - fMoveDownTimer * 4) * iSpriteHeight), 2.0f, 0, UG::SColour(173, 255, 47, 255));
				UG::DrawLine(playerPosition.xPos + 8 - (float)(1 - trunc((fMoveRightTimer + 10) / 10.25)) * ((1 - fMoveRightTimer * 4) * iSpriteHeight) + (float)(1 - trunc((fMoveLeftTimer + 10) / 10.25)) * ((1 - fMoveLeftTimer * 4) * iSpriteHeight), playerPosition.yPos - 8 - (float)(1 - trunc((fMoveUpTimer + 10) / 10.25)) * ((1 - fMoveUpTimer * 4) * iSpriteHeight) + (float)(1 - trunc((fMoveDownTimer + 10) / 10.25)) * ((1 - fMoveDownTimer * 4) * iSpriteHeight), playerPosition.xPos + 8 - (float)(1 - trunc((fMoveRightTimer + 10) / 10.25)) * ((1 - fMoveRightTimer * 4) * iSpriteHeight) + (float)(1 - trunc((fMoveLeftTimer + 10) / 10.25)) * ((1 - fMoveLeftTimer * 4) * iSpriteHeight), playerPosition.yPos + 8 - (float)(1 - trunc((fMoveUpTimer + 10) / 10.25)) * ((1 - fMoveUpTimer * 4) * iSpriteHeight) + (float)(1 - trunc((fMoveDownTimer + 10) / 10.25)) * ((1 - fMoveDownTimer * 4) * iSpriteHeight), 2.0f, 0, UG::SColour(173, 255, 47, 255));
			}
			else if (bDrowned)
			{
				// White frog (skeleton).
				UG::DrawLine(playerPosition.xPos - 16 - float((1 - trunc((fMoveRightTimer + 10) / 10.25))) * ((1 - fMoveRightTimer * 4) * iSpriteHeight) + float((1 - trunc((fMoveLeftTimer + 10) / 10.25))) * ((1 - fMoveLeftTimer * 4) * iSpriteHeight), playerPosition.yPos - 16 - float((1 - trunc((fMoveUpTimer + 10) / 10.25))) * ((1 - fMoveUpTimer * 4) * iSpriteHeight) + float((1 - trunc((fMoveDownTimer + 10) / 10.25))) * ((1 - fMoveDownTimer * 4) * iSpriteHeight), playerPosition.xPos + 16 - float((1 - trunc((fMoveRightTimer + 10) / 10.25))) * ((1 - fMoveRightTimer * 4) * iSpriteHeight) + float((1 - trunc((fMoveLeftTimer + 10) / 10.25))) * ((1 - fMoveLeftTimer * 4) * iSpriteHeight), playerPosition.yPos - 16 - float((1 - trunc((fMoveUpTimer + 10) / 10.25))) * ((1 - fMoveUpTimer * 4) * iSpriteHeight) + float((1 - trunc((fMoveDownTimer + 10) / 10.25))) * ((1 - fMoveDownTimer * 4) * iSpriteHeight), 2.0f, 0, UG::SColour(200, 200, 200, (unsigned char)(255 * (1 - fDeathTimer)))); //the +10 part in the trunc allows for an extra 10 seconds of lag.
				UG::DrawLine(playerPosition.xPos - 16 - float((1 - trunc((fMoveRightTimer + 10) / 10.25))) * ((1 - fMoveRightTimer * 4) * iSpriteHeight) + float((1 - trunc((fMoveLeftTimer + 10) / 10.25))) * ((1 - fMoveLeftTimer * 4) * iSpriteHeight), playerPosition.yPos + 16 - float((1 - trunc((fMoveUpTimer + 10) / 10.25))) * ((1 - fMoveUpTimer * 4) * iSpriteHeight) + float((1 - trunc((fMoveDownTimer + 10) / 10.25))) * ((1 - fMoveDownTimer * 4) * iSpriteHeight), playerPosition.xPos + 16 - float((1 - trunc((fMoveRightTimer + 10) / 10.25))) * ((1 - fMoveRightTimer * 4) * iSpriteHeight) + float((1 - trunc((fMoveLeftTimer + 10) / 10.25))) * ((1 - fMoveLeftTimer * 4) * iSpriteHeight), playerPosition.yPos + 16 - float((1 - trunc((fMoveUpTimer + 10) / 10.25))) * ((1 - fMoveUpTimer * 4) * iSpriteHeight) + float((1 - trunc((fMoveDownTimer + 10) / 10.25))) * ((1 - fMoveDownTimer * 4) * iSpriteHeight), 2.0f, 0, UG::SColour(200, 200, 200, (unsigned char)(255 * (1 - fDeathTimer))));
				UG::DrawLine(playerPosition.xPos - 16 - float((1 - trunc((fMoveRightTimer + 10) / 10.25))) * ((1 - fMoveRightTimer * 4) * iSpriteHeight) + float((1 - trunc((fMoveLeftTimer + 10) / 10.25))) * ((1 - fMoveLeftTimer * 4) * iSpriteHeight), playerPosition.yPos - 16 - float((1 - trunc((fMoveUpTimer + 10) / 10.25))) * ((1 - fMoveUpTimer * 4) * iSpriteHeight) + float((1 - trunc((fMoveDownTimer + 10) / 10.25))) * ((1 - fMoveDownTimer * 4) * iSpriteHeight), playerPosition.xPos - 16 - float((1 - trunc((fMoveRightTimer + 10) / 10.25))) * ((1 - fMoveRightTimer * 4) * iSpriteHeight) + float((1 - trunc((fMoveLeftTimer + 10) / 10.25))) * ((1 - fMoveLeftTimer * 4) * iSpriteHeight), playerPosition.yPos + 16 - float((1 - trunc((fMoveUpTimer + 10) / 10.25))) * ((1 - fMoveUpTimer * 4) * iSpriteHeight) + float((1 - trunc((fMoveDownTimer + 10) / 10.25))) * ((1 - fMoveDownTimer * 4) * iSpriteHeight), 2.0f, 0, UG::SColour(200, 200, 200, (unsigned char)(255 * (1 - fDeathTimer))));
				UG::DrawLine(playerPosition.xPos + 16 - float((1 - trunc((fMoveRightTimer + 10) / 10.25))) * ((1 - fMoveRightTimer * 4) * iSpriteHeight) + float((1 - trunc((fMoveLeftTimer + 10) / 10.25))) * ((1 - fMoveLeftTimer * 4) * iSpriteHeight), playerPosition.yPos - 16 - float((1 - trunc((fMoveUpTimer + 10) / 10.25))) * ((1 - fMoveUpTimer * 4) * iSpriteHeight) + float((1 - trunc((fMoveDownTimer + 10) / 10.25))) * ((1 - fMoveDownTimer * 4) * iSpriteHeight), playerPosition.xPos + 16 - float((1 - trunc((fMoveRightTimer + 10) / 10.25))) * ((1 - fMoveRightTimer * 4) * iSpriteHeight) + float((1 - trunc((fMoveLeftTimer + 10) / 10.25))) * ((1 - fMoveLeftTimer * 4) * iSpriteHeight), playerPosition.yPos + 16 - float((1 - trunc((fMoveUpTimer + 10) / 10.25))) * ((1 - fMoveUpTimer * 4) * iSpriteHeight) + float((1 - trunc((fMoveDownTimer + 10) / 10.25))) * ((1 - fMoveDownTimer * 4) * iSpriteHeight), 2.0f, 0, UG::SColour(200, 200, 200, (unsigned char)(255 * (1 - fDeathTimer))));

				UG::DrawLine(playerPosition.xPos - 8 - float((1 - trunc((fMoveRightTimer + 10) / 10.25))) * ((1 - fMoveRightTimer * 4) * iSpriteHeight) + float((1 - trunc((fMoveLeftTimer + 10) / 10.25))) * ((1 - fMoveLeftTimer * 4) * iSpriteHeight), playerPosition.yPos - 8 - float((1 - trunc((fMoveUpTimer + 10) / 10.25))) * ((1 - fMoveUpTimer * 4) * iSpriteHeight) + float((1 - trunc((fMoveDownTimer + 10) / 10.25))) * ((1 - fMoveDownTimer * 4) * iSpriteHeight), playerPosition.xPos + 8 - float((1 - trunc((fMoveRightTimer + 10) / 10.25))) * ((1 - fMoveRightTimer * 4) * iSpriteHeight) + float((1 - trunc((fMoveLeftTimer + 10) / 10.25))) * ((1 - fMoveLeftTimer * 4) * iSpriteHeight), playerPosition.yPos - 8 - float((1 - trunc((fMoveUpTimer + 10) / 10.25))) * ((1 - fMoveUpTimer * 4) * iSpriteHeight) + float((1 - trunc((fMoveDownTimer + 10) / 10.25))) * ((1 - fMoveDownTimer * 4) * iSpriteHeight), 2.0f, 0, UG::SColour(200, 200, 200, (unsigned char)(255 * (1 - fDeathTimer))));
				UG::DrawLine(playerPosition.xPos - 8 - float((1 - trunc((fMoveRightTimer + 10) / 10.25))) * ((1 - fMoveRightTimer * 4) * iSpriteHeight) + float((1 - trunc((fMoveLeftTimer + 10) / 10.25))) * ((1 - fMoveLeftTimer * 4) * iSpriteHeight), playerPosition.yPos + 8 - float((1 - trunc((fMoveUpTimer + 10) / 10.25))) * ((1 - fMoveUpTimer * 4) * iSpriteHeight) + float((1 - trunc((fMoveDownTimer + 10) / 10.25))) * ((1 - fMoveDownTimer * 4) * iSpriteHeight), playerPosition.xPos + 8 - float((1 - trunc((fMoveRightTimer + 10) / 10.25))) * ((1 - fMoveRightTimer * 4) * iSpriteHeight) + float((1 - trunc((fMoveLeftTimer + 10) / 10.25))) * ((1 - fMoveLeftTimer * 4) * iSpriteHeight), playerPosition.yPos + 8 - float((1 - trunc((fMoveUpTimer + 10) / 10.25))) * ((1 - fMoveUpTimer * 4) * iSpriteHeight) + float((1 - trunc((fMoveDownTimer + 10) / 10.25))) * ((1 - fMoveDownTimer * 4) * iSpriteHeight), 2.0f, 0, UG::SColour(200, 200, 200, (unsigned char)(255 * (1 - fDeathTimer))));
				UG::DrawLine(playerPosition.xPos - 8 - float((1 - trunc((fMoveRightTimer + 10) / 10.25))) * ((1 - fMoveRightTimer * 4) * iSpriteHeight) + float((1 - trunc((fMoveLeftTimer + 10) / 10.25))) * ((1 - fMoveLeftTimer * 4) * iSpriteHeight), playerPosition.yPos - 8 - float((1 - trunc((fMoveUpTimer + 10) / 10.25))) * ((1 - fMoveUpTimer * 4) * iSpriteHeight) + float((1 - trunc((fMoveDownTimer + 10) / 10.25))) * ((1 - fMoveDownTimer * 4) * iSpriteHeight), playerPosition.xPos - 8 - float((1 - trunc((fMoveRightTimer + 10) / 10.25))) * ((1 - fMoveRightTimer * 4) * iSpriteHeight) + float((1 - trunc((fMoveLeftTimer + 10) / 10.25))) * ((1 - fMoveLeftTimer * 4) * iSpriteHeight), playerPosition.yPos + 8 - float((1 - trunc((fMoveUpTimer + 10) / 10.25))) * ((1 - fMoveUpTimer * 4) * iSpriteHeight) + float((1 - trunc((fMoveDownTimer + 10) / 10.25))) * ((1 - fMoveDownTimer * 4) * iSpriteHeight), 2.0f, 0, UG::SColour(200, 200, 200, (unsigned char)(255 * (1 - fDeathTimer))));
				UG::DrawLine(playerPosition.xPos + 8 - float((1 - trunc((fMoveRightTimer + 10) / 10.25))) * ((1 - fMoveRightTimer * 4) * iSpriteHeight) + float((1 - trunc((fMoveLeftTimer + 10) / 10.25))) * ((1 - fMoveLeftTimer * 4) * iSpriteHeight), playerPosition.yPos - 8 - float((1 - trunc((fMoveUpTimer + 10) / 10.25))) * ((1 - fMoveUpTimer * 4) * iSpriteHeight) + float((1 - trunc((fMoveDownTimer + 10) / 10.25))) * ((1 - fMoveDownTimer * 4) * iSpriteHeight), playerPosition.xPos + 8 - float((1 - trunc((fMoveRightTimer + 10) / 10.25))) * ((1 - fMoveRightTimer * 4) * iSpriteHeight) + float((1 - trunc((fMoveLeftTimer + 10) / 10.25))) * ((1 - fMoveLeftTimer * 4) * iSpriteHeight), playerPosition.yPos + 8 - float((1 - trunc((fMoveUpTimer + 10) / 10.25))) * ((1 - fMoveUpTimer * 4) * iSpriteHeight) + float((1 - trunc((fMoveDownTimer + 10) / 10.25))) * ((1 - fMoveDownTimer * 4) * iSpriteHeight), 2.0f, 0, UG::SColour(200, 200, 200, (unsigned char)(255 * (1 - fDeathTimer))));
			}
			// Draw death animation
			else
			{
				// White frog (skeleton).
				UG::DrawLine(playerPosition.xPos - 16 - float((1 - trunc((fMoveRightTimer + 10) / 10.25))) * ((1 - fMoveRightTimer * 4) * iSpriteHeight) + float((1 - trunc((fMoveLeftTimer + 10) / 10.25))) * ((1 - fMoveLeftTimer * 4) * iSpriteHeight), playerPosition.yPos - 16 - float((1 - trunc((fMoveUpTimer + 10) / 10.25))) * ((1 - fMoveUpTimer * 4) * iSpriteHeight) + float((1 - trunc((fMoveDownTimer + 10) / 10.25))) * ((1 - fMoveDownTimer * 4) * iSpriteHeight), playerPosition.xPos + 16 - float((1 - trunc((fMoveRightTimer + 10) / 10.25))) * ((1 - fMoveRightTimer * 4) * iSpriteHeight) + float((1 - trunc((fMoveLeftTimer + 10) / 10.25))) * ((1 - fMoveLeftTimer * 4) * iSpriteHeight), playerPosition.yPos - 16 - float((1 - trunc((fMoveUpTimer + 10) / 10.25))) * ((1 - fMoveUpTimer * 4) * iSpriteHeight) + float((1 - trunc((fMoveDownTimer + 10) / 10.25))) * ((1 - fMoveDownTimer * 4) * iSpriteHeight), 2.0f, 0, UG::SColour(200, 200, 200, (unsigned char)(255 * (1 - fDeathTimer)))); //the +10 part in the trunc allows for an extra 10 seconds of lag.
				UG::DrawLine(playerPosition.xPos - 16 - float((1 - trunc((fMoveRightTimer + 10) / 10.25))) * ((1 - fMoveRightTimer * 4) * iSpriteHeight) + float((1 - trunc((fMoveLeftTimer + 10) / 10.25))) * ((1 - fMoveLeftTimer * 4) * iSpriteHeight), playerPosition.yPos + 16 - float((1 - trunc((fMoveUpTimer + 10) / 10.25))) * ((1 - fMoveUpTimer * 4) * iSpriteHeight) + float((1 - trunc((fMoveDownTimer + 10) / 10.25))) * ((1 - fMoveDownTimer * 4) * iSpriteHeight), playerPosition.xPos + 16 - float((1 - trunc((fMoveRightTimer + 10) / 10.25))) * ((1 - fMoveRightTimer * 4) * iSpriteHeight) + float((1 - trunc((fMoveLeftTimer + 10) / 10.25))) * ((1 - fMoveLeftTimer * 4) * iSpriteHeight), playerPosition.yPos + 16 - float((1 - trunc((fMoveUpTimer + 10) / 10.25))) * ((1 - fMoveUpTimer * 4) * iSpriteHeight) + float((1 - trunc((fMoveDownTimer + 10) / 10.25))) * ((1 - fMoveDownTimer * 4) * iSpriteHeight), 2.0f, 0, UG::SColour(200, 200, 200, (unsigned char)(255 * (1 - fDeathTimer))));
				UG::DrawLine(playerPosition.xPos - 16 - float((1 - trunc((fMoveRightTimer + 10) / 10.25))) * ((1 - fMoveRightTimer * 4) * iSpriteHeight) + float((1 - trunc((fMoveLeftTimer + 10) / 10.25))) * ((1 - fMoveLeftTimer * 4) * iSpriteHeight), playerPosition.yPos - 16 - float((1 - trunc((fMoveUpTimer + 10) / 10.25))) * ((1 - fMoveUpTimer * 4) * iSpriteHeight) + float((1 - trunc((fMoveDownTimer + 10) / 10.25))) * ((1 - fMoveDownTimer * 4) * iSpriteHeight), playerPosition.xPos - 16 - float((1 - trunc((fMoveRightTimer + 10) / 10.25))) * ((1 - fMoveRightTimer * 4) * iSpriteHeight) + float((1 - trunc((fMoveLeftTimer + 10) / 10.25))) * ((1 - fMoveLeftTimer * 4) * iSpriteHeight), playerPosition.yPos + 16 - float((1 - trunc((fMoveUpTimer + 10) / 10.25))) * ((1 - fMoveUpTimer * 4) * iSpriteHeight) + float((1 - trunc((fMoveDownTimer + 10) / 10.25))) * ((1 - fMoveDownTimer * 4) * iSpriteHeight), 2.0f, 0, UG::SColour(200, 200, 200, (unsigned char)(255 * (1 - fDeathTimer))));
				UG::DrawLine(playerPosition.xPos + 16 - float((1 - trunc((fMoveRightTimer + 10) / 10.25))) * ((1 - fMoveRightTimer * 4) * iSpriteHeight) + float((1 - trunc((fMoveLeftTimer + 10) / 10.25))) * ((1 - fMoveLeftTimer * 4) * iSpriteHeight), playerPosition.yPos - 16 - float((1 - trunc((fMoveUpTimer + 10) / 10.25))) * ((1 - fMoveUpTimer * 4) * iSpriteHeight) + float((1 - trunc((fMoveDownTimer + 10) / 10.25))) * ((1 - fMoveDownTimer * 4) * iSpriteHeight), playerPosition.xPos + 16 - float((1 - trunc((fMoveRightTimer + 10) / 10.25))) * ((1 - fMoveRightTimer * 4) * iSpriteHeight) + float((1 - trunc((fMoveLeftTimer + 10) / 10.25))) * ((1 - fMoveLeftTimer * 4) * iSpriteHeight), playerPosition.yPos + 16 - float((1 - trunc((fMoveUpTimer + 10) / 10.25))) * ((1 - fMoveUpTimer * 4) * iSpriteHeight) + float((1 - trunc((fMoveDownTimer + 10) / 10.25))) * ((1 - fMoveDownTimer * 4) * iSpriteHeight), 2.0f, 0, UG::SColour(200, 200, 200, (unsigned char)(255 * (1 - fDeathTimer))));

				UG::DrawLine(playerPosition.xPos - 8 - float((1 - trunc((fMoveRightTimer + 10) / 10.25))) * ((1 - fMoveRightTimer * 4) * iSpriteHeight) + float((1 - trunc((fMoveLeftTimer + 10) / 10.25))) * ((1 - fMoveLeftTimer * 4) * iSpriteHeight), playerPosition.yPos - 8 - float((1 - trunc((fMoveUpTimer + 10) / 10.25))) * ((1 - fMoveUpTimer * 4) * iSpriteHeight) + float((1 - trunc((fMoveDownTimer + 10) / 10.25))) * ((1 - fMoveDownTimer * 4) * iSpriteHeight), playerPosition.xPos + 8 - float((1 - trunc((fMoveRightTimer + 10) / 10.25))) * ((1 - fMoveRightTimer * 4) * iSpriteHeight) + float((1 - trunc((fMoveLeftTimer + 10) / 10.25))) * ((1 - fMoveLeftTimer * 4) * iSpriteHeight), playerPosition.yPos - 8 - float((1 - trunc((fMoveUpTimer + 10) / 10.25))) * ((1 - fMoveUpTimer * 4) * iSpriteHeight) + float((1 - trunc((fMoveDownTimer + 10) / 10.25))) * ((1 - fMoveDownTimer * 4) * iSpriteHeight), 2.0f, 0, UG::SColour(200, 200, 200, (unsigned char)(255 * (1 - fDeathTimer))));
				UG::DrawLine(playerPosition.xPos - 8 - float((1 - trunc((fMoveRightTimer + 10) / 10.25))) * ((1 - fMoveRightTimer * 4) * iSpriteHeight) + float((1 - trunc((fMoveLeftTimer + 10) / 10.25))) * ((1 - fMoveLeftTimer * 4) * iSpriteHeight), playerPosition.yPos + 8 - float((1 - trunc((fMoveUpTimer + 10) / 10.25))) * ((1 - fMoveUpTimer * 4) * iSpriteHeight) + float((1 - trunc((fMoveDownTimer + 10) / 10.25))) * ((1 - fMoveDownTimer * 4) * iSpriteHeight), playerPosition.xPos + 8 - float((1 - trunc((fMoveRightTimer + 10) / 10.25))) * ((1 - fMoveRightTimer * 4) * iSpriteHeight) + float((1 - trunc((fMoveLeftTimer + 10) / 10.25))) * ((1 - fMoveLeftTimer * 4) * iSpriteHeight), playerPosition.yPos + 8 - float((1 - trunc((fMoveUpTimer + 10) / 10.25))) * ((1 - fMoveUpTimer * 4) * iSpriteHeight) + float((1 - trunc((fMoveDownTimer + 10) / 10.25))) * ((1 - fMoveDownTimer * 4) * iSpriteHeight), 2.0f, 0, UG::SColour(200, 200, 200, (unsigned char)(255 * (1 - fDeathTimer))));
				UG::DrawLine(playerPosition.xPos - 8 - float((1 - trunc((fMoveRightTimer + 10) / 10.25))) * ((1 - fMoveRightTimer * 4) * iSpriteHeight) + float((1 - trunc((fMoveLeftTimer + 10) / 10.25))) * ((1 - fMoveLeftTimer * 4) * iSpriteHeight), playerPosition.yPos - 8 - float((1 - trunc((fMoveUpTimer + 10) / 10.25))) * ((1 - fMoveUpTimer * 4) * iSpriteHeight) + float((1 - trunc((fMoveDownTimer + 10) / 10.25))) * ((1 - fMoveDownTimer * 4) * iSpriteHeight), playerPosition.xPos - 8 - float((1 - trunc((fMoveRightTimer + 10) / 10.25))) * ((1 - fMoveRightTimer * 4) * iSpriteHeight) + float((1 - trunc((fMoveLeftTimer + 10) / 10.25))) * ((1 - fMoveLeftTimer * 4) * iSpriteHeight), playerPosition.yPos + 8 - float((1 - trunc((fMoveUpTimer + 10) / 10.25))) * ((1 - fMoveUpTimer * 4) * iSpriteHeight) + float((1 - trunc((fMoveDownTimer + 10) / 10.25))) * ((1 - fMoveDownTimer * 4) * iSpriteHeight), 2.0f, 0, UG::SColour(200, 200, 200, (unsigned char)(255 * (1 - fDeathTimer))));
				UG::DrawLine(playerPosition.xPos + 8 - float((1 - trunc((fMoveRightTimer + 10) / 10.25))) * ((1 - fMoveRightTimer * 4) * iSpriteHeight) + float((1 - trunc((fMoveLeftTimer + 10) / 10.25))) * ((1 - fMoveLeftTimer * 4) * iSpriteHeight), playerPosition.yPos - 8 - float((1 - trunc((fMoveUpTimer + 10) / 10.25))) * ((1 - fMoveUpTimer * 4) * iSpriteHeight) + float((1 - trunc((fMoveDownTimer + 10) / 10.25))) * ((1 - fMoveDownTimer * 4) * iSpriteHeight), playerPosition.xPos + 8 - float((1 - trunc((fMoveRightTimer + 10) / 10.25))) * ((1 - fMoveRightTimer * 4) * iSpriteHeight) + float((1 - trunc((fMoveLeftTimer + 10) / 10.25))) * ((1 - fMoveLeftTimer * 4) * iSpriteHeight), playerPosition.yPos + 8 - float((1 - trunc((fMoveUpTimer + 10) / 10.25))) * ((1 - fMoveUpTimer * 4) * iSpriteHeight) + float((1 - trunc((fMoveDownTimer + 10) / 10.25))) * ((1 - fMoveDownTimer * 4) * iSpriteHeight), 2.0f, 0, UG::SColour(200, 200, 200, (unsigned char)(255 * (1 - fDeathTimer))));

				if (bHitByCar)
				{
					// Blood spreading.
					UG::DrawLine(playerPosition.xPos - fDeathTimer * .4f * iSpriteWidth, playerPosition.yPos, playerPosition.xPos + fDeathTimer * .4f * iSpriteWidth, playerPosition.yPos, 51 * fDeathTimer, 0, UG::SColour(138, 7, 7, 64));
					UG::DrawLine(playerPosition.xPos - fDeathTimer * .35f * iSpriteWidth, playerPosition.yPos, playerPosition.xPos + fDeathTimer * .35f * iSpriteWidth, playerPosition.yPos, 45 * fDeathTimer, 0, UG::SColour(138, 7, 7, 64));
					UG::DrawLine(playerPosition.xPos - fDeathTimer * .3f * iSpriteWidth, playerPosition.yPos, playerPosition.xPos + fDeathTimer * .3f * iSpriteWidth, playerPosition.yPos, 38 * fDeathTimer, 0, UG::SColour(138, 7, 7, 64));
					UG::DrawLine(playerPosition.xPos - fDeathTimer * .25f * iSpriteWidth, playerPosition.yPos, playerPosition.xPos + fDeathTimer * .25f * iSpriteWidth, playerPosition.yPos, 32 * fDeathTimer, 0, UG::SColour(138, 7, 7, 64));
					UG::DrawLine(playerPosition.xPos - fDeathTimer * .2f * iSpriteWidth, playerPosition.yPos, playerPosition.xPos + fDeathTimer * .25f * iSpriteWidth, playerPosition.yPos, 26 * fDeathTimer, 0, UG::SColour(138, 7, 7, 64));
					UG::DrawLine(playerPosition.xPos - fDeathTimer * .15f * iSpriteWidth, playerPosition.yPos, playerPosition.xPos + fDeathTimer * .25f * iSpriteWidth, playerPosition.yPos, 19 * fDeathTimer, 0, UG::SColour(138, 7, 7, 64));
					UG::DrawLine(playerPosition.xPos - fDeathTimer * .1f * iSpriteWidth, playerPosition.yPos, playerPosition.xPos + fDeathTimer * .25f * iSpriteWidth, playerPosition.yPos, 13 * fDeathTimer, 0, UG::SColour(138, 7, 7, 64));
				}
			}
#pragma endregion

			// Quit out to the menu when escape is pressed.
			if (UG::WasKeyPressed(UG::KEY_ESCAPE))
				eCurrentState = MENU;
			break;
#pragma endregion

#pragma region HIGHSCORES
		case HIGHSCORES:

#pragma region Draw background
			// Blue portion of the background.
			UG::DrawSprite(sBlueBackground.iSpriteID);

			// Frogger title.
			UG::SetSpriteScale(sTitle.iSpriteID, iSpriteWidth * 9.5f + iSpriteWidth * (float)sin(M_PI * fMenuTimer / 1.5), (float)iSpriteHeight);
			UG::DrawSprite(sTitle.iSpriteID);
#pragma endregion

#pragma region Draw Highscores
			// Highscores text.
			UG::DrawString("Highscores", 267, (int)(iScreenHeight - (7.5 - 4 * (fHighscoresTimer)) * iSpriteHeight), 0, yellow); //y-position increases with highscore timer for a smooth transition from the menu			

			// Increment the menu timer (I use the menu timer so that the scale of the Frogger title is preserved).
			fMenuTimer += UG::GetDeltaTime();
			
			// Increment High scores timer which is used in the first animation.
			if (fHighscoresTimer < 0.5f) 
			{
				fHighscoresTimer += UG::GetDeltaTime();
			}
			else 
			{
				fHighscoresTimer = 0.5f; //set a fixed value for the High scores timer so the 'High scores' string stays in the right place
				fHighscoresTimer2 += UG::GetDeltaTime(); //increment second High scores timer
			}

			// Every 0.1 seconds another score is displayed.
			if (fHighscoresTimer2 > 0.1f) 
			{
				UG::DrawString("1.", 267, (int)(iScreenHeight - 6.5 * iSpriteHeight), 0, magenta);
				DrawLargeNumberHS(iaHighScores[9], 267.0f + 3 * iSpriteWidth + 4, (float)(iScreenHeight - 6.25 * iSpriteHeight) , (char)255, (char)71, (char)247);
				UG::DrawString(caHighScoreNames[9], 267 + 4 * iSpriteWidth, (int)(iScreenHeight - 6.5 * iSpriteHeight), 0, magenta);
			}
			if (fHighscoresTimer2 > 0.2f)
			{
				UG::DrawString("2.", 267, (int)(iScreenHeight - 7 * iSpriteHeight), 0, cyan);
				DrawLargeNumberHS(iaHighScores[8], 267.0f + 3 * iSpriteWidth + 4, (float)(iScreenHeight - 6.75 * iSpriteHeight), (char)0, (char)222, (char)247);
				UG::DrawString(caHighScoreNames[8], 267 + 4 * iSpriteWidth, (int)(iScreenHeight - 7 * iSpriteHeight), 0, cyan);
			}
			if (fHighscoresTimer2 > 0.3f)
			{
				UG::DrawString("3.", 267, (int)(iScreenHeight - 7.5 * iSpriteHeight), 0, cyan);
				DrawLargeNumberHS(iaHighScores[7], 267.0f + 3 * iSpriteWidth + 4, (float)(iScreenHeight - 7.25 * iSpriteHeight), (char)0, (char)222, (char)247);
				UG::DrawString(caHighScoreNames[7], 267 + 4 * iSpriteWidth, (int)(iScreenHeight - 7.5 * iSpriteHeight), 0, cyan);
			}
			if (fHighscoresTimer2 > 0.4f)
			{
				UG::DrawString("4.", 267, (int)(iScreenHeight - 8 * iSpriteHeight), 0, red);
				DrawLargeNumberHS(iaHighScores[6], 267.0f + 3 * iSpriteWidth + 4, (float)(iScreenHeight - 7.75 * iSpriteHeight), (char)255, (char)0, (char)0);
				UG::DrawString(caHighScoreNames[6], 267 + 4 * iSpriteWidth, (int)(iScreenHeight - 8 * iSpriteHeight), 0, red);
			}
			if (fHighscoresTimer2 > 0.5f)
			{
				UG::DrawString("5.", 267, (int)(iScreenHeight - 8.5 * iSpriteHeight), 0, red);
				DrawLargeNumberHS(iaHighScores[5], 267.0f + 3 * iSpriteWidth + 4, (float)(iScreenHeight - 8.25 * iSpriteHeight), (char)255, (char)0, (char)0);
				UG::DrawString(caHighScoreNames[5], 267 + 4 * iSpriteWidth, (int)(iScreenHeight - 8.5 * iSpriteHeight), 0, red);
			}
			if (fHighscoresTimer2 > 0.6f)
			{
				UG::DrawString("6.", 267, (int)(iScreenHeight - 9 * iSpriteHeight), 0, red);
				DrawLargeNumberHS(iaHighScores[4], 267.0f + 3 * iSpriteWidth + 4, (float)(iScreenHeight - 8.75 * iSpriteHeight), (char)255, (char)0, (char)0);
				UG::DrawString(caHighScoreNames[4], 267 + 4 * iSpriteWidth, (int)(iScreenHeight - 9 * iSpriteHeight), 0, red);
			}
			if (fHighscoresTimer2 > 0.7f)
			{
				UG::DrawString("7.", 267, (int)(iScreenHeight - 9.5 * iSpriteHeight), 0, grey);
				DrawLargeNumberHS(iaHighScores[3], 267.0f + 3 * iSpriteWidth + 4, (float)(iScreenHeight - 9.25 * iSpriteHeight), (char)222, (char)222, (char)247);
				UG::DrawString(caHighScoreNames[3], 267 + 4 * iSpriteWidth, (int)(iScreenHeight - 9.5 * iSpriteHeight), 0, grey);
			}
			if (fHighscoresTimer2 > 0.8f)
			{
				UG::DrawString("8.", 267, (int)(iScreenHeight - 10 * iSpriteHeight), 0, grey);
				DrawLargeNumberHS(iaHighScores[2], 267.0f + 3 * iSpriteWidth + 4, (float)(iScreenHeight - 9.75 * iSpriteHeight), (char)222, (char)222, (char)247);
				UG::DrawString(caHighScoreNames[2], 267 + 4 * iSpriteWidth, (int)(iScreenHeight - 10 * iSpriteHeight), 0, grey);
			}
			if (fHighscoresTimer2 > 0.9f)
			{
				UG::DrawString("9.", 267, (int)(iScreenHeight - 10.5 * iSpriteHeight), 0, grey);
				DrawLargeNumberHS(iaHighScores[1], 267.0f + 3 * iSpriteWidth + 4, (float)(iScreenHeight - 10.25 * iSpriteHeight), (char)222, (char)222, (char)247);
				UG::DrawString(caHighScoreNames[1], 267 + 4 * iSpriteWidth, (int)(iScreenHeight - 10.5 * iSpriteHeight), 0, grey);
			}
			if (fHighscoresTimer2 > 1.0f)
			{
				UG::DrawString("10.", 234, (int)(iScreenHeight - 11 * iSpriteHeight), 0, grey);
				DrawLargeNumberHS(iaHighScores[0], 267.0f + 3 * iSpriteWidth + 4, (float)(iScreenHeight - 10.75 * iSpriteHeight), (char)222, (char)222, (char)247);
				UG::DrawString(caHighScoreNames[0], 267 + 4 * iSpriteWidth, (int)(iScreenHeight - 11 * iSpriteHeight), 0, grey);
			}
#pragma endregion

#pragma region Exit
			// Quit out to the menu when escape is pressed.
			if (UG::WasKeyPressed(UG::KEY_ESCAPE)) 
			{
				fHighscoresTimer = 0.0f;
				fHighscoresTimer2 = 0.0f;
				eCurrentState = MENU;
			}
			break;
#pragma endregion
#pragma endregion

#pragma region HOWTOPLAY
		case HOWTOPLAY:
#pragma region First instruction
			// Draws the first instruction.
			if (bMovementInstruction)
			{
				UG::DrawString("Use        or", iSpriteWidth, iSpriteHeight * 7, 0, grey);
				UG::DrawString("W,A,S,D", (int)(2.9 * iSpriteWidth), iSpriteHeight * 7, 0, cyan);
				UG::DrawString("Arrow keys", iSpriteWidth, iSpriteHeight * 6, 0, cyan);
				UG::DrawString("to", (int)(iSpriteWidth * 6.5), iSpriteHeight * 6, 0, grey);
				UG::DrawString("move around", iSpriteWidth, iSpriteHeight * 5, 0, grey);

				fTargetYPosition = 3.5 * iSpriteHeight;				
			}

			// When frog reaches target, first instruction disappears and second one appears.
			if (sHTPFrog.yPos > 3 * iSpriteHeight && bMovementInstruction) 
			{
				bMovementInstruction = false;
				bCarInstruction = true;
			}
#pragma endregion

#pragma region Second instruction
			// Draws the second instruction.
			if (bCarInstruction) 
			{
				UG::DrawString("Avoid getting", iSpriteWidth, iSpriteHeight * 10, 0, grey);
				UG::DrawString("hit by", iSpriteWidth, iSpriteHeight * 9, 0, grey);
				UG::DrawString("cars", (int)(4.5 * iSpriteWidth), iSpriteHeight * 9, 0, cyan);

				fTargetYPosition = 7.5 * iSpriteHeight;
			}

			// When frog reaches target, second instruction disappears and third one appears.
			if (sHTPFrog.yPos > 7 * iSpriteHeight && bCarInstruction)
			{
				bCarInstruction = false;
				bLogInstruction = true;
			}
#pragma endregion

#pragma region Third instruction
			// Draws the third instruction.
			if (bLogInstruction) 
			{
				UG::DrawString("Use      to get", iSpriteWidth, iSpriteHeight * 14, 0, grey);
				UG::DrawString("Logs", 3 * iSpriteWidth, iSpriteHeight * 14, 0, cyan);
				UG::DrawString("across the river", iSpriteWidth, iSpriteHeight * 13, 0, grey);
				
				fTargetYPosition = 12.5 * iSpriteHeight;
			}

			// When frog reaches target, third instruction disappears and fourth one appears.
			if (sHTPFrog.yPos > 12 * iSpriteHeight && bLogInstruction)
			{
				bLogInstruction = false;
				bLilyPadInstruction = true;
			}
#pragma endregion

#pragma region Fourth instruction
			// Draws the fourth instruction.
			if (bLilyPadInstruction)
			{
				UG::DrawSprite(sHTPRiver2.iSpriteID);
				UG::DrawSprite(sHTPBush.iSpriteID);
				
				bDrawTarget = false;

				UG::DrawString("Land on a", iSpriteWidth, (int)(iSpriteHeight * 13.5 - 2), 0, grey);
				UG::DrawString("lily pad", iSpriteWidth, (int)(iSpriteHeight * 12.67), 0, cyan);
				UG::DrawString("and you're", iSpriteWidth, (int)(iSpriteHeight * 11.83 + 3), 0, grey);
				UG::DrawString("home", iSpriteWidth, iSpriteHeight * 11 + 5, 0, grey);
			}

			// When frog reaches lily pad, fourth instruction disappears and end screen appears.
			if (sPhantomFrog.xPos == sHTPLilyPad1.xPos && sPhantomFrog.yPos == sHTPLilyPad1.yPos && bLilyPadInstruction)
			{				
				bLilyPadInstruction = false;
				bComplete = true;
				fHTPTimer = 0.0f;				
			}
#pragma endregion
			
#pragma region End screen
			// Draws end screen.
			if (bComplete) 
			{
				UG::DrawSprite(sHTPFrogOnLilyPad.iSpriteID);

				// First line of text appears instantly, then the next blocks of text appear after every 0.5 seconds.
				UG::DrawString("Congratulations!", 189, (int)(iScreenHeight - 3.5 * iSpriteHeight), 0, red);
								
				if (fHTPTimer > 0.5f) 
				{
					UG::DrawString("Get all 5 frogs home", iSpriteWidth * 2, (int)(iScreenHeight - 5.5 * iSpriteHeight), 0, magenta);
					UG::DrawString("safely for bonus", iSpriteWidth * 2, (int)(iScreenHeight - 6.5 * iSpriteHeight), 0, magenta);
					UG::DrawString("points.", iSpriteWidth * 2, (int)(iScreenHeight - 7.5 * iSpriteHeight), 0, magenta);
				}

				if (fHTPTimer > 1.0f) 
				{
					UG::DrawString("Press ESC to return", iSpriteWidth * 2, (int)(iScreenHeight - 9.5 * iSpriteHeight), 0, yellow);
					UG::DrawString("To the menu or", iSpriteWidth * 2, (int)(iScreenHeight - 10.5 * iSpriteHeight), 0, yellow);
					UG::DrawString("Press P to play", iSpriteWidth * 2, (int)(iScreenHeight - 11.5 * iSpriteHeight), 0, yellow);
					UG::DrawString("The full game", iSpriteWidth * 2, (int)(iScreenHeight - 12.5 * iSpriteHeight), 0, yellow);
				}

			}
#pragma endregion
			
#pragma region Car
			// Draws the car once the car instruction is reached.
			if (bCarInstruction || bLogInstruction || bLilyPadInstruction) 
			{
				sHTPCar.xPos -= 2.0f;

				if (sHTPCar.xPos > iScreenWidth + iSpriteWidth || sHTPCar.xPos < 1 * -iSpriteWidth)
				{
					sHTPCar.xPos -= sHTPCar.xPos / abs(sHTPCar.xPos) * (iScreenWidth + 2 * iSpriteWidth);
				}

				UG::SetSpritePosition(sHTPCar.iSpriteID, sHTPCar.xPos, sHTPCar.yPos);
				UG::DrawSprite(sHTPCar.iSpriteID);
			}
#pragma endregion

#pragma region River
			// Draws the log and river once the log instruction is reached.
			if (bLogInstruction || bLilyPadInstruction)
			{				
				UG::DrawSprite(sHTPRiver1.iSpriteID); 
				
				sHTPLog.xPos -= 2.0f;

				if (sHTPLog.xPos > iScreenWidth + 2 * iSpriteWidth || sHTPLog.xPos < 2 * -iSpriteWidth)
				{
					sHTPLog.xPos -= sHTPLog.xPos / abs(sHTPLog.xPos) * (iScreenWidth + 4 * iSpriteWidth);
				}

				UG::SetSpritePosition(sHTPLog.iSpriteID, sHTPLog.xPos, sHTPLog.yPos);
				UG::DrawSprite(sHTPLog.iSpriteID);
			}
#pragma endregion

#pragma region Lily pad
			// Draws and animates the lily pad once the lily pad instruction is reached.
			if (bLilyPadInstruction || bComplete)
			{
				if ((int)(2 * fHTPTimer) % 2 == 0)
				{
					UG::DrawSprite(sHTPLilyPad1.iSpriteID);
				}
				else
				{
					UG::DrawSprite(sHTPLilyPad2.iSpriteID);
				}
			}
#pragma endregion

#pragma region User input
			// Changes player location upon user input and sets screen bounds.
			if (fMovementTimer >= .25f && !bComplete && fDeathTimer >= 1.0f)
			{
				if ((UG::WasKeyPressed(UG::KEY_UP) || UG::WasKeyPressed(UG::KEY_W)) && sHTPFrog.yPos < iScreenHeight - 2 * iSpriteHeight)
				{
					sHTPFrog.yPos += iSpriteHeight;
					fMovementTimer = 0;
					fMoveUpTimer = 0;
					eCurrentSprite = UP;
				}
				if ((UG::WasKeyPressed(UG::KEY_DOWN) || UG::WasKeyPressed(UG::KEY_S)) && sHTPFrog.yPos > iSpriteHeight)
				{
					sHTPFrog.yPos -= iSpriteHeight;
					fMovementTimer = 0;
					fMoveDownTimer = 0;
					eCurrentSprite = DOWN;
				}
				if ((UG::WasKeyPressed(UG::KEY_LEFT) || UG::WasKeyPressed(UG::KEY_A)) && sHTPFrog.xPos > iSpriteWidth)
				{
					sHTPFrog.xPos -= iSpriteWidth;
					fMovementTimer = 0;
					fMoveLeftTimer = 0;
					eCurrentSprite = LEFT;
				}
				if ((UG::WasKeyPressed(UG::KEY_RIGHT) || UG::WasKeyPressed(UG::KEY_D)) && sHTPFrog.xPos < iScreenWidth - iSpriteWidth)
				{
					sHTPFrog.xPos += iSpriteWidth;
					fMovementTimer = 0;
					fMoveRightTimer = 0;
					eCurrentSprite = RIGHT;
				}
			}			

			// Update phantom frog position
			sPhantomFrog = sHTPFrog;

			// Controls the movement timers.
			if (fMovementTimer < .25f && fDeathTimer >= 1.0f)
			{
				if (fMoveUpTimer < .25f)
				{
					sPhantomFrog.yPos = sHTPFrog.yPos - iSpriteHeight * 4 * (.25f - fMovementTimer);

					fMoveUpTimer += UG::GetDeltaTime();
				}
				if (fMoveDownTimer < .25f)
				{
					sPhantomFrog.yPos = sHTPFrog.yPos + iSpriteHeight * 4 * (.25f - fMovementTimer);

					fMoveDownTimer += UG::GetDeltaTime();
				}
				if (fMoveLeftTimer < .25f)
				{
					sPhantomFrog.xPos = sHTPFrog.xPos + iSpriteHeight * 4 * (.25f - fMovementTimer);

					fMoveLeftTimer += UG::GetDeltaTime();
				}
				if (fMoveRightTimer < .25f)
				{
					sPhantomFrog.xPos = sHTPFrog.xPos - iSpriteHeight * 4 * (.25f - fMovementTimer);

					fMoveRightTimer += UG::GetDeltaTime();
				}

				fMovementTimer += UG::GetDeltaTime();
			}
#pragma endregion

#pragma region Correct player position upon exiting river
			// Moves player onto a sprite position upon exiting the river.
			if (sHTPFrog.yPos == 11.5 * iSpriteHeight || sHTPFrog.yPos == 9.5 * iSpriteHeight)
			{
				int offset = ((int)sHTPFrog.xPos) % iSpriteWidth;

				if (offset <= 32) //rounds the correction to the left or right
				{
					sHTPFrog.xPos -= offset;
				}
				else
				{
					sHTPFrog.xPos += iSpriteWidth - offset;
				}
			}
#pragma endregion

#pragma region Draw target
			// Draws pulsing target.
			if (bDrawTarget)
			{
				UG::DrawLine(iSpriteWidth * 7 - iSpriteWidth / 2 * (fHTPTimer - (int)fHTPTimer), fTargetYPosition - iSpriteWidth / 2 * (fHTPTimer - (int)fHTPTimer), iSpriteWidth * 7 + iSpriteWidth / 2 * (fHTPTimer - (int)fHTPTimer), fTargetYPosition - iSpriteWidth / 2 * (fHTPTimer - (int)fHTPTimer), 1.0f, 0, UG::SColour(0, 255, 0, (char)(255 * (1 - (fHTPTimer - (int)fHTPTimer)))));
				UG::DrawLine(iSpriteWidth * 7 - iSpriteWidth / 2 * (fHTPTimer - (int)fHTPTimer), fTargetYPosition + iSpriteWidth / 2 * (fHTPTimer - (int)fHTPTimer), iSpriteWidth * 7 + iSpriteWidth / 2 * (fHTPTimer - (int)fHTPTimer), fTargetYPosition + iSpriteWidth / 2 * (fHTPTimer - (int)fHTPTimer), 1.0f, 0, UG::SColour(0, 255, 0, (char)(255 * (1 - (fHTPTimer - (int)fHTPTimer)))));
				UG::DrawLine(iSpriteWidth * 7 - iSpriteWidth / 2 * (fHTPTimer - (int)fHTPTimer), fTargetYPosition - iSpriteWidth / 2 * (fHTPTimer - (int)fHTPTimer), iSpriteWidth * 7 - iSpriteWidth / 2 * (fHTPTimer - (int)fHTPTimer), fTargetYPosition + iSpriteWidth / 2 * (fHTPTimer - (int)fHTPTimer), 1.0f, 0, UG::SColour(0, 255, 0, (char)(255 * (1 - (fHTPTimer - (int)fHTPTimer)))));
				UG::DrawLine(iSpriteWidth * 7 + iSpriteWidth / 2 * (fHTPTimer - (int)fHTPTimer), fTargetYPosition - iSpriteWidth / 2 * (fHTPTimer - (int)fHTPTimer), iSpriteWidth * 7 + iSpriteWidth / 2 * (fHTPTimer - (int)fHTPTimer), fTargetYPosition + iSpriteWidth / 2 * (fHTPTimer - (int)fHTPTimer), 1.0f, 0, UG::SColour(0, 255, 0, (char)(255 * (1 - (fHTPTimer - (int)fHTPTimer)))));
				
				UG::DrawLine(iSpriteWidth * 7 - iSpriteWidth / 2 * (fHTPTimer + .5f - (int)(fHTPTimer + .5f)), fTargetYPosition - iSpriteWidth / 2 * (fHTPTimer + .5f - (int)(fHTPTimer + .5f)), iSpriteWidth * 7 + iSpriteWidth / 2 * (fHTPTimer + .5f - (int)(fHTPTimer + .5f)), fTargetYPosition - iSpriteWidth / 2 * (fHTPTimer + .5f - (int)(fHTPTimer + .5f)), 1.0f, 0, UG::SColour(0, 255, 0, (char)(255 * (1 - (fHTPTimer + .5 - (int)(fHTPTimer + .5))))));
				UG::DrawLine(iSpriteWidth * 7 - iSpriteWidth / 2 * (fHTPTimer + .5f - (int)(fHTPTimer + .5f)), fTargetYPosition + iSpriteWidth / 2 * (fHTPTimer + .5f - (int)(fHTPTimer + .5f)), iSpriteWidth * 7 + iSpriteWidth / 2 * (fHTPTimer + .5f - (int)(fHTPTimer + .5f)), fTargetYPosition + iSpriteWidth / 2 * (fHTPTimer + .5f - (int)(fHTPTimer + .5f)), 1.0f, 0, UG::SColour(0, 255, 0, (char)(255 * (1 - (fHTPTimer + .5 - (int)(fHTPTimer + .5))))));
				UG::DrawLine(iSpriteWidth * 7 - iSpriteWidth / 2 * (fHTPTimer + .5f - (int)(fHTPTimer + .5f)), fTargetYPosition - iSpriteWidth / 2 * (fHTPTimer + .5f - (int)(fHTPTimer + .5f)), iSpriteWidth * 7 - iSpriteWidth / 2 * (fHTPTimer + .5f - (int)(fHTPTimer + .5f)), fTargetYPosition + iSpriteWidth / 2 * (fHTPTimer + .5f - (int)(fHTPTimer + .5f)), 1.0f, 0, UG::SColour(0, 255, 0, (char)(255 * (1 - (fHTPTimer + .5 - (int)(fHTPTimer + .5))))));
				UG::DrawLine(iSpriteWidth * 7 + iSpriteWidth / 2 * (fHTPTimer + .5f - (int)(fHTPTimer + .5f)), fTargetYPosition - iSpriteWidth / 2 * (fHTPTimer + .5f - (int)(fHTPTimer + .5f)), iSpriteWidth * 7 + iSpriteWidth / 2 * (fHTPTimer + .5f - (int)(fHTPTimer + .5f)), fTargetYPosition + iSpriteWidth / 2 * (fHTPTimer + .5f - (int)(fHTPTimer + .5f)), 1.0f, 0, UG::SColour(0, 255, 0, (char)(255 * (1 - (fHTPTimer + .5 - (int)(fHTPTimer + .5))))));
			}
#pragma endregion
			
#pragma region Death
			// Draws death animations.
			if (fDeathTimer < 1) 
			{
				if (bDrowned) 
				{
					sDrown1.xPos = sHTPFrog.xPos;
					sDrown1.yPos = sHTPFrog.yPos;

					if (fDeathTimer < .33f) 
					{
						UG::SetSpritePosition(sDrown1.iSpriteID, sDrown1.xPos, sDrown1.yPos);
						UG::DrawSprite(sDrown1.iSpriteID);
					}
					else if (fDeathTimer < .67f) 
					{
						UG::SetSpritePosition(sDrown2.iSpriteID, sDrown1.xPos, sDrown1.yPos);
						UG::DrawSprite(sDrown2.iSpriteID);
					}
					else 
					{
						UG::SetSpritePosition(sDrown3.iSpriteID, sDrown1.xPos, sDrown1.yPos);
						UG::DrawSprite(sDrown3.iSpriteID);
					}
				}
				else if (bHitByCar)
				{
					sHitByCar1.xPos = sHTPFrog.xPos;
					sHitByCar1.yPos = sHTPFrog.yPos;

					if (fDeathTimer < .25f)
					{
						UG::SetSpritePosition(sHitByCar1.iSpriteID, sHitByCar1.xPos, sHitByCar1.yPos);
						UG::DrawSprite(sHitByCar1.iSpriteID);
					}
					else if (fDeathTimer < .5f)
					{
						UG::SetSpritePosition(sHitByCar2.iSpriteID, sHitByCar1.xPos, sHitByCar1.yPos);
						UG::DrawSprite(sHitByCar2.iSpriteID);
					}
					else if (fDeathTimer < .75f)
					{
						UG::SetSpritePosition(sHitByCar3.iSpriteID, sHitByCar1.xPos, sHitByCar1.yPos);
						UG::DrawSprite(sHitByCar3.iSpriteID);
					}
					else
					{
						UG::SetSpritePosition(sHitByCar4.iSpriteID, sHitByCar1.xPos, sHitByCar1.yPos);
						UG::DrawSprite(sHitByCar4.iSpriteID);
					}
				}

				fDeathTimer += UG::GetDeltaTime();
			}
			// Resets game values
			else if (bIsFrogDead) 
			{
				sHTPFrog.xPos = 7 * iSpriteWidth;
				sHTPFrog.yPos = 1.5 * iSpriteHeight;
				bIsFrogDead = false;
				bDrowned = false;
				bHitByCar = false;
			}
			// Defines death conditions
			else if (!bComplete)
			{
				// Floating off-screen.
				if (sHTPFrog.xPos < 0 || sHTPFrog.xPos > 0)
				{
					bIsFrogDead;
				}
				// Not landing on the lily pad.
				if (sPhantomFrog.xPos != sHTPLilyPad1.xPos && sPhantomFrog.yPos == sHTPLilyPad1.yPos && bLilyPadInstruction)
				{
					bIsFrogDead = true;
					bDrowned = true;
					fDeathTimer = 0;
				}
				
				// Drowning in river.
				else if (sPhantomFrog.yPos == sHTPLog.yPos)
				{
					// Updates player position if player lands on log.
					if (sHTPFrog.xPos >= sHTPLog.xPos - 84 && sHTPFrog.xPos <= sHTPLog.xPos + 84)
						sHTPFrog.xPos -= 2;
					// Kills player if player lands in river.
					else 
					{
						bIsFrogDead = true;
						fDeathTimer = 0;
						bDrowned = true;
					}
				}
				// Hit by car
				else if (sPhantomFrog.yPos == sHTPCar.yPos && sHTPFrog.xPos >= sHTPCar.xPos - 48 && sHTPFrog.xPos <= sHTPCar.xPos + 48)
				{
					bIsFrogDead = true;
					fDeathTimer = 0;
					bHitByCar = true;
				}

				// Draws frog.
				switch (eCurrentSprite)
				{
				case UP:
					if (fMovementTimer < .25)
					{
						UG::SetSpritePosition(sFrogUpJump.iSpriteID, sPhantomFrog.xPos, sPhantomFrog.yPos);
						UG::DrawSprite(sFrogUpJump.iSpriteID);
					}
					else
					{
						UG::SetSpritePosition(sFrogUp.iSpriteID, sPhantomFrog.xPos, sPhantomFrog.yPos);
						UG::DrawSprite(sFrogUp.iSpriteID);
					}
					break;
				case DOWN:
					if (fMovementTimer < .25)
					{
						UG::SetSpritePosition(sFrogDownJump.iSpriteID, sPhantomFrog.xPos, sPhantomFrog.yPos);
						UG::DrawSprite(sFrogDownJump.iSpriteID);
					}
					else
					{
						UG::SetSpritePosition(sFrogDown.iSpriteID, sPhantomFrog.xPos, sPhantomFrog.yPos);
						UG::DrawSprite(sFrogDown.iSpriteID);
					}
					break;
				case LEFT:
					if (fMovementTimer < .25)
					{
						UG::SetSpritePosition(sFrogLeftJump.iSpriteID, sPhantomFrog.xPos, sPhantomFrog.yPos);
						UG::DrawSprite(sFrogLeftJump.iSpriteID);
					}
					else
					{
						UG::SetSpritePosition(sFrogLeft.iSpriteID, sPhantomFrog.xPos, sPhantomFrog.yPos);
						UG::DrawSprite(sFrogLeft.iSpriteID);
					}
					break;
				case RIGHT:
					if (fMovementTimer < .25)
					{
						UG::SetSpritePosition(sFrogRightJump.iSpriteID, sPhantomFrog.xPos, sPhantomFrog.yPos);
						UG::DrawSprite(sFrogRightJump.iSpriteID);
					}
					else
					{
						UG::SetSpritePosition(sFrogRight.iSpriteID, sPhantomFrog.xPos, sPhantomFrog.yPos);
						UG::DrawSprite(sFrogRight.iSpriteID);
					}
					break;
				};
			}
#pragma endregion

#pragma region Draw welcome message
			// Draws the welcome message up until the lily pad instruction.
			if (!(bLilyPadInstruction || bComplete)) 
			{
				UG::DrawString("Welcome to Frogger!", 139, iScreenHeight - iSpriteHeight, 0, yellow);
			}
#pragma endregion

#pragma region Increment How to play timer
			fHTPTimer += UG::GetDeltaTime();
#pragma endregion

#pragma region Quit
			// Quit out to the menu when ESC is pressed or switch to play mode when P is pressed and tutorial is complete (also resets values.).
			if (UG::WasKeyPressed(UG::KEY_ESCAPE))
			{
				eCurrentState = MENU; 
				sHTPCar.xPos = iScreenWidth + iSpriteWidth;
				sHTPLog.xPos = iScreenWidth + 2 * iSpriteWidth;
				sHTPFrog.xPos = 7 * iSpriteWidth;
				sHTPFrog.yPos = 1.5 * iSpriteHeight;
				eCurrentSprite = UP;
				bMovementInstruction = true;
				bCarInstruction = false;
				bLogInstruction = false;
				bLilyPadInstruction = false;
				bComplete = false;
				fHTPTimer = 0;
				fTargetYPosition = 3.5 * iSpriteHeight;
				bDrawTarget = true;
			}
			if (UG::WasKeyPressed(UG::KEY_P) && bComplete) 
			{
				eCurrentState = PLAY;
				sHTPCar.xPos = iScreenWidth + iSpriteWidth;
				sHTPLog.xPos = iScreenWidth + 2 * iSpriteWidth;
				sHTPFrog.xPos = 7 * iSpriteWidth;
				sHTPFrog.yPos = 1.5 * iSpriteHeight;
				eCurrentSprite = UP;
				bMovementInstruction = true;
				bCarInstruction = false;
				bLogInstruction = false;
				bLilyPadInstruction = false;
				bComplete = false;
				fHTPTimer = 0;
				fTargetYPosition = 3.5 * iSpriteHeight;
				bDrawTarget = true;
			}
#pragma endregion
			break;

#pragma endregion
		}
		
		UG::ClearScreen();		

	} while (UG::Process());

	//Close down the framework by calling dispose.
	UG::Dispose();

	return 0;
}

#pragma region Defining methods
#pragma region DrawCharacter()
// Fn that draws characters based on a character input with optional co-ordinate positions and RGB colour values.
void DrawCharacter(char character, float xStart, float yStart, char r, char g, char b, char a)
{
	if (tolower(character) == 'c')
	{
		UG::DrawLine(xStart + 8, yStart + 8, xStart + 12, yStart + 8, 4.0f, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart - 10, yStart + 10, xStart + 10, yStart + 10, 4, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart - 10, yStart + 10, xStart - 10, yStart - 10, 4, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart - 10, yStart - 10, xStart + 10, yStart - 10, 4, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart + 8, yStart - 8, xStart + 12, yStart - 8, 4, 1, UG::SColour(r, g, b, a));

	}
	else if (tolower(character) == 'e')
	{
		UG::DrawLine(xStart + 8, yStart + 8, xStart + 12, yStart + 8, 4, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart - 10, yStart + 10, xStart - 10, yStart - 10, 4, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart - 10, yStart + 10, xStart + 10, yStart + 10, 4, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart - 10, yStart, xStart + 10, yStart, 4, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart - 10, yStart - 10, xStart + 10, yStart - 10, 4, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart + 8, yStart - 8, xStart + 12, yStart - 8, 4, 1, UG::SColour(r, g, b, a));
	}
	else if (tolower(character) == 'i')
	{
		UG::DrawLine(xStart, yStart + 10, xStart, yStart - 10, 4, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart - 10, yStart + 10, xStart + 10, yStart + 10, 4, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart - 10, yStart - 10, xStart + 10, yStart - 10, 4, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart - 8, yStart + 8, xStart - 12, yStart + 8, 4, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart + 8, yStart + 8, xStart + 12, yStart + 8, 4, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart - 8, yStart - 8, xStart - 12, yStart - 8, 4, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart + 8, yStart - 8, xStart + 12, yStart - 8, 4, 1, UG::SColour(r, g, b, a));
	}
	else if (tolower(character) == 'l')
	{
		UG::DrawLine(xStart - 10, yStart - 10, xStart + 10, yStart - 10, 4, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart - 10, yStart + 12, xStart - 10, yStart - 12, 4, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart + 8, yStart - 8, xStart + 12, yStart - 8, 4, 1, UG::SColour(r, g, b, a));
	}
	else if (tolower(character) == 'm')
	{
		UG::DrawLine(xStart - 10, yStart + 10, xStart + 10, yStart + 10, 4, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart - 10, yStart + 10, xStart - 10, yStart - 10, 4, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart + 10, yStart + 10, xStart + 10, yStart - 10, 4, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart, yStart + 10, xStart, yStart - 10, 4, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart - 6, yStart - 10, xStart - 10, yStart - 10, 4, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart + 6, yStart - 10, xStart + 10, yStart - 10, 4, 1, UG::SColour(r, g, b, a));
	}
	else if (tolower(character) == 'o')
	{
		UG::DrawLine(xStart - 10, yStart + 10, xStart + 10, yStart + 10, 4, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart - 10, yStart + 10, xStart - 10, yStart - 10, 4, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart - 10, yStart - 10, xStart + 10, yStart - 10, 4, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart + 10, yStart + 10, xStart + 10, yStart - 10, 4, 1, UG::SColour(r, g, b, a));
	}
	else if (tolower(character) == 'r')
	{
		UG::DrawLine(xStart - 10, yStart + 10, xStart + 10, yStart + 10, 4, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart - 10, yStart + 10, xStart - 10, yStart - 12, 4, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart + 10, yStart + 10, xStart + 10, yStart, 4, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart - 10, yStart, xStart + 10, yStart, 4, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart, yStart - 1, xStart + 10, yStart - 11, 4, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart + 8, yStart - 10, xStart + 12, yStart - 10, 4, 1, UG::SColour(r, g, b, a));

	}
	else if (tolower(character) == 's')
	{
		UG::DrawLine(xStart + 8, yStart + 8, xStart + 12, yStart + 8, 4, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart - 10, yStart + 10, xStart + 10, yStart + 10, 4, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart - 10, yStart + 10, xStart - 10, yStart, 4, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart - 10, yStart, xStart + 10, yStart, 4, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart + 10, yStart, xStart + 10, yStart - 10, 4, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart - 10, yStart - 10, xStart + 10, yStart - 10, 4, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart - 8, yStart - 8, xStart - 12, yStart - 8, 4, 1, UG::SColour(r, g, b, a));
	}
	else if (tolower(character) == 't')
	{
		UG::DrawLine(xStart - 10, yStart + 10, xStart + 10, yStart + 10, 4, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart, yStart + 10, xStart, yStart - 12, 4, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart - 8, yStart + 8, xStart - 12, yStart + 8, 4, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart + 8, yStart + 8, xStart + 12, yStart + 8, 4, 1, UG::SColour(r, g, b, a));
	}
	else if (tolower(character) == 'v')
	{
		UG::DrawLine(xStart - 10, yStart + 12, xStart - 10, yStart, 4, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart - 10, yStart + 2, xStart, yStart - 11, 4, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart - 2, yStart - 10, xStart + 2, yStart - 10, 4, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart + 10, yStart + 2, xStart, yStart - 11, 4, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart + 10, yStart + 12, xStart + 10, yStart, 4, 1, UG::SColour(r, g, b, a));
	}
	else if (tolower(character) == '+')
	{
		UG::DrawLine(xStart - 10, yStart, xStart + 10, yStart, 4, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart, yStart + 10, xStart, yStart - 10, 4, 1, UG::SColour(r, g, b, a));
	}
}
#pragma endregion

#pragma region DrawNumber()
// Fn that draws numbers based on an integer input with optional co-ordinate positions and RGB colour values.
void DrawNumber(int number, float xStart, float yStart, char r, char g, char b, char a)
{
	if (number == 0)
	{
		UG::DrawLine(xStart - 10, yStart + 10, xStart + 10, yStart + 10, 4, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart - 10, yStart + 10, xStart - 10, yStart - 10, 4, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart - 10, yStart - 10, xStart + 10, yStart - 10, 4, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart + 10, yStart + 10, xStart + 10, yStart - 10, 4, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart - 10, yStart - 10, xStart + 10, yStart + 10, 4, 1, UG::SColour(r, g, b, a));
	}
	else if (number == 1)
	{
		UG::DrawLine(xStart, yStart + 12, xStart, yStart - 12, 4, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart - 10, yStart - 10, xStart + 10, yStart - 10, 4, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart, yStart + 11, xStart - 8, yStart + 1, 4, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart - 8, yStart + 2, xStart - 8, yStart - 2, 4, 1, UG::SColour(r, g, b, a));
	}
	else if (number == 2)
	{
		UG::DrawLine(xStart - 8, yStart + 8, xStart - 12, yStart + 8, 4, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart + 10, yStart + 10, xStart - 10, yStart + 10, 4, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart + 10, yStart + 10, xStart + 10, yStart, 4, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart - 10, yStart, xStart + 10, yStart, 4, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart - 10, yStart, xStart - 10, yStart - 10, 4, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart - 12, yStart - 10, xStart + 12, yStart - 10, 4, 1, UG::SColour(r, g, b, a));
	}
	else if (number == 3)
	{
		UG::DrawLine(xStart - 8, yStart + 8, xStart - 12, yStart + 8, 4, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart - 10, yStart + 10, xStart + 10, yStart + 10, 4, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart + 10, yStart + 10, xStart + 10, yStart + 2, 4, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart - 10, yStart, xStart + 10, yStart, 4, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart + 10, yStart - 2, xStart + 10, yStart - 10, 4, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart - 10, yStart - 10, xStart + 10, yStart - 10, 4, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart - 8, yStart - 8, xStart - 12, yStart - 8, 4, 1, UG::SColour(r, g, b, a));
	}
	else if (number == 4)
	{
		UG::DrawLine(xStart - 10, yStart, xStart + 10, yStart, 4, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart, yStart + 12, xStart, yStart - 12, 4, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart - 10, yStart + 12, xStart - 10, yStart, 4, 1, UG::SColour(r, g, b, a));
	}
	else if (number == 5)
	{
		UG::DrawLine(xStart - 10, yStart + 10, xStart + 12, yStart + 10, 4, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart - 10, yStart + 12, xStart - 10, yStart, 4, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart - 10, yStart, xStart + 10, yStart, 4, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart + 10, yStart, xStart + 10, yStart - 10, 4, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart - 10, yStart - 10, xStart + 10, yStart - 10, 4, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart - 8, yStart - 8, xStart - 12, yStart - 8, 4, 1, UG::SColour(r, g, b, a));
	}
	else if (number == 6)
	{
		UG::DrawLine(xStart + 8, yStart + 8, xStart + 12, yStart + 8, 4, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart - 10, yStart + 10, xStart + 10, yStart + 10, 4, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart - 10, yStart + 10, xStart - 10, yStart - 10, 4, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart - 10, yStart, xStart + 10, yStart, 4, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart + 10, yStart, xStart + 10, yStart - 10, 4, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart - 10, yStart - 10, xStart + 10, yStart - 10, 4, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart - 8, yStart - 8, xStart - 12, yStart - 8, 4, 1, UG::SColour(r, g, b, a));
	}
	else if (number == 7)
	{
		UG::DrawLine(xStart - 10, yStart + 10, xStart + 10, yStart + 10, 4, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart + 8, yStart + 10, xStart, yStart - 11, 4, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart - 8, yStart + 8, xStart - 12, yStart + 8, 4, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart - 2, yStart - 10, xStart + 2, yStart - 10, 4, 1, UG::SColour(r, g, b, a));
	}
	else if (number == 8)
	{
		UG::DrawLine(xStart - 10, yStart + 10, xStart + 10, yStart + 10, 4, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart - 10, yStart + 10, xStart - 10, yStart + 2, 4, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart - 10, yStart - 2, xStart - 10, yStart - 10, 4, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart - 10, yStart, xStart + 10, yStart, 4, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart - 10, yStart - 10, xStart + 10, yStart - 10, 4, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart + 10, yStart + 10, xStart + 10, yStart + 2, 4, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart + 10, yStart - 2, xStart + 10, yStart - 10, 4, 1, UG::SColour(r, g, b, a));
	}
	else if (number == 9)
	{
		UG::DrawLine(xStart - 10, yStart + 10, xStart + 10, yStart + 10, 4, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart - 10, yStart + 10, xStart - 10, yStart, 4, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart - 10, yStart, xStart + 10, yStart, 4, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart - 10, yStart - 10, xStart + 10, yStart - 10, 4, 1, UG::SColour(r, g, b, a));
		UG::DrawLine(xStart + 10, yStart + 10, xStart + 10, yStart - 10, 4, 1, UG::SColour(r, g, b, a));
	}
}
#pragma endregion

#pragma region DrawLargeNumber()
// Fn that draws number with more than 1 digit and draws a '+' symbol before the number.
void DrawLargeNumber(int number, float xStart, float yStart, char r, char g, char b, char a)
{
	while (number != 0)
	{
		vScoreGain.push_back(number % 10);
		number /= 10;
	}

	for (int i = 0; i < vScoreGain.size(); i++)
	{
		DrawNumber(vScoreGain[i], xStart - i * (iSpriteWidth / 2), yStart, r, g, b, a);
	}

	DrawCharacter('+', xStart - vScoreGain.size() * (iSpriteWidth / 2), yStart, r, g, b, a);


	while (vScoreGain.size() > 0)
	{
		vScoreGain.pop_back();
	}
}
#pragma endregion

#pragma region DrawLargeNumberHS()
// Fn that draws number with more than 1 digit.
void DrawLargeNumberHS(int number, float xStart, float yStart, char r, char g, char b, char a)
{
	while (number != 0)
	{
		vScoreGain.push_back(number % 10);
		number /= 10;
	}

	for (int i = 0; i < vScoreGain.size(); i++)
	{
		DrawNumber(vScoreGain[i], xStart - i * (iSpriteWidth / 2), yStart, r, g, b, a);
	}

	while (vScoreGain.size() > 0)
	{
		vScoreGain.pop_back();
	}
}
#pragma endregion

#pragma region ScoreGain()
// Fn that displays the score gained in the last update descending from where the total score is displayed and fades after 1 second. 
void ScoreGain()
{
	// Draws score gain.
	if (fScoreTimer < 1)
	{
		DrawLargeNumber(iScoreGain, 4.5f * iSpriteWidth, iScreenHeight - 2 * iSpriteHeight / 2 - fScoreTimer * 2 * iSpriteHeight / 2, (char)220, (char)20, (char)20, char(128 - trunc(fScoreTimer / .5) * (fScoreTimer - .5) * 255));

		fScoreTimer += UG::GetDeltaTime();
	}
}
#pragma endregion
#pragma endregion
