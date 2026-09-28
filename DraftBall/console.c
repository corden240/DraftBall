#include "main.h"
#include <stdio.h>
#include <windows.h>
#include <string.h>

int GetDirectionOfPlay(struct response* res);

/*
* NAME:        ResetScreen
* DESCRIPTION: clears screen and sets cursor to top left
* INPUTS:      None
* OUTPUTS:     None
*/
void ResetScreen()
{
	printf("\033[H");
}

/*
* NAME:        CardName
* DESCRIPTION: look up the display text for a card
* INPUTS:      card (full card value; FACE/color/DECK bits ignored)
* OUTPUTS:     pointer to static name string, or "??" if not found
*/
const char* CardName(int card)
{
	const int key = card & (VALUE | SUIT);
	for (size_t i = 0; i < CARDS_IN_A_DECK; i++) {
		if (card_names[i].val == key)
			return card_names[i].name;
	}
	return "??";
}

/*
* NAME:        ShowTimeoutBlocks
* DESCRIPTION: prints one team's timeout pips as spaces; grey background =
*              timeout remaining, black = used. Remaining pips sit closest
*              to the team name so the count reads inward.
* INPUTS:      hConsole         - console output handle
*              remaining        - timeouts left (0..TIMEOUTS_PER_TEAM_PER_HALF)
*              pips_before_name - 1 if pips print before the team name (team 1),
*                                 0 if they print after it (team 2)
* OUTPUTS:     None
*/
static void ShowTimeoutBlocks(HANDLE hConsole, int remaining, int pips_before_name)
{
	if (remaining < 0) remaining = 0;
	if (remaining > TIMEOUTS_PER_TEAM_PER_HALF) remaining = TIMEOUTS_PER_TEAM_PER_HALF;

	for (int i = 0; i < TIMEOUTS_PER_TEAM_PER_HALF; i++) {
		int is_remaining = pips_before_name
			? (i >= TIMEOUTS_PER_TEAM_PER_HALF - remaining)
			: (i < remaining);
		if (is_remaining) {
			SetConsoleTextAttribute(hConsole, CONSOLE_BG_LIGHTBLUE);
			putchar(' ');
			SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_LIGHTBLUE);
		}
		else {
			putchar(' ');
		}
		putchar(' ');
	}
}

