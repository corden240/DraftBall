#include <stdio.h>
#include <stdlib.h>   /* srand, rand, atoi */
#include <time.h>     /* time() */
#include "main.h"
#include <string.h>

void SwitchPossession(struct game_data* game_data);
void ResetLineOfFirstDown(struct game_data* game_data);
void ResetLineOfScrimmage(struct game_data* game_data);
void PlayGame(struct game_data* game_data);
int GetDirectionOfPlay(struct game_data* game_data);
bool OutBackOfEndZone(struct game_data* game_data);

/*
* NAME:        ResetLineOfScrimmage
* DESCRIPTION: make line of scrimmage to starting for team in possession
* INPUTS:      game_data
* OUTPUTS:     none
*/
void ResetLineOfScrimmage(struct game_data* game_data)
{
	if (GetDirectionOfPlay(game_data) == 1)
	{
		game_data->line_of_scrimmage = STARTING_LINE_OF_SCRIMMAGE + END_ZONE_WIDTH;
	}
	else
	{
		game_data->line_of_scrimmage = FIELD_WIDTH - STARTING_LINE_OF_SCRIMMAGE + END_ZONE_WIDTH;
	}
}

/*
* NAME:        MoveLineOfScrimmage
* DESCRIPTION: move LoS by yards depending on who has the ball
* INPUTS:      game_data, yardsa
* OUTPUTS:     none
*/
void MoveLineOfScrimmage(struct game_data* game_data, int yards)
{
	int yards_moved = -yards, final_line_of_scrimmage = 0;

	if (GetDirectionOfPlay(game_data) == 1)
	{
		yards_moved = yards;
	}

	final_line_of_scrimmage = game_data->line_of_scrimmage + yards_moved;

	if (OutBackOfEndZone(game_data))
	{
		if (GetDirectionOfPlay(game_data) == 1)
		{
			final_line_of_scrimmage = (END_ZONE_WIDTH + FIELD_WIDTH + END_ZONE_WIDTH);
		}
		else
		{
			final_line_of_scrimmage = 0;
		}
	}

	do {
		if (final_line_of_scrimmage < game_data->line_of_scrimmage)
			game_data->line_of_scrimmage--;
		else if (final_line_of_scrimmage > game_data->line_of_scrimmage)
			game_data->line_of_scrimmage++;

		ConsoleShowGameScreen(game_data, 0, (GAME_SPEED / 10));
	} while (final_line_of_scrimmage != game_data->line_of_scrimmage);
}

/*
* NAME:        ResetLineOfFirstDown
* DESCRIPTION: make line of first down 10 yards ahead of current LoS
* INPUTS:      game_data
* OUTPUTS:     none
*/
void ResetLineOfFirstDown(struct game_data* game_data)
{
	int first_down = -YARDS_PER_FIRST_DOWN;

	if (GetDirectionOfPlay(game_data) == 1)
	{
		first_down = YARDS_PER_FIRST_DOWN;
	}

	game_data->line_of_first_down = game_data->line_of_scrimmage + first_down;

	if (game_data->line_of_first_down < END_ZONE_WIDTH)
		game_data->line_of_first_down = END_ZONE_WIDTH;
	else if (game_data->line_of_first_down > (PITCH_WIDTH - END_ZONE_WIDTH))
		game_data->line_of_first_down = (PITCH_WIDTH - END_ZONE_WIDTH);
}

/*
* NAME:        SwitchEndLineOfFirstDown
* DESCRIPTION: switch direction of play 1st down line
* INPUTS:      game_data
* OUTPUTS:     none
*/
void SwitchEndLineOfFirstDown(struct game_data* game_data)
{
	int halfway_line = (END_ZONE_WIDTH + (FIELD_WIDTH / 2));

	if ((game_data->line_of_first_down - halfway_line) < 0)
	{
		game_data->line_of_first_down = halfway_line + (halfway_line - game_data->line_of_first_down - END_ZONE_WIDTH);
	}
	else if ((game_data->line_of_first_down - halfway_line) > 0)
	{
		game_data->line_of_first_down = halfway_line - (game_data->line_of_first_down - halfway_line);
	}
	// else in the case that it is 0 then the line does not need to move anyway
}

