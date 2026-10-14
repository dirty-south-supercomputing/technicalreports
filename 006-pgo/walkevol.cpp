#include "html.h"

static void
emit_walk_evol(const species &s){
  if(s.evolkm){
    const auto prev = get_previous_evolution(s);
    std::cout << "<tr>";
    std::cout << "<td>" << prev->idx << "</td>";
    std::cout << "<td>";
    html_types(prev->t1, prev->t2);
    std::cout << "</td>";
    std::cout << "<td>";
    link_to_name(std::cout, prev->name, true);
    std::cout << "</td>";
    std::cout << "<td>" << s.evolkm << "</td>";
    std::cout << "<td>";
    link_to_name(std::cout, s.name, true);
    std::cout << "</td>";
    std::cout << "<td>";
    html_types(s.t1, s.t2);
    std::cout << "</td>";
    std::cout << "<td>" << s.idx << "</td>";
    std::cout << "</tr>" << std::endl;
  }
}

// generate a table of all evolutions requiring walking
int main(void){
  std::cout << "<table id=\"evolcond\">" << std::endl;
  std::cout << "<tr><th>Dex#</th><th>T</th><th>Base</th><th>km</th><th>Evolution</th><th>T</th><th>Dex#</th></tr>" << std::endl;
  for(auto s = species_begin() ; s != species_end() ; ++s){
    emit_walk_evol(*s);
  }
  std::cout << "</table>" << std::endl;
  return EXIT_SUCCESS;
}
