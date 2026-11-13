#include <stdio.h>
#include <stdlib.h>   /* srand, rand, atoi */
#include <time.h>     /* time() */
#include "main.h"
#include <string.h>
#include <windows.h>
#include "teamdata.h"

void ShowMenuTitle();
void PrintTextWithDelays(char txt[1000]);
void ResetScreen();
int GetDivisionID(struct season_data* season_data, int team_id);


/*
* NAME:        GetSeasonDivision
* DESCRIPTION: gets division id of current season
* INPUTS:      season_data
* OUTPUTS:     division id
*/
int GetSeasonDivision(struct season_data* season_data)
{
	const struct team_data* team1 = &default_league.teams[season_data->team_1_id];
	return team1->division;
}

/*
* NAME:        GetHomeOrAway
* DESCRIPTION: returns 0 for home 1 for away
* INPUTS:      season_data, team_id, week chosen
* OUTPUTS:     int
*/
int GetHomeOrAway(struct season_data* season_data, int team_id, int week)
{
	int team_1_div_id = GetDivisionID(season_data, season_data->team_1_id);

	int team_1_id_within_division = -1;

	if (team_1_div_id == 0) {
		for (int i = 0; i < 4; i++) {
			if (season_data->afc_east_team_ids[i] == team_id)
				team_1_id_within_division = i;
		}
	}
	else if (team_1_div_id == 1) {
		for (int i = 0; i < 4; i++) {
			if (season_data->afc_north_team_ids[i] == team_id)
				team_1_id_within_division = i;
		}
	}
	else if (team_1_div_id == 2) {
		for (int i = 0; i < 4; i++) {
			if (season_data->afc_south_team_ids[i] == team_id)
				team_1_id_within_division = i;
		}
	}
	else if (team_1_div_id == 3) {
		for (int i = 0; i < 4; i++) {
			if (season_data->afc_west_team_ids[i] == team_id)
				team_1_id_within_division = i;
		}
	}
	else if (team_1_div_id == 4) {
		for (int i = 0; i < 4; i++) {
			if (season_data->nfc_east_team_ids[i] == team_id)
				team_1_id_within_division = i;
		}
	}
	else if (team_1_div_id == 5) {
		for (int i = 0; i < 4; i++) {
			if (season_data->nfc_north_team_ids[i] == team_id)
				team_1_id_within_division = i;
		}
	}
	else if (team_1_div_id == 6) {
		for (int i = 0; i < 4; i++) {
			if (season_data->nfc_south_team_ids[i] == team_id)
				team_1_id_within_division = i;
		}
	}
	else if (team_1_div_id == 7) {
		for (int i = 0; i < 4; i++) {
			if (season_data->nfc_west_team_ids[i] == team_id)
				team_1_id_within_division = i;
		}
	}

	if (team_1_id_within_division < 0)
		printf("Error");

	int team_week_id = 0;
	for (int i = 0; i < 4; i++) {
		if (schedule[week - 1][i] == (team_1_id_within_division + 1))
			team_week_id = i;
	}

	if (team_week_id == 0 || team_week_id == 2)
		return 0;
	else
		return 1;
}

