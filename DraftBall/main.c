#include <math.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>   /* srand, rand, atoi */
#include <time.h>     /* time() */
#include "main.h"

void HideCursor();
void UnHideCursor();
void Startup();

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
* NAME:        main
* DESCRIPTION: main game loop
* INPUTS:      None
* OUTPUTS:     None
*/
void main()
{
	SetupWindow();
	
	Startup(); // Loading and intro page -> Homepage -> menu -> settings -> game modes

    struct response res;
    ResponseConstructor(&res);
    SetupGame(&res);
	
    PlayGame(&res);

	// end of game, back to menu, onto next game in season etc
}