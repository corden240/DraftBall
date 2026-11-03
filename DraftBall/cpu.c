#include <stdio.h>
#include <stdlib.h>   /* srand, rand, atoi */
#include "main.h"

bool PcChance(int chance);
int GetDirectionOfPlay(struct game_data* game_data);
int GetFieldGoalDistance(struct game_data* game_data);


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

	if (punt_or_receive == 1)
	{
		char buf[256];
		snprintf(buf, sizeof(buf), "%s won the toss and chose to punt ",
			(team_that_won_cointoss == 1) ? game_data->team_1_name : game_data->team_2_name);
		ConsoleShowGameScreen(game_data, buf, true, GAME_SPEED * 10);
	
		game_data->team_started_with_possession = team_that_won_cointoss;
	}
	else if (punt_or_receive == 2)
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

/*
* NAME:        GetYardsToEndZone
* DESCRIPTION: get distance
* INPUTS:      game_data
* OUTPUTS:     yards
*/
int GetYardsToEndZone(struct game_data* game_data)
{
	if (GetDirectionOfPlay(game_data) == 1)
	{
		return (FIELD_WIDTH - (game_data->line_of_scrimmage - END_ZONE_WIDTH));
	}
	else
	{
		return (game_data->line_of_scrimmage - END_ZONE_WIDTH);
	}
}

/*
* NAME:        GetYardsToFirstDown
* DESCRIPTION: get distance
* INPUTS:      game_data
* OUTPUTS:     yards
*/
int GetYardsToFirstDown(struct game_data* game_data)
{
	return abs(game_data->line_of_scrimmage - game_data->line_of_first_down);
}

/*
* NAME:        CpuPuntCardsToDrawDecision
* DESCRIPTION: cpu chooses decision
* INPUTS:      game_data
* OUTPUTS:     int
*/
int CpuPuntCardsToDrawDecision(struct game_data* game_data)
{
	int yards_to_endzone = GetYardsToEndZone(game_data);
	
	if (yards_to_endzone > 40) // if > 40 yards to end zone draw 6
		return PUNT_MAX_CARDS;
	else if (yards_to_endzone > 33)
		return PUNT_MAX_CARDS - 1;
	else if (yards_to_endzone > 26)
		return PUNT_MAX_CARDS - 2;
	else if (yards_to_endzone > 19)
		return PUNT_MAX_CARDS - 3;
	else if (yards_to_endzone > 12)
		return PUNT_MAX_CARDS - 4;
	else
		return 1;
}

/*
* NAME:        CpuPuntCardsToRedrawDecision
* DESCRIPTION: cpu chooses decision
* INPUTS:      game_data, offence yards
* OUTPUTS:     int
*/
int CpuPuntCardsToRedrawDecision(struct game_data* game_data, int total_yards)
{
	int yards_to_endzone = GetYardsToEndZone(game_data);

	if((total_yards - yards_to_endzone) > 8)
		return PUNT_REDRAW_MAX_CARDS;
	else if ((total_yards - yards_to_endzone) > -1)
		return 1;

	if ((yards_to_endzone - total_yards) < 10)
		return 0;
	else if ((yards_to_endzone - total_yards) < 15)
		return 1;
	else
		return PUNT_REDRAW_MAX_CARDS;
}

/*
* NAME:        CpuFieldGoalCardsToRedrawDecision
* DESCRIPTION: cpu chooses decision
* INPUTS:      game_data, offence yards
* OUTPUTS:     int
*/
int CpuFieldGoalCardsToRedrawDecision(struct game_data* game_data, int total_yards)
{
	int yards_to_fg = GetFieldGoalDistance(game_data);

	if ((yards_to_fg - total_yards) > 0)
		return FIELD_GOAL_REDRAW_MAX_CARDS;
	else
		return 0;
}

/*
* NAME:        CpuCardsToRedrawHiLoDecision
* DESCRIPTION: cpu chooses decision
* INPUTS:      game_data, offence yards, num redrawn
* OUTPUTS:     int
*/
int CpuCardsToRedrawHiLoDecision(struct game_data* game_data, int total_yards, int num) // called in onside kick, XP, FG, punt
{
	if (game_data->play_type == PLAY_ONSIDE_KICK)
	{
		if (total_yards > ONSIDE_KICK_TARGET_TOTAL)
			return 2;
		else
			return 1;
	}
	else if (game_data->play_type == PLAY_TYPE_PUNT)
	{
		int yards_to_endzone = GetYardsToEndZone(game_data);
		if ((total_yards - yards_to_endzone) > -1)
			return 2;
		else
			return 1;
	}
	else if (game_data->play_type == PLAY_TYPE_FIELD_GOAL)
	{
		return 1;
	}
	return 1;
}

