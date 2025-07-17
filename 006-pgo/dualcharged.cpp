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

// build the 171 typesets
static void
build_tsets(std::vector<typeset> &tsets){
  for(int t0 = 0 ; t0 < TYPECOUNT ; ++t0){
    for(int t1 = t0 ; t1 < TYPECOUNT ; ++t1){
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
        ara += (static_cast<int>(i) - 3) * totals[i];
      }
      ara /= 18;
      unsigned pop = 0;
      tsets.emplace(tsets.end(), static_cast<pgo_types_e>(t0),
          static_cast<pgo_types_e>(t1), totals, pop, ara);
    }
  }
}

int main(void){
  std::vector<typeset> tsets;
  build_tsets(tsets);
  std::sort(tsets.begin(), tsets.end(), std::greater<typeset>());
  printf("\\begin{longtable}{crrrrrrrr}\\footnotesize");
  printf("& -3 & -2 & -1 & 0 & 1 & 2 & ARA & Pop\\\\\\Midrule\\endhead\n");
  for(const auto &ts : tsets){
    print_types(ts.t0, ts.t1);
    putc(' ', stdout);
    for(unsigned i = 0 ; i < sizeof(ts.totals) / sizeof(*ts.totals) ; ++i){
      printf("& %d ", ts.totals[i]);
    }
    printf("& %.3f & %u\\\\\n", ts.ara, ts.pop);
  }
  printf("\\end{longtable}\n");
}
