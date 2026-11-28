#include <stdio.h>
#include <stdlib.h>   /* srand, rand, atoi */
#include <time.h>     /* time() */
#include "main.h"
#include <string.h>
#include <windows.h>
#include <conio.h>
#include <ctype.h>
#include "teamdata.h"

void ShowMenuTitle();
void PlayGame(struct game_data* game_data);
void CoinToss(struct game_data* game_data);
void DisableTeamsDecks(struct game_data* game_data);
void SetupTeamDataSeasonGameOtherTeam(
	struct season_data* season_data,
	struct game_data* game_data,
	int team_1_id,
	int team_2_id);
void SimGameOtherTeam(struct game_data* game_data);
void RegularSeasonLeaderboardMenu(struct season_data* season_data);

#define PLAYOFF_TEAMS_PER_CONFERENCE 8
#define PLAYOFF_GAMES_PER_CONFERENCE (PLAYOFF_TEAMS_PER_CONFERENCE / 2)
#define POSTSEASON_MENU_OPTION_COUNT 4

struct playoff_team
{
	int team_id;
	int seed;
	float record;
};

struct playoff_game
{
	struct playoff_team away;
	struct playoff_team home;
	int away_score;
	int home_score;
	bool played;
};

static float GetPlayoffRecord(const struct season_data* season_data, int team_id)
{
	int wins = season_data->team_records[team_id][WINS];
	int losses = season_data->team_records[team_id][LOSSES];
	int draws = season_data->team_records[team_id][DRAWS];
	int games = wins + losses + draws;

	return games > 0
		? ((float)wins + 0.5f * (float)draws) / (float)games
		: 0.0f;
}

static int BuildConferenceSeeds(
	const struct season_data* season_data,
	bool afc,
	struct playoff_team seeds[PLAYOFF_TEAMS_PER_CONFERENCE])
{
	struct playoff_team standings[CONFERENCE_TEAM_COUNT];
	int count = 0;
	for (int i = 0; i < LEAGUE_TEAM_COUNT; i++)
	{
		const struct team_data* team = &default_league.teams[i];
		if ((afc && team->division >= 5) || (!afc && team->division <= 4))
			continue;

		standings[count].team_id = team->id;
		standings[count].seed = 0;
		standings[count].record = GetPlayoffRecord(season_data, team->id);
		count++;
	}

	for (int i = 1; i < count; i++)
	{
		struct playoff_team team = standings[i];
		int j = i;
		while (j > 0 && standings[j - 1].record < team.record)
		{
			standings[j] = standings[j - 1];
			j--;
		}
		standings[j] = team;
	}

	int playoff_count = count < PLAYOFF_TEAMS_PER_CONFERENCE
		? count : PLAYOFF_TEAMS_PER_CONFERENCE;
	for (int i = 0; i < playoff_count; i++)
	{
		seeds[i] = standings[i];
		seeds[i].seed = i + 1;
	}

	return playoff_count;
}

static struct playoff_game CreatePlayoffGame(
	struct playoff_team team_a,
	struct playoff_team team_b)
{
	struct playoff_game game = { 0 };
	struct playoff_team home = team_a;
	struct playoff_team away = team_b;

	if (team_b.seed < team_a.seed ||
		(team_b.seed == team_a.seed && team_b.record > team_a.record))
	{
		home = team_b;
		away = team_a;
	}

	game.away = away;
	game.home = home;
	return game;
}

static void BuildOpeningRound(
	const struct playoff_team seeds[PLAYOFF_TEAMS_PER_CONFERENCE],
	struct playoff_game games[PLAYOFF_GAMES_PER_CONFERENCE])
{
	games[0] = CreatePlayoffGame(seeds[0], seeds[7]);
	games[1] = CreatePlayoffGame(seeds[3], seeds[4]);
	games[2] = CreatePlayoffGame(seeds[1], seeds[6]);
	games[3] = CreatePlayoffGame(seeds[2], seeds[5]);
}

static struct playoff_team GetPlayoffWinner(const struct playoff_game* game)
{
	return game->away_score > game->home_score ? game->away : game->home;
}

static void BuildNextRound(
	const struct playoff_game* previous_round,
	int previous_game_count,
	struct playoff_game* next_round)
{
	for (int i = 0; i < previous_game_count / 2; i++)
	{
		struct playoff_team first_winner = GetPlayoffWinner(&previous_round[i * 2]);
		struct playoff_team second_winner = GetPlayoffWinner(&previous_round[i * 2 + 1]);
		next_round[i] = CreatePlayoffGame(first_winner, second_winner);
	}
}

