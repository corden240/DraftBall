#include <math.h>
#include <windows.h>
#include <stdio.h>
#include "main.h"
#include "main_d.c"

/*
* NAME:        DoPlay
* DESCRIPTION: find and then action each play
* INPUTS:      response
* OUTPUTS:     None
*/
void DoPlay(struct response* res)
{
	// player picks what to do: normal play, punt, field goal, spike, kneel, timeout

	// dont allow time out unless less than 9 plays left
}

/*
* NAME:        Play
* DESCRIPTION: plays out the gameplay
* INPUTS:      response
* OUTPUTS:     None
*/
void Play(struct response* res)
{
	for (res->quarter = 1; res->quarter <= NUMBER_OF_QUARTERS; res->quarter++)
	{
		for (res->plays_remaining_in_quarter = PLAYS_PER_QUARTER; res->plays_remaining_in_quarter > 0; res->plays_remaining_in_quarter--)
		{
			if (res->quarter == 1 || res->quarter == 3)
			{
				if (res->plays_remaining_in_quarter == PLAYS_PER_QUARTER) {
					res->team_1_timeouts_remaining = TIMEOUTS_PER_TEAM_PER_HALF;
					res->team_2_timeouts_remaining = TIMEOUTS_PER_TEAM_PER_HALF;
				}
			}

			if (res->plays_remaining_in_quarter == PLAYS_PER_QUARTER) {
				; // TODO: switch ends
			}

			DoPlay(res);	
		}
	}
}

/*
* NAME:        CoinToss
* DESCRIPTION: do coin toss
* INPUTS:      response
* OUTPUTS:     None
*/
void CoinToss(struct response* res)
{
	ConsoleShowScreen(res, "Coin toss:", 500);
	for (int i = 0; i < 4; i++)
	{
		ConsoleShowScreen(res, "\\", 50);
		ConsoleShowScreen(res, "|", 50);
		ConsoleShowScreen(res, "/", 50);
		ConsoleShowScreen(res, "-", 50);
	}

	int team_that_won_cointoss = rand() % 2 + 1; // 1 or 2

	char buf[256];
	snprintf(buf, sizeof(buf), "%s won the coin toss! Pick to punt or receive (1 or 2): ",
		(team_that_won_cointoss == 1) ? res->team_1_name : res->team_2_name);
	ConsoleShowScreen(res, buf, 500);

	char input[16];
	if (fgets(input, sizeof(input), stdin)) {
		int pick = atoi(input);
		if (pick < 1 || pick > 2) pick = 1; // validate / default

		if (pick == 2)
			res->team_started_with_possession = team_that_won_cointoss;
		else
			res->team_started_with_possession = team_that_won_cointoss - 1 ? team_that_won_cointoss - 1 : team_that_won_cointoss + 1;
	}
}

/*
* NAME:        GetTeamNames
* DESCRIPTION: setup game names
* INPUTS:      response
* OUTPUTS:     None
*/
void GetTeamNames(struct response* res)
{
	ConsoleShowScreen(res, "Enter name for Team 1: ", 0);
	fgets(res->team_1_name, sizeof(res->team_1_name), stdin);
	res->team_1_name[strcspn(res->team_1_name, "\n")] = '\0';
	ConsoleShowScreen(res, "Enter name for Team 2: ", 0);
	fgets(res->team_2_name, sizeof(res->team_2_name), stdin);
	res->team_2_name[strcspn(res->team_2_name, "\n")] = '\0';
	
	ConsoleShowScreen(res, 0, 0);
}

/*
* NAME:        SetupGame
* DESCRIPTION: initialises data such as team names, coin toss, starting possession etc
* INPUTS:      response
* OUTPUTS:     None
*/
void SetupGame(struct response* res)
{
	srand((unsigned)time(NULL));

	GetTeamNames(res);

	CoinToss(res);  
}

/*
* NAME:        ResponseConstructor
* DESCRIPTION: starts all of responses data to default settings
* INPUTS:      response
* OUTPUTS:     None
*/
void ResponseConstructor(struct response* res)
{
	res->line_of_scrimmage = STARTING_LINE_OF_SCRIMMAGE;
	res->line_of_first_down = 0;
	res->down = 1;
	res->team_in_possession = 0;
	res->score_team_1 = 0;
	res->score_team_2 = 0;
	res->quarter = 1;
	res->plays_remaining_in_quarter = PLAYS_PER_QUARTER;
	for (int i = 0; i < NUMBER_OF_DECKS; i++) {
		res->shuffle_deck_needed[i] = 0;
	}
	res->replay_game = false;
	res->team_1_timeouts_remaining = TIMEOUTS_PER_TEAM_PER_HALF;
	res->team_2_timeouts_remaining = TIMEOUTS_PER_TEAM_PER_HALF;
	res->team_started_with_possession = 0;
	strcpy(res->team_1_name, "Team 1");
	strcpy(res->team_2_name, "Team 2");
}

/*
* NAME:        EndOfGame
* DESCRIPTION: handles end of game logic
* INPUTS:      response
* OUTPUTS:     None
*/
void EndOfGame(struct response* res)
{
	// display final score and winner
	// replay game?
}

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
    struct response res;
    ResponseConstructor(&res);
    SetupGame(&res);
	
    Play(&res);

    EndOfGame(&res);
}