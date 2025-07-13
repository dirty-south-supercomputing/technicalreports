constexpr unsigned TEAMSIZE = 3;
constexpr int ENERGY_MAX = 100;

typedef struct results {
  unsigned long wins[2], ties; // win/tie counts
  unsigned long nodes;         // total turns evaluated
} results;

using hpoints = uint16_t;
using energy = uint16_t;

typedef struct simulstate { // dynamic elements
  hpoints hp[2][TEAMSIZE]; // hp of pokemon
  uint8_t turns[2];        // turns left in fast attack   [0..5]
  energy e[2][TEAMSIZE];   // energy of pokemon         [0..100]
  uint8_t subtimer[2];     // substitution timers       [0..100]
  uint8_t buffleva[2];     // active attack buff levels  [-4..4]
  uint8_t bufflevd[2];     // active defense buff levels [-4..4]
  uint8_t shields[2];      // number of shields available [0..2]
  uint8_t active[2];       // indexes of active pokémon   [0..2]
} simulstate;

typedef struct pmon { // static elements
  struct stats s;
  const attack *fa, *ca1, *ca2;
  bool shadow;
} pmon;

extern pmon pmons[2][TEAMSIZE];

static inline int other_player(int player){
  return !player; // player is always 0 or 1
}
