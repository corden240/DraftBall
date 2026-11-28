#include <stdio.h>
#include <stdlib.h>   /* srand, rand, atoi */
#include <time.h>     /* time() */
#include "main.h"
#include <string.h>
#include <windows.h>
#include "teamdata.h"
#include "main_d.prt"

void SetupTeamDataSeasonGame(struct season_data* season_data, struct game_data* game_data);
void ExportGameData(struct season_data* season_data, struct game_data* game_data);
bool PcChance(int chance);
void ResetScreen();
int TruncateKeepRight(char* dst, size_t dst_size, const char* src, int max_len);
void ShowMenuTitle();



/*
* NAME:        ShowTopSimInfo
* DESCRIPTION: displays timeouts, teams, scores, and clock
* INPUTS:      none
* OUTPUTS:     none
*/
void ShowTopSimInfo(struct game_data* game_data, int plays_per_quarter_in_sim)
{
	printf("\r");
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	/* ---- Colours: change any value here to recolour that part ---- */
	const int COLOUR_BASE = CONSOLE_TEXT_WHITE;   /* padding / separators / default */
	const int COLOUR_TEAM_1_NAME = game_data->team_1_colour;
	const int COLOUR_TEAM_2_NAME = game_data->team_2_colour;
	const int COLOUR_SCORE = CONSOLE_TEXT_WHITE;   /* "N : N" */
	const int COLOUR_CLOCK = CONSOLE_TEXT_WHITE;   /* quarter / plays left */
	/* --------------------------------------------------------------- */

	SetConsoleTextAttribute(hConsole, COLOUR_BASE);

	const int SIDE_GAP = 6;          /* exactly 10 spaces on each side of the middle */
	const char DIV = ' ';
	/* team 1 block: pips only (their trailing space is the separator);
	   team 2 block: 1 space + pips. Total = 2 * BLOCKS_WIDTH - 1 */
	int width = WINDOW_WIDTH;
	int center = (width / 2);     /* index where middle should appear (0-based) */
	if (center < 0) center = 0;

	/* ---- Build the individual segments (local buffers, no accumulation) ---- */
	char t1_name[64];
	char score[32];
	char t2_name[64];
	char right_raw[128];
	snprintf(t1_name, sizeof(t1_name), "%s", game_data->team_1_name);
	snprintf(score, sizeof(score), "  %d : %d  ", game_data->team_1_score, game_data->team_2_score);
	snprintf(t2_name, sizeof(t2_name), "%s", game_data->team_2_name);
	snprintf(right_raw, sizeof(right_raw), "Quarter: %d  Plays Left: %d",
		game_data->quarter, plays_per_quarter_in_sim);

	/* left gap must start SIDE_GAP chars left of center */
	int left_gap_start = center - SIDE_GAP;
	if (left_gap_start < 0) left_gap_start = 0;

	/* Left side layout: [team1 pips] t1_arrows t1_name score t2_name t2_arrows [team2 pips].
	   Pips print with colour changes, so only the core string is truncated;
	   keep the rightmost characters so scores remain visible. Truncation is
	   applied proportionally across the segments. */
	int left_max_len = left_gap_start;
	int core_max_len = left_max_len;
	int left_raw_len = strlen(t1_name) + strlen(score)
		+ strlen(t2_name);
	double keep_ratio = 1.0;
	if (left_raw_len > core_max_len) {
		if (core_max_len <= 0) {
			/* too narrow for pips; drop them */
			core_max_len = left_max_len;
		}
		keep_ratio = (left_raw_len > 0) ? ((double)core_max_len / (double)left_raw_len) : 0.0;
	}

	char t1_name_d[64];
	char score_d[32];
	char t2_name_d[64];
	int left_display_len = 0;
	left_display_len += TruncateKeepRight(t1_name_d, sizeof(t1_name_d), t1_name, (int)(strlen(t1_name) * keep_ratio));
	left_display_len += TruncateKeepRight(score_d, sizeof(score_d), score, (int)(strlen(score) * keep_ratio));
	left_display_len += TruncateKeepRight(t2_name_d, sizeof(t2_name_d), t2_name, (int)(strlen(t2_name) * keep_ratio));

	/* Leading padding so the left display ends at left_gap_start-1 */
	int leading_spaces = left_gap_start - left_display_len;
	if (leading_spaces < 0) leading_spaces = 0;

	/* Right display must start after the right gap (center + SIDE_GAP + 1).
	   Truncate if it won't fit to the right edge. */
	int right_start = center + SIDE_GAP + 1;
	if (right_start < 0) right_start = 0;
	int right_max_len = width - right_start;
	size_t right_raw_len = strlen(right_raw);
	char right_display[128];
	if (right_max_len <= 0) {
		right_display[0] = '\0';
	}
	else if ((int)right_raw_len > right_max_len) {
		strncpy(right_display, right_raw, right_max_len);
		right_display[right_max_len] = '\0';
	}
	else {
		strcpy(right_display, right_raw);
	}

	/* ---- Print in segments so each part can use its own colour ---- */
	SetConsoleTextAttribute(hConsole, COLOUR_BASE);
	for (int i = 0; i < leading_spaces; i++) putchar(' ');
	SetConsoleTextAttribute(hConsole, COLOUR_TEAM_1_NAME);
	printf("%s", t1_name_d);
	SetConsoleTextAttribute(hConsole, COLOUR_SCORE);
	printf("%s", score_d);
	SetConsoleTextAttribute(hConsole, COLOUR_TEAM_2_NAME);
	printf("%s", t2_name_d);
	SetConsoleTextAttribute(hConsole, COLOUR_BASE);
	for (int i = 0; i < SIDE_GAP; i++) putchar(' ');
	putchar(DIV);
	for (int i = 0; i < SIDE_GAP; i++) putchar(' ');
	SetConsoleTextAttribute(hConsole, COLOUR_CLOCK);
	printf("%s", right_display);

	/* Pad the rest of the line out to the window width */
	SetConsoleTextAttribute(hConsole, COLOUR_BASE);
	int line_len = left_gap_start + SIDE_GAP + 1 + SIDE_GAP + (int)strlen(right_display);
	for (int i = line_len; i < width; i++) putchar(' ');

	SetConsoleTextAttribute(hConsole, COLOUR_BASE);

	SetConsoleTextAttribute(hConsole, COLOUR_BASE);
}

