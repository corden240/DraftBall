#include <math.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>   /* srand, rand, atoi */
#include <time.h>     /* time() */
#include "main.h"

void SwitchPossession(struct response* res);
void ResetLineOfFirstDown(struct response* res);
void ResetLineOfScrimmage(struct response* res);

/*
* NAME:        ResetLineOfScrimmage
* DESCRIPTION: make line of scrimmage to starting for team in possession
* INPUTS:      response
* OUTPUTS:     None
*/
void ResetLineOfScrimmage(struct response* res)
{
	if ((res->team_in_possession == 1 && res->team_1_direction == 1) || (res->team_in_possession == 2 && res->team_2_direction == 1))
	{
		res->line_of_scrimmage = STARTING_LINE_OF_SCRIMMAGE + END_ZONE_WIDTH;
	}
	else
	{
		res->line_of_scrimmage = FIELD_WIDTH - STARTING_LINE_OF_SCRIMMAGE + END_ZONE_WIDTH;
	}
}

/*
* NAME:        MoveLineOfScrimmage
* DESCRIPTION: move LoS by yards depending on who has the ball
* INPUTS:      response, yardsa
* OUTPUTS:     None
*/
void MoveLineOfScrimmage(struct response* res, int yards)
{
	int yards_moved = -yards;

	if ((res->team_in_possession == 1 && res->team_1_direction == 1) || (res->team_in_possession == 2 && res->team_2_direction == 1))
	{
		yards_moved = yards;
	}

	res->line_of_scrimmage += yards_moved;
}

/*
* NAME:        ResetLineOfFirstDown
* DESCRIPTION: make line of first down 10 yards ahead of current LoS
* INPUTS:      response
* OUTPUTS:     None
*/
void ResetLineOfFirstDown(struct response* res)
{
	int first_down = -YARDS_PER_FIRST_DOWN;

	if ((res->team_in_possession == 1 && res->team_1_direction == 1) || (res->team_in_possession == 2 && res->team_2_direction == 1))
	{
		first_down = YARDS_PER_FIRST_DOWN;
	}

	res->line_of_first_down = res->line_of_scrimmage + first_down;

	if (res->line_of_first_down < END_ZONE_WIDTH)
		res->line_of_first_down = END_ZONE_WIDTH;
	else if (res->line_of_first_down > (PITCH_WIDTH - END_ZONE_WIDTH))
		res->line_of_first_down = (PITCH_WIDTH - END_ZONE_WIDTH);
}

/*
* NAME:        SwitchEnds
* DESCRIPTION: switch direction of play
* INPUTS:      response
* OUTPUTS:     None
*/
void SwitchEnds(struct response* res)
{
	if (res->team_1_direction == 1)
	{
		res->team_1_direction = 2;
		res->team_2_direction = 1;
	}
	else
	{
		res->team_1_direction = 1;
		res->team_2_direction = 2;
	}

	ResetLineOfScrimmage(res);
	ResetLineOfFirstDown(res);
}

/*
* NAME:        SwitchPossession
* DESCRIPTION: switch team with possession
* INPUTS:      response
* OUTPUTS:     None
*/
void SwitchPossession(struct response* res)
{
	res->team_not_in_possession = res->team_in_possession;
	res->team_in_possession = res->team_in_possession - 1 ? res->team_in_possession - 1 : res->team_in_possession + 1;
	return;
}

