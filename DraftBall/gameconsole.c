#include "main.h"
#include <stdio.h>
#include <windows.h>
#include <string.h>

int GetDirectionOfPlay(struct game_data* game_data);

/*
* NAME:        ResetScreen
* DESCRIPTION: clears screen and sets cursor to top left
* INPUTS:      none
* OUTPUTS:     none
*/
void ResetScreen()
{
	printf("\033[H");
}

/*
* NAME:        HideCursor
* DESCRIPTION: hides cursor
* INPUTS:      none
* OUTPUTS:     none
*/
void HideCursor()
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(consoleHandle, &info);
}

/*
* NAME:        UnHideCursor
* DESCRIPTION: hides cursor
* INPUTS:      none
* OUTPUTS:     none
*/
void UnHideCursor()
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = TRUE;
	SetConsoleCursorInfo(consoleHandle, &info);
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
* OUTPUTS:     none
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
			SetConsoleTextAttribute(hConsole, CONSOLE_BG_GREY);
			putchar(' ');
			SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
		}
		else {
			putchar(' ');
		}
		putchar(' ');
	}
}

/*
* NAME:        TruncateKeepRight
* DESCRIPTION: copies src into dst keeping at most max_len characters; when
*              truncating, the rightmost characters are kept so scores remain
*              visible. Returns the length of the resulting string.
* INPUTS:      dst     - destination buffer
*              dst_size - size of dst including null terminator
*              src     - source string
*              max_len - maximum number of characters to keep
* OUTPUTS:     length of the string written to dst
*/
static int TruncateKeepRight(char* dst, size_t dst_size, const char* src, int max_len)
{
	size_t src_len = strlen(src);
	if (max_len <= 0) {
		dst[0] = '\0';
	}
	else if ((int)src_len > max_len) {
		size_t start = src_len - (size_t)max_len;
		strncpy(dst, src + start, (size_t)max_len);
		dst[max_len] = '\0';
	}
	else {
		strcpy(dst, src);
	}
	(void)dst_size;
	return (int)strlen(dst);
}