static void SetupPlayoffGame(
	struct season_data* season_data,
	struct game_data* game_data,
	const struct playoff_game* playoff_game)
{
	memset(game_data, 0, sizeof(*game_data));
	game_data->line_of_scrimmage = STARTING_LINE_OF_SCRIMMAGE + END_ZONE_WIDTH;
	game_data->down = 1;
	game_data->team_1_score = 0;
	game_data->team_2_score = 0;
	game_data->quarter = 1;
	game_data->plays_per_quarter = season_data->plays_per_quarter;
	game_data->plays_remaining_in_quarter = season_data->plays_per_quarter;
	game_data->team_1_timeouts_remaining = TIMEOUTS_PER_TEAM_PER_HALF;
	game_data->team_2_timeouts_remaining = TIMEOUTS_PER_TEAM_PER_HALF;
	game_data->team_1_direction = 1;
	game_data->team_2_direction = 2;
	game_data->kickoff_needed = true;
	game_data->fouls = season_data->fouls;
	game_data->current_game_mode = 1;
	game_data->playoffs = true;
	game_data->one_player = true;
	game_data->team_1_id = playoff_game->away.team_id;
	game_data->team_2_id = playoff_game->home.team_id;
	game_data->one_player_team_controlled =
		season_data->team_1_id == game_data->team_1_id ? 1 : 2;
	game_data->show_play_types = false;
	game_data->show_qb_scramble_option = false;

	const struct team_data* team_1 = &default_league.teams[game_data->team_1_id];
	const struct team_data* team_2 = &default_league.teams[game_data->team_2_id];
	snprintf(game_data->team_1_name, sizeof(game_data->team_1_name), "%s", team_1->name);
	snprintf(game_data->team_2_name, sizeof(game_data->team_2_name), "%s", team_2->name);
	snprintf(game_data->team_1_qb_names[0], sizeof(game_data->team_1_qb_names[0]), "%s", team_1->qb_name);
	snprintf(game_data->team_2_qb_names[0], sizeof(game_data->team_2_qb_names[0]), "%s", team_2->qb_name);

	for (int i = 0; i < NUMBER_OF_RB_NAMES_PER_TEAM; i++)
	{
		snprintf(game_data->team_1_rb_names[i], sizeof(game_data->team_1_rb_names[i]), "%s", team_1->rb_names[i]);
		snprintf(game_data->team_2_rb_names[i], sizeof(game_data->team_2_rb_names[i]), "%s", team_2->rb_names[i]);
	}
	for (int i = 0; i < NUMBER_OF_WR_NAMES_PER_TEAM; i++)
	{
		snprintf(game_data->team_1_wr_names[i], sizeof(game_data->team_1_wr_names[i]), "%s", team_1->wr_names[i]);
		snprintf(game_data->team_2_wr_names[i], sizeof(game_data->team_2_wr_names[i]), "%s", team_2->wr_names[i]);
	}

	game_data->team_1_colour = team_1->colour;
	game_data->team_2_colour = team_2->colour;
	game_data->players_named = true;
	for (int i = 0; i < NUMBER_OF_TEAMS_PER_GAME; i++)
	{
		int team_id = i == 0 ? game_data->team_1_id : game_data->team_2_id;
		for (int j = 0; j < WIN_LOSS_DRAW; j++)
			game_data->team_records[i][j] = season_data->team_records[team_id][j];
	}

	DisableTeamsDecks(game_data);
}

static void SimulatePlayoffGame(
	struct season_data* season_data,
	struct playoff_game* playoff_game)
{
	struct game_data game_data = { 0 };
	SetupTeamDataSeasonGameOtherTeam(
		season_data,
		&game_data,
		playoff_game->away.team_id,
		playoff_game->home.team_id);
	SimGameOtherTeam(&game_data);
	playoff_game->away_score = game_data.team_1_score;
	playoff_game->home_score = game_data.team_2_score;
	playoff_game->played = true;
}

