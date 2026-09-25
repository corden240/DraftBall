#include <math.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>   /* srand, rand, atoi */
#include <time.h>     /* time() */
#include "main.h"

void ResetLineOfFirstDown(struct response* res);
void SwitchPossession(struct response* res);
void ResetLineOfScrimmage(struct response* res);
int TableWeightTotalWeight(struct table_weight* table);
void TableWeightSet(struct table_weight* table, int val, bool on_off);
void MoveLineOfScrimmage(struct response* res, int yards);

/*
* NAME:        ClearAllCards
* DESCRIPTION: clear o_cards and d_cards
* INPUTS:      response
* OUTPUTS:     None
*/
void ClearAllCards(struct response* res)
{
	for (int i = 0; i < CARDS_IN_A_DECK; i++)
	{
		res->o_cards[i] = 0;
		res->d_cards[i] = 0;
	}
}

/*
* NAME:        DrawOneCard
* DESCRIPTION: pick an available card from the deck
* INPUTS:      response
* OUTPUTS:     card
*/
int DrawOneCard(struct response* res, struct table_weight* deck)
{
	//return (KING | CLUBS); // force
	if (TableWeightTotalWeight(deck) < 0)
	{
		TableWeightEnableAll(deck);
		ConsoleShowScreen(res, "Deck empty, reshuffling...", GAME_SPEED * 20);
	}

	int card = TableWeightDecision(deck, false);

	TableWeightSet(deck, card, false);

	return card;
}

/*
* NAME:        DrawCards
* DESCRIPTION: draw and assign x cards
* INPUTS:      response
* OUTPUTS:     None
*/
void DrawCards(struct response* res, int team_to_draw, int num)
{
	/* pick which hand to fill */
	int* hand = (team_to_draw == res->team_in_possession)
		? res->o_cards
		: res->d_cards;

	/* skip over cards already in the hand; an empty slot has VALUE == 0 */
	int start = 0;
	while (start < CARDS_IN_A_DECK && (hand[start] & VALUE) != 0)
		start++;

	/* append num cards at the end, without overflowing the array */
	for (int i = 0; i < num && (start + i) < CARDS_IN_A_DECK; i++)
	{
		hand[start + i] = DrawOneCard(res, shared_deck);

		ConsoleShowScreen(res, 0, GAME_SPEED * 8); // show cards drawn
	}
}

/*
* NAME:        RedrawCards
* DESCRIPTION: draw and assign x cards
* INPUTS:      response
* OUTPUTS:     None
*/
void RedrawCards(struct response* res, int team_to_draw, int num)
{
	if (!num)
		return;

	char buf[256];
	snprintf(buf, sizeof(buf), "Replace the low or high cards (1 - 2): ");
	ConsoleShowScreen(res, buf, GAME_SPEED * 2);

	int hilo = 0;
	char input[16];
	if (fgets(input, sizeof(input), stdin)) {
		hilo = atoi(input);
	}

	if (hilo == 1 || hilo == 2)
	{
		/* find how many real cards are actually in the hand;
		   an empty slot has VALUE == 0, which no real card can have */
		int hand_size = 0;
		while (hand_size < CARDS_IN_A_DECK && (res->o_cards[hand_size] & VALUE) != 0)
			hand_size++;

		if (hand_size == 0)
			return;

		/* build an index array over just the real cards in the HAND */
		int indices[CARDS_IN_A_DECK];
		for (int i = 0; i < hand_size; i++)
			indices[i] = i;

		/* sort indices ascending by card rank (mask off suit/flag bits) */
		for (int i = 0; i < hand_size - 1; i++)
		{
			for (int j = 0; j < hand_size - 1 - i; j++)
			{
				int rank_a = res->o_cards[indices[j]] & VALUE;
				int rank_b = res->o_cards[indices[j + 1]] & VALUE;
				if (rank_a > rank_b)
				{
					int tmp = indices[j];
					indices[j] = indices[j + 1];
					indices[j + 1] = tmp;
				}
			}
		}

		int num_to_replace = (num > hand_size) ? hand_size : num;

		if (hilo == 1)
		{
			// lowest "num" cards -> front of sorted indices
			for (int k = 0; k < num_to_replace; k++)
			{
				int idx = indices[k];
				res->o_cards[idx] = DrawOneCard(res, shared_deck);
				ConsoleShowScreen(res, 0, GAME_SPEED * 8); // show cards drawn
			}
		}
		else /* hilo == 2 */
		{
			// highest "num" cards -> back of sorted indices
			for (int k = 0; k < num_to_replace; k++)
			{
				int idx = indices[hand_size - 1 - k];
				res->o_cards[idx] = DrawOneCard(res, shared_deck);
				ConsoleShowScreen(res, 0, GAME_SPEED * 8); // show cards drawn
			}
		}
	}
}

