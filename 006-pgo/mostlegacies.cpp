#include "pgotypes.h"
#include <algorithm>

struct legacyform {
  const species* s;
  unsigned legacycount;

  legacyform(const species* S, unsigned Legacycount) :
   s(S),
   legacycount(Legacycount)
   { }

  bool operator<(const legacyform& r){
    if(legacycount < r.legacycount){
      return true;
    }else if(legacycount > r.legacycount){
      return false;
    }else if(s->name < r.s->name){
      return true;
    }
    return false;
  }

};

unsigned legacy_move_count(const species* s){
  unsigned count = 0;
  for(const auto& a : s->attacks){
    if(exclusive_attack_p(s, a)){
      ++count;
    }
  }
  return count;
}

// print all forms with at least one legacy move, and the number of legacy moves
int main(void){
  std::vector<legacyform> lforms;
  for(unsigned i = 0 ; i < SPECIESCOUNT ; ++i){
    const species* s = &sdex[i];
    auto count = legacy_move_count(s);
    if(count){
      lforms.emplace_back(s, count);
    }
  }
  std::sort(lforms.begin(), lforms.end());
  for(const auto& lf : lforms){
    std::cout << lf.s->name << " " << lf.legacycount << std::endl;
  }
  return EXIT_SUCCESS;
}
