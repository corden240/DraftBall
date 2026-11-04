#include <math.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>   /* srand, rand, atoi */
#include <time.h>     /* time() */
#include "main.h"
#include <string.h>
#include "teamdata.h"

void ResetScreen();
void HideCursor();
void UnHideCursor();
void Startup();
int MenuOutcome();
void ExhibitionGameDataConstructor(struct game_data* game_data);
int SetupExhibitionGame(struct game_data* game_data);
void PlayGame(struct game_data* game_data);
void Settings();
int SetupSeason(struct season_data* season_data);
void PlaySeason(struct season_data* season_data);
void TwoMinDriveGameDataConstructor(struct game_data* game_data);
void SeasonDataConstructor(struct season_data* season_data);
int SetupTwoMinDrive(struct game_data* game_data);
void HowToPlay();

/*
* NAME:        SetupWindow
* DESCRIPTION: opens window at certain size and sets font size
* INPUTS:      none
* OUTPUTS:     none
*/
void SetupWindow()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SMALL_RECT windowSize = { 0, 0, WINDOW_WIDTH - 1, WINDOW_HEIGHT - 1 };
	COORD bufferSize = { WINDOW_WIDTH, WINDOW_HEIGHT };

	SetConsoleWindowInfo(hConsole, 1, &windowSize);
	SetConsoleScreenBufferSize(hConsole, bufferSize);

	CONSOLE_FONT_INFOEX fontInfo;
	fontInfo.cbSize = sizeof(CONSOLE_FONT_INFOEX);
	GetCurrentConsoleFontEx(hConsole, FALSE, &fontInfo);

	fontInfo.dwFontSize.X = WINDOW_SCALE_X;
	fontInfo.dwFontSize.Y = WINDOW_SCALE_Y;
	SetCurrentConsoleFontEx(hConsole, FALSE, &fontInfo);

	HideCursor();
}

/*
* NAME:        ExecuteMenuChoice
* DESCRIPTION: play out each menu option
* INPUTS:      none
* OUTPUTS:     none
*/
void ExecuteMenuChoice()
{
	int menu_outcome = 0;
	do {
		menu_outcome = MenuOutcome(menu_outcome);
		system("cls");

		if (menu_outcome == 0) // play 1 game
		{
			struct game_data game_data;
			if (SetupExhibitionGame(&game_data) == 1)
				PlayGame(&game_data);
		}
		else if (menu_outcome == 1) // play season
		{
			struct season_data season_data;	
			if(SetupSeason(&season_data) == 1)
				PlaySeason(&season_data);
		}
		else if (menu_outcome == 2) // play 2 min drive
		{
			struct game_data game_data;	
			if (SetupTwoMinDrive(&game_data) == 1)
				PlayGame(&game_data);
		}
		else if (menu_outcome == 3) // how to play page
		{
			HowToPlay();
		}
		system("cls");
	} while (1);
}

/*
* NAME:        main
* DESCRIPTION: main game loop
* INPUTS:      none
* OUTPUTS:     none
*/
void main()
{
	srand((unsigned)time(NULL));
	SetupWindow();
	Startup();
	ExecuteMenuChoice();
}