/*
* NAME:        CoinToss
* DESCRIPTION: do coin toss
* INPUTS:      response
* OUTPUTS:     None
*/
void CoinToss(struct response* res)
{
	ConsoleShowScreen(res, "Coin toss: ", GAME_SPEED * 10);
	for (int i = 0; i < 4; i++)
	{
		ConsoleShowScreen(res, "\\", GAME_SPEED / 5);
		ConsoleShowScreen(res, "|", GAME_SPEED / 5);
		ConsoleShowScreen(res, "/", GAME_SPEED / 5);
		ConsoleShowScreen(res, "-", GAME_SPEED / 5);
	}

	ConsoleShowScreen(res, 0, GAME_SPEED * 20);

	int team_that_won_cointoss = rand() % 2 + 1; // 1 or 2

	char buf[256];
	snprintf(buf, sizeof(buf), "%s won the coin toss! Punt or receive (1 or 2): ",
		(team_that_won_cointoss == 1) ? res->team_1_name : res->team_2_name);
	ConsoleShowScreen(res, buf, GAME_SPEED * 10);

	char input[16];
	if (fgets(input, sizeof(input), stdin)) {
		int pick = atoi(input);
		if (pick < 1 || pick > 2) pick = 1; // validate / default

		if (pick == 2)
			res->team_started_with_possession = team_that_won_cointoss - 1 ? team_that_won_cointoss - 1 : team_that_won_cointoss + 1;
		else
			res->team_started_with_possession = team_that_won_cointoss;

		res->team_in_possession = res->team_started_with_possession;

		if (res->team_started_with_possession == 1)
		{
			res->line_of_scrimmage = STARTING_LINE_OF_SCRIMMAGE + END_ZONE_WIDTH;
			res->line_of_first_down = res->line_of_scrimmage + YARDS_PER_FIRST_DOWN;
		}
		else
		{
			res->line_of_scrimmage = FIELD_WIDTH - STARTING_LINE_OF_SCRIMMAGE + END_ZONE_WIDTH;
			res->line_of_first_down = res->line_of_scrimmage - YARDS_PER_FIRST_DOWN;
		}
		ConsoleShowScreen(res, 0, GAME_SPEED);
	}
}

/*
* NAME:        GetTeamNames
* DESCRIPTION: setup game names
* INPUTS:      response
* OUTPUTS:     None
*/
void SetTeamNames(struct response* res)
{
	ConsoleShowScreen(res, "Would you like to name your teams? y/n ", 0);
	char input[16];
	fgets(input, sizeof(input), stdin);

	if (input[0] == 'N' || input[0] == 'n')
		res->players_named = false;
	else
	{
		ConsoleShowScreen(res, "Enter name for Team 1: ", 0);
		fgets(res->team_1_name, sizeof(res->team_1_name), stdin);
		res->team_1_name[strcspn(res->team_1_name, "\n")] = '\0';
		ConsoleShowScreen(res, "Enter name for Team 2: ", 0);
		fgets(res->team_2_name, sizeof(res->team_2_name), stdin);
		res->team_2_name[strcspn(res->team_2_name, "\n")] = '\0';
		ConsoleShowScreen(res, 0, 0);
	}
}

