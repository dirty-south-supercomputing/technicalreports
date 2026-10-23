#include "pgotypes.h"
#include <iostream>
#include <cstdlib>

// print a table of region-specific pokémon
int main(void){
  std::cout << "<table class=\"evenshade\" id=\"regional\">" << std::endl;
  std::cout << "<tr><th>Form</th><th>Region</th></tr>" << std::endl;
  for(auto s = species_begin() ; s != species_end() ; ++s){
    const auto regions = s->regionstr();
    if(regions){
      std::cout << "<tr><td>" << s->name << "</td><td>" << regions << "</td></tr>" << std::endl;
    }
  }
  std::cout << "</table>" << std::endl;
  return EXIT_SUCCESS;
}