/*
* NAME:        TotalYardsOffence
* DESCRIPTION: add up total yards gained
* INPUTS:      response
* OUTPUTS:     int
*/
int TotalYardsOffence(struct response* res)
{
	int sum_value = 0;
	for (int i = 0; i < CARDS_IN_A_DECK; i++)
	{
		sum_value += (res->o_cards[i] & VALUE);
	}
	return sum_value;
}

/*
* NAME:        TotalYardsDefence
* DESCRIPTION: add up total yards gained
* INPUTS:      response
* OUTPUTS:     int
*/
int TotalYardsDefence(struct response* res)
{
	int sum_value = 0;
	for (int i = 0; i < CARDS_IN_A_DECK; i++)
	{
		sum_value += (res->d_cards[i] & VALUE);
	}
	return sum_value;
}

/*
* NAME:        Interception
* DESCRIPTION: execute the play
* INPUTS:      response
* OUTPUTS:     None
*/
void Interception(struct response* res)
{

}

/*
* NAME:        Fumble
* DESCRIPTION: execute the play
* INPUTS:      response
* OUTPUTS:     None
*/
void Fumble(struct response* res)
{

}

/*
* NAME:        OpenFieldRun
* DESCRIPTION: execute the play
* INPUTS:      response
* OUTPUTS:     None
*/
void OpenFieldRun(struct response* res, int offence_1, int defence_1)
{
	ConsoleShowScreen(res, "Open field run!", GAME_SPEED * 20);

	int ofr_o_value = (offence_1 & VALUE) - 10;

	if (ofr_o_value < 0)
		ofr_o_value = 0;

	int ofr_d_value = (defence_1 & VALUE) - 10;

	if (ofr_d_value < 0)
		ofr_d_value = 0;

	int cards_to_draw = (ofr_o_value - ofr_d_value);

	if (cards_to_draw < 0 || cards_to_draw > 3)
		printf("Error in OFR cards to draw");

	ClearAllCards(res);

	char buf[256];
	snprintf(buf, sizeof(buf), "Drawing %d cards...", cards_to_draw);
	ConsoleShowScreen(res, buf, GAME_SPEED * 20);

	int yards = 0;

	for (int i = 0; i < cards_to_draw; i++)
	{
		DrawCards(res, res->team_in_possession, 1);
		if(res->o_cards[i] & FACE)
		{
			ofr_o_value = (res->o_cards[i] & VALUE) - 10;

			if (ofr_o_value < 0)
				ofr_o_value = 0;
			cards_to_draw += ofr_o_value;
			char buf[256];
			snprintf(buf, sizeof(buf), "Face Card! %d extra cards", ofr_o_value);
			ConsoleShowScreen(res, buf, GAME_SPEED * 20);
		}
		yards = TotalYardsOffence(res);
	}

	yards = TotalYardsOffence(res);
	MoveLineOfScrimmage(res, yards);

	char buf2[256];
	snprintf(buf2, sizeof(buf2), "OFR yards = %d  ", yards);
	ConsoleShowScreen(res, buf2, GAME_SPEED * 30);

	return;
}

/*
* NAME:        PlayPass
* DESCRIPTION: execute the play
* INPUTS:      response
* OUTPUTS:     None
*/
void PlayPass(struct response* res, int offence_1, int defence_1)
{

}

/*
* NAME:        PlayRun
* DESCRIPTION: execute the play
* INPUTS:      response
* OUTPUTS:     None
*/
void PlayRun(struct response* res, int offence_1, int defence_1)
{

}

