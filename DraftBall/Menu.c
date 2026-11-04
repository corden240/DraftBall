#include "main.h"
#include <windows.h>
#include <stdio.h>
#include <string.h>

void ResetScreen();
void MenuScreen(bool first_text, int menu_selection);

/*
* NAME:        MenuOutcome
* DESCRIPTION: Gets which game type wants to be played
* INPUTS:      none
* OUTPUTS:     int choice
*/
int MenuOutcome(int menu_selection) {

	bool game_chosen = false;
	do {
		MenuScreen(false, menu_selection);
		int ch = getch();

		if (ch == 0 || ch == 224) {   // extended key: read the scan code
			ch = getch();
			switch (ch) {
			case 72: // up arrow
				if (menu_selection > 0) {
					menu_selection--;
				}
				else
					menu_selection = (MENU_OPTIONS_NUMBER - 1);
				break;
			case 80: // down arrow
				if (menu_selection < (MENU_OPTIONS_NUMBER - 1)) {
					menu_selection++;
				}
				else
					menu_selection = 0;
				break;
			default:
				break;
			}
		}
		else if (ch == 13) {// Enter key
			game_chosen = true;
			if (menu_selection == (MENU_OPTIONS_NUMBER - 1))
				exit(-1);
			return menu_selection;
		}
		else
			;
		// any other key: ignored, loop waits for another press
	} while (!game_chosen);
	return -1;
}