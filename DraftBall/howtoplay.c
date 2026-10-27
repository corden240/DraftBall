#include "main.h"
#include <windows.h>
#include <stdio.h>
#include <string.h>

void ShowMenuTitle();
void PrintTextWithDelays(char txt[1000]);
void ResetScreen();

/*
* NAME:        ShowRulesGeneral
* DESCRIPTION: show general rules
* INPUTS:      none
* OUTPUTS:     none
*/
static void ShowRulesGeneral()
{
	system("cls");
	ShowMenuTitle();
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	printf("\r\n");
	printf("\r\n                            ");
	PrintTextWithDelays("DraftBall is an American Football card game");
		
	printf("\r\n                            ");
	PrintTextWithDelays("It uses 3 decks of cards, one each for offence and defence and one shared deck");

	printf("\r\n\r\n                            ");
	PrintTextWithDelays("The rules are mostly the same as NFL");

	printf("\r\n                            ");
	PrintTextWithDelays("The only controls needed to play are numbers, arrows and enter");

	printf("\r\n                            ");
	char buffer[256];
	snprintf(buffer, sizeof(buffer), "Games consist of 4 quarters with %d plays, timeouts add 1 play to the clock", DEFAULT_PLAYS_PER_QUARTER);
	PrintTextWithDelays(buffer);

	printf("\r\n\r\n                            ");
	PrintTextWithDelays("Cards are shown as ");
	SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_GREEN);
	PrintTextWithDelays("Ac");
	SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
	PrintTextWithDelays(" = Ace of Clubs, ");
	SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_RED);
	PrintTextWithDelays("Th");
	SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
	PrintTextWithDelays(" = Ten of Hearts and ");
	SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_LIGHTBLUE);
	PrintTextWithDelays("4d");
	SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
	PrintTextWithDelays(" = 4 of Diamonds");

	printf("\r\n\r\n                                        ");
	printf("               ");
	SetConsoleTextAttribute(hConsole, CONSOLE_BG_WHITE | CONSOLE_TEXT_GREEN);
	PrintTextWithDelays("Back");
	SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);

	int ch = getch();
	if (ch == 0 || ch == 224)
		return;
}

/*
* NAME:        ShowRulesPass
* DESCRIPTION: show pass play rules
* INPUTS:      none
* OUTPUTS:     none
*/
static void ShowRulesPass()
{
	system("cls");
	ShowMenuTitle();
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	printf("\r\n");
	printf("\r\n                            ");
	PrintTextWithDelays("During a pass play:");

	printf("\r\n\r\n                            ");
	PrintTextWithDelays("The offence picks a formation, 0 - 2 Backs with 2 - 4 Receivers");

	printf("\r\n                            ");
	PrintTextWithDelays("and fills those 4 positions with cards drawn from their own deck");

	printf("\r\n                            ");
	PrintTextWithDelays("The defence then draws their own 4 cards and fills their 4 positions blind");

	printf("\r\n\r\n                            ");
	PrintTextWithDelays("If the offence picks to pass, they will draw cards until they choose to stop");

	printf("\r\n                            ");
	PrintTextWithDelays("When drawing these cards, any the same suit as the first one = incomplete");

	printf("\r\n                            ");
	PrintTextWithDelays("Any the same suit and number as the first one = Sack");

	printf("\r\n                            ");
	PrintTextWithDelays("Ace of spades = interception");

	printf("\r\n                            ");
	PrintTextWithDelays("When offence stops drawing, the defence draw 1 card");

	printf("\r\n                            ");
	PrintTextWithDelays("The sum of all offence cards - sum of defence cards = yards gained");

	printf("\r\n                            ");
	PrintTextWithDelays("One more offence and defence card is drawn to calculate yards after catch");

	printf("\r\n                            ");
	PrintTextWithDelays("If O card is a face card AND it is bigger than D card then its an open field run");
	
	printf("\r\n\r\n                                        ");
	printf("               ");
	SetConsoleTextAttribute(hConsole, CONSOLE_BG_WHITE | CONSOLE_TEXT_GREEN);
	PrintTextWithDelays("Back");
	SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);

	int ch = getch();
	if (ch == 0 || ch == 224)
		return;
}


