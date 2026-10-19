#include <stdio.h>
#include <stdlib.h>   /* srand, rand, atoi */
#include <time.h>     /* time() */
#include "main.h"
#include <string.h>
#include <windows.h>

void ShowMenuTitle();
void ResetScreen();

/*
* NAME:        findLen
* DESCRIPTION: length of string
* INPUTS:      string
* OUTPUTS:     length int
*/
int findLen(char* s) {
	int c = 0;

	// Traverse string until null terminator
	// is found while incrementing counter
	while (*s++)
		c++;

	return c;
}

/*
* NAME:        FillMissingGameData
* DESCRIPTION: calculates remaining game stats
* INPUTS:      game_data
* OUTPUTS:     none
*/
void FillMissingGameData(struct game_data* game_data)
{
	for (int team = 0; team < NUMBER_OF_TEAMS_PER_GAME; team++)
	{
		int attempts = game_data->quarterback_stats[team][QB_ATTEMPTED_PASSES];

		if (attempts == 0)
		{
			game_data->quarterback_stats[team][QB_AVERAGE] = 0;
			game_data->quarterback_stats[team][QB_RATING] = 0;
			continue;
		}

		/* QB_AVERAGE is an int field, so this remains a whole-number average. */
		game_data->quarterback_stats[team][QB_AVERAGE] =
			game_data->quarterback_stats[team][QB_YARDS] / attempts;

		float a = (((float)game_data->quarterback_stats[team][QB_COMPLETE_PASSES] / attempts) - 0.3f) * 5.0f;
		float b = (((float)game_data->quarterback_stats[team][QB_YARDS] / attempts) - 3.0f) * 0.25f;
		float c = ((float)game_data->quarterback_stats[team][QB_TOUCHDOWNS] / attempts) * 20.0f;
		float d = 2.375f - (((float)game_data->quarterback_stats[team][QB_INTERCEPTIONS_THROWN] / attempts) * 25.0f);

		float passer_rating = ((a + b + c + d) / 6.0f) * 100.0f;
		if (passer_rating < 0)
			passer_rating = 0;
		else if (passer_rating > 158.3)
			passer_rating = 158.3;
		game_data->quarterback_stats[team][QB_RATING] = (int)passer_rating;
	}


	for (int team = 0; team < NUMBER_OF_TEAMS_PER_GAME; team++)
	{
		for (int i = 0; i < NUMBER_OF_RB_NAMES_PER_TEAM; i++)
		{
			int carries = game_data->runningback_stats[team][RB_CARRIES];

			if (carries == 0)
			{
				game_data->runningback_stats[i + (NUMBER_OF_RB_NAMES_PER_TEAM * team)][RB_AVERAGE] = 0;
				continue;
			}

			/* RB_AVERAGE is an int field, so this remains a whole-number average. */
			game_data->runningback_stats[i + (NUMBER_OF_RB_NAMES_PER_TEAM * team)][RB_AVERAGE] =
				game_data->runningback_stats[i + (NUMBER_OF_RB_NAMES_PER_TEAM * team)][RB_YARDS] / carries;
		}
	}

	for (int team = 0; team < NUMBER_OF_TEAMS_PER_GAME; team++)
	{
		for (int i = 0; i < NUMBER_OF_WR_NAMES_PER_TEAM; i++)
		{
			int receptions = game_data->receiver_stats[team][WR_RECEPTIONS];

			if (receptions == 0)
			{
				game_data->receiver_stats[i + (NUMBER_OF_RB_NAMES_PER_TEAM * team)][WR_AVERAGE] = 0;
				continue;
			}

			/* WR_AVERAGE is an int field, so this remains a whole-number average. */
			game_data->receiver_stats[i + (NUMBER_OF_RB_NAMES_PER_TEAM * team)][WR_AVERAGE] =
				game_data->receiver_stats[i + (NUMBER_OF_RB_NAMES_PER_TEAM * team)][WR_YARDS] / receptions;
		}
	}

	for (int team = 0; team < NUMBER_OF_TEAMS_PER_GAME; team++)
	{
		int carries = game_data->quarterback_stats[team][QB_RUNNING_CARRIES];

		if (carries == 0)
		{
			game_data->quarterback_stats[team][QB_RUNNING_AVERAGE] = 0;	
			continue;
		}
		/* QB_RUNNING_AVERAGE is an int field, so this remains a whole-number average. */
		game_data->quarterback_stats[team][QB_RUNNING_AVERAGE] =
		game_data->quarterback_stats[team][QB_RUNNING_YARDS] / carries;
	}
}

