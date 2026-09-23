#pragma once
#include <stdbool.h>
#include <stddef.h>

#define TIMEOUTS_PER_TEAM_PER_HALF (3)
#define STARTING_LINE_OF_SCRIMMAGE (35)
#define PLAYS_PER_QUARTER (20)
#define NUMBER_OF_QUARTERS (4)
#define NUMBER_OF_RB_NAMES_PER_TEAM (2)
#define NUMBER_OF_WR_NAMES_PER_TEAM (4)
#define NUMBER_OF_PLAYER_NAMES_PER_TEAM (NUMBER_OF_RB_NAMES_PER_TEAM + NUMBER_OF_WR_NAMES_PER_TEAM)

#define WINDOW_WIDTH  (120)
#define WINDOW_HEIGHT (50)

#define END_ZONE_WIDTH (5)
#define FIELD_WIDTH    (100)
#define PITCH_WIDTH    (END_ZONE_WIDTH + END_ZONE_WIDTH + FIELD_WIDTH)
#define FIELD_HEIGHT   (5)

#define NUMBER_OF_DECKS (3)
#define CARDS_IN_A_DECK (52)

#define VALUE       (0x00000f)
#define SUIT        (0x0000f0)
#define FACE        (0x000f00)
#define RED         (0x00f000)
#define BLACK       (0x0f0000)
#define DECK        (0xf00000)

#define ACE         (0x000001)
#define TWO         (0x000002)
#define THREE       (0x000003)
#define FOUR        (0x000004)
#define FIVE        (0x000005)
#define SIX         (0x000006)
#define SEVEN       (0x000007)
#define EIGHT       (0x000008)
#define NINE        (0x000009)
#define TEN         (0x00000a)
#define JACK        (0x00000b)
#define QUEEN       (0x00000c)
#define KING        (0x00000d)

#define CLUBS       (0x000010)
#define DIAMONDS    (0x000020)
#define HEARTS      (0x000030)
#define SPADES      (0x000040)

#define DECK1       (0x100000)
#define DECK2       (0x200000)
#define DECK_SHARED (0x300000)

#define CONSOLE_TEXT_BLUE	   (1)
#define CONSOLE_TEXT_GREEN	   (2)
#define CONSOLE_TEXT_LIGHTBLUE (3)
#define CONSOLE_TEXT_RED       (4)
#define CONSOLE_TEXT_PURPLE    (5)
#define CONSOLE_TEXT_YELLOW    (6)
#define CONSOLE_TEXT_WHITE     (7)
#define CONSOLE_TEXT_GREY      (8)

enum play_types {
	PLAY_TYPE_NORMAL = 1,
	PLAY_TYPE_PUNT = 2,
	PLAY_TYPE_FIELD_GOAL = 3,
	PLAY_TYPE_KNEEL = 4,
	PLAY_TYPE_TIMEOUT = 5
};

struct response {
	int line_of_scrimmage;
	int line_of_first_down;
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
	char team_1_name[32];
	char team_2_name[32];
	int team_1_direction; // 1 if team 1 is going left to right, 2 if team 1 is going right to left
	int team_2_direction; // 1 if team 2 is going left to right, 2 if team 2 is going right to left
	char team_1_rb_names[2][32];
	char team_2_rb_names[2][32];
	char team_1_wr_names[4][32];
	char team_2_wr_names[4][32];
	bool players_named;
	int o_cards[CARDS_IN_A_DECK];
	int d_cards[CARDS_IN_A_DECK];
};

struct table_weight {
	int val;
	int weight;
	bool on_off;
};

/* Prototypes */
void ConsoleShowScreen(struct response* res, char txt[1000], int pause);
void TableWeightEnableAll(struct table_weight* table);
int TableWeightDecision(struct table_weight* table, bool enable_all);