/*
* NAME:        ShowRulesRun
* DESCRIPTION: show run play rules
* INPUTS:      none
* OUTPUTS:     none
*/
static void ShowRulesRun()
{
	system("cls");
	ShowMenuTitle();
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	printf("\r\n");
	printf("\r\n                            ");
	PrintTextWithDelays("During a run play:");

	printf("\r\n\r\n                            ");
	PrintTextWithDelays("The offence picks a formation, 0 - 2 Backs with 2 - 4 Receivers");

	printf("\r\n                            ");
	PrintTextWithDelays("and fills those 4 positions with cards drawn from their own deck");

	printf("\r\n                            ");
	PrintTextWithDelays("The defence then draws their own 4 cards and fills their 4 positions blind");

	printf("\r\n\r\n                            ");
	PrintTextWithDelays("If the offence picks to run, the offence and defence will draw 1 more card");

	printf("\r\n                            ");
	PrintTextWithDelays("((O1 + O2) - (D1 + D2)) / 2 = yards gained");

	printf("\r\n                            ");
	PrintTextWithDelays("If yards gained is a negative number then divide by 2 again for tackle for loss yards");

	printf("\r\n                            ");
	PrintTextWithDelays("If O2 OR D2 is the Ace of spades then it is a fumble");

	printf("\r\n                            ");
	PrintTextWithDelays("If O2 is a face card AND it's bigger than D2 then its an open field run");

	printf("\r\n                            ");
	PrintTextWithDelays("An open field run allows the runner to draw cards for more yards");

	printf("\r\n                            ");
	PrintTextWithDelays("Jack = 1 card, Queen = 2 cards, King = 3 cards");


	printf("\r\n\r\n                                        ");
	printf("               ");
	SetConsoleTextAttribute(hConsole, CONSOLE_BG_WHITE | CONSOLE_TEXT_GREEN);
	PrintTextWithDelays("Back");
	SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);

	int ch = getch();
	if (ch == 0 || ch == 224)
		return;
}

/*
* NAME:        ShowRulesFumblesandint
* DESCRIPTION: show fumble and int rules
* INPUTS:      none
* OUTPUTS:     none
*/
static void ShowRulesFumblesandint()
{
	system("cls");
	ShowMenuTitle();
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	printf("\r\n");
	printf("\r\n                            ");
	PrintTextWithDelays("Fumbles:");

	printf("\r\n\r\n                            ");
	PrintTextWithDelays("When the ball is fumbled each team draws 1 card");

	printf("\r\n                            ");
	PrintTextWithDelays("If they are the same suit, the ball moves ( O1 - D1 ) yards, then redraw");

	printf("\r\n                            ");
	PrintTextWithDelays("If they are the same number, then redraw");

	printf("\r\n                            ");
	PrintTextWithDelays("Otherwise the team with the highest card recovers the ball");

	printf("\r\n\r\n                            ");
	PrintTextWithDelays("Interceptions:");

	printf("\r\n                            ");
	PrintTextWithDelays("The team that intercepted the ball does a yards after catch draw like in a pass play");

	printf("\r\n\r\n                                        ");
	printf("               ");
	SetConsoleTextAttribute(hConsole, CONSOLE_BG_WHITE | CONSOLE_TEXT_GREEN);
	PrintTextWithDelays("Back");
	SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);

	int ch = getch();
	if (ch == 0 || ch == 224)
		return;
}

