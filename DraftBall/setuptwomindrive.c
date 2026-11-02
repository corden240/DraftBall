#include <stdio.h>
#include <stdlib.h>   /* srand, rand, atoi */
#include <time.h>     /* time() */
#include "main.h"
#include <string.h>
#include <windows.h>
#include "teamdata.h"
#include "main_d.prt"

void ResetLineOfFirstDown(struct game_data* game_data);
void SetupTeamData(struct game_data* game_data);
void ShowMenuTitle();
void PrintTextWithDelays(char txt[1000]);
void ResetScreen();
int GetTeam1LastID(struct game_data* game_data);
int GetTeam1CurrentID(struct game_data* game_data);
int GetTeam1NextID(struct game_data* game_data);
int GetTeam2LastID(struct game_data* game_data);
int GetTeam2CurrentID(struct game_data* game_data);
int GetTeam2NextID(struct game_data* game_data);
void PrintTeamNameField(HANDLE hConsole, const struct team_data* team, WORD name_attributes);
void PrintThreeTeamNames(HANDLE hConsole, int last_id, int current_id, int next_id);

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
	game_data->one_player = true;
	game_data->two_player = false;

	game_data->team_1_id = 10;
	game_data->team_2_id = 21;

	game_data->two_min_drive_deficit_index = 0;
	game_data->two_min_drive_yards_index = 1;
}

/*
* NAME:        SetTeamDeficit
* DESCRIPTION: setup team 1 deficit
* INPUTS:      game_data
* OUTPUTS:     none
*/
void SetTeamDeficit(struct game_data* game_data)
{
	int score_difference = 0;
	if (game_data->two_min_drive_deficit_index == 0)
		score_difference = TWO_MIN_DRIVE_DEFICIT_0;
	else if (game_data->two_min_drive_deficit_index == 1)
		score_difference = TWO_MIN_DRIVE_DEFICIT_1;
	else if (game_data->two_min_drive_deficit_index == 2)
		score_difference = TWO_MIN_DRIVE_DEFICIT_2;

	game_data->team_2_score = game_data->team_1_score + score_difference;
}

/*
* NAME:        SetTeamStartLine
* DESCRIPTION: setup team 1 start line
* INPUTS:      game_data
* OUTPUTS:     none
*/
void SetTeamStartLine(struct game_data* game_data)
{
	game_data->line_of_scrimmage = (FIELD_WIDTH - (two_min_drive_yard_options[game_data->two_min_drive_yards_index])) + END_ZONE_WIDTH;
	ResetLineOfFirstDown(game_data);
}

