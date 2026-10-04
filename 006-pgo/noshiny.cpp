#include "pgotypes.h"

// print a list of all forms with no shiny variant
int main(void){
  std::cout << "<table>" << std::endl;
  std::cout << "<tr><th>Dex#</th><th>T</th><th>Form</th></tr>" << std::endl;
  for(unsigned i = 0 ; i < SPECIESCOUNT ; ++i){
    const auto& s = sdex[i];
    if(!s.shiny){
      std::cout << "<tr><td>" << s.idx << "</td>";
      std::cout << "<td>";
      html_types(s.t1, s.t2);
      std::cout << "</td>";
      std::cout << "<td>" << s.name << "</td></tr>" << std::endl;
    }
  }
  std::cout << "</table>" << std::endl;
  return EXIT_SUCCESS;
}
