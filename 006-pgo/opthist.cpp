#include "pgotypes.h"
#include <iostream>

static void
usage(const char *argv0){
  std::cerr << "usage: " << argv0 << " species" << std::endl;
  exit(EXIT_FAILURE);
}

static void
emit_gmeans(const species* s){
  const struct {
    int cpcap;
    const char* label;
  } leagues[] = {
    { GLCPCAP, "GL", },
    { ULCPCAP, "UL", },
    { 0, "ML", },
    { -1, NULL, }
  }, *league;
  // print header
  for(league = leagues ; league->label ; ++league){
    std::cout << league->label << " ";
  }
  std::cout << std::endl;
  for(int iva = 0 ; iva < 16 ; ++iva){
    for(int ivd = 0 ; ivd < 16 ; ++ivd){
      for(int ivs = 0 ; ivs < 16 ; ++ivs){
        for(league = leagues ; league->label ; ++league){
          int cp;
          unsigned hl = maxlevel_cp_bounded(s->atk + iva, s->def + ivd, s->sta + ivs, league->cpcap, &cp);
          float effa = calc_eff_a(s->atk + iva, hl, false);
          float effd = calc_eff_d(s->def + ivd, hl, false);
          float mhp = calc_mhp(s->sta + ivs, hl);
          float gmean = calc_gmean(effa, effd, mhp);
          std::cout << gmean << ' ';
        }
        std::cout << std::endl;
      }
    }
  }
}

// generate all the gmeans for each league, suitable for R histogram
// one column per league
// gmeans separated by spaces on line
int main(int argc, char **argv){
  if(argc != 2){
    usage(argv[0]);
  }
  const species *s = lookup_species(argv[1]);
  if(!s){
    std::cerr << "unknown species: " << argv[1] << std::endl;
    usage(argv[0]);
  }
  emit_gmeans(s);
  return EXIT_SUCCESS;
}
