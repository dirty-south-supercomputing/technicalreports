#include "pgotypes.h"

// generate list of all max-capable attackers with a 0.5s fast attack
int main(void){
  for(unsigned u = 0 ; u < SPECIESCOUNT ; ++u){
    const auto* s = &sdex[u];
    if(s->name != "Zacian Crowned Sword"
        && s->name != "Zamazenta Crowned Shield"
        && s->name != "Eternatus"){
      if(!has_dmax(s) && !has_gmax(s)){
        continue;
      }
    }
    bool printed = false;
    for(const auto* a : s->attacks){
      if(!fast_attack_p(a)){
        continue;
      }
      if(a->animdur > 1){
        continue;
      }
      if(printed){
        std::cout << ", " << a->name;
      }else{
        std::cout << s->name << " (" << a->name;
        printed = true;
      }
    }
    if(printed){
      std::cout << ")" << std::endl;
    }
  }
  return EXIT_SUCCESS;
}
