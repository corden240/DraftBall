#include <stdio.h>
#include <stdlib.h>   /* srand, rand, atoi */
#include <time.h>     /* time() */
#include "main.h"
#include <string.h>


/*
* NAME:        SetTeamColoursSeason
* DESCRIPTION: setup team colours
* INPUTS:      game_data
* OUTPUTS:     none
*/
void SetTeamColoursSeason(struct game_data* game_data)
{
	ConsoleShowGameScreen(game_data, "Would you like to pick team colours? y/n ", 0);
	char input[16];
	fgets(input, sizeof(input), stdin);

	if (input[0] == 'Y' || input[0] == 'y')
	{
		bool team_1_colour_done = false;
		bool team_2_colour_done = false;
		do {
			ConsoleShowGameScreen(game_data, "Enter colour for Team 1 (1 - 8): ", 0);
			char input1[16];
			fgets(input1, sizeof(input1), stdin);
			int team_1_colour = atoi(input1);
			if (team_1_colour < 1)
				team_1_colour = 1;
			else if (team_1_colour > 8)
				team_1_colour = 8;
			game_data->team_1_colour = team_1_colour;

			ConsoleShowGameScreen(game_data, "Are you happy with this colour? y/n ", 0);
			char input2[16];
			fgets(input2, sizeof(input2), stdin);
			if (input2[0] == 'Y' || input2[0] == 'y')
				team_1_colour_done = true;
		} while (!team_1_colour_done);

		do {
			ConsoleShowGameScreen(game_data, "Enter colour for Team 2 (1 - 8): ", 0);
			char input3[16];
			fgets(input3, sizeof(input3), stdin);
			int team_2_colour = atoi(input3);
			if (team_2_colour < 1)
				team_2_colour = 1;
			else if (team_2_colour > 8)
				team_2_colour = 8;
			game_data->team_2_colour = team_2_colour;

			ConsoleShowGameScreen(game_data, "Are you happy with this colour? y/n ", 0);
			char input4[16];
			fgets(input4, sizeof(input4), stdin);
			if (input4[0] == 'Y' || input4[0] == 'y')
				team_2_colour_done = true;
		} while (!team_2_colour_done);
	}
	else
		game_data->players_named = false;
}

/*
* NAME:        SetTeamNamesSeason
* DESCRIPTION: setup team names
* INPUTS:      game_data
* OUTPUTS:     none
*/
void SetTeamNamesSeason(struct game_data* game_data)
{
	ConsoleShowGameScreen(game_data, "Would you like to name your teams? y/n ", 0);
	char input[16];
	fgets(input, sizeof(input), stdin);

	if (input[0] == 'Y' || input[0] == 'y')
	{
		strcpy(game_data->team_1_name, " ");
		strcpy(game_data->team_2_name, " ");

		ConsoleShowGameScreen(game_data, "Enter name for Team 1: ", 0);
		fgets(game_data->team_1_name, sizeof(game_data->team_1_name), stdin);
		game_data->team_1_name[strcspn(game_data->team_1_name, "\n")] = '\0';
		ConsoleShowGameScreen(game_data, "Enter name for Team 2: ", 0);
		fgets(game_data->team_2_name, sizeof(game_data->team_2_name), stdin);
		game_data->team_2_name[strcspn(game_data->team_2_name, "\n")] = '\0';
		ConsoleShowGameScreen(game_data, 0, 0);
	}
	else
		game_data->players_named = false;
}

