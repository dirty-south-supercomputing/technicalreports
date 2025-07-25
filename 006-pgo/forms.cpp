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
  bool bg = false; // background
  std::map<std::string, const species *> amap;
  if(strcasecmp(argv[1], "mega") == 0){
    bg = true;
    for(unsigned u = 0 ; u < MEGACOUNT ; ++u){
      amap.emplace(megasdex[u].name, &megasdex[u]);
    }
  }else if(strcasecmp(argv[1], "dynamax") == 0){
    for(unsigned u = 0 ; u < SPECIESCOUNT ; ++u){
      const auto &s = sdex[u];
      if(has_dmax(&s)){
        amap.emplace(s.name, &s);
      }
    }
  }else if(strcasecmp(argv[1], "gigantamax") == 0){
    for(unsigned u = 0 ; u < SPECIESCOUNT ; ++u){
      const auto &s = sdex[u];
      if(has_gmax(&s)){
        amap.emplace(s.name, &s);
      }
    }
  }else{
    fprintf(stderr, "usage: %s mega|dynamax|gigantamax\n", argv[0]);
    return EXIT_FAILURE;
  }
  for(const auto &s : amap){
    print_species_latex(s.second, bg, false);
  }
  return EXIT_SUCCESS;
}
