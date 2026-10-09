#include <math.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>   /* srand, rand, atoi */
#include <time.h>     /* time() */
#include "main.h"
#include <string.h>

void ResetScreen();
void HideCursor();
void UnHideCursor();
void Startup();
int MenuOutcome(struct total_data* menu);
void ExhibitionGameDataConstructor(struct game_data* game_data);
void SetupExhibitionGame(struct game_data* game_data);
void PlayGame(struct game_data* game_data);
void Settings();
void PlaySeason(struct season_data* season_data);
void TwoMinDriveGameDataConstructor(struct game_data* game_data);
void SeasonDataConstructor(struct season_data* season_data);
void SetupTwoMinDrive(struct game_data* game_data);

/*
* NAME:        TotalDataConstructor
* DESCRIPTION: starts all of total_data data to default settings
* INPUTS:      game_data
* OUTPUTS:     none
*/
void TotalDataConstructor(struct total_data* total_data)
{
	total_data->fouls = 0;
	total_data->game_speed = 0;
	total_data->current_game_mode = 0;
	total_data->current_menu_selection = 0;
}

/*
* NAME:        SetupWindow
* DESCRIPTION: opens window at certain size and sets font size
* INPUTS:      none
* OUTPUTS:     none
*/
void SetupWindow()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SMALL_RECT windowSize = { 0, 0, WINDOW_HEIGHT - 1, WINDOW_WIDTH - 1 };
	COORD bufferSize = { WINDOW_HEIGHT , WINDOW_WIDTH };

	SetConsoleWindowInfo(hConsole, 1, &windowSize);
	SetConsoleScreenBufferSize(hConsole, bufferSize);

	CONSOLE_FONT_INFOEX fontInfo;
	fontInfo.cbSize = sizeof(CONSOLE_FONT_INFOEX);
	GetCurrentConsoleFontEx(hConsole, FALSE, &fontInfo);

	fontInfo.dwFontSize.X = 12;
	fontInfo.dwFontSize.Y = 24;
	SetCurrentConsoleFontEx(hConsole, FALSE, &fontInfo);

	HideCursor();
}

/*
* NAME:        ExecuteMenuChoice
* DESCRIPTION: play out each menu option
* INPUTS:      total_data
* OUTPUTS:     none
*/
void ExecuteMenuChoice(struct total_data* total_data)
{
	do {
		int menu_outcome = MenuOutcome(total_data);

		if (menu_outcome == 0) // play 1 game
		{
			total_data->current_game_mode = 0;
			system("cls");
			struct game_data game_data;
			SetupExhibitionGame(&game_data);
			PlayGame(&game_data);
		}
		else if (menu_outcome == 1) // play season
		{
			total_data->current_game_mode = 1;
			system("cls");
			struct season_data season_data;
			SeasonDataConstructor(&season_data);  // TODO: send total_data in with this to transfer settings in?
			SetupSeason(&season_data); // TODO: move contructor in here?
			PlaySeason(&season_data);
		}
		else if (menu_outcome == 2) // play 2 min drive, down 6 points in the 4th? 2 timeouts, 5 plays, start at own 35?
		{
			total_data->current_game_mode = 2;
			system("cls");
			struct game_data game_data;		
			SetupTwoMinDrive(&game_data);
			PlayGame(&game_data); // TODO: needs to leave if lose the ball, needs to be only one player
		}
		else if (menu_outcome == 3) // settings page
		{
			total_data->current_game_mode = 3;
			Settings();
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
	struct total_data total_data;
	TotalDataConstructor(&total_data);
	Startup(&total_data);

	ExecuteMenuChoice(&total_data);
}