/*
* NAME:        ShowSeasonSimScreen
* DESCRIPTION: show sim screen
* INPUTS:      game data
* OUTPUTS:     none
*/
void ShowSeasonSimScreen(struct game_data* game_data, int pause, int plays_per_quarter_in_sim)
{
	ShowTopSimInfo(game_data, plays_per_quarter_in_sim);
	if (pause)
		Sleep(pause);
}

/*
* NAME:        GetTeamRating
* DESCRIPTION: gets teams difficulty rating from team id
* INPUTS:      season_data
* OUTPUTS:     deck/team rating id
*/
int GetTeamRating(int teamid)
{
	const struct team_data* team = &default_league.teams[teamid];
	return team->deck_id;
}

/*
* NAME:        SimGame
* DESCRIPTION: sim the next regular season game
* INPUTS:      game data
* OUTPUTS:     none
*/
void SimGame(struct game_data* game_data)
{
	system("cls");
	ShowMenuTitle();
	printf("\n\r\n\r\n\r");
	ShowSeasonSimScreen(game_data, 1000, SIM_PLAYS_PER_QUARTER); // show sim screen, pause for "start game"

	int team1rating = GetTeamRating(game_data->team_1_id);
	int team2rating = GetTeamRating(game_data->team_2_id);
	int team_with_ball = 1;

	for (game_data->quarter;
		game_data->quarter <= NUMBER_OF_QUARTERS ||
		(game_data->playoffs && game_data->team_1_score == game_data->team_2_score);
		game_data->quarter++)
	{
		for (int plays_per_quarter_in_sim = SIM_PLAYS_PER_QUARTER; plays_per_quarter_in_sim > 0; plays_per_quarter_in_sim--)
		{
			bool scored = false;
			if (team_with_ball == 1 && PcChance(team_ratings_to_pcchance[team1rating]))
			{
				game_data->team_1_score += TableWeightDecision(sim_game_points_to_add, true);
				scored = true;
			}
			else if (team_with_ball == 2 && PcChance(team_ratings_to_pcchance[team2rating]))
			{
				game_data->team_2_score += TableWeightDecision(sim_game_points_to_add, true);
				scored = true;
			}

			if (scored) {
				if (team_with_ball == 1)
					team_with_ball = 2;
				else
					team_with_ball = 1;
			}
			else if(PcChance(SIM_CHANCE_TO_CHANGE_POSSESSION))
			{
				if (team_with_ball == 1)
					team_with_ball = 2;
				else
					team_with_ball = 1;
			}
			ShowSeasonSimScreen(game_data, 2, plays_per_quarter_in_sim); // TODO: set back to 400
		}
	}
	game_data->quarter--;
	ShowSeasonSimScreen(game_data, 0, 0);
	printf("\n\r\n\r\n\r\n\r                                                   ");
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, CONSOLE_BG_WHITE);
	printf("Continue");
	SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
	int ch = getch();
}

/*
* NAME:        SimRegularSeasonGame
* DESCRIPTION: sim the next regular season game
* INPUTS:      season data
* OUTPUTS:     void
*/
void SimRegularSeasonGame(struct season_data* season_data)
{
	struct game_data game_data;

	// setup game data with right teams etc from season data
	SetupTeamDataSeasonGame(season_data, &game_data);

	// play next season game
	SimGame(&game_data);

	// export game data back into season data
	ExportGameData(season_data, &game_data);
}