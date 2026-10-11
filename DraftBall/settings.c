#include "main.h"
#include <windows.h>
#include <stdio.h>
#include <string.h>

void ShowMenuTitle();
void PrintTextWithDelays(char txt[1000]);
void ResetScreen();

/*
* NAME:        ShowSettingsOptions
* DESCRIPTION: game settings options
* INPUTS:      current selection, delay or not
* OUTPUTS:     none
*/
static void ShowSettingsOptions(int current_menu_selection, bool first_text, struct total_data* total_data)
{
	ResetScreen();
	ShowMenuTitle();
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	printf("\r\n\r\n                                        ");
	printf("             ");
	PrintTextWithDelays("Settings");

	for (int i = 0; i < RULES_OPTIONS_NUMBER; i++)
	{
		printf("\r\n\r\n                                        ");
		if (i == 0)
		{
			printf("         ");
			if (current_menu_selection == i)
				SetConsoleTextAttribute(hConsole, CONSOLE_BG_WHITE | CONSOLE_TEXT_GREEN);
			if (first_text)
				PrintTextWithDelays("Fouls");
			else
				printf("Fouls");
			SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
			printf("    ");
			if(total_data->fouls)
				SetConsoleTextAttribute(hConsole, CONSOLE_BG_WHITE | CONSOLE_TEXT_GREEN);
			printf("On");
			SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
			printf(" / ");
			if (!total_data->fouls)
				SetConsoleTextAttribute(hConsole, CONSOLE_BG_WHITE | CONSOLE_TEXT_GREEN);
			printf("Off");
		}
		else if (i == 1)
		{
			printf("           ");
			if (current_menu_selection == i)
				SetConsoleTextAttribute(hConsole, CONSOLE_BG_WHITE | CONSOLE_TEXT_GREEN);
			if (first_text)
				PrintTextWithDelays("Back (Save)");
			else
				printf("Back (Save)");
		}
		SetConsoleTextAttribute(hConsole, CONSOLE_TEXT_WHITE);
	}
}

/*
* NAME:        SettingsMenuOutcome
* DESCRIPTION: Gets which entry picked
* INPUTS:      none
* OUTPUTS:     int choice
*/
int SettingsMenuOutcome(struct total_data* total_data) {

	bool game_chosen = false;
	int current_menu_selection = 0;
	do {
		ShowSettingsOptions(current_menu_selection, false, total_data);
		int ch = getch();

		if (ch == 0 || ch == 224) {   // extended key: read the scan code
			ch = getch();
			switch (ch) {
			case 72: // up arrow
				if (current_menu_selection > 0) {
					current_menu_selection--;
				}
				else
					current_menu_selection = (SETTINGS_OPTIONS_NUMBER - 1);
				break;
			case 80: // down arrow
				if (current_menu_selection < (SETTINGS_OPTIONS_NUMBER - 1)) {
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
}

/*
* NAME:        ExecuteSettingsMenuChoice
* DESCRIPTION: play out each menu option
* INPUTS:      selection
* OUTPUTS:     none
*/
void ExecuteSettingsMenuChoice(int selection, struct total_data* total_data)
{
	if (selection == 0) // fouls
	{
		if(total_data->fouls)
			total_data->fouls = false;
		else
			total_data->fouls = true;
	}
	system("cls");
}

/*
* NAME:        Settings
* DESCRIPTION: display settings and resolve changes
* INPUTS:      none
* OUTPUTS:     none
*/
void Settings(struct total_data* total_data)
{
	total_data->fouls;
	system("cls");
	ShowMenuTitle();
	ShowSettingsOptions(0, true, total_data);
	do {
		int selection = SettingsMenuOutcome(total_data);
		if (selection == (SETTINGS_OPTIONS_NUMBER - 1))
			return;
		ExecuteSettingsMenuChoice(selection, total_data);
	} while (1);
}