/*
* NAME:        ShowRulesPunting
* DESCRIPTION: show punt rules
* INPUTS:      none
* OUTPUTS:     none
*/
static void ShowRulesPunting()
{
	system("cls");
	ShowMenuTitle();
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	printf("\r\n");
	printf("\r\n                            ");
	PrintTextWithDelays("Punting:");

	printf("\r\n\r\n                            ");
	char buffer[256];
	snprintf(buffer, sizeof(buffer), "Punter draws 1-%d cards and can replace up to %d of them", PUNT_MAX_CARDS, PUNT_REDRAW_MAX_CARDS);
	PrintTextWithDelays(buffer);

	printf("\r\n                            ");
	PrintTextWithDelays("the total value of all the cards = distance punted");

	printf("\r\n\r\n                            ");
	PrintTextWithDelays("Punt returns:");

	printf("\r\n                            ");
	PrintTextWithDelays("If the kick is received in the endzone, the receiver can declare a touchback");

	printf("\r\n                            ");
	char buffer2[256];
	snprintf(buffer2, sizeof(buffer2), "otherwise the receiver draws %d cards and the punter draws %d card", PUNT_RETURN_OFFENCE_CARDS_TO_DRAW, PUNT_RETURN_DEFENCE_CARDS_TO_DRAW);
	PrintTextWithDelays(buffer2);

	printf("\r\n                            ");
	PrintTextWithDelays("(O1 + O2) - D1 = yards returned");

	printf("\r\n                            ");
	PrintTextWithDelays("If O2 is a face card AND it's bigger than D1 then its an open field run");

	printf("\r\n                            ");
	PrintTextWithDelays("An open field run allows the runner to draw cards for more yards");

	printf("\r\n                            ");
	PrintTextWithDelays("Jack = 1 card, Queen = 2 cards, King = 3 cards");

	printf("\r\n\r\n                            ");
	PrintTextWithDelays("Kickoffs work in the same way as punts");

	printf("\r\n\r\n                                        ");
	printf("               ");
	SetConsoleTextAttribute(hConsole, CONSOLE_BG_WHITE | CONSOLE_TEXT_GREEN);
	PrintTextWithDelays("Back");
	SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);

	int ch = getch();
	if (ch == 0 || ch == 224)
		return;
}

/*
* NAME:        ShowRulesFieldGoalsandXP
* DESCRIPTION: show FG and XP rules
* INPUTS:      none
* OUTPUTS:     none
*/
static void ShowRulesFieldGoalsandXP()
{
	system("cls");
	ShowMenuTitle();
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	printf("\r\n");
	printf("\r\n                            ");
	PrintTextWithDelays("Field goals:");

	printf("\r\n\r\n                            ");
	char buffer[256];
	snprintf(buffer, sizeof(buffer), "Kicker draws 1-%d cards and can replace up to %d of them", FIELD_GOAL_MAX_CARDS, FIELD_GOAL_REDRAW_MAX_CARDS);
	PrintTextWithDelays(buffer);

	printf("\r\n                            ");
	PrintTextWithDelays("the total value of all the cards = distance, if it reaches the goal its good");

	printf("\r\n                            ");
	PrintTextWithDelays("A missed field goal turns the ball over at the line of scrimmage");

	printf("\r\n\r\n                            ");
	PrintTextWithDelays("Extra points:");

	printf("\r\n                            ");
	char buffer2[256];
	snprintf(buffer2, sizeof(buffer2), "A 1 point play is a field goal from the %d yard line", (EXTRA_POINT_YARDAGE_FROM_END_ZONE + FIELD_GOAL_YARDS_AFTER_END_ZONE));
	PrintTextWithDelays(buffer2);

	printf("\r\n                            ");
	char buffer3[256];
	snprintf(buffer3, sizeof(buffer3), "A 2 point play is played from the %d yard line", TWO_POINT_CONVERSION_YARDS);
	PrintTextWithDelays(buffer3);

	printf("\r\n\r\n                                        ");
	printf("               ");
	SetConsoleTextAttribute(hConsole, CONSOLE_BG_WHITE | CONSOLE_TEXT_GREEN);
	PrintTextWithDelays("Back");
	SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);

	int ch = getch();
	if (ch == 0 || ch == 224)
		return;
}