static void ResolvePlayoffGame(
	struct season_data* season_data,
	struct playoff_game* playoff_game,
	bool play_user_game)
{
	if (play_user_game)
	{
		struct game_data game_data;
		SetupPlayoffGame(season_data, &game_data, playoff_game);
		CoinToss(&game_data);
		PlayGame(&game_data);
		playoff_game->away_score = game_data.team_1_score;
		playoff_game->home_score = game_data.team_2_score;
		playoff_game->played = true;
	}
	else
	{
		SimulatePlayoffGame(season_data, playoff_game);
	}
}

static const char* GetPlayoffRoundName(int round)
{
	static const char* names[SEASON_PLAYOFF_NUMBER_OF_GAMES] = {
		"Wildcard Round",
		"Conference Semifinals",
		"Conference Championships",
		"SuperBowl"
	};
	return names[round];
}

static void SetBracketCursor(int x, int y)
{
	COORD position = { (SHORT)x, (SHORT)y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
}

static void PrintBracketEntry(
	int x,
	int y,
	const struct playoff_team* team,
	int score,
	bool score_available,
	bool right_side)
{
	SetBracketCursor(x, y);
	if (team->seed == 0)
	{
		printf("%-*s", TEAM_NAME_FIELD_WIDTH + 3, "");
		return;
	}

	const char* name = default_league.teams[team->team_id].name;
	if (right_side)
	{
		if (score_available)
			printf("%2d", score);
		else
			printf("  ");
		printf(" %-*.*s", TEAM_NAME_FIELD_WIDTH,
			TEAM_NAME_FIELD_WIDTH, name);
	}
	else
	{
		printf("%-*.*s", TEAM_NAME_FIELD_WIDTH,
			TEAM_NAME_FIELD_WIDTH, name);
		if (score_available)
			printf("%2d", score);
		else
			printf("  ");
	}
}

static void PrintBracketGame(
	int x,
	const struct playoff_game* game,
	int away_y,
	int home_y,
	bool right_side)
{
	if (!game->played)
	{
		const struct playoff_team placeholder = { 0 };
		PrintBracketEntry(x, away_y,
			game->away.seed != 0 ? &game->away : &placeholder,
		game->away_score, game->away.seed != 0, right_side);
		PrintBracketEntry(x, home_y,
			game->home.seed != 0 ? &game->home : &placeholder,
		game->home_score, game->home.seed != 0, right_side);
		return;
	}

	PrintBracketEntry(x, away_y, &game->away, game->away_score,
		true, right_side);
	PrintBracketEntry(x, home_y, &game->home, game->home_score,
		true, right_side);
}

static void ShowPlayoffRound(
	int round,
	const struct playoff_game afc_rounds[3][PLAYOFF_GAMES_PER_CONFERENCE],
	const struct playoff_game nfc_rounds[3][PLAYOFF_GAMES_PER_CONFERENCE],
	const struct playoff_game* championship)
{
	system("cls");
	ShowMenuTitle();
	SetBracketCursor(0, 8);
	printf("             AFC                                                     NFC       \r\n");
	// Divisional   Conf Semi   Conf Final    Superbowl   Conf Final   Conf Semi   Divisional
	// 
	//  Seed 8 x																	x Seed 8
	//  Seed 1 y	[     ] x										  x [     ]     y Seed 1
	// 				[     ] y									      y [     ]			
	// 	Seed 5 x																    x Seed 5
	// 	Seed 4 y		         [     ] x	  [     ] x   x [     ]			        y Seed 4
	// 							 [     ] y	  [     ] y	  y [     ]							
	// 	Seed 7 x																	x Seed 7
	// 	Seed 2 y    [     ] x									      x [     ]     y Seed 2
	// 				[     ] y									      y [     ]			
	// 	Seed 6 x																	x Seed 6
	// 	Seed 3 y																	y Seed 3

	const int column_x[7] = { 3, 17, 31, 45, 59, 73, 87 };
	const char* headings[7] = {
		"Divisional", "Conf Semi", "Conf Final", "Superbowl",
		"Conf Final", "Conf Semi", "Divisional"
	};
	const int seed_rows[PLAYOFF_TEAMS_PER_CONFERENCE] = {
		12, 18, 21, 15, 14, 20, 17, 11
	};
	const int opening_game_for_seed[PLAYOFF_TEAMS_PER_CONFERENCE] = {
		0, 2, 3, 1, 1, 3, 2, 0
	};
	const int opening_seed_for_row[PLAYOFF_TEAMS_PER_CONFERENCE] = {
		8, 1, 5, 4, 7, 2, 6, 3
	};

	for (int column = 0; column < 7; column++)
	{
		SetBracketCursor(column_x[column], 9);
		printf("%-*.*s", TEAM_NAME_FIELD_WIDTH,
			TEAM_NAME_FIELD_WIDTH, headings[column]);
	}


	for (int row = 0; row < PLAYOFF_TEAMS_PER_CONFERENCE; row++)
	{
		int seed = opening_seed_for_row[row];
		int game_index = opening_game_for_seed[seed - 1];
		const struct playoff_game* afc_game = &afc_rounds[0][game_index];
		const struct playoff_game* nfc_game = &nfc_rounds[0][game_index];
		const struct playoff_team* afc_team = afc_game->away.seed == seed
			? &afc_game->away : &afc_game->home;
		const struct playoff_team* nfc_team = nfc_game->away.seed == seed
			? &nfc_game->away : &nfc_game->home;
		int afc_score = afc_team == &afc_game->away
			? afc_game->away_score : afc_game->home_score;
		int nfc_score = nfc_team == &nfc_game->away
			? nfc_game->away_score : nfc_game->home_score;
		int y = seed_rows[row];
		PrintBracketEntry(column_x[0], y, afc_team, afc_score,
			afc_game->played, false);
		PrintBracketEntry(column_x[6], y, nfc_team, nfc_score,
			nfc_game->played, true);
	}

	PrintBracketGame(column_x[1], &afc_rounds[1][0], 13, 16,
		false);
	PrintBracketGame(column_x[1], &afc_rounds[1][1], 19, 22,
		false);
	PrintBracketGame(column_x[5], &nfc_rounds[1][0], 13, 16,
		true);
	PrintBracketGame(column_x[5], &nfc_rounds[1][1], 19, 22,
		true);
	PrintBracketGame(column_x[2], &afc_rounds[2][0], 14, 20,
		false);
	PrintBracketGame(column_x[4], &nfc_rounds[2][0], 14, 20,
		true);

	if (championship->played)
	{
		PrintBracketEntry(column_x[3], 15, &championship->away,
			championship->away_score, true, false);
		PrintBracketEntry(column_x[4], 19, &championship->home,
			championship->home_score, true, true);
	}
	else
	{
		const struct playoff_team placeholder = { 0 };
		PrintBracketEntry(column_x[3], 15, &placeholder, 0, false, false);
		PrintBracketEntry(column_x[4], 19, &placeholder, 0, false, true);
	}
	(void)round;
}

enum postseason_action
{
	POSTSEASON_PLAY_ROUND,
	POSTSEASON_SIMULATE_ROUND,
	POSTSEASON_QUIT
};

static void ShowPostseasonActionOptions(int current_menu_selection)
{
	static const char* options[POSTSEASON_MENU_OPTION_COUNT] = {
		"Play round",
		"Simulate round",
		"Leaderboard",
		"Quit"
	};
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetBracketCursor(0, 23);
	printf("\r\n          ");
	for (int i = 0; i < POSTSEASON_MENU_OPTION_COUNT; i++)
	{
		if (i == current_menu_selection)
			SetConsoleTextAttribute(hConsole, CONSOLE_BG_WHITE | CONSOLE_TEXT_GREEN);
		else
			SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
		printf("%s", options[i]);
		SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
		if (i < POSTSEASON_MENU_OPTION_COUNT - 1)
			printf("       ");
	}
	SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
}

static int GetPostseasonAction(
	struct season_data* season_data,
	int round,
	const struct playoff_game afc_rounds[3][PLAYOFF_GAMES_PER_CONFERENCE],
	const struct playoff_game nfc_rounds[3][PLAYOFF_GAMES_PER_CONFERENCE],
	const struct playoff_game* championship)
{
	int current_menu_selection = 0;
	for (;;)
	{
		ShowPlayoffRound(round, afc_rounds, nfc_rounds, championship);
		ShowPostseasonActionOptions(current_menu_selection);
		int key = getch();

		if (key == 0 || key == 224)
		{
			key = getch();
			switch (key)
			{
			case 75:
				if (current_menu_selection > 0)
					current_menu_selection--;
				break;
			case 77:
				if (current_menu_selection < POSTSEASON_MENU_OPTION_COUNT - 1)
					current_menu_selection++;
				break;
			default:
				break;
			}
		}
		else if (key == 13)
		{
			if (current_menu_selection == 0)
				return POSTSEASON_PLAY_ROUND;
			if (current_menu_selection == 1)
				return POSTSEASON_SIMULATE_ROUND;
			if (current_menu_selection == 2)
				RegularSeasonLeaderboardMenu(season_data);
			else
				return POSTSEASON_QUIT;
		}
	}
}

static void ShowPlayoffRoundResults(
	int round,
	const struct playoff_game afc_rounds[3][PLAYOFF_GAMES_PER_CONFERENCE],
	const struct playoff_game nfc_rounds[3][PLAYOFF_GAMES_PER_CONFERENCE],
	const struct playoff_game* championship)
{
	ShowPlayoffRound(round, afc_rounds, nfc_rounds, championship);
	getch();
}


/*
* NAME:        PlayPostSeason
* DESCRIPTION: play out post season games
* INPUTS:      season data
* OUTPUTS:     none
*/
void PlayPostSeason(struct season_data* season_data)
{
	struct playoff_team afc_seeds[PLAYOFF_TEAMS_PER_CONFERENCE];
	struct playoff_team nfc_seeds[PLAYOFF_TEAMS_PER_CONFERENCE];
	struct playoff_game afc_rounds[3][PLAYOFF_GAMES_PER_CONFERENCE] = { 0 };
	struct playoff_game nfc_rounds[3][PLAYOFF_GAMES_PER_CONFERENCE] = { 0 };
	struct playoff_game championship = { 0 };
	const int games_per_round[3] = { 4, 2, 1 };

	if (BuildConferenceSeeds(season_data, true, afc_seeds) != PLAYOFF_TEAMS_PER_CONFERENCE ||
		BuildConferenceSeeds(season_data, false, nfc_seeds) != PLAYOFF_TEAMS_PER_CONFERENCE)
	{
		printf("Unable to create the playoff bracket.\r\n");
		return;
	}

	BuildOpeningRound(afc_seeds, afc_rounds[0]);
	BuildOpeningRound(nfc_seeds, nfc_rounds[0]);
	season_data->playoffs = true;

	for (int round = 0; round < 3; round++)
	{
		int game_count = games_per_round[round];
		int action = GetPostseasonAction(
			season_data, round, afc_rounds, nfc_rounds, &championship);

		if (action == POSTSEASON_QUIT)
		{
			season_data->playoffs = false;
			return;
		}

		for (int i = 0; i < game_count; i++)
		{
			bool user_plays = action == POSTSEASON_PLAY_ROUND &&
				(afc_rounds[round][i].away.team_id == season_data->team_1_id ||
				 afc_rounds[round][i].home.team_id == season_data->team_1_id);
			ResolvePlayoffGame(season_data, &afc_rounds[round][i], user_plays);

			user_plays = action == POSTSEASON_PLAY_ROUND &&
				(nfc_rounds[round][i].away.team_id == season_data->team_1_id ||
				 nfc_rounds[round][i].home.team_id == season_data->team_1_id);
			ResolvePlayoffGame(season_data, &nfc_rounds[round][i], user_plays);
		}

		ShowPlayoffRoundResults(
			round, afc_rounds, nfc_rounds, &championship);
		if (round < 2)
		{
			BuildNextRound(afc_rounds[round], game_count, afc_rounds[round + 1]);
			BuildNextRound(nfc_rounds[round], game_count, nfc_rounds[round + 1]);
		}
		else
		{
			championship = CreatePlayoffGame(
				GetPlayoffWinner(&afc_rounds[round][0]),
				GetPlayoffWinner(&nfc_rounds[round][0]));
		}
	}

	int action = GetPostseasonAction(
		season_data, 3, afc_rounds, nfc_rounds, &championship);
	if (action != POSTSEASON_QUIT)
	{
		bool user_plays = action == POSTSEASON_PLAY_ROUND &&
			(championship.away.team_id == season_data->team_1_id ||
			 championship.home.team_id == season_data->team_1_id);
		ResolvePlayoffGame(season_data, &championship, user_plays);
		ShowPlayoffRoundResults(3, afc_rounds, nfc_rounds, &championship);
		printf("\r\n%s are the champions!\r\n",
			default_league.teams[GetPlayoffWinner(&championship).team_id].name);
		getch();
	}

	season_data->playoffs = false;
}