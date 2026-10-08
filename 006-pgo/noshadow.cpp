#include "html.h"

// print a list of all forms with no shadow variant
int main(void){
  std::cout << "<table class=\"nofoo\">" << std::endl;
  std::cout << "<tr><th>Dex#</th><th>T</th><th>Form</th></tr>" << std::endl;
  for(auto s = species_begin() ; s != species_end() ; ++s){
    if(!s->shadow){
      std::cout << "<tr><td>" << s->idx << "</td>";
      std::cout << "<td>";
      html_types(s->t1, s->t2);
      std::cout << "</td>";
      std::cout << "<td>";
      link_to_name(std::cout, s->name, true);
      std::cout << "</td></tr>" << std::endl;
    }
  }
  std::cout << "</table>" << std::endl;
  return EXIT_SUCCESS;
}
