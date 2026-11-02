#include <stdio.h>
#include <stdlib.h>   /* srand, rand, atoi */
#include "main.h"

bool PcChance(int chance);




/*
* NAME:        CpuCoinTossDecision
* DESCRIPTION: cpu chooses decision
* INPUTS:      game_data, team_that_won_cointoss
* OUTPUTS:     none
*/
void CpuCoinTossDecision(struct game_data* game_data, int team_that_won_cointoss)
{
	int punt_or_receive = 0;
	
	if (PcChance(500)) // 50/50 to pick punt or receive
		punt_or_receive = 1;
	else
		punt_or_receive = 2;

	if (punt_or_receive == 0)
	{
		char buf[256];
		snprintf(buf, sizeof(buf), "%s won the toss and chose to punt ",
			(team_that_won_cointoss == 1) ? game_data->team_1_name : game_data->team_2_name);
		ConsoleShowGameScreen(game_data, buf, true, GAME_SPEED * 10);
	
		game_data->team_started_with_possession = team_that_won_cointoss;
	}
	else if (punt_or_receive == 1)
	{
		char buf[256];
		snprintf(buf, sizeof(buf), "%s won the toss and chose to receive ",
			(team_that_won_cointoss == 1) ? game_data->team_1_name : game_data->team_2_name);
		ConsoleShowGameScreen(game_data, buf, true, GAME_SPEED * 10);
	
		game_data->team_started_with_possession = team_that_won_cointoss - 1 ? team_that_won_cointoss - 1 : team_that_won_cointoss + 1;	
	}

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

/*
* NAME:        CpuPuntOrOnsideKickDecision
* DESCRIPTION: cpu chooses decision
* INPUTS:      game_data
* OUTPUTS:     int
*/
int CpuPuntOrOnsideKickDecision(struct game_data* game_data)
{
	int kickoff_or_onside_kick = 1;

	if (game_data->quarter == 4 &&
		(game_data->plays_remaining_in_quarter + game_data->team_2_timeouts_remaining) < 7 &&
		((game_data->team_2_score < game_data->team_1_score) && (game_data->team_2_score > (game_data->team_1_score - 9))) &&
		PcChance(900))
		kickoff_or_onside_kick = 2;

	if (kickoff_or_onside_kick == 1)
	{
		char buf[256];
		snprintf(buf, sizeof(buf), "%s kickoff ",
			(game_data->team_in_possession == 1) ? game_data->team_1_name : game_data->team_2_name);
		ConsoleShowGameScreen(game_data, buf, true, GAME_SPEED * 5);
	}
	else if (kickoff_or_onside_kick == 2)
	{
		char buf[256];
		snprintf(buf, sizeof(buf), "%s choose to onside kick ",
			(game_data->team_in_possession == 1) ? game_data->team_1_name : game_data->team_2_name);
		ConsoleShowGameScreen(game_data, buf, true, GAME_SPEED * 5);
	}

	return kickoff_or_onside_kick;
}