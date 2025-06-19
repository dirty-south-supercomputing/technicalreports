#include "pgotypes.cpp"
#include <memory>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv){
  if(argc != 2){
    fprintf(stderr, "usage: %s mega|primal|crowned|dynamax|gigantamax\n", argv[0]);
    return EXIT_FAILURE;
  }
  const species* dex;
  unsigned count;
  if(strcasecmp(argv[1], "mega") == 0){
    dex = megasdex;
    count = MEGACOUNT;
  }else if(strcasecmp(argv[1], "primal") == 0){
    dex = primalsdex;
    count = PRIMALCOUNT;
  }else if(strcasecmp(argv[1], "crowned") == 0){
    dex = crownedsdex;
    count = CROWNEDCOUNT;
  }else if(strcasecmp(argv[1], "dynamax") == 0){
    dex = dynadex;
    count = DYNACOUNT;
  }else if(strcasecmp(argv[1], "gigantamax") == 0){
    dex = gigantasdex;
    count = GIGANTACOUNT;
  }else{
    fprintf(stderr, "usage: %s mega|primal|crowned|dynamax|gigantamax\n", argv[0]);
    return EXIT_FAILURE;
  }
  // for forms, we just want them alphabetical, not by type
  // for now we use dex entry FIXME
  for(unsigned u = 0 ; u < count ; ++u){
    print_species_latex(&dex[u], false);
  }
  return EXIT_SUCCESS;
}