/*
* NAME:        GetOpponentTeamId
* DESCRIPTION: returns id of team that given team is playing in given week.
* INPUTS:      season_data, team_id, week chosen
* OUTPUTS:     int
*/
int GetOpponentTeamId(struct season_data* season_data, int team_id, int week)
{
	int team_1_div_id = GetDivisionID(season_data, season_data->team_1_id);

	int team_1_id_within_division = -1;

	if (team_1_div_id == 0) {
		for (int i = 0; i < 4; i++) {
			if (season_data->afc_east_team_ids[i] == team_id)
				team_1_id_within_division = i;
		}
	}
	else if (team_1_div_id == 1) {
		for (int i = 0; i < 4; i++) {
			if (season_data->afc_north_team_ids[i] == team_id)
				team_1_id_within_division = i;
		}
	}
	else if (team_1_div_id == 2) {
		for (int i = 0; i < 4; i++) {
			if (season_data->afc_south_team_ids[i] == team_id)
				team_1_id_within_division = i;
		}
	}
	else if (team_1_div_id == 3) {
		for (int i = 0; i < 4; i++) {
			if (season_data->afc_west_team_ids[i] == team_id)
				team_1_id_within_division = i;
		}
	}
	else if (team_1_div_id == 4) {
		for (int i = 0; i < 4; i++) {
			if (season_data->nfc_east_team_ids[i] == team_id)
				team_1_id_within_division = i;
		}
	}
	else if (team_1_div_id == 5) {
		for (int i = 0; i < 4; i++) {
			if (season_data->nfc_north_team_ids[i] == team_id)
				team_1_id_within_division = i;
		}
	}
	else if (team_1_div_id == 6) {
		for (int i = 0; i < 4; i++) {
			if (season_data->nfc_south_team_ids[i] == team_id)
				team_1_id_within_division = i;
		}
	}
	else if (team_1_div_id == 7) {
		for (int i = 0; i < 4; i++) {
			if (season_data->nfc_west_team_ids[i] == team_id)
				team_1_id_within_division = i;
		}
	}

	if (team_1_id_within_division < 0)
		printf("Error");
		
	int team_week_id = 0;
	for (int i = 0; i < 4; i++) {
		if (schedule[week - 1][i] == (team_1_id_within_division + 1))
			team_week_id = i;
	}

	int opponent_week_id = 0;
	if (team_week_id == 0)
		opponent_week_id = 1;
	else if (team_week_id == 1)
		opponent_week_id = 0;
	else if (team_week_id == 2)
		opponent_week_id = 3;
	else if (team_week_id == 3)
		opponent_week_id = 2;

	int team_2_id_within_division = schedule[week - 1][opponent_week_id] - 1;

	if (team_1_div_id == 0) {
		return season_data->afc_east_team_ids[team_2_id_within_division];
	}
	else if (team_1_div_id == 1) {
		return season_data->afc_north_team_ids[team_2_id_within_division];
	}
	else if (team_1_div_id == 2) {
		return season_data->afc_south_team_ids[team_2_id_within_division];
	}
	else if (team_1_div_id == 3) {
		return season_data->afc_west_team_ids[team_2_id_within_division];
	}
	else if (team_1_div_id == 4) {
		return season_data->nfc_east_team_ids[team_2_id_within_division];
	}
	else if (team_1_div_id == 5) {
		return season_data->nfc_north_team_ids[team_2_id_within_division];
	}
	else if (team_1_div_id == 6) {
		return season_data->nfc_south_team_ids[team_2_id_within_division];
	}
	else if (team_1_div_id == 7) {
		return season_data->nfc_west_team_ids[team_2_id_within_division];
	}
	printf("Error");

	return -1;
}

/*
* NAME:        PrintSizedTeamName
* DESCRIPTION: uniform size printed team name
* INPUTS:      team id
* OUTPUTS:     none
*/
void PrintSizedTeamName(int team_id, bool invert)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
	const struct team_data* team1 = &default_league.teams[team_id];
	int textLength = 0;
	while (textLength < MAX_STRING_LENGTH && team1->name[textLength] != '\0')
		textLength++;

	int padding = TEAM_NAME_FIELD_WIDTH - textLength - 2;

	printf("%*s", padding, "");

	if (invert)
		SetConsoleTextAttribute(hConsole, team1->colour << 4);
	else
		SetConsoleTextAttribute(hConsole, team1->colour);

	printf("%s", team1->name);
	SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
}