/*
* NAME:        ShowTwoMinDriveGameSetupOptions
* DESCRIPTION: game menu options
* INPUTS:      current selection, delay or not
* OUTPUTS:     none
*/
static void ShowTwoMinDriveGameSetupOptions(int current_menu_selection, bool first_text, struct game_data* game_data)
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

	for (int i = 0; i < TWO_MIN_DRIVE_SETTINGS_OPTIONS_NUMBER; i++)
	{
		printf("\r\n\r\n                              ");
		if (i == 0)
		{
			printf("                  ");
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
			printf("          ");
			if (current_menu_selection == i)
				SetConsoleTextAttribute(hConsole, CONSOLE_BG_WHITE | CONSOLE_TEXT_GREEN);
			if (first_text)
				PrintTextWithDelays("Team 1");
			else
				printf("Team 1");
			SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
			printf("   ");
			PrintThreeTeamNames(hConsole, team_1_last_id, team_1_current_id, team_1_next_id);
			printf("              ");
		}
		else if (i == 2)
		{
			printf("          ");
			if (current_menu_selection == i)
				SetConsoleTextAttribute(hConsole, CONSOLE_BG_WHITE | CONSOLE_TEXT_GREEN);
			if (first_text)
				PrintTextWithDelays("Team 2");
			else
				printf("Team 2");
			SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
			printf("   ");

			PrintThreeTeamNames(hConsole, team_2_last_id, team_2_current_id, team_2_next_id);
			printf("              ");
		}
		else if (i == 3) // points deficit
		{
			printf("                ");
			if (current_menu_selection == i)
				SetConsoleTextAttribute(hConsole, CONSOLE_BG_WHITE | CONSOLE_TEXT_GREEN);
			if (first_text)
				PrintTextWithDelays("Points deficit");
			else
				printf("Points deficit");

			SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
			printf("     ");

			if (game_data->two_min_drive_deficit_index == 0)
				SetConsoleTextAttribute(hConsole, CONSOLE_BG_WHITE | CONSOLE_TEXT_GREEN);
			printf("%d", TWO_MIN_DRIVE_DEFICIT_0);
			SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);

			printf("  ");
			if (game_data->two_min_drive_deficit_index == 1)
				SetConsoleTextAttribute(hConsole, CONSOLE_BG_WHITE | CONSOLE_TEXT_GREEN);
			printf("%d", TWO_MIN_DRIVE_DEFICIT_1);
			SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);

			printf("  ");
			if (game_data->two_min_drive_deficit_index == 2)
				SetConsoleTextAttribute(hConsole, CONSOLE_BG_WHITE | CONSOLE_TEXT_GREEN);
			printf("%d", TWO_MIN_DRIVE_DEFICIT_2);
			SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
		}
		else if (i == 4) // yards from end zone
		{
			printf("             ");
			if (current_menu_selection == i)
				SetConsoleTextAttribute(hConsole, CONSOLE_BG_WHITE | CONSOLE_TEXT_GREEN);
			if (first_text)
				PrintTextWithDelays("Yards to end zone");
			else
				printf("Yards to end zone");

			SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
			printf("     ");
			
			if (game_data->two_min_drive_yards_index > 0)
				printf("%2d", two_min_drive_yard_options[game_data->two_min_drive_yards_index - 1]);
			else
				printf("  ");
			
			SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
			printf("  ");

			SetConsoleTextAttribute(hConsole, CONSOLE_BG_WHITE | CONSOLE_TEXT_GREEN);
			printf("%2d", two_min_drive_yard_options[game_data->two_min_drive_yards_index]);

			SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
			printf("  ");

			if (game_data->two_min_drive_yards_index < (TWO_MIN_DRIVE_YARDS_OPTIONS - 1))
				printf("%2d", two_min_drive_yard_options[game_data->two_min_drive_yards_index + 1]);
			else
				printf("  ");

			SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
			printf("  ");
		}
		else if (i == 5)
		{
			printf("                    ");
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
		else if (i == 6)
		{
			printf("                       ");
			if (current_menu_selection == i)
				SetConsoleTextAttribute(hConsole, CONSOLE_BG_WHITE | CONSOLE_TEXT_GREEN);
			if (first_text)
				PrintTextWithDelays("Start game");
			else
				printf("Start game");
		}
		else if (i == 7)
		{
			printf("                          ");
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
* NAME:        TwoMinDriveSetupMenuOutcome
* DESCRIPTION: Gets which entry picked
* INPUTS:      none
* OUTPUTS:     menu selection
*/
int TwoMinDriveSetupMenuOutcome(struct game_data* game_data) {

	bool game_chosen = false;
	int current_menu_selection = 0;
	do {
		ShowTwoMinDriveGameSetupOptions(current_menu_selection, false, game_data);
		int ch = getch();

		if (ch == 0 || ch == 224) {   // extended key: read the scan code
			ch = getch();
			switch (ch) {
			case 72: // up arrow
				if (current_menu_selection > 0) {
					current_menu_selection--;
				}
				else
					current_menu_selection = (TWO_MIN_DRIVE_SETTINGS_OPTIONS_NUMBER - 1);
				break;
			case 80: // down arrow
				if (current_menu_selection < (TWO_MIN_DRIVE_SETTINGS_OPTIONS_NUMBER - 1)) {
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
				else if (current_menu_selection == 3)
				{
					if (game_data->two_min_drive_deficit_index > 0)
						game_data->two_min_drive_deficit_index--;
				}
				else if (current_menu_selection == 4)
				{
					if (game_data->two_min_drive_yards_index > 0)
						game_data->two_min_drive_yards_index--;
				}
				else if (current_menu_selection == 5 && game_data->fouls == false)
					game_data->fouls = true;
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
				else if (current_menu_selection == 3)
				{
					if (game_data->two_min_drive_deficit_index < (TWO_MIN_DRIVE_DEFICIT_OPTIONS - 1))
						game_data->two_min_drive_deficit_index++;
				}
				else if (current_menu_selection == 4)
				{
					if (game_data->two_min_drive_yards_index < (TWO_MIN_DRIVE_YARDS_OPTIONS - 1))
						game_data->two_min_drive_yards_index++;
				}
				else if (current_menu_selection == 5 && game_data->fouls == true)
					game_data->fouls = false;
				break;
			default:
				break;
			}
		}
		else if (ch == 13) {// Enter key
			if (current_menu_selection == (TWO_MIN_DRIVE_SETTINGS_OPTIONS_NUMBER - 1) || current_menu_selection == (TWO_MIN_DRIVE_SETTINGS_OPTIONS_NUMBER - 2)) {
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
* NAME:        SetupTwoMinDriveMenu
* DESCRIPTION: initialises data such as team names, coin toss, starting possession etc
* INPUTS:      game_data
* OUTPUTS:     start / back
*/
int SetupTwoMinDriveMenu(struct game_data* game_data)
{
	system("cls");
	ShowMenuTitle();
	ShowTwoMinDriveGameSetupOptions(0, true, game_data);
	do {
		int selection = TwoMinDriveSetupMenuOutcome(game_data);
		if (selection == (TWO_MIN_DRIVE_SETTINGS_OPTIONS_NUMBER - 1) || selection == (TWO_MIN_DRIVE_SETTINGS_OPTIONS_NUMBER - 2))
			return selection;
		system("cls");
	} while (1);
	return -1;
}

/*
* NAME:        SetupTwoMinDrive
* DESCRIPTION: initialises datafor 2 min drive
* INPUTS:      game_data
* OUTPUTS:     start / back
*/
int SetupTwoMinDrive(struct game_data* game_data, struct total_data* total_data)
{
	TwoMinDriveGameDataConstructor(game_data, total_data);

	if (SetupTwoMinDriveMenu(game_data) == (TWO_MIN_DRIVE_SETTINGS_OPTIONS_NUMBER - 1))
		return -1;

	SetupTeamData(game_data);

	SetTeamDeficit(game_data);

	SetTeamStartLine(game_data);
	system("cls");
	return 1;
}