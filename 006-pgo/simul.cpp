#include "pgotypes.cpp"
#include <cstdio>
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
  // FIXME need attacks
  printf("%s effa: %g effd: %g mhp: %u cp %u\n",
        p->s.s->name.c_str(), p->s.effa, p->s.effd, p->s.mhp, p->s.cp);
  printf(" f attack: %s\t%3u %3d %u\n", p->fa->name, p->fa->powertrain, p->fa->energytrain, p->fa->turns);
  printf(" c attack: %s\t%3u %3d\n", p->ca1->name, p->ca1->powertrain, -p->ca1->energytrain);
  if(p->ca2){
    printf(" c attack: %s\t%3u %3d\n", p->ca2->name, p->ca2->powertrain, -p->ca2->energytrain);
  }
}

typedef struct results {
  unsigned p1wins, p2wins, ties;
} results;

// those parts which change over the course of the simulation
typedef struct simulstate {
  unsigned turn;
  int hp[2];
  int energy[2];
  // number of turns remaining in an ongoing fast attack, can be 0
  unsigned turns[2];
  unsigned subtimer[2];
} simulstate;

static void
inflict_damage(unsigned *hp, unsigned damage){
  printf("inflicting %u damage on %u hp\n", damage, *hp);
  if(*hp < damage){
    *hp = 0;
  }else{
    *hp -= damage;
  }
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
  MOVEMAX
} pgo_move_e;

static inline bool
charged_move_p(pgo_move_e m){
  return m == MOVE_CHARGED1 || m == MOVE_CHARGED2;
}

static void tophalf(const simulstate *s, results *r);

static inline void
bottomhalf(simulstate *s, results *r, pgo_move_e m1, pgo_move_e m2){
  if(charged_move_p(m1) && charged_move_p(m2)){
    // FIXME determine CMP
    // FIXME launch CMP winner's charged move
    // did loser die? if so return
    // launch loser's charged move
    // did winner die? if so return
  }else if(charged_move_p(m1)){
    // FIXME run p1's charged move
    // FIXME did p2 die? if so return
    if(m2 == MOVE_FAST){
      // FIXME set up p2's fast move
    }
  }else if(charged_move_p(m2)){
    // FIXME run p2's charged move
  }
  // FIXME account for fast moves (decrement turns by 1, inflict damage if 0)
  // FIXME did anyone die? return if so
  tophalf(s, r);
}

// determine which of the moves can be taken this turn by this player
static void
sift_choices(const simulstate *s, bool *m, int player){
  if(s->turns[player]){ // if we're in a fast move, we can only wait
    m[MOVE_WAIT] = true;
    return;
  }
  m[MOVE_FAST] = true; // we can launch a fast move
  if(s->energy[player] >= -pmons[player].ca1->energytrain){
    m[MOVE_CHARGED1] = true;
  }
  if(pmons[player].ca2){
    if(s->energy[player] >= -pmons[player].ca2->energytrain){
      m[MOVE_CHARGED2] = true;
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
  bool m1[MOVEMAX] = {};
  bool m2[MOVEMAX] = {};
  sift_choices(s, m1, 0);
  sift_choices(s, m2, 1);
  for(int c1 = 0 ; c1 < MOVEMAX ; ++c1){
    if(m1[c1]){
      for(int c2 = 0 ; c2 < MOVEMAX ; ++c2){
        if(m2[c2]){
          simulstate cs = *s;
          bottomhalf(&cs, r, static_cast<pgo_move_e>(c1), static_cast<pgo_move_e>(c2));
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
  r.p1wins = r.p2wins = r.ties = 0;
  simul(&sstate, &r);
  unsigned long total = r.p1wins + r.p2wins + r.ties;
  printf("p1 wins: %u p2 wins: %u ties: %u total: %lu\n",
         r.p1wins, r.p2wins, r.ties, total);
  return EXIT_SUCCESS;
}
