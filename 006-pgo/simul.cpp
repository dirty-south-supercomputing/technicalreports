#include "pgotypes.cpp"
#include <cstdio>
#include <memory>
#include <cstdlib>

// holds only those elements which don't change over the course of the simulation
typedef struct pmon {
  struct stats s;
  const attack *fa, *ca1, *ca2;
} pmon;

static pmon pmons[2];

static void
usage(const char *argv0){
  fprintf(stderr, "usage: %s pokémon1 iv@level fast charged pokémon2 iv@level fast charged\n", argv0);
  exit(EXIT_FAILURE);
}

// lex out iv and level in the form iva-ivd-ivs@l
int lex_ivlevel(const char* ivl, stats* s){
  int r;
  if((r = sscanf(ivl, "%u-%u-%u@%u", &s->ia, &s->id, &s->is, &s->hlevel)) != 4){
    fprintf(stderr, "error lexing A-D-S@L from %s (got %d)\n", ivl, r);
    return -1;
  }
  if(s->hlevel < 1 || s->hlevel > 99){
    fprintf(stderr, "invalid hlevel %u\n", s->hlevel);
    return -1;
  }
  if(s->ia > 15 || s->id > 15 || s->is > 15){
    fprintf(stderr, "invalid iv %u-%u-%u\n", s->ia, s->id, s->is);
    return -1;
  }
  return 0;
}

// fill in a stats structure given only species, IVs, and level
static void
fill_stats(stats* s){
  s->atk = s->s->atk;
  s->def = s->s->def;
  s->sta = s->s->sta;
  // FIXME handle shadow forms
  s->effa = calc_eff_a(s->atk + s->ia, s->hlevel, false);
  s->effd = calc_eff_d(s->def + s->id, s->hlevel, false);
  s->mhp = calc_mhp(s->sta + s->is, s->hlevel);
  s->geommean = calc_fit(s->effa, s->effd, s->mhp);
  s->cp = calccp(s->atk + s->ia, s->def + s->id, s->sta + s->is, s->hlevel);
  s->next = NULL;
}

static void
print_pmon(const pmon *p){
  printf("%s effa: %g effd: %g mhp: %u cp %u\n",
        p->s.s->name.c_str(), p->s.effa, p->s.effd, p->s.mhp, p->s.cp);
  printf(" f attack: %s\t%3u %3d %u\n", p->fa->name, p->fa->powertrain, p->fa->energytrain, p->fa->turns);
  printf(" c attack: %s\t%3u %3d\n", p->ca1->name, p->ca1->powertrain, -p->ca1->energytrain);
  if(p->ca2){
    printf(" c attack: %s\t%3u %3d\n", p->ca2->name, p->ca2->powertrain, -p->ca2->energytrain);
  }
}

typedef struct results {
  unsigned wins[2];
  unsigned ties;
} results;

// those parts which change over the course of the simulation
typedef struct simulstate {
  unsigned turn;
  int hp[2];
  int energy[2];
  int shields[2]; // number of shields available, [0..2]
  unsigned turns[2]; // number of turns remaining in an ongoing fast attack, can be 0
  unsigned subtimer[2];
} simulstate;

// returns true on a KO
static bool
inflict_damage(int *hp, int damage){
  //printf("inflicting %d damage on %d hp\n", damage, *hp);
  if(*hp < damage){
    *hp = 0;
  }else{
    *hp -= damage;
  }
  return !*hp;
}

static void
accumulate_energy(int *e, int energy){
  const int ENERGY_MAX = 100;
  if((*e += energy) > ENERGY_MAX){
    *e = ENERGY_MAX;
  }
//  printf("accumulated %d energy -> %d\n", energy, *e);
}

// the various things each side can do on a turn. MOVE_WAIT can mean either
// a mandatory wait while the ongoing fast attack completes, or an optional
// wait while doing nothing.
typedef enum {
  MOVE_WAIT,
  MOVE_FAST,
  MOVE_CHARGED1,
  MOVE_CHARGED2,
  MOVE_SUBSTITUTION,
  MOVE_WAIT_SHIELD,
  MOVE_FAST_SHIELD,
  MOVE_CHARGED1_SHIELD,
  MOVE_CHARGED2_SHIELD,
  MOVEMAX
} pgo_move_e;

static inline bool
charged_move_p(pgo_move_e m){
  return m == MOVE_CHARGED1 || m == MOVE_CHARGED2 ||
          m == MOVE_CHARGED1_SHIELD || m == MOVE_CHARGED2_SHIELD;
}

static inline bool
fast_move_p(pgo_move_e m){
  return m == MOVE_FAST || m == MOVE_FAST_SHIELD;
}

static inline bool
shielded_move_p(pgo_move_e m){
  return m == MOVE_WAIT_SHIELD || m == MOVE_FAST_SHIELD ||
          m == MOVE_CHARGED1_SHIELD || m == MOVE_CHARGED2_SHIELD;
}