/*
* NAME:        SwitchEndLineOfScrimmage
* DESCRIPTION: switch direction of play LoS
* INPUTS:      game_data
* OUTPUTS:     none
*/
void SwitchEndLineOfScrimmage(struct game_data* game_data)
{
	int halfway_line = (END_ZONE_WIDTH + (FIELD_WIDTH / 2));

	if ((game_data->line_of_scrimmage - halfway_line) < 0)
	{
		game_data->line_of_scrimmage = halfway_line + (halfway_line - game_data->line_of_scrimmage - END_ZONE_WIDTH);
	}
	else if ((game_data->line_of_scrimmage - halfway_line) > 0)
	{
		game_data->line_of_scrimmage = halfway_line - (game_data->line_of_scrimmage - halfway_line);
	}
	// else in the case that it is 0 then the line does not need to move anyway
}

/*
* NAME:        SwitchEnds
* DESCRIPTION: switch direction of play
* INPUTS:      game_data
* OUTPUTS:     none
*/
void SwitchEnds(struct game_data* game_data)
{
	if (game_data->team_1_direction == 1)
	{
		game_data->team_1_direction = 2;
		game_data->team_2_direction = 1;
	}
	else
	{
		game_data->team_1_direction = 1;
		game_data->team_2_direction = 2;
	}

	SwitchEndLineOfScrimmage(game_data);
	SwitchEndLineOfFirstDown(game_data);
}

/*
* NAME:        SwitchPossession
* DESCRIPTION: switch team with possession
* INPUTS:      game_data
* OUTPUTS:     none
*/
void SwitchPossession(struct game_data* game_data)
{
	ConsoleShowGameScreen(game_data, "Shared deck shuffled on change of possession ", GAME_SPEED * 20);
	TableWeightEnableAll(shared_deck);

	game_data->team_not_in_possession = game_data->team_in_possession;
	game_data->team_in_possession = game_data->team_in_possession - 1 ? game_data->team_in_possession - 1 : game_data->team_in_possession + 1;

	/* two-minute drill ends when the defence has the ball */
	if (game_data->current_game_mode == 2 && game_data->team_in_possession == 2)
		game_data->drive_over = true;

	return;
}

/*
* NAME:        CoinToss
* DESCRIPTION: do coin toss
* INPUTS:      game_data
* OUTPUTS:     none
*/
void CoinToss(struct game_data* game_data)
{
	ConsoleShowGameScreen(game_data, "Coin toss: ", GAME_SPEED * 10);
	for (int i = 0; i < NUMER_OF_COIN_TOSSES_ANIMATION; i++)
	{
		ConsoleShowGameScreen(game_data, "\\", GAME_SPEED / 5);
		ConsoleShowGameScreen(game_data, "|", GAME_SPEED / 5);
		ConsoleShowGameScreen(game_data, "/", GAME_SPEED / 5);
		ConsoleShowGameScreen(game_data, "-", GAME_SPEED / 5);
	}

	ConsoleShowGameScreen(game_data, 0, GAME_SPEED * 20);

	int team_that_won_cointoss = rand() % 2 + 1; // 1 or 2

	char buf[256];
	snprintf(buf, sizeof(buf), "%s won the coin toss! Punt or receive (1 or 2): ",
		(team_that_won_cointoss == 1) ? game_data->team_1_name : game_data->team_2_name);
	ConsoleShowGameScreen(game_data, buf, GAME_SPEED * 10);

	char input[16];
	if (fgets(input, sizeof(input), stdin)) {
		int pick = atoi(input);
		if (pick < 1 || pick > 2) pick = 1; // validate / default

		if (pick == 2)
			game_data->team_started_with_possession = team_that_won_cointoss - 1 ? team_that_won_cointoss - 1 : team_that_won_cointoss + 1;
		else
			game_data->team_started_with_possession = team_that_won_cointoss;

		game_data->team_in_possession = game_data->team_started_with_possession;
		game_data->team_not_in_possession = (game_data->team_in_possession - 1) ? 1 : 2;

		if (game_data->team_started_with_possession == 1)
		{
			game_data->line_of_scrimmage = STARTING_LINE_OF_SCRIMMAGE + END_ZONE_WIDTH;
			game_data->line_of_first_down = game_data->line_of_scrimmage + YARDS_PER_FIRST_DOWN;
		}
		else
		{
			game_data->line_of_scrimmage = FIELD_WIDTH - STARTING_LINE_OF_SCRIMMAGE + END_ZONE_WIDTH;
			game_data->line_of_first_down = game_data->line_of_scrimmage - YARDS_PER_FIRST_DOWN;
		}
		ConsoleShowGameScreen(game_data, 0, GAME_SPEED);
	}
}

