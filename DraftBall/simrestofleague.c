#include <stdio.h>
#include <stdlib.h>   /* srand, rand, atoi */
#include <time.h>     /* time() */
#include "main.h"
#include <string.h>
#include <windows.h>
#include "teamdata.h"
#include "main_d.prt"


/*
* NAME:        SetupTeamDataSeasonGameOtherTeam
* DESCRIPTION: builds team names colours player names etc from season data
* INPUTS:      season_data, game_data
* OUTPUTS:     void
*/
void SetupTeamDataSeasonGameOtherTeam(struct season_data* season_data, struct game_data* game_data, int team_1_id, int team_2_id)
{
	game_data->team_1_id = team_1_id;
	game_data->team_2_id = team_2_id;
	game_data->team_1_score = 0;
	game_data->team_2_score = 0;
	game_data->quarter = 1;


	const struct team_data* team_1;
	const struct team_data* team_2;

	team_1 = &default_league.teams[game_data->team_1_id];
	team_2 = &default_league.teams[game_data->team_2_id];

	snprintf(game_data->team_1_name, sizeof game_data->team_1_name,
		"%s", team_1->name);
	snprintf(game_data->team_2_name, sizeof game_data->team_2_name,
		"%s", team_2->name);
}

/*
* NAME:        SimGame
* DESCRIPTION: sim the next regular season game
* INPUTS:      game data
* OUTPUTS:     none
*/
void SimGameOtherTeam(struct game_data* game_data)
{
	int team1rating = GetTeamRating(game_data->team_1_id);
	int team2rating = GetTeamRating(game_data->team_2_id);
	int team_with_ball = 1;

	for (game_data->quarter; game_data->quarter <= NUMBER_OF_QUARTERS; game_data->quarter++)
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
			else if (PcChance(SIM_CHANCE_TO_CHANGE_POSSESSION))
			{
				if (team_with_ball == 1)
					team_with_ball = 2;
				else
					team_with_ball = 1;
			}
		}
	}
}

/*
* NAME:        ExportGameDataOtherTeam
* DESCRIPTION: export game data to season data
* INPUTS:      season data, game data
* OUTPUTS:     void
*/
void ExportGameDataOtherTeam(struct season_data* season_data, struct game_data* game_data)
{
	if (game_data->team_1_score > game_data->team_2_score)
	{
		season_data->team_records[game_data->team_1_id][WINS]++;
		season_data->team_records[game_data->team_2_id][LOSSES]++;
	}
	else if (game_data->team_2_score > game_data->team_1_score)
	{
		season_data->team_records[game_data->team_2_id][WINS]++;
		season_data->team_records[game_data->team_1_id][LOSSES]++;
	}
	else
	{
		season_data->team_records[game_data->team_1_id][DRAWS]++;
		season_data->team_records[game_data->team_2_id][DRAWS]++;
	}
}

enum matchup_result {
	MATCHUP_ERROR = -1,
	MATCHUP_READY = 0,
	MATCHUP_CONTROLLED_BY_USER = 1
};

/*
* NAME:        GetDivisionTeamIds
* DESCRIPTION: gets division ids
* INPUTS:      season data, division index
* OUTPUTS:     int
*/
static const int* GetDivisionTeamIds(
	const struct season_data* season_data,
	int division_index)
{
	switch (division_index)
	{
	case 0: return season_data->afc_east_team_ids;
	case 1: return season_data->afc_north_team_ids;
	case 2: return season_data->afc_south_team_ids;
	case 3: return season_data->afc_west_team_ids;
	case 4: return season_data->nfc_east_team_ids;
	case 5: return season_data->nfc_north_team_ids;
	case 6: return season_data->nfc_south_team_ids;
	case 7: return season_data->nfc_west_team_ids;
	default: return NULL;
	}
}

/*
* NAME:        GetLeagueMatchupTeamIds
* DESCRIPTION: Returns away team in team_1_id and home team in team_2_id
* INPUTS:      season data, matchup index, team ids
* OUTPUTS:     int
*/
static int GetLeagueMatchupTeamIds(
	const struct season_data* season_data,
	int matchup_index,
	int* team_1_id,
	int* team_2_id)
{
	if (!season_data || !team_1_id || !team_2_id ||
		season_data->week < 1 || season_data->week > SEASON_NUMBER_OF_GAMES ||
		matchup_index < 0 || matchup_index >= LEAGUE_TEAM_COUNT / 2)
	{
		return MATCHUP_ERROR;
	}

	int division_index = matchup_index / 2;
	int game_index = matchup_index % 2;
	int schedule_column = game_index * 2;
	const int* division_teams = GetDivisionTeamIds(season_data, division_index);

	if (!division_teams)
		return MATCHUP_ERROR;

	int away_slot = schedule[season_data->week - 1][schedule_column] - 1;
	int home_slot = schedule[season_data->week - 1][schedule_column + 1] - 1;

	if (away_slot < 0 || away_slot >= DIVISION_TEAM_COUNT ||
		home_slot < 0 || home_slot >= DIVISION_TEAM_COUNT)
	{
		return MATCHUP_ERROR;
	}

	*team_1_id = division_teams[away_slot];
	*team_2_id = division_teams[home_slot];

	if (*team_1_id == season_data->team_1_id ||
		*team_2_id == season_data->team_1_id)
	{
		return MATCHUP_CONTROLLED_BY_USER;
	}

	return MATCHUP_READY;
}

/*
* NAME:        SimRestOfLeagueGames
* DESCRIPTION: sim the weeks regular season games
* INPUTS:      season data
* OUTPUTS:     void
*/
void SimRestOfLeagueGames(struct season_data* season_data)
{
	for (int i = 0; i < LEAGUE_TEAM_COUNT / 2; i++)
	{
		int team_1_id;
		int team_2_id;
		int result = GetLeagueMatchupTeamIds(
			season_data, i, &team_1_id, &team_2_id);

		if (result == MATCHUP_CONTROLLED_BY_USER)
			continue;
		if (result == MATCHUP_ERROR)
		{
			fprintf(stderr, "Could not resolve matchup %d for week %d.\n",
				i, season_data->week);
			return;
		}

		struct game_data game_data;
		SetupTeamDataSeasonGameOtherTeam(
			season_data, &game_data, team_1_id, team_2_id);
		SimGameOtherTeam(&game_data);
		ExportGameDataOtherTeam(
			season_data, &game_data);
	}
}