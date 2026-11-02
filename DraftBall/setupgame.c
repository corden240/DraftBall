#include <stdio.h>
#include <stdlib.h>   /* srand, rand, atoi */
#include <time.h>     /* time() */
#include "main.h"
#include <string.h>
#include <windows.h>
#include "teamdata.h"

void SwitchPossession(struct game_data* game_data);
void ResetLineOfFirstDown(struct game_data* game_data);
void ResetLineOfScrimmage(struct game_data* game_data);
void PlayGame(struct game_data* game_data);
int GetDirectionOfPlay(struct game_data* game_data);
bool OutBackOfEndZone(struct game_data* game_data);
void ShowMenuTitle();
void PrintTextWithDelays(char txt[1000]);
void ResetScreen();

/*
* NAME:        ResetScreenReadyForMenuPrint
* DESCRIPTION: print title and then space ready for text
* INPUTS:      none
* OUTPUTS:     none
*/
void ResetScreenReadyForMenuPrint(void)
{
	system("cls");
	ResetScreen();
	ShowMenuTitle();

	printf("\r\n\r\n                                        ");
	printf("     ");
}

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

		ConsoleShowGameScreen(game_data, 0, true, (GAME_SPEED / 10));
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
	ConsoleShowGameScreen(game_data, "Shared deck shuffled on change of possession ", true, GAME_SPEED * 20);
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
	system("cls");
	ConsoleShowGameScreen(game_data, 0, true, GAME_SPEED * 30);
	ConsoleShowGameScreen(game_data, "Coin toss: ", true, GAME_SPEED * 20);
	for (int i = 0; i < NUMER_OF_COIN_TOSSES_ANIMATION; i++)
	{
		ConsoleShowGameScreen(game_data, "\\", true, GAME_SPEED / 5);
		ConsoleShowGameScreen(game_data,  "|", true, GAME_SPEED / 5);
		ConsoleShowGameScreen(game_data,  "/", true, GAME_SPEED / 5);
		ConsoleShowGameScreen(game_data,  "-", true, GAME_SPEED / 5);
	}

	ConsoleShowGameScreen(game_data, 0, true, GAME_SPEED * 20);

	int team_that_won_cointoss = rand() % 2 + 1; // 1 or 2

	char buf[256];
	snprintf(buf, sizeof(buf), "%s won the toss! Kickoff or receive? (1 or 2): ",
		(team_that_won_cointoss == 1) ? game_data->team_1_name : game_data->team_2_name);
	ConsoleShowGameScreen(game_data, buf, true, GAME_SPEED * 10);

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
		ConsoleShowGameScreen(game_data, 0, true, GAME_SPEED);
	}
}

/*
* NAME:        SetExhibitionTeamColours
* DESCRIPTION: setup team colours
* INPUTS:      game_data
* OUTPUTS:     none
*/
void SetExhibitionTeamColours(struct game_data* game_data)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
	bool team_1_colour_done = false;
	bool team_2_colour_done = false;
	do {
		ResetScreenReadyForMenuPrint();
		PrintTextWithDelays("Enter colour for Team 1 (1-8): ");
		char input1[16];
		fgets(input1, sizeof(input1), stdin);
		int team_1_colour = atoi(input1);
		if (team_1_colour < 1)
			team_1_colour = 1;
		else if (team_1_colour > 8)
			team_1_colour = 8;
		game_data->team_1_colour = team_1_colour;

		ResetScreenReadyForMenuPrint();
		SetConsoleTextAttribute(hConsole, game_data->team_1_colour);
		PrintTextWithDelays("Are you happy with this colour? y/n ");
		SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
		char input2[16];
		fgets(input2, sizeof(input2), stdin);
		if (input2[0] == 'Y' || input2[0] == 'y')
			team_1_colour_done = true;
	} while (!team_1_colour_done);

	do {
		ResetScreenReadyForMenuPrint();
		PrintTextWithDelays("Enter colour for Team 2 (1-8): ");
		char input3[16];
		fgets(input3, sizeof(input3), stdin);
		int team_2_colour = atoi(input3);
		if (team_2_colour < 1)
			team_2_colour = 1;
		else if (team_2_colour > 8)
			team_2_colour = 8;
		game_data->team_2_colour = team_2_colour;

		ResetScreenReadyForMenuPrint();
		SetConsoleTextAttribute(hConsole, game_data->team_2_colour);
		PrintTextWithDelays("Are you happy with this colour? y/n ");
		SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
		char input4[16];
		fgets(input4, sizeof(input4), stdin);
		if (input4[0] == 'Y' || input4[0] == 'y')
			team_2_colour_done = true;
	} while (!team_2_colour_done);

	game_data->players_named = true;
}

