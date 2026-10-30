#include "main.h"

/* per-player decks (terminated by entry with weight < 0) */
struct table_weight player_1_deck[] = {
	{(ACE   |    CLUBS | BLACK        | DECK1), 100, true},
	{(TWO   |    CLUBS | BLACK        | DECK1), 100, true},
	{(THREE |    CLUBS | BLACK        | DECK1), 100, true},
	{(FOUR  |    CLUBS | BLACK        | DECK1), 100, true},
	{(FIVE  |    CLUBS | BLACK        | DECK1), 100, true},
	{(SIX   |    CLUBS | BLACK        | DECK1), 100, true},
	{(SEVEN |    CLUBS | BLACK        | DECK1), 100, true},
	{(EIGHT |    CLUBS | BLACK        | DECK1), 100, true},
	{(NINE  |    CLUBS | BLACK        | DECK1), 100, true},
	{(TEN   |    CLUBS | BLACK        | DECK1), 100, true},
	{(JACK  |    CLUBS | BLACK | FACE | DECK1), 100, true},
	{(QUEEN |    CLUBS | BLACK | FACE | DECK1), 100, true},
	{(KING  |    CLUBS | BLACK | FACE | DECK1), 100, true},

	{(ACE   | DIAMONDS |   RED        | DECK1), 100, true},
	{(TWO   | DIAMONDS |   RED        | DECK1), 100, true},
	{(THREE | DIAMONDS |   RED        | DECK1), 100, true},
	{(FOUR  | DIAMONDS |   RED        | DECK1), 100, true},
	{(FIVE  | DIAMONDS |   RED        | DECK1), 100, true},
	{(SIX   | DIAMONDS |   RED        | DECK1), 100, true},
	{(SEVEN | DIAMONDS |   RED        | DECK1), 100, true},
	{(EIGHT | DIAMONDS |   RED        | DECK1), 100, true},
	{(NINE  | DIAMONDS |   RED        | DECK1), 100, true},
	{(TEN   | DIAMONDS |   RED        | DECK1), 100, true},
	{(JACK  | DIAMONDS |   RED | FACE | DECK1), 100, true},
	{(QUEEN | DIAMONDS |   RED | FACE | DECK1), 100, true},
	{(KING  | DIAMONDS |   RED | FACE | DECK1), 100, true},

	{(ACE   |   HEARTS |   RED        | DECK1), 100, true},
	{(TWO   |   HEARTS |   RED        | DECK1), 100, true},
	{(THREE |   HEARTS |   RED        | DECK1), 100, true},
	{(FOUR  |   HEARTS |   RED        | DECK1), 100, true},
	{(FIVE  |   HEARTS |   RED        | DECK1), 100, true},
	{(SIX   |   HEARTS |   RED        | DECK1), 100, true},
	{(SEVEN |   HEARTS |   RED        | DECK1), 100, true},
	{(EIGHT |   HEARTS |   RED        | DECK1), 100, true},
	{(NINE  |   HEARTS |   RED        | DECK1), 100, true},
	{(TEN   |   HEARTS |   RED        | DECK1), 100, true},
	{(JACK  |   HEARTS |   RED | FACE | DECK1), 100, true},
	{(QUEEN |   HEARTS |   RED | FACE | DECK1), 100, true},
	{(KING  |   HEARTS |   RED | FACE | DECK1), 100, true},

	{(ACE   |   SPADES | BLACK        | DECK1), 100, true},
	{(TWO   |   SPADES | BLACK        | DECK1), 100, true},
	{(THREE |   SPADES | BLACK        | DECK1), 100, true},
	{(FOUR  |   SPADES | BLACK        | DECK1), 100, true},
	{(FIVE  |   SPADES | BLACK        | DECK1), 100, true},
	{(SIX   |   SPADES | BLACK        | DECK1), 100, true},
	{(SEVEN |   SPADES | BLACK        | DECK1), 100, true},
	{(EIGHT |   SPADES | BLACK        | DECK1), 100, true},
	{(NINE  |   SPADES | BLACK        | DECK1), 100, true},
	{(TEN   |   SPADES | BLACK        | DECK1), 100, true},
	{(JACK  |   SPADES | BLACK | FACE | DECK1), 100, true},
	{(QUEEN |   SPADES | BLACK | FACE | DECK1), 100, true},
	{(KING  |   SPADES | BLACK | FACE | DECK1), 100, true},

