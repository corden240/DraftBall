#include <stdio.h>
#include <stdlib.h>   /* srand, rand, atoi */
#include <time.h>     /* time() */
#include "main.h"
#include <string.h>
#include <windows.h>
#include "teamdata.h"

void ShowMenuTitle();
void ResetScreen();

/*
* NAME:        GetSeasonTeam1LastID
* DESCRIPTION: get correct team id
* INPUTS:      season_data
* OUTPUTS:     none
*/
int GetSeasonTeam1LastID(struct season_data* season_data)
{
	if (season_data->team_1_id == 0)
		return (LEAGUE_TEAM_COUNT - 1);
	else
		return (season_data->team_1_id - 1);
}

/*
* NAME:        GetSeasonTeam1CurrentID
* DESCRIPTION: get correct team id
* INPUTS:      season_data
* OUTPUTS:     none
*/
int GetSeasonTeam1CurrentID(struct season_data* season_data)
{
	return season_data->team_1_id;
}

/*
* NAME:        GetSeasonTeam1NextID
* DESCRIPTION: get correct team id
* INPUTS:      season_data
* OUTPUTS:     none
*/
int GetSeasonTeam1NextID(struct season_data* season_data)
{
	if (season_data->team_1_id == (LEAGUE_TEAM_COUNT - 1))
		return 0;
	else
		return (season_data->team_1_id + 1);
}

