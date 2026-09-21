#include <math.h>
#include "main.h"
#include "main_d.c"

void Play()
{
    // game loop for each quarter

	// if quarter 1 or 3, do punt
	// if quarter 3 reset timeouts etc
	// do punt return

	// do play

}

void SetupGame()
{
    // Ask team names and players etc
    // Coin toss for start, res->team_started_with_possession = 0;
    // anything that only happens once at the start of the game
	// draw field and standard setup ready for starting punt
}

void ResponseConstructor(struct response* res)
{
	res->line_of_scrimmage = STARTING_LINE_OF_SCRIMMAGE;
	res->down = 1;
	res->team_in_possession = 0;
	res->score_team_1 = 0;
	res->score_team_2 = 0;
	res->quarter = 1;
	res->plays_remaining_in_quarter = PLAYS_PER_QUARTER;
	int i;
	for (i = 0; i < NUMBER_OF_DECKS; i++) {
		res->shuffle_deck_needed[i] = 0;
	}
	res->replay_game = false;
	res->team_1_timeouts_remaining = TIMEOUTS_PER_TEAM_PER_HALF;
	res->team_2_timeouts_remaining = TIMEOUTS_PER_TEAM_PER_HALF;
	res->team_started_with_possession = 0;
}

void EndOfGame()
{
	// display final score and winner
	// replay game?
}

void main()
{
    // starts dos window at right size
    struct response res;
    ResponseConstructor(&res);
    SetupGame();
	ConsoleShowScreen(&res, "Welcome to the game!", 0, 0);

    Play();

    EndOfGame();
}