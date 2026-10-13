#include <stdio.h>
#include <stdlib.h>   /* srand, rand, atoi */
#include <time.h>     /* time() */
#include "main.h"
#include <string.h>
#include <windows.h>

void ShowMenuTitle();
void ResetScreen();

/*
* NAME:        ShowGameStats
* DESCRIPTION: shows game stats
* INPUTS:      current selection, game_data
* OUTPUTS:     none
*/
void ShowGameStats(int selected, struct game_data* game_data)
{
	ResetScreen();
	ShowMenuTitle();
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
	printf("                                     ");
	if (selected == 0)
		SetConsoleTextAttribute(hConsole, CONSOLE_BG_WHITE);
	printf("Team 1 name");
	SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
	printf("   QTR   1   2   3   4   ");
	if (selected == 1)
		SetConsoleTextAttribute(hConsole, CONSOLE_BG_WHITE);
	printf("Team 2 name");
	SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
	printf("      \r\n");

	printf("                                        24 pts     T1    0   7   17  0      16 pts        \r\n");
	printf("                                        3-4-1      T2    7   3   0   6       4-4          \r\n");
	printf("                                                                                          \r\n");
	printf("                                                                                          \r\n");
	printf("                                     Passing     C/ATT   YDS  AVG  TD  INT  SACKS  RTG    \r\n");
	printf("                                     QB name     8/11    120  15   1   1    1-11   92     \r\n");
	printf("                                                                                          \r\n");
	printf("                                     Rushing     CAR  YDS  AVG  TD  LONG                  \r\n");
	printf("                                     RB1         7     50  7.1  1   15                    \r\n");
	printf("                                     RB2         6     21  3.5  0   8                     \r\n");
	printf("                                                                                          \r\n");
	printf("                                     Receiving   REC  YDS  AVG  TD  LONG  TGTS            \r\n");
	printf("                                     WR1         3    13   4.3  0    7     4              \r\n");
	printf("                                     WR2         3    34   9.3  1    17    3              \r\n");
	printf("                                     WR3         0    0    0.0  0    0     1              \r\n");
	printf("                                     WR4         2    9    4.5  0    7     3              \r\n");
	printf("                                                                                          \r\n");
	printf("                                                          ");
	if (selected == 2)
		SetConsoleTextAttribute(hConsole, CONSOLE_BG_WHITE);
	printf("Continue");
	SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
	printf("                        ");
}

/*
* NAME:        GameStatsMenuOutcome
* DESCRIPTION: Gets which entry picked
* INPUTS:      game_data
* OUTPUTS:     int choice
*/
int GameStatsMenuOutcome(struct game_data* game_data) {

	bool game_chosen = false;
	int current_menu_selection = 0;
	do {
		ShowGameStats(current_menu_selection, game_data);
		int ch = getch();

		if (ch == 0 || ch == 224) {   // extended key: read the scan code
			ch = getch();
			switch (ch) {
			case 72: // up arrow
				if (current_menu_selection == 2)
					current_menu_selection = 0;
				break;
			case 80: // down arrow
				if (current_menu_selection == 0 || current_menu_selection == 1)
					current_menu_selection = 2;
				break;
			case 75: // left arrow
				if (current_menu_selection == 1)
					current_menu_selection = 0;
				break;
			case 77: // right arrow
				if (current_menu_selection == 0)
					current_menu_selection = 1;
				break;
			default:
				break;
			}
		}
		else if (ch == 13 && current_menu_selection == (GAME_STATS_NUMBER - 1)) {// Enter key
			game_chosen = true;
			return current_menu_selection;
		}
		else
			;
		// any other key: ignored, loop waits for another press
	} while (!game_chosen);
}

/*
* NAME:        GameStats
* DESCRIPTION: shows game stats
* INPUTS:      game_data
* OUTPUTS:     none
*/
void GameStats(struct game_data* game_data)
{
	system("cls");
	ShowMenuTitle();
	do {
		int selection = GameStatsMenuOutcome(game_data); // get left arrow, right arrow, down arrow.
		if (selection == (GAME_STATS_NUMBER - 1))
			return;
	} while (1);
}