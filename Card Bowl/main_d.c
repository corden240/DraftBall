#include "main.h"

struct response {
	int line_of_scrimmage;
	int down;
	int team_in_possession;
	int score_team_1;
	int score_team_2;
	int quarter;
	int plays_remaining_in_quarter;
	int shuffle_deck_needed[NUMBER_OF_DECKS]; // int 1 if that deck needs to be shuffled, 0 if not
	bool replay_game;
	int team_1_timeouts_remaining;
	int team_2_timeouts_remaining;
	int team_started_with_possession; // 1 if team 1, 2 if team 2
	char team_1_name[20];
	char team_2_name[20];
};

struct table_weight {
	int val;
	int weight;
	bool on_off;
};