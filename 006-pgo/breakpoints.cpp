#include <cstdio>
#include <memory>
#include <cstdlib>
#include "pgotypes.cpp"
#include "s/moves.h"
#include "s/simul.h"
#include "s/charged.h"

static pmon pm[2];

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

/*
      if(!pm[1][j].s.s){
        continue;
      }
      pm[0][i].damage[0][j] = calc_damage(&pm[0][i], &pm[1][j], pm[0][i].fa);
      pm[0][i].damage[1][j] = calc_damage(&pm[0][i], &pm[1][j], pm[0][i].ca1);
      if(pm[0][i].ca2){
        pm[0][i].damage[2][j] = calc_damage(&pm[0][i], &pm[1][j], pm[0][i].ca2);
      }
      pm[1][j].damage[0][i] = calc_damage(&pm[1][j], &pm[0][i], pm[1][j].fa);
      pm[1][j].damage[1][i] = calc_damage(&pm[1][j], &pm[0][i], pm[1][j].ca1);
      if(pm[1][j].ca2){
        pm[1][j].damage[2][i] = calc_damage(&pm[1][j], &pm[0][i], pm[1][j].ca2);
      }
    }
  }
  */

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

// print the d breakpoints for p
static void
print_dbreaks(pmon *p, pmon *atk){
  printf("\\begin{table}\\setlength{\\tabcolsep}{1pt}\\footnotesize\\centering\\begin{tabular}{lcccccccccccccccc}\n");
  printf("$IV_D$ & 0 & 1 & 2 & 3 & 4 & 5 & 6 & 7 & 8 & 9 & 10 & 11 & 12 & 13 & 14 & 15\\\\\n");
  printf("\\Midrule\n");
  for(int ivd = 0 ; ivd < 16 ; ++ivd){
    printf("%d", ivd);
    p->s.id = ivd;
    for(int iva = 0 ; iva < 16 ; ++iva){
      atk->s.ia = iva;
      float d = calc_damage(atk, p, atk->fa);
      printf(" & %d", static_cast<int>(d));
    }
    printf("\\\\\n");
  }
  printf("\\end{tabular}\\caption{FIXME graph}\\end{table}\n");
  printf("\\begin{table}\\setlength{\\tabcolsep}{1pt}\\footnotesize\\centering\\begin{tabular}{lcccccccccccccccc}\n");
  printf("$IV_D$ & 0 & 1 & 2 & 3 & 4 & 5 & 6 & 7 & 8 & 9 & 10 & 11 & 12 & 13 & 14 & 15\\\\\n");
  printf("\\Midrule\n");
  for(int ivd = 0 ; ivd < 16 ; ++ivd){
    p->s.id = ivd;
    printf("%d", ivd);
    for(int iva = 0 ; iva < 16 ; ++iva){
      atk->s.ia = iva;
      float d = calc_damage(atk, p, atk->ca1);
      printf(" & %d", static_cast<int>(d));
    }
    printf("\\\\\n");
  }
  printf("\\end{tabular}\\caption{FIXME graph}\\end{table}\n");
  printf("\\begin{table}\\setlength{\\tabcolsep}{1pt}\\footnotesize\\centering\\begin{tabular}{lcccccccccccccccc}\n");
  printf("$IV_D$ & 0 & 1 & 2 & 3 & 4 & 5 & 6 & 7 & 8 & 9 & 10 & 11 & 12 & 13 & 14 & 15\\\\\n");
  printf("\\Midrule\n");
  if(atk->ca2){
    for(int ivd = 0 ; ivd < 16 ; ++ivd){
      p->s.id = ivd;
      printf("%d", ivd);
      for(int iva = 0 ; iva < 16 ; ++iva){
        atk->s.ia = iva;
        float d = calc_damage(atk, p, atk->ca2);
        printf(" & %d", static_cast<int>(d));
      }
      printf("\\\\\n");
    }
  }
  printf("\\end{tabular}\\caption{FIXME graph}\\end{table}\n");
}

static void
usage(const char *argv0){
  std::cerr << "usage: " << argv0 << " pokémonN iv@level fast charged..." << std::endl;
  exit(EXIT_FAILURE);
}

int main(int argc, char** argv){
  const char* argv0 = *argv;
  --argc;
  ++argv;
  int hp0, hp1;
  if(lex_pmon(&pm[0], &hp0, &argc, &argv)){
    usage(argv0);
  }
  if(lex_pmon(&pm[1], &hp1, &argc, &argv)){
    usage(argv0);
  }
  //print_pmon(&pm[0]);
  //print_pmon(&pm[1]);
  print_dbreaks(&pm[1], &pm[0]);
  // FIXME print damage for iv[a] 0..15 and opposing iv[d] 0..15
  if(argc){
    std::cerr << "unexpected argument: " << *argv << std::endl;
    usage(argv0);
  }
  return EXIT_SUCCESS;
}
