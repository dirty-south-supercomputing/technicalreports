#include "html.h"

static void
emit_special_evol(const species &s){
  if(s.evolkm || s.evolitem != species::EVOL_NOITEM){
    auto prev = get_previous_evolution(s);
    if(!prev){
      std::forward_list<species> store;
      std::vector<const species*> evols;
      if(!get_evolutions(s, evols, store)){
        return;
      }
      if(evols.size() != 1){
        std::cerr << "only expected one evolution for " << s.name << std::endl;
        throw std::exception();
      }
      prev = evols[0];
      if(!bidirectional_evol_p(*prev)){
        std::cerr << "expected bidirectional for " << s.name << std::endl;
        throw std::exception();
      }
    }
    std::cout << "<tr>";
    std::cout << "<td>";
    html_types(prev->t1, prev->t2);
    std::cout << "</td>";
    std::cout << "<td>";
    link_to_name(std::cout, prev->name, true);
    std::cout << "</td>";
    std::cout << "<td>";
    if(s.evolkm){
      std::cout << "Walk " << s.evolkm << " km<br/>";
    }
    const char *evolstr = s.evolitemstr();
    if(strcmp(evolstr, "")){
      std::cout << evolstr << "<br/>";
    }
    std::cout << "</td>";
    std::cout << "<td>";
    link_to_name(std::cout, s.name, true);
    std::cout << "</td>";
    std::cout << "<td>";
    html_types(s.t1, s.t2);
    std::cout << "</td>";
    std::cout << "</tr>" << std::endl;
  }
}

// generate a table of all evolutions requiring special resources/conditions
int main(void){
  std::cout << "<table class=\"evenshade\" id=\"evolcond\">" << std::endl;
  std::cout << "<tr><th>T</th><th>Base</th><th>Requirement(s)</th><th>Evolution</th><th>T</th></tr>" << std::endl;
  for(auto s = species_begin() ; s != species_end() ; ++s){
    emit_special_evol(*s);
  }
  std::cout << "</table>" << std::endl;
  return EXIT_SUCCESS;
}