/*
* NAME:        PrintSeasonTeamNameField
* DESCRIPTION: print 1 name field
* INPUTS:      hConsole
* OUTPUTS:     none
*/
void PrintSeasonTeamNameField(
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
* NAME:        PrintSeasonThreeTeamNames
* DESCRIPTION: print last current and next team names
* INPUTS:      ids and hConsole
* OUTPUTS:     none
*/
void PrintSeasonThreeTeamNames(
	HANDLE hConsole,
	int last_id,
	int current_id,
	int next_id)
{
	const struct team_data* last = &default_league.teams[last_id];
	const struct team_data* current = &default_league.teams[current_id];
	const struct team_data* next = &default_league.teams[next_id];

	PrintSeasonTeamNameField(hConsole, last, (WORD)last->colour);
	putchar(' ');

	PrintSeasonTeamNameField(
		hConsole,
		current,
		(WORD)(CONSOLE_TEXT_WHITE | (current->colour << 4)));
	putchar(' ');

	PrintSeasonTeamNameField(hConsole, next, (WORD)next->colour);
}

/*
* NAME:        SeasonDataConstructor
* DESCRIPTION: starts all of season_data data to default settings
* INPUTS:      game_data
* OUTPUTS:     none
*/
void SeasonDataConstructor(struct season_data* season_data)
{
	season_data->fouls = true;
	season_data->current_game_mode = 1;
	season_data->plays_per_quarter = DEFAULT_PLAYS_PER_QUARTER;
	season_data->team_1_id = 0;

	for (int i = 0; i < LEAGUE_TEAM_COUNT; i++)
	{
		for (int j = 0; j < WIN_LOSS_DRAW; j++)
		{
			season_data->team_records[i][j] = 0;
		}
	}

	int afc_teams = 0, nfc_teams = 0, afc_east_teams = 0, afc_north_teams = 0, afc_south_teams = 0, afc_west_teams = 0, nfc_east_teams = 0, nfc_north_teams = 0, nfc_south_teams = 0, nfc_west_teams = 0;
	for (int i = 0; i < LEAGUE_TEAM_COUNT; i++)
	{
		const struct team_data* team = &default_league.teams[i];
		if (team->division < 5) {
			season_data->afc_team_ids[afc_teams] = team->id;
			afc_teams++;
		}
		else {
			season_data->nfc_team_ids[nfc_teams] = team->id;
			nfc_teams++;
		}

		if (team->division == 1) {
			season_data->afc_east_team_ids[afc_east_teams] = team->id;
			afc_east_teams++;
		}
		else if (team->division == 2) {
			season_data->afc_north_team_ids[afc_north_teams] = team->id;
			afc_north_teams++;
		}
		else if (team->division == 3) {
			season_data->afc_south_team_ids[afc_south_teams] = team->id;
			afc_south_teams++;
		}
		else if (team->division == 4) {
			season_data->afc_west_team_ids[afc_west_teams] = team->id;
			afc_west_teams++;
		}
		else if (team->division == 5) {
			season_data->nfc_east_team_ids[nfc_east_teams] = team->id;
			nfc_east_teams++;
		}
		else if (team->division == 6) {
			season_data->nfc_north_team_ids[nfc_north_teams] = team->id;
			nfc_north_teams++;
		}
		else if (team->division == 7) {
			season_data->nfc_south_team_ids[nfc_south_teams] = team->id;
			nfc_south_teams++;
		}
		else if (team->division == 8) {
			season_data->nfc_west_team_ids[nfc_west_teams] = team->id;
			nfc_west_teams++;
		}
	}
	season_data->week = 1;
}

/*
* NAME:        ShowSeasonGameSetupOptions
* DESCRIPTION: game menu options
* INPUTS:      current selection, delay or not
* OUTPUTS:     none
*/
static void ShowSeasonGameSetupOptions(int current_menu_selection, bool first_text, struct season_data* season_data)
{
	ResetScreen();
	ShowMenuTitle();
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	int team_1_last_id = GetSeasonTeam1LastID(season_data);
	int team_1_current_id = GetSeasonTeam1CurrentID(season_data);
	int team_1_next_id = GetSeasonTeam1NextID(season_data);

	printf("\r\n\r\n                                  ");
	printf("            ");
	if (first_text)
		PrintTextWithDelays("Game Settings");
	else
		printf("Game Settings");

	for (int i = 0; i < SEASON_GAME_SETTINGS_OPTIONS_NUMBER; i++)
	{
		printf("\r\n\r\n                        ");
		if (i == 0)
		{
			printf("          ");
			if (current_menu_selection == i)
				SetConsoleTextAttribute(hConsole, CONSOLE_BG_WHITE | CONSOLE_TEXT_GREEN);
			if (first_text)
				PrintTextWithDelays("Team");
			else
				printf("Team");
			SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
			printf("     ");
			PrintSeasonThreeTeamNames(hConsole, team_1_last_id, team_1_current_id, team_1_next_id);
			printf("              ");
		}
		else if (i == 1)
		{
			printf("        ");
			if (current_menu_selection == i)
				SetConsoleTextAttribute(hConsole, CONSOLE_BG_WHITE | CONSOLE_TEXT_GREEN);
			if (first_text)
				PrintTextWithDelays("Plays per quarter");
			else
				printf("Plays per quarter");

			SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
			printf("     ");

			if (season_data->plays_per_quarter == MIN_PLAYS_PER_QUARTER)
				SetConsoleTextAttribute(hConsole, CONSOLE_BG_WHITE | CONSOLE_TEXT_GREEN);
			printf("5");
			SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);

			printf("  ");
			if (season_data->plays_per_quarter == MIN_PLAYS_PER_QUARTER + STEPS_PLAYS_PER_QUARTER)
				SetConsoleTextAttribute(hConsole, CONSOLE_BG_WHITE | CONSOLE_TEXT_GREEN);
			printf("10");
			SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);

			printf("  ");
			if (season_data->plays_per_quarter == MIN_PLAYS_PER_QUARTER + (STEPS_PLAYS_PER_QUARTER * 2))
				SetConsoleTextAttribute(hConsole, CONSOLE_BG_WHITE | CONSOLE_TEXT_GREEN);
			printf("15");
			SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);

			printf("  ");
			if (season_data->plays_per_quarter == MIN_PLAYS_PER_QUARTER + (STEPS_PLAYS_PER_QUARTER * 3))
				SetConsoleTextAttribute(hConsole, CONSOLE_BG_WHITE | CONSOLE_TEXT_GREEN);
			printf("20");
			SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);

			printf("  ");
			if (season_data->plays_per_quarter == MIN_PLAYS_PER_QUARTER + ((5) * 4))
				SetConsoleTextAttribute(hConsole, CONSOLE_BG_WHITE | CONSOLE_TEXT_GREEN);
			printf("25");
			SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);

			printf("  ");
			if (season_data->plays_per_quarter == MIN_PLAYS_PER_QUARTER + (STEPS_PLAYS_PER_QUARTER * 5))
				SetConsoleTextAttribute(hConsole, CONSOLE_BG_WHITE | CONSOLE_TEXT_GREEN);
			printf("30");
			SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);

		}
		else if (i == 2)
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
			if (season_data->fouls)
				SetConsoleTextAttribute(hConsole, CONSOLE_BG_WHITE | CONSOLE_TEXT_GREEN);
			printf("On");
			SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
			printf(" / ");
			if (!season_data->fouls)
				SetConsoleTextAttribute(hConsole, CONSOLE_BG_WHITE | CONSOLE_TEXT_GREEN);
			printf("Off");
		}
		else if (i == 3)
		{
			printf("                      ");
			if (current_menu_selection == i)
				SetConsoleTextAttribute(hConsole, CONSOLE_BG_WHITE | CONSOLE_TEXT_GREEN);
			if (first_text)
				PrintTextWithDelays("Start season");
			else
				printf("Start season");
		}
		else if (i == 4)
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
* NAME:        SeasonSetupMenuOutcome
* DESCRIPTION: Gets which entry picked
* INPUTS:      none
* OUTPUTS:     menu selection
*/
int SeasonSetupMenuOutcome(struct season_data* season_data) {

	bool game_chosen = false;
	int current_menu_selection = 0;
	do {
		ShowSeasonGameSetupOptions(current_menu_selection, false, season_data);
		int ch = getch();

		if (ch == 0 || ch == 224) {   // extended key: read the scan code
			ch = getch();
			switch (ch) {
			case 72: // up arrow
				if (current_menu_selection > 0) {
					current_menu_selection--;
				}
				else
					current_menu_selection = (SEASON_GAME_SETTINGS_OPTIONS_NUMBER - 1);
				break;
			case 80: // down arrow
				if (current_menu_selection < (SEASON_GAME_SETTINGS_OPTIONS_NUMBER - 1)) {
					current_menu_selection++;
				}
				else
					current_menu_selection = 0;
				break;
			case 75: // left arrow
				if (current_menu_selection == 0)
				{
					if (season_data->team_1_id == 0)
						season_data->team_1_id = (LEAGUE_TEAM_COUNT - 1);
					else
						season_data->team_1_id--;
				}
				else if (current_menu_selection == 1 && season_data->plays_per_quarter > MIN_PLAYS_PER_QUARTER)
				{
					season_data->plays_per_quarter -= STEPS_PLAYS_PER_QUARTER;
				}
				else if (current_menu_selection == 2 && season_data->fouls == false)
					season_data->fouls = true;
				break;
			case 77: // right arrow
				if (current_menu_selection == 0)
				{
					if (season_data->team_1_id == (LEAGUE_TEAM_COUNT - 1))
						season_data->team_1_id = 0;
					else
						season_data->team_1_id++;
				}
				else if (current_menu_selection == 1 && season_data->plays_per_quarter < MAX_PLAYS_PER_QUARTER)
				{
					season_data->plays_per_quarter += STEPS_PLAYS_PER_QUARTER;
				}
				else if (current_menu_selection == 2 && season_data->fouls == true)
					season_data->fouls = false;
				break;
			default:
				break;
			}
		}
		else if (ch == 13) {// Enter key
			if (current_menu_selection == (SEASON_GAME_SETTINGS_OPTIONS_NUMBER - 1) || current_menu_selection == (SEASON_GAME_SETTINGS_OPTIONS_NUMBER - 2)) {
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
* NAME:        SetupSeasonMenu
* DESCRIPTION: initialises data such as team names, coin toss, starting possession etc
* INPUTS:      game_data
* OUTPUTS:     start / back
*/
int SetupSeasonMenu(struct season_data* season_data)
{
	system("cls");
	ShowMenuTitle();
	ShowSeasonGameSetupOptions(0, true, season_data);
	do {
		int selection = SeasonSetupMenuOutcome(season_data);
		if (selection == (SEASON_GAME_SETTINGS_OPTIONS_NUMBER - 1) || selection == (SEASON_GAME_SETTINGS_OPTIONS_NUMBER - 2))
			return selection;
		system("cls");
	} while (1);
	return -1;
}

/*
* NAME:        GetDivisionID
* DESCRIPTION: returns division id from team id
* INPUTS:      season_data, team id
* OUTPUTS:     none
*/
int GetDivisionID(struct season_data* season_data, int team_id)
{
	int division_id = 0;
	for (int i = 0; i < DIVISION_TEAM_COUNT; i++) {
		if (season_data->afc_east_team_ids[i] == team_id) {
			division_id = 0;
		}
		else if (season_data->afc_north_team_ids[i] == team_id) {
			division_id = 1;
		}
		else if (season_data->afc_south_team_ids[i] == team_id) {
			division_id = 2;
		}
		else if (season_data->afc_west_team_ids[i] == team_id) {
			division_id = 3;
		}
		else if (season_data->nfc_east_team_ids[i] == team_id) {
			division_id = 4;
		}
		else if (season_data->nfc_north_team_ids[i] == team_id) {
			division_id = 5;
		}
		else if (season_data->nfc_south_team_ids[i] == team_id) {
			division_id = 6;
		}
		else if (season_data->nfc_west_team_ids[i] == team_id) {
			division_id = 7;
		}
	}
	return division_id;
}

/*
* NAME:        SetupSeasonData
* DESCRIPTION: sets up remaining season data
* INPUTS:      game_data
* OUTPUTS:     none
*/
void SetupSeasonData(struct season_data* season_data)
{
	season_data->division_id = GetDivisionID(season_data, season_data->team_1_id);
	return;
}

/*
* NAME:        SetupSeason
* DESCRIPTION: initialises season data
* INPUTS:      game_data
* OUTPUTS:     none
*/
int SetupSeason(struct season_data* season_data)
{
	SeasonDataConstructor(season_data);
	
	if (SetupSeasonMenu(season_data) == (EXHIBITION_GAME_SETTINGS_OPTIONS_NUMBER - 1))
		return -1; 

	SetupSeasonData(season_data);

	return  1;
}