/*
* NAME:        ShowTopInfo
* DESCRIPTION: displays timeouts, teams, scores, and clock
* INPUTS:      none
* OUTPUTS:     none
*/
void ShowTopInfo(struct game_data* game_data)
{
	printf("\n\r");
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	/* ---- Colours: change any value here to recolour that part ---- */
	const int COLOUR_TIMEOUT_REMAINING = CONSOLE_BG_GREY;      /* used inside ShowTimeoutBlocks */
	const int COLOUR_BASE = CONSOLE_TEXT_WHITE;   /* padding / separators / default */
	const int COLOUR_TEAM_1_NAME = game_data->team_1_colour;
	const int COLOUR_TEAM_2_NAME = game_data->team_2_colour;
	const int COLOUR_ARROWS = CONSOLE_TEXT_WHITE;   /* direction arrows */
	const int COLOUR_SCORE = CONSOLE_TEXT_WHITE;   /* "N : N" */
	const int COLOUR_CLOCK = CONSOLE_TEXT_WHITE;   /* quarter / plays left */
	(void)COLOUR_TIMEOUT_REMAINING;
	/* --------------------------------------------------------------- */

	SetConsoleTextAttribute(hConsole, COLOUR_BASE);

	const int SIDE_GAP = 6;          /* exactly 10 spaces on each side of the middle */
	const char DIV = ' ';
	/* team 1 block: pips only (their trailing space is the separator);
	   team 2 block: 1 space + pips. Total = 2 * BLOCKS_WIDTH - 1 */
	const int BLOCKS_WIDTH = (TIMEOUTS_PER_TEAM_PER_HALF * 2) + 1;
	int width = WINDOW_WIDTH;
	int center = (width / 2) - 6;     /* index where middle should appear (0-based) */
	if (center < 0) center = 0;

	/* ---- Build the individual segments (local buffers, no accumulation) ---- */
	char t1_arrows[8];
	char t1_name[64];
	char score[32];
	char t2_name[64];
	char t2_arrows[8];
	char right_raw[128];
	snprintf(t1_arrows, sizeof(t1_arrows), "%s ", (game_data->team_1_direction == 1) ? ">>" : "<<");
	snprintf(t1_name, sizeof(t1_name), "%s", game_data->team_1_name);
	snprintf(score, sizeof(score), "  %d : %d  ", game_data->team_1_score, game_data->team_2_score);
	snprintf(t2_name, sizeof(t2_name), "%s", game_data->team_2_name);
	snprintf(t2_arrows, sizeof(t2_arrows), " %s", (game_data->team_2_direction == 1) ? ">>" : "<<");
	snprintf(right_raw, sizeof(right_raw), "Quarter: %d  Plays Left: %d",
		game_data->quarter, game_data->plays_remaining_in_quarter);

	/* left gap must start SIDE_GAP chars left of center */
	int left_gap_start = center - SIDE_GAP;
	if (left_gap_start < 0) left_gap_start = 0;

	/* Left side layout: [team1 pips] t1_arrows t1_name score t2_name t2_arrows [team2 pips].
	   Pips print with colour changes, so only the core string is truncated;
	   keep the rightmost characters so scores remain visible. Truncation is
	   applied proportionally across the segments. */
	int left_max_len = left_gap_start;
	int show_blocks = 1;
	int core_max_len = left_max_len - (2 * BLOCKS_WIDTH - 1);
	int left_raw_len = (int)(strlen(t1_arrows) + strlen(t1_name) + strlen(score)
		+ strlen(t2_name) + strlen(t2_arrows));
	double keep_ratio = 1.0;
	if (left_raw_len > core_max_len) {
		if (core_max_len <= 0) {
			/* too narrow for pips; drop them */
			show_blocks = 0;
			core_max_len = left_max_len;
		}
		keep_ratio = (left_raw_len > 0) ? ((double)core_max_len / (double)left_raw_len) : 0.0;
	}

	char t1_arrows_d[8];
	char t1_name_d[64];
	char score_d[32];
	char t2_name_d[64];
	char t2_arrows_d[8];
	int left_display_len = 0;
	left_display_len += TruncateKeepRight(t1_arrows_d, sizeof(t1_arrows_d), t1_arrows, (int)(strlen(t1_arrows) * keep_ratio));
	left_display_len += TruncateKeepRight(t1_name_d, sizeof(t1_name_d), t1_name, (int)(strlen(t1_name) * keep_ratio));
	left_display_len += TruncateKeepRight(score_d, sizeof(score_d), score, (int)(strlen(score) * keep_ratio));
	left_display_len += TruncateKeepRight(t2_name_d, sizeof(t2_name_d), t2_name, (int)(strlen(t2_name) * keep_ratio));
	left_display_len += TruncateKeepRight(t2_arrows_d, sizeof(t2_arrows_d), t2_arrows, (int)(strlen(t2_arrows) * keep_ratio));
	if (show_blocks) left_display_len += 2 * BLOCKS_WIDTH - 1;

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
	if (show_blocks) {
		ShowTimeoutBlocks(hConsole, game_data->team_1_timeouts_remaining, 1);
	}
	SetConsoleTextAttribute(hConsole, COLOUR_ARROWS);
	printf("%s", t1_arrows_d);
	SetConsoleTextAttribute(hConsole, COLOUR_TEAM_1_NAME);
	printf("%s", t1_name_d);
	SetConsoleTextAttribute(hConsole, COLOUR_SCORE);
	printf("%s", score_d);
	SetConsoleTextAttribute(hConsole, COLOUR_TEAM_2_NAME);
	printf("%s", t2_name_d);
	SetConsoleTextAttribute(hConsole, COLOUR_ARROWS);
	printf("%s", t2_arrows_d);
	if (show_blocks) {
		SetConsoleTextAttribute(hConsole, COLOUR_BASE);
		putchar(' ');
		ShowTimeoutBlocks(hConsole, game_data->team_2_timeouts_remaining, 0);
	}
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

	printf("\n\r\n\r          ");

	SetConsoleTextAttribute(hConsole, CONSOLE_BG_RED);
	printf(" ");// red box = line of scrim
	SetConsoleTextAttribute(hConsole, COLOUR_BASE);
	printf(" = Line of scrimmage   ");

	SetConsoleTextAttribute(hConsole, CONSOLE_BG_PURPLE);
	printf(" ");// purple box = line of 1st down
	SetConsoleTextAttribute(hConsole, COLOUR_BASE);
	printf(" = Line of 1st down    ");

	if (game_data->team_in_possession == 1 || game_data->team_in_possession == 2)
	{
		char buf[256];
		snprintf(buf, sizeof(buf), "   %s %s in possession % s   ",
			GetDirectionOfPlay(game_data) == 1 ? ">>" : "<<",
			(game_data->team_in_possession - 1) ? game_data->team_2_name : game_data->team_1_name,
			GetDirectionOfPlay(game_data) == 1 ? ">>" : "<<");
		printf(buf);
	}

	SetConsoleTextAttribute(hConsole, COLOUR_BASE);
}

