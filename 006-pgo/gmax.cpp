#include "html.h"

// table of released GMax
int main(){
  std::cout << "<table id=\"gmaxen\">";
  std::cout << "<tr>";
  std::cout << "<th/><th>Pokémon</th><th>G-Max attack</th>";
  std::cout << "</tr>" << std::endl;
  for(auto s = species_begin() ; s != species_end() ; ++s){
    const auto *gm = lookup_gmax_attack(*s);
    if(!gm){
      continue;
    }
    species gs{&*s, *gm};
    std::cout << "<tr>";
    std::cout << "<td>";
    html_types(gs.t1, gs.t2, false);
    std::cout << "</td><td>";
    link_to_name(std::cout, gs.name, true, false);
    std::cout << "</td><td>";
    html_type(gm->type, false);
    std::cout << ' ';
    link_to_attack(std::cout, gm->name, true, false);
    std::cout << "</td>";
    std::cout << "</tr>" << std::endl;
  }
  std::cout << "</table>" << std::endl;
  return EXIT_SUCCESS;
}
