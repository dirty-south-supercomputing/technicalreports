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
    fprintf(stderr, "usage: %s mega\n", argv[0]);
    return EXIT_FAILURE;
  }
  bool zoom = false; // light card inset
  std::map<std::string, species> amap;
  if(strcasecmp(argv[1], "mega") == 0){
    zoom = true;
    for(auto s = species_begin() ; s != species_end() ; ++s){
      for(const auto &m : s->mforms){
        species smeg{&*s, m};
        smeg.mforms.emplace_back(m);
        amap.emplace(m.name, smeg);
      }
    }
  }else{
    fprintf(stderr, "usage: %s mega\n", argv[0]);
    return EXIT_FAILURE;
  }
  for(const auto &s : amap){
    print_species_latex(s.second, zoom, true, false);
  }
  return EXIT_SUCCESS;
}
