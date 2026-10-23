#include "html.h"

static void
emit_megas(const species &s){
  for(const auto &m : s.mforms){
    std::cout << "<tr>";
    std::cout << "<td>";
    html_types(m.t1, m.t2);
    std::cout << "</td>";
    std::cout << "<td>";
    link_to_name(std::cout, m.name, true);
    std::cout << "</td>";
    std::cout << "<td>";
    if(m.plusatk){
      link_to_attack(std::cout, m.plusatk->name, true);
    }
    std::cout << "</td>";
    std::cout << "<td>" << m.initialcost << "</td>";
    std::cout << "</tr>" << std::endl;
  }
}

// dump a table of all released megas
int main(void){
  std::cout << "<table class=\"evenshade\" id=\"allmegas\">" << std::endl;
  std::cout << "<tr><th>T</th><th>Form</th><th>Plus attack</th><th>Initial cost</th></tr>" << std::endl;
  for(auto s = species_begin() ; s != species_end() ; ++s){
    emit_megas(*s);
  }
  std::cout << "</table>" << std::endl;
  return EXIT_SUCCESS;
}
