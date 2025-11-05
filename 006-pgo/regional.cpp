#include "pgotypes.h"
#include <iostream>
#include <cstdlib>

// print a table of region-specific pokémon
int main(void){
  for(unsigned idx = 0 ; idx < SPECIESCOUNT ; ++idx){
    const species& s = sdex[idx];
    const auto regions = s.regionstr();
    if(regions){
      std::cout << s.name << std::endl; // FIXME
    }
  }
  return EXIT_SUCCESS;
}