/*
* NAME:        PlayNormal
* DESCRIPTION: execute the play
* INPUTS:      response
* OUTPUTS:     None
*/
void PlayNormal(struct response* res)
{
	/* start with forcing a 4 yard gain assigned to a random RB or WR, get working like that first, 
	messaging, moving lines, metering, gaining a new first down, scoring, end of quarter, end of game */

	/* Then get working as 1 v 1 card O1 v D1 pure maths O1 - D1 calculation */
	
	int yards = rand() % 10 - 2;

	int total_yards = TotalYardsOffence(res);
	char buf[256];
	snprintf(buf, sizeof(buf), "Yards Gained: %d", yards);
	ConsoleShowScreen(res, buf, GAME_SPEED * 10);

	MoveLineOfScrimmage(res, yards);

	ConsoleShowScreen(res, 0, GAME_SPEED * 10);

	// What the function will do when actually working:
	// ask player on offence to pick layout
	// O: pick 4 cards then place into desired player positions within chosen layout
	// D: pick 4 cards then place into desired player positions
	// Reveal all 8 cards 1 by 1
	// O: pick which of the 4 match ups to play

	// PlayRun(res, O1, D1);
	// PlayPass(res, O1, D1);
}

/*
* NAME:        GetPuntCardsToRedraw
* DESCRIPTION: ask and return how many cards to draw in the punt redraw
* INPUTS:      response
* OUTPUTS:     int
*/
int GetPuntCardsToRedraw(struct response* res) {

	char buf[256];
	snprintf(buf, sizeof(buf), "Current total Yards: %d  How many to redraw? (0 - %d): ", TotalYardsOffence(res), PUNT_REDRAW_MAX_CARDS);
	ConsoleShowScreen(res, buf, GAME_SPEED * 2);

	int num = 0;
	char input[16];
	if (fgets(input, sizeof(input), stdin)) {
		num = atoi(input);
	}

	if (num > PUNT_REDRAW_MAX_CARDS)
		num = PUNT_REDRAW_MAX_CARDS;
	else if (num < 0)
		num = 0;

	return num;
}

/*
* NAME:        GetPuntCardsToDraw
* DESCRIPTION: ask and return how many cards to draw in the punt
* INPUTS:      response
* OUTPUTS:     int
*/
int GetPuntCardsToDraw(struct response* res) {
	char buf[256];
	snprintf(buf, sizeof(buf), "How many cards do you want to draw? (1 - %d): ", PUNT_MAX_CARDS);
	ConsoleShowScreen(res, buf, GAME_SPEED * 2);

	int num = 0;
	char input[16];
	if (fgets(input, sizeof(input), stdin)) {
		num = atoi(input);
	}

	if (num > PUNT_MAX_CARDS)
		num = PUNT_MAX_CARDS;
	else if (num < 0)
		num = 0;

	return num;
}

/*
* NAME:        GetFieldGoalCardsToRedraw
* DESCRIPTION: ask and return how many cards to draw in the field goal redraw
* INPUTS:      response
* OUTPUTS:     int
*/
int GetFieldGoalCardsToRedraw(struct response* res) {

	char buf[256];
	snprintf(buf, sizeof(buf), "Current total Yards: %d  How many to redraw? (0 - %d): ", TotalYardsOffence(res), FIELD_GOAL_REDRAW_MAX_CARDS);
	ConsoleShowScreen(res, buf, GAME_SPEED * 2);

	int num = 0;
	char input[16];
	if (fgets(input, sizeof(input), stdin)) {
		num = atoi(input);
	}

	if (num > FIELD_GOAL_REDRAW_MAX_CARDS)
		num = FIELD_GOAL_REDRAW_MAX_CARDS;
	else if (num < 0)
		num = 0;

	return num;
}

/*
* NAME:        GetFieldGoalCardsToDraw
* DESCRIPTION: ask and return how many cards to draw in the field goal
* INPUTS:      response
* OUTPUTS:     int
*/
int GetFieldGoalCardsToDraw(struct response* res) {
	char buf[256];
	snprintf(buf, sizeof(buf), "How many cards do you want to draw? (1 - %d): ", FIELD_GOAL_MAX_CARDS);
	ConsoleShowScreen(res, buf, GAME_SPEED * 2);

	int num = 0;
	char input[16];
	if (fgets(input, sizeof(input), stdin)) {
		num = atoi(input);
	}

	if (num > FIELD_GOAL_MAX_CARDS)
		num = FIELD_GOAL_MAX_CARDS;
	else if (num < 0)
		num = 0;

	return num;
}

