#include "pgotypes.c"
#include <cstdio>
#include <cstdlib>

typedef struct pmon {
  struct stats s;
  unsigned hp;
} pmon;

static void
usage(const char *argv0){
  fprintf(stderr, "usage: %s pokémon1 iv@level pokémon2 iv@level\n", argv0);
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
}

// pass in argv at the start of the pmon spec with argc downadjusted
static int
lex_pmon(pmon* p, int *argc, char*** argv){
  if(*argc < 4){
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
  fill_stats(&p->s);
  p->hp = p->s.mhp;
  (*argv) += 3;
  *argc -= 3;
  return 0;
}

int main(int argc, char** argv){
  const char* argv0 = *argv;
  pmon p1, p2;
  --argc;
  ++argv;
  if(lex_pmon(&p1, &argc, &argv)){
    usage(argv0);
  }
  print_pmon(&p1);
  if(lex_pmon(&p2, &argc, &argv)){
    usage(argv0);
  }
  print_pmon(&p2);
  return EXIT_SUCCESS;
}
