#include "html.h"

static void
emit_shadow(std::ostream &fp, const species &s){
  fp << "<tr><td>";
  html_types(fp, s.t1, s.t2, false);
  fp << "</td>";
  fp << "<td>";
  link_to_name(fp, s.name, true);
  fp << "</td>";
  fp << "<td>" << purification_cost(s) << "</td>";
  fp << "</tr>" << std::endl;
}

// print a list of all forms with a shadow, cost to purify,
// and change in arithmetic mean for #1 in each league
int main(void){
  std::cout << "<table class=\"evenshade shadows\">" << std::endl;
  std::cout << "<tr><th>T</th><th>Form</th><th>Purification cost (kSD)</th></tr>" << std::endl;
  for(auto s = species_begin() ; s != species_end() ; ++s){
    if(s->shadow){
      emit_shadow(std::cout, *s);
    }
  }
  std::cout << "</table>" << std::endl;
  return EXIT_SUCCESS;
}