static void tophalf(const simulstate *s, results *r);

static inline int
other_player(int player){
  return !player; // player is always 0 or 1
}

// returns true if we KO the opponent
// mt must be a charged move, and the player must have sufficient energy
// mt and mo can be shielded moves only if the appropriate player has a shield
static inline bool
throw_charged_move(simulstate *s, int player, pgo_move_e mt, pgo_move_e mo){
  const attack *a;
  if(mt == MOVE_CHARGED1 || mt == MOVE_CHARGED1_SHIELD){
    a = pmons[player].ca1;
  }else{
    a = pmons[player].ca2;
  }
  accumulate_energy(&s->energy[player], a->energytrain);
  if(shielded_move_p(mo)){
    --s->shields[other_player(player)];
    return inflict_damage(&s->hp[other_player(player)], 1);
  }
  // FIXME adjust for STAB, shadow, buffs, and typing!
  return inflict_damage(&s->hp[other_player(player)], a->powertrain);
}

// if player has an ongoing fast move, decrement turns by one. if the fast
// attack concludes as a result, inflict damage and add energy. returns true
// in the case of a KO.
static bool
account_fast_move(simulstate *s, int player){
  if(s->turns[player]){
    if(!--s->turns[player]){
      accumulate_energy(&s->energy[player], pmons[player].fa->energytrain);
      // FIXME adjust for STAB, shadow, buffs, and typing!
      return inflict_damage(&s->hp[other_player(player)], pmons[player].fa->powertrain);
    }
  }
  return false;
}

// return true iff p0 wins cmp; false indicates p1 won it
static bool
p0_wins_cmp(void){
  float moda0 = pmons[0].s.atk + pmons[0].s.ia;
  float moda1 = pmons[1].s.atk + pmons[1].s.ia;
  bool cmp0 = moda0 > moda1 ? true : moda1 > moda0 ? false : rand() % 2;
  return cmp0;
}

// run a single choice-pair, which ought be known to be viable (i.e. if we
// request a shielded move, that player ought have a shield). we ought receive
// out own simulstate in which we can scribble. corecurses back into tophalf().
static inline void
bottomhalf(simulstate *s, results *r, pgo_move_e m0, pgo_move_e m1){
  if(m0 == MOVE_SUBSTITUTION || m1 == MOVE_SUBSTITUTION){
    std::cout << "substitution is not yet handled!" << std::endl;
    return;
  }
  //printf("bottom hp %d %d moves %d %d\n", s->hp[0], s->hp[1], m0, m1);
  if(charged_move_p(m0) && charged_move_p(m1)){ // both throw charged attacks
    if(p0_wins_cmp()){
      if(throw_charged_move(s, 0, m0, m1)){
        ++r->wins[0]; return;
      }else if(throw_charged_move(s, 1, m1, m0)){
        ++r->wins[1]; return;
      }
    }else{
      if(throw_charged_move(s, 1, m1, m0)){
        ++r->wins[1]; return;
      }else if(throw_charged_move(s, 0, m0, m1)){
        ++r->wins[0]; return;
      }
    }
  }else if(charged_move_p(m0)){ // only player 0 is throwing a charged attack
    if(throw_charged_move(s, 0, m0, m1)){
      ++r->wins[0]; return;
    }
  }else if(charged_move_p(m1)){ // only player 1 is throwing a charged attack
    if(throw_charged_move(s, 1, m1, m0)){
      ++r->wins[1]; return;
    }
  }
  if(fast_move_p(m0)){
    s->turns[0] = pmons[0].fa->turns;
  }
  if(fast_move_p(m1)){
    s->turns[1] = pmons[1].fa->turns;
  }
  bool k0 = account_fast_move(s, 0);
  bool k1 = account_fast_move(s, 1);
  if(k0 && k1){
    ++r->ties; return;
  }else if(k0){
    ++r->wins[0]; return;
  }else if(k1){
    ++r->wins[1]; return;
  }
  //printf("MUST RECURSE hp %d %d moves %d %d\n", s->hp[0], s->hp[1], m0, m1);
  tophalf(s, r); // no one got knocked out; recurse to next turn
}

// determine which of the moves can be taken this turn by this player
static void
sift_choices(const simulstate *s, bool *m, int player){
  if(s->turns[player]){ // if we're in a fast move, we can only wait
    m[MOVE_WAIT] = true;
    if(s->shields[player]){
      m[MOVE_WAIT_SHIELD] = true;
    }
    return;
  }
  m[MOVE_FAST] = true; // we can launch a fast move
  if(s->shields[player]){
    m[MOVE_FAST_SHIELD] = true;
  }
  if(s->energy[player] >= -pmons[player].ca1->energytrain){
    m[MOVE_CHARGED1] = true;
    if(s->shields[player]){
      m[MOVE_CHARGED1_SHIELD] = true;
    }
  }
  if(pmons[player].ca2){
    if(s->energy[player] >= -pmons[player].ca2->energytrain){
      m[MOVE_CHARGED2] = true;
      if(s->shields[player]){
        m[MOVE_CHARGED2_SHIELD] = true;
      }
    }
  }
  if(s->subtimer[player] == 0){
    m[MOVE_SUBSTITUTION] = true;
  }
}

