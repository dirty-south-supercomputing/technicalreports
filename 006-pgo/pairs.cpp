#include "pgotypes.cpp"
#include <vector>
#include <iostream>

// get all viable pairs of fast attack + charged attack
// (i.e., someone can learn both)
// and calculate those pairs' properties
class attackpair {
  attackpair() {
  }
};

static void
get_attack_pairs(const species *s, std::vector<attackpair> &pairs){
  // FIXME
}

static void
get_attack_pairs_dex(const spokedex &sd, std::vector<attackpair> &pairs){
  for(unsigned i = 0 ; i < sd.dcount ; ++i){
    const species *s = &sd.dex[i];
    get_attack_pairs(s, pairs);
  }
}

int main(void){
  std::vector<attackpair> pairs;
  for(const auto &sdex : sdexen){
    get_attack_pairs_dex(sdex, pairs);
  }
  // FIXME
  return EXIT_SUCCESS;
}