/*
* NAME:        SetExhibitionTeamNames
* DESCRIPTION: setup team names
* INPUTS:      game_data
* OUTPUTS:     none
*/
void SetExhibitionTeamNames(struct game_data* game_data)
{
	ResetScreenReadyForMenuPrint();

	strcpy(game_data->team_1_name, " ");
	strcpy(game_data->team_2_name, " ");

	PrintTextWithDelays("Enter name for Team 1: ");
	fgets(game_data->team_1_name, sizeof(game_data->team_1_name), stdin);
	game_data->team_1_name[strcspn(game_data->team_1_name, "\n")] = '\0';
	
	ResetScreenReadyForMenuPrint();
	PrintTextWithDelays("Enter name for Team 2: ");
	fgets(game_data->team_2_name, sizeof(game_data->team_2_name), stdin);
	game_data->team_2_name[strcspn(game_data->team_2_name, "\n")] = '\0';
}

/*
* NAME:        SetExhibitionPlayerNames
* DESCRIPTION: setup player names
* INPUTS:      game_data
* OUTPUTS:     none
*/
void SetExhibitionPlayerNames(struct game_data* game_data)
{
	ResetScreenReadyForMenuPrint();

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
			ResetScreenReadyForMenuPrint();
			char buf[256];
			snprintf(buf, sizeof(buf), "Enter name for Team 1 QB Player %d: ", i + 1);
			PrintTextWithDelays(buf);
			fgets(game_data->team_1_qb_names[i], sizeof(game_data->team_1_qb_names[i]), stdin);
			game_data->team_1_qb_names[i][strcspn(game_data->team_1_qb_names[i], "\n")] = '\0';
		}
		else if (i < (NUMBER_OF_QB_NAMES_PER_TEAM + NUMBER_OF_RB_NAMES_PER_TEAM))
		{
			ResetScreenReadyForMenuPrint();
			char buf[256];
			snprintf(buf, sizeof(buf), "Enter name for Team 1 RB Player %d: ", i);
			printf(buf);
			fgets(game_data->team_1_rb_names[i - NUMBER_OF_QB_NAMES_PER_TEAM], sizeof(game_data->team_1_rb_names[i - NUMBER_OF_QB_NAMES_PER_TEAM]), stdin);
			game_data->team_1_rb_names[i - NUMBER_OF_QB_NAMES_PER_TEAM][strcspn(game_data->team_1_rb_names[i - NUMBER_OF_QB_NAMES_PER_TEAM], "\n")] = '\0';
		}
		else
		{
			ResetScreenReadyForMenuPrint();
			char buf[256];
			snprintf(buf, sizeof(buf), "Enter name for Team 1 WR Player %d: ", i - (NUMBER_OF_QB_NAMES_PER_TEAM + NUMBER_OF_RB_NAMES_PER_TEAM) + 1);
			printf(buf);
			fgets(game_data->team_1_wr_names[i - (NUMBER_OF_QB_NAMES_PER_TEAM + NUMBER_OF_RB_NAMES_PER_TEAM)], sizeof(game_data->team_1_wr_names[i - (NUMBER_OF_QB_NAMES_PER_TEAM + NUMBER_OF_RB_NAMES_PER_TEAM)]), stdin);
			game_data->team_1_wr_names[i - (NUMBER_OF_QB_NAMES_PER_TEAM + NUMBER_OF_RB_NAMES_PER_TEAM)][strcspn(game_data->team_1_wr_names[i - (NUMBER_OF_QB_NAMES_PER_TEAM + NUMBER_OF_RB_NAMES_PER_TEAM)], "\n")] = '\0';
		}
	}

	for (int i = 0; i < NUMBER_OF_PLAYER_NAMES_PER_TEAM; i++)
	{
		if (i == 0)
		{
			ResetScreenReadyForMenuPrint();
			char buf[256];
			snprintf(buf, sizeof(buf), "Enter name for Team 2 QB Player %d: ", i + 1);
			printf(buf);
			fgets(game_data->team_2_qb_names[i], sizeof(game_data->team_2_qb_names[i]), stdin);
			game_data->team_2_qb_names[i][strcspn(game_data->team_2_qb_names[i], "\n")] = '\0';
		}
		else if (i < (NUMBER_OF_QB_NAMES_PER_TEAM + NUMBER_OF_RB_NAMES_PER_TEAM))
		{
			ResetScreenReadyForMenuPrint();
			char buf[256];
			snprintf(buf, sizeof(buf), "Enter name for Team 2 RB Player %d: ", i);
			printf(buf);
			fgets(game_data->team_2_rb_names[i - NUMBER_OF_QB_NAMES_PER_TEAM], sizeof(game_data->team_2_rb_names[i - NUMBER_OF_QB_NAMES_PER_TEAM]), stdin);
			game_data->team_2_rb_names[i - NUMBER_OF_QB_NAMES_PER_TEAM][strcspn(game_data->team_2_rb_names[i - NUMBER_OF_QB_NAMES_PER_TEAM], "\n")] = '\0';
		}
		else
		{
			ResetScreenReadyForMenuPrint();
			char buf[256];
			snprintf(buf, sizeof(buf), "Enter name for Team 2 WR Player %d: ", i - (NUMBER_OF_QB_NAMES_PER_TEAM + NUMBER_OF_RB_NAMES_PER_TEAM) + 1);
			printf(buf);
			fgets(game_data->team_2_wr_names[i - (NUMBER_OF_QB_NAMES_PER_TEAM + NUMBER_OF_RB_NAMES_PER_TEAM)], sizeof(game_data->team_2_wr_names[i - (NUMBER_OF_QB_NAMES_PER_TEAM + NUMBER_OF_RB_NAMES_PER_TEAM)]), stdin);
			game_data->team_2_wr_names[i - (NUMBER_OF_QB_NAMES_PER_TEAM + NUMBER_OF_RB_NAMES_PER_TEAM)][strcspn(game_data->team_2_wr_names[i - (NUMBER_OF_QB_NAMES_PER_TEAM + NUMBER_OF_RB_NAMES_PER_TEAM)], "\n")] = '\0';
		}
	}

	game_data->players_named = true;

	ConsoleShowGameScreen(game_data, 0, true, 0);
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
	game_data->flag_thrown = false;

	game_data->one_player = true;
	game_data->two_player = false;

	game_data->team_1_id = 10;
	game_data->team_2_id = 21;
}