/*
* NAME:        FillGameStatsNamesLengths
* DESCRIPTION: find name lengths
* INPUTS:      names array, game_data
* OUTPUTS:     none
*/
void FillGameStatsNamesLengths(int game_stats_name_widths[NUMBER_OF_TEAMS_PER_GAME][NUMBER_OF_QB_NAMES_PER_TEAM + NUMBER_OF_RB_NAMES_PER_TEAM + NUMBER_OF_WR_NAMES_PER_TEAM + 1], struct game_data* game_data)
{
	game_stats_name_widths[0][0] = findLen(game_data->team_1_name);
	game_stats_name_widths[0][1] = findLen(game_data->team_1_qb_names[0]);
	game_stats_name_widths[0][2] = findLen(game_data->team_1_rb_names[0]);
	game_stats_name_widths[0][3] = findLen(game_data->team_1_rb_names[1]);
	game_stats_name_widths[0][4] = findLen(game_data->team_1_wr_names[0]);
	game_stats_name_widths[0][5] = findLen(game_data->team_1_wr_names[1]);
	game_stats_name_widths[0][6] = findLen(game_data->team_1_wr_names[2]);
	game_stats_name_widths[0][7] = findLen(game_data->team_1_wr_names[3]);

	game_stats_name_widths[1][0] = findLen(game_data->team_2_name);
	game_stats_name_widths[1][1] = findLen(game_data->team_2_qb_names[0]);
	game_stats_name_widths[1][2] = findLen(game_data->team_2_rb_names[0]);
	game_stats_name_widths[1][3] = findLen(game_data->team_2_rb_names[1]);
	game_stats_name_widths[1][4] = findLen(game_data->team_2_wr_names[0]);
	game_stats_name_widths[1][5] = findLen(game_data->team_2_wr_names[1]);
	game_stats_name_widths[1][6] = findLen(game_data->team_2_wr_names[2]);
	game_stats_name_widths[1][7] = findLen(game_data->team_2_wr_names[3]);
}

/*
* NAME:        GameStatsRow1
* DESCRIPTION: shows row 1
* INPUTS:      game_data
* OUTPUTS:     none
*/
void GameStatsRow1(struct game_data* game_data, int left_buffer, int right_buffer, int selected, int game_stats_name_widths[NUMBER_OF_TEAMS_PER_GAME][NUMBER_OF_QB_NAMES_PER_TEAM + NUMBER_OF_RB_NAMES_PER_TEAM + NUMBER_OF_WR_NAMES_PER_TEAM + 1])
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
	printf("%*c", (left_buffer - game_stats_name_widths[0][0]), ' ');
	if (selected == 0)
		SetConsoleTextAttribute(hConsole, CONSOLE_BG_WHITE);
	printf("%s", game_data->team_1_name);
	SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);

	printf("   QTR   1   2   3   4   ");

	if (selected == 1)
		SetConsoleTextAttribute(hConsole, CONSOLE_BG_WHITE);
	printf("%s", game_data->team_2_name);
	SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
	printf("%*c", (right_buffer - game_stats_name_widths[1][0]), ' ');
	printf("\r\n");
}

/*
* NAME:        GameStatsRow2
* DESCRIPTION: shows row 2
* INPUTS:      game_data
* OUTPUTS:     none
*/
void GameStatsRow2(struct game_data* game_data)
{
	printf("                                       %2d pts    T1  %2d  %2d  %2d  %2d   %2d pts        \r\n",
		game_data->team_1_score,
		game_data->quarter_scores[0][0],
		game_data->quarter_scores[0][1],
		game_data->quarter_scores[0][2],
		game_data->quarter_scores[0][3],
		game_data->team_2_score);
}

