#include <cstdio>
#include <memory>
#include <cstdlib>
#include "../pgotypes.cpp"
#include "simul.h"
#include "moves.h"
#include "sift.h"
#include "charged.h"
static void tophalf(const simulstate *s, results *r);
#include "bottom.h"
#include "ko.h"
#include "top.h"

pmon pmons[2][TEAMSIZE] = {};

static void
usage(const char *argv0){
  std::cerr << "usage: " << argv0 << " pokémonN iv@level fast charged..." << std::endl;
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
  s->effa = calc_eff_a(s->atk + s->ia, s->hlevel, false);
  s->effd = calc_eff_d(s->def + s->id, s->hlevel, false);
  s->mhp = calc_mhp(s->sta + s->is, s->hlevel);
  s->geommean = calc_fit(s->effa, s->effd, s->mhp);
  s->cp = calccp(s->atk + s->ia, s->def + s->id, s->sta + s->is, s->hlevel);
  s->next = NULL;
}

static void
print_pmon(const pmon *p){
  printf("%s %seffa: %g effd: %g mhp: %u cp %u\n",
        p->s.s->name.c_str(), p->shadow ? "(shadow) " : "",
        p->s.effa, p->s.effd, p->s.mhp, p->s.cp);
  printf(" f attack: %s \t%3u %3d %u%s\n", p->fa->name, p->fa->powertrain, p->fa->energytrain, p->fa->turns,
          has_stab_p(p->s.s, p->fa) ? " (*)" : "");
  printf(" c attack: %s \t%3u %3d%s\n", p->ca1->name, p->ca1->powertrain, -p->ca1->energytrain,
          has_stab_p(p->s.s, p->ca1) ? " (*)" : "");
  if(p->ca2){
    printf(" c attack: %s \t%3u %3d%s\n", p->ca2->name, p->ca2->powertrain, -p->ca2->energytrain,
          has_stab_p(p->s.s, p->ca2) ? " (*)" : "");
  }
}

static void
simul(simulstate *s, results *r){
  s->turns[0] = s->turns[1] = 0u;
  for(unsigned i = 0 ; i < TEAMSIZE ; ++i){
    s->e[0][i] = s->e[1][i] = 0;
  }
  s->buffleva[0] = s->buffleva[1] = 0;
  s->bufflevd[0] = s->bufflevd[1] = 0;
  s->subtimer[0] = s->subtimer[1] = 0u;
  s->shields[0] = s->shields[1] = 2;
  s->active[0] = s->active[1] = 0;
  s->turn = 0;
  tophalf(s, r);
}

static const attack *
lex_species_charged_attacks(const species *s, const char *spec, const attack **ca2){
  *ca2 = NULL;
  // FIXME handle two charged attacks delimited by '/'
  const attack *ca1 = species_charged_attack(s, spec);
  return ca1;
}

// pass in argv at the start of the pmon spec with argc downadjusted
static int
lex_pmon(pmon* p, int *hp, int *argc, char ***argv){
  if(*argc < 4){
    std::cerr << "expected 4 arguments, " << *argc << " left" << std::endl;
    return -1;
  }
#define SHADOWSTR "shadow "
  const char *spstr;
  if(!strncasecmp(**argv, SHADOWSTR, strlen(SHADOWSTR))){
    p->shadow = true;
    spstr = **argv + strlen(SHADOWSTR);
  }else{
    p->shadow = false;
    spstr = **argv;
  }
  if((p->s.s = lookup_species(spstr)) == NULL){
    std::cerr << "no such species: " << spstr << std::endl;
    return -1;
  }
  if(p->shadow){
    if(!p->s.s->shadow){ // still allow it, but warn
      std::cerr << "warning: " << spstr << " does not have a shadow form" << std::endl;
    }
  }
  if(lex_ivlevel((*argv)[1], &p->s)){
    std::cerr << "invalid IV@level in " << (*argv)[1] << std::endl;
    return -1;
  }
  p->fa = species_fast_attack(p->s.s, (*argv)[2]);
  p->ca1 = lex_species_charged_attacks(p->s.s, (*argv)[3], &p->ca2);
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

static void
print_team(int player){
  for(unsigned i = 0 ; i < TEAMSIZE ; ++i){
    if(pmons[player][i].s.s){
      print_pmon(&pmons[player][i]);
    }
  }
}

int main(int argc, char** argv){
  const char* argv0 = *argv;
  simulstate sstate = {};
  --argc;
  ++argv;
  for(unsigned i = 0 ; i < TEAMSIZE ; ++i){
    if(!argc && i){
      break;
    }
    if(lex_pmon(&pmons[0][i], &sstate.hp[0][i], &argc, &argv)){
      usage(argv0);
    }
    if(lex_pmon(&pmons[1][i], &sstate.hp[1][i], &argc, &argv)){
      usage(argv0);
    }
  }
  print_team(0);
  print_team(1);
  if(argc){
    std::cerr << "unexpected argument: " << *argv << std::endl;
    usage(argv0);
  }
  results r;
  r.wins[0] = r.wins[1] = r.ties = 0;
  simul(&sstate, &r);
  unsigned long total = r.wins[0] + r.wins[1] + r.ties;
  printf("p0 wins: %u p1 wins: %u ties: %u total: %lu\n",
        r.wins[0], r.wins[1], r.ties, total);
  printf("p0 %.04f%% p1 %.04f%% t %.04f%%\n", r.wins[0] * 100.0 / total,
        r.wins[1] * 100.0 / total, r.ties * 100.0 / total);
  return EXIT_SUCCESS;
}
