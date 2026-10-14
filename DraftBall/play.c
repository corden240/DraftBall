#include <math.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>   /* srand, rand, atoi */
#include <time.h>     /* time() */
#include "main.h"
#include <string.h>

void ResetLineOfFirstDown(struct game_data* game_data);
void SwitchPossession(struct game_data* game_data);
void ResetLineOfScrimmage(struct game_data* game_data);
int TableWeightTotalWeight(struct table_weight* table);
void TableWeightSet(struct table_weight* table, int val, bool on_off);
void MoveLineOfScrimmage(struct game_data* game_data, int yards);
void PlayNormal(struct game_data* game_data);
bool TouchdownScored(struct game_data* game_data);
void ResetForKickoff(struct game_data* game_data);
void SwitchEnds(struct game_data* game_data);
int GetDirectionOfPlay(struct game_data* game_data);
const char* GetTeamName(struct game_data* game_data, int team);
int GetDirectionOfPlay(struct game_data* game_data);
const char* GetTargetPlayerName(struct game_data* game_data);
bool GetTableValOnOff(struct table_weight* table, int val);
const char* GetQBName(struct game_data* game_data);
bool SafetyScored(struct game_data* game_data);
void PlayGame(struct game_data* game_data);
int ConvertLosToYardLine(struct game_data* game_data);
int GetTimeoutTeam(struct game_data* game_data);
void GameStats(struct game_data* game_data);

/*
* NAME:        ClearAllCards
* DESCRIPTION: clear o_cards and d_cards
* INPUTS:      game_data
* OUTPUTS:     none
*/
void ClearAllCards(struct game_data* game_data)
{
	for (int i = 0; i < CARDS_IN_A_DECK; i++)
	{
		game_data->o_cards[i] = 0;
		game_data->d_cards[i] = 0;
	}

	for (int i = 0; i < NORMAL_PLAY_CARDS_TO_DRAW; i++)
	{
		game_data->o_cards_in_position[i] = 0;
		game_data->d_cards_in_position[i] = 0;
	}
}

/*
* NAME:        ClearOCards
* DESCRIPTION: clear o_cards
* INPUTS:      game_data
* OUTPUTS:     none
*/
void ClearOCards(struct game_data* game_data)
{
	for (int i = 0; i < CARDS_IN_A_DECK; i++)
	{
		game_data->o_cards[i] = 0;
	}
}

/*
* NAME:        ClearDCards
* DESCRIPTION: clear d_cards
* INPUTS:      game_data
* OUTPUTS:     none
*/
void ClearDCards(struct game_data* game_data)
{
	for (int i = 0; i < CARDS_IN_A_DECK; i++)
	{
		game_data->d_cards[i] = 0;
	}
}

/*
* NAME:        DrawOneCard
* DESCRIPTION: pick an available card from the deck
* INPUTS:      game_data
* OUTPUTS:     card
*/
int DrawOneCard(struct game_data* game_data, struct table_weight* deck)
{
	//return (KING | CLUBS); // force
	if (TableWeightTotalWeight(deck) <= 0)
	{
		TableWeightEnableAll(deck);
		ConsoleShowGameScreen(game_data, "Deck empty, reshuffling...", GAME_SPEED * 20);
	}

	int card = TableWeightDecision(deck, false);

	TableWeightSet(deck, card, false);

	return card;
}

/*
* NAME:        DrawCards
* DESCRIPTION: draw and assign x cards
* INPUTS:      game_data
* OUTPUTS:     none
*/
void DrawCards(struct game_data* game_data, int team_to_draw, int num, bool shared)
{
	/* pick which hand to fill */
	int* hand = (team_to_draw == game_data->team_in_possession)
		? game_data->o_cards
		: game_data->d_cards;

	/* skip over cards already in the hand; an empty slot has VALUE == 0 */
	int start = 0;
	while (start < CARDS_IN_A_DECK && (hand[start] & VALUE) != 0)
		start++;

	/* append num cards at the end, without overflowing the array */
	for (int i = 0; i < num && (start + i) < CARDS_IN_A_DECK; i++)
	{
		if(shared)
			hand[start + i] = DrawOneCard(game_data, shared_deck);
		else if (team_to_draw == 1)
			hand[start + i] = DrawOneCard(game_data, player_1_deck);
		else
			hand[start + i] = DrawOneCard(game_data, player_2_deck);

		ConsoleShowGameScreen(game_data, 0, GAME_SPEED * 8); // show cards drawn
	}
}

/*
* NAME:        RedrawCards
* DESCRIPTION: draw and assign x cards
* INPUTS:      game_data
* OUTPUTS:     none
*/
void RedrawCards(struct game_data* game_data, int team_to_draw, int num)
{
	if (!num)
		return;

	char buf[256];
	snprintf(buf, sizeof(buf), "Replace the low or high cards (1 - 2): ");
	ConsoleShowGameScreen(game_data, buf, GAME_SPEED * 2);

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
		while (hand_size < CARDS_IN_A_DECK && (game_data->o_cards[hand_size] & VALUE) != 0)
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
				int rank_a = game_data->o_cards[indices[j]] & VALUE;
				int rank_b = game_data->o_cards[indices[j + 1]] & VALUE;
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
				game_data->o_cards[idx] = DrawOneCard(game_data, shared_deck);
				ConsoleShowGameScreen(game_data, 0, GAME_SPEED * 8); // show cards drawn
			}
		}
		else /* hilo == 2 */
		{
			// highest "num" cards -> back of sorted indices
			for (int k = 0; k < num_to_replace; k++)
			{
				int idx = indices[hand_size - 1 - k];
				game_data->o_cards[idx] = DrawOneCard(game_data, shared_deck);
				ConsoleShowGameScreen(game_data, 0, GAME_SPEED * 8); // show cards drawn
			}
		}
	}
}

/*
* NAME:        TotalYardsOffence
* DESCRIPTION: add up total yards gained
* INPUTS:      game_data
* OUTPUTS:     int
*/
int TotalYardsOffence(struct game_data* game_data)
{
	int sum_value = 0;
	for (int i = 0; i < CARDS_IN_A_DECK; i++)
	{
		sum_value += (game_data->o_cards[i] & VALUE);
	}
	return sum_value;
}

/*
* NAME:        TotalYardsDefence
* DESCRIPTION: add up total yards gained
* INPUTS:      game_data
* OUTPUTS:     int
*/
int TotalYardsDefence(struct game_data* game_data)
{
	int sum_value = 0;
	for (int i = 0; i < CARDS_IN_A_DECK; i++)
	{
		sum_value += (game_data->d_cards[i] & VALUE);
	}
	return sum_value;
}

/*
* NAME:        Interception
* DESCRIPTION: execute the play
* INPUTS:      game_data
* OUTPUTS:     none
*/
void Interception(struct game_data* game_data)
{
	SwitchPossession(game_data);
	ResetLineOfFirstDown(game_data);
	game_data->down = 0;
}

/*
* NAME:        Fumble
* DESCRIPTION: execute the play
* INPUTS:      game_data
* OUTPUTS:     none
*/
void Fumble(struct game_data* game_data)
{
	bool fumble_resolved = false;
	int save_team_started_with_ball = game_data->team_in_possession;
	do {
		ClearAllCards(game_data);
		ConsoleShowGameScreen(game_data, "Drawing fumble cards ", GAME_SPEED * 20);
		DrawCards(game_data, game_data->team_in_possession, 1, true);
		DrawCards(game_data, game_data->team_not_in_possession, 1, true);

		if ((game_data->o_cards[0] & SUIT) == (game_data->d_cards[0] & SUIT))
		{
			ConsoleShowGameScreen(game_data, "Same suit, ball moves, new cards needed ", GAME_SPEED * 20);
			MoveLineOfScrimmage(game_data, TotalYardsOffence(game_data) - TotalYardsDefence(game_data));
			ConsoleShowGameScreen(game_data, 0, GAME_SPEED * 20);
		}
		else if ((game_data->o_cards[0] & VALUE) == (game_data->d_cards[0] & VALUE))
		{
			ConsoleShowGameScreen(game_data, "Draw, new cards needed ", GAME_SPEED * 20);
		}
		else
		{
			if ((game_data->d_cards[0] & VALUE) > (game_data->o_cards[0] & VALUE))
			{
				SwitchPossession(game_data);
				ResetLineOfFirstDown(game_data);
				game_data->down = 0;
			}

			char buf4[256];
			snprintf(buf4, sizeof(buf4), "The %s recovered the ball at the %d yard line ", GetTeamName(game_data, game_data->team_in_possession), ConvertLosToYardLine(game_data));
			ConsoleShowGameScreen(game_data, buf4, GAME_SPEED * 30);
			fumble_resolved = true;
		}
	} while (!fumble_resolved);

	if (save_team_started_with_ball != game_data->team_in_possession && SafetyScored(game_data))
	{
		// team recovers ball in their own end zone after opponents fumble
		ConsoleShowGameScreen(game_data, "Ball recovered in own end zone, touchback issued ", 0);
		ResetLineOfScrimmage(game_data);
		ResetLineOfFirstDown(game_data);
		game_data->down = 0;
	}
}

