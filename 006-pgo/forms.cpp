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
  if(argc != 2){
    fprintf(stderr, "usage: %s mega|fused|crowned|dynamax|gigantamax\n", argv[0]);
    return EXIT_FAILURE;
  }
  bool zoom = false; // light card inset
  std::map<std::string, species> amap;
  if(strcasecmp(argv[1], "mega") == 0){
    zoom = true;
    for(unsigned u = 0 ; u < MEGACOUNT ; ++u){
      const mega &m = megasdex[u];
      const species *s = lookup_species(m.idx);
      species sm{
          m.idx, m.name.c_str(), m.t1, m.t2, m.atk, m.def, m.sta,
          s->name.c_str(), s->attacks, s->shiny, false, false, s->elite,
          s->category, s->a2cost, nullptr
      };
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