/*
* NAME:        SetPlayerNamesSeason
* DESCRIPTION: setup player names
* INPUTS:      game_data
* OUTPUTS:     none
*/
void SetPlayerNamesSeason(struct game_data* game_data)
{
	ConsoleShowGameScreen(game_data, "Would you like to name your players? y/n ", 0);
	char input[16];
	fgets(input, sizeof(input), stdin);

	if (input[0] == 'Y' || input[0] == 'y')
	{
		game_data->players_named = true;
		strcpy(game_data->team_1_qb_names[0], " ");
		strcpy(game_data->team_2_qb_names[0], " ");

		strcpy(game_data->team_1_rb_names[0], " ");
		strcpy(game_data->team_2_rb_names[0], " ");
		strcpy(game_data->team_1_rb_names[1], " ");
		strcpy(game_data->team_2_rb_names[1], " ");

		strcpy(game_data->team_1_wr_names[0], " ");
		strcpy(game_data->team_2_wr_names[0], " ");
		strcpy(game_data->team_1_wr_names[1], " ");
		strcpy(game_data->team_2_wr_names[1], " ");
		strcpy(game_data->team_1_wr_names[2], " ");
		strcpy(game_data->team_2_wr_names[2], " ");
		strcpy(game_data->team_1_wr_names[3], " ");
		strcpy(game_data->team_2_wr_names[3], " ");

		for (int i = 0; i < NUMBER_OF_PLAYER_NAMES_PER_TEAM; i++)
		{
			if (i == 0)
			{
				char buf[256];
				snprintf(buf, sizeof(buf), "Enter name for Team 1 QB Player %d: ", i + 1);
				ConsoleShowGameScreen(game_data, buf, 0);
				fgets(game_data->team_1_qb_names[i], sizeof(game_data->team_1_qb_names[i]), stdin);
				game_data->team_1_qb_names[i][strcspn(game_data->team_1_qb_names[i], "\n")] = '\0';
			}
			else if (i < (NUMBER_OF_QB_NAMES_PER_TEAM + NUMBER_OF_RB_NAMES_PER_TEAM))
			{
				char buf[256];
				snprintf(buf, sizeof(buf), "Enter name for Team 1 RB Player %d: ", i);
				ConsoleShowGameScreen(game_data, buf, 0);
				fgets(game_data->team_1_rb_names[i - NUMBER_OF_QB_NAMES_PER_TEAM], sizeof(game_data->team_1_rb_names[i - NUMBER_OF_QB_NAMES_PER_TEAM]), stdin);
				game_data->team_1_rb_names[i - NUMBER_OF_QB_NAMES_PER_TEAM][strcspn(game_data->team_1_rb_names[i - NUMBER_OF_QB_NAMES_PER_TEAM], "\n")] = '\0';
			}
			else
			{
				char buf[256];
				snprintf(buf, sizeof(buf), "Enter name for Team 1 WR Player %d: ", i - (NUMBER_OF_QB_NAMES_PER_TEAM + NUMBER_OF_RB_NAMES_PER_TEAM) + 1);
				ConsoleShowGameScreen(game_data, buf, 0);
				fgets(game_data->team_1_wr_names[i - (NUMBER_OF_QB_NAMES_PER_TEAM + NUMBER_OF_RB_NAMES_PER_TEAM)], sizeof(game_data->team_1_wr_names[i - (NUMBER_OF_QB_NAMES_PER_TEAM + NUMBER_OF_RB_NAMES_PER_TEAM)]), stdin);
				game_data->team_1_wr_names[i - (NUMBER_OF_QB_NAMES_PER_TEAM + NUMBER_OF_RB_NAMES_PER_TEAM)][strcspn(game_data->team_1_wr_names[i - (NUMBER_OF_QB_NAMES_PER_TEAM + NUMBER_OF_RB_NAMES_PER_TEAM)], "\n")] = '\0';
			}
		}
		for (int i = 0; i < NUMBER_OF_PLAYER_NAMES_PER_TEAM; i++)
		{
			if (i == 0)
			{
				char buf[256];
				snprintf(buf, sizeof(buf), "Enter name for Team 2 QB Player %d: ", i + 1);
				ConsoleShowGameScreen(game_data, buf, 0);
				fgets(game_data->team_2_qb_names[i], sizeof(game_data->team_2_qb_names[i]), stdin);
				game_data->team_2_qb_names[i][strcspn(game_data->team_2_qb_names[i], "\n")] = '\0';
			}
			else if (i < (NUMBER_OF_QB_NAMES_PER_TEAM + NUMBER_OF_RB_NAMES_PER_TEAM))
			{
				char buf[256];
				snprintf(buf, sizeof(buf), "Enter name for Team 2 RB Player %d: ", i);
				ConsoleShowGameScreen(game_data, buf, 0);
				fgets(game_data->team_2_rb_names[i - NUMBER_OF_QB_NAMES_PER_TEAM], sizeof(game_data->team_2_rb_names[i - NUMBER_OF_QB_NAMES_PER_TEAM]), stdin);
				game_data->team_2_rb_names[i - NUMBER_OF_QB_NAMES_PER_TEAM][strcspn(game_data->team_2_rb_names[i - NUMBER_OF_QB_NAMES_PER_TEAM], "\n")] = '\0';
			}
			else
			{
				char buf[256];
				snprintf(buf, sizeof(buf), "Enter name for Team 2 WR Player %d: ", i - (NUMBER_OF_QB_NAMES_PER_TEAM + NUMBER_OF_RB_NAMES_PER_TEAM) + 1);
				ConsoleShowGameScreen(game_data, buf, 0);
				fgets(game_data->team_2_wr_names[i - (NUMBER_OF_QB_NAMES_PER_TEAM + NUMBER_OF_RB_NAMES_PER_TEAM)], sizeof(game_data->team_2_wr_names[i - (NUMBER_OF_QB_NAMES_PER_TEAM + NUMBER_OF_RB_NAMES_PER_TEAM)]), stdin);
				game_data->team_2_wr_names[i - (NUMBER_OF_QB_NAMES_PER_TEAM + NUMBER_OF_RB_NAMES_PER_TEAM)][strcspn(game_data->team_2_wr_names[i - (NUMBER_OF_QB_NAMES_PER_TEAM + NUMBER_OF_RB_NAMES_PER_TEAM)], "\n")] = '\0';
			}
		}
	}
	else
	{
		game_data->players_named = false;
	}
	ConsoleShowGameScreen(game_data, 0, 0);
}



/*
* NAME:        SeasonDataConstructor
* DESCRIPTION: starts all of season_data data to default settings
* INPUTS:      game_data
* OUTPUTS:     none
*/
void SeasonDataConstructor(struct season_data* season_data, struct total_data* total_data)
{
	season_data->fouls = total_data->fouls;
	season_data->current_game_mode = total_data->current_game_mode;
	season_data->plays_per_quarter = total_data->plays_per_quarter;
}


/*
* NAME:        SetupSeason
* DESCRIPTION: initialises season data
* INPUTS:      game_data
* OUTPUTS:     none
*/
void SetupSeason(struct season_data* season_data, struct total_data* total_data)
{
	SeasonDataConstructor(season_data, total_data);

	SetTeamNamesSeason(season_data);

	SetTeamColoursSeason(season_data);

	SetPlayerNamesSeason(season_data);
}
