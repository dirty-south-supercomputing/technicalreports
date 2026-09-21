#include "pgotypes.h"

// generate list of all max-capable attackers with a 0.5s fast attack
int main(void){
  for(unsigned u = 0 ; u < SPECIESCOUNT ; ++u){
    const auto* s = &sdex[u];
    if(!has_dmax(s) && !has_gmax(s)){
      continue;
    }
    for(const auto* a : s->attacks){
      if(!fast_attack_p(a)){
        continue;
      }
      if(a->animdur > 1){
        continue;
      }
      std::cout << s->name << " (" << a->name << ")" << std::endl;
    }
  }
  return EXIT_SUCCESS;
}
