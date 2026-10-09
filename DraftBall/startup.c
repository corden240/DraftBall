#include "main.h"
#include <windows.h>
#include <stdio.h>
#include <string.h>

void ResetScreen();

/*
* NAME:        PrintTextWithDelays
* DESCRIPTION: prints text given to function with a delay between each letter
* INPUTS:      text to display
* OUTPUTS:     none
*/
void PrintTextWithDelays(char txt[1000])
{
	for (int i = 0; i < MAX_STRING_lENGTH && txt[i] != '\0'; i++)
	{
		printf("%c", txt[i]);
		Sleep(10);
	}
}

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

int DraftBallBall1[GAME_BIT_FONT_HEIGHT][DRAFT_BALL_BALL_WIDTH] =
{
	{0, 0, 0, 2, 1, 1, 1, 1, 1, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
	{0, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,},
	{1, 1, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,},
	{1, 1, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,},
	{0, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,},
	{0, 0, 0, 2, 1, 1, 1, 1, 1, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
};

int DraftBallBall2[GAME_BIT_FONT_HEIGHT][DRAFT_BALL_BALL_WIDTH] =
{
	{0, 0, 0, 2, 1, 1, 1, 1, 1, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
	{0, 1, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,},
	{1, 1, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,},
	{1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,},
	{0, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,},
	{0, 0, 0, 2, 1, 1, 1, 1, 1, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
};

int DraftBallBall3[GAME_BIT_FONT_HEIGHT][DRAFT_BALL_BALL_WIDTH] =
{
	{0, 0, 0, 2, 1, 2, 1, 2, 1, 2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
	{0, 1, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,},
	{1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,},
	{1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,},
	{0, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,},
	{0, 0, 0, 2, 1, 1, 1, 1, 1, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
};

int DraftBallBall4[GAME_BIT_FONT_HEIGHT][DRAFT_BALL_BALL_WIDTH] =
{
	{0, 0, 0, 2, 1, 2, 1, 2, 1, 2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
	{0, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,},
	{1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,},
	{1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,},
	{0, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,},
	{0, 0, 0, 2, 1, 1, 1, 1, 1, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
};

int DraftBallBall5[GAME_BIT_FONT_HEIGHT][DRAFT_BALL_BALL_WIDTH] =
{
	{0, 0, 0, 2, 1, 1, 1, 1, 1, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
	{0, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,},
	{1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,},
	{1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,},
	{0, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,},
	{0, 0, 0, 2, 1, 1, 1, 1, 1, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
};

int DraftBallBall6[GAME_BIT_FONT_HEIGHT][DRAFT_BALL_BALL_WIDTH] =
{
	{0, 0, 0, 2, 1, 1, 1, 1, 1, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
	{0, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,},
	{1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,},
	{1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,},
	{0, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,},
	{0, 0, 0, 2, 1, 1, 1, 1, 1, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
};

int DraftBallBall7[GAME_BIT_FONT_HEIGHT][DRAFT_BALL_BALL_WIDTH] =
{
	{0, 0, 0, 2, 1, 1, 1, 1, 1, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
	{0, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,},
	{1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,},
	{1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,},
	{0, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,},
	{0, 0, 0, 2, 1, 2, 1, 2, 1, 2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
};

int DraftBallBall8[GAME_BIT_FONT_HEIGHT][DRAFT_BALL_BALL_WIDTH] =
{
	{0, 0, 0, 2, 1, 1, 1, 1, 1, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
	{0, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,},
	{1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,},
	{1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,},
	{0, 1, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,},
	{0, 0, 0, 2, 1, 2, 1, 2, 1, 2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
};

int DraftBallBall9[GAME_BIT_FONT_HEIGHT][DRAFT_BALL_BALL_WIDTH] =
{
	{0, 0, 0, 2, 1, 1, 1, 1, 1, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
	{0, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,},
	{1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,},
	{1, 1, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,},
	{0, 1, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,},
	{0, 0, 0, 2, 1, 1, 1, 1, 1, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
};

int (*DraftBallBalls[DRAFT_BALL_BALL_FRAMES])[DRAFT_BALL_BALL_WIDTH] =
{
	&DraftBallBall1,
	&DraftBallBall2,
	&DraftBallBall3,
	&DraftBallBall4,
	&DraftBallBall5,
	&DraftBallBall6,
	&DraftBallBall7,
	&DraftBallBall8,
	&DraftBallBall9,
};

/*
* NAME:        ShowGameName
* DESCRIPTION: loading page large game name
* INPUTS:      frame number
* OUTPUTS:     none
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
			if (DraftBallBalls[frame_number % DRAFT_BALL_BALL_FRAMES][x][i] == 1)
			{
				SetConsoleTextAttribute(hConsole, CONSOLE_BG_YELLOW);
			}
			else if (DraftBallBalls[frame_number % DRAFT_BALL_BALL_FRAMES][x][i] == 2)
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
			if (DraftBallBalls[frame_number % DRAFT_BALL_BALL_FRAMES][x][i] == 1)
			{
				SetConsoleTextAttribute(hConsole, CONSOLE_BG_YELLOW);
			}
			else if (DraftBallBalls[frame_number % DRAFT_BALL_BALL_FRAMES][x][i] == 2)
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
* NAME:        ShowMenuTransitionGameName
* DESCRIPTION: game name move for menu	
* INPUTS:      frame number
* OUTPUTS:     none
*/
void ShowMenuTransitionGameName(int frame_number)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	int buffer = 12, height_buffer = (frame_number > 7) ? 1 : 8 - frame_number;

	for (int i = 0; i < height_buffer; i++)
	{
		printf("\r\n");
	}

	for (int x = 0; x < GAME_BIT_FONT_HEIGHT; x++)
	{
		SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);

		for (int y = 0; y < buffer; y++)
		{
			printf(" ");
		}

		for (int i = 0; i < DRAFT_BALL_BALL_WIDTH; i++)
		{
			if (DraftBallBalls[frame_number % DRAFT_BALL_BALL_FRAMES][x][i] == 1)
			{
				SetConsoleTextAttribute(hConsole, CONSOLE_BG_YELLOW);
			}
			else if (DraftBallBalls[frame_number % DRAFT_BALL_BALL_FRAMES][x][i] == 2)
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
			if (DraftBallBalls[frame_number % DRAFT_BALL_BALL_FRAMES][x][i] == 1)
			{
				SetConsoleTextAttribute(hConsole, CONSOLE_BG_YELLOW);
			}
			else if (DraftBallBalls[frame_number % DRAFT_BALL_BALL_FRAMES][x][i] == 2)
				SetConsoleTextAttribute(hConsole, CONSOLE_BG_WHITE);
			else
				SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
			printf(" ");
		}

		printf("\r\n");
	}
	for (int i = 0; i < height_buffer; i++)
	{
		for (int j = 0; j < WINDOW_WIDTH; j++)
		{
			printf(" ");
		}
	}
	printf("\r\n");
}

/*
* NAME:        ShowMenuTitle
* DESCRIPTION: game name in menu
* INPUTS:      none
* OUTPUTS:     none
*/
void ShowMenuTitle()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	int buffer = 12;
	printf("\r\n");
	for (int x = 0; x < GAME_BIT_FONT_HEIGHT; x++)
	{
		SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);

		for (int y = 0; y < buffer; y++)
		{
			printf(" ");
		}

		for (int i = 0; i < DRAFT_BALL_BALL_WIDTH; i++)
		{
			if (DraftBallBalls[0][x][i] == 1)
			{
				SetConsoleTextAttribute(hConsole, CONSOLE_BG_YELLOW);
			}
			else if (DraftBallBalls[0][x][i] == 2)
				SetConsoleTextAttribute(hConsole, CONSOLE_BG_WHITE);
			else
				SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
			printf(" ");
		}

		for (int i = 0; i < DRAFT_BALL_TITLE_WIDTH; i++)
		{
			if (DraftBallTitle[x][i] == 1)
			{
				SetConsoleTextAttribute(hConsole, CONSOLE_BG_BLUE);
			}
			else
				SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
			printf(" ");
		}

		for (int i = 0; i < DRAFT_BALL_BALL_WIDTH; i++)
		{
			if (DraftBallBalls[0][x][i] == 1)
			{
				SetConsoleTextAttribute(hConsole, CONSOLE_BG_YELLOW);
			}
			else if (DraftBallBalls[0][x][i] == 2)
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
* NAME:        ShowMenuOptions
* DESCRIPTION: game menu options
* INPUTS:      menu choices
* OUTPUTS:     none
*/
void ShowMenuOptions(struct total_data* total_data, bool first_text)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	for (int i = 0; i < MENU_OPTIONS_NUMBER; i++)
	{
		printf("\r\n\r\n                                             ");
		if (i == 0)
		{
			printf("     ");
			if (total_data->current_menu_selection == i)//if this one is currently selected, print > and then console invert the text
				SetConsoleTextAttribute(hConsole, CONSOLE_BG_WHITE | CONSOLE_TEXT_GREEN);
			if(first_text)
				PrintTextWithDelays("Play Exhibition");
			else
				printf("Play Exhibition");
		}
		else if (i == 1)
		{
			printf("       ");
			if (total_data->current_menu_selection == i)//if this one is currently selected, print > and then console invert the text
				SetConsoleTextAttribute(hConsole, CONSOLE_BG_WHITE | CONSOLE_TEXT_GREEN);
			if (first_text)
				PrintTextWithDelays("Play Season");
			else
				printf("Play Season");
		}
		else if (i == 2)
		{
			printf("     ");
			if (total_data->current_menu_selection == i)//if this one is currently selected, print > and then console invert the text
				SetConsoleTextAttribute(hConsole, CONSOLE_BG_WHITE | CONSOLE_TEXT_GREEN);
			if (first_text)
				PrintTextWithDelays("Play 2 min drive");
			else
				printf("Play 2 min drive");
		}
		else if (i == 3)
		{
			printf("        ");
			if (total_data->current_menu_selection == i)//if this one is currently selected, print > and then console invert the text
				SetConsoleTextAttribute(hConsole, CONSOLE_BG_WHITE | CONSOLE_TEXT_GREEN);
			if (first_text)
				PrintTextWithDelays("Settings");
			else
				printf("Settings");
		}
		else if (i == 4)
		{
			printf("          ");
			if (total_data->current_menu_selection == i)//if this one is currently selected, print > and then console invert the text
				SetConsoleTextAttribute(hConsole, CONSOLE_BG_WHITE | CONSOLE_TEXT_GREEN);
			if (first_text)
				PrintTextWithDelays("Quit");
			else
				printf("Quit");
		}
		SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
	}
}

/*
* NAME:        ShowMenu
* DESCRIPTION: game menu
* INPUTS:      menu choices
* OUTPUTS:     none
*/
void ShowMenu(struct total_data* total_data, bool first_text)
{
	ShowMenuTitle();
	ShowMenuOptions(total_data, first_text);
}

/*
* NAME:        ShowLoadingStars
* DESCRIPTION: loading page stars
* INPUTS:      frame number
* OUTPUTS:     none
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
* INPUTS:      none
* OUTPUTS:     none
*/
void LoadingScreen()
{
	for (int x = 0; x < LOADING_SCREEN_ANIMATIONS; x++)
	{
		ResetScreen();
		ShowLoadingStars(x);	
		ShowGameName(x);
		ShowLoadingStars(x);
		Sleep(75);
	}
	system("cls");
}

/*
* NAME:        TransitionToMenuScreen
* DESCRIPTION: Animates from loading to menu
* INPUTS:      none
* OUTPUTS:     none
*/
void TransitionToMenuScreen()
{
	for (int x = 0; x < LOADING_TRANSITION_SCREEN_ANIMATIONS; x++)
	{
		ResetScreen();
		ShowMenuTransitionGameName(x);
		Sleep(75);
	}
	system("cls");
}

/*
* NAME:        MenuScreen
* DESCRIPTION: Menu
* INPUTS:      menu choices, first text
* OUTPUTS:     none
*/
void MenuScreen(struct total_data* total_data, bool first_text)
{
	ResetScreen();
	ShowMenu(total_data, first_text);
}

/*
* NAME:        Startup
* DESCRIPTION: Opens the game with a loading, start and menu screen
* INPUTS:      none
* OUTPUTS:     none
*/
void Startup(struct menu_choices* menu) {
	
	LoadingScreen();
	TransitionToMenuScreen();
	MenuScreen(menu, true);
}