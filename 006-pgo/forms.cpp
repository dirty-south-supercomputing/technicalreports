#include "pgotypes.h"
#include <map>
#include <memory>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cstring>

int main(int argc, char **argv){
  setlocale(LC_ALL, "");
  if(argc != 2){
    fprintf(stderr, "usage: %s mega|dynamax|gigantamax\n", argv[0]);
    return EXIT_FAILURE;
  }
  bool zoom = false; // light card inset
  std::map<std::string, species> amap;
  if(strcasecmp(argv[1], "mega") == 0){
    zoom = true;
    for(unsigned u = 0 ; u < SPECIESCOUNT ; ++u){
      const auto *s = &sdex[u];
      for(const auto &m : s->mforms){
        species smeg{s, m};
        smeg.mforms.emplace_back(m);
        amap.emplace(m.name, smeg);
      }
    }
  }else if(strcasecmp(argv[1], "dynamax") == 0){
    for(unsigned u = 0 ; u < SPECIESCOUNT ; ++u){
      const auto &s = sdex[u];
      if(has_dmax(&s)){
        amap.emplace(s.name, s);
      }
    }
  }else if(strcasecmp(argv[1], "gigantamax") == 0){
    for(unsigned u = 0 ; u < SPECIESCOUNT ; ++u){
      const auto &s = sdex[u];
      if(has_gmax(&s)){
        amap.emplace(s.name, s);
      }
    }
  }else{
    fprintf(stderr, "usage: %s mega|dynamax|gigantamax\n", argv[0]);
    return EXIT_FAILURE;
  }
  for(const auto &s : amap){
    print_species_latex(&s.second, zoom, true, false);
  }
  return EXIT_SUCCESS;
}