/*
* NAME:        PrintYardNumbers
* DESCRIPTION: show yard lines below the field
* INPUTS:      none
* OUTPUTS:     none
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
* INPUTS:      none
* OUTPUTS:     none
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
* INPUTS:      game_data
* OUTPUTS:     none
*/
void DrawField(struct game_data* game_data)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);

	char* plays[] = { "Play", "Punt", "FG", "Kneel", "Timeout" };
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
				if (i == game_data->line_of_scrimmage)
					SetConsoleTextAttribute(hConsole, CONSOLE_BG_RED);
				else if (i == game_data->line_of_first_down && game_data->play_type != PLAY_TYPE_PUNT && game_data->play_type != PLAY_TYPE_FIELD_GOAL && !game_data->kickoff_needed)
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
		if (j < (PITCH_WIDTH - 1))
		{
			SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
			printf(" %d %s ", j + 1, plays[j]);
		}
	}

	PrintSideline();

	PrintYardNumbers();
}

/*
* NAME:        ShowGameInfo
* DESCRIPTION: prints down and distance info
* INPUTS:      game_data
* OUTPUTS:     none
*/
void ShowGameInfo(struct game_data* game_data)
{
	int distance = abs(game_data->line_of_scrimmage - game_data->line_of_first_down);
	if (game_data->play_type == PLAY_TYPE_PUNT || game_data->play_type == PLAY_TYPE_FIELD_GOAL || game_data->kickoff_needed)
		distance = 0;

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
	printf("\n\r\n\r");
	printf("                                                  ");

	switch (game_data->down) {
	case 1: { printf("%dst & %d", game_data->down, distance); break; }
	case 2: { printf("%dnd & %d", game_data->down, distance); break; }
	case 3: { printf("%drd & %d", game_data->down, distance); break; }
	case 4: { printf("%dth & %d", game_data->down, distance); break; }
	}
}

/*
* NAME:        PrintCard
* DESCRIPTION: prints 1 card in correct colour
* INPUTS:      game_data
* OUTPUTS:     none
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
* INPUTS:      game_data
* OUTPUTS:     none
*/
void PrintOCards(struct game_data* game_data)
{
	for (int i = 0; i < CARDS_IN_A_DECK && game_data->o_cards[i] != '\0'; i++)
	{
		if (i == 0)
			printf("O: ");
		if (!game_data->o_cards[i])
			break;
		PrintCard(game_data->o_cards[i]);
		if (game_data->o_cards[i + 1])
			printf(", ");
	}
}