/*
* NAME:        TouchBack
* DESCRIPTION: ask for touchback
* INPUTS:      response
* OUTPUTS:     bool
*/
bool TouchBack(struct response* res)
{
	ConsoleShowScreen(res, "Would you like to action a touchback? y/n ", 0);
	char input[16];
	fgets(input, sizeof(input), stdin);

	if (input[0] == 'N' || input[0] == 'n')
		return false;

	return true;
}

/*
* NAME:        PlayPuntReturn
* DESCRIPTION: execute the play
* INPUTS:      response
* OUTPUTS:     None
*/
void PlayPuntReturn(struct response* res)
{
	ClearAllCards(res);

	// if the kick is received in the endzone, the receiver can declare a touchback
	if (res->line_of_scrimmage <= END_ZONE_WIDTH || res->line_of_scrimmage >= (END_ZONE_WIDTH + FIELD_WIDTH))
	{
		if (TouchBack(res))
		{
			ResetLineOfScrimmage(res);
			ResetLineOfFirstDown(res);
			return;
		}
	}

	ConsoleShowScreen(res, "Punt return...", GAME_SPEED * 20);

	// Receiver draws two cards and the punter draws one card from the shared deck
	DrawCards(res, res->team_in_possession, 2);
	DrawCards(res, res->team_not_in_possession, 1);
	
	// (two received draws - one punter draw) = (O1 + O2) - D1 = yardage returned
	int yards = TotalYardsOffence(res) - TotalYardsDefence(res);
	if (yards < 0)
		yards = 0;

	char buf[256];
	snprintf(buf, sizeof(buf), "Return yards = %d  ", yards);
	ConsoleShowScreen(res, buf, GAME_SPEED * 30);

	MoveLineOfScrimmage(res, yards); // move line of scrimmage 

	ConsoleShowScreen(res, 0, GAME_SPEED * 20);

	// if O2 is a face card and larger than D1 the go to Open field run
	if ((res->o_cards[1] & FACE) && ((res->o_cards[1] & VALUE) > (res->d_cards[0] & VALUE)))
	{
		OpenFieldRun(res, res->o_cards[1], res->d_cards[0]);
	}

	res->down = 0;

	ResetLineOfFirstDown(res);
}

/*
* NAME:        ConvertLosToYardLine
* DESCRIPTION: los to yard line, intake LoS (0 - 110), output 0 - 50 yard line on pitch
* INPUTS:      response
* OUTPUTS:     int
*/
int ConvertLosToYardLine(struct response* res)
{
	if (res->line_of_scrimmage > ((FIELD_WIDTH / 2) + END_ZONE_WIDTH))
	{
		return (FIELD_WIDTH - (res->line_of_scrimmage - END_ZONE_WIDTH));
	}
	else
	{
		return (res->line_of_scrimmage - END_ZONE_WIDTH);
	}
}

/*
* NAME:        PlayPunt
* DESCRIPTION: execute the play
* INPUTS:      response
* OUTPUTS:     None
*/
void PlayPunt(struct response* res)
{
	ConsoleShowScreen(res, "Punting...", GAME_SPEED * 20); // announce punt

	DrawCards(res, res->team_in_possession, GetPuntCardsToDraw(res));// draw 1-5 cards

	RedrawCards(res, res->team_in_possession, GetPuntCardsToRedraw(res));// redraw 0-2 cards, get player choice on high or low ones

	int total_yards = TotalYardsOffence(res);
	char buf[256];
	snprintf(buf, sizeof(buf), "Total Yards: %d", total_yards);
	ConsoleShowScreen(res, buf, GAME_SPEED * 10);

	MoveLineOfScrimmage(res, total_yards);

	char buf2[256];
	snprintf(buf2, sizeof(buf2), "Ball caught at the %d", ConvertLosToYardLine(res));
	ConsoleShowScreen(res, buf2, GAME_SPEED * 20);

	SwitchPossession(res);

	ConsoleShowScreen(res, 0, GAME_SPEED * 10);

	PlayPuntReturn(res);
}