/*
* NAME:        GameStatsRow3
* DESCRIPTION: shows row 3
* INPUTS:      game_data
* OUTPUTS:     none
*/
void GameStatsRow3(struct game_data* game_data, int left_buffer)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
	char team_1_record[32];
	char team_2_record[32];
	if (game_data->current_game_mode == 1)
	{
		if (game_data->team_records[0][DRAWS] == 0)
			snprintf(team_1_record, sizeof(team_1_record), "%d-%d",
				game_data->team_records[0][WINS],
				game_data->team_records[0][LOSSES]);
		else
			snprintf(team_1_record, sizeof(team_1_record), "%d-%d-%d",
				game_data->team_records[0][WINS],
				game_data->team_records[0][LOSSES],
				game_data->team_records[0][DRAWS]);

		if (game_data->team_records[1][DRAWS] == 0)
			snprintf(team_2_record, sizeof(team_2_record), "%d-%d",
				game_data->team_records[1][WINS],
				game_data->team_records[1][LOSSES]);
		else
			snprintf(team_2_record, sizeof(team_2_record), "%d-%d-%d",
				game_data->team_records[1][WINS],
				game_data->team_records[1][LOSSES],
				game_data->team_records[1][DRAWS]);
	}

	if (game_data->current_game_mode == 1) {
		printf("%*c", (left_buffer - findLen(team_1_record) - 2), ' ');
		printf("%s", team_1_record);
	}
	else
		printf("%*c", (left_buffer + 3), ' ');

	printf("      T2  %2d  %2d  %2d  %2d    ",
		game_data->quarter_scores[1][0],
		game_data->quarter_scores[1][1],
		game_data->quarter_scores[1][2],
		game_data->quarter_scores[1][3]);

	if (game_data->current_game_mode == 1)
		printf("%s", team_2_record);

	printf("                \r\n");
}

/*
* NAME:        GameStatsRow4
* DESCRIPTION: shows row 4
* INPUTS:      game_data
* OUTPUTS:     none
*/
void GameStatsRow4(struct game_data* game_data)
{
	printf("                                     Passing     C/ATT   YDS  AVG  TD  INT  SACKS  RTG    \r\n");
}

/*
* NAME:        GameStatsRow5
* DESCRIPTION: shows row 5
* INPUTS:      game_data
* OUTPUTS:     none
*/
void GameStatsRow5(struct game_data* game_data, int left_buffer, int team_selected, int game_stats_name_widths[NUMBER_OF_TEAMS_PER_GAME][NUMBER_OF_QB_NAMES_PER_TEAM + NUMBER_OF_RB_NAMES_PER_TEAM + NUMBER_OF_WR_NAMES_PER_TEAM + 1])
{
	printf("%*c", (left_buffer - game_stats_name_widths[team_selected][1] - 1), ' ');
	printf("%s     %2d/%2d   %3d   %2d  %2d   %2d  %2d-%2d  %3d     \r\n",
		team_selected ? game_data->team_2_qb_names[0] : game_data->team_1_qb_names[0],
		game_data->quarterback_stats[team_selected][QB_COMPLETE_PASSES],
		game_data->quarterback_stats[team_selected][QB_ATTEMPTED_PASSES],
		game_data->quarterback_stats[team_selected][QB_YARDS],
		game_data->quarterback_stats[team_selected][QB_AVERAGE],
		game_data->quarterback_stats[team_selected][QB_TOUCHDOWNS],
		game_data->quarterback_stats[team_selected][QB_INTERCEPTIONS_THROWN],
		game_data->quarterback_stats[team_selected][QB_SACKS],
		game_data->quarterback_stats[team_selected][QB_SACK_YARDS_LOST],
		game_data->quarterback_stats[team_selected][QB_RATING]);
}

/*
* NAME:        GameStatsRow6
* DESCRIPTION: shows row 6
* INPUTS:      game_data
* OUTPUTS:     none
*/
void GameStatsRow6(struct game_data* game_data)
{
	printf("                                     Rushing     CAR  YDS  AVG  TD  LONG                  \r\n");
}

/*
* NAME:        GameStatsRowRB
* DESCRIPTION: shows RB rows
* INPUTS:      game_data
* OUTPUTS:     none
*/
void GameStatsRB(struct game_data* game_data, int left_buffer, int team_selected, int game_stats_name_widths[NUMBER_OF_TEAMS_PER_GAME][NUMBER_OF_QB_NAMES_PER_TEAM + NUMBER_OF_RB_NAMES_PER_TEAM + NUMBER_OF_WR_NAMES_PER_TEAM + 1])
{
	for (int i = 0; i < NUMBER_OF_RB_NAMES_PER_TEAM; i++)
	{
		printf("%*c", (left_buffer - game_stats_name_widths[team_selected][2 + i] - 1), ' ');
		printf("%s      %2d   %2d   %2d  %2d   %2d                     \r\n",
			team_selected ? game_data->team_2_rb_names[i] : game_data->team_1_rb_names[i],
			game_data->runningback_stats[i + (team_selected * NUMBER_OF_RB_NAMES_PER_TEAM)][RB_CARRIES],
			game_data->runningback_stats[i + (team_selected * NUMBER_OF_RB_NAMES_PER_TEAM)][RB_YARDS],
			game_data->runningback_stats[i + (team_selected * NUMBER_OF_RB_NAMES_PER_TEAM)][RB_AVERAGE],
			game_data->runningback_stats[i + (team_selected * NUMBER_OF_RB_NAMES_PER_TEAM)][RB_TOUCHDOWNS],
			game_data->runningback_stats[i + (team_selected * NUMBER_OF_RB_NAMES_PER_TEAM)][RB_LONGEST_PLAY]);
	}
}

