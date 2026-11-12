#include <cstdio>
#include <memory>
#include <cstdlib>
#include "../pgotypes.h"

struct results {
  uint64_t wins[2], ties, timeouts; // win/tie counts
  bool dom;                         // did team0 dominate the subtree?
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
  int16_t ticks;            // timer ticks thus far
  int cmp;                  // <=> for charged move prio

  simulstate() :
    ticks(0),
    cmp(0)
  {
    memset(hp, 0, sizeof(hp));
    memset(e, 0, sizeof(e));
    memset(dam, 0, sizeof(dam));
    memset(turns, 0, sizeof(turns));
    memset(subtimer, 0, sizeof(subtimer));
    memset(buffleva, 0, sizeof(buffleva));
    memset(bufflevd, 0, sizeof(bufflevd));
    memset(shields, 0, sizeof(shields));
    memset(active, 0, sizeof(active));
  }

  void invert(simulstate &targ) const {
    memcpy(targ.hp[0], hp[1], sizeof(uint16_t) * TEAMSIZE);
    memcpy(targ.hp[1], hp[0], sizeof(uint16_t) * TEAMSIZE);
    memcpy(targ.e[0], e[1], sizeof(uint16_t) * TEAMSIZE);
    memcpy(targ.e[1], e[0], sizeof(uint16_t) * TEAMSIZE);
    memcpy(targ.dam[0], dam[1], sizeof(uint16_t) * TEAMSIZE);
    memcpy(targ.dam[1], dam[0], sizeof(uint16_t) * TEAMSIZE);
    targ.turns[0] = turns[1];
    targ.turns[1] = turns[0];
    targ.subtimer[0] = subtimer[1];
    targ.subtimer[1] = subtimer[0];
    targ.buffleva[0] = buffleva[1];
    targ.buffleva[1] = buffleva[0];
    targ.bufflevd[0] = bufflevd[1];
    targ.bufflevd[1] = bufflevd[0];
    targ.shields[0] = shields[1];
    targ.shields[1] = shields[0];
    targ.shields[0] = shields[1];
    targ.shields[1] = shields[0];
    targ.ticks = ticks;
    targ.cmp = cmp;
  }

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
static bool tophalf(simulstate *s, results *r);
#include "bottom.h"
#include "ko.h"
#include "top.h"

pmon pmons[2][TEAMSIZE] = {};

// since "tales of transformation", the switch timer is 45s (90 turns)
static constexpr unsigned long SWITCH_TIMER_TURNS = 90;

// runs the full simulation. returns true iff team 1 dominates team 2.
static inline bool
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
  return tophalf(s, r);
}