/*
* NAME:        OpenFieldRun
* DESCRIPTION: execute the play
* INPUTS:      game_data
* OUTPUTS:     none
*/
void OpenFieldRun(struct game_data* game_data, int offence_1, int defence_1)
{
	ConsoleShowGameScreen(game_data, "Open field run!", GAME_SPEED * 20);

	int ofr_o_value = (offence_1 & VALUE) - 10;

	if (ofr_o_value < 0)
		ofr_o_value = 0;

	int ofr_d_value = (defence_1 & VALUE) - 10;

	if (ofr_d_value < 0)
		ofr_d_value = 0;

	int cards_to_draw = (ofr_o_value - ofr_d_value);

	if (cards_to_draw < 0 || cards_to_draw > 3)
		printf("Error in OFR cards to draw");

	ClearAllCards(game_data);

	char buf[256];
	snprintf(buf, sizeof(buf), "Drawing %d cards...", cards_to_draw);
	ConsoleShowGameScreen(game_data, buf, GAME_SPEED * 20);

	int yards = 0;

	for (int i = 0; i < cards_to_draw; i++)
	{
		DrawCards(game_data, game_data->team_in_possession, 1, true);
		if(game_data->o_cards[i] & FACE)
		{
			ofr_o_value = (game_data->o_cards[i] & VALUE) - 10;

			if (ofr_o_value < 0)
				ofr_o_value = 0;
			cards_to_draw += ofr_o_value;
			char buf[256];
			snprintf(buf, sizeof(buf), "Face Card! %d extra cards", ofr_o_value);
			ConsoleShowGameScreen(game_data, buf, GAME_SPEED * 20);
		}
		yards = (game_data->o_cards[i] & VALUE);
		MoveLineOfScrimmage(game_data, yards);
		ConsoleShowGameScreen(game_data, 0, GAME_SPEED * 5);

		if (TouchdownScored(game_data))
			return;

		if ((game_data->o_cards[i] & VALUE) == ACE && (game_data->o_cards[i] & SUIT) == SPADES)
		{
			ConsoleShowGameScreen(game_data, "Ball fumbled", GAME_SPEED * 20);
			Fumble(game_data);
			return;
		}
	}

	char buf2[256];
	snprintf(buf2, sizeof(buf2), "OFR yards = %d  ", TotalYardsOffence(game_data));
	ConsoleShowGameScreen(game_data, buf2, GAME_SPEED * 30);

	return;
}

/*
* NAME:        PlayPass
* DESCRIPTION: execute the play
* INPUTS:      game_data
* OUTPUTS:     none
*/
void PlayPass(struct game_data* game_data)
{
	ConsoleShowGameScreen(game_data, "Pass play ", GAME_SPEED * 10);

	for (int i = 0; i < PASS_PLAY_MAX_CARDS_TO_DRAW; i++)
	{
		if (i > 0)
		{
			char buf[256];
			snprintf(buf, sizeof(buf), "Sum = %d, draw a card? y/n ", TotalYardsOffence(game_data));
			ConsoleShowGameScreen(game_data, buf, 0);

			char input[16];
			fgets(input, sizeof(input), stdin);

			if (input[0] == 'N' || input[0] == 'n')
				break;
		}

		DrawCards(game_data, game_data->team_in_possession, 1, true);

		if ((game_data->o_cards[i + 1] & VALUE) == ACE && (game_data->o_cards[i + 1] & SUIT) == SPADES)
		{
			ConsoleShowGameScreen(game_data, "Pass intercepted ", GAME_SPEED * 20);
			if (game_data->extra_point_needed)
				return;
			game_data->interception = true;
			ConsoleShowGameScreen(game_data, "Replacing card... ", GAME_SPEED * 20);
			game_data->o_cards[i + 1] = 0;
			ConsoleShowGameScreen(game_data, 0, GAME_SPEED * 10);
			DrawCards(game_data, game_data->team_in_possession, 1, true);
			break;
		}
		else if ((game_data->o_cards[i + 1] & VALUE) == (game_data->o_cards[0] & VALUE)
			&& (((game_data->o_cards[i + 1] & RED) && (game_data->o_cards[0] & RED))
				|| ((game_data->o_cards[i + 1] & BLACK) && (game_data->o_cards[0] & BLACK)))) // If it is the same rank and colour as card O1 then it is a sack
		{
			ConsoleShowGameScreen(game_data, "QB sacked ", GAME_SPEED * 20);
			ClearAllCards(game_data);
			DrawCards(game_data, game_data->team_in_possession, QB_SACK_CARDS_TO_DRAW, true);// draw for sack loss yardage

			if (((game_data->o_cards[0] & VALUE) == ACE && (game_data->o_cards[0] & SUIT) == SPADES) || ((game_data->o_cards[1] & VALUE) == ACE && (game_data->o_cards[1] & SUIT) == SPADES))
			{
				char buffer[256];
				snprintf(buffer, sizeof(buffer), "%s fumbled the ball", GetQBName(game_data));
				ConsoleShowGameScreen(game_data, buffer, GAME_SPEED * 20);
				game_data->fumble = true;
				ConsoleShowGameScreen(game_data, "Replacing card... ", GAME_SPEED * 20);
				if (((game_data->o_cards[0] & VALUE) == ACE && (game_data->o_cards[0] & SUIT) == SPADES))
				{
					game_data->o_cards[0] = game_data->o_cards[1];
				}
				game_data->o_cards[1] = 0;
				ConsoleShowGameScreen(game_data, 0, GAME_SPEED * 10);
				DrawCards(game_data, game_data->team_in_possession, 1, true);
			}

			int sack_yards = TotalYardsOffence(game_data);

			if (sack_yards % 2 == 0)
				sack_yards = -(sack_yards / 2);
			else
				sack_yards = -((sack_yards + 1) / 2);

			char buf2[256];
			snprintf(buf2, sizeof(buf2), "%d yards lost ", sack_yards);
			ConsoleShowGameScreen(game_data, buf2, 0);

			MoveLineOfScrimmage(game_data, -sack_yards);
			ResetLineOfFirstDown(game_data);

			if (game_data->fumble)
				Fumble(game_data);

			return;
		}
		else if ((game_data->o_cards[i + 1] & SUIT) == ((game_data->o_cards[0] & SUIT))) // If it is the same suit as card O1 then the pass is incomplete
		{
			char buf6[256];
			snprintf(buf6, sizeof(buf6), "Pass to %s incomplete ", GetTargetPlayerName(game_data));
			ConsoleShowGameScreen(game_data, buf6, GAME_SPEED * 20);
			ClearAllCards(game_data);
			return;
		}
	}

	DrawCards(game_data, game_data->team_not_in_possession, 1, true); // defence draws 1
	int catch_yards = TotalYardsOffence(game_data) - TotalYardsDefence(game_data);

	char buf3[256];
	if (catch_yards < 0)
	{
		if (catch_yards % 2 == 0)
			catch_yards = catch_yards / 2;
		else
			catch_yards = (catch_yards + 1) / 2;
		snprintf(buf3, sizeof(buf3), "%d yards lost ", catch_yards);
	}
	else if (game_data->interception)
		snprintf(buf3, sizeof(buf3), "Interception on a %d yard throw ", catch_yards);
	else
		snprintf(buf3, sizeof(buf3), "%d yard catch made ", catch_yards);

	ConsoleShowGameScreen(game_data, buf3, GAME_SPEED * 10);

	MoveLineOfScrimmage(game_data, catch_yards);

	if (game_data->interception)
	{
		Interception(game_data);

		if (SafetyScored(game_data))
		{
			// if intercepted in own end zone, force a touchback as otherwise they could run and fumble which causes all sorts of issues
			ConsoleShowGameScreen(game_data, "Ball caught in own end zone, touchback issued ", 0);
			ResetLineOfScrimmage(game_data);
			ResetLineOfFirstDown(game_data);
			game_data->down = 0;
			return;
		}
	}

	char buf4[256];

	if (TouchdownScored(game_data))
		snprintf(buf4, sizeof(buf4), "Ball caught in the end zone");
	else if (game_data->interception)
		snprintf(buf4, sizeof(buf4), "%s caught the ball at the %d", GetTeamName(game_data, game_data->team_in_possession), ConvertLosToYardLine(game_data));
	else
		snprintf(buf4, sizeof(buf4), "%s caught the ball at the %d", GetTargetPlayerName(game_data), ConvertLosToYardLine(game_data));
	ConsoleShowGameScreen(game_data, buf4, GAME_SPEED * 20);

	ClearAllCards(game_data);

	if (TouchdownScored(game_data))
		return;

	ConsoleShowGameScreen(game_data, "YAC draw", GAME_SPEED * 20);

	DrawCards(game_data, game_data->team_in_possession, 1, true);
	DrawCards(game_data, game_data->team_not_in_possession, 1, true);

	if (((game_data->o_cards[0] & VALUE) == ACE && (game_data->o_cards[0] & SUIT) == SPADES) || ((game_data->d_cards[0] & VALUE) == ACE && (game_data->d_cards[0] & SUIT) == SPADES))
	{
		char buffer[256];
		snprintf(buffer, sizeof(buffer), "%s fumbled the ball at the %d", GetTargetPlayerName(game_data), ConvertLosToYardLine(game_data));
		ConsoleShowGameScreen(game_data, buffer, GAME_SPEED * 20);
		game_data->fumble = true;
		ConsoleShowGameScreen(game_data, "Replacing card... ", GAME_SPEED * 20);
		if (((game_data->o_cards[0] & VALUE) == ACE && (game_data->o_cards[0] & SUIT) == SPADES))
		{
			game_data->o_cards[0] = 0;
			ConsoleShowGameScreen(game_data, 0, GAME_SPEED * 10);
			DrawCards(game_data, game_data->team_in_possession, 1, true);
		}
		else
		{
			game_data->d_cards[0] = 0;
			ConsoleShowGameScreen(game_data, 0, GAME_SPEED * 10);
			DrawCards(game_data, game_data->team_not_in_possession, 1, true);
		}

		Fumble(game_data);
	}

	int yac_yards = TotalYardsOffence(game_data) - TotalYardsDefence(game_data);

	if (yac_yards < 0)
		yac_yards = 0;

	MoveLineOfScrimmage(game_data, yac_yards);

	if (TouchdownScored(game_data))
	{
		if (game_data->fumble)
			ConsoleShowGameScreen(game_data, "End zone reached before fumble! ", GAME_SPEED * 20);
		return;
	}
	
	if (game_data->fumble)
	{	
		Fumble(game_data);
		return;
	}

	if ((game_data->o_cards[0] & FACE) && ((game_data->o_cards[0] & VALUE) > (game_data->d_cards[0] & VALUE)))
	{
		OpenFieldRun(game_data, game_data->o_cards[0], game_data->d_cards[0]);
	}
	else
	{
		char buf5[256];
		snprintf(buf5, sizeof(buf5), "%d yards after catch ", yac_yards);
		ConsoleShowGameScreen(game_data, buf5, GAME_SPEED * 10);
	}
}