	{0, -1, false}
};

struct table_weight player_2_deck[] = {
	{(ACE   |    CLUBS | BLACK        | DECK2), 100, true},
	{(TWO   |    CLUBS | BLACK        | DECK2), 100, true},
	{(THREE |    CLUBS | BLACK        | DECK2), 100, true},
	{(FOUR  |    CLUBS | BLACK        | DECK2), 100, true},
	{(FIVE  |    CLUBS | BLACK        | DECK2), 100, true},
	{(SIX   |    CLUBS | BLACK        | DECK2), 100, true},
	{(SEVEN |    CLUBS | BLACK        | DECK2), 100, true},
	{(EIGHT |    CLUBS | BLACK        | DECK2), 100, true},
	{(NINE  |    CLUBS | BLACK        | DECK2), 100, true},
	{(TEN   |    CLUBS | BLACK        | DECK2), 100, true},
	{(JACK  |    CLUBS | BLACK | FACE | DECK2), 100, true},
	{(QUEEN |    CLUBS | BLACK | FACE | DECK2), 100, true},
	{(KING  |    CLUBS | BLACK | FACE | DECK2), 100, true},

	{(ACE   | DIAMONDS |   RED        | DECK2), 100, true},
	{(TWO   | DIAMONDS |   RED        | DECK2), 100, true},
	{(THREE | DIAMONDS |   RED        | DECK2), 100, true},
	{(FOUR  | DIAMONDS |   RED        | DECK2), 100, true},
	{(FIVE  | DIAMONDS |   RED        | DECK2), 100, true},
	{(SIX   | DIAMONDS |   RED        | DECK2), 100, true},
	{(SEVEN | DIAMONDS |   RED        | DECK2), 100, true},
	{(EIGHT | DIAMONDS |   RED        | DECK2), 100, true},
	{(NINE  | DIAMONDS |   RED        | DECK2), 100, true},
	{(TEN   | DIAMONDS |   RED        | DECK2), 100, true},
	{(JACK  | DIAMONDS |   RED | FACE | DECK2), 100, true},
	{(QUEEN | DIAMONDS |   RED | FACE | DECK2), 100, true},
	{(KING  | DIAMONDS |   RED | FACE | DECK2), 100, true},

	{(ACE   |   HEARTS |   RED        | DECK2), 100, true},
	{(TWO   |   HEARTS |   RED        | DECK2), 100, true},
	{(THREE |   HEARTS |   RED        | DECK2), 100, true},
	{(FOUR  |   HEARTS |   RED        | DECK2), 100, true},
	{(FIVE  |   HEARTS |   RED        | DECK2), 100, true},
	{(SIX   |   HEARTS |   RED        | DECK2), 100, true},
	{(SEVEN |   HEARTS |   RED        | DECK2), 100, true},
	{(EIGHT |   HEARTS |   RED        | DECK2), 100, true},
	{(NINE  |   HEARTS |   RED        | DECK2), 100, true},
	{(TEN   |   HEARTS |   RED        | DECK2), 100, true},
	{(JACK  |   HEARTS |   RED | FACE | DECK2), 100, true},
	{(QUEEN |   HEARTS |   RED | FACE | DECK2), 100, true},
	{(KING  |   HEARTS |   RED | FACE | DECK2), 100, true},

	{(ACE   |   SPADES | BLACK        | DECK2), 100, true},
	{(TWO   |   SPADES | BLACK        | DECK2), 100, true},
	{(THREE |   SPADES | BLACK        | DECK2), 100, true},
	{(FOUR  |   SPADES | BLACK        | DECK2), 100, true},
	{(FIVE  |   SPADES | BLACK        | DECK2), 100, true},
	{(SIX   |   SPADES | BLACK        | DECK2), 100, true},
	{(SEVEN |   SPADES | BLACK        | DECK2), 100, true},
	{(EIGHT |   SPADES | BLACK        | DECK2), 100, true},
	{(NINE  |   SPADES | BLACK        | DECK2), 100, true},
	{(TEN   |   SPADES | BLACK        | DECK2), 100, true},
	{(JACK  |   SPADES | BLACK | FACE | DECK2), 100, true},
	{(QUEEN |   SPADES | BLACK | FACE | DECK2), 100, true},
	{(KING  |   SPADES | BLACK | FACE | DECK2), 100, true},