/*
* NAME:        GetTeam1LastID
* DESCRIPTION: get correct team id
* INPUTS:      game data
* OUTPUTS:     none
*/
int GetTeam1LastID(struct game_data* game_data)
{
	if (game_data->team_1_id == 0)
		return (LEAGUE_TEAM_COUNT - 1);
	else
		return (game_data->team_1_id - 1);
}

/*
* NAME:        GetTeam1CurrentID
* DESCRIPTION: get correct team id
* INPUTS:      game data
* OUTPUTS:     none
*/
int GetTeam1CurrentID(struct game_data* game_data)
{
	return game_data->team_1_id;
}

/*
* NAME:        GetTeam1NextID
* DESCRIPTION: get correct team id
* INPUTS:      game data
* OUTPUTS:     none
*/
int GetTeam1NextID(struct game_data* game_data)
{
	if (game_data->team_1_id == (LEAGUE_TEAM_COUNT - 1))
		return 0;
	else
		return (game_data->team_1_id + 1);
}

/*
* NAME:        GetTeam2LastID
* DESCRIPTION: get correct team id
* INPUTS:      game data
* OUTPUTS:     none
*/
int GetTeam2LastID(struct game_data* game_data)
{
	if (game_data->team_2_id == 0)
		return (LEAGUE_TEAM_COUNT - 1);
	else
		return (game_data->team_2_id - 1);
}