/*
* NAME:        SetLosExtraPoint
* DESCRIPTION: get field distance to fg
* INPUTS:      response
* OUTPUTS:     distance
*/
int SetLosExtraPoint(struct response* res)
{
	int distance;

	if ((res->team_in_possession == 1 && res->team_1_direction == 1) || (res->team_in_possession == 2 && res->team_2_direction == 1))
	{
		res->line_of_scrimmage = END_ZONE_WIDTH + FIELD_WIDTH - EXTRA_POINT_YARDAGE_FROM_END_ZONE;
	}
	else
	{
		res->line_of_scrimmage = END_ZONE_WIDTH + EXTRA_POINT_YARDAGE_FROM_END_ZONE;
	}
}

/*
* NAME:        GetFieldGoalDistance
* DESCRIPTION: get field distance to fg
* INPUTS:      response
* OUTPUTS:     distance
*/
int GetFieldGoalDistance(struct response* res)
{
	int distance;

	if ((res->team_in_possession == 1 && res->team_1_direction == 1) || (res->team_in_possession == 2 && res->team_2_direction == 1))
	{
		distance = (FIELD_WIDTH - (res->line_of_scrimmage - END_ZONE_WIDTH)) + FIELD_GOAL_YARDS_AFTER_END_ZONE;
	}
	else
	{
		distance = (res->line_of_scrimmage - END_ZONE_WIDTH) + FIELD_GOAL_YARDS_AFTER_END_ZONE;
	}
}

/*
* NAME:        ResetForKickoff
* DESCRIPTION: set field line of scrimmage and first down ready for kickoff
* INPUTS:      response
* OUTPUTS:     none
*/
void ResetForKickoff(struct response* res)
{
	res->kickoff_needed = true;

	if ((res->team_in_possession == 1 && res->team_1_direction == 1) || (res->team_in_possession == 2 && res->team_2_direction == 1))
	{
		res->line_of_scrimmage = STARTING_LINE_OF_SCRIMMAGE + END_ZONE_WIDTH;
		res->line_of_first_down = res->line_of_scrimmage + YARDS_PER_FIRST_DOWN;
	}
	else
	{
		res->line_of_scrimmage = FIELD_WIDTH - STARTING_LINE_OF_SCRIMMAGE + END_ZONE_WIDTH;
		res->line_of_first_down = res->line_of_scrimmage - YARDS_PER_FIRST_DOWN;
	}
}

/*
* NAME:        TurnoverOnDowns
* DESCRIPTION: switch team in possession
* INPUTS:      response
* OUTPUTS:     None
*/
void TurnoverOnDowns(struct response* res, bool field_goal)
{
	if (res->plays_remaining_in_quarter > 1) // dont change possession if 1 play left because end of quarter could take possession off this team when it shouldn't
	{
		SwitchPossession(res);
		if(field_goal)
			MoveLineOfScrimmage(res, FIELD_GOAL_MISS_PENALTY_YARDS);
		ResetLineOfFirstDown(res);
	}
	res->down = 0;
}
/*
* NAME:        PlayExtraPoint
* DESCRIPTION: execute the play
* INPUTS:      response
* OUTPUTS:     None
*/
void PlayExtraPoint(struct response* res)
{
	int field_goal_distance = SetLosExtraPoint(res);
	ResetLineOfFirstDown(res);

	char buf[256];
	snprintf(buf, sizeof(buf), "Attempting extra point.", field_goal_distance);
	ConsoleShowScreen(res, buf, GAME_SPEED * 20);

	DrawCards(res, res->team_in_possession, GetFieldGoalCardsToDraw(res));// draw 1-5 cards

	// TODO: check for an As block?

	RedrawCards(res, res->team_in_possession, GetFieldGoalCardsToRedraw(res));// redraw 0-2 cards, get player choice on high or low ones

	// TODO: check for an As block?

	int total_yards = TotalYardsOffence(res);
	char buf2[256];
	snprintf(buf2, sizeof(buf2), "Total distance: %d yards", total_yards);
	ConsoleShowScreen(res, buf2, GAME_SPEED * 20);

	if (total_yards >= field_goal_distance)
	{
		ConsoleShowScreen(res, "Extra Point Scored!", GAME_SPEED * 50); // resolve, scored
		res->team_1_score += EXTRA_POINT_POINTS;
	}
	else if (total_yards < field_goal_distance)
	{
		ConsoleShowScreen(res, "Missed Extra Point", GAME_SPEED * 20);
	}
	
	ResetForKickoff(res);
	SwitchPossession(res);

	res->down = 0;
	res->plays_remaining_in_quarter++; // should not take a play clock play so add one back
	res->extra_point_needed = false;
}

