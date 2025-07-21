#include <cstdio>
#include <memory>
#include <cstdlib>
#include "../pgotypes.cpp"
#include "simul.h"
#include "sift.h"
#include "charged.h"
static void tophalf(simulstate *s, results *r);
#include "bottom.h"
#include "ko.h"
#include "top.h"

pmon pmons[2][TEAMSIZE] = {};

static void
usage(const char *argv0){
  std::cerr << "usage: " << argv0 << " pokémonN iv@level fast charged..." << std::endl;
  std::cerr << " both teams must have the same number of pokémon (not more than 3)" << std::endl;
  exit(EXIT_FAILURE);
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
  unsigned hl;
  unsigned l = halflevel_to_level(p->s.hlevel, &hl);
  printf("%s %s%u%s effa: %g effd: %g mhp: %u cp %u\n",
        p->s.s->name.c_str(),
        p->shadow ? "(shadow) " : "",
        l, hl ? ".5" : "",
        p->s.effa, p->s.effd, p->s.mhp, p->s.cp);
  printf(" f %s%20s %3u %3d %u\n", has_stab_p(p->s.s, p->fa) ? "(*)" : "   ",
          p->fa->name, p->fa->powertrain, p->fa->energytrain, p->fa->turns);
  printf(" c %s%20s %3u %3d\n", has_stab_p(p->s.s, p->ca1) ? "(*)" : "   ",
          p->ca1->name, p->ca1->powertrain, -p->ca1->energytrain);
  if(p->ca2){
    printf(" c %s%20s %3u %3d\n", has_stab_p(p->s.s, p->ca2) ? "(*)" : "   ",
          p->ca2->name, p->ca2->powertrain, -p->ca2->energytrain);
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
  s->subtimer[0] = s->subtimer[1] = 1;
  s->shields[0] = s->shields[1] = 2;
  s->active[0] = s->active[1] = 0;
  // calculate eff_a and eff_d for all players
  for(unsigned i = 0 ; i < TEAMSIZE ; ++i){
    const species *s;
    pmon *p = &pmons[0][i];
    if( (s = p->s.s) ){
      p->effa = calc_eff_a(s->atk + p->s.ia, p->s.hlevel, p->shadow);
      p->effd = calc_eff_d(s->def + p->s.id, p->s.hlevel, p->shadow);
    }
    p = &pmons[1][i];
    if( (s = p->s.s) ){
      p->effa = calc_eff_a(s->atk + p->s.ia, p->s.hlevel, p->shadow);
      p->effd = calc_eff_d(s->def + p->s.id, p->s.hlevel, p->shadow);
    }
  }
  calculate_damages(s);
  tophalf(s, r);
}

static const attack *
lex_species_charged_attacks(const species *s, const char *spec, const attack **ca2){
  *ca2 = NULL;
  const char *sep = strchr(spec, '/');
  if(sep){
    char *fspec = strndup(spec, sep - spec);
    const attack *ca1 = species_charged_attack(s, fspec);
    *ca2 = species_charged_attack(s, sep + 1);
    free(fspec);
    if(ca2 == NULL){
      return NULL;
    }
    return ca1;
  }
  const attack *ca1 = species_charged_attack(s, spec);
  return ca1;
}

// pass in argv at the start of the pmon spec with argc downadjusted
static int
lex_pmon(pmon* p, hpoints *hp, int *argc, char ***argv){
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
  if(lex_ivlevel((*argv)[1], &p->s, p->shadow)){
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

static void
print_intro(void){
  printf("pokémon exhaustive simulator state is %zuB\n", sizeof(simulstate));
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
  print_intro();
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
  printf("p0 wins: %lu p1 wins: %lu ties: %lu total: %lu\n",
        r.wins[0], r.wins[1], r.ties, total);
  printf("p0 %.04f%% p1 %.04f%% t %.04f%%\n", r.wins[0] * 100.0 / total,
        r.wins[1] * 100.0 / total, r.ties * 100.0 / total);
  return EXIT_SUCCESS;
}