/*
* NAME:        GetTeam2CurrentID
* DESCRIPTION: get correct team id
* INPUTS:      game data
* OUTPUTS:     none
*/
int GetTeam2CurrentID(struct game_data* game_data)
{
	return game_data->team_2_id;
}

/*
* NAME:        GetTeam2NextID
* DESCRIPTION: get correct team id
* INPUTS:      game data
* OUTPUTS:     none
*/
int GetTeam2NextID(struct game_data* game_data)
{
	if (game_data->team_2_id == (LEAGUE_TEAM_COUNT - 1))
		return 0;
	else
		return (game_data->team_2_id + 1);
}

/*
* NAME:        PrintTeamNameField
* DESCRIPTION: print 1 name field
* INPUTS:      hConsole
* OUTPUTS:     none
*/
static void PrintTeamNameField(
	HANDLE hConsole,
	const struct team_data* team,
	WORD name_attributes)
{
	size_t name_length = strlen(team->name);

	if (name_length > TEAM_NAME_FIELD_WIDTH)
		name_length = TEAM_NAME_FIELD_WIDTH;

	SetConsoleTextAttribute(hConsole, name_attributes);
	printf("%.*s", (int)name_length, team->name);

	SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
	for (size_t i = name_length; i < TEAM_NAME_FIELD_WIDTH; i++)
		putchar(' ');
}

/*
* NAME:        PrintThreeTeamNames
* DESCRIPTION: print last current and next team names
* INPUTS:      ids and hConsole
* OUTPUTS:     none
*/
static void PrintThreeTeamNames(
	HANDLE hConsole,
	int last_id,
	int current_id,
	int next_id)
{
	const struct team_data* last = &default_league.teams[last_id];
	const struct team_data* current = &default_league.teams[current_id];
	const struct team_data* next = &default_league.teams[next_id];

	PrintTeamNameField(hConsole, last, (WORD)last->colour);
	putchar(' ');

	PrintTeamNameField(
		hConsole,
		current,
		(WORD)(CONSOLE_TEXT_WHITE | (current->colour << 4)));
	putchar(' ');

	PrintTeamNameField(hConsole, next, (WORD)next->colour);
}

