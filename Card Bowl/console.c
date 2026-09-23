#include "main.h"
#include <stdio.h>
#include <windows.h>
#include <string.h>

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
* NAME:        ShowTopInfo
* DESCRIPTION: displays teams, scores, and clock
* INPUTS:      None
* OUTPUTS:     None
*/
void ShowTopInfo(struct response* res)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_LIGHTBLUE);

	const int SIDE_GAP = 10;          /* exactly 10 spaces on each side of '|' */
	const char DIV = '|';
	int width = WINDOW_WIDTH;
	int center = (width / 2) - 6;     /* index where '|' should appear (0-based) */
	if (center < 0) center = 0;

	/* Build left and right raw strings (local buffers, no accumulation across calls) */
	char left_raw[256];
	char right_raw[128];
	snprintf(left_raw, sizeof(left_raw), "%s  %d : %d  %s",
		res->team_1_name, res->score_team_1, res->score_team_2, res->team_2_name);
	snprintf(right_raw, sizeof(right_raw), "Quarter: %d  Plays Left: %d",
		res->quarter, res->plays_remaining_in_quarter);

	/* left gap must start SIDE_GAP chars left of center */
	int left_gap_start = center - SIDE_GAP;
	if (left_gap_start < 0) left_gap_start = 0;

	/* Determine left display that must end immediately before the left gap.
	   If too long, keep the rightmost characters so scores remain visible. */
	int left_max_len = left_gap_start;
	size_t left_raw_len = strlen(left_raw);
	char left_display[256];
	if ((int)left_raw_len > left_max_len) {
		if (left_max_len <= 0) {
			left_display[0] = '\0';
		}
		else {
			/* keep the rightmost characters */
			size_t start = left_raw_len - (size_t)left_max_len;
			strncpy(left_display, left_raw + start, left_max_len);
			left_display[left_max_len] = '\0';
		}
	}
	else {
		strcpy(left_display, left_raw);
	}
	int left_display_len = (int)strlen(left_display);

	/* Leading padding so left_display ends at left_gap_start-1 */
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

	/* Prepare component strings */
	char left_pad_str[512];
	memset(left_pad_str, ' ', leading_spaces);
	left_pad_str[leading_spaces] = '\0';

	char gapStr[11];
	memset(gapStr, ' ', SIDE_GAP);
	gapStr[SIDE_GAP] = '\0';

	/* Compose final line and ensure it's exactly 'width' chars (pad or truncate) */
	char line[1024];
	snprintf(line, sizeof(line), "%s%s%s%c%s%s",
		left_pad_str, left_display, gapStr, DIV, gapStr, right_display);

	int line_len = (int)strlen(line);
	if (line_len < width) {
		int trailing = width - line_len;
		if (trailing > 0) {
			char trail[512];
			memset(trail, ' ', trailing);
			trail[trailing] = '\0';
			strncat(line, trail, trailing);
		}
	}
	else if (line_len > width) {
		line[width] = '\0';
	}

	printf("%s", line);
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
	// TODO: print different colour on line of scrimmage and line of first down
	for (int j = 0; j < FIELD_HEIGHT; j++)
	{
		printf("\n\r");
		for (int i = 1; i <= (END_ZONE_WIDTH + FIELD_WIDTH + END_ZONE_WIDTH - 1); i++) {
			if (i < END_ZONE_WIDTH || i > (PITCH_WIDTH - END_ZONE_WIDTH))
			{
				SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_RED);
				printf("/");
			}
			else if ((i - 5) % 10 == 0)
			{
				SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
				printf("|");
			}
			else
			{
				SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
				printf(" ");
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
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
	printf("\n\r\n\r");
	printf("                                 Down: %d  Distance: %d  Line of Scrimmage: %d                                   ", res->down, res->line_of_scrimmage - res->line_of_first_down, res->line_of_scrimmage);
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
	printf("\n\r\n\r                                                                                             ");
	printf("\r                                 ");
	for (int i = 0; i < CARDS_IN_A_DECK && res->o_cards[i] != '\0'; i++)
	{
		printf("%c", CardName(res->o_cards[i]));
		Sleep(10);
		printf("\n\r");
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
	printf("\n\r\n\r                                                                                             ");
	printf("\r                                 ");
	for (int i = 0; i < 1000 && txt[i] != '\0'; i++)
	{
		printf("%c", txt[i]);
		Sleep(10);
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
	ShowCards(res);    //card drawn info?
	if(txt)
		ShowText(res, txt);// then draw text sent to function
	if(pause)
		Sleep(pause);      // pause for a bit if needed)
}