/*
* NAME:        GetPlayerNames
* DESCRIPTION: setup player names
* INPUTS:      response
* OUTPUTS:     None
*/
void SetPlayerNames(struct response* res)
{
	ConsoleShowScreen(res, "Would you like to name your players? y/n ", 0);
	char input[16];
	fgets(input, sizeof(input), stdin);

	if (input[0] == 'N' || input[0] == 'n')
		res->players_named = false;
	else
	{
		res->players_named = true;
		for (int i = 0; i < NUMBER_OF_PLAYER_NAMES_PER_TEAM; i++)
		{
			if(i < NUMBER_OF_RB_NAMES_PER_TEAM)
			{
				char buf[256];
				snprintf(buf, sizeof(buf), "Enter name for Team 1 RB Player %d: ", i + 1);
				ConsoleShowScreen(res, buf, 0);
				fgets(res->team_1_rb_names[i], sizeof(res->team_1_rb_names[i]), stdin);
				res->team_1_rb_names[i][strcspn(res->team_1_rb_names[i], "\n")] = '\0';
			}
			else
			{
				char buf[256];
				snprintf(buf, sizeof(buf), "Enter name for Team 1 WR Player %d: ", i - NUMBER_OF_RB_NAMES_PER_TEAM + 1);
				ConsoleShowScreen(res, buf, 0);
				fgets(res->team_1_wr_names[i], sizeof(res->team_1_wr_names[i]), stdin);
				res->team_1_wr_names[i][strcspn(res->team_1_wr_names[i], "\n")] = '\0';
			}
		}
		for (int i = 0; i < NUMBER_OF_PLAYER_NAMES_PER_TEAM; i++)
		{
			if (i < NUMBER_OF_RB_NAMES_PER_TEAM)
			{
				char buf[256];
				snprintf(buf, sizeof(buf), "Enter name for Team 2 RB Player %d: ", i + 1);
				ConsoleShowScreen(res, buf, 0);
				fgets(res->team_2_rb_names[i], sizeof(res->team_2_rb_names[i]), stdin);
				res->team_2_rb_names[i][strcspn(res->team_2_rb_names[i], "\n")] = '\0';
			}
			else
			{
				char buf[256];
				snprintf(buf, sizeof(buf), "Enter name for Team 2 WR Player %d: ", i - NUMBER_OF_RB_NAMES_PER_TEAM + 1);
				ConsoleShowScreen(res, buf, 0);
				fgets(res->team_2_wr_names[i], sizeof(res->team_2_wr_names[i]), stdin);
				res->team_2_wr_names[i][strcspn(res->team_2_wr_names[i], "\n")] = '\0';
			}
		}
	}
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

	SetTeamNames(res);

	SetPlayerNames(res);

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
	res->line_of_scrimmage = STARTING_LINE_OF_SCRIMMAGE + END_ZONE_WIDTH;
	res->line_of_first_down = 0;
	res->down = 1;
	res->team_in_possession = 0;
	res->team_not_in_possession = 0;
	res->team_1_score = 0;
	res->team_2_score = 0;
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
	res->team_1_direction = 1; // default direction
	res->team_2_direction = 2; // default direction

	strcpy(res->team_1_rb_names[0], "Team 1 RB1");
	strcpy(res->team_2_rb_names[0], "Team 2 RB1");
	strcpy(res->team_1_rb_names[1], "Team 1 RB2");
	strcpy(res->team_2_rb_names[1], "Team 2 RB2");

	strcpy(res->team_1_wr_names[0], "Team 1 WR1");
	strcpy(res->team_2_wr_names[0], "Team 2 WR1");
	strcpy(res->team_1_wr_names[1], "Team 1 WR2");
	strcpy(res->team_2_wr_names[1], "Team 2 WR2");
	strcpy(res->team_1_wr_names[2], "Team 1 WR3");
	strcpy(res->team_2_wr_names[2], "Team 2 WR3");
	strcpy(res->team_1_wr_names[3], "Team 1 WR4");
	strcpy(res->team_2_wr_names[3], "Team 2 WR4");

	res->players_named = false;
	
	for (int i = 0; i < CARDS_IN_A_DECK; i++) {
		res->o_cards[i] = 0;
		res->d_cards[i] = 0;
	}

	res->kickoff_needed = true;
	res->extra_point_needed = false;
}

/*
* NAME:        EndOfGame
* DESCRIPTION: handles end of game logic
* INPUTS:      response
* OUTPUTS:     None
*/
void EndOfGame(struct response* res)
{
	ConsoleShowScreen(res, "Game Over!", GAME_SPEED * 20);
	if (res->team_1_score > res->team_2_score)
	{
		char buf[256];
		snprintf(buf, sizeof(buf), "%s wins!", res->team_1_name);
		ConsoleShowScreen(res, buf, GAME_SPEED * 20);
	}
	else if (res->team_2_score > res->team_1_score)
	{
		char buf[256];
		snprintf(buf, sizeof(buf), "%s wins!", res->team_2_name);
		ConsoleShowScreen(res, buf, GAME_SPEED * 20);
	}
	else
	{
		ConsoleShowScreen(res, "It's a tie!", GAME_SPEED * 20);
	}
	
	// print all the game stats? TODO: player stats, quarter stats, yards, downs etc
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
	// TODO: Be able to play a season / career / playoffs etc 

	// TODO: start of game settings and info page, red line = LoS, purple line = first down, plays 1-5 = normal, punt etc

	SetupWindow();	
    struct response res;
    ResponseConstructor(&res);
    SetupGame(&res);
	
    Play(&res);
	
    EndOfGame(&res);
}