/*
* NAME:        ShowTopInfo
* DESCRIPTION: displays timeouts, teams, scores, and clock
* INPUTS:      None
* OUTPUTS:     None
*/
void ShowTopInfo(struct response* res)
{
	printf("\n\r");
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_LIGHTBLUE);

	const int SIDE_GAP = 6;          /* exactly 10 spaces on each side of the middle */
	const char DIV = ' ';
	/* team 1 block: pips only (their trailing space is the separator);
	   team 2 block: 1 space + pips. Total = 2 * BLOCKS_WIDTH - 1 */
	const int BLOCKS_WIDTH = (TIMEOUTS_PER_TEAM_PER_HALF * 2) + 1;
	int width = WINDOW_WIDTH;
	int center = (width / 2) - 6;     /* index where middle should appear (0-based) */
	if (center < 0) center = 0;

	const char* team_1_direction = (res->team_1_direction == 1) ? ">>" : "<<";
	const char* team_2_direction = (res->team_2_direction == 1) ? ">>" : "<<";

	/* Build left and right raw strings (local buffers, no accumulation across calls) */
	char left_raw[256];
	char right_raw[128];
	snprintf(left_raw, sizeof(left_raw), "%s %s  %d : %d  %s %s",
		team_1_direction, res->team_1_name, res->team_1_score, res->team_2_score, res->team_2_name, team_2_direction);
	snprintf(right_raw, sizeof(right_raw), "Quarter: %d  Plays Left: %d",
		res->quarter, res->plays_remaining_in_quarter);

	/* left gap must start SIDE_GAP chars left of center */
	int left_gap_start = center - SIDE_GAP;
	if (left_gap_start < 0) left_gap_start = 0;

	/* Left side layout: [team1 pips] names/scores [team2 pips].
	   Pips print with color changes, so only the core string is truncated;
	   keep the rightmost characters so scores remain visible. */
	int left_max_len = left_gap_start;
	size_t left_raw_len = strlen(left_raw);
	char left_display[256];
	int show_blocks = 1;
	int core_max_len = left_max_len - (2 * BLOCKS_WIDTH - 1);
	if ((int)left_raw_len > core_max_len) {
		if (core_max_len > 0) {
			size_t start = left_raw_len - (size_t)core_max_len;
			strncpy(left_display, left_raw + start, core_max_len);
			left_display[core_max_len] = '\0';
		}
		else if (left_max_len > 0) {
			/* too narrow for pips; drop them and keep rightmost core chars */
			size_t start = left_raw_len - (size_t)left_max_len;
			strncpy(left_display, left_raw + start, left_max_len);
			left_display[left_max_len] = '\0';
			show_blocks = 0;
		}
		else {
			left_display[0] = '\0';
			show_blocks = 0;
		}
	}
	else {
		strcpy(left_display, left_raw);
	}
	int left_display_len = (int)strlen(left_display) + (show_blocks ? 2 * BLOCKS_WIDTH - 1 : 0);

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

	/* Print in segments so the pips can use a grey background */
	for (int i = 0; i < leading_spaces; i++) putchar(' ');
	if (show_blocks) {
		ShowTimeoutBlocks(hConsole, res->team_1_timeouts_remaining, 1);
	}
	printf("%s", left_display);
	if (show_blocks) {
		putchar(' ');
		ShowTimeoutBlocks(hConsole, res->team_2_timeouts_remaining, 0);
	}
	for (int i = 0; i < SIDE_GAP; i++) putchar(' ');
	putchar(DIV);
	for (int i = 0; i < SIDE_GAP; i++) putchar(' ');
	printf("%s", right_display);

	/* Pad the rest of the line out to the window width */
	int line_len = left_gap_start + SIDE_GAP + 1 + SIDE_GAP + (int)strlen(right_display);
	for (int i = line_len; i < width; i++) putchar(' ');

	SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);

	printf("\n\r\n\r          ");

	SetConsoleTextAttribute(hConsole, CONSOLE_BG_RED);
	printf(" ");// red box = line of scrim
	SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
	printf(" = Line of scrimmage   ");

	SetConsoleTextAttribute(hConsole, CONSOLE_BG_PURPLE);
	printf(" ");// purple box = line of 1st down
	SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
	printf(" = Line of 1st down    ");

	if (res->team_in_possession == 1 || res->team_in_possession == 2)
	{
		char buf[256];
		snprintf(buf, sizeof(buf), "   %s %s in possession % s   ",
			GetDirectionOfPlay(res) == 1 ? ">>" : "<<",
			(res->team_in_possession - 1) ? res->team_2_name : res->team_1_name,
			GetDirectionOfPlay(res) == 1 ? ">>" : "<<");
		printf(buf);
	}

	SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
}

/*
* NAME:        PrintYardNumbers
* DESCRIPTION: show yard lines below the field
* INPUTS:      None
* OUTPUTS:     None
*/
void PrintYardNumbers(void) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
	printf("\n\r     ");
	int yards[] = { 10, 20, 30, 40, 50, 40, 30, 20, 10 };
	int n = sizeof(yards) / sizeof(yards[0]);

	for (int i = 0; i < n; i++) {
		printf("%10d", yards[i]);   // field width 10, right-justified
	}
}

/*
* NAME:        PrintSideline
* DESCRIPTION: print sideline
* INPUTS:      None
* OUTPUTS:     None
*/
void PrintSideline(void) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
	printf("\n\r");
	SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
	for (int i = 0; i < (END_ZONE_WIDTH + FIELD_WIDTH + END_ZONE_WIDTH - 1); i++) {
		printf("-");
	}
}

