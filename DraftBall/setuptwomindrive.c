#include <stdio.h>
#include <stdlib.h>   /* srand, rand, atoi */
#include <time.h>     /* time() */
#include "main.h"
#include <string.h>

void ResetLineOfFirstDown(struct game_data* game_data);

/*
* NAME:        TwoMinDriveGameDataConstructor
* DESCRIPTION: starts all of game_data data to 2 min drive settings
* INPUTS:      game_data
* OUTPUTS:     none
*/
void TwoMinDriveGameDataConstructor(struct game_data* game_data, struct total_data* total_data)
{
	game_data->line_of_scrimmage = STARTING_LINE_OF_SCRIMMAGE + END_ZONE_WIDTH;
	game_data->line_of_first_down = 0;
	game_data->down = 1;
	game_data->team_in_possession = 1;
	game_data->team_not_in_possession = 2;
	game_data->team_1_score = ((rand() % 4) + 1) * 7;
	game_data->team_2_score = 0;
	game_data->quarter = NUMBER_OF_QUARTERS;
	game_data->plays_remaining_in_quarter = TWO_MIN_DRIVE_PLAYS;
	game_data->plays_per_quarter = total_data->plays_per_quarter;
	for (int i = 0; i < NUMBER_OF_DECKS; i++) {
		game_data->shuffle_deck_needed[i] = 0;
	}
	game_data->team_1_timeouts_remaining = TWO_MIN_DRIVE_TIMEOUTS;
	game_data->team_2_timeouts_remaining = 0;
	game_data->team_started_with_possession = 0;
	strcpy(game_data->team_1_name, "Team 1");
	strcpy(game_data->team_2_name, "Team 2");
	game_data->team_1_direction = 1; // default direction
	game_data->team_2_direction = 2; // default direction

	strcpy(game_data->team_1_qb_names[0], "Team 1 QB1");
	strcpy(game_data->team_2_qb_names[0], "Team 2 QB1");

	strcpy(game_data->team_1_rb_names[0], "Team 1 RB1");
	strcpy(game_data->team_2_rb_names[0], "Team 2 RB1");
	strcpy(game_data->team_1_rb_names[1], "Team 1 RB2");
	strcpy(game_data->team_2_rb_names[1], "Team 2 RB2");

	strcpy(game_data->team_1_wr_names[0], "Team 1 WR1");
	strcpy(game_data->team_2_wr_names[0], "Team 2 WR1");
	strcpy(game_data->team_1_wr_names[1], "Team 1 WR2");
	strcpy(game_data->team_2_wr_names[1], "Team 2 WR2");
	strcpy(game_data->team_1_wr_names[2], "Team 1 WR3");
	strcpy(game_data->team_2_wr_names[2], "Team 2 WR3");
	strcpy(game_data->team_1_wr_names[3], "Team 1 WR4");
	strcpy(game_data->team_2_wr_names[3], "Team 2 WR4");

	game_data->players_named = false;

	for (int i = 0; i < CARDS_IN_A_DECK; i++) {
		game_data->o_cards[i] = 0;
		game_data->d_cards[i] = 0;
	}

	for (int i = 0; i < NORMAL_PLAY_CARDS_TO_DRAW; i++) {
		game_data->o_cards_in_position[i] = 0;
		game_data->d_cards_in_position[i] = 0;
	}

	game_data->kickoff_needed = false;
	game_data->extra_point_needed = false;
	game_data->play_type = 0;
	game_data->formation = 0;
	game_data->show_O_D_Both = 0;
	game_data->matchup_number = 0;

	game_data->pass_play = 0;
	game_data->run_play = 0;

	game_data->interception = false;
	game_data->fumble = false;
	game_data->drive_over = false;

	game_data->team_1_colour = CONSOLE_TEXT_WHITE;
	game_data->team_2_colour = CONSOLE_TEXT_WHITE;

	ResetLineOfFirstDown(game_data);

	game_data->fouls = total_data->fouls;
	game_data->current_game_mode = total_data->current_game_mode;

	/*game stats data*/
	game_data->playoffs = false; // true if we are currently in the playoff part of the season, use to not add to record
	for (int i = 0; i < NUMBER_OF_TEAMS_PER_GAME; i++) {
		for (int j = 0; j < NUMBER_OF_QUARTERS; j++) {
			game_data->quarter_scores[i][j] = 0;
		}
	}
	for (int i = 0; i < NUMBER_OF_TEAMS_PER_GAME; i++) {
		for (int j = 0; j < WIN_LOSS_DRAW; j++) {
			game_data->team_records[i][j] = 0;
		}
	}
	for (int i = 0; i < (NUMBER_OF_QB_NAMES_PER_TEAM * NUMBER_OF_TEAMS_PER_GAME); i++) {
		for (int j = 0; j < NUMBER_OF_QUARTERBACK_STATS; j++) {
			game_data->quarterback_stats[i][j] = 0;
		}
	}
	for (int i = 0; i < (NUMBER_OF_RB_NAMES_PER_TEAM * NUMBER_OF_TEAMS_PER_GAME); i++) {
		for (int j = 0; j < NUMBER_OF_RUNNINGBACK_STATS; j++) {
			game_data->runningback_stats[i][j] = 0;
		}
	}
	for (int i = 0; i < (NUMBER_OF_WR_NAMES_PER_TEAM * NUMBER_OF_TEAMS_PER_GAME); i++) {
		for (int j = 0; j < NUMBER_OF_RECEIVER_STATS; j++) {
			game_data->receiver_stats[i][j] = 0;
		}
	}
	for (int i = 0; i < (CARDS_IN_A_DECK); i++) {
		game_data->o_cards_to_flash[i] = false;
	}
	for (int i = 0; i < (CARDS_IN_A_DECK); i++) {
		game_data->d_cards_to_flash[i] = false;
	}
	game_data->flag_thrown = false;
}