// each turn has a top half and bottom halves.
// in the top half we determine which choice-pairs can be performed.
// in the bottom halves, we simulate a choice-pair.
static void
tophalf(const simulstate *s, results *r){
  //printf("tophalf turn %u %u %u %u\n", s->turn, r->wins[0], r->wins[1], r->ties);
  bool m0[MOVEMAX] = {};
  bool m1[MOVEMAX] = {};
  sift_choices(s, m0, 0);
  sift_choices(s, m1, 1);
  for(int c0 = 0 ; c0 < MOVEMAX ; ++c0){
    if(m0[c0]){
      for(int c1 = 0 ; c1 < MOVEMAX ; ++c1){
        if(m1[c1]){
          // if c0 is a shielded move, it's only relevant if c1 is a charged
          // move, and vice versa
          if(shielded_move_p(static_cast<pgo_move_e>(c0))){
            if(!charged_move_p(static_cast<pgo_move_e>(c1))){
              continue;
            }
          }
          if(shielded_move_p(static_cast<pgo_move_e>(c1))){
            if(!charged_move_p(static_cast<pgo_move_e>(c0))){
              continue;
            }
          }
          auto cs = std::make_unique<simulstate>();
          *cs.get() = *s;
          ++cs->turn;
          //printf("calling bottomhalf with moves %d %d hp %d %d turn %u %u\n", c1, c2, cs->hp[0], cs->hp[1], s->turn, cs->turn);
          bottomhalf(cs.get(), r, static_cast<pgo_move_e>(c0), static_cast<pgo_move_e>(c1));
        }
      }
    }
  }
}

static void
simul(simulstate *s, results *r){
  s->turns[0] = s->turns[1] = 0u;
  s->energy[0] = s->energy[1] = 0;
  s->subtimer[0] = s->subtimer[1] = 0u;
  s->shields[0] = s->shields[1] = 2;
  s->turn = 0;
  tophalf(s, r);
}

// pass in argv at the start of the pmon spec with argc downadjusted
static int
lex_pmon(pmon* p, int *hp, int *argc, char*** argv){
  if(*argc < 4){
    fprintf(stderr, "expected 4 arguments, %d left\n", (*argc) - 1);
    return -1;
  }
  if((p->s.s = lookup_species(**argv)) == NULL){
    fprintf(stderr, "no such species: %s\n", **argv);
    return -1;
  };
  if(lex_ivlevel((*argv)[1], &p->s)){
    fprintf(stderr, "invalid IV@level in %s\n", (*argv)[1]);
    return -1;
  }
  p->fa = species_fast_attack(p->s.s, (*argv)[2]);
  // FIXME handle two charged attacks delimited by '/'
  p->ca2 = NULL;
  p->ca1 = species_charged_attack(p->s.s, (*argv)[3]);
  if(!p->fa || !p->ca1){
    fprintf(stderr, "invalid attacks for %s: '%s' '%s'\n", p->s.s->name.c_str(),
            (*argv)[2], (*argv)[3]);
    return -1;
  }
  fill_stats(&p->s);
  *hp = p->s.mhp;
  (*argv) += 4;
  *argc -= 4;
  return 0;
}

int main(int argc, char** argv){
  const char* argv0 = *argv;
  simulstate sstate;
  --argc;
  ++argv;
  if(lex_pmon(&pmons[0], &sstate.hp[0], &argc, &argv)){
    usage(argv0);
  }
  print_pmon(&pmons[0]);
  if(lex_pmon(&pmons[1], &sstate.hp[1], &argc, &argv)){
    usage(argv0);
  }
  print_pmon(&pmons[1]);
  if(argc){
    fprintf(stderr, "unexpected argument: %s\n", *argv);
    usage(argv0);
  }
  results r;
  r.wins[0] = r.wins[1] = r.ties = 0;
  simul(&sstate, &r);
  float total = r.wins[0] + r.wins[1] + r.ties;
  printf("p0 wins: %u p1 wins: %u ties: %u total: %g\n",
         r.wins[0], r.wins[1], r.ties, total);
  printf("p0 %.04f%% p1 %.04f%% t %.04f%%\n", r.wins[0] * 100 / total,
      r.wins[1] * 100 / total, r.ties * 100 / total);
  return EXIT_SUCCESS;
}
