#include "pgotypes.c"
#include <cstdio>
#include <cstdlib>

// holds only those elements which don't change over the course of the simulation
typedef struct pmon {
  struct stats s;
  const attack *fa, *ca1, *ca2;
} pmon;

static pmon p1, p2;

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
  s->effa = calc_eff_a(s->atk + s->ia, s->hlevel);
  s->effd = calc_eff_d(s->def + s->id, s->hlevel);
  s->mhp = calc_mhp(s->sta + s->is, s->hlevel);
  s->geommean = calc_fit(s->effa, s->effd, s->mhp);
  s->cp = calccp(s->atk + s->ia, s->def + s->id, s->sta + s->is, s->hlevel);
  s->next = NULL;
}

static void
print_pmon(const pmon *p){
  // FIXME need attacks
  printf("%s effa: %g effd: %g mhp: %u cp %u\n",
        p->s.s->name, p->s.effa, p->s.effd, p->s.mhp, p->s.cp);
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
  unsigned p1hp, p2hp;
  int p1energy, p2energy;
  // number of turns remaining in an ongoing fast attack, can be 0
  unsigned p1turns, p2turns;
} simulstate;

static void
inflict_damage(unsigned *hp, unsigned damage){
  if(*hp < damage){
    *hp = 0;
  }else{
    *hp -= damage;
  }
}