/*
* NAME:        ShowRegularSeasonMenuTop
* DESCRIPTION:
* INPUTS:      current selection, delay or not, season_data
* OUTPUTS:     none
*/
void ShowRegularSeasonMenuTop(bool first_text, struct season_data* season_data)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);

	printf("           ");
	if (first_text)
		PrintTextWithDelays("AFC East    W   L   D  Record                 Next Game");
	else
		printf("AFC East    W   L   D  Record                 Next Game");

	printf("\r\n");

	const struct team_data* team1 = &default_league.teams[season_data->team_1_id];
	bool show_game = true;
	for (int i = 0; i < LEAGUE_TEAM_COUNT; i++)
	{
		const struct team_data* team = &default_league.teams[i];
		if (team->division == team1->division)
		{
			printf("         ");
			PrintSizedTeamName(team->id, (team->id == team1->id) ? true : false);
			printf("   ");

			int wins = season_data->team_records[team->id][WINS];
			int losses = season_data->team_records[team->id][LOSSES];
			int draws = season_data->team_records[team->id][DRAWS];
			float record = 0;
			if ((wins + losses) > 0)
				record = (float)(season_data->team_records[team->id][WINS] / (season_data->team_records[team->id][WINS] + season_data->team_records[team->id][LOSSES]));
			
			printf("%2d -%2d", wins, losses);
			if(draws > 0)
				printf(" -%2d", draws);
			else
				printf("    ");
			printf("  %.3f", record);

			if (show_game) {
				int opp_id = GetOpponentTeamId(season_data, season_data->team_1_id, season_data->week);
				int home_or_away = GetHomeOrAway(season_data, season_data->team_1_id, season_data->week);
				const struct team_data* opponent = &default_league.teams[opp_id];
				printf("           ");
				if (home_or_away == 0) {				
					PrintSizedTeamName(opp_id, false);
					printf(" @ ");
					PrintSizedTeamName(team1->id, true);
				}
				else {
					PrintSizedTeamName(team1->id, true);
					printf(" @ ");
					PrintSizedTeamName(opp_id, false);
				}
				show_game = false;
			}
			printf("\r\n");
		}
	}

	SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
}

/*
* NAME:        ShowRegularSeasonMenuBottom
* DESCRIPTION: 
* INPUTS:      current selection, delay or not, season_data
* OUTPUTS:     none
*/
void ShowRegularSeasonMenuBottom(int current_menu_selection, bool first_text, struct season_data* season_data)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	printf("\r\n\r\n\r\n\r\n");
	printf("                       ");
	if (current_menu_selection == 0)
		SetConsoleTextAttribute(hConsole, CONSOLE_BG_WHITE | CONSOLE_TEXT_GREEN);
	if (first_text)
		PrintTextWithDelays("Play next game");
	else
		printf("Play next game");

	SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
	printf("       ");
	if (current_menu_selection == 1)
		SetConsoleTextAttribute(hConsole, CONSOLE_BG_WHITE | CONSOLE_TEXT_GREEN);
	if (first_text)
		PrintTextWithDelays("Sim next game");
	else
		printf("Sim next game");

	SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
	printf("       ");
	if (current_menu_selection == 2)
		SetConsoleTextAttribute(hConsole, CONSOLE_BG_WHITE | CONSOLE_TEXT_GREEN);
	if (first_text)
		PrintTextWithDelays("Leaderboard");
	else
		printf("Leaderboard");

	SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);

	printf("       ");
	if (current_menu_selection == 3)
		SetConsoleTextAttribute(hConsole, CONSOLE_BG_WHITE | CONSOLE_TEXT_GREEN);
	if (first_text)
		PrintTextWithDelays("Quit");
	else
		printf("Quit");

	SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
}

/*
* NAME:        ShowRegularSeasonMenu
* DESCRIPTION: game menu options
* INPUTS:      current selection, delay or not, season_data
* OUTPUTS:     none
*/
static void ShowRegularSeasonMenu(int current_menu_selection, bool first_text, struct season_data* season_data)
{
	ResetScreen();
	ShowMenuTitle();
	
	printf("\r\n\r\n");
	printf("\r\n\r\n");

	ShowRegularSeasonMenuTop(first_text, season_data);
	
	ShowRegularSeasonMenuBottom(current_menu_selection, first_text, season_data);
}

