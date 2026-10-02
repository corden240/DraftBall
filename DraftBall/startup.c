#include "main.h"
#include <windows.h>
//#include "gameconsole.c"

void ResetScreen();

int DraftBallTitle[GAME_BIT_FONT_HEIGHT][DRAFT_BALL_TITLE_WIDTH] =
{	//     D                     R                     A                       F                   T                     B                     A                     L                     L
	{1, 1, 1, 1, 0,   0,   1, 1, 1, 1, 0,   0,   0, 1, 1, 1, 0,   0,   1, 1, 1, 1, 1,   0,   1, 1, 1, 1, 1,   0,   1, 1, 1, 1, 0,   0,   0, 1, 1, 1, 0,   0,   1, 0, 0, 0, 0,   0,   1, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0,},
	{1, 0, 0, 0, 1,   0,   1, 0, 0, 0, 1,   0,   1, 0, 0, 0, 1,   0,   1, 0, 0, 0, 0,   0,   0, 0, 1, 0, 0,   0,   1, 0, 0, 0, 1,   0,   1, 0, 0, 0, 1,   0,   1, 0, 0, 0, 0,   0,   1, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0,},
	{1, 0, 0, 0, 1,   0,   1, 0, 0, 0, 1,   0,   1, 0, 0, 0, 1,   0,   1, 0, 0, 0, 0,   0,   0, 0, 1, 0, 0,   0,   1, 0, 0, 0, 1,   0,   1, 0, 0, 0, 1,   0,   1, 0, 0, 0, 0,   0,   1, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0,},
	{1, 0, 0, 0, 1,   0,   1, 1, 1, 1, 1,   0,   1, 1, 1, 1, 1,   0,   1, 1, 1, 1, 0,   0,   0, 0, 1, 0, 0,   0,   1, 1, 1, 1, 0,   0,   1, 1, 1, 1, 1,   0,   1, 0, 0, 0, 0,   0,   1, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0,},
	{1, 0, 0, 0, 1,   0,   1, 0, 1, 0, 0,   0,   1, 0, 0, 0, 1,   0,   1, 0, 0, 0, 0,   0,   0, 0, 1, 0, 0,   0,   1, 0, 0, 0, 1,   0,   1, 0, 0, 0, 1,   0,   1, 0, 0, 0, 0,   0,   1, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0,},
	{1, 0, 0, 0, 1,   0,   1, 0, 0, 1, 0,   0,   1, 0, 0, 0, 1,   0,   1, 0, 0, 0, 0,   0,   0, 0, 1, 0, 0,   0,   1, 0, 0, 0, 1,   0,   1, 0, 0, 0, 1,   0,   1, 0, 0, 0, 0,   0,   1, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0,},
	{1, 1, 1, 1, 0,   0,   1, 0, 0, 0, 1,   0,   1, 0, 0, 0, 1,   0,   1, 0, 0, 0, 0,   0,   0, 0, 1, 0, 0,   0,   1, 1, 1, 1, 0,   0,   1, 0, 0, 0, 1,   0,   1, 1, 1, 1, 1,   0,   1, 1, 1, 1, 1,   0, 0, 0, 0, 0, 0, 0,},
};

int DraftBallBall[GAME_BIT_FONT_HEIGHT][DRAFT_BALL_BALL_WIDTH] =
{
	{0, 0, 0, 2, 1, 1, 1, 1, 1, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
	{0, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,},
	{1, 1, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,},
	{1, 1, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,},
	{0, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,},
	{0, 0, 0, 2, 1, 1, 1, 1, 1, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
};

/*
* NAME:        ShowGameName
* DESCRIPTION: loading page large game name
* INPUTS:      frame number
* OUTPUTS:     None
*/
void ShowGameName(int frame_number)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	int buffer = 12;
	printf("\r\n\r\n");
	for (int x = 0; x < GAME_BIT_FONT_HEIGHT; x++)
	{
		for (int y = 0; y < buffer; y++)
		{
			printf(" ");
		}

		for (int i = 0; i < DRAFT_BALL_BALL_WIDTH; i++)
		{
			if (DraftBallBall[x][i] == 1)
			{
				SetConsoleTextAttribute(hConsole, CONSOLE_BG_YELLOW);
			}
			else if (DraftBallBall[x][i] == 2)
				SetConsoleTextAttribute(hConsole, CONSOLE_BG_WHITE);
			else
				SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
			printf(" ");
		}

		for (int i = 0; i < DRAFT_BALL_TITLE_WIDTH; i++)
		{
			if (DraftBallTitle[x][i] == 1)
			{
				if (frame_number % 10 > 4)
					SetConsoleTextAttribute(hConsole, CONSOLE_BG_BLUE);
				else
					SetConsoleTextAttribute(hConsole, CONSOLE_BG_RED);
			}
			else
				SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
			printf(" ");
		}

		for (int i = 0; i < DRAFT_BALL_BALL_WIDTH; i++)
		{
			if (DraftBallBall[x][i] == 1)
			{
				SetConsoleTextAttribute(hConsole, CONSOLE_BG_YELLOW);
			}
			else if (DraftBallBall[x][i] == 2)
				SetConsoleTextAttribute(hConsole, CONSOLE_BG_WHITE);
			else
				SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
			printf(" ");
		}

		printf("\r\n");
	}
	printf("\r\n");
}

/*
* NAME:        ShowLoadingStars
* DESCRIPTION: loading page stars
* INPUTS:      frame number
* OUTPUTS:     None
*/
void ShowLoadingStars(int frame_number)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	printf("\r\n");

	for (int i = 0; i < DRAFT_BALL_NUM_STAR_ROWS; i++)
	{
		for (int j = 0; j < 30; j++)
		{
			if ((i % (DRAFT_BALL_TITLE_UNIQUE_ROWS * 2)) < DRAFT_BALL_TITLE_UNIQUE_ROWS)
			{
				if ((j % 2 == 0 && frame_number % 2 == 0) || (j % 2 == 1 && frame_number % 2 == 1))
				{
					if (i % 2 == 0)
						SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_BLUE);
					else
						SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_RED);
				}
				else if (i % 2 == 0)
					SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_RED);
				else
					SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_BLUE);
			}
			else
			{
				if ((j % 2 == 1 && frame_number % 1 == 0) || (j % 2 == 0 && frame_number % 2 == 1))
				{
					if (i % 2 == 0)
						SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_BLUE);
					else
						SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_RED);
				}
				else if (i % 2 == 0)
					SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_RED);
				else
					SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_BLUE);
			}
			
			if(i % 2 == 0)
				printf("*   ");
			else
				printf("  * ");
		}
		printf("\r\n");
	}
	SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
}

/*
* NAME:        LoadingScreen
* DESCRIPTION: Animates opening of game
* INPUTS:      None
* OUTPUTS:     None
*/
void LoadingScreen()
{
	for (int x = 0; x < LOADING_SCREEN_ANIMATIONS; x++)
	{
		ResetScreen();
		ShowLoadingStars(x);	
		ShowGameName(x);
		ShowLoadingStars(x);
		Sleep(200);
	}
	system("cls");
}

/*
* NAME:        Startup
* DESCRIPTION: Opens the game with a loading, start and menu screen
* INPUTS:      None
* OUTPUTS:     None
*/
void Startup() {
	
	LoadingScreen();
}