/*
* NAME:        PlayFieldGoal
* DESCRIPTION: execute the play
* INPUTS:      response
* OUTPUTS:     None
*/
void PlayFieldGoal(struct response* res)
{
	int field_goal_distance = GetFieldGoalDistance(res);

	char buf[256];
	snprintf(buf, sizeof(buf), "Attempting a %d yard field goal.", field_goal_distance);
	ConsoleShowScreen(res, buf, GAME_SPEED * 20);

	DrawCards(res, res->team_in_possession, GetFieldGoalCardsToDraw(res));// draw 1-5 cards

	// TODO: check for an As block?

	RedrawCards(res, res->team_in_possession, GetFieldGoalCardsToRedraw(res));// redraw 0-2 cards, get player choice on high or low ones

	// TODO: check for an As block?

	int total_yards = TotalYardsOffence(res);
	char buf2[256];
	snprintf(buf2, sizeof(buf2), "Total distance: %d yards", total_yards);
	ConsoleShowScreen(res, buf2, GAME_SPEED * 20);

	if (total_yards >= field_goal_distance)
	{
		ConsoleShowScreen(res, "Field Goal Scored!", GAME_SPEED * 50); // resolve, scored
		res->team_1_score += FIELD_GOAL_POINTS;
		ResetForKickoff(res);
	}
	else if (total_yards < field_goal_distance)
	{
		TurnoverOnDowns(res, true); // turnover at line of scrimmage + 10 if missed
		ConsoleShowScreen(res, "Missed field goal, turnover at LoS + 10", GAME_SPEED * 20);
	}
}

/*
* NAME:        PlayKneel
* DESCRIPTION: execute the play
* INPUTS:      response
* OUTPUTS:     None
*/
void PlayKneel(struct response* res)
{
	ConsoleShowScreen(res, "QB Kneel executed. Play over.", GAME_SPEED * 20);
}

/*
* NAME:        PlayTimeout
* DESCRIPTION: execute the play
* INPUTS:      response
* OUTPUTS:     None
*/
void PlayTimeout(struct response* res)
{
	ConsoleShowScreen(res, "Timeout Used", GAME_SPEED * 20);

	res->plays_remaining_in_quarter += 2; //add 2 plays back to the clock but then 1 will be deleted as this counts as a play

	if (res->team_in_possession == 1)
		res->team_1_timeouts_remaining--;
	else
		res->team_2_timeouts_remaining--;


	for (int i = 0; i < 5; i++)
	{
		res->plays_remaining_in_quarter -= 2; //add 2 plays back to the clock but then 1 will be deleted as this counts as a play

		if (res->team_in_possession == 1)
			res->team_1_timeouts_remaining++;
		else
			res->team_2_timeouts_remaining++;

		ConsoleShowScreen(res, 0, ANIMATE_SPEED);

		res->plays_remaining_in_quarter += 2; //add 2 plays back to the clock but then 1 will be deleted as this counts as a play

		if (res->team_in_possession == 1)
			res->team_1_timeouts_remaining--;
		else
			res->team_2_timeouts_remaining--;

		ConsoleShowScreen(res, 0, ANIMATE_SPEED);
	}

	ConsoleShowScreen(res, 0, GAME_SPEED * 10);
}

/*
* NAME:        GetPlayType
* DESCRIPTION: get input from player for play type
* INPUTS:      response
* OUTPUTS:     None
*/
int GetPlayType(struct response* res)
{
	int play_type = 0;
	ConsoleShowScreen(res, "Pick game type (1 - 5): ", GAME_SPEED * 2);
	char input[16];
	if (fgets(input, sizeof(input), stdin)) {
		play_type = atoi(input);
	}
	return play_type;
}

/*
* NAME:        GetTeamName
* DESCRIPTION: team name from input number
* INPUTS:      team number
* OUTPUTS:     name
*/
const char* GetTeamName(struct response* res, int team)
{
	if (team == 1)
		return res->team_1_name;
	else
		return res->team_2_name;
}

