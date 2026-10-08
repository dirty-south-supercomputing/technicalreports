#include "html.h"

// print a list of all forms with no shiny variant
int main(void){
  std::cout << "<table>" << std::endl;
  std::cout << "<tr><th>Dex#</th><th>T</th><th>Form</th></tr>" << std::endl;
  for(auto s = species_begin() ; s != species_end() ; ++s){
    // hardcode the exemptions; these will never have a dmax form
    if(s->name.contains("Zacian")){
      continue;
    }
    if(s->name.contains("Zamazenta")){
      continue;
    }
    if(s->name.contains("Eternatus")){
      continue;
    }
    if(!s->dmax){
      std::cout << "<tr><td>" << s->idx << "</td>";
      std::cout << "<td>";
      html_types(s->t1, s->t2);
      std::cout << "</td>";
      std::cout << "<td>" << s->name << "</td></tr>" << std::endl;
    }
  }
  std::cout << "</table>" << std::endl;
  return EXIT_SUCCESS;
}