// must simulate cartesian of p1 options and p2 options.
// options include:
//  - continue fast attack if one is ongoing
//  - substitution (if timer has not expired)
//  - fast attack
//  - charged attack (with sufficient energy)
//  - do nothing
// but we do not simulate both doing nothing in the same turn
static int
simulturn(const simulstate *ins, results *r){
  bool p1ongoing = false;
  bool p2ongoing = false;
  simulstate s = *ins;
  ++s.turn;
  //printf("Turn %u HP: %u %u Energy: %d %d wait: %u %u\n", s.turn, ins->p1hp, ins->p2hp, ins->p1energy, ins->p2energy, ins->p1turns, ins->p2turns);
  if(s.p1turns){
    // p1 needs make a choice
    if(!--s.p1turns){
      // deal damage to p2
      inflict_damage(&s.p2hp, p1.fa->powertrain);
    }else{
      // p1's ongoing fast attack has not yet ended
      p1ongoing = true;
    }
  }
  if(s.p2turns){
    // p2 needs make a choice
    if(--s.p2turns == 0){
      // deal damage to p1
      inflict_damage(&s.p1hp, p2.fa->powertrain);
    }else{
      // p2's ongoing fast attack has not yet ended
      p2ongoing = true;
    }
  }
  if(s.p1hp == 0 && s.p2hp == 0){
    //printf("tie!\n");
    ++r->ties;
    return 0;
  }else if(s.p1hp == 0){
    //printf("p2 wins!\n");
    ++r->p2wins;
    return 0;
  }else if(s.p2hp == 0){
    //printf("p1 wins!\n");
    ++r->p1wins;
    return 0;
  }
  // run this turn then call the next turns, accumulate into r
  if(p1ongoing && p2ongoing){
    // only one option: both wait for attack to finish
    //printf("turn %u recursing both wait %u %u\n", s.turn, s.p1turns, s.p2turns);
    simulturn(&s, r);
    return 0;
  }else if(!p1ongoing && p2ongoing){ // cartesian of p1 and wait
    //printf("recursing p2wait\n");
    //simulturn(&s, r); // p1 does nothing
    s.p1turns = p1.fa->turns; // p1 launches fast attack
    s.p1energy += p1.fa->energytrain;
    simulturn(&s, r);
    s.p1energy -= p1.fa->energytrain;
    s.p1turns = 0;
    s.p2turns = 0; // charged attack ends opponent's fast attack
    inflict_damage(&s.p1hp, p2.fa->powertrain);
    if(!s.p1hp){
      ++r->p2wins;
    }else{
      unsigned p2hppreserve = s.p2hp;
      if(s.p1energy >= -p1.ca1->energytrain){
        s.p1energy += p1.ca1->energytrain;
        inflict_damage(&s.p2hp, p1.ca1->powertrain);
        if(!s.p2hp){
          ++r->p1wins;
        }else{
          simulturn(&s, r);
        }
        s.p1energy -= p1.ca1->energytrain;
      }
      s.p2hp = p2hppreserve;
      if(p1.ca2){
        if(s.p1energy >= -p1.ca2->energytrain){
          s.p1energy += p1.ca2->energytrain;
          inflict_damage(&s.p2hp, p1.ca2->powertrain);
          if(!s.p2hp){
            ++r->p1wins;
          }else{
            simulturn(&s, r);
          }
          s.p1energy -= p1.ca2->energytrain;
        }
      }
    }
    return 0;
  }else if(p1ongoing && !p2ongoing){ // cartesian of wait and p2
    //printf("turn %u recursing p1wait %u %u\n", s.turn, s.p1turns, s.p2turns);
    //simulturn(&s, r); // p2 does nothing
    s.p2turns = p2.fa->turns; // p2 launches fast attack
    s.p2energy += p2.fa->energytrain;
    simulturn(&s, r);
    s.p2energy -= p2.fa->energytrain;
    s.p2turns = 0;
    // now try p2 charged attacks
    s.p1turns = 0; // charged attack ends opponent's fast attack
    inflict_damage(&s.p2hp, p1.fa->powertrain);
    if(!s.p2hp){
      ++r->p1wins;
    }else{
      unsigned p1hppreserve = s.p1hp;
      if(s.p2energy >= -p2.ca1->energytrain){
        s.p2energy += p2.ca1->energytrain;
        inflict_damage(&s.p1hp, p2.ca1->powertrain);
        if(!s.p1hp){
          ++r->p2wins;
        }else{
          simulturn(&s, r);
        }
        s.p2energy -= p2.ca1->energytrain;
      }
      s.p1hp = p1hppreserve;
      if(p2.ca2){
        if(s.p2energy >= -p2.ca2->energytrain){
          s.p2energy += p2.ca2->energytrain;
          inflict_damage(&s.p1hp, p2.ca2->powertrain);
          if(!s.p1hp){
            ++r->p2wins;
          }else{
            simulturn(&s, r);
          }
          s.p2energy -= p2.ca2->energytrain;
        }
      }
    }
    return 0;
  }
  // cartesian of all options
  //printf("recursing cartesian\n");
  // first, have them both do a fast attack
  s.p1turns = p1.fa->turns;
  s.p2turns = p2.fa->turns;
  s.p1energy += p1.fa->energytrain;
  s.p2energy += p2.fa->energytrain;
  simulturn(&s, r);
  // now, p1 fast while p2 does nothing
  s.p1turns = p1.fa->turns;
  s.p2turns = 0;
  s.p2energy -= p2.fa->energytrain;
  simulturn(&s, r);
  // now, p2 fast while p1 does nothing
  s.p2turns = p2.fa->turns;
  s.p1turns = 0;
  s.p2energy += p2.fa->energytrain;
  s.p1energy -= p1.fa->energytrain;
  simulturn(&s, r);
  s.p2energy -= p2.fa->energytrain;
  if(s.p1energy >= -p1.ca1->energytrain){
    s.p1energy += p1.ca1->energytrain;
    inflict_damage(&s.p2hp, p1.ca1->powertrain);
    if(!s.p2hp){
      ++r->p1wins;
    }else{
      simulturn(&s, r);
    }
    s.p1energy -= p1.ca1->energytrain;
  }
  unsigned p2hppreserve = s.p2hp;
  if(s.p1energy >= -p1.ca1->energytrain){
    s.p1energy += p1.ca1->energytrain;
    inflict_damage(&s.p2hp, p1.ca1->powertrain);
    if(!s.p2hp){
      ++r->p1wins;
    }else{
      simulturn(&s, r);
    }
    s.p1energy -= p1.ca1->energytrain;
  }
  s.p2hp = p2hppreserve;
  if(p1.ca2){
    if(s.p1energy >= -p1.ca2->energytrain){
      s.p1energy += p1.ca2->energytrain;
      inflict_damage(&s.p2hp, p1.ca2->powertrain);
      if(!s.p2hp){
        ++r->p1wins;
      }else{
        simulturn(&s, r);
      }
      s.p1energy -= p1.ca2->energytrain;
    }
  }
  s.p2hp = p2hppreserve;
  unsigned p1hppreserve = s.p1hp;
  if(s.p2energy >= -p2.ca1->energytrain){
    s.p2energy += p2.ca1->energytrain;
    inflict_damage(&s.p1hp, p2.ca1->powertrain);
    if(!s.p1hp){
      ++r->p2wins;
    }else{
      simulturn(&s, r);
    }
    s.p2energy -= p2.ca1->energytrain;
  }
  s.p1hp = p1hppreserve;
  if(p2.ca2){
    if(s.p2energy >= -p2.ca2->energytrain){
      s.p2energy += p2.ca2->energytrain;
      inflict_damage(&s.p1hp, p2.ca2->powertrain);
      if(!s.p1hp){
        ++r->p2wins;
      }else{
        simulturn(&s, r);
      }
      s.p2energy -= p2.ca2->energytrain;
    }
  }
  s.p1hp = p1hppreserve;
  printf("p1: %8u p2:%8u t: %8u\n", r->p1wins, r->p2wins, r->ties);
  return 0;
}

static int
simul(simulstate *s, results *r){
  s->p1turns = s->p2turns = 0;
  s->p1energy = s->p2energy = 0;
  s->turn = 0;
  return simulturn(s, r);
}

// pass in argv at the start of the pmon spec with argc downadjusted
static int
lex_pmon(pmon* p, unsigned *hp, int *argc, char*** argv){
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
    fprintf(stderr, "invalid attacks for %s: '%s' '%s'\n", p->s.s->name, (*argv)[2], (*argv)[3]);
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
  if(lex_pmon(&p1, &sstate.p1hp, &argc, &argv)){
    usage(argv0);
  }
  print_pmon(&p1);
  if(lex_pmon(&p2, &sstate.p2hp, &argc, &argv)){
    usage(argv0);
  }
  print_pmon(&p2);
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