/*
* NAME:        AnnounceDown
* DESCRIPTION: display current down
* INPUTS:      response
* OUTPUTS:     None
*/
void AnnounceDown(struct response* res)
{
	char buf[256];
	size_t len = 0;

	len += snprintf(buf + len, sizeof(buf) - len, "%s ", GetTeamName(res, res->team_in_possession));

	const char* down_str;
	switch (res->down) {
	case 1: down_str = "1st"; break;
	case 2: down_str = "2nd"; break;
	case 3: down_str = "3rd"; break;
	case 4: down_str = "4th"; break;
	default: down_str = "Error"; break;
	}

	len += snprintf(buf + len, sizeof(buf) - len, "%s down", down_str);

	ConsoleShowScreen(res, buf, GAME_SPEED * 20);
}

/*
* NAME:        TouchdownScored
* DESCRIPTION: check if we need to award a touchdown
* INPUTS:      response
* OUTPUTS:     bool
*/
bool TouchdownScored(struct response* res)
{
	// check if touchdown, make sure not a safety
	if (((res->team_in_possession == 1 && res->team_1_direction == 1) || (res->team_in_possession == 2 && res->team_2_direction == 1)) && (res->line_of_scrimmage >= (END_ZONE_WIDTH + FIELD_WIDTH)))
	{
		return true;
	}
	else if (((res->team_in_possession == 1 && res->team_1_direction == 2) || (res->team_in_possession == 2 && res->team_2_direction == 2)) && (res->line_of_scrimmage <= END_ZONE_WIDTH))
	{
		return true;
	}
	else
		return false;
}

/*
* NAME:        SafetyScored
* DESCRIPTION: check if we need to award a safety
* INPUTS:      response
* OUTPUTS:     bool
*/
bool SafetyScored(struct response* res)
{
	// check if safety
	if (((res->team_in_possession == 1 && res->team_1_direction == 1) || (res->team_in_possession == 2 && res->team_2_direction == 1)) && (res->line_of_scrimmage <= END_ZONE_WIDTH))
	{
		return true;
	}
	else if (((res->team_in_possession == 1 && res->team_1_direction == 2) || (res->team_in_possession == 2 && res->team_2_direction == 2)) && (res->line_of_scrimmage >= (END_ZONE_WIDTH + FIELD_WIDTH)))
	{
		return true;
	}
	else
		return false;
}

/*
* NAME:        FirstDownScored
* DESCRIPTION: check if we need to award a first down
* INPUTS:      response
* OUTPUTS:     bool
*/
bool FirstDownScored(struct response* res)
{
	if (((res->team_in_possession == 1 && res->team_1_direction == 1) || (res->team_in_possession == 2 && res->team_2_direction == 1)) && (res->line_of_scrimmage >= res->line_of_first_down))
	{
		return true;
	}
	else if (((res->team_in_possession == 1 && res->team_1_direction == 2) || (res->team_in_possession == 2 && res->team_2_direction == 2)) && (res->line_of_scrimmage <= res->line_of_first_down))
	{
		return true;
	}
	else
		return false;
	return true;
}

/*
* NAME:        AwardTouchdown
* DESCRIPTION: award a touchdown
* INPUTS:      response
* OUTPUTS:     None
*/
void AwardTouchdown(struct response* res)
{
	if(res->team_in_possession == 1)
		res->team_1_score += TOUCHDOWN_POINTS;
	else if (res->team_in_possession == 2)
		res->team_2_score += TOUCHDOWN_POINTS;

	res->extra_point_needed = true;

	ConsoleShowScreen(res, "Touch Down Scored!", GAME_SPEED * 50);
}

/*
* NAME:        AwardSafety
* DESCRIPTION: award a safety
* INPUTS:      response
* OUTPUTS:     None
*/
void AwardSafety(struct response* res)
{
	if (res->team_in_possession == 1)
		res->team_2_score += SAFETY_POINTS;
	else if (res->team_in_possession == 2)
		res->team_1_score += SAFETY_POINTS;

	ConsoleShowScreen(res, "Safety Scored!", GAME_SPEED * 50);
	
	ResetForKickoff(res);
}

