#include "pgotypes.cpp"
#include <vector>
#include <iostream>
#include <unordered_map>

// either a fast attack with all charged attacks it can be paired with (on some
// form or another), or a charged attack with all fast attacks yadda yadda.
class attackset {
 public:
  attackset(const attack *a) :
   A(a) {}

  // add if not already present
  void add(const attack *paired) {
    if(std::find(As.begin(), As.end(), paired) == As.end()){
      std::cout << "adding " << paired->name << std::endl;
      As.emplace_back(paired);
    }
  }

  const attack *A;
  std::vector<const attack *> As;
};

using pairmap = std::unordered_map<const char *, attackset>;

static void
get_fast_pairs(const species *s, const attack *fast, attackset &pairs){
  for(const attack **as = s->attacks ; *as ; ++as){
    const attack *a = *as;
    if(a->energytrain < 0){
      pairs.add(a);
    }
  }
}

static void
get_fast_attack_pairs(const species *s, pairmap &pairs){
  for(const attack **as = s->attacks ; *as ; ++as){
    const attack *a = *as;
    if(a->energytrain > 0){ // fast attack
      auto &p = *pairs.try_emplace(a->name, a).first;
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
get_charged_pairs(const species *s, const attack *charged, attackset &pairs){
  for(const attack **as = s->attacks ; *as ; ++as){
    const attack *a = *as;
    if(a->energytrain > 0){
      pairs.add(a);
    }
  }
}

static void
get_charged_attack_pairs(const species *s, pairmap &pairs){
  for(const attack **as = s->attacks ; *as ; ++as){
    const attack *a = *as;
    if(a->energytrain < 0){ // charged attack
      auto &p = *pairs.try_emplace(a->name, a).first;
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
  for(const auto &a : pairs){
    std::cout << a.second.A->name << ": " << a.second.As.size() << std::endl;
    for(const auto p : a.second.As){
      std::cout << " " << p->name << std::endl;
    }
  }
  return EXIT_SUCCESS;
}
