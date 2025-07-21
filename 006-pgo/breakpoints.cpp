#include <cstdio>
#include <memory>
#include <cstdlib>
#include "pgotypes.cpp"
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
print_dbreak_table(pmon *p, pmon *atk, const attack *a, int tableno){
  printf("\\begin{table}\\setlength{\\tabcolsep}{2pt}\\footnotesize\\centering\\begin{tabular}{rcccccccccccccccc}\n");
  printf("$IV_D$ & 0 & 1 & 2 & 3 & 4 & 5 & 6 & 7 & 8 & 9 & 10 & 11 & 12 & 13 & 14 & 15\\\\\n");
  printf("\\Midrule\n");
  int firstd = -1;
  for(int ivd = 0 ; ivd < 16 ; ++ivd){
    printf("%d", ivd);
    p->s.id = ivd;
    for(int iva = 0 ; iva < 16 ; ++iva){
      atk->s.ia = iva;
      int d = static_cast<int>(calc_damage(atk, p, a, 0, 0));
      if(firstd < 0){
        firstd = d;
      }
      printf(" & \\textcolor[RGB]{%d,%d,%d}{%d}", 128 + 14 * (d - firstd),
              128 + 14 * (firstd - d), 128 + 14 * (firstd - d), d);
    }
    printf("\\\\\n");
  }
  printf("\\end{tabular}\\caption{%s using %s against %s\\label{table:bpoints%d}}\\end{table}\n",
          atk->s.s->name.c_str(), a->name, p->s.s->name.c_str(), tableno);
}

// print the d breakpoints for p
static void
print_dbreaks(pmon *p, pmon *atk){
  print_dbreak_table(p, atk, atk->ca1, 1);
  if(atk->ca2){
    print_dbreak_table(p, atk, atk->ca2, 2);
  }
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
  print_dbreaks(&pm[1], &pm[0]);
  if(argc){
    std::cerr << "unexpected argument: " << *argv << std::endl;
    usage(argv0);
  }
  return EXIT_SUCCESS;
}
