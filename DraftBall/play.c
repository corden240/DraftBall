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
void PlayNormal(struct response* res);
bool TouchdownScored(struct response* res);
void ResetForKickoff(struct response* res);
void SwitchEnds(struct response* res);
int GetDirectionOfPlay(struct response* res);
const char* GetTeamName(struct response* res, int team);
int GetDirectionOfPlay(struct response* res);
const char* GetTargetPlayerName(struct response* res);

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

	for (int i = 0; i < NORMAL_PLAY_CARDS_TO_DRAW; i++)
	{
		res->o_cards_in_position[i] = 0;
		res->d_cards_in_position[i] = 0;
	}
}

/*
* NAME:        ClearOCards
* DESCRIPTION: clear o_cards
* INPUTS:      response
* OUTPUTS:     None
*/
void ClearOCards(struct response* res)
{
	for (int i = 0; i < CARDS_IN_A_DECK; i++)
	{
		res->o_cards[i] = 0;
	}
}

/*
* NAME:        ClearDCards
* DESCRIPTION: clear d_cards
* INPUTS:      response
* OUTPUTS:     None
*/
void ClearDCards(struct response* res)
{
	for (int i = 0; i < CARDS_IN_A_DECK; i++)
	{
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
	if (TableWeightTotalWeight(deck) <= 0)
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
void DrawCards(struct response* res, int team_to_draw, int num, bool shared)
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
		if(shared)
			hand[start + i] = DrawOneCard(res, shared_deck);
		else if (team_to_draw == 1)
			hand[start + i] = DrawOneCard(res, player_1_deck);
		else
			hand[start + i] = DrawOneCard(res, player_2_deck);

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
	SwitchPossession(res);
	res->down = 0;
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
		DrawCards(res, res->team_in_possession, 1, true);
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
		MoveLineOfScrimmage(res, yards);

		if (TouchdownScored(res))
			return;
	}

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
void PlayPass(struct response* res)
{
	ConsoleShowScreen(res, "Pass play ", GAME_SPEED * 10);

	for (int i = 0; i < PASS_PLAY_MAX_CARDS_TO_DRAW; i++)
	{
		if (i > 0)
		{
			char buf[256];
			snprintf(buf, sizeof(buf), "Sum = %d, draw a card? y/n ", TotalYardsOffence(res));
			ConsoleShowScreen(res, buf, 0);

			char input[16];
			fgets(input, sizeof(input), stdin);

			if (input[0] == 'N' || input[0] == 'n')
				break;
		}

		DrawCards(res, res->team_in_possession, 1, true);

		if ((res->o_cards[i + 1] & VALUE) == ACE && (res->o_cards[i + 1] & SUIT) == SPADES) // If O2+ is the Ace of spades then draw a replacement for that card and then go to Interception.
		{
			ConsoleShowScreen(res, "Pass intercepted ", GAME_SPEED * 20);
			// replace card, set line of scrim
			Interception(res);
			return;
		}
		else if ((res->o_cards[i + 1] & VALUE) == (res->o_cards[0] & VALUE)
	   && (((res->o_cards[i + 1] & RED) && (res->o_cards[0] & RED))
	    || ((res->o_cards[i + 1] & BLACK) && (res->o_cards[0] & BLACK)))) // If it is the same rank and colour as card O1 then it is a sack
		{
			ConsoleShowScreen(res, "QB sacked ", GAME_SPEED * 20);
			ClearAllCards(res);
			DrawCards(res, res->team_in_possession, 2, true);// draw for sack loss yardage, if and As then fumble
			//if (((res->o_cards[0] & VALUE) == ACE && (res->o_cards[0] & SUIT) == SPADES) || ((res->o_cards[1] & VALUE) == ACE && (res->o_cards[1] & SUIT) == SPADES))
			//{
			//	Fumble(res);
			//  return;
			//}
			int sack_yards = TotalYardsOffence(res);

			if (sack_yards % 2 == 0)
				sack_yards = -(sack_yards / 2);
			else
				sack_yards = -((sack_yards + 1) / 2);

			char buf2[256];
			snprintf(buf2, sizeof(buf2), "%d yards lost ", sack_yards);
			ConsoleShowScreen(res, buf2, 0);

			MoveLineOfScrimmage(res, -sack_yards);
			ResetLineOfFirstDown(res);

			return;
		}
		else if ((res->o_cards[i + 1] & SUIT) == ((res->o_cards[0] & SUIT))) // If it is the same suit as card O1 then the pass is incomplete
		{
			char buf6[256];
			snprintf(buf6, sizeof(buf6), "Pass to %s incomplete ", GetTargetPlayerName(res));
			ConsoleShowScreen(res, buf6, GAME_SPEED * 20);
			ClearAllCards(res);
			return;
		}
	}
	
	DrawCards(res, res->team_not_in_possession, 1, true); // defence draws 1
	int catch_yards = TotalYardsOffence(res) - TotalYardsDefence(res);

	char buf3[256];
	if (catch_yards < 0)
	{
		if (catch_yards % 2 == 0)
			catch_yards = catch_yards / 2;
		else
			catch_yards = (catch_yards + 1) / 2;
		snprintf(buf3, sizeof(buf3), "%d yards lost ", catch_yards);
	}
	else
		snprintf(buf3, sizeof(buf3), "%d yard catch made ", catch_yards);

	ConsoleShowScreen(res, buf3, GAME_SPEED * 10);

	MoveLineOfScrimmage(res, catch_yards);

	char buf4[256];

	if (TouchdownScored(res))
		snprintf(buf4, sizeof(buf4), "Ball caught in the end zone");
	else
		snprintf(buf4, sizeof(buf4), "%s caught the ball at the %d", GetTargetPlayerName(res), ConvertLosToYardLine(res));
	ConsoleShowScreen(res, buf4, GAME_SPEED * 10);

	ClearAllCards(res);

	if (TouchdownScored(res))
		return;

	ConsoleShowScreen(res, "YAC draw", GAME_SPEED * 10);

	DrawCards(res, res->team_in_possession, 1, true);
	DrawCards(res, res->team_not_in_possession, 1, true);

	//if (((res->o_cards[0] & VALUE) == ACE && (res->o_cards[0] & SUIT) == SPADES) || ((res->d_cards[0] & VALUE) == ACE && (res->d_cards[0] & SUIT) == SPADES))
	//{
	//	Fumble(res);
	//}else
	
	int yac_yards = TotalYardsOffence(res) - TotalYardsDefence(res);
	
	if (yac_yards < 0)
		yac_yards = 0;

	MoveLineOfScrimmage(res, yac_yards);

	if (TouchdownScored(res))
		return;

	if ((res->o_cards[0] & FACE) && ((res->o_cards[0] & VALUE) > (res->d_cards[0] & VALUE)))
	{
		OpenFieldRun(res, res->o_cards[0], res->d_cards[0]);
	}
	else
	{
		char buf5[256];
		snprintf(buf5, sizeof(buf5), "%d yards after catch ", yac_yards);
		ConsoleShowScreen(res, buf5, GAME_SPEED * 10);
	}
}

/*
* NAME:        PlayRun
* DESCRIPTION: execute the play
* INPUTS:      response
* OUTPUTS:     None
*/
void PlayRun(struct response* res)
{
	ConsoleShowScreen(res, "Run play ", GAME_SPEED * 10);

	DrawCards(res, res->team_in_possession, RUN_PLAY_OFFENCE_CARDS_TO_DRAW, true);
	DrawCards(res, res->team_not_in_possession, RUN_PLAY_DEFENCE_CARDS_TO_DRAW, true);

	int yards_gained = 0;
	int difference = TotalYardsOffence(res) - TotalYardsDefence(res);

	if (difference < 0)
	{
		if (difference % 4 == 0)
			yards_gained = difference / 4;
		else
			yards_gained = (difference - (4 - (difference % 4))) / 4; // rounding up
	}
	else
	{
		if (difference % 2 == 0)
			yards_gained =  difference / 2;
		else
			yards_gained = (difference + 1) / 2; // rounding up
	}

	MoveLineOfScrimmage(res, yards_gained);
	
	char buf[256];
	snprintf(buf, sizeof(buf), "%s gained %d yards ", GetTargetPlayerName(res), yards_gained);
	ConsoleShowScreen(res, buf, GAME_SPEED * 30);

	if (TouchdownScored(res))
		return;

	// if O2 is a face card and larger than D2 the go to Open field run
	if ((res->o_cards[RUN_PLAY_OFFENCE_CARDS_TO_DRAW] & FACE) && ((res->o_cards[RUN_PLAY_OFFENCE_CARDS_TO_DRAW] & VALUE) > (res->d_cards[RUN_PLAY_DEFENCE_CARDS_TO_DRAW] & VALUE)))
	{
		OpenFieldRun(res, res->o_cards[RUN_PLAY_OFFENCE_CARDS_TO_DRAW], res->d_cards[RUN_PLAY_DEFENCE_CARDS_TO_DRAW]);
	}

	// If O2 OR D2 is the Ace of spades then draw a replacement for that card and then go to Fumble
}

/*
* NAME:        PlayTwoPointConversion
* DESCRIPTION: execute the play
* INPUTS:      response
* OUTPUTS:     None
*/
void PlayTwoPointConversion(struct response* res)
{
	if (GetDirectionOfPlay(res) == 1)
	{
		res->line_of_scrimmage = END_ZONE_WIDTH + FIELD_WIDTH - TWO_POINT_CONVERSION_YARDS;
	}
	else
	{
		res->line_of_scrimmage = END_ZONE_WIDTH + TWO_POINT_CONVERSION_YARDS;
	}

	PlayNormal(res); // not right as in conversion a pick 6 cant happen but it can in PlayNormal, need to guard for this

	if (TouchdownScored(res)) // if in the end zone
	{
		ConsoleShowScreen(res, "2 Point Conversion Scored!", GAME_SPEED * 50); // resolve, scored
		res->team_1_score += TWO_POINT_CONVERSION_POINTS;
	}
	else
	{
		ConsoleShowScreen(res, "Missed Extra Points", GAME_SPEED * 20);
	}

	ResetForKickoff(res);

	res->down = 0;
	res->plays_remaining_in_quarter++; // should not take a play clock play so add one back
	res->extra_point_needed = false;
}

/*
* NAME:        GetFormation
* DESCRIPTION: ask for and set the formation for the play
* INPUTS:      response
* OUTPUTS:     int
*/
int GetFormation(struct response* res)
{
	// []       []  []       []
	// 
	// []         []     []  []
	// 
	// []  []            []  []
	ConsoleShowScreen(res, "Pick formation (RB-WR) 2:2 or 1:3 or 0:4 (1 - 3): ", GAME_SPEED * 10);

	char input[16];
	fgets(input, sizeof(input), stdin);
	int pick = atoi(input);
	if (pick < 1 || pick > 3)
		pick = 1;
	return pick;
}

/*
* NAME:        GetMatchupNumber
* DESCRIPTION: ask for and set the matchup in play 1-4
* INPUTS:      response
* OUTPUTS:     int
*/
int GetMatchupNumber(struct response* res)
{
	char buf[256];
	snprintf(buf, sizeof(buf), "%s, pick matchup to play (1 - 4):", GetTeamName(res, res->team_in_possession));
	ConsoleShowScreen(res, buf, 0);

	char input[16];
	fgets(input, sizeof(input), stdin);
	int pick = atoi(input);
	if (pick < 1 || pick > 4)
		pick = 1;
	return pick;
}

/*
* NAME:        PutCardsInFormation
* DESCRIPTION: asks and then places the 4 cards into their formation
* INPUTS:      response, offence_or_defence (1-2)
* OUTPUTS:     None
*/
void PutCardsInFormation(struct response* res, int offence_or_defence)
{
	int pick = 0;
	bool retry = false;
	for (int i = 0; i < NORMAL_PLAY_CARDS_TO_DRAW; i++)
	{	
		do {
			retry = false;
			char buf[256];
			snprintf(buf, sizeof(buf), "What position (1 - %d) to place card %d: ", NORMAL_PLAY_CARDS_TO_DRAW, i + 1);
			ConsoleShowScreen(res, buf, GAME_SPEED * 1);

			char input[16];
			fgets(input, sizeof(input), stdin);
			pick = atoi(input);

			if (pick < 1 || pick > NORMAL_PLAY_CARDS_TO_DRAW || (offence_or_defence == 1 && res->o_cards_in_position[pick - 1]) || (offence_or_defence == 2 && res->d_cards_in_position[pick - 1]))
			{
				retry = true;
				ConsoleShowScreen(res, "Invalid position chosen, choose again.", GAME_SPEED * 20);
			}

		} while (retry);
		if(offence_or_defence == 1)
			res->o_cards_in_position[pick - 1] = res->o_cards[i];
		else if (offence_or_defence == 2)
			res->d_cards_in_position[pick - 1] = res->d_cards[i];
		ConsoleShowScreen(res, 0, GAME_SPEED * 1);
	}
}

/*
* NAME:        SetRunOrPassPlay
* DESCRIPTION: record if the play is a run or a pass
* INPUTS:      response
* OUTPUTS:     None
*/
void SetRunOrPassPlay(struct response* res)
{
	// []       []  []       []
	// 
	// []         []     []  []
	// 
	// []  []            []  []

	if (res->formation == 1)
	{
		if(res->matchup_number == 2 || res->matchup_number == 3)
		{
			res->pass_play = 0;
			res->run_play = 1;
		}
		else
		{
			res->pass_play = 1;
			res->run_play = 0;
		}
	}
	else if (res->formation == 2)
	{
		if (res->matchup_number == 2)
		{
			res->pass_play = 0;
			res->run_play = 1;
		}
		else
		{
			res->pass_play = 1;
			res->run_play = 0;
		}
	}
	else if (res->formation == 3)
	{
		res->pass_play = 1;
		res->run_play = 0;
	}
}

/*
* NAME:        PlayNormal
* DESCRIPTION: execute the play
* INPUTS:      response
* OUTPUTS:     None
*/
void PlayNormal(struct response* res)
{
	char buf[256];
	snprintf(buf, sizeof(buf), "%s, press enter to draw", GetTeamName(res, res->team_in_possession));
	ConsoleShowScreen(res, buf, GAME_SPEED * 30);

	char input[16];
	fgets(input, sizeof(input), stdin);

	DrawCards(res, res->team_in_possession, NORMAL_PLAY_CARDS_TO_DRAW, false); // O: draw 4 cards

	res->formation = GetFormation(res); // O: pick layout/formation/personnel
	res->show_O_D_Both = 1;

	ConsoleShowScreen(res, 0, GAME_SPEED * 10); // show formation with blanks ready for cards to be put in

	PutCardsInFormation(res, 1); // Offence place cards in positions, send 1 for offence
	
	res->show_O_D_Both = 0; // Cover offence cards
	
	char buf2[256];
	snprintf(buf2, sizeof(buf2), "%s, press enter to draw", GetTeamName(res, res->team_not_in_possession));
	ConsoleShowScreen(res, buf2, GAME_SPEED * 30);

	char input2[16];
	fgets(input2, sizeof(input2), stdin);

	int save_formation = res->formation;
	res->formation = 0;
	ClearOCards(res);

	res->show_O_D_Both = 2; // show D cards
	DrawCards(res, res->team_not_in_possession, NORMAL_PLAY_CARDS_TO_DRAW, false); // D: draw 4 cards

	res->formation = save_formation;
	PutCardsInFormation(res, 2); // Defence place cards in positions, send 1 for Defence
	
	res->show_O_D_Both = 0; // cover both sets of cards
	ConsoleShowScreen(res, "Press enter to reveal matchups", GAME_SPEED * 10);
	char input3[16];
	fgets(input3, sizeof(input3), stdin);

	res->show_O_D_Both = 3;

	// save off cards
	int save_o_cards[] = { 0,0,0,0 };
	int save_d_cards[] = { 0,0,0,0 };
	
	for (int i = 0; i < NORMAL_PLAY_CARDS_TO_DRAW; i++)
	{
		save_o_cards[i] = res->o_cards_in_position[i];
		save_d_cards[i] = res->d_cards_in_position[i];
		res->o_cards_in_position[i] = 0;
		res->d_cards_in_position[i] = 0;
	}
	ConsoleShowScreen(res, 0, GAME_SPEED * 10);
	for (int i = 0; i < NORMAL_PLAY_CARDS_TO_DRAW; i++)
	{
		res->o_cards_in_position[i] = save_o_cards[i];
		res->d_cards_in_position[i] = save_d_cards[i];
		ConsoleShowScreen(res, 0, GAME_SPEED * 10);
	}

	res->matchup_number = GetMatchupNumber(res); // Show all 8 cards and "O: pick which of the 4 match ups to play"
	
	SetRunOrPassPlay(res);

	// meter here? player being targetted/given the ball
	
	// store 2 head to head cards
	int O1 = res->o_cards_in_position[res->matchup_number - 1];
	int D1 = res->d_cards_in_position[res->matchup_number - 1];

	ClearAllCards(res);

	res->o_cards[0] = O1;
	res->d_cards[0] = D1;
	
	ConsoleShowScreen(res, 0, GAME_SPEED * 10);

	if(res->pass_play)
		PlayPass(res);
	else if (res->run_play)
		PlayRun(res);
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
	else if (num < 1)
		num = 1;

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
	else if (num < 1)
		num = 1;

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
			res->down = 0;
			return;
		}
	}

	ConsoleShowScreen(res, "Punt return...", GAME_SPEED * 20);

	// Receiver draws two cards and the punter draws one card from the shared deck
	DrawCards(res, res->team_in_possession, PUNT_RETURN_OFFENCE_CARDS_TO_DRAW, true);
	DrawCards(res, res->team_not_in_possession, PUNT_RETURN_DEFENCE_CARDS_TO_DRAW, true);
	
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
	if ((res->o_cards[PUNT_RETURN_OFFENCE_CARDS_TO_DRAW - 1] & FACE) && ((res->o_cards[PUNT_RETURN_OFFENCE_CARDS_TO_DRAW - 1] & VALUE) > (res->d_cards[PUNT_RETURN_DEFENCE_CARDS_TO_DRAW - 1] & VALUE)))
	{
		OpenFieldRun(res, res->o_cards[PUNT_RETURN_OFFENCE_CARDS_TO_DRAW - 1], res->d_cards[PUNT_RETURN_DEFENCE_CARDS_TO_DRAW - 1]);
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
* NAME:        GetTargetPlayerName
* DESCRIPTION: return the name of the player involved in this play
* INPUTS:      response
* OUTPUTS:     name
*/
const char* GetTargetPlayerName(struct response* res)
{
	// []       []  []       []
	// 
	// []         []     []  []
	// 
	// []  []            []  []
	
	if (res->team_in_possession == 1)
	{
		if (res->matchup_number == 1)
			return res->team_1_wr_names[0];
		else if ((res->formation == 1 && res->matchup_number == 4) || (res->formation == 2 && res->matchup_number == 3) || (res->formation == 3 && res->matchup_number == 2))
			return res->team_1_wr_names[1];
		else if ((res->formation == 2 && res->matchup_number == 4) || (res->formation == 3 && res->matchup_number == 3))
			return res->team_1_wr_names[2];
		else if ((res->formation == 3 && res->matchup_number == 4))
			return res->team_1_wr_names[3];
		else if ((res->formation == 1 && res->matchup_number == 2) || (res->formation == 2 && res->matchup_number == 2))
			return res->team_1_rb_names[0];
		else if ((res->formation == 1 && res->matchup_number == 3))
			return res->team_1_rb_names[1];
	}
	else
	{
		if (res->matchup_number == 1)
			return res->team_2_wr_names[0];
		else if ((res->formation == 1 && res->matchup_number == 4) || (res->formation == 2 && res->matchup_number == 3) || (res->formation == 3 && res->matchup_number == 2))
			return res->team_2_wr_names[1];
		else if ((res->formation == 2 && res->matchup_number == 4) || (res->formation == 3 && res->matchup_number == 3))
			return res->team_2_wr_names[2];
		else if ((res->formation == 3 && res->matchup_number == 4))
			return res->team_2_wr_names[3];
		else if ((res->formation == 1 && res->matchup_number == 2) || (res->formation == 2 && res->matchup_number == 2))
			return res->team_2_rb_names[0];
		else if ((res->formation == 1 && res->matchup_number == 3))
			return res->team_2_rb_names[1];
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

	DrawCards(res, res->team_in_possession, GetPuntCardsToDraw(res), true);// draw 1-5 cards

	RedrawCards(res, res->team_in_possession, GetPuntCardsToRedraw(res));// redraw 0-2 cards, get player choice on high or low ones

	int total_yards = TotalYardsOffence(res);
	char buf[256];
	snprintf(buf, sizeof(buf), "Total Yards: %d", total_yards);
	ConsoleShowScreen(res, buf, GAME_SPEED * 10);

	MoveLineOfScrimmage(res, total_yards);

	char buf2[256];
	if ((GetDirectionOfPlay(res) == 1) && (res->line_of_scrimmage >= (END_ZONE_WIDTH + FIELD_WIDTH)))
	{
		snprintf(buf2, sizeof(buf2), "Ball caught %d yards into the end zone", abs(ConvertLosToYardLine(res)));
	}
	else if ((GetDirectionOfPlay(res) == 2) && (res->line_of_scrimmage <= END_ZONE_WIDTH))
	{
		snprintf(buf2, sizeof(buf2), "Ball caught %d yards into the end zone", abs(ConvertLosToYardLine(res)));
	}	
	else
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
	if (GetDirectionOfPlay(res) == 1)
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

	if (GetDirectionOfPlay(res) == 1)
	{
		distance = (FIELD_WIDTH - (res->line_of_scrimmage - END_ZONE_WIDTH)) + FIELD_GOAL_YARDS_AFTER_END_ZONE;
	}
	else
	{
		distance = (res->line_of_scrimmage - END_ZONE_WIDTH) + FIELD_GOAL_YARDS_AFTER_END_ZONE;
	}
	return distance;
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
	res->down = 0;

	if (GetDirectionOfPlay(res) == 1)
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
	int field_goal_distance = EXTRA_POINT_YARDAGE_FROM_END_ZONE + FIELD_GOAL_YARDS_AFTER_END_ZONE;
	SetLosExtraPoint(res);
	ResetLineOfFirstDown(res);

	char buf[256];
	snprintf(buf, sizeof(buf), "Attempting extra point. %d yards needed. ", field_goal_distance);
	ConsoleShowScreen(res, buf, GAME_SPEED * 20);

	DrawCards(res, res->team_in_possession, GetFieldGoalCardsToDraw(res), true);// draw 1-5 cards

	// check for an As block?

	RedrawCards(res, res->team_in_possession, GetFieldGoalCardsToRedraw(res));// redraw 0-2 cards, get player choice on high or low ones

	// check for an As block?

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

	DrawCards(res, res->team_in_possession, GetFieldGoalCardsToDraw(res), true);// draw 1-5 cards

	// check for an As block?

	RedrawCards(res, res->team_in_possession, GetFieldGoalCardsToRedraw(res));// redraw 0-2 cards, get player choice on high or low ones

	// check for an As block?

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
* NAME:        GetDirectionOfPlay
* DESCRIPTION: return 1-2 if team with ball is going right or left
* INPUTS:      response
* OUTPUTS:     1-2 right or left
*/
int GetDirectionOfPlay(struct response* res)
{
	return res->team_in_possession == 1 ? res->team_1_direction : res->team_2_direction;
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
	if ((GetDirectionOfPlay(res) == 1) && (res->line_of_scrimmage >= (END_ZONE_WIDTH + FIELD_WIDTH)))
	{
		return true;
	}
	else if ((GetDirectionOfPlay(res) == 2) && (res->line_of_scrimmage <= END_ZONE_WIDTH))
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
	if ((res->play_type != PLAY_TYPE_NORMAL) && (res->play_type != PLAY_TYPE_PUNT))
	{
		return false;
	}
	else if ((GetDirectionOfPlay(res) == 1) && (res->line_of_scrimmage <= END_ZONE_WIDTH))
	{
		return true;
	}
	else if ((GetDirectionOfPlay(res) == 2) && (res->line_of_scrimmage >= (END_ZONE_WIDTH + FIELD_WIDTH)))
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
	if ((GetDirectionOfPlay(res) == 1) && (res->line_of_scrimmage >= res->line_of_first_down))
	{
		return true;
	}
	else if ((GetDirectionOfPlay(res) == 2) && (res->line_of_scrimmage <= res->line_of_first_down))
	{
		return true;
	}
	
	return false;
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
	
	char buf[256];
	snprintf(buf, sizeof(buf), "Touch Down Scored by %s!", GetTargetPlayerName(res));
	ConsoleShowScreen(res, buf, GAME_SPEED * 50);
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
* NAME:        GetOneOrTwoPointConversion
* DESCRIPTION: ask for and return the conversion type
* INPUTS:      response
* OUTPUTS:     int 1 = 1 pt attempt, 2 = 2 pt attempt
*/
int GetOneOrTwoPointConversion(struct response* res)
{
	int points = 1;
	ConsoleShowScreen(res, "Extra point attempt (1 - 2): ", GAME_SPEED * 2);
	char input[16];
	if (fgets(input, sizeof(input), stdin)) {
		points = atoi(input);
	}
	if (points != 1 && points != 2)
		points = 1;
	return points;
}

/*
* NAME:        ResolveEndOfPlay
* DESCRIPTION: resolve first down, touchdown etc
* INPUTS:      response
* OUTPUTS:     None
*/
void ResolveEndOfPlay(struct response* res)
{
	if (res->play_type == PLAY_TYPE_TIMEOUT) // if a timeout was just played successfully, do nothing
		;
	else if (TouchdownScored(res))// if in end zone, award touchdown and set res->extra_point_needed = true;
	{
		AwardTouchdown(res);
	}
	else if (SafetyScored(res))
	{
		AwardSafety(res);
	}
	else if ((res->play_type != PLAY_TYPE_FIELD_GOAL) && (res->play_type != PLAY_TYPE_PUNT) && (FirstDownScored(res)))
	{
		char buf[256];
		snprintf(buf, sizeof(buf), "%s first down achieved", GetTeamName(res, res->team_in_possession));
		ConsoleShowScreen(res, buf, GAME_SPEED * 30);
		ResetLineOfFirstDown(res);
		res->down = 0;
	}
	else if (res->down == 4)
	{
		TurnoverOnDowns(res, false);
	}

	res->show_O_D_Both = 0;
	res->pass_play = 0;
	res->run_play = 0;
	res->matchup_number = 0;
	res->formation = 0;
	res->play_type = 0;
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
	if (res->kickoff_needed)
	{
		res->play_type = 2; // always punt on first play of each half
		res->kickoff_needed = false;
	}
	else if (res->extra_point_needed)
	{
		if (GetOneOrTwoPointConversion(res) == 2) // ask player if 1 or 2 point conversion?
			res->play_type = 1;						  // 2 pt attempt
		else
			res->play_type = 3;						  // 1 pt attempt
	}
	else
	{
		AnnounceDown(res);
	}

	bool retry = false;
	if (!res->play_type)
		res->play_type = GetPlayType(res);

	do {
		retry = false;
		switch (res->play_type) {
		case PLAY_TYPE_NORMAL:
		{
			if (res->extra_point_needed)
				PlayTwoPointConversion(res);
			else
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
		if (retry || !res->play_type)
			res->play_type = GetPlayType(res); // player picks what to do: normal play, punt, field goal, kneel, timeout
	} while (retry);

	ResolveEndOfPlay(res);
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

			if (res->plays_remaining_in_quarter == PLAYS_PER_QUARTER && res->quarter != 1) { // move this down below
				SwitchEnds(res);
				ConsoleShowScreen(res, "Switching ends...", GAME_SPEED * 10);
			}

			DoPlay(res);
		}
		// announce end of quarter, announce who will have the ball / be punting next, shuffle decks?
	}
}