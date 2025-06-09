#include "pgotypes.c"
#include <cstdio>
#include <cstdlib>

typedef struct pmon {
  const struct stats* s;
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

int main(int argc, char** argv){
  if(argc != 5){
    usage(argv[0]);
  }
  stats st1, st2;
  if(lex_ivlevel(argv[2], &st1) || lex_ivlevel(argv[4], &st2)){
    fprintf(stderr, "invalid IV@level in %s %s\n", argv[2], argv[4]);
    usage(argv[0]);
  }
  // FIXME need extract attacks
  st1.s = lookup_species(argv[1]);
  st2.s = lookup_species(argv[3]);
  if(!st1.s || !st2.s){
    fprintf(stderr, "invalid pokémon name in %s %s\n", argv[1], argv[3]);
    usage(argv[0]);
  }
  fill_stats(&st1);
  fill_stats(&st2);
  pmon p1 = {
    .s = &st1,
    .hp = st1.mhp,
  };
  pmon p2 = {
    .s = &st2,
    .hp = st2.mhp,
  };
  return EXIT_SUCCESS;
}
