#include "pgotypes.h"

static constexpr auto PPETHRESH = 0.0;

static unsigned
dualcharge_list(pgo_types_e t0, pgo_types_e t1){
  unsigned pop = 0;
  for(auto s = species_begin() ; s != species_end() ; ++s){
    for(unsigned aidx = 0 ; aidx < s->attacks.size() ; ++aidx){
      const auto a = s->attacks[aidx];
      if(!charged_attack_p(a)){
        continue;
      }
      if(a->type == t0){
        float appe = a->powertrain / static_cast<float>(-a->energytrain);
        if(has_stab_p(*s, a)){
          appe *= 1.2;
        }
        if(appe < PPETHRESH){
          continue;
        }
        for(unsigned a2idx = 0 ; a2idx < s->attacks.size() ; ++a2idx){
          const auto a2 = s->attacks[a2idx];
          if(!charged_attack_p(a2)){
            continue;
          }
          if(a2->type == t1){
            float a2ppe = a2->powertrain / static_cast<float>(-a2->energytrain);
            if(has_stab_p(*s, a2)){
              a2ppe *= 1.2;
            }
            if(a2ppe < PPETHRESH){
              continue;
            }
            printf("\t%s: %s (%.2f) + %s (%.2f)\n", s->name.c_str(),
                a->name, appe, a2->name, a2ppe);
          }
        }
      }
    }
  }
  return pop;
}

static void
usage(const char *a0){
  fprintf(stderr, "usage: %s\n", a0);
  exit(EXIT_FAILURE);
}

// generate pairs of attack types, pokémon supporting those pairs, and
// details of the implementations (ppt etc).
int main(int argc, char **argv){
  if(argc != 1){
    usage(argv[0]);
  }
  std::vector<typeset> tsets;
  build_tsets(tsets, false);
  std::sort(tsets.begin(), tsets.end(), std::greater<typeset>());
  for(const auto &ts : tsets){
    printf("%s", tname_capitalized(ts.t0));
    if(ts.t0 != ts.t1){
      printf("/%s", tname_capitalized(ts.t1));
    }
    putc('\t', stdout);
    for(unsigned i = 0 ; i < sizeof(ts.totals) / sizeof(*ts.totals) ; ++i){
      printf("%d\t", ts.totals[i]);
    }
    printf("%.3f\t%lu\n", ts.ara, ts.learnpop.size());
    dualcharge_list(static_cast<pgo_types_e>(ts.t0), static_cast<pgo_types_e>(ts.t1));
  }
}