/*
* NAME:        ShowExhibitionGameSetupOptions
* DESCRIPTION: game menu options
* INPUTS:      current selection, delay or not
* OUTPUTS:     none
*/
static void ShowExhibitionGameSetupOptions(int current_menu_selection, bool first_text, struct game_data* game_data)
{
	ResetScreen();
	ShowMenuTitle();
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	int team_1_last_id = GetTeam1LastID(game_data);
	int team_1_current_id = GetTeam1CurrentID(game_data);
	int team_1_next_id = GetTeam1NextID(game_data);
	int team_2_last_id = GetTeam2LastID(game_data);
	int team_2_current_id = GetTeam2CurrentID(game_data);
	int team_2_next_id = GetTeam2NextID(game_data);

	printf("\r\n\r\n                                        ");
	printf("            ");
	if (first_text)
		PrintTextWithDelays("Game Settings");
	else
		printf("Game Settings");

	for (int i = 0; i < EXHIBITION_GAME_SETTINGS_OPTIONS_NUMBER; i++)
	{
		printf("\r\n\r\n                                        ");
		if (i == 0)
		{
			printf("          ");
			if (current_menu_selection == i)
				SetConsoleTextAttribute(hConsole, CONSOLE_BG_WHITE | CONSOLE_TEXT_GREEN);
			if (first_text)
				PrintTextWithDelays("Players");
			else
				printf("Players");
			SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
			printf("    ");
			if (game_data->one_player)
				SetConsoleTextAttribute(hConsole, CONSOLE_BG_WHITE | CONSOLE_TEXT_GREEN);
			printf("One");
			SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
			printf(" / ");
			if (game_data->two_player)
				SetConsoleTextAttribute(hConsole, CONSOLE_BG_WHITE | CONSOLE_TEXT_GREEN);
			printf("Two");
		}
		else if (i == 1)
		{
			printf("           ");
			if (current_menu_selection == i)
				SetConsoleTextAttribute(hConsole, CONSOLE_BG_WHITE | CONSOLE_TEXT_GREEN);
			if (first_text)
				PrintTextWithDelays("Team 1:");
			else
				printf("Team 1:");
			SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
			printf(" ");
			PrintThreeTeamNames(hConsole, team_1_last_id, team_1_current_id, team_1_next_id);
			printf("              ");
		}
		else if (i == 2)
		{
			printf("          ");
			if (current_menu_selection == i)
				SetConsoleTextAttribute(hConsole, CONSOLE_BG_WHITE | CONSOLE_TEXT_GREEN);
			if (first_text)
				PrintTextWithDelays("Team 2:");
			else
				printf("Team 2:");
			SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
			printf(" ");

			PrintThreeTeamNames(hConsole, team_2_last_id, team_2_current_id, team_2_next_id);
			printf("              ");
		}
		else if (i == 3)
		{
			printf("          ");
			if (current_menu_selection == i)
				SetConsoleTextAttribute(hConsole, CONSOLE_BG_WHITE | CONSOLE_TEXT_GREEN);
			if (first_text)
				PrintTextWithDelays("Fouls");
			else
				printf("Fouls");
			SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
			printf("    ");
			if (game_data->fouls)
				SetConsoleTextAttribute(hConsole, CONSOLE_BG_WHITE | CONSOLE_TEXT_GREEN);
			printf("On");
			SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
			printf(" / ");
			if (!game_data->fouls)
				SetConsoleTextAttribute(hConsole, CONSOLE_BG_WHITE | CONSOLE_TEXT_GREEN);
			printf("Off");
		}
		else if (i == 4)
		{
			if (current_menu_selection == i)
				SetConsoleTextAttribute(hConsole, CONSOLE_BG_WHITE | CONSOLE_TEXT_GREEN);
			if (first_text)
				PrintTextWithDelays("Plays per quarter");
			else
				printf("Plays per quarter");

			SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
			printf("     ");

			if (game_data->plays_per_quarter == MIN_PLAYS_PER_QUARTER)
				SetConsoleTextAttribute(hConsole, CONSOLE_BG_WHITE | CONSOLE_TEXT_GREEN);
			printf("5");
			SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);

			printf("  ");
			if (game_data->plays_per_quarter == MIN_PLAYS_PER_QUARTER + STEPS_PLAYS_PER_QUARTER)
				SetConsoleTextAttribute(hConsole, CONSOLE_BG_WHITE | CONSOLE_TEXT_GREEN);
			printf("10");
			SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);

			printf("  ");
			if (game_data->plays_per_quarter == MIN_PLAYS_PER_QUARTER + (STEPS_PLAYS_PER_QUARTER * 2))
				SetConsoleTextAttribute(hConsole, CONSOLE_BG_WHITE | CONSOLE_TEXT_GREEN);
			printf("15");
			SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);

			printf("  ");
			if (game_data->plays_per_quarter == MIN_PLAYS_PER_QUARTER + (STEPS_PLAYS_PER_QUARTER * 3))
				SetConsoleTextAttribute(hConsole, CONSOLE_BG_WHITE | CONSOLE_TEXT_GREEN);
			printf("20");
			SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);

			printf("  ");
			if (game_data->plays_per_quarter == MIN_PLAYS_PER_QUARTER + ((5) * 4))
				SetConsoleTextAttribute(hConsole, CONSOLE_BG_WHITE | CONSOLE_TEXT_GREEN);
			printf("25");
			SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);

			printf("  ");
			if (game_data->plays_per_quarter == MIN_PLAYS_PER_QUARTER + (STEPS_PLAYS_PER_QUARTER * 5))
				SetConsoleTextAttribute(hConsole, CONSOLE_BG_WHITE | CONSOLE_TEXT_GREEN);
			printf("30");
			SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
		}
		else if (i == 5)
		{
			printf("             ");
			if (current_menu_selection == i)
				SetConsoleTextAttribute(hConsole, CONSOLE_BG_WHITE | CONSOLE_TEXT_GREEN);
			if (first_text)
				PrintTextWithDelays("Start game");
			else
				printf("Start game");
		}
		else if (i == 6)
		{
			printf("                ");
			if (current_menu_selection == i)
				SetConsoleTextAttribute(hConsole, CONSOLE_BG_WHITE | CONSOLE_TEXT_GREEN);
			if (first_text)
				PrintTextWithDelays("Back");
			else
				printf("Back");
		}
		SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
	}
}

