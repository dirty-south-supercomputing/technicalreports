#include "pgotypes.h"

// print a list of all forms with no shadow variant
int main(void){
  for(unsigned i = 0 ; i < SPECIESCOUNT ; ++i){
    const auto& s = sdex[i];
    if(!s.shadow){
      std::cout << s.name << std::endl;
    }
  }
  return EXIT_SUCCESS;
}