/*
* NAME:        SetTeamColoursTwoMinDrive
* DESCRIPTION: setup team colours
* INPUTS:      game_data
* OUTPUTS:     none
*/
void SetTeamColoursTwoMinDrive(struct game_data* game_data)
{
	ConsoleShowGameScreen(game_data, "Would you like to pick team colours? y/n ", true, 0);
	char input[16];
	fgets(input, sizeof(input), stdin);

	if (input[0] == 'Y' || input[0] == 'y')
	{
		bool team_1_colour_done = false;
		bool team_2_colour_done = false;
		do {
			ConsoleShowGameScreen(game_data, "Enter colour for Team 1 (1-8): ", true, 0);
			char input1[16];
			fgets(input1, sizeof(input1), stdin);
			int team_1_colour = atoi(input1);
			if (team_1_colour < 1)
				team_1_colour = 1;
			else if (team_1_colour > 8)
				team_1_colour = 8;
			game_data->team_1_colour = team_1_colour;

			ConsoleShowGameScreen(game_data, "Are you happy with this colour? y/n ", true, 0);
			char input2[16];
			fgets(input2, sizeof(input2), stdin);
			if (input2[0] == 'Y' || input2[0] == 'y')
				team_1_colour_done = true;
		} while (!team_1_colour_done);

		do {
			ConsoleShowGameScreen(game_data, "Enter colour for Team 2 (1-8): ", true, 0);
			char input3[16];
			fgets(input3, sizeof(input3), stdin);
			int team_2_colour = atoi(input3);
			if (team_2_colour < 1)
				team_2_colour = 1;
			else if (team_2_colour > 8)
				team_2_colour = 8;
			game_data->team_2_colour = team_2_colour;

			ConsoleShowGameScreen(game_data, "Are you happy with this colour? y/n ", true, 0);
			char input4[16];
			fgets(input4, sizeof(input4), stdin);
			if (input4[0] == 'Y' || input4[0] == 'y')
				team_2_colour_done = true;
		} while (!team_2_colour_done);
	}
	else
		game_data->players_named = false;
}

/*
* NAME:        SetTeamNamesTwoMinDrive
* DESCRIPTION: setup team names
* INPUTS:      game_data
* OUTPUTS:     none
*/
void SetTeamNamesTwoMinDrive(struct game_data* game_data)
{
	ConsoleShowGameScreen(game_data, "Would you like to name your teams? y/n ", true, 0);
	char input[16];
	fgets(input, sizeof(input), stdin);

	if (input[0] == 'Y' || input[0] == 'y')
	{
		strcpy(game_data->team_1_name, " ");
		strcpy(game_data->team_2_name, " ");

		ConsoleShowGameScreen(game_data, "Enter name for Team 1: ", true, 0);
		fgets(game_data->team_1_name, sizeof(game_data->team_1_name), stdin);
		game_data->team_1_name[strcspn(game_data->team_1_name, "\n")] = '\0';
		ConsoleShowGameScreen(game_data, "Enter name for Team 2: ", true, 0);
		fgets(game_data->team_2_name, sizeof(game_data->team_2_name), stdin);
		game_data->team_2_name[strcspn(game_data->team_2_name, "\n")] = '\0';
		ConsoleShowGameScreen(game_data, 0, true, 0);
	}
	else
		game_data->players_named = false;
}

/*
* NAME:        SetTeamDeficit
* DESCRIPTION: setup team 1 deficit
* INPUTS:      game_data
* OUTPUTS:     none
*/
void SetTeamDeficit(struct game_data* game_data)
{
	bool score_set = false;
	do {
		ConsoleShowGameScreen(game_data, "Is the deficit 2/6/7: ", true, 0);
		char input[16];
		fgets(input, sizeof(input), stdin);
		int choice = atoi(input);

		if (choice == 2 || choice == 6 || choice == 7)
		{
			game_data->team_2_score = game_data->team_1_score + choice;
			score_set = true;
		}
		else
			ConsoleShowGameScreen(game_data, "Invalid response ", true, 0);
	} while (!score_set);
}

/*
* NAME:        SetTeamStartLine
* DESCRIPTION: setup team 1 start line
* INPUTS:      game_data
* OUTPUTS:     none
*/
void SetTeamStartLine(struct game_data* game_data)
{
	bool yards_set = false;
	do {
		ConsoleShowGameScreen(game_data, "How many yards to the end zone? ", true, 0);
		char input[16];
		fgets(input, sizeof(input), stdin);
		int choice = atoi(input);

		if (choice > 0 && choice < FIELD_WIDTH)
		{
			game_data->line_of_scrimmage = (FIELD_WIDTH - choice) + END_ZONE_WIDTH;
			ResetLineOfFirstDown(game_data);
			yards_set = true;
		}
		else
			ConsoleShowGameScreen(game_data, "Invalid response, must be 100 > yards > 0 ", true, 0);
	} while (!yards_set);
}

/*
* NAME:        SetupTwoMinDrive
* DESCRIPTION: initialises datafor 2 min drive
* INPUTS:      game_data
* OUTPUTS:     none
*/
void SetupTwoMinDrive(struct game_data* game_data, struct total_data* total_data)
{
	TwoMinDriveGameDataConstructor(game_data, total_data);

	SetTeamNamesTwoMinDrive(game_data);

	SetTeamColoursTwoMinDrive(game_data);

	SetTeamDeficit(game_data);

	SetTeamStartLine(game_data);
}