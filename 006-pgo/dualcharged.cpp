#include "pgotypes.h"

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
      build_tset(tsets, static_cast<pgo_types_e>(t0), static_cast<pgo_types_e>(t1));
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
  printf("& -3 & -2 & -1 & 0 & 1 & 2 & \\ARA{} & Pop\\\\\\Midrule\\endhead\n");
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
