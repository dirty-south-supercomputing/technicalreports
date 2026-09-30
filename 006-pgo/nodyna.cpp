#include "pgotypes.h"

// print a list of all forms with no shiny variant
int main(void){
  std::cout << "<table>" << std::endl;
  std::cout << "<tr><th>Dex #</th><th>Form</th></tr>" << std::endl;
  for(unsigned i = 0 ; i < SPECIESCOUNT ; ++i){
    const auto& s = sdex[i];
    // hardcode the exemptions; these will never have a dmax form
    if(s.name.contains("Zacian")){
      continue;
    }
    if(s.name.contains("Zamazenta")){
      continue;
    }
    if(s.name.contains("Eternatus")){
      continue;
    }
    if(!s.dmax){
      std::cout << "<tr><td>" << s.idx << "</td><td>" << s.name << "</td></tr>" << std::endl;
    }
  }
  std::cout << "</table>" << std::endl;
  return EXIT_SUCCESS;
}