/*
* NAME:        GameStatsRow7
* DESCRIPTION: shows row 7
* INPUTS:      game_data
* OUTPUTS:     none
*/
void GameStatsRow7(struct game_data* game_data)
{
	printf("                                   Receiving     REC  YDS  AVG  TD  LONG  TGTS            \r\n");
}

/*
* NAME:        GameStatsRowWR
* DESCRIPTION: shows WR rows
* INPUTS:      game_data
* OUTPUTS:     none
*/
void GameStatsWR(struct game_data* game_data, int left_buffer, int team_selected, int game_stats_name_widths[NUMBER_OF_TEAMS_PER_GAME][NUMBER_OF_QB_NAMES_PER_TEAM + NUMBER_OF_RB_NAMES_PER_TEAM + NUMBER_OF_WR_NAMES_PER_TEAM + 1])
{
	for (int i = 0; i < NUMBER_OF_WR_NAMES_PER_TEAM; i++)
	{
		printf("%*c", (left_buffer - game_stats_name_widths[team_selected][4 + i] - 1), ' ');
		printf("%s      %2d   %2d   %2d  %2d   %2d    %2d               \r\n",
			team_selected ? game_data->team_2_wr_names[i] : game_data->team_1_wr_names[i],
			game_data->receiver_stats[i + (team_selected * NUMBER_OF_WR_NAMES_PER_TEAM)][WR_RECEPTIONS],
			game_data->receiver_stats[i + (team_selected * NUMBER_OF_WR_NAMES_PER_TEAM)][WR_YARDS],
			game_data->receiver_stats[i + (team_selected * NUMBER_OF_WR_NAMES_PER_TEAM)][WR_AVERAGE],
			game_data->receiver_stats[i + (team_selected * NUMBER_OF_WR_NAMES_PER_TEAM)][WR_TOUCHDOWNS], 
			game_data->receiver_stats[i + (team_selected * NUMBER_OF_WR_NAMES_PER_TEAM)][WR_LONGEST_PLAY],
			game_data->receiver_stats[i + (team_selected * NUMBER_OF_WR_NAMES_PER_TEAM)][WR_TARGETS]);
	}
}

/*
* NAME:        ShowGameStats
* DESCRIPTION: shows game stats
* INPUTS:      current selection, game_data
* OUTPUTS:     none
*/
void ShowGameStats(int selected, struct game_data* game_data)
{
	FillMissingGameData(game_data);

	ResetScreen();
	ShowMenuTitle();
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);

	int team_selected = selected;
	if (team_selected == 2)
		team_selected = 1;

	int left_buffer = 45, right_buffer = 20;
	int game_stats_name_widths[NUMBER_OF_TEAMS_PER_GAME][NUMBER_OF_QB_NAMES_PER_TEAM + NUMBER_OF_RB_NAMES_PER_TEAM  + NUMBER_OF_WR_NAMES_PER_TEAM + 1];

	FillGameStatsNamesLengths(&game_stats_name_widths, game_data);

	GameStatsRow1(game_data, left_buffer, right_buffer, selected, &game_stats_name_widths);
	GameStatsRow2(game_data);
	GameStatsRow3(game_data, left_buffer);
	printf("                                                                                          \r\n");
	printf("                                                                                          \r\n");
	GameStatsRow4(game_data);
	GameStatsRow5(game_data, left_buffer, team_selected, &game_stats_name_widths);
	printf("                                                                                          \r\n");
	GameStatsRow6(game_data);
	GameStatsRB(game_data, left_buffer, team_selected, &game_stats_name_widths);
	printf("                                                                                          \r\n");
	GameStatsRow7(game_data);
	GameStatsWR(game_data, left_buffer, team_selected, &game_stats_name_widths);
	printf("                                                                                          \r\n");
	printf("                                                         ");
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