/*
* NAME:        CpuOnsideKickCardsToRedrawDecision
* DESCRIPTION: cpu chooses decision
* INPUTS:      game_data, offence yards
* OUTPUTS:     int
*/
int CpuOnsideKickCardsToRedrawDecision(struct game_data* game_data, int total)
{
	if (total == ONSIDE_KICK_TARGET_TOTAL)
		return 0;
	
	if (total > (ONSIDE_KICK_TARGET_TOTAL + 6))
		return ONSIDE_KICK_REDRAW_MAX_CARDS;
	
	if (total > (ONSIDE_KICK_TARGET_TOTAL))
		return 1;

	if (total < (ONSIDE_KICK_TARGET_TOTAL - 6))
		return ONSIDE_KICK_REDRAW_MAX_CARDS;

	if (total < (ONSIDE_KICK_TARGET_TOTAL))
		return 1;
}

/*
* NAME:        CpuTimeoutDecision
* DESCRIPTION: cpu chooses decision
* INPUTS:      game_data
* OUTPUTS:     bool
*/
bool CpuTimeoutDecision(struct game_data* game_data)
{
	if (!game_data->one_player)
		return false;

	if (game_data->team_2_timeouts_remaining == 0)
		return false;

	if (((game_data->team_1_score - game_data->team_2_score) > 0) && // behind in score
		((game_data->quarter == 2) || (game_data->quarter == 4)) &&  // 2nd or 4th quarter
		(game_data->plays_remaining_in_quarter < 3))				 // 2 or less plays left
		return true;

	if (((game_data->team_2_score - game_data->team_1_score) > 0) && // ahead in score
		((game_data->quarter == 2)) && 								 // 2nd quarter
		(game_data->plays_remaining_in_quarter < 3) && 				 // 2 or less plays left
		game_data->team_in_possession == 2)							 // we have the ball
		return true;

	return false;
}

/*
* NAME:        CpuKneelDecision
* DESCRIPTION: cpu chooses decision
* INPUTS:      game_data
* OUTPUTS:     bool
*/
bool CpuKneelDecision(struct game_data* game_data)
{
	if (game_data->team_2_score > game_data->team_1_score) // if winning
	{
		if ((game_data->quarter == 2) || (game_data->quarter == 4)) // if in quarter 2 or 4
		{
			if (game_data->team_in_possession == 2) // cpu has possession
			{
				if ((game_data->plays_remaining_in_quarter + game_data->team_1_timeouts_remaining) <= (DOWNS_PER_FIRST_DOWN - game_data->down + 1)) // can kneel away all plays left
				{
					return true;
				}
			}
		}
	}
	return false;
}

/*
* NAME:        CpuGoFourthDown
* DESCRIPTION: cpu chooses decision
* INPUTS:      game_data
* OUTPUTS:     bool
*/
bool CpuGoFourthDown(struct game_data* game_data)
{
	if (GetYardsToFirstDown(game_data) <= 2 && GetYardsToEndZone(game_data) < 70)
		return true;
}

/*
* NAME:        CpuPuntDecision
* DESCRIPTION: cpu chooses decision
* INPUTS:      game_data
* OUTPUTS:     bool
*/
bool CpuPuntDecision(struct game_data* game_data)
{
	if (game_data->team_in_possession == 2)
	{
		if (game_data->down == 4)
		{
			if (!CpuGoFourthDown(game_data))
			{
				return true;
			}
		}
	}
	return false;
}

/*
* NAME:        CpuFGDecision
* DESCRIPTION: cpu chooses decision
* INPUTS:      game_data
* OUTPUTS:     bool
*/
bool CpuFGDecision(struct game_data* game_data)
{
	if (game_data->team_in_possession == 2)
	{
		if (game_data->down == 4)
		{
			if (GetYardsToEndZone(game_data) < 35)
			{
				if (!CpuGoFourthDown(game_data))
				{
					return true;
				}
			}
		}
	}
}

/*
* NAME:        CpuPlayTypeDecision
* DESCRIPTION: cpu chooses decision
* INPUTS:      game_data
* OUTPUTS:     int
*/
int CpuPlayTypeDecision(struct game_data* game_data)
{
	ConsoleShowGameScreen(game_data, "Would you like to timeout? y/n: ", true, GAME_SPEED * 2);
	char input[16];
	fgets(input, sizeof(input), stdin);

	if (input[0] == 'Y' || input[0] == 'y')
	{
		return PLAY_TYPE_TIMEOUT;
	}

	if (CpuKneelDecision(game_data))
		return PLAY_TYPE_KNEEL;
	else if (CpuFGDecision(game_data))
		return PLAY_TYPE_FIELD_GOAL;
	else if (CpuPuntDecision(game_data))
		return PLAY_TYPE_PUNT;
	else 
		return PLAY_TYPE_NORMAL;
}