	{0, -1, false}
};

/* array of pointers to player decks */
struct table_weight* player_decks[] = {
	player_1_deck,
	player_2_deck
};

struct table_weight shared_deck[] = {
	{(ACE   |    CLUBS | BLACK        | DECK_SHARED), 100, true},
	{(TWO   |    CLUBS | BLACK        | DECK_SHARED), 100, true},
	{(THREE |    CLUBS | BLACK        | DECK_SHARED), 100, true},
	{(FOUR  |    CLUBS | BLACK        | DECK_SHARED), 100, true},
	{(FIVE  |    CLUBS | BLACK        | DECK_SHARED), 100, true},
	{(SIX   |    CLUBS | BLACK        | DECK_SHARED), 100, true},
	{(SEVEN |    CLUBS | BLACK        | DECK_SHARED), 100, true},
	{(EIGHT |    CLUBS | BLACK        | DECK_SHARED), 100, true},
	{(NINE  |    CLUBS | BLACK        | DECK_SHARED), 100, true},
	{(TEN   |    CLUBS | BLACK        | DECK_SHARED), 100, true},
	{(JACK  |    CLUBS | BLACK | FACE | DECK_SHARED), 100, true},
	{(QUEEN |    CLUBS | BLACK | FACE | DECK_SHARED), 100, true},
	{(KING  |    CLUBS | BLACK | FACE | DECK_SHARED), 100, true},

	{(ACE   | DIAMONDS |   RED        | DECK_SHARED), 100, true},
	{(TWO   | DIAMONDS |   RED        | DECK_SHARED), 100, true},
	{(THREE | DIAMONDS |   RED        | DECK_SHARED), 100, true},
	{(FOUR  | DIAMONDS |   RED        | DECK_SHARED), 100, true},
	{(FIVE  | DIAMONDS |   RED        | DECK_SHARED), 100, true},
	{(SIX   | DIAMONDS |   RED        | DECK_SHARED), 100, true},
	{(SEVEN | DIAMONDS |   RED        | DECK_SHARED), 100, true},
	{(EIGHT | DIAMONDS |   RED        | DECK_SHARED), 100, true},
	{(NINE  | DIAMONDS |   RED        | DECK_SHARED), 100, true},
	{(TEN   | DIAMONDS |   RED        | DECK_SHARED), 100, true},
	{(JACK  | DIAMONDS |   RED | FACE | DECK_SHARED), 100, true},
	{(QUEEN | DIAMONDS |   RED | FACE | DECK_SHARED), 100, true},
	{(KING  | DIAMONDS |   RED | FACE | DECK_SHARED), 100, true},

	{(ACE   |   HEARTS |   RED        | DECK_SHARED), 100, true},
	{(TWO   |   HEARTS |   RED        | DECK_SHARED), 100, true},
	{(THREE |   HEARTS |   RED        | DECK_SHARED), 100, true},
	{(FOUR  |   HEARTS |   RED        | DECK_SHARED), 100, true},
	{(FIVE  |   HEARTS |   RED        | DECK_SHARED), 100, true},
	{(SIX   |   HEARTS |   RED        | DECK_SHARED), 100, true},
	{(SEVEN |   HEARTS |   RED        | DECK_SHARED), 100, true},
	{(EIGHT |   HEARTS |   RED        | DECK_SHARED), 100, true},
	{(NINE  |   HEARTS |   RED        | DECK_SHARED), 100, true},
	{(TEN   |   HEARTS |   RED        | DECK_SHARED), 100, true},
	{(JACK  |   HEARTS |   RED | FACE | DECK_SHARED), 100, true},
	{(QUEEN |   HEARTS |   RED | FACE | DECK_SHARED), 100, true},
	{(KING  |   HEARTS |   RED | FACE | DECK_SHARED), 100, true},

