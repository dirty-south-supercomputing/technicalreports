constexpr unsigned TEAMSIZE = 3;
constexpr int ENERGY_MAX = 100;

typedef struct results {
  unsigned long wins[2], ties; // win/tie counts
  unsigned long nodes;         // total turns evaluated
} results;

typedef struct simulstate { // dynamic elements
  int hp[2][TEAMSIZE]; // hp of pokemon
  int e[2][TEAMSIZE];  // energy of pokemon         [0..100]
  int buffleva[2];     // active attack buff levels  [-4..4]
  int bufflevd[2];     // active defense buff levels [-4..4]
  int shields[2];      // number of shields available [0..2]
  int active[2];       // indexes of active pokémon   [0..2]
  unsigned turns[2];   // turns left in fast attack   [0..5]
  unsigned subtimer[2];// substitution timers       [0..100]
} simulstate;

typedef struct pmon { // static elements
  struct stats s;
  bool shadow;
  const attack *fa, *ca1, *ca2;
  // we precalculate the damage its (up to) three attacks will inflict upon
  // the (up to) three opponents, except for buffs (which change across the
  // match). then all we need do is multiply cached damage by ratio of buffs.
  float damage[3][TEAMSIZE];
} pmon;

extern pmon pmons[2][TEAMSIZE];

static inline int other_player(int player){
  return !player; // player is always 0 or 1
}
