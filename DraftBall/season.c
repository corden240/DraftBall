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
void PlayGame(struct game_data* game_data);
void DisableTeamsDecks(struct game_data* game_data);
void CoinToss(struct game_data* game_data);
void SimRegularSeasonGame(struct season_data* season_data);
void SimRestOfLeagueGames(struct season_data* season_data);
void PlayPostSeason(struct season_data* season_data);



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

	// Create array to hold teams and their records for sorting
	struct {
		const struct team_data* team;
		float record;
	} teams_in_division[LEAGUE_TEAM_COUNT];

	int division_count = 0;

	// Collect all teams in the same division with their records calculated
	for (int i = 0; i < LEAGUE_TEAM_COUNT; i++)
	{
		const struct team_data* team = &default_league.teams[i];
		if (team->division == team1->division)
		{
			int wins = season_data->team_records[team->id][WINS];
			int losses = season_data->team_records[team->id][LOSSES];
			int draws = season_data->team_records[team->id][DRAWS];

			int games = wins + losses + draws;
			float record = games > 0
				? ((float)wins + 0.5f * (float)draws) / (float)games
				: 0.0f;

			teams_in_division[division_count].team = team;
			teams_in_division[division_count].record = record;
			division_count++;
		}
	}

	// Sort teams by record in descending order (highest first)
	for (int i = 0; i < division_count - 1; i++)
	{
		for (int j = i + 1; j < division_count; j++)
		{
			if (teams_in_division[j].record > teams_in_division[i].record)
			{
				// Swap
				const struct team_data* temp_team = teams_in_division[i].team;
				float temp_record = teams_in_division[i].record;

				teams_in_division[i].team = teams_in_division[j].team;
				teams_in_division[i].record = teams_in_division[j].record;

				teams_in_division[j].team = temp_team;
				teams_in_division[j].record = temp_record;
			}
		}
	}

	// Print teams in sorted order
	bool show_game = true;
	for (int i = 0; i < division_count; i++)
	{
		const struct team_data* team = teams_in_division[i].team;

		printf("         ");
		PrintSizedTeamName(team->id, (team->id == team1->id) ? true : false);
		printf("   ");

		int wins = season_data->team_records[team->id][WINS];
		int losses = season_data->team_records[team->id][LOSSES];
		int draws = season_data->team_records[team->id][DRAWS];
		float record = teams_in_division[i].record;

		printf("%2d -%2d", wins, losses);
		if (draws > 0)
			printf(" -%2d", draws);
		else
			printf("    ");
		printf("  %.3f", record);

		if (show_game && season_data->week >= 1 && season_data->week <= SEASON_NUMBER_OF_GAMES) {
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
* NAME:        ShowEndOfRegularSeasonMenuBottom
* DESCRIPTION:
* INPUTS:      current selection, delay or not, season_data
* OUTPUTS:     none
*/
void ShowEndOfRegularSeasonMenuBottom(int current_menu_selection, bool first_text, struct season_data* season_data)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	printf("\r\n\r\n\r\n\r\n");
	printf("                       ");
	if (current_menu_selection == 0)
		SetConsoleTextAttribute(hConsole, CONSOLE_BG_WHITE | CONSOLE_TEXT_GREEN);
	if (first_text)
		PrintTextWithDelays("Play post season");
	else
		printf("Play post season");

	SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
	printf("       ");
	if (current_menu_selection == 1)
		SetConsoleTextAttribute(hConsole, CONSOLE_BG_WHITE | CONSOLE_TEXT_GREEN);
	if (first_text)
		PrintTextWithDelays("Leaderboard");
	else
		printf("Leaderboard");

	SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);

	printf("       ");
	if (current_menu_selection == 2)
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
* NAME:        ShowEndOfRegularSeasonMenu
* DESCRIPTION: game menu options
* INPUTS:      current selection, delay or not, season_data
* OUTPUTS:     none
*/
static void ShowEndOfRegularSeasonMenu(int current_menu_selection, bool first_text, struct season_data* season_data)
{
	ResetScreen();
	ShowMenuTitle();

	printf("\r\n\r\n");
	printf("\r\n\r\n");

	ShowRegularSeasonMenuTop(first_text, season_data);

	ShowEndOfRegularSeasonMenuBottom(current_menu_selection, first_text, season_data);
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
* NAME:        EndOfRegularSeasonMenuOutcome
* DESCRIPTION: Gets which entry picked
* INPUTS:      season_data
* OUTPUTS:     menu selection
*/
int EndOfRegularSeasonMenuOutcome(struct season_data* season_data) {

	bool game_chosen = false;
	int current_menu_selection = 0;
	do {
		ShowEndOfRegularSeasonMenu(current_menu_selection, false, season_data);
		int ch = getch();

		if (ch == 0 || ch == 224) {   // extended key: read the scan code
			ch = getch();
			switch (ch) {
			case 72: // up arrow
				break;
			case 80: // down arrow
				break;
			case 75: // left arrow
				if (current_menu_selection > 0)
					current_menu_selection--;
				break;
			case 77: // right arrow
				if (current_menu_selection < (END_OF_SEASON_MENU_OPTIONS_NUMBER - 1))
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
* NAME:        ShowRegularSeasonLeaderboardMenu
* DESCRIPTION: leaderboard
* INPUTS:      current selection, season_data
* OUTPUTS:     none
*/
static void ShowRegularSeasonLeaderboardMenu(struct season_data* season_data)
{
	struct standings_entry {
		const struct team_data* team;
		float record;
	} afc[CONFERENCE_TEAM_COUNT], nfc[CONFERENCE_TEAM_COUNT];

	int afc_count = 0;
	int nfc_count = 0;

	for (int i = 0; i < LEAGUE_TEAM_COUNT; i++)
	{
		const struct team_data* team = &default_league.teams[i];
		int wins = season_data->team_records[team->id][WINS];
		int losses = season_data->team_records[team->id][LOSSES];
		int draws = season_data->team_records[team->id][DRAWS];
		int games = wins + losses + draws;

		struct standings_entry entry = {
			team,
			games > 0
				? ((float)wins + 0.5f * (float)draws) / (float)games
				: 0.0f
		};

		if (team->division < 5)
			afc[afc_count++] = entry;
		else if (team->division > 4)
			nfc[nfc_count++] = entry;
	}

	/* Stable insertion sort: highest record first. */
	for (int i = 1; i < afc_count; i++)
	{
		struct standings_entry entry = afc[i];
		int j = i;
		while (j > 0 && afc[j - 1].record < entry.record)
		{
			afc[j] = afc[j - 1];
			j--;
		}
		afc[j] = entry;
	}

	for (int i = 1; i < nfc_count; i++)
	{
		struct standings_entry entry = nfc[i];
		int j = i;
		while (j > 0 && nfc[j - 1].record < entry.record)
		{
			nfc[j] = nfc[j - 1];
			j--;
		}
		nfc[j] = entry;
	}

	system("cls");
	ResetScreen();
	ShowMenuTitle();

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
	printf("                      AFC     W   L   D  Record              NFC     W   L   D  Record\r\n");

	for (int i = 0; i < afc_count || i < nfc_count; i++)
	{
		if (i == CONFERENCE_TEAM_COUNT / 2)
		{
			printf("\r\n                ----------");
			for (int space = 0; space < 29; space++)
				putchar(' ');
			printf("----------");
		}

		printf("\r\n                ");

		if (i < afc_count)
		{
			const struct team_data* team = afc[i].team;
			int wins = season_data->team_records[team->id][WINS];
			int losses = season_data->team_records[team->id][LOSSES];
			int draws = season_data->team_records[team->id][DRAWS];

			PrintSizedTeamName(team->id, team->id == season_data->team_1_id);
			printf("   %2d -%2d", wins, losses);
			if (draws > 0)
				printf(" -%2d", draws);
			else
				printf("    ");
			printf("  %.3f", afc[i].record);
		}

		printf("         ");

		if (i < nfc_count)
		{
			const struct team_data* team = nfc[i].team;
			int wins = season_data->team_records[team->id][WINS];
			int losses = season_data->team_records[team->id][LOSSES];
			int draws = season_data->team_records[team->id][DRAWS];

			PrintSizedTeamName(team->id, team->id == season_data->team_1_id);
			printf("   %2d -%2d", wins, losses);
			if (draws > 0)
				printf(" -%2d", draws);
			else
				printf("    ");
			printf("  %.3f", nfc[i].record);
		}
	}

	printf("\r\n");
	printf("                                                   ");
	SetConsoleTextAttribute(hConsole, CONSOLE_BG_WHITE | CONSOLE_TEXT_GREEN);
	printf("Back");
	SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
}

/*
* NAME:        RegularSeasonLeaderboardMenu
* DESCRIPTION: shows regular season leaderboard
* INPUTS:      season_data
* OUTPUTS:     void
*/
void RegularSeasonLeaderboardMenu(struct season_data* season_data)
{
	bool game_chosen = false;
	do {
		ShowRegularSeasonLeaderboardMenu(season_data);
		int ch = getch();

		if (ch == 0 || ch == 224) {   // extended key: read the scan code
			ch = getch();
			switch (ch) {
			default:
				break;
			}
		}
		else if (ch == 13) {// Enter key
			return;
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
		if (selection == 2)
			RegularSeasonLeaderboardMenu(season_data);
		else
			return selection;
		system("cls");
	} while (1);
	return -1;
}

/*
* NAME:        SetupTeamDataSeasonGame
* DESCRIPTION: builds team names colours player names etc from season data
* INPUTS:      season_data, game_data
* OUTPUTS:     void
*/
void SetupTeamDataSeasonGame(struct season_data* season_data, struct game_data* game_data)
{
	int opp_id = GetOpponentTeamId(season_data, season_data->team_1_id, season_data->week);
	int home_or_away = GetHomeOrAway(season_data, season_data->team_1_id, season_data->week);
	game_data->line_of_scrimmage = STARTING_LINE_OF_SCRIMMAGE + END_ZONE_WIDTH;
	game_data->line_of_first_down = 0;
	game_data->down = 1;
	game_data->team_in_possession = 0;
	game_data->team_not_in_possession = 0;
	game_data->team_1_score = 0;
	game_data->team_2_score = 0;
	game_data->quarter = 1;
	game_data->plays_per_quarter = season_data->plays_per_quarter;
	game_data->plays_remaining_in_quarter = game_data->plays_per_quarter;
	for (int i = 0; i < NUMBER_OF_DECKS; i++) {
		game_data->shuffle_deck_needed[i] = 0;
	}
	game_data->team_1_timeouts_remaining = TIMEOUTS_PER_TEAM_PER_HALF;
	game_data->team_2_timeouts_remaining = TIMEOUTS_PER_TEAM_PER_HALF;
	game_data->team_started_with_possession = 0;
	game_data->team_1_direction = 1; // default direction
	game_data->team_2_direction = 2; // default direction

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

	game_data->fouls = season_data->fouls;
	game_data->current_game_mode = 1;

	/*game stats data*/
	game_data->playoffs = season_data->playoffs; // true if we are currently in the playoff part of the season, use to not add to record
	for (int i = 0; i < NUMBER_OF_TEAMS_PER_GAME; i++) {
		for (int j = 0; j < NUMBER_OF_QUARTERS; j++) {
			game_data->quarter_scores[i][j] = 0;
		}
	}
	for (int i = 0; i < NUMBER_OF_TEAMS_PER_GAME; i++) {
		for (int j = 0; j < WIN_LOSS_DRAW; j++) {
			if((i == 0 && home_or_away == 1) || (i == 1 && home_or_away == 0))
				game_data->team_records[i][j] = season_data->team_records[season_data->team_1_id][j];
			else 
				game_data->team_records[i][j] = season_data->team_records[opp_id][j];
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
	game_data->one_player_team_controlled = GetHomeOrAway(season_data, season_data->team_1_id, season_data->week) ? 1 : 2;

	game_data->team_1_id = season_data->team_1_id;
	game_data->team_2_id = GetOpponentTeamId(season_data, season_data->team_1_id, season_data->week);

	game_data->two_min_drive_deficit_index = 0;
	game_data->two_min_drive_yards_index = 0;

	game_data->cpu_called_timeout = false;

	game_data->show_play_types = false;
	game_data->show_qb_scramble_option = false;


	const struct team_data* team_1;
	const struct team_data* team_2;

	if (home_or_away == 0) {
		team_1 = &default_league.teams[opp_id];
		team_2 = &default_league.teams[season_data->team_1_id];
	}
	else {
		team_1 = &default_league.teams[season_data->team_1_id];
		team_2 = &default_league.teams[opp_id];
	}

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

	DisableTeamsDecks(game_data);
}

/*
* NAME:        ExportGameData
* DESCRIPTION: export game data to season data
* INPUTS:      season data, game data
* OUTPUTS:     void
*/
void ExportGameData(struct season_data* season_data, struct game_data* game_data)
{
	int home_or_away = GetHomeOrAway(season_data, season_data->team_1_id, season_data->week);
	if (game_data->team_1_score > game_data->team_2_score)
	{
		if (home_or_away == 1)
		{
			season_data->team_records[season_data->team_1_id][WINS]++;
			season_data->team_records[GetOpponentTeamId(season_data, season_data->team_1_id, season_data->week)][LOSSES]++;
		}
		else
		{
			season_data->team_records[season_data->team_1_id][LOSSES]++;
			season_data->team_records[GetOpponentTeamId(season_data, season_data->team_1_id, season_data->week)][WINS]++;
		}
	}
	else if (game_data->team_2_score > game_data->team_1_score)
	{
		if (home_or_away == 0)
		{
			season_data->team_records[season_data->team_1_id][WINS]++;
			season_data->team_records[GetOpponentTeamId(season_data, season_data->team_1_id, season_data->week)][LOSSES]++;
		}
		else
		{
			season_data->team_records[season_data->team_1_id][LOSSES]++;
			season_data->team_records[GetOpponentTeamId(season_data, season_data->team_1_id, season_data->week)][WINS]++;
		}
	}
	else
	{
		season_data->team_records[season_data->team_1_id][DRAWS]++; // add to both teams draws
		season_data->team_records[GetOpponentTeamId(season_data, season_data->team_1_id, season_data->week)][DRAWS]++;
	}
}

/*
* NAME:        PlayRegularSeasonGame
* DESCRIPTION: play out the next regular season game
* INPUTS:      season data
* OUTPUTS:     int
*/
void PlayRegularSeasonGame(struct season_data* season_data)
{
	struct game_data game_data;
	
	// setup game data with right teams etc from season data
	SetupTeamDataSeasonGame(season_data, &game_data);

	CoinToss(&game_data);

	// play next season game
	PlayGame(&game_data);

	// export game data back into season data
	ExportGameData(season_data, &game_data); 
}

/*
* NAME:        EndOfRegularSeasonMenu
* DESCRIPTION: shows between game regular season menu
* INPUTS:      season_data
* OUTPUTS:     play, sim or quit
*/
int EndOfRegularSeasonMenu(struct season_data* season_data)
{
	system("cls");
	ShowMenuTitle();
	ShowEndOfRegularSeasonMenu(0, true, season_data);
	do {
		int selection = EndOfRegularSeasonMenuOutcome(season_data);
		if (selection == 1)
			RegularSeasonLeaderboardMenu(season_data);
		else
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

		if (play_or_sim_or_quit == (SEASON_MENU_OPTIONS_NUMBER - 1)) // "Quit"
			return -1;
		else if (play_or_sim_or_quit == (SEASON_MENU_OPTIONS_NUMBER - 3)) // "Sim game"
		{
			SimRegularSeasonGame(season_data);
		}
		else if (play_or_sim_or_quit == (SEASON_MENU_OPTIONS_NUMBER - 4)) // "Play game"
		{
			PlayRegularSeasonGame(season_data);
		}	

		SimRestOfLeagueGames(season_data); // simulate all other games for this week
		season_data->week++;
	}

	return EndOfRegularSeasonMenu(season_data) == 0 ? 0 : -1;
}

/*
* NAME:        PlaySeason
* DESCRIPTION: play out full season
* INPUTS:      season data
* OUTPUTS:     none
*/
void PlaySeason(struct season_data* season_data)
{
	if (PlayRegularSeason(season_data) != 0)
		return;

	PlayPostSeason(season_data);
}