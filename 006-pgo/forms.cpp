#include "pgotypes.cpp"
#include <map>
#include <memory>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cstring>

int main(int argc, char **argv){
  if(argc != 2){
    fprintf(stderr, "usage: %s mega|fused|crowned|dynamax|gigantamax\n", argv[0]);
    return EXIT_FAILURE;
  }
  const species* dex;
  unsigned count;
  bool bg = false; // background
  if(strcasecmp(argv[1], "mega") == 0){
    dex = megasdex;
    count = MEGACOUNT;
    bg = true;
  }else if(strcasecmp(argv[1], "dynamax") == 0){
    dex = dynadex;
    count = DYNACOUNT;
  }else if(strcasecmp(argv[1], "gigantamax") == 0){
    dex = gigantasdex;
    count = GIGANTACOUNT;
  }else{
    fprintf(stderr, "usage: %s mega|dynamax|gigantamax\n", argv[0]);
    return EXIT_FAILURE;
  }
  std::map<std::string, const species *> amap;
  for(unsigned u = 0 ; u < count ; ++u){
    amap.emplace(dex[u].name, &dex[u]);
  }
  for(const auto &s : amap){
    print_species_latex(s.second, bg, false);
  }
  return EXIT_SUCCESS;
}
