#include "pgotypes.h"
#include <map>
#include <memory>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cstring>

static const char *megasortstr(const char *name){
#define MEGASTR "Mega "
#define PRIMALSTR "Primal "
  const char *str;
  if(strncmp(name, MEGASTR, strlen(MEGASTR)) == 0){
    str = name + strlen(MEGASTR);
  }else if(strncmp(name, PRIMALSTR, strlen(PRIMALSTR)) == 0){
    str = name + strlen(PRIMALSTR);
  }else{
    str = name;
  }
  return str;
#undef PRIMALSTR
#undef MEGASTR
}

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
    for(unsigned u = 0 ; u < MEGACOUNT ; ++u){
      const mega &m = megasdex[u];
      species sm{m};
      char *sstr = strdup(megasortstr(m.name.c_str()));
      amap.emplace(sstr, sm);
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
