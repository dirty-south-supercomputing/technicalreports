constexpr unsigned TEAMSIZE = 3;

typedef struct results {
  unsigned wins[2];
  unsigned ties;
} results;

// those parts which change over the course of the simulation
typedef struct simulstate {
  unsigned turn;
  int hp[2][TEAMSIZE]; // hp of pokemon
  int e[2][TEAMSIZE];  // energy of pokemon
  int shields[2];      // number of shields available, [0..2]
  int active[2];       // indexes of active pokémon
  unsigned turns[2];   // turns left in ongoing fast attack, can be 0
  unsigned subtimer[2];// substitution timers
} simulstate;

// holds only those elements which don't change over the course of the simulation
typedef struct pmon {
  struct stats s;
  const attack *fa, *ca1, *ca2;
} pmon;

extern pmon pmons[2][TEAMSIZE];
