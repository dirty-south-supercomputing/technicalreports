#include "pgotypes.h"

// print a list of all forms with no shadow variant
int main(void){
  std::cout << "<table>" << std::endl;
  std::cout << "<tr><th>Dex #</th><th>Form</th></tr>" << std::endl;
  for(unsigned i = 0 ; i < SPECIESCOUNT ; ++i){
    const auto& s = sdex[i];
    if(!s.shadow){
      std::cout << "<tr><td>" << s.idx << "</td><td>" << s.name << "</td></tr>" << std::endl;
    }
  }
  std::cout << "</table>" << std::endl;
  return EXIT_SUCCESS;
}
