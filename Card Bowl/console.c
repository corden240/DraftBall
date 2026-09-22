#include "main_d.c"
#include "main.h"
#include <stdio.h>
#include <windows.h>

/*
* NAME:        ResetScreen
* DESCRIPTION: clears screen and sets cursor to top left
* INPUTS:      None
* OUTPUTS:     None
*/
void ResetScreen()
{
	printf("\033[H");
}

/* 
* NAME:			ShowTopInfo
* DESCRIPTION:  Displays the top information on the screen
* INPUTS:       response
* OUTPUTS:      None
*/
void ShowTopInfo(struct response* res)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_LIGHTBLUE);
	printf("                         %s  %d : %d  %s        |        Quarter: %d      Plays Left: %d                      ", res->team_1_name, res->score_team_1, res->score_team_2, res->team_2_name, res->quarter, res->plays_remaining_in_quarter);
	SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
}

/*
* NAME:        PrintYardNumbers
* DESCRIPTION: show yard lines below the field
* INPUTS:      None
* OUTPUTS:     None
*/
void PrintYardNumbers(void) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
	printf("\n\r     ");
	int yards[] = { 10, 20, 30, 40, 50, 40, 30, 20, 10 };
	int n = sizeof(yards) / sizeof(yards[0]);

	for (int i = 0; i < n; i++) {
		printf("%10d", yards[i]);   // field width 10, right-justified
	}
}

/*
* NAME:        PrintSideline
* DESCRIPTION: print sideline
* INPUTS:      None
* OUTPUTS:     None
*/
void PrintSideline(void) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
	printf("\n\r");
	SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
	for (int i = 0; i < (END_ZONE_WIDTH + FIELD_WIDTH + END_ZONE_WIDTH - 1); i++) {
		printf("-");
	}
}

/*
* NAME:        DrawField
* DESCRIPTION: shows field and lines 
* INPUTS:      response
* OUTPUTS:     None
*/
void DrawField(struct response* res)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
	printf("\n\r");

	PrintSideline();
	// TODO: print different colour on line of scrimmage and line of first down
	for (int j = 0; j < FIELD_HEIGHT; j++)
	{
		printf("\n\r");
		for (int i = 1; i <= (END_ZONE_WIDTH + FIELD_WIDTH + END_ZONE_WIDTH - 1); i++) {
			if (i < END_ZONE_WIDTH || i > (PITCH_WIDTH - END_ZONE_WIDTH))
			{
				SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_RED);
				printf("/");
			}
			else if ((i - 5) % 10 == 0)
			{
				SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
				printf("|");
			}
			else
			{
				SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
				printf(" ");
			}
		}
	}

	PrintSideline();

	PrintYardNumbers();
}

/*
* NAME:        ShowGameInfo
* DESCRIPTION: prints down and distance info
* INPUTS:      response
* OUTPUTS:     None
*/
void ShowGameInfo(struct response* res)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
	printf("\n\r\n\r");
	printf("                                 Down: %d  Distance: %d  Line of Scrimmage: %d                                   ", res->down, res->line_of_scrimmage - res->line_of_first_down, res->line_of_scrimmage);
}

/*
* NAME:        ShowText
* DESCRIPTION: prints text given to function
* INPUTS:      response, text to display
* OUTPUTS:     None
*/
void ShowText(struct response* res, char txt[1000])
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
	printf("\n\r\n\r                                                                                             ");
	printf("\r                                 ");
	for (int i = 0; i < 1000 && txt[i] != '\0'; i++)
	{
		printf("%c", txt[i]);
		Sleep(10);
	}
}

/*
* NAME:        ConsoleShowScreen
* DESCRIPTION: updates the whole screen with current data
* INPUTS:      response, text to display, pause time, wait for keypress
* OUTPUTS:     None
*/
void ConsoleShowScreen(struct response* res, char txt[1000], int pause) // if called this asking a question to the player, make sure to called getch straight after
{
	ResetScreen();     // go to top left of screen and clear it
	ShowTopInfo(res);  // draw score at top of screen with play clock
	DrawField(res);    // draw field with line of scrimmage and down and distance lines
	ShowGameInfo(res); // then write down and distance numbers
	//card drawn info?
	if(txt)
		ShowText(res, txt);// then draw text sent to function
	if(pause)
		Sleep(pause);      // pause for a bit if needed)
}