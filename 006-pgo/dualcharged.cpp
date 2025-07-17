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
};

// build the 171 typesets
static void
build_tsets(std::vector<typeset> &tsets){
  for(int t0 = 0 ; t0 < TYPECOUNT ; ++t0){
    for(int t1 = t0 ; t1 < TYPECOUNT ; ++t1){
      int totals[6] = {};
      unsigned pop = 0;
      float ara = 0;
      tsets.emplace(tsets.end(), static_cast<pgo_types_e>(t0),
          static_cast<pgo_types_e>(t1), totals, pop, ara);
    }
  }
}

int main(void){
  std::vector<typeset> tsets;
  build_tsets(tsets);
  std::sort(tsets.begin(), tsets.end());
  printf("\\begin{longtable}{crrrrrrr}\\footnotesize");
  printf("& -2 & -1 & 0 & 1 & 2 & ARA & Pop\\\\\\Midrule\\endhead\n");
  for(const auto &ts : tsets){
    print_types(ts.t0, ts.t1);
    putc(' ', stdout);
    printf("& 0 & 0 & 0 & 0 & 0 & 0.0 & 0\\\\");
  }
  printf("\\end{longtable}\n");
}
