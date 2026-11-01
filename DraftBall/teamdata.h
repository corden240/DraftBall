#pragma once

#include <array>
#include <cstdint>
#include <string_view>
#include "main.h"

//struct league_data league_data;
//
//struct league_data {
//	char team_names[64][64]; // there will be 32 team names
//	char qb_names[64][64]; // there will be 32 qb names
//	char rb1_names[64][64]; // there will be 32 rb1 names
//	char rb2_names[64][64]; // there will be 32 rb2 names
//	char wr1_names[64][64]; // there will be 32 wr1 names
//	char wr2_names[64][64]; // there will be 32 wr2 names
//	char wr3_names[64][64]; // there will be 32 wr3 names
//	char wr4_names[64][64]; // there will be 32 wr4 names
//	int team_colours[32]; // there will be 32 team colours
//	int team_deck_used[32]; // there will be 32 deck id references
//};

struct conference_data {

};

struct division_data {

};

struct Team {
    std::string_view name;
    std::string_view qb_name;
    std::array<std::string_view, 2> rb_names;
    std::array<std::string_view, 4> wr_names;
    std::uint32_t colour;
    int deck_id;
};

using LeagueData = std::array<Team, 32>;

inline constexpr LeagueData default_league{ {
 //    Team name	  qb name                  RB1              RB2                 WR1             WR2           WR3          WR4                 team colour    deck id 
	{  "Cardinals",  "Brissett", {            "Love",       "Allgeier"}, {       "Wilson",       "Bourne",     "Virgil",    "McBride"},       CONSOLE_TEXT_RED,   0},
	{    "Falcons", "Penix Jr.", {        "Robinson",   "Robinson Jr."}, {       "London",       "Dotson",  "Zaccheaus",      "Pitts"},       CONSOLE_TEXT_RED,   0},
	{     "Ravens",   "Jackson", {           "Henry",           "Hill"}, {      "Flowers",      "Bateman",       "Lane",    "Andrews"},    CONSOLE_TEXT_PURPLE,   0},
	{      "Bills",     "Allen", {            "Cook",        "Johnson"}, {        "Moore",       "Shakir",    "Coleman",    "Kincaid"},      CONSOLE_TEXT_BLUE,   0},
	{   "Panthers",     "Young", {         "Hubbard",        "Etienne"}, {     "McMillan",        "Coker",    "Legette",    "Tremble"},      CONSOLE_TEXT_BLUE,   0},
	{      "Bears",  "Williams", {           "Swift",       "Monangai"}, {       "Odunze",       "Burden",    "Raymond",   "Loveland"},    CONSOLE_TEXT_YELLOW,   0},
	{    "Bengals",    "Burrow", {          "Browne",         "Perine"}, {        "Chase",      "Higgins",     "Meyers",    "Gesicki"},    CONSOLE_TEXT_YELLOW,   0},
	{     "Browns",    "Watson", {         "Judkins",        "Sampson"}, {       "Boston",   "Concepcion",      "Jeudy",     "Fannin"},    CONSOLE_TEXT_YELLOW,   0},
	{    "Cowboys",  "Prescott", {        "Williams",          "Davis"}, {         "Lamb",      "Pickens",   "Flournoy",     "Turpin"},      CONSOLE_TEXT_BLUE,   0},
	{    "Broncos",       "Nix", {         "Dobbins",         "Harvey"}, {       "Waddle",       "Sutton",   "Franklin",       "Mims"},    CONSOLE_TEXT_YELLOW,   0},
	{      "Lions",      "Goff", {           "Gibbs",        "Pacheco"}, {    "St. Brown",     "Williams",     "TeSlaa",    "LaPorta"}, CONSOLE_TEXT_LIGHTBLUE,   0},
	{    "Packers",      "Love", {          "Jacobs",          "Lloyd"}, {       "Watson",       "Golden",   "Williams",      "Kraft"},     CONSOLE_TEXT_GREEN,   0},
	{     "Texans",    "Stroud", {      "Montgomery",          "Marks"}, {      "Collins",     "Harrison", "Hutchinson",       "Dell"},       CONSOLE_TEXT_RED,   0},
	{      "Colts",     "Jones", {          "Taylor",        "McGowan"}, {       "Pierce",        "Downs",      "Allen",     "Warren"},      CONSOLE_TEXT_BLUE,   0},
	{    "Jaguars",  "Lawrence", {           "Tuten",      "Rodriguez"}, {   "Washington",       "Meyers",    "Cameron",    "Strange"},     CONSOLE_TEXT_GREEN,   0},
	{     "Chiefs",   "Mahomes", {          "Walker",        "Johnson"}, {         "Rice",       "Worthy",   "Thornton",      "Kelce"},       CONSOLE_TEXT_RED,   0},
	{    "Raiders",   "Mendoza", {          "Jeanty",     "Washington"}, {       "Tucker",       "Nailor",       "Bech",     "Bowers"},      CONSOLE_TEXT_GREY,   0},
	{   "Chargers",   "Herbert", {         "Hampton",       "Mitchell"}, {     "McConkey",     "Johnston",     "Harris",      "Kolar"},    CONSOLE_TEXT_YELLOW,   0},
	{       "Rams",  "Stafford", {        "Williams",          "Corum"}, {        "Nacua",        "Adams",  "Mumpfield",     "Higbee"},    CONSOLE_TEXT_YELLOW,   0},
	{   "Dolphins",    "Willis", {          "Achane",         "Gordon"}, {   "Washington",      "Douglas",       "Bell",    "Dulcich"}, CONSOLE_TEXT_LIGHTBLUE,   0},
	{    "Vikings",    "Murray", {       "Jones Sr.",          "Mason"}, {    "Jefferson",      "Addison",   "Jennings",  "Hockenson"},    CONSOLE_TEXT_PURPLE,   0},
	{   "Patriots",      "Maye", {       "Stevenson",      "Henderson"}, {        "Brown",      "Douglas",      "Chism",      "Henry"},      CONSOLE_TEXT_BLUE,   0},
	{     "Saints",    "Shough", {         "Etienne",         "Kamara"}, {        "Olave",         "Vele",      "Lance",    "Johnson"},    CONSOLE_TEXT_YELLOW,   0},
	{     "Giants",  "McCarthy", {        "Skattebo",     "Singletary"}, {       "Nabers",       "Fields",     "Mooney",     "Likely"},      CONSOLE_TEXT_BLUE,   0},
	{       "Jets",     "Smith", {            "Hall",          "Allen"}, {       "Wilson",       "Cooper",   "Mitchell",     "Taylor"},     CONSOLE_TEXT_GREEN,   0},
	{     "Eagles",     "Hurts", {         "Barkley",         "Bigsby"}, {        "Smith",        "Wicks",      "Lemon",    "Goedert"},     CONSOLE_TEXT_GREEN,   0},
	{   "Steelers",   "Rodgers", {          "Warren",         "Dowdle"}, {      "Pittman",      "Metcalf",    "Bernard", "Washington"},    CONSOLE_TEXT_YELLOW,   0},
	{      "49ers",     "Purdy", {       "McCaffrey",          "Black"}, {        "Evans",       "Samuel",  "Stribling",     "Kittle"},       CONSOLE_TEXT_RED,   0},
	{   "Seahawks",   "Darnold", {           "Price",     "Charbonnet"}, { "Smith-Njigba",      "Shaheed",       "Kupp",     "Barner"},      CONSOLE_TEXT_GREY,   0},
	{ "Buccaneers",  "Mayfield", {          "Irving",       "Gainwell"}, {       "Egbuka",       "Godwin",    "Johnson",      "Otton"},       CONSOLE_TEXT_RED,   0},
	{     "Titans",      "Ward", {         "Pollard",         "Spears"}, {         "Tate",     "Robinson",   "Ayomanor",       "Helm"}, CONSOLE_TEXT_LIGHTBLUE,   0},
	{ "Commanders",   "Damiels", { "Croskey-Merritt",          "White"}, {     "McLaurin",        "Diggs",   "Williams",    "Okonkwo"},       CONSOLE_TEXT_RED,   0},
} };