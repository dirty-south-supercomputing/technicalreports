#include <cstdio>
#include <memory>
#include <cstdlib>
#include "../pgotypes.h"

struct results {
  unsigned long wins[2], ties; // win/tie counts
};

struct simulstate { // dynamic elements
  uint16_t hp[2][TEAMSIZE]; // hp of pokemon
  uint16_t e[2][TEAMSIZE];  // energy of pokemon         [0..100]
  uint16_t dam[2][TEAMSIZE];// precalc damage for attacks
  uint8_t turns[2];         // turns left in fast attack   [0..5]
  uint8_t subtimer[2];      // substitution timers        [0..90]
  int8_t buffleva[2];       // active attack buff levels  [-4..4]
  int8_t bufflevd[2];       // active defense buff levels [-4..4]
  uint8_t shields[2];       // number of shields available [0..2]
  uint8_t active[2];        // indexes of active pokémon   [0..2]
  int cmp;                  // <=> for charged move prio

  inline bool operator==(const simulstate& r) {
    for(unsigned p = 0 ; p < 2 ; ++p){
      if(turns[p] != r.turns[p] ||
          subtimer[p] != r.subtimer[p] ||
          buffleva[p] != r.buffleva[p] ||
          bufflevd[p] != r.bufflevd[p] ||
          shields[p] != r.shields[p] ||
          active[p] != r.active[p]){
        return false;
      }
      for(unsigned t = 0 ; t < TEAMSIZE ; ++t){
        if(hp[p][t] != r.hp[p][t] ||
            e[p][t] != r.e[p][t]){
          return false;
        }
      }
    }
    return true;
  }

};

extern pmon pmons[2][TEAMSIZE];

static inline int other_player(int player){
  return !player; // player is always 0 or 1
}

static inline pmon *activemon(const simulstate *s, int p){
  return &pmons[p][s->active[p]];
}

#include "sift.h"
#include "damage.h"
#include "memo.h"
#include "in.h"
static void tophalf(simulstate *s, results *r);
#include "bottom.h"
#include "ko.h"
#include "top.h"

pmon pmons[2][TEAMSIZE] = {};

// since "tales of transformation", the switch timer is 45s (90 turns)
static constexpr unsigned long SWITCH_TIMER_TURNS = 90;

static void
simul(simulstate *s, results *r){
  s->turns[0] = s->turns[1] = 0u;
  for(unsigned i = 0 ; i < TEAMSIZE ; ++i){
    s->e[0][i] = s->e[1][i] = 0;
  }
  s->buffleva[0] = s->buffleva[1] = 0;
  s->bufflevd[0] = s->bufflevd[1] = 0;
  s->subtimer[0] = s->subtimer[1] = 1;
  s->shields[0] = s->shields[1] = 2;
  s->active[0] = s->active[1] = 0;
  calculate_damages(s);
  /*for(int p = 0 ; p < 2 ; ++p){
    for(unsigned i = 0 ; i < TEAMSIZE ; ++i){
      printf("dam[%d][%u] = %d\n", p, i, s->dam[p][i]);
    }
  }*/
  tophalf(s, r);
}
