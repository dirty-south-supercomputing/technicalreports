#include "pgotypes.h"
#include <iostream>
#include <cstdlib>

// print a table of region-specific pokémon
int main(void){
  for(auto s = species_begin() ; s != species_end() ; ++s){
    const auto regions = s->regionstr();
    if(regions){
      std::cout << s->name << std::endl; // FIXME
    }
  }
  return EXIT_SUCCESS;
}