/*
* NAME:        PlayRun
* DESCRIPTION: execute the play
* INPUTS:      game_data
* OUTPUTS:     none
*/
void PlayRun(struct game_data* game_data)
{
	ConsoleShowGameScreen(game_data, "Run play ", GAME_SPEED * 10);

	DrawCards(game_data, game_data->team_in_possession, RUN_PLAY_OFFENCE_CARDS_TO_DRAW, true);
	DrawCards(game_data, game_data->team_not_in_possession, RUN_PLAY_DEFENCE_CARDS_TO_DRAW, true);
	
	if (((game_data->o_cards[1] & VALUE) == ACE && (game_data->o_cards[1] & SUIT) == SPADES) || ((game_data->d_cards[1] & VALUE) == ACE && (game_data->d_cards[1] & SUIT) == SPADES)) // If O2 OR D2 is the Ace of spades then draw a replacement for that card and then go to Fumble
	{
		ConsoleShowGameScreen(game_data, "Ball fumbled ", GAME_SPEED * 20);
		game_data->fumble = true;
		ConsoleShowGameScreen(game_data, "Replacing card... ", GAME_SPEED * 20);
		if (((game_data->o_cards[1] & VALUE) == ACE && (game_data->o_cards[1] & SUIT) == SPADES))
		{
			game_data->o_cards[1] = 0;
			ConsoleShowGameScreen(game_data, 0, GAME_SPEED * 10);
			DrawCards(game_data, game_data->team_in_possession, 1, true);
		}
		else if ((game_data->d_cards[1] & VALUE) == ACE && (game_data->d_cards[1] & SUIT) == SPADES)
		{
		game_data->d_cards[1] = 0;
		ConsoleShowGameScreen(game_data, 0, GAME_SPEED * 10);
		DrawCards(game_data, game_data->team_not_in_possession, 1, true);
		}
	}

	int yards_gained = 0;
	int difference = TotalYardsOffence(game_data) - TotalYardsDefence(game_data);

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

	MoveLineOfScrimmage(game_data, yards_gained);
	
	if (game_data->fumble)
	{
		if (TouchdownScored(game_data))
			return;

		char buf[256];
		snprintf(buf, sizeof(buf), "%d yards gained before %s fumbled", yards_gained, GetTargetPlayerName(game_data));
		ConsoleShowGameScreen(game_data, buf, GAME_SPEED * 30);
		Fumble(game_data);
		return;
	}

	char buf[256];
	snprintf(buf, sizeof(buf), "%s gained %d yards ", GetTargetPlayerName(game_data), yards_gained);
	ConsoleShowGameScreen(game_data, buf, GAME_SPEED * 30);

	// if O2 is a face card and larger than D2 the go to Open field run
	if ((game_data->o_cards[RUN_PLAY_OFFENCE_CARDS_TO_DRAW] & FACE) && ((game_data->o_cards[RUN_PLAY_OFFENCE_CARDS_TO_DRAW] & VALUE) > (game_data->d_cards[RUN_PLAY_DEFENCE_CARDS_TO_DRAW] & VALUE)))
	{
		OpenFieldRun(game_data, game_data->o_cards[RUN_PLAY_OFFENCE_CARDS_TO_DRAW], game_data->d_cards[RUN_PLAY_DEFENCE_CARDS_TO_DRAW]);
	}
}

/*
* NAME:        PlayTwoPointConversion
* DESCRIPTION: execute the play
* INPUTS:      game_data
* OUTPUTS:     none
*/
void PlayTwoPointConversion(struct game_data* game_data)
{
	if (GetDirectionOfPlay(game_data) == 1)
	{
		game_data->line_of_scrimmage = END_ZONE_WIDTH + FIELD_WIDTH - TWO_POINT_CONVERSION_YARDS;
	}
	else
	{
		game_data->line_of_scrimmage = END_ZONE_WIDTH + TWO_POINT_CONVERSION_YARDS;
	}

	PlayNormal(game_data); // not fully correct because in a conversion a pick 6 cant happen but it can in PlayNormal, need to guard for this
}

