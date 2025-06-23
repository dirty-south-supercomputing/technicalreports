#include "pgotypes.cpp"
#include <iostream>

static void
usage(const char *argv0){
  std::cerr << "usage: " << argv0 << " species" << std::endl;
  exit(EXIT_FAILURE);
}

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
    for(int ivd = 0 ; ivd < 16 ; ++ivd){
      for(int ivs = 0 ; ivs < 16 ; ++ivs){
        int cp;
        unsigned hl = maxlevel_cp_bounded(s->atk + iva, s->def + ivd, s->sta + ivs, cpceil, &cp);
        std::cout << hl << " ";
        //std::cout << iva << " " << ivd << " " << ivs << " " << hl << std::endl;
      }
      std::cout << std::endl;
    }
  }
  return EXIT_SUCCESS;
}
