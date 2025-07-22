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
dualcharge_pop(pgo_types_e t0, pgo_types_e t1){
  unsigned pop = 0;
  for(unsigned u = 0 ; u < SPECIESCOUNT ; ++u){
    bool b0 = false;
    bool b1 = false;
    for(const attack **a = sdex[u].attacks ; *a ; ++a){
      if((*a)->energytrain >= 0){
        continue;
      }
      if((*a)->type == t0){
        b0 = true;
      }
      if((*a)->type == t1){
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
build_tsets(std::vector<typeset> &tsets, bool monomode){
  for(int t0 = 0 ; t0 < TYPECOUNT ; ++t0){
    int lbound, ubound;
    if(monomode){
      lbound = t0;
      ubound = t0 + 1;
    }else{
      lbound = t0 + 1;
      ubound = TYPECOUNT;
    }
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
  fprintf(stderr, "usage: %s m | d\n", a0);
  exit(EXIT_FAILURE);
}

// this misnamed program generates either a longtable of all pairs of types
// and their ARAs ('d'), or a regular table of all types and their ARAs ('m')
int main(int argc, char **argv){
  if(argc != 2){
    usage(argv[0]);
  }
  bool monomode;
  if(strcmp(argv[1], "m") == 0){
    monomode = true;
  }else if(strcmp(argv[1], "d") == 0){
    monomode = false;
  }else{
    usage(argv[0]);
  }
  std::vector<typeset> tsets;
  build_tsets(tsets, monomode);
  std::sort(tsets.begin(), tsets.end(), std::greater<typeset>());
  printf("\\begingroup\\footnotesize");
  printf("\\begin{longtable}{crrrrrrrr}\\footnotesize");
  printf("& -3 & -2 & -1 & 0 & 1 & 2 & \\ARA & Pop\\\\\\endhead\n");
  bool grey = true;
  for(const auto &ts : tsets){
    if( (grey = !grey) ){
      printf("\\rowcolor{Gray!25}");
    }
    print_types(ts.t0, ts.t1);
    putc(' ', stdout);
    for(unsigned i = 0 ; i < sizeof(ts.totals) / sizeof(*ts.totals) ; ++i){
      if(ts.totals[i]){
        printf("& %d ", ts.totals[i]);
      }else{
        printf("& ");
      }
    }
    printf("& %.3f & %u\\\\\n", ts.ara, ts.pop);
  }
  printf("\\caption{Type efficiency summaries for charged attack sets\\label{table:dualcharged}}");
  printf("\\end{longtable}\\endgroup\n");
}
