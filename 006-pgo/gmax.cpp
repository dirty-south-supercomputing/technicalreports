#include "html.h"

// table of released GMax
int main(){
  std::cout << "<table>";
  std::cout << "<tr>";
  std::cout << "<th>T</th><th>Pokémon</th><th>G-Max attack</th>";
  std::cout << "</tr>" << std::endl;
  for(auto s = species_begin() ; s != species_end() ; ++s){
    const auto *gm = lookup_gmax_attack(*s);
    if(!gm){
      continue;
    }
    std::cout << "<tr>";
    std::cout << "<td>";
    html_types(s->t1, s->t2);
    std::cout << "</td><td>";
    std::cout << s->name;
    std::cout << "</td><td>";
    html_type(gm->type);
    std::cout << ' ' << gm->name;
    std::cout << "</td>";
    std::cout << "</tr>" << std::endl;
  }
  std::cout << "</table>" << std::endl;
  return EXIT_SUCCESS;
}
