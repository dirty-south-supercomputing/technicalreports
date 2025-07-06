#include "pgotypes.cpp"
#include <vector>
#include <iostream>
#include <unordered_map>

// either a fast attack with all charged attacks it can be paired with (on some
// form or another), or a charged attack with all fast attacks yadda yadda.
class attackpair {
  attackpair(const attack *a) :
   A(a) {}

  // add if not already present
  void add(const attack *paired) {
    if(std::find(As.begin(), As.end(), paired) == As.end()){
      As.emplace_back(paired);
    }
  }

  const attack *A;
  std::vector<const attack *> As;
};

using pairmap = std::unordered_map<const char *, std::vector<attackpair>>;

static void
get_fast_pairs(const species *s, const attack *fast, std::vector<attackpair> &pairs){
  for(const attack **as = s->attacks ; *as ; ++as){
    const attack *a = *as;
    if(a->energytrain < 0){
      std::cout << fast->name << ":" << a->name << std::endl;
    }
  }
}

static void
get_fast_attack_pairs(const species *s, pairmap &pairs){
  for(const attack **as = s->attacks ; *as ; ++as){
    const attack *a = *as;
    if(a->energytrain > 0){ // fast attack
      std::vector<attackpair> ap;
      auto &p = *pairs.try_emplace(a->name, ap).first;
      get_fast_pairs(s, a, p.second);
    }
  }
}

static void
get_fast_attack_pairs_dex(const spokedex &sd, pairmap &pairs){
  for(unsigned i = 0 ; i < sd.dcount ; ++i){
    const species *s = &sd.dex[i];
    get_fast_attack_pairs(s, pairs);
  }
}

static void
get_charged_pairs(const species *s, const attack *charged, std::vector<attackpair> &pairs){
  for(const attack **as = s->attacks ; *as ; ++as){
    const attack *a = *as;
    if(a->energytrain > 0){
      std::cout << charged->name << ":" << a->name << std::endl;
    }
  }
}

static void
get_charged_attack_pairs(const species *s, pairmap &pairs){
  for(const attack **as = s->attacks ; *as ; ++as){
    const attack *a = *as;
    if(a->energytrain < 0){ // charged attack
      std::vector<attackpair> ap;
      auto &p = *pairs.try_emplace(a->name, ap).first;
      get_charged_pairs(s, a, p.second);
    }
  }
}

static void
get_charged_attack_pairs_dex(const spokedex &sd, pairmap &pairs){
  for(unsigned i = 0 ; i < sd.dcount ; ++i){
    const species *s = &sd.dex[i];
    get_charged_attack_pairs(s, pairs);
  }
}

static void usage(const char *argv0){
  std::cerr << "usage: " << argv0 << " fast | charged" << std::endl;
  exit(EXIT_FAILURE);
}

int main(int argc, const char **argv){
  pairmap pairs;
  if(argc != 2){
    usage(argv[0]);
  }
  if(!strcmp(argv[1], "fast")){
    for(const auto &sdex : sdexen){
      get_fast_attack_pairs_dex(sdex, pairs);
    }
  }else if(!strcmp(argv[1], "charged")){
    for(const auto &sdex : sdexen){
      get_charged_attack_pairs_dex(sdex, pairs);
    }
  }else{
    usage(argv[0]);
  }
  return EXIT_SUCCESS;
}