/*
* NAME:        ShowRulesOptions
* DESCRIPTION: game menu options
* INPUTS:      current selection, delay or not
* OUTPUTS:     none
*/
static void ShowRulesOptions(int current_menu_selection, bool first_text)
{
	ResetScreen();
	ShowMenuTitle();
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	printf("\r\n\r\n                                        ");
	printf("            ");
	PrintTextWithDelays("How To Play");

	for (int i = 0; i < RULES_OPTIONS_NUMBER; i++)
	{
		printf("\r\n\r\n                                        ");
		if (i == 0)
		{
			printf("              ");
			if (current_menu_selection == i)
				SetConsoleTextAttribute(hConsole, CONSOLE_BG_WHITE | CONSOLE_TEXT_GREEN);
			if (first_text)
				PrintTextWithDelays("General");
			else
				printf("General");
		}
		else if (i == 1)
		{
			printf("            ");
			if (current_menu_selection == i)
				SetConsoleTextAttribute(hConsole, CONSOLE_BG_WHITE | CONSOLE_TEXT_GREEN);
			if (first_text)
				PrintTextWithDelays("Pass plays");
			else
				printf("Pass plays");
		}
		else if (i == 2)
		{
			printf("             ");
			if (current_menu_selection == i)
				SetConsoleTextAttribute(hConsole, CONSOLE_BG_WHITE | CONSOLE_TEXT_GREEN);
			if (first_text)
				PrintTextWithDelays("Run plays");
			else
				printf("Run plays");
		}
		else if (i == 3)
		{
			printf("          ");
			if (current_menu_selection == i)
				SetConsoleTextAttribute(hConsole, CONSOLE_BG_WHITE | CONSOLE_TEXT_GREEN);
			if (first_text)
				PrintTextWithDelays("Fumbles and INTs");
			else
				printf("Fumbles and INTs");
		}
		else if (i == 4)
		{
			printf("              ");
			if (current_menu_selection == i)
				SetConsoleTextAttribute(hConsole, CONSOLE_BG_WHITE | CONSOLE_TEXT_GREEN);
			if (first_text)
				PrintTextWithDelays("Punting");
			else
				printf("Punting");
		}
		else if (i == 5)
		{
			printf("        ");
			if (current_menu_selection == i)
				SetConsoleTextAttribute(hConsole, CONSOLE_BG_WHITE | CONSOLE_TEXT_GREEN);
			if (first_text)
				PrintTextWithDelays("Field goals and XPs");
			else
				printf("Field goals and XPs");
		}
		else if (i == 6)
		{
			printf("               ");
			if (current_menu_selection == i)
				SetConsoleTextAttribute(hConsole, CONSOLE_BG_WHITE | CONSOLE_TEXT_GREEN);
			if (first_text)
				PrintTextWithDelays("Back");
			else
				printf("Back");
		}
		SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
	}
}

/*
* NAME:        HowToPlayMenuOutcome
* DESCRIPTION: Gets which entry picked
* INPUTS:      none
* OUTPUTS:     int choice
*/
int HowToPlayMenuOutcome() {

	bool game_chosen = false;
	int current_menu_selection = 0;
	do {
		ShowRulesOptions(current_menu_selection, false);
		int ch = getch();

		if (ch == 0 || ch == 224) {   // extended key: read the scan code
			ch = getch();
			switch (ch) {
			case 72: // up arrow
				if (current_menu_selection > 0) {
					current_menu_selection--;
				}
				else
					current_menu_selection = (RULES_OPTIONS_NUMBER - 1);
				break;
			case 80: // down arrow
				if (current_menu_selection < (RULES_OPTIONS_NUMBER - 1)) {
					current_menu_selection++;
				}
				else
					current_menu_selection = 0;
				break;
			default:
				break;
			}
		}
		else if (ch == 13) {// Enter key
			game_chosen = true;
			return current_menu_selection;
		}
		else
			;
		// any other key: ignored, loop waits for another press
	} while (!game_chosen);
	return -1;
}

/*
* NAME:        ExecuteHowToPlayMenuChoice
* DESCRIPTION: play out each menu option
* INPUTS:      selection
* OUTPUTS:     none
*/
void ExecuteHowToPlayMenuChoice(int selection)
{ 
	if (selection == 0) // General
	{
		ShowRulesGeneral();
	}
	else if (selection == 1) // Pass plays
	{
		ShowRulesPass();
	}
	else if (selection == 2) // Run plays
	{
		ShowRulesRun();
	}
	else if (selection == 3) // Fumbles and ints
	{
		ShowRulesFumblesandint();
	}
	else if (selection == 4) // Punting
	{
		ShowRulesPunting();
	}
	else if (selection == 5) // Field goals and XPs
	{
		ShowRulesFieldGoalsandXP();
	}
	system("cls");
}

/*
* NAME:        HowToPlay
* DESCRIPTION: display rules and instructions
* INPUTS:      none
* OUTPUTS:     none
*/
void HowToPlay()
{
	system("cls");
	ShowMenuTitle();
	ShowRulesOptions(0, true);
	do {
		int selection = HowToPlayMenuOutcome();
		if (selection == (RULES_OPTIONS_NUMBER - 1))
			return;
		ExecuteHowToPlayMenuChoice(selection);
	} while (1);
}