/*
* NAME:        ResolveEndOfPlay
* DESCRIPTION: resolve first down, touchdown etc
* INPUTS:      response
* OUTPUTS:     None
*/
void ResolveEndOfPlay(struct response* res, int play_type)
{
	if (play_type == PLAY_TYPE_TIMEOUT) // if a timeout was just played successfully, do nothing
		;
	else if (TouchdownScored(res))// if in end zone, award touchdown and set res->extra_point_needed = true;
	{
		AwardTouchdown(res);
	}
	else if (SafetyScored(res))
	{
		AwardSafety(res);
	}
	else if ((play_type != PLAY_TYPE_FIELD_GOAL) && (play_type != PLAY_TYPE_PUNT) && (FirstDownScored(res)))
	{
		ConsoleShowScreen(res, "First down achieved", GAME_SPEED * 30);
		ResetLineOfFirstDown(res);
		res->down = 0;
	}
	else if (res->down == 4)
	{
		TurnoverOnDowns(res, false);
	}
	ClearAllCards(res);
}

/*
* NAME:        DoPlay
* DESCRIPTION: find and then action each play
* INPUTS:      response
* OUTPUTS:     None
*/
void DoPlay(struct response* res)
{
	int play_type = 0;

	if (res->kickoff_needed)
	{
		play_type = 2; // always punt on first play of each half
		res->kickoff_needed = false;
	}
	else if (res->extra_point_needed)
	{
		play_type = 3;  // TODO: 2 point extra, just a normal play from 2 yard line?
	}
	else
	{
		AnnounceDown(res);
	}

	bool retry = false;
	if (!play_type)
		play_type = GetPlayType(res);

	do {
		retry = false;
		switch (play_type) {
		case PLAY_TYPE_NORMAL:
		{
			PlayNormal(res);
			break;
		}
		case PLAY_TYPE_PUNT:
		{
			PlayPunt(res);
			break;
		}
		case PLAY_TYPE_FIELD_GOAL:
		{
			if (res->extra_point_needed)
				PlayExtraPoint(res);
			else
				PlayFieldGoal(res);
			break;
		}
		case PLAY_TYPE_KNEEL:
		{
			PlayKneel(res);
			break;
		}
		case PLAY_TYPE_TIMEOUT:
		{
			if (res->plays_remaining_in_quarter > (PLAYS_PER_QUARTER - 2))
				retry = true;
			else if (res->team_in_possession == 1 && !res->team_1_timeouts_remaining)
				retry = true;
			else if (res->team_in_possession == 2 && !res->team_2_timeouts_remaining)
				retry = true;
			else
				PlayTimeout(res);
			break;
		}
		default:
		{
			retry = true;
			break;
		}
		}
		if(retry)
			ConsoleShowScreen(res, "Invalid play type. Please pick a number between 1 and 5.", GAME_SPEED * 20); // (you cannot timeout unless 2 plays have already happened this quarter and you have 1 remaining)
		if (retry || !play_type)
			play_type = GetPlayType(res); // player picks what to do: normal play, punt, field goal, kneel, timeout
	} while (retry);

	ResolveEndOfPlay(res, play_type);
}

/*
* NAME:        Play
* DESCRIPTION: plays out one american football game
* INPUTS:      response
* OUTPUTS:     None
*/
void Play(struct response* res)
{
	for (res->quarter = 1; res->quarter <= NUMBER_OF_QUARTERS; res->quarter++)
	{
		for (res->plays_remaining_in_quarter = PLAYS_PER_QUARTER; res->plays_remaining_in_quarter > 0; res->plays_remaining_in_quarter--, res->down++)
		{
			if (res->quarter == 1 || res->quarter == 3)
			{
				if (res->plays_remaining_in_quarter == PLAYS_PER_QUARTER) {
					res->team_1_timeouts_remaining = TIMEOUTS_PER_TEAM_PER_HALF;
					res->team_2_timeouts_remaining = TIMEOUTS_PER_TEAM_PER_HALF;
				}

				if (res->quarter == 3)
				{
					res->team_not_in_possession = res->team_in_possession;
					res->team_in_possession = res->team_started_with_possession - 1 ? res->team_started_with_possession - 1 : res->team_started_with_possession + 1;
				}
			}

			if (res->plays_remaining_in_quarter == PLAYS_PER_QUARTER && res->quarter != 1) {
				SwitchEnds(res);
				ConsoleShowScreen(res, "Switching ends...", GAME_SPEED * 10);
			}

			DoPlay(res);
		}
		//TODO: announce end of quarter, announce who will have the ball / be punting next, shuffle decks?
	}
}