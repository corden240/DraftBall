#include "main.h"
#include <windows.h>
#include <stdio.h>
#include <string.h>

void ResetScreen();
void MenuScreen();

/*
* NAME:        MenuOutcome
* DESCRIPTION: Gets which game type wants to be played
* INPUTS:      total_data
* OUTPUTS:     int choice
*/
int MenuOutcome(struct total_data* total_data) {

	bool game_chosen = false;
	do {
		MenuScreen(total_data, false);
		int ch = getch();

		if (ch == 0 || ch == 224) {   // extended key: read the scan code
			ch = getch();
			switch (ch) {
			case 72: // up arrow
				if (total_data->current_menu_selection > 0) {
					total_data->current_menu_selection--;
				}
				else
					total_data->current_menu_selection = (MENU_OPTIONS_NUMBER - 1);
				break;
			case 80: // down arrow
				if (total_data->current_menu_selection < (MENU_OPTIONS_NUMBER - 1)) {
					total_data->current_menu_selection++;
				}
				else
					total_data->current_menu_selection = 0;
				break;
			default:
				break;
			}
		}
		else if (ch == 13) {// Enter key
			game_chosen = true;
			if (total_data->current_menu_selection == (MENU_OPTIONS_NUMBER - 1))
				exit(-1);
			return total_data->current_menu_selection;
		}
		else
			;
		// any other key: ignored, loop waits for another press
	} while (!game_chosen);
	return -1;
}