/*
* NAME:        ExhibitionSetupMenuOutcome
* DESCRIPTION: Gets which entry picked
* INPUTS:      none
* OUTPUTS:     menu selection
*/
int ExhibitionSetupMenuOutcome(struct game_data* game_data) {

	bool game_chosen = false;
	int current_menu_selection = 0;
	do {
		ShowExhibitionGameSetupOptions(current_menu_selection, false, game_data);
		int ch = getch();

		if (ch == 0 || ch == 224) {   // extended key: read the scan code
			ch = getch();
			switch (ch) {
			case 72: // up arrow
				if (current_menu_selection > 0) {
					current_menu_selection--;
				}
				else
					current_menu_selection = (EXHIBITION_GAME_SETTINGS_OPTIONS_NUMBER - 1);
				break;
			case 80: // down arrow
				if (current_menu_selection < (EXHIBITION_GAME_SETTINGS_OPTIONS_NUMBER - 1)) {
					current_menu_selection++;
				}
				else
					current_menu_selection = 0;
				break;
			case 75: // left arrow
				if (current_menu_selection == 0)
				{
					if (game_data->two_player)
					{
						game_data->one_player = true;
						game_data->two_player = false;
					}
				}
				else if (current_menu_selection == 1)
				{
					if (game_data->team_1_id == 0)
						game_data->team_1_id = (LEAGUE_TEAM_COUNT - 1);
					else
						game_data->team_1_id--;
				}
				else if (current_menu_selection == 2)
				{
					if (game_data->team_2_id == 0)
						game_data->team_2_id = (LEAGUE_TEAM_COUNT - 1);
					else
						game_data->team_2_id--;
				}
				else if (current_menu_selection == 3 && game_data->fouls == false)
					game_data->fouls = true;
				else if (current_menu_selection == 4 && game_data->plays_per_quarter > MIN_PLAYS_PER_QUARTER)
				{
					game_data->plays_per_quarter -= STEPS_PLAYS_PER_QUARTER;
					game_data->plays_remaining_in_quarter = game_data->plays_per_quarter;
				}
				break;
			case 77: // right arrow
				if (current_menu_selection == 0)
				{
					if (game_data->one_player)
					{
						game_data->one_player = false;
						game_data->two_player = true;
					}
				}
				else if (current_menu_selection == 1)
				{
					if (game_data->team_1_id == (LEAGUE_TEAM_COUNT - 1))
						game_data->team_1_id = 0;
					else
						game_data->team_1_id++;
				}
				else if (current_menu_selection == 2)
				{
					if (game_data->team_2_id == (LEAGUE_TEAM_COUNT - 1))
						game_data->team_2_id = 0;
					else
						game_data->team_2_id++;
				}
				else if (current_menu_selection == 3 && game_data->fouls == true)
					game_data->fouls = false;
				else if (current_menu_selection == 4 && game_data->plays_per_quarter < MAX_PLAYS_PER_QUARTER)
				{
					game_data->plays_per_quarter += STEPS_PLAYS_PER_QUARTER;
					game_data->plays_remaining_in_quarter = game_data->plays_per_quarter;
				}
				break;
			default:
				break;
			}
		}
		else if (ch == 13) {// Enter key
			if (current_menu_selection == (EXHIBITION_GAME_SETTINGS_OPTIONS_NUMBER - 1) || current_menu_selection == (EXHIBITION_GAME_SETTINGS_OPTIONS_NUMBER - 2)) {
				game_chosen = true;
				return current_menu_selection;
			}
		}
		else
			;
		// any other key: ignored, loop waits for another press
	} while (!game_chosen);
	return -1;
}

