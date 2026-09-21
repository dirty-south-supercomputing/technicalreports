#include "pgotypes.h"

// emit all Max-capable users of the given attack
void print_users(const attack* a){
  bool printed = false;
  for(unsigned u = 0 ; u < SPECIESCOUNT ; ++u){
    const auto* s = &sdex[u];
    if(s->name != "Zacian Crowned Sword"
        && s->name != "Zamazenta Crowned Shield"
        && s->name != "Eternatus"){
      if(!has_dmax(s) && !has_gmax(s)){
        continue;
      }
    }
    for(const auto* sa : s->attacks){
      if(sa->name != a->name){
        continue;
      }
      if(printed){
        std::cout << ", ";
      }else{
        std::cout << a->name << " (" << tname_capitalized(a->type) << "): ";
        printed = true;
      }
      std::cout << s->name;
    }
  }
  if(printed){
    std::cout << std::endl;
  }
}

// generate list of all max-capable attackers with a 0.5s fast attack
int main(void){
  for(auto a = attacks_begin() ; a != attacks_end() ; ++a){
    if(!fast_attack_p(*a)){
      continue;
    }
    if((*a)->animdur > 1){
      continue;
    }
    print_users(*a);
  }
  return EXIT_SUCCESS;
}
