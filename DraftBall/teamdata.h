#pragma once

#include "main.h"

struct team_data {
    char name[64];
    char qb_name[64];
    char rb_names[2][64];
    char wr_names[4][64];
    int colour;
    int deck_id;
};

struct league_data {
    struct team_data teams[LEAGUE_TEAM_COUNT];
};

extern const struct league_data default_league;