	{(ACE   |   SPADES | BLACK        | DECK_SHARED), 100, true},
	{(TWO   |   SPADES | BLACK        | DECK_SHARED), 100, true},
	{(THREE |   SPADES | BLACK        | DECK_SHARED), 100, true},
	{(FOUR  |   SPADES | BLACK        | DECK_SHARED), 100, true},
	{(FIVE  |   SPADES | BLACK        | DECK_SHARED), 100, true},
	{(SIX   |   SPADES | BLACK        | DECK_SHARED), 100, true},
	{(SEVEN |   SPADES | BLACK        | DECK_SHARED), 100, true},
	{(EIGHT |   SPADES | BLACK        | DECK_SHARED), 100, true},
	{(NINE  |   SPADES | BLACK        | DECK_SHARED), 100, true},
	{(TEN   |   SPADES | BLACK        | DECK_SHARED), 100, true},
	{(JACK  |   SPADES | BLACK | FACE | DECK_SHARED), 100, true},
	{(QUEEN |   SPADES | BLACK | FACE | DECK_SHARED), 100, true},
	{(KING  |   SPADES | BLACK | FACE | DECK_SHARED), 100, true},

	{0, -1, false}
};

struct card_name card_names[] =
{
	{(ACE   |    CLUBS), "Ac"},
	{(TWO   |    CLUBS), "2c"},
	{(THREE |    CLUBS), "3c"},
	{(FOUR  |    CLUBS), "4c"},
	{(FIVE  |    CLUBS), "5c"},
	{(SIX   |    CLUBS), "6c"},
	{(SEVEN |    CLUBS), "7c"},
	{(EIGHT |    CLUBS), "8c"},
	{(NINE  |    CLUBS), "9c"},
	{(TEN   |    CLUBS), "Tc"},
	{(JACK  |    CLUBS), "Jc"},
	{(QUEEN |    CLUBS), "Qc"},
	{(KING  |    CLUBS), "Kc"},

	{(ACE   | DIAMONDS), "Ad"},
	{(TWO   | DIAMONDS), "2d"},
	{(THREE | DIAMONDS), "3d"},
	{(FOUR  | DIAMONDS), "4d"},
	{(FIVE  | DIAMONDS), "5d"},
	{(SIX   | DIAMONDS), "6d"},
	{(SEVEN | DIAMONDS), "7d"},
	{(EIGHT | DIAMONDS), "8d"},
	{(NINE  | DIAMONDS), "9d"},
	{(TEN   | DIAMONDS), "Td"},
	{(JACK  | DIAMONDS), "Jd"},
	{(QUEEN | DIAMONDS), "Qd"},
	{(KING  | DIAMONDS), "Kd"},

	{(ACE   |   HEARTS), "Ah"},
	{(TWO   |   HEARTS), "2h"},
	{(THREE |   HEARTS), "3h"},
	{(FOUR  |   HEARTS), "4h"},
	{(FIVE  |   HEARTS), "5h"},
	{(SIX   |   HEARTS), "6h"},
	{(SEVEN |   HEARTS), "7h"},
	{(EIGHT |   HEARTS), "8h"},
	{(NINE  |   HEARTS), "9h"},
	{(TEN   |   HEARTS), "Th"},
	{(JACK  |   HEARTS), "Jh"},
	{(QUEEN |   HEARTS), "Qh"},
	{(KING  |   HEARTS), "Kh"},

	{(ACE   |   SPADES), "As"},
	{(TWO   |   SPADES), "2s"},
	{(THREE |   SPADES), "3s"},
	{(FOUR  |   SPADES), "4s"},
	{(FIVE  |   SPADES), "5s"},
	{(SIX   |   SPADES), "6s"},
	{(SEVEN |   SPADES), "7s"},
	{(EIGHT |   SPADES), "8s"},
	{(NINE  |   SPADES), "9s"},
	{(TEN   |   SPADES), "Ts"},
	{(JACK  |   SPADES), "Js"},
	{(QUEEN |   SPADES), "Qs"},
	{(KING  |   SPADES), "Ks"},
};

struct table_weight los_fouls_chance[] = {
	{0,   14000, true},
	{1,    1000, true},
	{0, -1, false}
};

struct table_weight int_grounding_fouls_chance[] = {
	{0,   14000, true},
	{1,    1000, true},
	{0, -1, false}
};

struct table_weight pass_interference_chance[] = {
	{0,   14/*000*/, true},
	{1,    1000, true},
	{0, -1, false}
};

struct table_weight fifty_fifty_chance[] = {
	{0,    1000, true},
	{1,    1000, true},
	{0, -1, false}
};