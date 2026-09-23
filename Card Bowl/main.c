#include <math.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>   /* srand, rand, atoi */
#include <time.h>     /* time() */
#include "main.h"
/*
* NAME:        DrawCard
* DESCRIPTION: pick an available card from the deck 
* INPUTS:      response
* OUTPUTS:     None
*/
void DrawCard(struct response* res, int deck)
{

}

/*
* NAME:        Interception
* DESCRIPTION: execute the play
* INPUTS:      response
* OUTPUTS:     None
*/
void Interception(struct response* res)
{

}

/*
* NAME:        Fumble
* DESCRIPTION: execute the play
* INPUTS:      response
* OUTPUTS:     None
*/
void Fumble(struct response* res)
{

}

/*
* NAME:        OpenFieldRun
* DESCRIPTION: execute the play
* INPUTS:      response
* OUTPUTS:     None
*/
void OpenFieldRun(struct response* res, int Offence1, int Defence1)
{

}

/*
* NAME:        PlayPass
* DESCRIPTION: execute the play
* INPUTS:      response
* OUTPUTS:     None
*/
void PlayPass(struct response* res, int Offence1, int Defence1)
{

}

/*
* NAME:        PlayRun
* DESCRIPTION: execute the play
* INPUTS:      response
* OUTPUTS:     None
*/
void PlayRun(struct response* res, int Offence1, int Defence1)
{

}

/*
* NAME:        PlayNormal
* DESCRIPTION: execute the play
* INPUTS:      response
* OUTPUTS:     None
*/
void PlayNormal(struct response* res)
{
	// pick layout
	// O: pick card positions
	// D: pick card positions
	// Reveal 1 by 1
	// O: pick which match up to play

	// PlayRun(res, O1, D1);
	// PlayPass(res, O1, D1);
}

/*
* NAME:        PlayPunt
* DESCRIPTION: execute the play
* INPUTS:      response
* OUTPUTS:     None
*/
void PlayPunt(struct response* res)
{
	// announce punt
	// draw 1-5 cards
	// can replace 0-2
	// find total yards
	// resolve where it is caught
	// do punt return here? // TODO: should it be here?
}

/*
* NAME:        PlayFieldGoal
* DESCRIPTION: execute the play
* INPUTS:      response
* OUTPUTS:     None
*/
void PlayFieldGoal(struct response* res)
{
	// find target yards, distance to end zone + 18
	// draw 1-6 cards
	// can replace 0-2
	// find total yards
	// resolve, scored, missed, turnover at line of scrimmage, leave room for it we want block to be possible?
}

/*
* NAME:        PlayKneel
* DESCRIPTION: execute the play
* INPUTS:      response
* OUTPUTS:     None
*/
void PlayKneel(struct response* res)
{
	ConsoleShowScreen(res, "QB Kneel executed. Play over.", 1000);
}

/*
* NAME:        PlayTimeout
* DESCRIPTION: execute the play
* INPUTS:      response
* OUTPUTS:     None
*/
void PlayTimeout(struct response* res)
{
	res->plays_remaining_in_quarter += 2; //add 2 plays back to the clock but then 1 will be deleted as this counts as a play
}

/*
* NAME:        GetPlayType
* DESCRIPTION: get input from player for play type
* INPUTS:      response
* OUTPUTS:     None
*/
int GetPlayType(struct response* res)
{
	int play_type = 0;
	ConsoleShowScreen(res, "Pick game type (1 - 5): ", 100);
	char input[16];
	if (fgets(input, sizeof(input), stdin)) {
		play_type = atoi(input);
	}
	return play_type;
}

/*
* NAME:        DoPlay
* DESCRIPTION: find and then action each play
* INPUTS:      response
* OUTPUTS:     None
*/
void DoPlay(struct response* res)
{
	int play_type = 0;
	
	if ((res->quarter == 1 || res->quarter == 3) && (res->plays_remaining_in_quarter == PLAYS_PER_QUARTER))
	{
		play_type = 2; // always punt on first play of each half
	}
	//else if (1) // TODO: if touchdown just scored, automatically go for extra point
	//{
	//	// play extra point or 2?
	//}
	else
	{
		play_type = GetPlayType(res); // player picks what to do: normal play, punt, field goal, kneel, timeout
	}

	bool retry = false;
	do {
		retry = false;
		switch (play_type) {
		case PLAY_TYPE_NORMAL:
		{
			PlayNormal(res);
			break;
		}
		case PLAY_TYPE_PUNT:
		{
			PlayPunt(res);
			break;
		}
		case PLAY_TYPE_FIELD_GOAL:
		{
			PlayFieldGoal(res);
			break;
		}
		case PLAY_TYPE_KNEEL:
		{
			PlayKneel(res);
			break;
		}
		case PLAY_TYPE_TIMEOUT:
		{
			if(res->plays_remaining_in_quarter > (PLAYS_PER_QUARTER - 2))
				retry = true;
			else
				PlayTimeout(res);
			break;
		}
		default:
		{
			ConsoleShowScreen(res, "Invalid play type. Please pick a number between 1 and 5. (you cannot timeout unless 2 plays have already happened this quarter)", 1000);
			retry = true;
			break;
		}
		}

	} while (retry);
	
	// TODO: assess whether we made first down? otherwise go to the next down? unless it was a timeout? If it was 4th down resolve
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
				SwitchEnds(res);
				//TODO: announce switching of ends 
			}

			DoPlay(res);	
		}
		//TODO: announce end of quarter, announce who will have the ball / punt
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

		if (res->team_started_with_possession == 1)
		{
			res->team_1_direction = 1;
			res->team_2_direction = 2;
		}
		else
		{
			res->team_1_direction = 2;
			res->team_2_direction = 1;
		}
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
* NAME:        GetPlayerNames
* DESCRIPTION: setup player names
* INPUTS:      response
* OUTPUTS:     None
*/
void GetPlayerNames(struct response* res)
{
	ConsoleShowScreen(res, "Would you like to name your players? Y/N ", 0);
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

	GetTeamNames(res);

	GetPlayerNames(res);

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
}

/*
* NAME:        EndOfGame
* DESCRIPTION: handles end of game logic
* INPUTS:      response
* OUTPUTS:     None
*/
void EndOfGame(struct response* res)
{
	ConsoleShowScreen(res, "Game Over!", 1000);
	if (res->score_team_1 > res->score_team_2)
	{
		char buf[256];
		snprintf(buf, sizeof(buf), "%s wins!", res->team_1_name);
		ConsoleShowScreen(res, buf, 1000);
	}
	else if (res->score_team_2 > res->score_team_1)
	{
		char buf[256];
		snprintf(buf, sizeof(buf), "%s wins!", res->team_2_name);
		ConsoleShowScreen(res, buf, 1000);
	}
	else
	{
		ConsoleShowScreen(res, "It's a tie!", 1000);
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
	SetupWindow();	
    struct response res;
    ResponseConstructor(&res);
    SetupGame(&res);
	
    Play(&res);

    EndOfGame(&res);
}