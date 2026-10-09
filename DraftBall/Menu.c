#include "main.h"
#include <windows.h>
#include <stdio.h>
#include <string.h>

void ResetScreen();
void MenuScreen();

/*
* NAME:        MenuChoicesConstructor
* DESCRIPTION: starts all of menu_choices data to default settings
* INPUTS:      response
* OUTPUTS:     None
*/
void MenuChoicesConstructor(struct menu_choices* menu)
{
	menu->current_selection = 0;
}

/*
* NAME:        MenuOutcome
* DESCRIPTION: Gets which game type wants to be played
* INPUTS:      None
* OUTPUTS:     None
*/
int MenuOutcome(struct menu_choices* menu) {

	bool game_chosen = false;
	do {
		MenuScreen(menu, false);
		int ch = getch();

		if (ch == 0 || ch == 224) {   // extended key: read the scan code
			ch = getch();
			switch (ch) {
			case 72: // up arrow
				if (menu->current_selection > 0) {
					menu->current_selection--;
				}
				break;
			case 80: // down arrow
				if (menu->current_selection < (MENU_OPTIONS_NUMBER - 1)) {
					menu->current_selection++;
				}
				break;
			}
		}
		else if (ch == 13) {          // Enter key
			game_chosen = true;
			if (menu->current_selection == (MENU_OPTIONS_NUMBER - 1))
				exit(-1);
			return menu->current_selection;
		}
		// any other key: ignored, loop waits for another press
	} while (!game_chosen);
}