/*
* NAME:        ExecuteExhibitionSetupMenuChoice
* DESCRIPTION: play out each menu option
* INPUTS:      selection
* OUTPUTS:     none
*/
void ExecuteExhibitionSetupMenuChoice(int selection, struct game_data* game_data)
{
	//if (selection == 0) // customise team
	//{
	//	SetExhibitionTeamNames(game_data);	
	//	SetExhibitionTeamColours(game_data);
	//}
	//else if (selection == 1) // customise players
	//{
	//	SetExhibitionPlayerNames(game_data);
	//}

	system("cls");
}

/*
* NAME:        SetupExhibitionMenu
* DESCRIPTION: initialises data such as team names, coin toss, starting possession etc
* INPUTS:      game_data
* OUTPUTS:     start / back
*/
int SetupExhibitionMenu(struct game_data* game_data)
{
	system("cls");
	ShowMenuTitle();
	ShowExhibitionGameSetupOptions(0, true, game_data);
	do {
		int selection = ExhibitionSetupMenuOutcome(game_data);
		if (selection == (EXHIBITION_GAME_SETTINGS_OPTIONS_NUMBER - 1) || selection == (EXHIBITION_GAME_SETTINGS_OPTIONS_NUMBER - 2))
			return selection;
		ExecuteExhibitionSetupMenuChoice(selection, game_data);
	} while (1);
	return -1;
}

/*
* NAME:        SetupTeamData
* DESCRIPTION: builds team names colours player names etc from decision
* INPUTS:      game_data
* OUTPUTS:     void
*/
void SetupTeamData(struct game_data* game_data)
{
	if (game_data == NULL)
		return;

	if (game_data->team_1_id < 0 ||
		game_data->team_1_id >= LEAGUE_TEAM_COUNT ||
		game_data->team_2_id < 0 ||
		game_data->team_2_id >= LEAGUE_TEAM_COUNT)
		return;

	const struct team_data* team_1 =
		&default_league.teams[game_data->team_1_id];
	const struct team_data* team_2 =
		&default_league.teams[game_data->team_2_id];

	snprintf(game_data->team_1_name, sizeof game_data->team_1_name,
		"%s", team_1->name);
	snprintf(game_data->team_2_name, sizeof game_data->team_2_name,
		"%s", team_2->name);

	snprintf(game_data->team_1_qb_names[0],
		sizeof game_data->team_1_qb_names[0], "%s", team_1->qb_name);
	snprintf(game_data->team_2_qb_names[0],
		sizeof game_data->team_2_qb_names[0], "%s", team_2->qb_name);

	for (int i = 0; i < NUMBER_OF_RB_NAMES_PER_TEAM; i++) {
		snprintf(game_data->team_1_rb_names[i],
			sizeof game_data->team_1_rb_names[i], "%s", team_1->rb_names[i]);
		snprintf(game_data->team_2_rb_names[i],
			sizeof game_data->team_2_rb_names[i], "%s", team_2->rb_names[i]);
	}

	for (int i = 0; i < NUMBER_OF_WR_NAMES_PER_TEAM; i++) {
		snprintf(game_data->team_1_wr_names[i],
			sizeof game_data->team_1_wr_names[i], "%s", team_1->wr_names[i]);
		snprintf(game_data->team_2_wr_names[i],
			sizeof game_data->team_2_wr_names[i], "%s", team_2->wr_names[i]);
	}

	game_data->team_1_colour = team_1->colour;
	game_data->team_2_colour = team_2->colour;
	game_data->players_named = true;
}

/*
* NAME:        SetupExhibitionGame
* DESCRIPTION: initialises data such as team names, coin toss, starting possession etc
* INPUTS:      game_data
* OUTPUTS:     start / back
*/
int SetupExhibitionGame(struct game_data* game_data, struct total_data* total_data)
{
	ExhibitionGameDataConstructor(game_data, total_data);
	
	if (SetupExhibitionMenu(game_data) == (EXHIBITION_GAME_SETTINGS_OPTIONS_NUMBER - 1))
		return -1;

	SetupTeamData(game_data);
	
	CoinToss(game_data);
	return 1;
}