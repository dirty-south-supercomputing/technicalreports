#include "pgotypes.cpp"

struct typeset {
  pgo_types_e t0;
  pgo_types_e t1; // can be the same as t1 if we only have one attack type
  int totals[6];  // we range from -3 to 2, inclusive
  unsigned pop;   // population that can learn a charged attack of these types
  float ara;

  typeset(pgo_types_e T0, pgo_types_e T1, const int Totals[],
          unsigned Pop, float ARA) :
    t0(T0),
    t1(T1),
    pop(Pop),
    ara(ARA) {
      memcpy(totals, Totals, sizeof(totals));
  }

  friend bool operator<(const typeset &l, const typeset &r) {
    return l.ara < r.ara ? true :
            r.ara < l.ara ? false :
            l.pop < r.pop ? true :
            r.pop < l.pop ? false :
            l.t0 < r.t0 ? true :
            r.t0 < l.t0 ? false :
            l.t1 < r.t1 ? true : false;
  }

  friend bool operator>(const typeset &l, const typeset &r) {
    return !(l < r);
  }

};

static unsigned
dualcharge_list(pgo_types_e t0, pgo_types_e t1){
  unsigned pop = 0;
  for(unsigned u = 0 ; u < SPECIESCOUNT ; ++u){
    const species &s = sdex[u];
    for(unsigned aidx = 0 ; aidx < s.attacks.size() ; ++aidx){
      const auto a = s.attacks[aidx];
      if(!charged_attack_p(a)){
        continue;
      }
      if(a->type == t0){
        float appe = a->powertrain / static_cast<float>(-a->energytrain);
        if(has_stab_p(&s, a)){
          appe *= 1.2;
        }
        for(unsigned a2idx = aidx + 1 ; a2idx < s.attacks.size() ; ++a2idx){
          const auto a2 = s.attacks[a2idx];
          if(!charged_attack_p(a2)){
            continue;
          }
          if(a2->type == t1){
            float a2ppe = a2->powertrain / static_cast<float>(-a2->energytrain);
            if(has_stab_p(&s, a2)){
              a2ppe *= 1.2;
            }
            printf("\t%s: %s (%.2f) + %s (%.2f)\n", sdex[u].name.c_str(),
                a->name, appe, a2->name, a2ppe);
          }
        }
      }
    }
  }
  return pop;
}

static unsigned
dualcharge_pop(pgo_types_e t0, pgo_types_e t1){
  unsigned pop = 0;
  for(unsigned u = 0 ; u < SPECIESCOUNT ; ++u){
    bool b0 = false;
    bool b1 = false;
    for(const auto &a : sdex[u].attacks){
      if(a->energytrain >= 0){
        continue;
      }
      if(a->type == t0){
        b0 = true;
      }
      if(a->type == t1){
        b1 = true;
      }
    }
    if(b0 && (b1 || t1 == TYPECOUNT)){
      ++pop;
    }
  }
  return pop;
}

// build the 171 typesets
static void
build_tsets(std::vector<typeset> &tsets){
  for(int t0 = 0 ; t0 < TYPECOUNT ; ++t0){
    int lbound, ubound;
    lbound = t0 + 1;
    ubound = TYPECOUNT;
    for(int t1 = lbound ; t1 < ubound ; ++t1){
      int totals[6] = {};
      for(int tt0 = 0 ; tt0 < TYPECOUNT ; ++tt0){
        for(int tt1 = tt0 ; tt1 < TYPECOUNT ; ++tt1){
          int e0 = typing_relation(static_cast<pgo_types_e>(t0), static_cast<pgo_types_e>(tt0), static_cast<pgo_types_e>(tt1));
          int e1 = typing_relation(static_cast<pgo_types_e>(t1), static_cast<pgo_types_e>(tt0), static_cast<pgo_types_e>(tt1));
          int e = e0 > e1 ? e0 : e1;
          ++totals[e + 3];
        }
      }
      float ara = 0;
      for(unsigned i = 0 ; i < sizeof(totals) / sizeof(*totals) ; ++i){
        ara += type_effectiveness_mult(static_cast<int>(i) - 3) * totals[i];
      }
      ara /= TYPINGCOUNT;
      unsigned pop = dualcharge_pop(static_cast<pgo_types_e>(t0), static_cast<pgo_types_e>(t1));
      tsets.emplace(tsets.end(), static_cast<pgo_types_e>(t0),
          static_cast<pgo_types_e>(t1), totals, pop, ara);
    }
  }
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
  build_tsets(tsets);
  std::sort(tsets.begin(), tsets.end(), std::greater<typeset>());
  for(const auto &ts : tsets){
    printf("%s", TNames[ts.t0]);
    if(ts.t0 != ts.t1){
      printf("/%s", TNames[ts.t1]);
    }
    putc('\t', stdout);
    for(unsigned i = 0 ; i < sizeof(ts.totals) / sizeof(*ts.totals) ; ++i){
      printf("%d\t", ts.totals[i]);
    }
    printf("%.3f\t%u\n", ts.ara, ts.pop);
    dualcharge_list(static_cast<pgo_types_e>(ts.t0), static_cast<pgo_types_e>(ts.t1));
  }
}
