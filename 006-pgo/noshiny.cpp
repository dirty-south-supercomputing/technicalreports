#include "pgotypes.h"

// print a list of all forms with no shiny variant
int main(void){
  for(unsigned i = 0 ; i < SPECIESCOUNT ; ++i){
    const auto& s = sdex[i];
    if(!s.shiny){
      std::cout << s.name << std::endl;
    }
  }
  return EXIT_SUCCESS;
}