/*
* NAME:        PrintDCards
* DESCRIPTION: prints d cards
* INPUTS:      game_data
* OUTPUTS:     none
*/
void PrintDCards(struct game_data* game_data)
{

	for (int i = 0; i < CARDS_IN_A_DECK && game_data->d_cards[i] != '\0'; i++)
	{
		if (i == 0)
			printf("D: ");
		if (!game_data->d_cards[i])
			break;
		PrintCard(game_data->d_cards[i]);
		if (game_data->d_cards[i + 1])
			printf(", ");
	}
}

/*
* NAME:        ShowCardsInFormation
* DESCRIPTION: shows given cards in game_data->formation
* INPUTS:      game_data, offence_or_defence (1-2)
* OUTPUTS:     none
*/
ShowCardsInFormation(struct game_data* game_data, int offence_or_defence)
{
	int gaps[] = { 0, 0, 0, 0, };
	switch (game_data->formation) {
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

		//if (game_data->show_O_D_Both == 3)
		//	Sleep(GAME_SPEED * 5);
		if (offence_or_defence == 1 && game_data->o_cards_in_position[i])
			PrintCard(game_data->o_cards_in_position[i]);
		else if (offence_or_defence == 2 && game_data->d_cards_in_position[i])
			PrintCard(game_data->d_cards_in_position[i]);
		else
			printf("[]");
	}
}

/*
* NAME:        ShowCards
* DESCRIPTION: prints cards
* INPUTS:      game_data
* OUTPUTS:     none
*/
void ShowCards(struct game_data* game_data)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
	printf("\r\n                                 ");
	printf("\r\n                                 ");

	if (game_data->formation && !game_data->run_play && !game_data->pass_play)
	{
		if (game_data->show_O_D_Both == 0)
			printf("                                 \r\n                                                                  ");
		else if (game_data->show_O_D_Both == 1)
		{
			ShowCardsInFormation(game_data, 1);	// send 1 for offence, 2 for defence
			printf("\r\n");
		}
		else if (game_data->show_O_D_Both == 2)
		{
			ShowCardsInFormation(game_data, 2);	// send 1 for offence, 2 for defence
			printf("\r\n");
		}
		else if (game_data->show_O_D_Both == 3)
		{
			ShowCardsInFormation(game_data, 2);	// send 1 for offence, 2 for defence;
			printf("\r\n                                 ");
			ShowCardsInFormation(game_data, 1);
		}
		else
			printf("Error game_data->show_O_D_Both wrong");
	}
	else
	{
		if (game_data->o_cards[0] && game_data->d_cards[0])
		{
			if (game_data->d_cards[0])
				PrintDCards(game_data);
			else
				printf("                                 ");

			printf("                        ");

			printf("\r\n                                 ");

			if (game_data->o_cards[0])
				PrintOCards(game_data);
			else
				printf("                                 ");
		}
		else
		{
			printf("                                 ");

			printf("                        ");

			printf("\r\n                                 ");

			if (game_data->d_cards[0])
				PrintDCards(game_data);
			else if (game_data->o_cards[0])
				PrintOCards(game_data);
			else
				printf("                                 ");
		}

		printf("                        ");
	}
}

/*
* NAME:        ShowText
* DESCRIPTION: prints text given to function
* INPUTS:      game_data, text to display
* OUTPUTS:     none
*/
void ShowText(struct game_data* game_data, char txt[1000])
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
* NAME:        ConsoleShowGameScreen
* DESCRIPTION: updates the whole screen with current data
* INPUTS:      game_data, text to display, pause time, wait for keypress
* OUTPUTS:     none
*/
void ConsoleShowGameScreen(struct game_data* game_data, char txt[1000], int pause) // if called this asking a question to the player, make sure to called getch straight after
{
	ResetScreen();     // go to top left of screen and clear it
	ShowTopInfo(game_data);  // draw score at top of screen with play clock
	DrawField(game_data);    // draw field with line of scrimmage and down and distance lines
	ShowGameInfo(game_data); // then write down and distance numbers
	ShowCards(game_data);    // card drawn info?
	if(txt)
		ShowText(game_data, txt);// then draw text sent to function
	if(pause)
		Sleep(pause);      // pause for a bit if needed)
}