/*
* NAME:        DrawField
* DESCRIPTION: shows field and lines 
* INPUTS:      response
* OUTPUTS:     None
*/
void DrawField(struct response* res)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
	printf("\n\r");

	PrintSideline();
	for (int j = 0; j < FIELD_HEIGHT; j++)
	{
		printf("\n\r");
		for (int i = 1; i <= (PITCH_WIDTH - 1); i++) {
			if (i < END_ZONE_WIDTH || i > (PITCH_WIDTH - END_ZONE_WIDTH))
			{
				SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_RED);
				printf("/");
			}
			else {
				if (i == res->line_of_scrimmage)
					SetConsoleTextAttribute(hConsole, CONSOLE_BG_RED);
				else if (i == res->line_of_first_down && res->play_type != PLAY_TYPE_PUNT && res->play_type != PLAY_TYPE_FIELD_GOAL && !res->kickoff_needed)
					SetConsoleTextAttribute(hConsole, CONSOLE_BG_PURPLE);
				else
					SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);

				if ((i - END_ZONE_WIDTH) % YARDS_PER_FIRST_DOWN == 0)
				{
					printf("|");
				}
				else
				{
					printf(" ");
				}
				SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
			}
		}
	}

	PrintSideline();

	PrintYardNumbers();
}

/*
* NAME:        ShowGameInfo
* DESCRIPTION: prints down and distance info
* INPUTS:      response
* OUTPUTS:     None
*/
void ShowGameInfo(struct response* res)
{
	int distance = abs(res->line_of_scrimmage - res->line_of_first_down);
	if (res->play_type == PLAY_TYPE_PUNT || res->play_type == PLAY_TYPE_FIELD_GOAL)
		distance = 0;

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
	printf("\n\r\n\r");
	printf("                                 Down: %d  Distance: %d  Line of Scrimmage: %d                                   ", res->down,distance , ConvertLosToYardLine(res));
}

/*
* NAME:        PrintCard
* DESCRIPTION: prints 1 card in correct colour
* INPUTS:      response
* OUTPUTS:     None
*/
void PrintCard(int card)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if((card & SUIT) == CLUBS)
		SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_GREEN);
	else if ((card & SUIT) == DIAMONDS)
		SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_LIGHTBLUE);
	else if ((card & SUIT) == HEARTS)
		SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_RED);
	else if ((card & SUIT) == SPADES)
		SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
	printf("%s", CardName(card));
	SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
}

/*
* NAME:        PrintOCards
* DESCRIPTION: prints o cards
* INPUTS:      response
* OUTPUTS:     None
*/
void PrintOCards(struct response* res)
{
	for (int i = 0; i < CARDS_IN_A_DECK && res->o_cards[i] != '\0'; i++)
	{
		if (i == 0)
			printf("O: ");
		if (!res->o_cards[i])
			break;
		PrintCard(res->o_cards[i]);
		if (res->o_cards[i + 1])
			printf(", ");
	}
}

/*
* NAME:        PrintDCards
* DESCRIPTION: prints d cards
* INPUTS:      response
* OUTPUTS:     None
*/
void PrintDCards(struct response* res)
{

	for (int i = 0; i < CARDS_IN_A_DECK && res->d_cards[i] != '\0'; i++)
	{
		if (i == 0)
			printf("D: ");
		if (!res->d_cards[i])
			break;
		PrintCard(res->d_cards[i]);
		if (res->d_cards[i + 1])
			printf(", ");
	}
}

