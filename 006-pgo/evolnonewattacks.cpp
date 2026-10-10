#include "pgotypes.h"

// list forms which don't gain new attacks when evolving
int main(void){
  for(auto s = species_begin() ; s != species_end() ; ++s){
    if(s->from.empty()){
      continue;
    }
    const species *pre = lookup_species(s->from.c_str());
    if(!pre){
      continue;
    }
    // for every attack in the evol, check that it is present in the pre
    bool equal = true;
    for(const attack *sa : s->attacks){
      bool found = false;
      for(const attack *a : pre->attacks){
        if(a == sa){
          found = true;
          break;
        }
      }
      if(!found){
        equal = false;
        break;
      }
    }
    if(equal){
      std::cout << pre->name << " -> " << s->name << std::endl;
    }
  }
  return EXIT_SUCCESS;
}
