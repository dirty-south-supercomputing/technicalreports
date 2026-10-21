#include "html.h"

// list all species which lack a second charged attack to learn
int main(void){
  // track the previous match, since we don't print it immediately (we need
  // "and" prior to the last and only the last one).
  std::cout << "<table class=\"nofoo\">" << std::endl;
  std::cout << "<tr><th>T</th><th>Form</th></tr>" << std::endl;
  for(auto s = species_begin() ; s != species_end() ; ++s){
    unsigned ccount = 0;
    for(const auto &a : s->attacks){
      if(!charged_attack_p(*a)){
        continue;
      }
      if(++ccount > 1){
        break;
      }
    }
    if(ccount < 2){
      std::cout << "<tr><td>";
      html_types(s->t1, s->t2);
      std::cout << "</td><td>";
      link_to_name(std::cout, s->name, true);
      if(s->shadow){
        std::cout << " (*)";
      }
      // FIXME maybe also show the attack they can learn?
      std::cout << "</td></tr>" << std::endl;
    }
  }
  std::cout << "</table>" << std::endl;
  std::cout << "<br/>(*) has a Shadow/Purified form, which knows Frustration/Return" << std::endl;
  return EXIT_SUCCESS;
}
