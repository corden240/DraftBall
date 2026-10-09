#include <math.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>   /* srand, rand, atoi */
#include <time.h>     /* time() */
#include "main.h"
#include <string.h>

void HideCursor();
void UnHideCursor();
void Startup();
void MenuChoicesConstructor(struct menu_choices* menu);
int MenuOutcome(struct menu_choices* menu);
void ResponseConstructor(struct response* res);
void SetupGame(struct response* res);
void PlayGame(struct response* res);
void Settings();

/*
* NAME:        SetupWindow
* DESCRIPTION: opens window at certain size and sets font size
* INPUTS:      None
* OUTPUTS:     None
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
* INPUTS:      None
* OUTPUTS:     None
*/
void ExecuteMenuChoice(struct menu_choices* menu)
{
	do {
		int menu_outcome = MenuOutcome(menu);

		if (menu_outcome == 0) // play 1 game
		{
			struct response res;
			ResponseConstructor(&res);
			SetupGame(&res);
			PlayGame(&res);
		}
		else if (menu_outcome == 1) // play season
		{
		}
		else if (menu_outcome == 2) // play 2 min drive, down 6 points in the 4th? 2 timeouts, 5 plays, start at own 35?
		{
		}
		else if (menu_outcome == 3) // settings page
		{
			Settings();
		}
	} while (1);
}

/*
* NAME:        main
* DESCRIPTION: main game loop
* INPUTS:      None
* OUTPUTS:     None
*/
void main()
{
	SetupWindow();
	
	struct menu_choices menu;
	MenuChoicesConstructor(&menu);
	Startup(&menu);

	ExecuteMenuChoice(&menu);
}