/*
* NAME:        ShowCardsInFormation
* DESCRIPTION: shows given cards in res->formation
* INPUTS:      response, offence_or_defence (1-2)
* OUTPUTS:     None
*/
ShowCardsInFormation(struct response* res, int offence_or_defence)
{
	int gaps[] = { 0, 0, 0, 0, };
	switch (res->formation) {
	case 1:{ gaps[1] = 7, gaps[2] = 2, gaps[3] = 8; break;}
	case 2:{ gaps[1] = 9, gaps[2] = 6, gaps[3] = 2; break;}
	case 3:{ gaps[1] = 2, gaps[2] = 13, gaps[3] = 2; break;}
	}

	if (offence_or_defence == 1)
		printf("O: ");
	else
		printf("D: ");

	for (int i = 0; i < NORMAL_PLAY_CARDS_TO_DRAW; i++)
	{
		for (int j = 0; j < gaps[i]; j++)
			printf(" ");

		//if (res->show_O_D_Both == 3)
		//	Sleep(GAME_SPEED * 5);
		if (offence_or_defence == 1 && res->o_cards_in_position[i])
			PrintCard(res->o_cards_in_position[i]);
		else if (offence_or_defence == 2 && res->d_cards_in_position[i])
			PrintCard(res->d_cards_in_position[i]);
		else
			printf("[]");
	}
}

/*
* NAME:        ShowCards
* DESCRIPTION: prints cards
* INPUTS:      response
* OUTPUTS:     None
*/
void ShowCards(struct response* res)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
	printf("\r\n                                 ");
	printf("\r\n                                 ");

	if (res->formation && !res->run_play && !res->pass_play)
	{
		if (res->show_O_D_Both == 0)
			printf("                                 \r\n                                                                  ");
		else if (res->show_O_D_Both == 1)
		{
			ShowCardsInFormation(res, 1);	// send 1 for offence, 2 for defence
			printf("\r\n");
		}
		else if (res->show_O_D_Both == 2)
		{
			ShowCardsInFormation(res, 2);	// send 1 for offence, 2 for defence
			printf("\r\n");
		}
		else if (res->show_O_D_Both == 3)
		{
			ShowCardsInFormation(res, 2);	// send 1 for offence, 2 for defence;
			printf("\r\n                                 ");
			ShowCardsInFormation(res, 1);
		}
		else
			printf("Error res->show_O_D_Both wrong");
	}
	else
	{
		if (res->o_cards[0] && res->d_cards[0])
		{
			if (res->d_cards[0])
				PrintDCards(res);
			else
				printf("                                 ");

			printf("                        ");

			printf("\r\n                                 ");

			if (res->o_cards[0])
				PrintOCards(res);
			else
				printf("                                 ");
		}
		else
		{
			printf("                                 ");

			printf("                        ");

			printf("\r\n                                 ");

			if (res->d_cards[0])
				PrintDCards(res);
			else if (res->o_cards[0])
				PrintOCards(res);
			else
				printf("                                 ");
		}

		printf("                        ");
	}
}

/*
* NAME:        ShowText
* DESCRIPTION: prints text given to function
* INPUTS:      response, text to display
* OUTPUTS:     None
*/
void ShowText(struct response* res, char txt[1000])
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
	printf("\r\n\r\n                                                                                             ");
	printf("\r                                 ");
	for (int i = 0; i < MAX_STRING_lENGTH && txt[i] != '\0'; i++)
	{
		printf("%c", txt[i]);
		Sleep(GAME_SPEED / 5);
	}
}

/*
* NAME:        ConsoleShowScreen
* DESCRIPTION: updates the whole screen with current data
* INPUTS:      response, text to display, pause time, wait for keypress
* OUTPUTS:     None
*/
void ConsoleShowScreen(struct response* res, char txt[1000], int pause) // if called this asking a question to the player, make sure to called getch straight after
{
	ResetScreen();     // go to top left of screen and clear it
	ShowTopInfo(res);  // draw score at top of screen with play clock
	DrawField(res);    // draw field with line of scrimmage and down and distance lines
	ShowGameInfo(res); // then write down and distance numbers
	ShowCards(res);    // card drawn info?
	if(txt)
		ShowText(res, txt);// then draw text sent to function
	if(pause)
		Sleep(pause);      // pause for a bit if needed)
}