/*
* NAME:        RegularSeasonMenuOutcome
* DESCRIPTION: Gets which entry picked
* INPUTS:      season_data
* OUTPUTS:     menu selection
*/
int RegularSeasonMenuOutcome(struct season_data* season_data) {

	bool game_chosen = false;
	int current_menu_selection = 0;
	do {
		ShowRegularSeasonMenu(current_menu_selection, false, season_data);
		int ch = getch();

		if (ch == 0 || ch == 224) {   // extended key: read the scan code
			ch = getch();
			switch (ch) {
			case 72: // up arrow
				break;
			case 80: // down arrow
				break;
			case 75: // left arrow
				if(current_menu_selection > 0)
					current_menu_selection--;
				break;
			case 77: // right arrow
				if(current_menu_selection < (SEASON_MENU_OPTIONS_NUMBER - 1))
					current_menu_selection++;
				break;
			default:
				break;
			}
		}
		else if (ch == 13) {// Enter key
			return current_menu_selection;
		}
		else
			;
		// any other key: ignored, loop waits for another press
	} while (!game_chosen);
	return -1;
}

/*
* NAME:        RegularSeasonMenu
* DESCRIPTION: shows between game regular season menu
* INPUTS:      season_data
* OUTPUTS:     play, sim or quit
*/
int RegularSeasonMenu(struct season_data* season_data)
{
	system("cls");
	ShowMenuTitle();
	ShowRegularSeasonMenu(0, true, season_data);
	do {
		int selection = RegularSeasonMenuOutcome(season_data);
		if (selection != (SEASON_MENU_OPTIONS_NUMBER - 2))
			return selection;
		system("cls");
	} while (1);
	return -1;
}


/*
* NAME:        PlayRegularSeason
* DESCRIPTION: play out 6 regular season games
* INPUTS:      season data
* OUTPUTS:     int
*/
int PlayRegularSeason(struct season_data* season_data)
{
	for (int regular_season_game_no = 1; regular_season_game_no <= SEASON_NUMBER_OF_GAMES; regular_season_game_no++)
	{
		int play_or_sim_or_quit = RegularSeasonMenu(season_data);

		if (play_or_sim_or_quit == -1)
			return -1;

		// show default screen between games with options like "Next game", Quit season (with an are you sure screen?)
		// option "Show leaderboard" that shows, division, conference and league with playoff markers
		// get menu choice from that default screen^ and if go to next game then:
		
		
		
		// (SetupTeamData(season_data); or similar needs to happen before each game that is played)
		// play next season game
		// export game data back into season data

		// simulate other games for this week
		season_data->week++;
	}

	// show end of season screen? 
	// if not made post season "Quit" otherwise "Play post season"
}

/*
* NAME:        PlayPostSeason
* DESCRIPTION: play out post season games
* INPUTS:      season data
* OUTPUTS:     none
*/
void PlayPostSeason(struct season_data* season_data)
{
	for (int post_season_game_no = 1; post_season_game_no <= (SEASON_PLAYOFF_NUMBER_OF_GAMES + season_data->wildcard_round_needed); post_season_game_no++)
	{
		
		// Show default screen between games (shows playoff bracket) with options like "Next game", Quit season (with an are you sure screen?)
		// Option "Show leaderboard" that shows 2 conferences
		// Get menu choice from that default screen^ and if go to next game then:



		// (SetupTeamData(season_data); or similar needs to happen before each game that is played)
		// play next season game
		// export game data back into season data

		// simulate other games for this week
	}

	// show end of season screen? one to be photographable to show total season overall outcome
	// "Quit"
}

/*
* NAME:        PlaySeason
* DESCRIPTION: play out full season
* INPUTS:      season data
* OUTPUTS:     none
*/
void PlaySeason(struct season_data* season_data)
{
	if (PlayRegularSeason(season_data) == -1)
		return;
		
	PlayPostSeason(season_data);
}