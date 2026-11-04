#include "teamdata.h"
#include "main.h" // For CONSOLE_TEXT_* constants

const struct league_data default_league = {
    {
        //    Team name	  qb name                  RB1              RB2                 WR1             WR2           WR3           WR4              team colour    deck 
        {      "49ers",     "Purdy", {       "McCaffrey",          "Black"}, {        "Evans",       "Samuel",  "Stribling",     "Kittle"},       CONSOLE_TEXT_RED,   5},
        {      "Bears",  "Williams", {           "Swift",       "Monangai"}, {       "Odunze",       "Burden",    "Raymond",   "Loveland"},    CONSOLE_AS_CLOSE_TO_ORANGE_AS_POS,   5},
        {    "Bengals",    "Burrow", {          "Browne",         "Perine"}, {        "Chase",      "Higgins",     "Meyers",    "Gesicki"},    CONSOLE_AS_CLOSE_TO_ORANGE_AS_POS,   4},
        {      "Bills",     "Allen", {            "Cook",        "Johnson"}, {        "Moore",       "Shakir",    "Coleman",    "Kincaid"},      CONSOLE_TEXT_BLUE,   5},
        {    "Broncos",       "Nix", {         "Dobbins",         "Harvey"}, {       "Waddle",       "Sutton",   "Franklin",       "Mims"},    CONSOLE_AS_CLOSE_TO_ORANGE_AS_POS,   4},
        {     "Browns",    "Watson", {         "Judkins",        "Sampson"}, {       "Boston",   "Concepcion",      "Jeudy",     "Fannin"},    CONSOLE_AS_CLOSE_TO_ORANGE_AS_POS,   3},
        { "Buccaneers",  "Mayfield", {          "Irving",       "Gainwell"}, {       "Egbuka",       "Godwin",    "Johnson",      "Otton"},       CONSOLE_TEXT_RED,   3},
        {  "Cardinals",  "Brissett", {            "Love",       "Allgeier"}, {       "Wilson",       "Bourne",     "Virgil",    "McBride"},       CONSOLE_TEXT_RED,   2},
        {   "Chargers",   "Herbert", {         "Hampton",       "Mitchell"}, {     "McConkey",     "Johnston",     "Harris",      "Kolar"},    CONSOLE_TEXT_YELLOW,   1},
        {     "Chiefs",   "Mahomes", {          "Walker",        "Johnson"}, {         "Rice",       "Worthy",   "Thornton",      "Kelce"},       CONSOLE_TEXT_RED,   5},
        {      "Colts",     "Jones", {          "Taylor",        "McGowan"}, {       "Pierce",        "Downs",      "Allen",     "Warren"},      CONSOLE_TEXT_BLUE,   1},
        { "Commanders",   "Damiels", { "Croskey-Merritt",          "White"}, {     "McLaurin",        "Diggs",   "Williams",    "Okonkwo"},       CONSOLE_TEXT_RED,   1},
        {    "Cowboys",  "Prescott", {        "Williams",          "Davis"}, {         "Lamb",      "Pickens",   "Flournoy",     "Turpin"},      CONSOLE_TEXT_BLUE,   1},
        {   "Dolphins",    "Willis", {          "Achane",         "Gordon"}, {   "Washington",      "Douglas",       "Bell",    "Dulcich"}, CONSOLE_TEXT_LIGHTBLUE,   1},
        {     "Eagles",     "Hurts", {         "Barkley",         "Bigsby"}, {        "Smith",        "Wicks",      "Lemon",    "Goedert"},     CONSOLE_TEXT_GREEN,   3},
        {    "Falcons",     "Penix", {        "Robinson",   "Robinson Jr."}, {       "London",       "Dotson",  "Zaccheaus",      "Pitts"},       CONSOLE_TEXT_RED,   2},
        {     "Giants",  "McCarthy", {        "Skattebo",     "Singletary"}, {       "Nabers",       "Fields",     "Mooney",     "Likely"},      CONSOLE_TEXT_BLUE,   3},
        {    "Jaguars",  "Lawrence", {           "Tuten",      "Rodriguez"}, {   "Washington",       "Meyers",    "Cameron",    "Strange"},     CONSOLE_TEXT_GREEN,   4},
        {       "Jets",     "Smith", {            "Hall",          "Allen"}, {       "Wilson",       "Cooper",   "Mitchell",     "Taylor"},     CONSOLE_TEXT_GREEN,   2},
        {      "Lions",      "Goff", {           "Gibbs",        "Pacheco"}, {    "St. Brown",     "Williams",     "TeSlaa",    "LaPorta"}, CONSOLE_TEXT_LIGHTBLUE,   4},
        {    "Packers",      "Love", {          "Jacobs",          "Lloyd"}, {       "Watson",       "Golden",   "Williams",      "Kraft"},     CONSOLE_TEXT_GREEN,   3},
        {   "Panthers",     "Young", {         "Hubbard",        "Etienne"}, {     "McMillan",        "Coker",    "Legette",    "Tremble"},      CONSOLE_TEXT_BLUE,   3},
        {   "Patriots",      "Maye", {       "Stevenson",      "Henderson"}, {        "Brown",      "Douglas",      "Chism",      "Henry"},      CONSOLE_TEXT_BLUE,   4},
        {    "Raiders",   "Mendoza", {          "Jeanty",     "Washington"}, {       "Tucker",       "Nailor",       "Bech",     "Bowers"},      CONSOLE_TEXT_GREY,   2},
        {       "Rams",  "Stafford", {        "Williams",          "Corum"}, {        "Nacua",        "Adams",  "Mumpfield",     "Higbee"},    CONSOLE_TEXT_YELLOW,   4},
        {     "Ravens",   "Jackson", {           "Henry",           "Hill"}, {      "Flowers",      "Bateman",       "Lane",    "Andrews"},    CONSOLE_TEXT_PURPLE,   4},
        {     "Saints",    "Shough", {         "Etienne",         "Kamara"}, {        "Olave",         "Vele",      "Lance",    "Johnson"},    CONSOLE_TEXT_YELLOW,   2},
        {   "Seahawks",   "Darnold", {           "Price",     "Charbonnet"}, { "Smith-Njigba",      "Shaheed",       "Kupp",     "Barner"},      CONSOLE_TEXT_GREY,   5},
        {   "Steelers",   "Rodgers", {          "Warren",         "Dowdle"}, {      "Pittman",      "Metcalf",    "Bernard", "Washington"},    CONSOLE_TEXT_YELLOW,   3},
        {     "Texans",    "Stroud", {      "Montgomery",          "Marks"}, {      "Collins",     "Harrison", "Hutchinson",       "Dell"},       CONSOLE_TEXT_RED,   3},
        {     "Titans",      "Ward", {         "Pollard",         "Spears"}, {         "Tate",     "Robinson",   "Ayomanor",       "Helm"}, CONSOLE_TEXT_LIGHTBLUE,   1},
        {    "Vikings",    "Murray", {           "Mason",          "Jones"}, {    "Jefferson",      "Addison",   "Jennings",  "Hockenson"},    CONSOLE_TEXT_PURPLE,   4},
    }
};