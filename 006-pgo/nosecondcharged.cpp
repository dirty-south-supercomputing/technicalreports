#include "pgotypes.h"

// list all species which lack a second charged attack to learn
int main(void){
  // track the previous match, since we don't print it immediately (we need
  // "and" prior to the last and only the last one).
  const species *prevs = nullptr;
  for(unsigned i = 0 ; i < SPECIESCOUNT ; ++i){
    const auto &s = sdex[i];
    unsigned ccount = 0;
    for(const auto &a : s.attacks){
      if(!charged_attack_p(a)){
        continue;
      }
      if(++ccount > 1){
        break;
      }
    }
    if(ccount < 2){
      if(prevs){
        std::cout << prevs->name << ", ";
      }
      prevs = &s;
    }
  }
  if(!prevs){
    std::cerr << "didn't have any matches!" << std::endl;
    throw std::exception();
  }
  std::cout << "and " << prevs->name << " are exceptions," << std::endl;
  return EXIT_SUCCESS;
}
