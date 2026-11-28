#include <stdio.h>
#include <stdlib.h>   /* srand, rand, atoi */
#include <time.h>     /* time() */
#include "main.h"
#include <string.h>
#include <windows.h>
#include "teamdata.h"


/*
* NAME:        PlayPostSeason
* DESCRIPTION: play out post season games
* INPUTS:      season data
* OUTPUTS:     none
*/
void PlayPostSeason(struct season_data* season_data)
{
	// use conference leaderboards to get playoff teams and put then into a bracket, store brackets and teams data

	for (int post_season_game_no = 0; post_season_game_no <= SEASON_PLAYOFF_NUMBER_OF_GAMES; post_season_game_no++)
	{
		// Show default screen between games (shows playoff bracket) with options "Play next round", "Sim next round", "Show leaderboard", "Quit"
		// Get menu choice from that default screen^

		//if (play_or_sim_or_quit == (SEASON_PLAYOFFS_MENU_OPTIONS_NUMBER - 1)) // "Quit"
			//return -1;

		// if team out of playoffs then skip:
		// else if "play game" then play game
		// else if sim game then sim game
		// 
		// sim rest of playoff games, dont allow for a draw, store scores for bracket view
	}

	// Show default screen that was between playoff games (shows playoff bracket) with options "Show leaderboard" and "Quit"
}