/*
* NAME:        SetTeamColours
* DESCRIPTION: setup team colours
* INPUTS:      game_data
* OUTPUTS:     none
*/
void SetTeamColours(struct game_data* game_data)
{
	ConsoleShowGameScreen(game_data, "Would you like to pick team colours? y/n ", 0);
	char input[16];
	fgets(input, sizeof(input), stdin);

	if (input[0] == 'Y' || input[0] == 'y')
	{
		bool team_1_colour_done = false;
		bool team_2_colour_done = false;
		do {
			ConsoleShowGameScreen(game_data, "Enter colour for Team 1 (1 - 8): ", 0);
			char input1[16];
			fgets(input1, sizeof(input1), stdin);
			int team_1_colour = atoi(input1);
			if (team_1_colour < 1)
				team_1_colour = 1;
			else if (team_1_colour > 8)
				team_1_colour = 8;
			game_data->team_1_colour = team_1_colour;

			ConsoleShowGameScreen(game_data, "Are you happy with this colour? y/n ", 0);
			char input2[16];
			fgets(input2, sizeof(input2), stdin);
			if (input2[0] == 'Y' || input2[0] == 'y')
				team_1_colour_done = true;
		} while (!team_1_colour_done);

		do {
			ConsoleShowGameScreen(game_data, "Enter colour for Team 2 (1 - 8): ", 0);
			char input3[16];
			fgets(input3, sizeof(input3), stdin);
			int team_2_colour = atoi(input3);
			if (team_2_colour < 1)
				team_2_colour = 1;
			else if (team_2_colour > 8)
				team_2_colour = 8;
			game_data->team_2_colour = team_2_colour;

			ConsoleShowGameScreen(game_data, "Are you happy with this colour? y/n ", 0);
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
* NAME:        SetTeamNames
* DESCRIPTION: setup team names
* INPUTS:      game_data
* OUTPUTS:     none
*/
void SetTeamNames(struct game_data* game_data)
{
	ConsoleShowGameScreen(game_data, "Would you like to name your teams? y/n ", 0);
	char input[16];
	fgets(input, sizeof(input), stdin);

	if (input[0] == 'Y' || input[0] == 'y')
	{
		strcpy(game_data->team_1_name, " ");
		strcpy(game_data->team_2_name, " ");

		ConsoleShowGameScreen(game_data, "Enter name for Team 1: ", 0);
		fgets(game_data->team_1_name, sizeof(game_data->team_1_name), stdin);
		game_data->team_1_name[strcspn(game_data->team_1_name, "\n")] = '\0';
		ConsoleShowGameScreen(game_data, "Enter name for Team 2: ", 0);
		fgets(game_data->team_2_name, sizeof(game_data->team_2_name), stdin);
		game_data->team_2_name[strcspn(game_data->team_2_name, "\n")] = '\0';
		ConsoleShowGameScreen(game_data, 0, 0);
	}
	else
		game_data->players_named = false;
}

/*
* NAME:        GetPlayerNames
* DESCRIPTION: setup player names
* INPUTS:      game_data
* OUTPUTS:     none
*/
void SetPlayerNames(struct game_data* game_data)
{
	ConsoleShowGameScreen(game_data, "Would you like to name your players? y/n ", 0);
	char input[16];
	fgets(input, sizeof(input), stdin);

	if (input[0] == 'Y' || input[0] == 'y')
	{
		game_data->players_named = true;
		strcpy(game_data->team_1_qb_names[0], " ");
		strcpy(game_data->team_2_qb_names[0], " ");

		strcpy(game_data->team_1_rb_names[0], " ");
		strcpy(game_data->team_2_rb_names[0], " ");
		strcpy(game_data->team_1_rb_names[1], " ");
		strcpy(game_data->team_2_rb_names[1], " ");

		strcpy(game_data->team_1_wr_names[0], " ");
		strcpy(game_data->team_2_wr_names[0], " ");
		strcpy(game_data->team_1_wr_names[1], " ");
		strcpy(game_data->team_2_wr_names[1], " ");
		strcpy(game_data->team_1_wr_names[2], " ");
		strcpy(game_data->team_2_wr_names[2], " ");
		strcpy(game_data->team_1_wr_names[3], " ");
		strcpy(game_data->team_2_wr_names[3], " ");

		for (int i = 0; i < NUMBER_OF_PLAYER_NAMES_PER_TEAM; i++)
		{
			if (i == 0)
			{
				char buf[256];
				snprintf(buf, sizeof(buf), "Enter name for Team 1 QB Player %d: ", i + 1);
				ConsoleShowGameScreen(game_data, buf, 0);
				fgets(game_data->team_1_qb_names[i], sizeof(game_data->team_1_qb_names[i]), stdin);
				game_data->team_1_qb_names[i][strcspn(game_data->team_1_qb_names[i], "\n")] = '\0';
			}
			else if (i < (NUMBER_OF_QB_NAMES_PER_TEAM + NUMBER_OF_RB_NAMES_PER_TEAM))
			{
				char buf[256];
				snprintf(buf, sizeof(buf), "Enter name for Team 1 RB Player %d: ", i);
				ConsoleShowGameScreen(game_data, buf, 0);
				fgets(game_data->team_1_rb_names[i - NUMBER_OF_QB_NAMES_PER_TEAM], sizeof(game_data->team_1_rb_names[i - NUMBER_OF_QB_NAMES_PER_TEAM]), stdin);
				game_data->team_1_rb_names[i - NUMBER_OF_QB_NAMES_PER_TEAM][strcspn(game_data->team_1_rb_names[i - NUMBER_OF_QB_NAMES_PER_TEAM], "\n")] = '\0';
			}
			else
			{
				char buf[256];
				snprintf(buf, sizeof(buf), "Enter name for Team 1 WR Player %d: ", i - (NUMBER_OF_QB_NAMES_PER_TEAM + NUMBER_OF_RB_NAMES_PER_TEAM) + 1);
				ConsoleShowGameScreen(game_data, buf, 0);
				fgets(game_data->team_1_wr_names[i - (NUMBER_OF_QB_NAMES_PER_TEAM + NUMBER_OF_RB_NAMES_PER_TEAM)], sizeof(game_data->team_1_wr_names[i - (NUMBER_OF_QB_NAMES_PER_TEAM + NUMBER_OF_RB_NAMES_PER_TEAM)]), stdin);
				game_data->team_1_wr_names[i - (NUMBER_OF_QB_NAMES_PER_TEAM + NUMBER_OF_RB_NAMES_PER_TEAM)][strcspn(game_data->team_1_wr_names[i - (NUMBER_OF_QB_NAMES_PER_TEAM + NUMBER_OF_RB_NAMES_PER_TEAM)], "\n")] = '\0';
			}
		}
		for (int i = 0; i < NUMBER_OF_PLAYER_NAMES_PER_TEAM; i++)
		{
			if (i == 0)
			{
				char buf[256];
				snprintf(buf, sizeof(buf), "Enter name for Team 2 QB Player %d: ", i + 1);
				ConsoleShowGameScreen(game_data, buf, 0);
				fgets(game_data->team_2_qb_names[i], sizeof(game_data->team_2_qb_names[i]), stdin);
				game_data->team_2_qb_names[i][strcspn(game_data->team_2_qb_names[i], "\n")] = '\0';
			}
			else if (i < (NUMBER_OF_QB_NAMES_PER_TEAM + NUMBER_OF_RB_NAMES_PER_TEAM))
			{
				char buf[256];
				snprintf(buf, sizeof(buf), "Enter name for Team 2 RB Player %d: ", i);
				ConsoleShowGameScreen(game_data, buf, 0);
				fgets(game_data->team_2_rb_names[i - NUMBER_OF_QB_NAMES_PER_TEAM], sizeof(game_data->team_2_rb_names[i - NUMBER_OF_QB_NAMES_PER_TEAM]), stdin);
				game_data->team_2_rb_names[i - NUMBER_OF_QB_NAMES_PER_TEAM][strcspn(game_data->team_2_rb_names[i - NUMBER_OF_QB_NAMES_PER_TEAM], "\n")] = '\0';
			}
			else
			{
				char buf[256];
				snprintf(buf, sizeof(buf), "Enter name for Team 2 WR Player %d: ", i - (NUMBER_OF_QB_NAMES_PER_TEAM + NUMBER_OF_RB_NAMES_PER_TEAM) + 1);
				ConsoleShowGameScreen(game_data, buf, 0);
				fgets(game_data->team_2_wr_names[i - (NUMBER_OF_QB_NAMES_PER_TEAM + NUMBER_OF_RB_NAMES_PER_TEAM)], sizeof(game_data->team_2_wr_names[i - (NUMBER_OF_QB_NAMES_PER_TEAM + NUMBER_OF_RB_NAMES_PER_TEAM)]), stdin);
				game_data->team_2_wr_names[i - (NUMBER_OF_QB_NAMES_PER_TEAM + NUMBER_OF_RB_NAMES_PER_TEAM)][strcspn(game_data->team_2_wr_names[i - (NUMBER_OF_QB_NAMES_PER_TEAM + NUMBER_OF_RB_NAMES_PER_TEAM)], "\n")] = '\0';
			}
		}
	}
	else
	{
		game_data->players_named = false;
	}
	ConsoleShowGameScreen(game_data, 0, 0);
}

/*
* NAME:        ExhibitionGameDataConstructor
* DESCRIPTION: starts all of game_data data to default settings
* INPUTS:      game_data
* OUTPUTS:     none
*/
void ExhibitionGameDataConstructor(struct game_data* game_data, struct total_data* total_data)
{
	game_data->line_of_scrimmage = STARTING_LINE_OF_SCRIMMAGE + END_ZONE_WIDTH;
	game_data->line_of_first_down = 0;
	game_data->down = 1;
	game_data->team_in_possession = 0;
	game_data->team_not_in_possession = 0;
	game_data->team_1_score = 0;
	game_data->team_2_score = 0;
	game_data->quarter = 1;
	game_data->plays_remaining_in_quarter = total_data->plays_per_quarter;
	game_data->plays_per_quarter = total_data->plays_per_quarter;
	for (int i = 0; i < NUMBER_OF_DECKS; i++) {
		game_data->shuffle_deck_needed[i] = 0;
	}
	game_data->team_1_timeouts_remaining = TIMEOUTS_PER_TEAM_PER_HALF;
	game_data->team_2_timeouts_remaining = TIMEOUTS_PER_TEAM_PER_HALF;
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

	game_data->kickoff_needed = true;
	game_data->extra_point_needed = false;
	game_data->play_type = 0;
	game_data->formation = 0;
	game_data->show_O_D_Both = 0;
	game_data->matchup_number = 0;

	game_data->pass_play = 0;
	game_data->run_play = 0;

	game_data->interception = false;
	game_data->fumble = false;

	game_data->team_1_colour = CONSOLE_TEXT_WHITE;
	game_data->team_2_colour = CONSOLE_TEXT_WHITE;

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

	game_data->drive_over = false;
}

/*
* NAME:        SetupExhibitionGame
* DESCRIPTION: initialises data such as team names, coin toss, starting possession etc
* INPUTS:      game_data
* OUTPUTS:     none
*/
void SetupExhibitionGame(struct game_data* game_data, struct total_data* total_data)
{
	ExhibitionGameDataConstructor(game_data, total_data);

	SetTeamNames(game_data);

	SetTeamColours(game_data);

	SetPlayerNames(game_data);

	CoinToss(game_data);
}
