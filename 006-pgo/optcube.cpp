#include "pgotypes.cpp"
#include <iostream>

static void
usage(const char *argv0){
  std::cerr << "usage: " << argv0 << " species" << std::endl;
  exit(EXIT_FAILURE);
}

// we want to graph optlevel against x axis of DEF+STA and y axis of ATK
int main(int argc, char **argv){
  if(argc != 2){
    usage(argv[0]);
  }
  const species *s = lookup_species(argv[1]);
  if(!s){
    std::cerr << "unknown species: " << argv[1] << std::endl;
    usage(argv[0]);
  }
  int cpceil = 1500;
  for(int iva = 0 ; iva < 16 ; ++iva){
    for(int sum = 0 ; sum < 31 ; ++sum){
      int cp;
      unsigned hl = maxlevel_cp_bounded(s->atk + iva, s->def + sum / 2, s->sta + sum / 2, cpceil, &cp);
      float effa = calc_eff_a(s->atk + iva, hl, false);
      float effd = calc_eff_d(s->def + sum / 2, hl, false);
      float mhp = calc_mhp(s->sta + sum / 2, hl);
      float gmean = calc_fit(effa, effd, mhp);
      std::cout << iva << " " << sum << " " << gmean << std::endl;
    }
  }
  return EXIT_SUCCESS;
}