/*
* NAME:        GetFormation
* DESCRIPTION: ask for and set the formation for the play
* INPUTS:      game_data
* OUTPUTS:     int
*/
int GetFormation(struct game_data* game_data)
{
	// []       []  []       []
	// 
	// []         []     []  []
	// 
	// []  []            []  []
	ConsoleShowGameScreen(game_data, "Pick formation (RB-WR) 2:2 or 1:3 or 0:4 (1 - 3): ", GAME_SPEED * 10);

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
* INPUTS:      game_data
* OUTPUTS:     int
*/
int GetMatchupNumber(struct game_data* game_data)
{
	char buf[256];
	snprintf(buf, sizeof(buf), "%s, pick matchup to play (1 - 4): ", GetTeamName(game_data, game_data->team_in_possession));
	ConsoleShowGameScreen(game_data, buf, 0);

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
* INPUTS:      game_data, offence_or_defence (1-2)
* OUTPUTS:     none
*/
void PutCardsInFormation(struct game_data* game_data, int offence_or_defence)
{
	int pick = 0;
	bool retry = false;
	for (int i = 0; i < NORMAL_PLAY_CARDS_TO_DRAW; i++)
	{	
		do {
			retry = false;
			char buf[256];
			snprintf(buf, sizeof(buf), "What position (1 - %d) to place card %d: ", NORMAL_PLAY_CARDS_TO_DRAW, i + 1);
			ConsoleShowGameScreen(game_data, buf, GAME_SPEED * 1);

			char input[16];
			fgets(input, sizeof(input), stdin);
			pick = atoi(input);

			if (pick < 1 || pick > NORMAL_PLAY_CARDS_TO_DRAW || (offence_or_defence == 1 && game_data->o_cards_in_position[pick - 1]) || (offence_or_defence == 2 && game_data->d_cards_in_position[pick - 1]))
			{
				retry = true;
				ConsoleShowGameScreen(game_data, "Invalid position chosen, choose again.", GAME_SPEED * 20);
			}

		} while (retry);
		if(offence_or_defence == 1)
			game_data->o_cards_in_position[pick - 1] = game_data->o_cards[i];
		else if (offence_or_defence == 2)
			game_data->d_cards_in_position[pick - 1] = game_data->d_cards[i];
		ConsoleShowGameScreen(game_data, 0, GAME_SPEED * 1);
	}
}

/*
* NAME:        SetRunOrPassPlay
* DESCRIPTION: record if the play is a run or a pass
* INPUTS:      game_data
* OUTPUTS:     none
*/
void SetRunOrPassPlay(struct game_data* game_data)
{
	// []       []  []       []
	// 
	// []         []     []  []
	// 
	// []  []            []  []

	if (game_data->formation == 1)
	{
		if(game_data->matchup_number == 2 || game_data->matchup_number == 3)
		{
			game_data->pass_play = 0;
			game_data->run_play = 1;
		}
		else
		{
			game_data->pass_play = 1;
			game_data->run_play = 0;
		}
	}
	else if (game_data->formation == 2)
	{
		if (game_data->matchup_number == 2)
		{
			game_data->pass_play = 0;
			game_data->run_play = 1;
		}
		else
		{
			game_data->pass_play = 1;
			game_data->run_play = 0;
		}
	}
	else if (game_data->formation == 3)
	{
		game_data->pass_play = 1;
		game_data->run_play = 0;
	}
}

/*
* NAME:        PlayNormal
* DESCRIPTION: execute the play
* INPUTS:      game_data
* OUTPUTS:     none
*/
void PlayNormal(struct game_data* game_data)
{
	char buf[256];
	snprintf(buf, sizeof(buf), "%s, press enter to draw", GetTeamName(game_data, game_data->team_in_possession));
	ConsoleShowGameScreen(game_data, buf, 0);

	char input[16];
	fgets(input, sizeof(input), stdin);

	DrawCards(game_data, game_data->team_in_possession, NORMAL_PLAY_CARDS_TO_DRAW, false); // O: draw 4 cards

	game_data->formation = GetFormation(game_data); // O: pick layout/formation/personnel
	game_data->show_O_D_Both = 1;

	ConsoleShowGameScreen(game_data, 0, GAME_SPEED * 10); // show formation with blanks ready for cards to be put in

	PutCardsInFormation(game_data, 1); // Offence place cards in positions, send 1 for offence
	
	game_data->show_O_D_Both = 0; // Cover offence cards
	
	char buf2[256];
	snprintf(buf2, sizeof(buf2), "%s, press enter to draw", GetTeamName(game_data, game_data->team_not_in_possession));
	ConsoleShowGameScreen(game_data, buf2, 0);

	char input2[16];
	fgets(input2, sizeof(input2), stdin);

	int save_formation = game_data->formation;
	game_data->formation = 0;
	ClearOCards(game_data);

	game_data->show_O_D_Both = 2; // show D cards
	DrawCards(game_data, game_data->team_not_in_possession, NORMAL_PLAY_CARDS_TO_DRAW, false); // D: draw 4 cards

	game_data->formation = save_formation;
	PutCardsInFormation(game_data, 2); // Defence place cards in positions, send 1 for Defence
	
	game_data->show_O_D_Both = 0; // cover both sets of cards
	ConsoleShowGameScreen(game_data, "Press enter to reveal matchups", GAME_SPEED * 10);
	char input3[16];
	fgets(input3, sizeof(input3), stdin);

	game_data->show_O_D_Both = 3;

	// save off cards
	int save_o_cards[] = { 0,0,0,0 };
	int save_d_cards[] = { 0,0,0,0 };
	
	for (int i = 0; i < NORMAL_PLAY_CARDS_TO_DRAW; i++)
	{
		save_o_cards[i] = game_data->o_cards_in_position[i];
		save_d_cards[i] = game_data->d_cards_in_position[i];
		game_data->o_cards_in_position[i] = 0;
		game_data->d_cards_in_position[i] = 0;
	}
	ConsoleShowGameScreen(game_data, 0, GAME_SPEED * 10);
	for (int i = 0; i < NORMAL_PLAY_CARDS_TO_DRAW; i++)
	{
		game_data->o_cards_in_position[i] = save_o_cards[i];
		game_data->d_cards_in_position[i] = save_d_cards[i];
		ConsoleShowGameScreen(game_data, 0, GAME_SPEED * 10);
	}

	game_data->matchup_number = GetMatchupNumber(game_data); // Show all 8 cards and "O: pick which of the 4 match ups to play"
	
	SetRunOrPassPlay(game_data);

	// meter here? player being targetted/given the ball
	
	// store 2 head to head cards
	int O1 = game_data->o_cards_in_position[game_data->matchup_number - 1];
	int D1 = game_data->d_cards_in_position[game_data->matchup_number - 1];

	ClearAllCards(game_data);

	game_data->o_cards[0] = O1;
	game_data->d_cards[0] = D1;
	
	ConsoleShowGameScreen(game_data, 0, GAME_SPEED * 10);

	if(game_data->pass_play)
		PlayPass(game_data);
	else if (game_data->run_play)
		PlayRun(game_data);
}

/*
* NAME:        GetPuntCardsToRedraw
* DESCRIPTION: ask and return how many cards to draw in the punt redraw
* INPUTS:      game_data
* OUTPUTS:     int
*/
int GetPuntCardsToRedraw(struct game_data* game_data) {

	char buf[256];
	snprintf(buf, sizeof(buf), "Current total Yards: %d  How many to redraw? (0 - %d): ", TotalYardsOffence(game_data), PUNT_REDRAW_MAX_CARDS);
	ConsoleShowGameScreen(game_data, buf, GAME_SPEED * 2);

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
* INPUTS:      game_data
* OUTPUTS:     int
*/
int GetPuntCardsToDraw(struct game_data* game_data) {
	char buf[256];
	snprintf(buf, sizeof(buf), "How many cards do you want to draw? (1 - %d): ", PUNT_MAX_CARDS);
	ConsoleShowGameScreen(game_data, buf, GAME_SPEED * 2);

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
* INPUTS:      game_data
* OUTPUTS:     int
*/
int GetFieldGoalCardsToRedraw(struct game_data* game_data) {

	char buf[256];
	snprintf(buf, sizeof(buf), "Current total Yards: %d  How many to redraw? (0 - %d): ", TotalYardsOffence(game_data), FIELD_GOAL_REDRAW_MAX_CARDS);
	ConsoleShowGameScreen(game_data, buf, GAME_SPEED * 2);

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
* INPUTS:      game_data
* OUTPUTS:     int
*/
int GetFieldGoalCardsToDraw(struct game_data* game_data) {
	//char buf[256];
	//snprintf(buf, sizeof(buf), "How many cards do you want to draw? (1 - %d): ", FIELD_GOAL_MAX_CARDS);
	//ConsoleShowGameScreen(game_data, buf, GAME_SPEED * 2);
	//
	int num = 0;
	//char input[16];
	//if (fgets(input, sizeof(input), stdin)) {
	//	num = atoi(input);
	//}
	//
	//if (num > FIELD_GOAL_MAX_CARDS)
	//	num = FIELD_GOAL_MAX_CARDS;
	//else if (num < 1)
	//	num = 1;

	num = 6; // why would anyone not want to draw all 6 cards?
	return num;
}

/*
* NAME:        TouchBack
* DESCRIPTION: ask for touchback
* INPUTS:      game_data
* OUTPUTS:     bool
*/
bool TouchBack(struct game_data* game_data)
{
	ConsoleShowGameScreen(game_data, "Would you like to action a touchback? y/n ", 0);
	char input[16];
	fgets(input, sizeof(input), stdin);

	if (input[0] == 'N' || input[0] == 'n')
		return false;

	return true;
}

/*
* NAME:        PlayPuntReturn
* DESCRIPTION: execute the play
* INPUTS:      game_data
* OUTPUTS:     none
*/
void PlayPuntReturn(struct game_data* game_data)
{
	ClearAllCards(game_data);

	// if the kick is received in the endzone, the receiver can declare a touchback
	if (game_data->line_of_scrimmage <= END_ZONE_WIDTH || game_data->line_of_scrimmage >= (END_ZONE_WIDTH + FIELD_WIDTH))
	{
		if (TouchBack(game_data))
		{
			ResetLineOfScrimmage(game_data);
			ResetLineOfFirstDown(game_data);
			game_data->down = 0;
			return;
		}
	}

	ConsoleShowGameScreen(game_data, "Punt return...", GAME_SPEED * 20);

	// Receiver draws two cards and the punter draws one card from the shared deck
	DrawCards(game_data, game_data->team_in_possession, PUNT_RETURN_OFFENCE_CARDS_TO_DRAW, true);
	DrawCards(game_data, game_data->team_not_in_possession, PUNT_RETURN_DEFENCE_CARDS_TO_DRAW, true);
	
	// (two received draws - one punter draw) = (O1 + O2) - D1 = yardage returned
	int yards = TotalYardsOffence(game_data) - TotalYardsDefence(game_data);
	if (yards < 0)
		yards = 0;

	char buf[256];
	snprintf(buf, sizeof(buf), "Return yards = %d  ", yards);
	ConsoleShowGameScreen(game_data, buf, GAME_SPEED * 30);

	MoveLineOfScrimmage(game_data, yards); // move line of scrimmage 

	ConsoleShowGameScreen(game_data, 0, GAME_SPEED * 20);

	// if O2 is a face card and larger than D1 the go to Open field run
	if ((game_data->o_cards[PUNT_RETURN_OFFENCE_CARDS_TO_DRAW - 1] & FACE) && ((game_data->o_cards[PUNT_RETURN_OFFENCE_CARDS_TO_DRAW - 1] & VALUE) > (game_data->d_cards[PUNT_RETURN_DEFENCE_CARDS_TO_DRAW - 1] & VALUE)))
	{
		OpenFieldRun(game_data, game_data->o_cards[PUNT_RETURN_OFFENCE_CARDS_TO_DRAW - 1], game_data->d_cards[PUNT_RETURN_DEFENCE_CARDS_TO_DRAW - 1]);
	}

	game_data->down = 0;

	ResetLineOfFirstDown(game_data);
}

/*
* NAME:        ConvertLosToYardLine
* DESCRIPTION: los to yard line, intake LoS (0 - 110), output 0 - 50 yard line on pitch
* INPUTS:      game_data
* OUTPUTS:     int
*/
int ConvertLosToYardLine(struct game_data* game_data)
{
	if (game_data->line_of_scrimmage > ((FIELD_WIDTH / 2) + END_ZONE_WIDTH))
	{
		return (FIELD_WIDTH - (game_data->line_of_scrimmage - END_ZONE_WIDTH));
	}
	else
	{
		return (game_data->line_of_scrimmage - END_ZONE_WIDTH);
	}
}

/*
* NAME:        GetQBName
* DESCRIPTION: get qn name in possession
* INPUTS:      game_data
* OUTPUTS:     name
*/
const char* GetQBName(struct game_data* game_data)
{
	if (game_data->team_in_possession == 1)
		return game_data->team_1_qb_names[0];
	else
		return game_data->team_2_qb_names[0];
}

/*
* NAME:        GetTargetPlayerName
* DESCRIPTION: return the name of the player involved in this play
* INPUTS:      game_data
* OUTPUTS:     name
*/
const char* GetTargetPlayerName(struct game_data* game_data)
{
	// []       []  []       []
	// 
	// []         []     []  []
	// 
	// []  []            []  []
	
	if (game_data->team_in_possession == 1)
	{
		if (game_data->matchup_number == 1)
			return game_data->team_1_wr_names[0];
		else if ((game_data->formation == 1 && game_data->matchup_number == 4) || (game_data->formation == 2 && game_data->matchup_number == 3) || (game_data->formation == 3 && game_data->matchup_number == 2))
			return game_data->team_1_wr_names[1];
		else if ((game_data->formation == 2 && game_data->matchup_number == 4) || (game_data->formation == 3 && game_data->matchup_number == 3))
			return game_data->team_1_wr_names[2];
		else if ((game_data->formation == 3 && game_data->matchup_number == 4))
			return game_data->team_1_wr_names[3];
		else if ((game_data->formation == 1 && game_data->matchup_number == 2) || (game_data->formation == 2 && game_data->matchup_number == 2))
			return game_data->team_1_rb_names[0];
		else if ((game_data->formation == 1 && game_data->matchup_number == 3))
			return game_data->team_1_rb_names[1];
	}
	else
	{
		if (game_data->matchup_number == 1)
			return game_data->team_2_wr_names[0];
		else if ((game_data->formation == 1 && game_data->matchup_number == 4) || (game_data->formation == 2 && game_data->matchup_number == 3) || (game_data->formation == 3 && game_data->matchup_number == 2))
			return game_data->team_2_wr_names[1];
		else if ((game_data->formation == 2 && game_data->matchup_number == 4) || (game_data->formation == 3 && game_data->matchup_number == 3))
			return game_data->team_2_wr_names[2];
		else if ((game_data->formation == 3 && game_data->matchup_number == 4))
			return game_data->team_2_wr_names[3];
		else if ((game_data->formation == 1 && game_data->matchup_number == 2) || (game_data->formation == 2 && game_data->matchup_number == 2))
			return game_data->team_2_rb_names[0];
		else if ((game_data->formation == 1 && game_data->matchup_number == 3))
			return game_data->team_2_rb_names[1];
	}
	return -1;
}

/*
* NAME:        PlayPunt
* DESCRIPTION: execute the play
* INPUTS:      game_data
* OUTPUTS:     none
*/
void PlayPunt(struct game_data* game_data)
{
	ConsoleShowGameScreen(game_data, "Punting...", GAME_SPEED * 20); // announce punt

	DrawCards(game_data, game_data->team_in_possession, GetPuntCardsToDraw(game_data), true);// draw 1-5 cards

	RedrawCards(game_data, game_data->team_in_possession, GetPuntCardsToRedraw(game_data));// redraw 0-2 cards, get player choice on high or low ones

	int total_yards = TotalYardsOffence(game_data);
	char buf[256];
	snprintf(buf, sizeof(buf), "Total Yards: %d", total_yards);
	ConsoleShowGameScreen(game_data, buf, GAME_SPEED * 10);

	MoveLineOfScrimmage(game_data, total_yards);

	char buf2[256];
	if ((GetDirectionOfPlay(game_data) == 1) && (game_data->line_of_scrimmage >= (END_ZONE_WIDTH + FIELD_WIDTH)))
	{
		snprintf(buf2, sizeof(buf2), "Ball caught %d yards into the end zone", abs(ConvertLosToYardLine(game_data)));
	}
	else if ((GetDirectionOfPlay(game_data) == 2) && (game_data->line_of_scrimmage <= END_ZONE_WIDTH))
	{
		snprintf(buf2, sizeof(buf2), "Ball caught %d yards into the end zone", abs(ConvertLosToYardLine(game_data)));
	}	
	else
		snprintf(buf2, sizeof(buf2), "Ball caught at the %d", ConvertLosToYardLine(game_data));

	ConsoleShowGameScreen(game_data, buf2, GAME_SPEED * 20);

	SwitchPossession(game_data);

	ConsoleShowGameScreen(game_data, 0, GAME_SPEED * 10);

	PlayPuntReturn(game_data);
}

/*
* NAME:        SetLosExtraPoint
* DESCRIPTION: get field distance to fg
* INPUTS:      game_data
* OUTPUTS:     none
*/
void SetLosExtraPoint(struct game_data* game_data)
{
	if (GetDirectionOfPlay(game_data) == 1)
	{
		game_data->line_of_scrimmage = END_ZONE_WIDTH + FIELD_WIDTH - EXTRA_POINT_YARDAGE_FROM_END_ZONE;
	}
	else
	{
		game_data->line_of_scrimmage = END_ZONE_WIDTH + EXTRA_POINT_YARDAGE_FROM_END_ZONE;
	}
}

/*
* NAME:        GetFieldGoalDistance
* DESCRIPTION: get field distance to fg
* INPUTS:      game_data
* OUTPUTS:     distance
*/
int GetFieldGoalDistance(struct game_data* game_data)
{
	int distance;

	if (GetDirectionOfPlay(game_data) == 1)
	{
		distance = (FIELD_WIDTH - (game_data->line_of_scrimmage - END_ZONE_WIDTH)) + FIELD_GOAL_YARDS_AFTER_END_ZONE;
	}
	else
	{
		distance = (game_data->line_of_scrimmage - END_ZONE_WIDTH) + FIELD_GOAL_YARDS_AFTER_END_ZONE;
	}
	return distance;
}

/*
* NAME:        ResetForKickoff
* DESCRIPTION: set field line of scrimmage and first down ready for kickoff
* INPUTS:      game_data
* OUTPUTS:     none
*/
void ResetForKickoff(struct game_data* game_data)
{
	game_data->kickoff_needed = true;
	game_data->down = 0;

	if (GetDirectionOfPlay(game_data) == 1)
	{
		game_data->line_of_scrimmage = STARTING_LINE_OF_SCRIMMAGE + END_ZONE_WIDTH;
		game_data->line_of_first_down = game_data->line_of_scrimmage + YARDS_PER_FIRST_DOWN;
	}
	else
	{
		game_data->line_of_scrimmage = FIELD_WIDTH - STARTING_LINE_OF_SCRIMMAGE + END_ZONE_WIDTH;
		game_data->line_of_first_down = game_data->line_of_scrimmage - YARDS_PER_FIRST_DOWN;
	}
}

/*
* NAME:        TurnoverOnDowns
* DESCRIPTION: switch team in possession
* INPUTS:      game_data
* OUTPUTS:     none
*/
void TurnoverOnDowns(struct game_data* game_data, bool field_goal)
{
	SwitchPossession(game_data);
	if(field_goal)
		MoveLineOfScrimmage(game_data, FIELD_GOAL_MISS_PENALTY_YARDS);
	ResetLineOfFirstDown(game_data);
	game_data->down = 0;
}

/*
* NAME:        PlayExtraPoint
* DESCRIPTION: execute the play
* INPUTS:      game_data
* OUTPUTS:     none
*/
void PlayExtraPoint(struct game_data* game_data)
{
	int field_goal_distance = EXTRA_POINT_YARDAGE_FROM_END_ZONE + FIELD_GOAL_YARDS_AFTER_END_ZONE;
	SetLosExtraPoint(game_data);
	ResetLineOfFirstDown(game_data);

	char buf[256];
	snprintf(buf, sizeof(buf), "Attempting extra point. %d yards needed. ", field_goal_distance);
	ConsoleShowGameScreen(game_data, buf, GAME_SPEED * 20);

	DrawCards(game_data, game_data->team_in_possession, GetFieldGoalCardsToDraw(game_data), true);// draw 1-5 cards

	int total_yards = TotalYardsOffence(game_data);

	if (total_yards < field_goal_distance)
		RedrawCards(game_data, game_data->team_in_possession, GetFieldGoalCardsToRedraw(game_data));// redraw 0-2 cards, get player choice on high or low ones

	total_yards = TotalYardsOffence(game_data);

	char buf2[256];
	snprintf(buf2, sizeof(buf2), "Total distance: %d yards", total_yards);
	ConsoleShowGameScreen(game_data, buf2, GAME_SPEED * 20);

	if (total_yards >= field_goal_distance)
	{
		ConsoleShowGameScreen(game_data, "Extra Point Scored!", GAME_SPEED * 50); // resolve, scored
		if (game_data->team_in_possession == 1)
		{
			game_data->team_1_score += EXTRA_POINT_POINTS;
			game_data->quarter_scores[0][game_data->quarter - 1] += EXTRA_POINT_POINTS;
		}
		else if (game_data->team_in_possession == 2)
		{
			game_data->team_2_score += EXTRA_POINT_POINTS;
			game_data->quarter_scores[1][game_data->quarter - 1] += EXTRA_POINT_POINTS;
		}
	}
	else if (total_yards < field_goal_distance)
	{
		ConsoleShowGameScreen(game_data, "Missed Extra Point", GAME_SPEED * 20);
	}
	
	ResetForKickoff(game_data);

	game_data->down = 0;
	game_data->plays_remaining_in_quarter++; // should not take a play clock play so add one back
	game_data->extra_point_needed = false;
}

/*
* NAME:        PlayFieldGoal
* DESCRIPTION: execute the play
* INPUTS:      game_data
* OUTPUTS:     none
*/
void PlayFieldGoal(struct game_data* game_data)
{
	int field_goal_distance = GetFieldGoalDistance(game_data);

	char buf[256];
	snprintf(buf, sizeof(buf), "Attempting a %d yard field goal.", field_goal_distance);
	ConsoleShowGameScreen(game_data, buf, GAME_SPEED * 20);

	DrawCards(game_data, game_data->team_in_possession, GetFieldGoalCardsToDraw(game_data), true);// draw 1-5 cards

	int total_yards = TotalYardsOffence(game_data);
	
	if (total_yards < field_goal_distance)
		RedrawCards(game_data, game_data->team_in_possession, GetFieldGoalCardsToRedraw(game_data));// redraw 0-2 cards, get player choice on high or low ones

	total_yards = TotalYardsOffence(game_data);

	char buf2[256];
	snprintf(buf2, sizeof(buf2), "Total distance: %d yards", total_yards);
	ConsoleShowGameScreen(game_data, buf2, GAME_SPEED * 20);

	if (total_yards >= field_goal_distance)
	{
		ConsoleShowGameScreen(game_data, "Field Goal Scored!", GAME_SPEED * 50); // resolve, scored
		if (game_data->team_in_possession == 1) {
			game_data->team_1_score += FIELD_GOAL_POINTS;
			game_data->quarter_scores[0][game_data->quarter - 1] += FIELD_GOAL_POINTS;
		}
		else if (game_data->team_in_possession == 2) {
			game_data->team_2_score += FIELD_GOAL_POINTS;
			game_data->quarter_scores[1][game_data->quarter - 1] += FIELD_GOAL_POINTS;
		}
		ResetForKickoff(game_data);
	}
	else if (total_yards < field_goal_distance)
	{
		TurnoverOnDowns(game_data, true); // turnover at line of scrimmage + 10 if missed
		ConsoleShowGameScreen(game_data, "Missed field goal, turnover at LoS + 10", GAME_SPEED * 20);
	}
}

/*
* NAME:        PlayKneel
* DESCRIPTION: execute the play
* INPUTS:      game_data
* OUTPUTS:     none
*/
void PlayKneel(struct game_data* game_data)
{
	ConsoleShowGameScreen(game_data, "QB Kneel executed. Play over.", GAME_SPEED * 20);
}

/*
* NAME:        PlayTimeout
* DESCRIPTION: execute the play
* INPUTS:      game_data
* OUTPUTS:     bool need to pick different play
*/
bool PlayTimeout(struct game_data* game_data)
{
	// find which team wants to call the timeout, if they have 0 left return true
	int team = GetTimeoutTeam(game_data);
	
	if (team == 1 && !game_data->team_1_timeouts_remaining)
		return true;
	else if (team == 2 && !game_data->team_2_timeouts_remaining)
		return true;
	
	char buf[256];
	snprintf(buf, sizeof(buf), "%s timeout used", GetTeamName(game_data, team));
	ConsoleShowGameScreen(game_data, buf, GAME_SPEED * 20);

	//

	if (team == 1)
		game_data->team_1_timeouts_remaining--;
	else
		game_data->team_2_timeouts_remaining--;

	for (int i = 0; i < 5; i++)
	{
		if (team == 1)
			game_data->team_1_timeouts_remaining++;
		else
			game_data->team_2_timeouts_remaining++;

		ConsoleShowGameScreen(game_data, 0, ANIMATE_SPEED);

		if (team == 1)
			game_data->team_1_timeouts_remaining--;
		else
			game_data->team_2_timeouts_remaining--;

		ConsoleShowGameScreen(game_data, 0, ANIMATE_SPEED);
	}

	ConsoleShowGameScreen(game_data, 0, GAME_SPEED * 10);
	game_data->plays_remaining_in_quarter += 2;
	game_data->down -= 1;
	return false;
}

/*
* NAME:        GetTimeoutTeam
* DESCRIPTION: get input from player for which team called the timeout
* INPUTS:      game_data
* OUTPUTS:     none
*/
int GetTimeoutTeam(struct game_data* game_data)
{
	int team = 0;
	ConsoleShowGameScreen(game_data, "Which team called timeout? (1 - 2): ", GAME_SPEED * 2);
	char input[16];
	if (fgets(input, sizeof(input), stdin)) {
		team = atoi(input);
	}

	if (team < 1)
		team = 1;
	else if (team > 2)
		team = 2;
	return team;
}

/*
* NAME:        GetPlayType
* DESCRIPTION: get input from player for play type
* INPUTS:      game_data
* OUTPUTS:     none
*/
int GetPlayType(struct game_data* game_data)
{
	int play_type = 0;
	ConsoleShowGameScreen(game_data, "Pick game type (1 - 5): ", GAME_SPEED * 2);
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
const char* GetTeamName(struct game_data* game_data, int team)
{
	if (team == 1)
		return game_data->team_1_name;
	else
		return game_data->team_2_name;
}

/*
* NAME:        AnnounceDown
* DESCRIPTION: display current down
* INPUTS:      game_data
* OUTPUTS:     none
*/
void AnnounceDown(struct game_data* game_data)
{
	char buf[256];
	size_t len = 0;

	len += snprintf(buf + len, sizeof(buf) - len, "%s ", GetTeamName(game_data, game_data->team_in_possession));

	const char* down_str;
	switch (game_data->down) {
	case 1: down_str = "1st"; break;
	case 2: down_str = "2nd"; break;
	case 3: down_str = "3rd"; break;
	case 4: down_str = "4th"; break;
	default: down_str = "Error"; break;
	}

	len += snprintf(buf + len, sizeof(buf) - len, "%s down", down_str);

	ConsoleShowGameScreen(game_data, buf, GAME_SPEED * 20);
}

/*
* NAME:        GetDirectionOfPlay
* DESCRIPTION: return 1-2 if team with ball is going right or left
* INPUTS:      game_data
* OUTPUTS:     1-2 right or left
*/
int GetDirectionOfPlay(struct game_data* game_data)
{
	return game_data->team_in_possession == 1 ? game_data->team_1_direction : game_data->team_2_direction;
}

/*
* NAME:        TouchdownScored
* DESCRIPTION: check if we need to award a touchdown
* INPUTS:      game_data
* OUTPUTS:     bool
*/
bool TouchdownScored(struct game_data* game_data)
{
	// check if touchdown, make sure not a safety
	if ((GetDirectionOfPlay(game_data) == 1) && (game_data->line_of_scrimmage >= (END_ZONE_WIDTH + FIELD_WIDTH)))
	{
		return true;
	}
	else if ((GetDirectionOfPlay(game_data) == 2) && (game_data->line_of_scrimmage <= END_ZONE_WIDTH))
	{
		return true;
	}
	else
		return false;
}

/*
* NAME:        OutBackOfEndZone
* DESCRIPTION: check if line of scrimmage is out the back of the end zone
* INPUTS:      game_data
* OUTPUTS:     bool
*/
bool OutBackOfEndZone(struct game_data* game_data)
{
	if ((GetDirectionOfPlay(game_data) == 1) && (game_data->line_of_scrimmage >= (END_ZONE_WIDTH + FIELD_WIDTH + END_ZONE_WIDTH)))
	{
		return true;
	}
	else if ((GetDirectionOfPlay(game_data) == 2) && (game_data->line_of_scrimmage <= 0))
	{
		return true;
	}
	else
		return false;
}


/*
* NAME:        SafetyScored
* DESCRIPTION: check if we need to award a safety
* INPUTS:      game_data
* OUTPUTS:     bool
*/
bool SafetyScored(struct game_data* game_data)
{
	if ((game_data->play_type != PLAY_TYPE_NORMAL) && (game_data->play_type != PLAY_TYPE_PUNT))
	{
		return false;
	}
	else if ((GetDirectionOfPlay(game_data) == 1) && (game_data->line_of_scrimmage <= END_ZONE_WIDTH))
	{
		return true;
	}
	else if ((GetDirectionOfPlay(game_data) == 2) && (game_data->line_of_scrimmage >= (END_ZONE_WIDTH + FIELD_WIDTH)))
	{
		return true;
	}
	else
		return false;
}

/*
* NAME:        FirstDownScored
* DESCRIPTION: check if we need to award a first down
* INPUTS:      game_data
* OUTPUTS:     bool
*/
bool FirstDownScored(struct game_data* game_data)
{
	if ((GetDirectionOfPlay(game_data) == 1) && (game_data->line_of_scrimmage >= game_data->line_of_first_down))
	{
		return true;
	}
	else if ((GetDirectionOfPlay(game_data) == 2) && (game_data->line_of_scrimmage <= game_data->line_of_first_down))
	{
		return true;
	}
	
	return false;
}

/*
* NAME:        AwardTwoPointConversion
* DESCRIPTION: award 2 pts
* INPUTS:      game_data
* OUTPUTS:     none
*/
void AwardTwoPointConversion(struct game_data* game_data)
{
	ConsoleShowGameScreen(game_data, "2 Point Conversion Scored!", GAME_SPEED * 50); // resolve, scored
	if (game_data->team_in_possession == 1){
		game_data->team_1_score += TWO_POINT_CONVERSION_POINTS;
		game_data->quarter_scores[0][game_data->quarter - 1] += TWO_POINT_CONVERSION_POINTS;
	}
	else if (game_data->team_in_possession == 2){
		game_data->team_2_score += TWO_POINT_CONVERSION_POINTS;
		game_data->quarter_scores[1][game_data->quarter - 1] += TWO_POINT_CONVERSION_POINTS;
	}
}

/*
* NAME:        AwardTouchdown
* DESCRIPTION: award a touchdown
* INPUTS:      game_data
* OUTPUTS:     none
*/
void AwardTouchdown(struct game_data* game_data)
{
	if(game_data->team_in_possession == 1){
		game_data->team_1_score += TOUCHDOWN_POINTS;
		game_data->quarter_scores[0][game_data->quarter - 1] += TOUCHDOWN_POINTS;
	}
	else if (game_data->team_in_possession == 2){
		game_data->team_2_score += TOUCHDOWN_POINTS;
		game_data->quarter_scores[1][game_data->quarter - 1] += TOUCHDOWN_POINTS;
	}

	game_data->extra_point_needed = true;
	
	char buf[256];
	if(game_data->play_type == PLAY_TYPE_NORMAL)
		snprintf(buf, sizeof(buf), "Touch Down Scored by %s!", GetTargetPlayerName(game_data));
	else
		snprintf(buf, sizeof(buf), "Touch Down Scored");
	ConsoleShowGameScreen(game_data, buf, GAME_SPEED * 50);
}

/*
* NAME:        AwardSafety
* DESCRIPTION: award a safety
* INPUTS:      game_data
* OUTPUTS:     none
*/
void AwardSafety(struct game_data* game_data)
{
	if (game_data->team_in_possession == 1){
		game_data->team_2_score += SAFETY_POINTS;
		game_data->quarter_scores[1][game_data->quarter - 1] += SAFETY_POINTS;
	}
	else if (game_data->team_in_possession == 2){
		game_data->team_1_score += SAFETY_POINTS;
		game_data->quarter_scores[0][game_data->quarter - 1] += SAFETY_POINTS;
	}

	ConsoleShowGameScreen(game_data, "Safety Scored!", GAME_SPEED * 50);
	
	ResetForKickoff(game_data);
}

/*
* NAME:        GetOneOrTwoPointConversion
* DESCRIPTION: ask for and return the conversion type
* INPUTS:      game_data
* OUTPUTS:     int 1 = 1 pt attempt, 2 = 2 pt attempt
*/
int GetOneOrTwoPointConversion(struct game_data* game_data)
{
	int points = 1;
	ConsoleShowGameScreen(game_data, "Extra point attempt (1 - 2): ", GAME_SPEED * 2);
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
* INPUTS:      game_data
* OUTPUTS:     none
*/
void ResolveEndOfPlay(struct game_data* game_data)
{
	if (game_data->play_type == PLAY_TYPE_TIMEOUT || game_data->play_type == PLAY_TYPE_FIELD_GOAL) // if a timeout was just played successfully OR field goal just happened, do nothing
		;
	else if (game_data->extra_point_needed && game_data->play_type == PLAY_TYPE_NORMAL)
	{
		if (TouchdownScored(game_data)) // if in the end zone
			AwardTwoPointConversion(game_data); // 2 point conversion attempted
		else
			ConsoleShowGameScreen(game_data, "Missed Extra Points", GAME_SPEED * 20);

		ResetForKickoff(game_data);
		game_data->down = 0;
		game_data->plays_remaining_in_quarter++; // should not take a play clock play so add one back
		game_data->extra_point_needed = false;
	}
	else if (TouchdownScored(game_data))// if in end zone, award touchdown and set game_data->extra_point_needed = true;
	{
		AwardTouchdown(game_data);
	}
	else if (SafetyScored(game_data))
	{
		AwardSafety(game_data);
	}
	else if ((game_data->play_type != PLAY_TYPE_FIELD_GOAL) && (game_data->play_type != PLAY_TYPE_PUNT) && (FirstDownScored(game_data)))
	{
		char buf[256];
		snprintf(buf, sizeof(buf), "%s first down achieved", GetTeamName(game_data, game_data->team_in_possession));
		ConsoleShowGameScreen(game_data, buf, GAME_SPEED * 30);
		ResetLineOfFirstDown(game_data);
		game_data->down = 0;
	}
	else if (game_data->down == 4)
	{
		TurnoverOnDowns(game_data, false);
	}

	if (!GetTableValOnOff(shared_deck, (ACE | SPADES | BLACK | DECK_SHARED))) // if Ace of spades is disabled in shared deck, shuffle that deck
	{
		TableWeightEnableAll(shared_deck);
		ConsoleShowGameScreen(game_data, "Ace of Spades used, reshuffling...", GAME_SPEED * 20);
	}

	game_data->fumble = false;
	game_data->interception = false;
	game_data->show_O_D_Both = 0;
	game_data->pass_play = 0;
	game_data->run_play = 0;
	game_data->matchup_number = 0;
	game_data->formation = 0;
	game_data->play_type = 0;
	ClearAllCards(game_data);
}

/*
* NAME:        DoPlay
* DESCRIPTION: find and then action each play
* INPUTS:      game_data
* OUTPUTS:     none
*/
void DoPlay(struct game_data* game_data)
{
	if (game_data->kickoff_needed)
	{
		game_data->play_type = 2; // always punt on first play of each half
		game_data->kickoff_needed = false;
	}
	else if (game_data->extra_point_needed)
	{
		if (GetOneOrTwoPointConversion(game_data) == 2) // ask player if 1 or 2 point conversion?
			game_data->play_type = 1;						  // 2 pt attempt
		else
			game_data->play_type = 3;						  // 1 pt attempt
	}
	else
	{
		AnnounceDown(game_data);
	}

	bool retry = false;
	if (!game_data->play_type)
		game_data->play_type = GetPlayType(game_data);

	do {
		retry = false;
		switch (game_data->play_type) {
		case PLAY_TYPE_NORMAL:
		{
			if (game_data->extra_point_needed)
				PlayTwoPointConversion(game_data);
			else
				PlayNormal(game_data);
			break;
		}
		case PLAY_TYPE_PUNT:
		{
			PlayPunt(game_data);
			break;
		}
		case PLAY_TYPE_FIELD_GOAL:
		{
			if (game_data->extra_point_needed)
				PlayExtraPoint(game_data);
			else
				PlayFieldGoal(game_data);
			break;
		}
		case PLAY_TYPE_KNEEL:
		{
			PlayKneel(game_data);
			break;
		}
		case PLAY_TYPE_TIMEOUT:
		{
			if (game_data->plays_remaining_in_quarter > (PLAYS_PER_QUARTER - 2))
				retry = true;
			else
				retry = PlayTimeout(game_data);
			break;
		}
		default:
		{
			retry = true;
			break;
		}
		}
		if(retry)
			ConsoleShowGameScreen(game_data, "Invalid play type. Please pick a number between 1 and 5.", GAME_SPEED * 20); // (you cannot timeout unless 2 plays have already happened this quarter and you have 1 remaining)
		if (retry || !game_data->play_type)
			game_data->play_type = GetPlayType(game_data); // player picks what to do: normal play, punt, field goal, kneel, timeout
	} while (retry);

	ResolveEndOfPlay(game_data);
}

/*
* NAME:        EndOfQuarter
* DESCRIPTION: resolve end of quarter stuff
* INPUTS:      game_data
* OUTPUTS:     none
*/
void EndOfQuarter(struct game_data* game_data)
{
	char buf[256];
	snprintf(buf, sizeof(buf), "End of quarter %d.", game_data->quarter);
	ConsoleShowGameScreen(game_data, buf, GAME_SPEED * 20);

	if (game_data->quarter == 1 || game_data->quarter == 3)
	{
		SwitchEnds(game_data);
		ConsoleShowGameScreen(game_data, "Switching ends...", GAME_SPEED * 20);
	}
	else if (game_data->quarter == 2)
	{
		game_data->team_in_possession = (game_data->team_started_with_possession - 1) ? 2 : 1; // team that started 1st half in possession needs ball to kickoff to the other team.
		game_data->team_not_in_possession = (game_data->team_started_with_possession - 1) ? 1 : 2; // other team is receiving

		game_data->kickoff_needed;

		char buf2[256];
		snprintf(buf2, sizeof(buf2), "The %s will receive the ball to start the second half.",(game_data->team_started_with_possession - 1) ? game_data->team_1_name : game_data->team_2_name);
		ConsoleShowGameScreen(game_data, buf2, GAME_SPEED * 20);

		ConsoleShowGameScreen(game_data, "All decks shuffled at half time ", GAME_SPEED * 20);

		TableWeightEnableAll(player_1_deck); // shuffle all decks at half time
		TableWeightEnableAll(player_2_deck);
		TableWeightEnableAll(shared_deck);
	}
	game_data->plays_remaining_in_quarter = PLAYS_PER_QUARTER;
}

/*
* NAME:        EndOfGame
* DESCRIPTION: handles end of game logic
* INPUTS:      game_data
* OUTPUTS:     none
*/
void EndOfGame(struct game_data* game_data)
{
	ConsoleShowGameScreen(game_data, "Game Over!", GAME_SPEED * 20);
	if (game_data->team_1_score > game_data->team_2_score)
	{
		char buf[256];
		snprintf(buf, sizeof(buf), "The %s win!", game_data->team_1_name);
		ConsoleShowGameScreen(game_data, buf, GAME_SPEED * 20);
		if (game_data->playoffs == false && game_data->current_game_mode == 1) {
			game_data->team_records[0][WINS]++;
			game_data->team_records[1][LOSSES]++;
		}
	}
	else if (game_data->team_2_score > game_data->team_1_score)
	{
		char buf[256];
		snprintf(buf, sizeof(buf), "The %s win!", game_data->team_2_name);
		ConsoleShowGameScreen(game_data, buf, GAME_SPEED * 20);
		if (game_data->playoffs == false && game_data->current_game_mode == 1) {
			game_data->team_records[0][LOSSES]++;
			game_data->team_records[1][WINS]++;
		}
	}
	else
	{
		ConsoleShowGameScreen(game_data, "It's a tie!", GAME_SPEED * 20);
		if (game_data->playoffs == false && game_data->current_game_mode == 1) {
			game_data->team_records[0][DRAWS]++;
			game_data->team_records[1][DRAWS]++;
		}
	}

	GameStats(game_data);// print all the game stats? player stats, quarter stats, yards, downs etc
}


/*
* NAME:        PlayGame
* DESCRIPTION: plays out one american football game
* INPUTS:      game_data
* OUTPUTS:     none
*/
void PlayGame(struct game_data* game_data)
{
	for (game_data->quarter; game_data->quarter <= NUMBER_OF_QUARTERS; game_data->quarter++)
	{
		for (game_data->plays_remaining_in_quarter; game_data->plays_remaining_in_quarter > 0; game_data->plays_remaining_in_quarter--, game_data->down++)
		{
			if (game_data->quarter == 1 || game_data->quarter == 3)
			{
				if (game_data->plays_remaining_in_quarter == PLAYS_PER_QUARTER) {
					game_data->team_1_timeouts_remaining = TIMEOUTS_PER_TEAM_PER_HALF;
					game_data->team_2_timeouts_remaining = TIMEOUTS_PER_TEAM_PER_HALF;
				}

				if (game_data->quarter == 3)
				{
					game_data->team_not_in_possession = game_data->team_in_possession;
					game_data->team_in_possession = game_data->team_started_with_possession - 1 ? game_data->team_started_with_possession - 1 : game_data->team_started_with_possession + 1;
				}
			}
			DoPlay(game_data);
			if (game_data->drive_over || ((game_data->current_game_mode == 2) && (game_data->team_1_score > game_data->team_2_score)))
				break;
		}
		EndOfQuarter(game_data);
	}
	EndOfGame(game_data);
}